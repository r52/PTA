#include "itemapi.h"
#include "pitem.h"
#include "pta_types.h"
#include "statdialog.h"

#include <regex>
#include <sstream>
#include <string>

#include <QDebug>
#include <QDesktopServices>
#include <QEventLoop>
#include <QFileInfo>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QSettings>

// PoE trade api only allows 10 items at once
constexpr size_t papi_query_limit = 10;

// API URLs

// official
const QUrl u_api_league("https://www.pathofexile.com/api/trade/data/leagues");
const QUrl u_api_mods("https://www.pathofexile.com/api/trade/data/stats");
const QUrl u_api_items("https://www.pathofexile.com/api/trade/data/items");

// repoe
const QUrl u_repoe_base("https://raw.githubusercontent.com/brather1ng/RePoE/master/RePoE/data/base_items.min.json");

// pta
const QUrl u_pta_armourlocals("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/armour_locals.json");
const QUrl u_pta_basecat("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/base_categories.json");
const QUrl u_pta_currency("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/currency.json");
const QUrl u_pta_disc("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/discriminators.json");
const QUrl u_pta_enchantrules("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/enchant_rules.json");
const QUrl u_pta_pseudorules("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/pseudo_rules.json");
const QUrl u_pta_weaponlocals("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/weapon_locals.json");

// trade site
const QString u_trade_fetch("https://www.pathofexile.com/api/trade/fetch/%1?query=%2");
const QString u_trade_search("https://www.pathofexile.com/api/trade/search/");
const QString u_trade_exchange("https://www.pathofexile.com/api/trade/exchange/");
const QString u_trade_site("https://www.pathofexile.com/trade/search/");

// poe prices
const QString u_poeprices("https://www.poeprices.info/api?l=%1&i=%2");

ItemAPI::ItemAPI(QObject* parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    if (nullptr == m_manager)
    {
        throw std::runtime_error("Failed to create network manager");
    }

    json data;

    // Download leagues
    if (!synchronizedGetJSON(QNetworkRequest(u_api_league), data))
    {
        throw std::runtime_error("Failed to download league data");
    }

    auto& lgs = data["result"];

    m_leagues = json::array();
    for (size_t i = 0; i < lgs.size(); i++)
    {
        m_leagues.push_back(lgs[i]["id"].get<std::string>());
    }

    QString setlg = getLeague();

    qInfo() << "League data loaded. Setting league to" << setlg;

    // Download stats
    if (!synchronizedGetJSON(QNetworkRequest(u_api_mods), data))
    {
        throw std::runtime_error("Failed to download mod data");
    }

    auto& stt = data["result"];

    for (auto& type : stt)
    {
        auto& el = type["entries"];

        for (auto& et : el)
        {
            // Cut the key for multiline mods
            std::string::size_type nl;
            std::string            text = et["text"].get<std::string>();
            if ((nl = text.find("\n")) != std::string::npos)
            {
                text = text.substr(0, nl);
            }

            m_stats_by_text.insert({{text, et}});
            m_stats_by_id.insert({{et["id"].get<std::string>(), et}});
        }
    }

    qInfo() << "Mod data loaded";

    // Download unique items
    if (!synchronizedGetJSON(QNetworkRequest(u_api_items), data))
    {
        throw std::runtime_error("Failed to download unique item data");
    }

    auto& itm = data["result"];

    for (auto& type : itm)
    {
        auto& el = type["entries"];

        for (auto& et : el)
        {
            if (et.contains("name"))
            {
                m_uniques.insert({{et["name"].get<std::string>(), et}});
            }
            else if (et.contains("type"))
            {
                m_uniques.insert({{et["type"].get<std::string>(), et}});
            }
            else
            {
                qDebug() << "Item entry has neither name nor type:" << QString::fromStdString(et.dump());
            }
        }
    }

    qInfo() << "Unique item data loaded";

    // Load base categories
    QFile bc("data/base_categories.json");

    if (bc.open(QIODevice::ReadOnly))
    {
        QByteArray bdat = bc.readAll();

        c_baseCat = json::parse(bdat.toStdString());
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_basecat), data))
    {
        c_baseCat = data;
    }
    else
    {
        throw std::runtime_error("Cannot open base_categories.json");
    }

    qInfo() << "Base categories loaded";

    // Load RePoE base data (needs to be loaded AFTER c_baseCat)
    if (!synchronizedGetJSON(QNetworkRequest(u_repoe_base), data))
    {
        throw std::runtime_error("Failed to download base item data");
    }

    for (auto& [k, o] : data.items())
    {
        std::string typeName  = o["name"].get<std::string>();
        std::string itemClass = o["item_class"].get<std::string>();
        size_t      implicits = o["implicits"].size();

        auto search = c_baseCat.find(itemClass);
        if (search != c_baseCat.end())
        {
            std::string itemCat = search.value().get<std::string>();

            json cat;

            cat["category"]  = itemCat;
            cat["implicits"] = implicits;

            c_baseMap.insert({{typeName, cat}});
        }
    }

    qInfo() << "Item base data loaded";

    // Load pseudo rules
    QFile pr("data/pseudo_rules.json");

    if (pr.open(QIODevice::ReadOnly))
    {
        QByteArray pdat = pr.readAll();

        c_pseudoRules = json::parse(pdat.toStdString());
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_pseudorules), data))
    {
        c_pseudoRules = data;
    }
    else
    {
        throw std::runtime_error("Cannot open pseudo_rules.json");
    }

    qInfo() << "Pseudo rules loaded";

    // Load enchant rules
    QFile er("data/enchant_rules.json");

    if (er.open(QIODevice::ReadOnly))
    {
        QByteArray edat = er.readAll();

        c_enchantRules = json::parse(edat.toStdString());
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_enchantrules), data))
    {
        c_enchantRules = data;
    }
    else
    {
        throw std::runtime_error("Cannot open enchant_rules.json");
    }

    qInfo() << "Enchant rules loaded";

    // Load local rules
    QFile wl("data/weapon_locals.json");

    if (wl.open(QIODevice::ReadOnly))
    {
        QByteArray wdat = wl.readAll();

        json wlr = json::parse(wdat.toStdString());

        for (auto& e : wlr["data"])
        {
            c_weaponLocals.insert(e.get<std::string>());
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_weaponlocals), data))
    {
        for (auto& e : data["data"])
        {
            c_weaponLocals.insert(e.get<std::string>());
        }
    }
    else
    {
        throw std::runtime_error("Cannot open weapon_locals.json");
    }

    qInfo() << "Weapon Local rules loaded";

    //  armour
    QFile al("data/armour_locals.json");

    if (al.open(QIODevice::ReadOnly))
    {
        QByteArray adat = al.readAll();

        json alr = json::parse(adat.toStdString());

        for (auto& e : alr["data"])
        {
            c_armourLocals.insert(e.get<std::string>());
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_armourlocals), data))
    {
        for (auto& e : data["data"])
        {
            c_armourLocals.insert(e.get<std::string>());
        }
    }
    else
    {
        throw std::runtime_error("Cannot open armour_locals.json");
    }

    qInfo() << "Armour Local rules loaded";

    // Discriminators
    QFile disc("data/discriminators.json");

    if (disc.open(QIODevice::ReadOnly))
    {
        QByteArray ddat = disc.readAll();

        json dcr = json::parse(ddat.toStdString());

        for (auto [entry, list] : dcr.items())
        {
            for (auto value : list["unused"])
            {
                c_discriminators[entry].insert(value.get<std::string>());
            }
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_disc), data))
    {
        for (auto [entry, list] : data.items())
        {
            for (auto value : list["unused"])
            {
                c_discriminators[entry].insert(value.get<std::string>());
            }
        }
    }
    else

    {
        throw std::runtime_error("Cannot open discriminators.json");
    }

    qInfo() << "Discriminator rules loaded";

    // Currency
    QFile curr("data/currency.json");

    if (curr.open(QIODevice::ReadOnly))
    {
        QByteArray cdat = curr.readAll();

        c_currencyMap = json::parse(cdat.toStdString());
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_currency), data))
    {
        c_currencyMap = data;
    }
    else
    {
        throw std::runtime_error("Cannot open currency.json");
    }

    for (auto& [k, v] : c_currencyMap.items())
    {
        c_currencyCodes.insert(v.get<std::string>());
    }

    qInfo() << "Currency rules loaded";
}

int ItemAPI::readPropInt(QString prop)
{
    // Remove augmented tag
    prop.replace(" (augmented)", "");

    QRegularExpression      re("^([\\+\\-]?[\\d\\.]+)%?");
    QRegularExpressionMatch match = re.match(prop);

    if (match.hasMatch())
    {
        QString val = match.captured(1);
        return val.toInt();
    }

    return 0;
}

mmv_t ItemAPI::readPropIntRange(QString prop)
{
    mmv_t val = {0, 0};

    // If it is a list, process list
    if (prop.contains(", "))
    {
        // Split and recursively read
        auto list = prop.split(", ", QString::SkipEmptyParts);

        for (auto& item : list)
        {
            val += readPropIntRange(item);
        }

        return val;
    }

    // Otherwise process single entry

    // Remove augmented tag
    prop.replace(" (augmented)", "");

    QRegularExpression      re("^(\\d+)-(\\d+)");
    QRegularExpressionMatch match = re.match(prop);

    if (match.hasMatch())
    {
        QString v1 = match.captured(1);
        QString v2 = match.captured(2);

        val.min = v1.toInt();
        val.max = v2.toInt();
    }

    return val;
}

double ItemAPI::readPropFloat(QString prop)
{
    // Remove augmented tag
    prop.replace(" (augmented)", "");

    QRegularExpression      re("^([\\+\\-]?[\\d\\.]+)%?");
    QRegularExpressionMatch match = re.match(prop);

    if (match.hasMatch())
    {
        QString val = match.captured(1);
        return val.toDouble();
    }

    return 0.0;
}

socket_filters_t ItemAPI::readSockets(QString prop)
{
    socket_filters_t ss = {0, {0, 0, 0, 0, 0}};

    auto llist = prop.split(" ", QString::SkipEmptyParts);

    for (auto& lpart : llist)
    {
        auto socks = lpart.split("-", QString::SkipEmptyParts);

        if (socks.length() > 1 && socks.length() > ss.links)
        {
            // New max links
            ss.links = socks.length();
        }

        for (auto& s : socks)
        {
            if ("R" == s)
            {
                ss.sockets.r++;
            }
            else if ("G" == s)
            {
                ss.sockets.g++;
            }
            else if ("B" == s)
            {
                ss.sockets.b++;
            }
            else if ("W" == s)
            {
                ss.sockets.w++;
            }
            else if ("A" == s)
            {
                ss.sockets.a++;
            }
        }
    }

    return ss;
}

int ItemAPI::readPropExp(QString prop)
{
    // TODO: read exp
    return 0;
}

std::string ItemAPI::readName(QString name)
{
    name.remove(QRegularExpression("<<.*?>>|<.*?>"));

    return name.toStdString();
}

std::string ItemAPI::readType(PItem* item, QString type)
{
    type.remove(QRegularExpression("<<.*?>>|<.*?>"));
    type.remove("Superior ");

    if (type.startsWith("Synthesised "))
    {
        type.remove("Synthesised ");
        item->f_misc.synthesised_item = true;
    }

    if (item->f_type.rarity == "Magic")
    {
        // Parse out magic affixes
        QRegularExpression              re("([\\w']+)(?(?= of)( of [\\w\\s]+))");
        QRegularExpressionMatchIterator i = re.globalMatch(type);

        QStringList words;
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString                 word  = match.captured(1);
            words << word;
        }

        if (words.length() >= 3)
        {
            words.removeAt(0);
        }

        type = words.join(' ');
    }

    return type.toStdString();
}

void ItemAPI::captureNumerics(QString line, QRegularExpression& re, json& val, std::vector<QString>& captured)
{
    QRegularExpressionMatchIterator it = re.globalMatch(line);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString                 word  = match.captured(1);

        captured.push_back(word);

        // Process floats
        if (word.contains('.'))
        {
            double numval = word.toDouble();
            val.push_back(numval);
        }
        else
        {
            int numval = word.toInt();
            val.push_back(numval);
        }
    }
}

void ItemAPI::parseProp(PItem* item, QString prop)
{
    QString p = prop.section(":", 0, 0);
    QString v = prop.section(": ", 1, 1);

    if (!c_propMap.contains(p))
    {
        qDebug() << "Unknown/unimplemented prop:" << p;
        return;
    }

    auto& pev = c_propMap[p];

    switch (pev[0])
    {
        case weapon_filter:
        {
            switch (pev[1])
            {
                case weapon_filter_pdps:
                {
                    item->f_weapon.pdps = readPropIntRange(v);
                    break;
                }

                case weapon_filter_crit:
                {
                    item->f_weapon.crit = readPropFloat(v);
                    break;
                }

                case weapon_filter_aps:
                {
                    item->f_weapon.aps = readPropFloat(v);
                    break;
                }

                case weapon_filter_edps:
                {
                    item->f_weapon.edps = readPropIntRange(v);
                    break;
                }
            }

            item->is_weapon = true;

            break;
        }

        case armour_filter:
        {
            switch (pev[1])
            {
                case armour_filter_ar:
                {
                    item->f_armour.ar = readPropInt(v);
                    break;
                }

                case armour_filter_ev:
                {
                    item->f_armour.ev = readPropInt(v);
                    break;
                }

                case armour_filter_es:
                {
                    item->f_armour.es = readPropInt(v);
                    break;
                }

                case armour_filter_block:
                {
                    item->f_armour.block = readPropInt(v);
                    break;
                }
            }

            item->is_armour = true;

            break;
        }

        case socket_filter:
        {
            item->f_socket = readSockets(v);
            break;
        }

        case req_filter:
        {
            switch (pev[1])
            {
                case req_filter_lvl:
                {
                    item->f_req.lvl = readPropInt(v);
                    break;
                }

                case req_filter_str:
                {
                    item->f_req.str = readPropInt(v);
                    break;
                }

                case req_filter_dex:
                {
                    item->f_req.dex = readPropInt(v);
                    break;
                }

                case req_filter_int:
                {
                    item->f_req.intl = readPropInt(v);
                    break;
                }
            }

            break;
        }

        case misc_filter:
        {
            switch (pev[1])
            {
                case misc_filter_quality:
                {
                    item->f_misc.quality = readPropInt(v);
                    break;
                }

                case misc_filter_gem_level:
                {
                    item->f_misc.gem_level = readPropInt(v);
                    break;
                }

                case misc_filter_ilvl:
                {
                    item->f_misc.ilvl = readPropInt(v);
                    break;
                }

                case misc_filter_gem_level_progress:
                {
                    item->f_misc.gem_level_progress = readPropExp(v);
                    break;
                }

                case misc_filter_map_tier:
                {
                    item->f_misc.map_tier = readPropInt(v);
                    break;
                }
            }

            break;
        }

        case special_filter:
        {
            if (p == "Requirements")
            {
                m_section = "Requirements";
                break;
            }

            if (p == "Level")
            {
                QString fprop = "gem_level: ";

                if (m_section == "Requirements")
                {
                    fprop = "req_level: ";
                }

                QString cprop = fprop + v;
                parseProp(item, cprop);
                break;
            }

            qDebug() << "Unknown/unimplemented special prop:" << p;

            break;
        }
    }
}

bool ItemAPI::parseStat(PItem* item, QString stat, QTextStream& stream)
{
    QString orig_stat = stat;

    // Special rule for A Master Seeks Help
    if (item->f_type.category == "prophecy" && item->name == "A Master Seeks Help")
    {
        QRegularExpression      re("^You will find (\\w+) and complete her mission.$");
        QRegularExpressionMatch match = re.match(stat);
        if (match.hasMatch())
        {
            QString master = match.captured(1);

            item->f_misc.disc = master.toLower().toStdString();
        }

        return true;
    }

    if (stat == "Unidentified")
    {
        item->f_misc.identified = false;
        return true;
    }

    if (stat == "Shaper Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[shaper]);
        return true;
    }

    if (stat == "Elder Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[elder]);
        return true;
    }

    // XXX (subject to change): 3.9 Influences
    if (stat == "Crusader Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[crusader]);
        return true;
    }

    if (stat == "Redeemer Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[redeemer]);

        return true;
    }

    if (stat == "Hunter Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[hunter]);

        return true;
    }

    if (stat == "Warlord Item")
    {
        item->f_misc.influences.push_back(c_influenceMap[warlord]);

        return true;
    }

    if (stat == "Corrupted")
    {
        item->f_misc.corrupted = true;
        return true;
    }

    if (stat == "Synthesised Item")
    {
        // Should already have been processed
        item->f_misc.synthesised_item = true;
        return true;
    }

    // Vaal gems
    if (item->f_type.category == "gem" && stat.startsWith("Vaal "))
    {
        item->type = stat.toStdString();
        return true;
    }

    // PoE 3.9 adds the "(implicit)" description so we no longer have to guess
    std::string stat_type;

    if (stat.endsWith("(crafted)"))
    {
        stat_type = "crafted";
    }

    if (stat.endsWith("(implicit)"))
    {
        stat_type = "implicit";
    }

    stat.replace(" (crafted)", "");
    stat.replace(" (implicit)", "");

    // Match numerics
    QRegularExpression   re("([\\+\\-]?[\\d\\.]+)");
    std::vector<QString> captured;

    json val    = json::array();
    auto stoken = stat.toStdString();

    // First try original line
    bool found = m_stats_by_text.contains(stoken);

    if (!found)
    {
        // Then, try replacing the num stats
        captureNumerics(stat, re, val, captured);

        // Craft search token
        stat.replace(re, "#");

        stoken = stat.toStdString();
        found  = m_stats_by_text.contains(stoken);
    }

    // Process local rules
    if (item->is_weapon || item->is_armour)
    {
        bool is_local_stat = ((item->is_weapon && c_weaponLocals.contains(stoken)) || (item->is_armour && c_armourLocals.contains(stoken)));

        if (is_local_stat)
        {
            stat += " (Local)";

            stoken = stat.toStdString();
            found  = m_stats_by_text.contains(stoken);
        }
    }

    if (!found && val.size() && (stat.contains("reduced") || stat.contains("less")))
    {
        // If the stat line has a "reduced" value, try to
        // flip it and try again

        if (stat.contains("reduced"))
        {
            stat.replace("reduced", "increased");
            // orig_stat.replace("reduced", "increased");
        }
        else
        {
            stat.replace("less", "more");
            // orig_stat.replace("less", "more");
        }

        // replace last
        auto& v = val[val.size() - 1];

        if (v.is_number_float())
        {
            v = v.get<double>() * -1.0;
        }
        else
        {
            v = v.get<int>() * -1;
        }

        stoken = stat.toStdString();
        found  = m_stats_by_text.contains(stoken);
    }

    // Handle enchant rules
    if (c_enchantRules.contains(stoken))
    {
        auto& rule = c_enchantRules[stoken];

        if (rule.contains("id"))
        {
            found = true;

            stoken = m_stats_by_id[rule["id"].get<std::string>()]["text"].get<std::string>();
            stat   = QString::fromStdString(stoken);
        }

        if (rule.contains("value"))
        {
            val.push_back(rule["value"]);
        }
    }

    if (!found)
    {
        // Try forward replace search
        QString frep     = orig_stat;
        QString frepplus = frep;

        while (!found && frep.contains(re) && captured.size())
        {
            // Try putting back some values in case the mod itself has hardcoded values
            frep.replace(frep.indexOf(re), captured[0].length(), "#");
            frepplus.replace(frepplus.indexOf(re), captured[0].length(), "+#");

            stoken = frep.toStdString();
            found  = m_stats_by_text.contains(stoken);

            if (!found)
            {
                // Try plus version
                stoken = frepplus.toStdString();
                found  = m_stats_by_text.contains(stoken);
            }

            if (found)
            {
                // Delete value used
                captured.pop_back();
                val.erase(val.size() - 1);
            }
        }
    }

    if (!found)
    {
        // Reverse replace search
        QString rrep     = orig_stat;
        QString rrepplus = rrep;
        while (!found && rrep.contains(re) && captured.size())
        {
            // Try putting back some values in case the mod itself has hardcoded values
            rrep.replace(rrep.lastIndexOf(re), captured[captured.size() - 1].length(), "#");
            rrepplus.replace(rrepplus.lastIndexOf(re), captured[captured.size() - 1].length(), "+#");

            stoken = rrep.toStdString();
            found  = m_stats_by_text.contains(stoken);

            if (!found)
            {
                // Try plus version
                stoken = rrepplus.toStdString();
                found  = m_stats_by_text.contains(stoken);
            }

            if (found)
            {
                // Delete value used
                captured.erase(captured.begin());
                val.erase(0);
            }
        }
    }

    // Give up
    if (!found)
    {
        qDebug() << "Ignored/unprocessed line" << orig_stat;
        return false;
    }

    std::vector<QString> multiline;
    json                 filter;

    auto range = m_stats_by_text.equal_range(stoken);
    for (auto it = range.first; it != range.second; ++it)
    {
        auto& entry = it->second;

        std::string text = entry["text"];

        std::stringstream ss(text);
        std::string       tl;

        std::vector<QString> lines;

        while (std::getline(ss, tl, '\n'))
        {
            lines.push_back(QString::fromStdString(tl));
        }

        // Bump the first line since we know it has matched
        lines.erase(lines.begin());

        if (lines.size() > 0)
        {
            // If this is a multiline mod
            // Match the other lines as well
            bool matches = true;

            // Read in the other lines if we haven't yet
            while (multiline.size() < lines.size())
            {
                QString nextline;

                stream.readLineInto(&nextline);
                multiline.push_back(nextline);
            }

            assert(multiline.size() == lines.size());

            json                 lvals = json::array();
            std::vector<QString> lcap;

            for (size_t i = 0; i < multiline.size(); i++)
            {
                if (multiline[i] != lines[i])
                {
                    // Try capturing values
                    QString statline = multiline[i];
                    captureNumerics(statline, re, lvals, lcap);

                    statline.replace(re, "#");

                    if (statline != lines[i])
                    {
                        // Try the plus version
                        statline = multiline[i];
                        statline.replace(re, "+#");

                        if (statline != lines[i])
                        {
                            matches = false;
                            break;
                        }
                    }
                }
            }

            if (!matches)
            {
                // Doesn't match, continue
                continue;
            }

            // Need to merge captured numerics
            for (size_t i = 0; i < lvals.size(); i++)
            {
                val.push_back(lvals.at(i));
            }

            captured.insert(captured.end(), lcap.begin(), lcap.end());
        }

        if (!stat_type.empty())
        {
            if (entry["type"] != stat_type)
            {
                // skip this entry
                continue;
            }

            // use crafted stat
            filter["id"]    = entry["id"];
            filter["type"]  = entry["type"];
            filter["text"]  = entry["text"];
            filter["value"] = val;
            break;
        }
        else
        {
            if (entry["type"] == "pseudo")
            {
                // skip pseudos
                continue;
            }

            std::string id = entry["id"].get<std::string>();

            if (c_discriminators.contains(id) && c_discriminators[id].contains(item->f_type.category))
            {
                // Discriminator skip
                continue;
            }

            if (entry["type"] == "explicit")
            {
                filter["id"]    = entry["id"];
                filter["type"]  = entry["type"];
                filter["text"]  = entry["text"];
                filter["value"] = val;
            }

            // Peek next line
            QString peek;

            int pos = stream.pos();

            if (pos != -1)
            {
                peek = stream.read(3);
                stream.seek(pos);
            }

            if (item->filters.size() < 2 && peek == "---")
            {
                // First stat with a section break, try to look for an enchant
                if (entry["type"] == "enchant")
                {
                    filter["id"]    = entry["id"];
                    filter["type"]  = entry["type"];
                    filter["text"]  = entry["text"];
                    filter["value"] = val;
                }
            }
        }
    }

    if (filter.empty())
    {
        qDebug() << "Error parsing stat line" << orig_stat;
        return false;
    }

    std::string fid = filter["id"].get<std::string>();

    // If the item already has this filter, merge them
    if (item->filters.contains(fid))
    {
        auto& efil = item->filters[fid];

        auto count = efil["value"].size();

        for (size_t i = 0; i < count; i++)
        {
            if (efil["value"][i].is_number_float())
            {
                efil["value"][i] = efil["value"][i].get<double>() + filter["value"][i].get<double>();
            }
            else
            {
                efil["value"][i] = efil["value"][i].get<int>() + filter["value"][i].get<int>();
            }
        }
    }
    else
    {
        item->filters.emplace(fid, filter);
    }

    return true;
}

void ItemAPI::processPriceResults(std::shared_ptr<PItem> item, json results, bool isCurrency)
{
    bool   done  = false;
    size_t start = 0, end = 0;
    auto   flist = results["result"];
    auto   total = flist.size();
    json   endRes;

    std::unordered_set<std::string> accounts;

    QSettings settings;
    size_t    display_limit = settings.value(PTA_CONFIG_DISPLAYLIMIT, PTA_CONFIG_DEFAULT_DISPLAYLIMIT).toInt();
    bool      removedupes   = settings.value(PTA_CONFIG_REMOVE_DUPES, PTA_CONFIG_DEFAULT_REMOVE_DUPES).toBool();

    while (!done)
    {
        QStringList fetchcodes;

        if (total > papi_query_limit && total > end)
        {
            end += std::min(papi_query_limit, total - end);
        }
        else
        {
            end = total;
        }

        for (; start < end; start++)
        {
            fetchcodes.append(QString::fromStdString(flist.at(start)));
        }

        QString fcode = fetchcodes.join(',');

        QString fetchurl = u_trade_fetch.arg(fcode).arg(QString::fromStdString(results["id"]));

        if (isCurrency)
        {
            fetchurl += "&exchange";
        }

        QUrl resUrl = fetchurl;

        json dat;

        if (!synchronizedGetJSON(QNetworkRequest(resUrl), dat))
        {
            emit humour(tr("Error retrieving search results. See log for details"));
            return;
        }

        auto& rj = dat["result"];

        // Delete duplicate accounts
        if (removedupes)
        {
            for (json::iterator it = rj.begin(); it != rj.end();)
            {
                auto& entry = *it;

                std::string accname = entry["listing"]["account"]["name"].get<std::string>();

                if (accounts.contains(accname))
                {
                    // skip duplicate accounts
                    it = rj.erase(it);
                }
                else
                {
                    accounts.insert(accname);
                    ++it;
                }
            }
        }

        if (endRes.empty())
        {
            endRes = dat;
        }
        else
        {
            size_t limit = std::min(dat["result"].size(), display_limit - endRes["result"].size());

            for (size_t i = 0; i < limit; i++)
            {
                endRes["result"].push_back(dat["result"].at(i));
            }
        }

        done = (endRes["result"].size() == display_limit || total == end);
    }

    emit priceCheckFinished(item, QString::fromStdString(endRes.dump()));
}

void ItemAPI::doCurrencySearch(std::shared_ptr<PItem> item)
{
    QSettings settings;

    auto query = R"(
    {
        "exchange": {
            "status": {
                "option": "online"
            },
            "have": [],
            "want": []
        }
    }
    )"_json;

    std::string p_curr = settings.value(PTA_CONFIG_PRIMARY_CURRENCY, PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY).toString().toStdString();
    std::string s_curr = settings.value(PTA_CONFIG_SECONDARY_CURRENCY, PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY).toString().toStdString();

    // Reset setting that no longer exists
    if (!c_currencyCodes.contains(p_curr))
    {
        p_curr = PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY;
        settings.setValue(PTA_CONFIG_PRIMARY_CURRENCY, QString::fromStdString(p_curr));
    }

    if (!c_currencyCodes.contains(s_curr))
    {
        s_curr = PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY;
        settings.setValue(PTA_CONFIG_SECONDARY_CURRENCY, QString::fromStdString(s_curr));
    }

    // Check for existing currencies
    if (!c_currencyMap.contains(item->type))
    {
        emit humour(tr("Could not find this currency in the database. See log for details."));
        qWarning() << "Currency not found:" << QString::fromStdString(item->type);
        qWarning() << "If you believe that this is a mistake, please file a bug report on GitHub.";
        return;
    }

    // currency exchange format
    item->f_misc.exchange = true;

    std::string want = c_currencyMap[item->type].get<std::string>();
    std::string have = p_curr;

    if (want == p_curr)
    {
        have = s_curr;
    }

    query["exchange"]["want"].push_back(want);

    while (true)
    {
        item->m_options = "To " + have;

        query["exchange"]["have"].clear();
        query["exchange"]["have"].push_back(have);

        auto qba = query.dump();

        QNetworkRequest request;
        request.setUrl(QUrl(u_trade_exchange + getLeague()));
        request.setRawHeader("Content-Type", "application/json");

        QEventLoop loop;
        connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

        auto req = m_manager->post(request, QByteArray::fromStdString(qba));
        loop.exec();

        req->deleteLater();

        if (req->error() != QNetworkReply::NoError)
        {
            emit humour(tr("Error querying currency exchange API. See log for details"));
            qWarning() << "PAPI: Error querying currency exchange API" << req->error() << req->errorString();
            return;
        }

        auto respdata = req->readAll();

        if (!respdata.size())
        {
            emit humour(tr("Error querying currency exchange API. See log for details"));
            qWarning() << "PAPI: Error querying currency exchange API - returned no data";
            return;
        }

        auto resp = json::parse(respdata.toStdString());
        if (!resp.contains("result") || !resp.contains("id"))
        {
            emit humour(tr("Error querying currency exchange API. See log for details"));
            qWarning() << "PAPI: Error querying trade site";
            qWarning() << "PAPI: Site responded with" << respdata;
            return;
        }

        if (resp["result"].size() == 0)
        {
            // try secondary currency
            if (have != s_curr)
            {
                have = s_curr;
                continue;
            }

            emit humour(tr("No results found."));
            qDebug() << "No results";
            return;
        }

        // else process the results
        processPriceResults(item, resp, true);
        return;
    }
}

bool ItemAPI::synchronizedGetJSON(const QNetworkRequest& req, json& result)
{
    QEventLoop loop;
    connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    auto reply = m_manager->get(req);
    loop.exec();

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        QString msg = "PAPI: Error retrieving" + reply->url().toString() + "-" + reply->error() + reply->errorString();
        qWarning() << msg;
        return false;
    }

    auto rdat = reply->readAll();

    if (!rdat.size())
    {
        qWarning() << "PAPI: Error retrieving" << reply->url() << "-"
                   << "returned no data.";
        return false;
    }

    std::string sdat = rdat.toStdString();
    result           = json::parse(sdat);

    return true;
}

QString ItemAPI::getLeague()
{
    QSettings settings;
    int       league = settings.value(PTA_CONFIG_LEAGUE, PTA_CONFIG_DEFAULT_LEAGUE).toInt();

    if (league > m_leagues.size())
    {
        QString defleag = QString::fromStdString(m_leagues[PTA_CONFIG_DEFAULT_LEAGUE].get<std::string>());

        qWarning() << "Previously set league no longer available. Resetting to default league" << defleag;

        league = PTA_CONFIG_DEFAULT_LEAGUE;
        settings.setValue(PTA_CONFIG_LEAGUE, PTA_CONFIG_DEFAULT_LEAGUE);
    }

    return QString::fromStdString(m_leagues[league].get<std::string>());
}

PItem* ItemAPI::parse(QString itemText)
{
    QTextStream stream(&itemText, QIODevice::ReadOnly);
    QString     line;

    // Check first line for PoE item
    stream.readLineInto(&line);

    if (!line.startsWith("Rarity:"))
    {
        qWarning() << "Parse called on non PoE item text";
        return nullptr;
    }

    PItem* item = new PItem();

    // Full original text
    item->m_itemtext = itemText.toStdString();

    // Rarity
    item->f_type.rarity = line.section(": ", 1, 1).toStdString();

    // Read name/type
    QString nametype, type;
    stream.readLineInto(&nametype);
    stream.readLineInto(&type);

    if (type.startsWith("---"))
    {
        // nametype has to be item type and not name
        item->type = readType(item, nametype);
        item->m_sections++;
    }
    else
    {
        item->name = readName(nametype);
        item->type = readType(item, type);
    }

    // Process category
    if ("Gem" == item->f_type.rarity)
    {
        item->f_type.category = "gem";
    }
    else if ("Divination Card" == item->f_type.rarity)
    {
        item->f_type.category = item->f_type.rarity = "card";
    }

    if (item->type.ends_with("Map"))
    {
        item->f_type.category = "map";
        item->f_misc.disc     = m_mapdisc; // Default map discriminator

        item->type = std::regex_replace(item->type, std::regex("Elder "), "");
        item->type = std::regex_replace(item->type, std::regex("Shaped "), "");
    }

    if (item->f_type.category.empty() && m_uniques.contains(item->type))
    {
        auto search = m_uniques.find(item->type);
        if (search != m_uniques.end())
        {
            auto& je = search->second;
            if (je["type"] == "Prophecy")
            {
                // this is a prophecy
                item->name            = item->type;
                item->type            = "Prophecy";
                item->f_type.category = "prophecy";
            }
        }
    }

    if (item->f_type.category.empty())
    {
        auto base = c_baseMap.find(item->type);
        if (base != c_baseMap.end())
        {
            json cat              = base->second;
            item->f_type.category = cat["category"];
        }
    }

    // Read the rest of the crap

    while (stream.readLineInto(&line))
    {
        // Skip
        if (line.startsWith("---"))
        {
            m_section.clear();
            item->m_sections++;
            continue;
        }

        if (line.contains(":"))
        {
            // parse item prop
            parseProp(item, line);
        }
        else if (item->m_sections > 1)
        {
            // parse item stat
            parseStat(item, line, stream);
        }
    }

    // Process special/pseudo rules
    if (item->filters.size())
    {
        for (auto [key, fil] : item->filters.items())
        {
            if (c_pseudoRules.contains(key))
            {
                auto& rules = c_pseudoRules[key];

                for (auto& r : rules)
                {
                    std::string pid = r["id"].get<std::string>();

                    auto pentry = m_stats_by_id[pid];

                    if (!item->pseudos.contains(pid))
                    {
                        json ps_entry = pentry;

                        ps_entry["value"] = json::array();

                        for (auto v : fil["value"])
                        {
                            if (v.is_number_float())
                            {
                                ps_entry["value"].push_back(v.get<double>() * r["factor"].get<double>());
                            }
                            else
                            {
                                ps_entry["value"].push_back((int) (v.get<int>() * r["factor"].get<double>()));
                            }
                        }

                        item->pseudos.emplace(pid, ps_entry);
                    }
                    else
                    {
                        auto& ps_entry = item->pseudos[pid];

                        for (size_t i = 0; i < fil["value"].size(); i++)
                        {
                            auto v = fil["value"][i];

                            // XXX: only support one operation right now
                            // also remove is useless
                            if (r["op"] == "add")
                            {
                                if (v.is_number_float())
                                {
                                    ps_entry["value"][i] = ps_entry["value"][i].get<double>() + (v.get<double>() * r["factor"].get<double>());
                                }
                                else
                                {
                                    ps_entry["value"][i] = ps_entry["value"][i].get<int>() + ((int) (v.get<int>() * r["factor"].get<double>()));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return item;
}

QString ItemAPI::toJson(PItem* item)
{
    json j;

    j["prediction"] = item->is_prediction;

    j["exchange"] = item->f_misc.exchange;

    j["name"] = item->name;

    j["rarity"] = item->f_type.rarity;

    // process category
    if (!item->f_type.category.empty())
    {
        j["category"] = item->f_type.category;
    }

    if (item->name != item->type)
    {
        j["type"] = item->type;
    }

    j["sockets"] = item->f_socket.sockets.total();
    j["links"]   = item->f_socket.links;

    if (item->f_type.category == "gem")
    {
        j["gem_level"] = item->f_misc.gem_level;
    }

    j["identified"] = item->f_misc.identified;

    // This stuff not relevant to cards
    if (item->f_type.category != "card")
    {
        j["ilvl"]    = item->f_misc.ilvl;
        j["quality"] = item->f_misc.quality;

        if (!item->f_misc.influences.empty())
        {
            j["influences"] = json::array();

            for (auto i : item->f_misc.influences)
            {
                std::string inf = i;
                inf[0]          = toupper(inf[0]);

                j["influences"].push_back(inf);
            }
        }

        j["corrupted"] = item->f_misc.corrupted;
    }

    if (!item->m_options.empty())
    {
        j["options"] = item->m_options;
    }

    return QString::fromStdString(j.dump());
}

void ItemAPI::simplePriceCheck(std::shared_ptr<PItem> item)
{
    // If its a currency and the currency is listed in the bulk exchange, try that first
    // Otherwise, try a regular search
    if (item->f_type.category == "currency" && c_currencyMap.contains(item->type))
    {
        doCurrencySearch(item);
        return;
    }

    QSettings settings;

    auto query = R"(
    {
        "query": {
            "status": {
                "option": "online"
            },
            "stats": [{
                "type": "and",
                "filters": []
            }]
        },
        "sort": {
            "price": "asc"
        }
    }
    )"_json;

    bool        is_unique_base = false;
    std::string searchToken;

    // Take care of settings
    bool onlineonly = settings.value(PTA_CONFIG_ONLINE_ONLY, PTA_CONFIG_DEFAULT_ONLINE_ONLY).toBool();
    if (!onlineonly)
    {
        query["query"]["status"]["option"] = "any";
    }

    bool buyoutonly = settings.value(PTA_CONFIG_BUYOUT_ONLY, PTA_CONFIG_DEFAULT_BUYOUT_ONLY).toBool();
    if (buyoutonly)
    {
        query["query"]["filters"]["trade_filters"]["filters"]["sale_type"]["option"] = "priced";
    }

    // Search by type if rare map, or if it has no name
    if ((item->f_type.category == "map" && item->f_type.rarity == "Rare") || item->name.empty())
    {
        is_unique_base = m_uniques.contains(item->type);
        searchToken    = item->type;
    }
    else
    {
        is_unique_base = m_uniques.contains(item->name);
        searchToken    = item->name;
    }

    // Force rarity if unique
    if (item->f_type.rarity == "Unique")
    {
        std::string rarity = item->f_type.rarity;
        std::transform(rarity.begin(), rarity.end(), rarity.begin(), ::tolower);

        query["query"]["filters"]["type_filters"]["filters"]["rarity"]["option"] = rarity;
    }

    // Force category
    if (!item->f_type.category.empty())
    {
        std::string category = item->f_type.category;
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);

        query["query"]["filters"]["type_filters"]["filters"]["category"]["option"] = category;
    }

    // Check for unique items
    if (is_unique_base)
    {
        auto& qe = query["query"];

        auto range = m_uniques.equal_range(searchToken);
        for (auto it = range.first; it != range.second; ++it)
        {
            auto& entry = it->second;

            // If has discriminator, match discriminator and type
            if (!item->f_misc.disc.empty())
            {
                if (entry["disc"] == item->f_misc.disc && entry["type"] == item->type)
                {
                    if (entry.contains("name"))
                    {
                        qe["name"] = {{"discriminator", entry["disc"]}, {"option", entry["name"]}};
                    }

                    qe["type"] = {{"discriminator", entry["disc"]}, {"option", entry["type"]}};

                    break;
                }
            }
            else if (entry["type"] == item->type)
            {
                // For everything else, just match type
                qe["type"] = entry["type"];

                if (entry.contains("name"))
                {
                    qe["name"] = entry["name"];
                }

                break;
            }
        }

        item->m_options = getLeague().toStdString();

        // Default Gem options
        if (item->f_type.category == "gem")
        {
            qe["filters"]["misc_filters"]["filters"]["gem_level"]["min"] = item->f_misc.gem_level;
            qe["filters"]["misc_filters"]["filters"]["quality"]["min"]   = item->f_misc.quality;

            item->m_options += ", Lv" + std::to_string(item->f_misc.gem_level) + "/" + std::to_string(item->f_misc.quality) + "%";
        }

        // Default socket options
        if (item->f_socket.sockets.total() == 6)
        {
            qe["filters"]["socket_filters"]["filters"]["sockets"]["min"] = item->f_socket.sockets.total();

            item->m_options += ", " + std::to_string(item->f_socket.sockets.total()) + "S";
        }

        // Default link options
        if (item->f_socket.links > 4)
        {
            qe["filters"]["socket_filters"]["filters"]["links"]["min"] = item->f_socket.links;

            item->m_options += ", " + std::to_string(item->f_socket.links) + "L";
        }

        // Force iLvl
        if (item->f_type.rarity != "Unique" && item->f_type.category != "card" && item->f_misc.ilvl)
        {
            qe["filters"]["misc_filters"]["filters"]["ilvl"]["min"] = item->f_misc.ilvl;

            item->m_options += ", iLvl=" + std::to_string(item->f_misc.ilvl);
        }

        // Force map tier
        if (item->f_type.category == "map" && item->f_misc.map_tier)
        {
            qe["filters"]["map_filters"]["filters"]["map_tier"]["min"] = item->f_misc.map_tier;

            item->m_options += ", Map Tier=" + std::to_string(item->f_misc.map_tier);
        }

        // Note discriminator
        if (!item->f_misc.disc.empty())
        {
            item->m_options += ", Disc=" + item->f_misc.disc;
        }

        // Force Influences
        if (item->f_type.category != "card" && !item->f_misc.influences.empty())
        {
            for (auto i : item->f_misc.influences)
            {
                std::string inftype = i + "_item";

                qe["filters"]["misc_filters"]["filters"][inftype]["option"] = true;

                i[0] = toupper(i[0]);
                item->m_options += ", " + i + " Influence";
            }
        }

        // Force Synthesis
        if (item->f_misc.synthesised_item)
        {
            qe["filters"]["misc_filters"]["filters"]["synthesised_item"]["option"] = true;
            item->m_options += ", Synthesis Base";
        }

        // Default corrupt options
        bool corrupt_override = settings.value(PTA_CONFIG_CORRUPTOVERRIDE, PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE).toBool();

        // No such thing as corrupted cards
        if (item->f_type.category != "card")
        {
            if (corrupt_override)
            {
                QString corrupt_search = settings.value(PTA_CONFIG_CORRUPTSEARCH, PTA_CONFIG_DEFAULT_CORRUPTSEARCH).toString();

                if (corrupt_search != "Any")
                {
                    qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = (corrupt_search == "Yes");

                    item->m_options += ", Corrupted=" + corrupt_search.toStdString();
                }
                else
                {
                    item->m_options += ", Corrupted=Any";
                }

                item->m_options += " (override)";
            }
            else
            {
                qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = item->f_misc.corrupted;

                item->m_options += ", Corrupted=";
                item->m_options += item->f_misc.corrupted ? "Yes" : "No";
            }
        }

        item->m_options += ", Mods ignored";

        auto qba = query.dump();

        qDebug() << QString::fromStdString(qba);

        QNetworkRequest request;
        request.setUrl(QUrl(u_trade_search + getLeague()));
        request.setRawHeader("Content-Type", "application/json");

        auto req = m_manager->post(request, QByteArray::fromStdString(qba));
        connect(req, &QNetworkReply::finished, [=]() {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API" << req->error() << req->errorString();
                return;
            }

            auto respdata = req->readAll();

            if (!respdata.size())
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API - returned no data";
                return;
            }

            auto resp = json::parse(respdata.toStdString());
            if (!resp.contains("result") || !resp.contains("id"))
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            if (resp["result"].size() == 0)
            {
                emit humour(tr("No results found."));
                qDebug() << "No results";
                return;
            }

            // else process the results
            processPriceResults(item, resp);
        });
    }
    else
    {
        // poeprices.info

        item->is_prediction = true;

        QString itemText = QString::fromStdString(item->m_itemtext);

        itemText.remove(QRegularExpression("<<.*?>>|<.*?>"));

        // Remove notes
        QStringList lines = itemText.trimmed().split('\n');

        if (lines[lines.size() - 1].startsWith("Note:"))
        {
            lines.removeLast();
            lines.removeLast();
        }

        itemText = lines.join('\n');

        QByteArray itemData = itemText.toUtf8().toBase64(QByteArray::Base64UrlEncoding);

        QNetworkRequest request;

        QString qurl = u_poeprices.arg(getLeague()).arg(QString::fromUtf8(itemData));

        request.setUrl(QUrl(qurl));

        auto req = m_manager->get(request);
        connect(req, &QNetworkReply::finished, [=]() {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                emit humour(tr("Error querying poeprices.info. See log for details"));
                qWarning() << "PAPI: Error querying poeprices.info" << req->error() << req->errorString();
                return;
            }

            auto respdata = req->readAll();

            if (!respdata.size())
            {
                emit humour(tr("Error querying poeprices.info. See log for details"));
                qWarning() << "PAPI: Error querying poeprices.info - returned no data";
                return;
            }

            auto resp = json::parse(respdata.toStdString());
            if (resp["error"].get<int>() != 0)
            {
                emit humour(tr("Error querying poeprices.info. See log for details"));
                qWarning() << "PAPI: Error querying poeprices.info";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            if (!resp.contains("min") || !resp.contains("max"))
            {
                emit humour(tr("No prediction data available from poeprices.info for this item."));
                qInfo() << "PAPI: No prediction data available from poeprices.info for this item.";
                return;
            }

            // else process the results
            emit priceCheckFinished(item, QString::fromStdString(resp.dump()));
        });
    }
}

void ItemAPI::advancedPriceCheck(std::shared_ptr<PItem> item)
{
    if (item->filters.empty() || item->f_type.category == "map")
    {
        // Cannot advanced search items with no filters
        emit humour(tr("Advanced search is unavailable for this item type"));
        return;
    }

    if (!item->f_misc.identified)
    {
        emit humour(tr("Advanced search is unavailable for unidentified items"));
        return;
    }

    StatDialog* dlg = new StatDialog(item.get());

    connect(dlg, &QDialog::finished, [=](int result) {
        if (result == QDialog::Rejected)
        {
            // cancelled
            return;
        }

        bool searchonsite = (result == SEARCH_ON_SITE);

        json filters = dlg->filters;
        json misc    = dlg->misc;

        auto query = R"(
    {
        "query": {
            "status": {
                "option": "online"
            },
            "stats": [{
                "type": "and",
                "filters": []
            }]
        },
        "sort": {
            "price": "asc"
        }
    }
    )"_json;

        QSettings settings;
        auto&     qe = query["query"];

        // Take care of settings
        bool onlineonly = settings.value(PTA_CONFIG_ONLINE_ONLY, PTA_CONFIG_DEFAULT_ONLINE_ONLY).toBool();
        if (!onlineonly)
        {
            query["query"]["status"]["option"] = "any";
        }

        bool buyoutonly = settings.value(PTA_CONFIG_BUYOUT_ONLY, PTA_CONFIG_DEFAULT_BUYOUT_ONLY).toBool();
        if (buyoutonly)
        {
            query["query"]["filters"]["trade_filters"]["filters"]["sale_type"]["option"] = "priced";
        }

        bool        is_unique_base = false;
        std::string searchToken;

        if (!item->name.empty())
        {
            is_unique_base = m_uniques.contains(item->name);
            searchToken    = item->name;
        }
        else
        {
            is_unique_base = m_uniques.contains(item->type);
            searchToken    = item->type;
        }

        // Force rarity
        std::string rarity = "nonunique";

        if (item->f_type.rarity == "Unique")
        {
            rarity = item->f_type.rarity;
            std::transform(rarity.begin(), rarity.end(), rarity.begin(), ::tolower);
        }

        query["query"]["filters"]["type_filters"]["filters"]["rarity"]["option"] = rarity;

        // Force category
        if (!item->f_type.category.empty())
        {
            std::string category = item->f_type.category;
            std::transform(category.begin(), category.end(), category.begin(), ::tolower);

            query["query"]["filters"]["type_filters"]["filters"]["category"]["option"] = category;
        }

        // weapon/armour base mods

        if (item->f_weapon.search_pdps)
        {
            if (item->f_weapon.pdps_filter.min > 0)
            {
                query["query"]["filters"]["weapon_filters"]["filters"]["pdps"]["min"] = item->f_weapon.pdps_filter.min;
            }

            if (item->f_weapon.pdps_filter.max > 0)
            {
                query["query"]["filters"]["weapon_filters"]["filters"]["pdps"]["max"] = item->f_weapon.pdps_filter.max;
            }
        }

        if (item->f_weapon.search_edps)
        {
            if (item->f_weapon.edps_filter.min > 0)
            {
                query["query"]["filters"]["weapon_filters"]["filters"]["edps"]["min"] = item->f_weapon.edps_filter.min;
            }

            if (item->f_weapon.edps_filter.max > 0)
            {
                query["query"]["filters"]["weapon_filters"]["filters"]["edps"]["max"] = item->f_weapon.edps_filter.max;
            }
        }

        if (item->f_armour.search_ar)
        {
            if (item->f_armour.ar_filter.min > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["ar"]["min"] = item->f_armour.ar_filter.min;
            }

            if (item->f_armour.ar_filter.max > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["ar"]["max"] = item->f_armour.ar_filter.max;
            }
        }

        if (item->f_armour.search_ev)
        {
            if (item->f_armour.ev_filter.min > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["ev"]["min"] = item->f_armour.ev_filter.min;
            }

            if (item->f_armour.ev_filter.max > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["ev"]["max"] = item->f_armour.ev_filter.max;
            }
        }

        if (item->f_armour.search_es)
        {
            if (item->f_armour.es_filter.min > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["es"]["min"] = item->f_armour.es_filter.min;
            }

            if (item->f_armour.es_filter.max > 0)
            {
                query["query"]["filters"]["armour_filters"]["filters"]["es"]["max"] = item->f_armour.es_filter.max;
            }
        }

        // Checked mods
        for (auto& [k, e] : filters.items())
        {
            // set id
            e["id"] = k;

            if (e["disabled"] == false)
            {
                qe["stats"][0]["filters"].push_back(e);
            }
        }

        // Check for unique items
        if (is_unique_base)
        {
            auto range = m_uniques.equal_range(searchToken);
            for (auto it = range.first; it != range.second; ++it)
            {
                auto& entry = it->second;

                // For everything else, match type
                if (entry["type"] == item->type)
                {
                    qe["type"] = entry["type"];

                    if (entry.contains("name"))
                    {
                        qe["name"] = entry["name"];
                    }

                    break;
                }
            }
        }

        item->m_options = getLeague().toStdString();

        // Use sockets
        if (misc.contains("use_sockets") && misc["use_sockets"])
        {
            qe["filters"]["socket_filters"]["filters"]["sockets"]["min"] = item->f_socket.sockets.total();

            item->m_options += ", " + std::to_string(item->f_socket.sockets.total()) + "S";
        }

        // Use links
        if (misc.contains("use_links") && misc["use_links"])
        {
            qe["filters"]["socket_filters"]["filters"]["links"]["min"] = item->f_socket.links;

            item->m_options += ", " + std::to_string(item->f_socket.links) + "L";
        }

        // Use iLvl
        if (misc.contains("use_ilvl") && misc["use_ilvl"])
        {
            qe["filters"]["misc_filters"]["filters"]["ilvl"]["min"] = misc["ilvl"];

            item->m_options += ", iLvl=" + std::to_string(misc["ilvl"].get<int>());
        }

        // Use item base
        if (misc.contains("use_item_base") && misc["use_item_base"])
        {
            qe["type"] = item->type;

            item->m_options += ", Use Base Type";
        }

        // Influences

        if (misc.contains("influences"))
        {
            for (auto [key, value] : misc["influences"].items())
            {
                if (!key.empty() && value.get<bool>())
                {
                    std::string inf    = key;
                    std::string infkey = inf + "_item";

                    qe["filters"]["misc_filters"]["filters"][infkey]["option"] = true;

                    inf[0] = toupper(inf[0]);
                    item->m_options += ", " + inf + " Influence";
                }
            }
        }

        // Synthesis
        if (misc.contains("use_synthesis_base") && misc["use_synthesis_base"])
        {
            qe["filters"]["misc_filters"]["filters"]["synthesised_item"]["option"] = true;
            item->m_options += ", Synthesis Base";
        }

        // Default corrupt options
        bool corrupt_override = settings.value(PTA_CONFIG_CORRUPTOVERRIDE, PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE).toBool();

        if (corrupt_override)
        {
            QString corrupt_search = settings.value(PTA_CONFIG_CORRUPTSEARCH, PTA_CONFIG_DEFAULT_CORRUPTSEARCH).toString();

            if (corrupt_search != "Any")
            {
                qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = (corrupt_search == "Yes");

                item->m_options += ", Corrupted=" + corrupt_search.toStdString();
            }
            else
            {
                item->m_options += ", Corrupted=Any";
            }
        }
        else
        {
            qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = item->f_misc.corrupted;

            item->m_options += ", Corrupted=";
            item->m_options += item->f_misc.corrupted ? "Yes" : "No";
        }

        auto qba = query.dump();

        QNetworkRequest request;
        request.setUrl(QUrl(u_trade_search + getLeague()));
        request.setRawHeader("Content-Type", "application/json");

        auto req = m_manager->post(request, QByteArray::fromStdString(qba));
        connect(req, &QNetworkReply::finished, [=]() {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API" << req->error() << req->errorString();
                return;
            }

            auto respdata = req->readAll();

            if (!respdata.size())
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API - returned no data";
                return;
            }

            auto resp = json::parse(respdata.toStdString());
            if (!resp.contains("result") || !resp.contains("id"))
            {
                emit humour(tr("Error querying trade API. See log for details"));
                qWarning() << "PAPI: Error querying trade API";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            if (searchonsite)
            {
                QDesktopServices::openUrl(QUrl(u_trade_site + getLeague() + "/" + QString::fromStdString(resp["id"].get<std::string>())));
                return;
            }

            if (resp["result"].size() == 0)
            {
                emit humour(tr("No results found."));
                qDebug() << "No results";
                return;
            }

            // else process the results
            processPriceResults(item, resp);
        });
    });

    dlg->open();
}
