#include "itemapi.h"
#include "pitem.h"
#include "pta_types.h"

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
#include <QUrl>

// PoE trade api only allows 10 items at once
constexpr size_t papi_query_limit = 10;

// API URLs

// official
const QUrl u_api_league("https://www.pathofexile.com/api/trade/data/leagues");
const QUrl u_api_stats("https://www.pathofexile.com/api/trade/data/stats");
const QUrl u_api_items("https://www.pathofexile.com/api/trade/data/items");

// repoe
const QUrl u_repoe_base("https://raw.githubusercontent.com/brather1ng/RePoE/master/RePoE/data/base_items.min.json");
const QUrl u_repoe_mods("https://raw.githubusercontent.com/brather1ng/RePoE/master/RePoE/data/mods.min.json");

// pta
const QUrl u_pta_armourlocals("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/armour_locals.json");
const QUrl u_pta_basecat("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/base_categories.json");
const QUrl u_pta_currency("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/currency.json");
const QUrl u_pta_disc("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/discriminators.json");
const QUrl u_pta_enchantrules("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/enchant_rules.json");
const QUrl u_pta_pseudorules("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/pseudo_rules.json");
const QUrl u_pta_weaponlocals("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/weapon_locals.json");
const QUrl u_pta_excludes("https://raw.githubusercontent.com/r52/PTA/master/PTA/data/excludes.json");

// trade site
const QString u_trade_fetch("https://www.pathofexile.com/api/trade/fetch/%1?query=%2");
const QString u_trade_search("https://www.pathofexile.com/api/trade/search/");
const QString u_trade_exchange("https://www.pathofexile.com/api/trade/exchange/");
const QString u_trade_site("https://www.pathofexile.com/trade/search/");
const QString u_poewiki("https://pathofexile.gamepedia.com/");

// poe prices
const QString u_poeprices("https://www.poeprices.info/api?l=%1&i=%2");

ItemAPI::ItemAPI(QNetworkAccessManager* netmanager, QObject* parent) : QObject(parent), m_manager(netmanager)
{
    json data;

    ///////////////////////////////////////////// Download leagues

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

    ///////////////////////////////////////////// Load excludes (needs to be loaded BEFORE stats)

    QFile excl("data/excludes.json");

    if (excl.open(QIODevice::ReadOnly))
    {
        QByteArray edat = excl.readAll();

        json excj = json::parse(edat.toStdString());

        for (const auto& e : excj["excludes"])
        {
            c_excludes.insert(e.get<std::string>());
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_excludes), data))
    {
        for (const auto& e : data["excludes"])
        {
            c_excludes.insert(e.get<std::string>());
        }
    }
    else
    {
        throw std::runtime_error("Cannot open excludes.json");
    }

    qInfo() << "Exclude rules loaded";

    ///////////////////////////////////////////// Download stats

    if (!synchronizedGetJSON(QNetworkRequest(u_api_stats), data))
    {
        throw std::runtime_error("Failed to download stats data");
    }

    auto& stt = data["result"];

    for (const auto& type : stt)
    {
        auto& el = type["entries"];

        for (const auto& et : el)
        {
            // Cut the key for multiline mods
            std::string::size_type nl;
            std::string            text = et["text"].get<std::string>();
            std::string            id   = et["id"].get<std::string>();

            if ((nl = text.find("\n")) != std::string::npos)
            {
                text = text.substr(0, nl);
            }

            if (!c_excludes.contains(id))
            {
                m_stats_by_text.insert({{text, et}});
                m_stats_by_id.insert({{id, et}});
            }
        }
    }

    qInfo() << "Mod stats loaded";

    ///////////////////////////////////////////// Download unique items

    if (!synchronizedGetJSON(QNetworkRequest(u_api_items), data))
    {
        throw std::runtime_error("Failed to download unique item data");
    }

    auto& itm = data["result"];

    for (const auto& type : itm)
    {
        const auto& el = type["entries"];

        for (const auto& et : el)
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

    ///////////////////////////////////////////// Load base categories

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

    ///////////////////////////////////////////// Load RePoE base data (needs to be loaded AFTER c_baseCat)

    if (!synchronizedGetJSON(QNetworkRequest(u_repoe_base), data))
    {
        throw std::runtime_error("Failed to download base item data");
    }

    for (const auto& [k, o] : data.items())
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

    ///////////////////////////////////////////// Load RePoE mod data

    if (!synchronizedGetJSON(QNetworkRequest(u_repoe_mods), data))
    {
        throw std::runtime_error("Failed to download mod type data");
    }

    for (const auto& [k, o] : data.items())
    {
        std::string modname = o["name"].get<std::string>();
        std::string modtype = o["generation_type"].get<std::string>();

        if (modname.empty())
        {
            // Skip the mods with no name
            continue;
        }

        mod_generation_type type = mod_generation_type::mod_unknown;

        if (modtype == "prefix")
        {
            type = mod_generation_type::mod_prefix;
        }
        else if (modtype == "suffix")
        {
            type = mod_generation_type::mod_suffix;
        }

        if (type == mod_generation_type::mod_unknown)
        {
            // We only care about magic mods for now here so
            // skip all other mod types like corrupted/unique mods
            // qDebug() << "Skipped mod type" << QString::fromStdString(modtype) << "in mod" << QString::fromStdString(modname);
            continue;
        }

        c_mods.insert({{modname, type}});
    }

    qInfo() << "Mod types loaded";

    ///////////////////////////////////////////// Load pseudo rules

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

    ///////////////////////////////////////////// Load enchant rules

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

    ///////////////////////////////////////////// Load local rules

    QFile wl("data/weapon_locals.json");

    if (wl.open(QIODevice::ReadOnly))
    {
        QByteArray wdat = wl.readAll();

        json wlr = json::parse(wdat.toStdString());

        for (const auto& e : wlr["data"])
        {
            c_weaponLocals.insert(e.get<std::string>());
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_weaponlocals), data))
    {
        for (const auto& e : data["data"])
        {
            c_weaponLocals.insert(e.get<std::string>());
        }
    }
    else
    {
        throw std::runtime_error("Cannot open weapon_locals.json");
    }

    qInfo() << "Weapon Local rules loaded";

    /////////////////////////////////////////////  Armour locals
    QFile al("data/armour_locals.json");

    if (al.open(QIODevice::ReadOnly))
    {
        QByteArray adat = al.readAll();

        json alr = json::parse(adat.toStdString());

        for (const auto& e : alr["data"])
        {
            c_armourLocals.insert(e.get<std::string>());
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_armourlocals), data))
    {
        for (const auto& e : data["data"])
        {
            c_armourLocals.insert(e.get<std::string>());
        }
    }
    else
    {
        throw std::runtime_error("Cannot open armour_locals.json");
    }

    qInfo() << "Armour Local rules loaded";

    ///////////////////////////////////////////// Mod Discriminators

    QFile disc("data/discriminators.json");

    if (disc.open(QIODevice::ReadOnly))
    {
        QByteArray ddat = disc.readAll();

        json dcr = json::parse(ddat.toStdString());

        for (const auto [entry, list] : dcr.items())
        {
            for (const auto value : list["unused"])
            {
                c_discriminators[entry].insert(value.get<std::string>());
            }
        }
    }
    else if (synchronizedGetJSON(QNetworkRequest(u_pta_disc), data))
    {
        for (const auto [entry, list] : data.items())
        {
            for (const auto value : list["unused"])
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

    ///////////////////////////////////////////// Currency

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

    for (const auto& [k, v] : c_currencyMap.items())
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

json ItemAPI::readPropIntRange(QString prop)
{
    json val = {{"min", 0}, {"max", 0}};

    // If it is a list, process list
    if (prop.contains(", "))
    {
        // Split and recursively read
        auto list = prop.split(", ", QString::SkipEmptyParts);

        for (auto& item : list)
        {
            json nxt   = readPropIntRange(item);
            val[p_min] = val[p_min].get<int>() + nxt[p_min].get<int>();
            val[p_max] = val[p_max].get<int>() + nxt[p_max].get<int>();
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

        val[p_min] = v1.toInt();
        val[p_max] = v2.toInt();
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

json ItemAPI::readSockets(QString prop)
{
    json sockets;

    sockets["links"] = 0;
    sockets["total"] = 0;
    sockets["R"]     = 0;
    sockets["G"]     = 0;
    sockets["B"]     = 0;
    sockets["W"]     = 0;
    sockets["A"]     = 0;

    auto llist = prop.split(" ", QString::SkipEmptyParts);

    for (auto& lpart : llist)
    {
        auto socks = lpart.split("-", QString::SkipEmptyParts);

        if (socks.length() > 1 && socks.length() > sockets["links"].get<int>())
        {
            // New max links
            sockets["links"] = socks.length();
        }

        for (const auto& s : socks)
        {
            auto clr         = s.toStdString();
            sockets[clr]     = sockets[clr].get<int>() + 1;
            sockets["total"] = sockets["total"].get<int>() + 1;
        }
    }

    return sockets;
}

std::string ItemAPI::readName(QString name)
{
    name.remove(QRegularExpression("<<.*?>>|<.*?>"));

    return name.toStdString();
}

std::string ItemAPI::readType(Item& item, QString type)
{
    type.remove(QRegularExpression("<<.*?>>|<.*?>"));
    type.remove("Superior ");

    if (type.startsWith("Synthesised "))
    {
        type.remove("Synthesised ");
        item[p_msynth] = true;
    }

    if (item[p_rarity].get<std::string>() == "Magic")
    {
        // Parse out magic affixes
        // Try to get rid of all suffixes by forward catching " of"
        QRegularExpression              re("([\\w'-]+)(?(?= of)( of [\\w\\s]+))");
        QRegularExpressionMatchIterator i = re.globalMatch(type);

        QStringList words;
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            QString                 word  = match.captured(1);
            words << word;
        }

        std::string prefix = words.at(0).toStdString();

        // Remove prefixes
        if (c_mods.contains(prefix) && c_mods[prefix] == mod_generation_type::mod_prefix)
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

void ItemAPI::parseProp(Item& item, QString prop)
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
                    item[p_wpdps] = readPropIntRange(v);
                    break;
                }

                case weapon_filter_crit:
                {
                    item[p_wcrit] = readPropFloat(v);
                    break;
                }

                case weapon_filter_aps:
                {
                    item[p_waps] = readPropFloat(v);
                    break;
                }

                case weapon_filter_edps:
                {
                    item[p_wedps] = readPropIntRange(v);
                    break;
                }
            }

            item[p_weapon][p_enabled] = false;

            break;
        }

        case armour_filter:
        {
            switch (pev[1])
            {
                case armour_filter_ar:
                {
                    item[p_aar] = readPropInt(v);
                    break;
                }

                case armour_filter_ev:
                {
                    item[p_aev] = readPropInt(v);
                    break;
                }

                case armour_filter_es:
                {
                    item[p_aes] = readPropInt(v);
                    break;
                }

                case armour_filter_block:
                {
                    item[p_ablock] = readPropInt(v);
                    break;
                }
            }

            item[p_armour][p_enabled] = false;

            break;
        }

        case socket_filter:
        {
            item[p_sockets] = readSockets(v);
            break;
        }

        case req_filter:
        {
            switch (pev[1])
            {
                case req_filter_lvl:
                {
                    item[p_reqlvl] = readPropInt(v);
                    break;
                }

                case req_filter_str:
                {
                    item[p_reqstr] = readPropInt(v);
                    break;
                }

                case req_filter_dex:
                {
                    item[p_reqdex] = readPropInt(v);
                    break;
                }

                case req_filter_int:
                {
                    item[p_reqint] = readPropInt(v);
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
                    item[p_quality] = readPropInt(v);
                    break;
                }

                case misc_filter_gem_level:
                {
                    item[p_mglvl] = readPropInt(v);
                    break;
                }

                case misc_filter_ilvl:
                {
                    item[p_ilvl] = readPropInt(v);
                    break;
                }

                case misc_filter_gem_level_progress:
                {
                    item[p_mgexp] = v.toStdString();
                    break;
                }

                case misc_filter_map_tier:
                {
                    item[p_mmtier] = readPropInt(v);
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

bool ItemAPI::parseStat(Item& item, QString stat, QTextStream& stream)
{
    QString orig_stat = stat;

    // Special rule for A Master Seeks Help
    if (item.contains(p_category) && item[p_category].get<std::string>() == "prophecy" && item[p_name].get<std::string>() == "A Master Seeks Help")
    {
        QRegularExpression      re("^You will find (\\w+) and complete her mission.$");
        QRegularExpressionMatch match = re.match(stat);
        if (match.hasMatch())
        {
            QString master = match.captured(1);

            item[p_mdisc] = master.toLower().toStdString();
        }

        return true;
    }

    if (stat == "Unidentified")
    {
        item[p_unidentified] = true;
        return true;
    }

    if (stat == "Shaper Item")
    {
        item[p_influences].push_back(c_influenceMap[shaper]);
        return true;
    }

    if (stat == "Elder Item")
    {
        item[p_influences].push_back(c_influenceMap[elder]);
        return true;
    }

    if (stat == "Crusader Item")
    {
        item[p_influences].push_back(c_influenceMap[crusader]);
        return true;
    }

    if (stat == "Redeemer Item")
    {
        item[p_influences].push_back(c_influenceMap[redeemer]);

        return true;
    }

    if (stat == "Hunter Item")
    {
        item[p_influences].push_back(c_influenceMap[hunter]);

        return true;
    }

    if (stat == "Warlord Item")
    {
        item[p_influences].push_back(c_influenceMap[warlord]);

        return true;
    }

    if (stat == "Corrupted")
    {
        item[p_corrupted] = true;
        return true;
    }

    if (stat == "Synthesised Item")
    {
        // Should already have been processed
        item[p_msynth] = true;
        return true;
    }

    // Vaal gems
    if (item.contains(p_category) && item[p_category].get<std::string>() == "gem" && stat.startsWith("Vaal "))
    {
        item[p_type] = stat.toStdString();
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
    if (item.contains(p_weapon) || item.contains(p_armour))
    {
        bool is_local_stat = ((item.contains(p_weapon) && c_weaponLocals.contains(stoken)) || (item.contains(p_armour) && c_armourLocals.contains(stoken)));

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
            filter["id"]      = entry["id"];
            filter["type"]    = entry["type"];
            filter["text"]    = entry["text"];
            filter["value"]   = val;
            filter[p_enabled] = false;
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

            if (c_discriminators.contains(id) && c_discriminators[id].contains(item[p_category].get<std::string>()))
            {
                // Discriminator skip
                continue;
            }

            if (entry["type"] == "explicit")
            {
                filter["id"]      = entry["id"];
                filter["type"]    = entry["type"];
                filter["text"]    = entry["text"];
                filter["value"]   = val;
                filter[p_enabled] = false;
            }

            // Peek next line
            QString peek;

            int pos = stream.pos();

            if (pos != -1)
            {
                peek = stream.read(3);
                stream.seek(pos);
            }

            if (item[p_filters].size() < 2 && peek == "---")
            {
                // First stat with a section break, try to look for an enchant
                if (entry["type"] == "enchant")
                {
                    filter["id"]      = entry["id"];
                    filter["type"]    = entry["type"];
                    filter["text"]    = entry["text"];
                    filter["value"]   = val;
                    filter[p_enabled] = false;
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
    if (item[p_filters].contains(fid))
    {
        auto& efil = item[p_filters][fid];

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
        item[p_filters].emplace(fid, filter);
    }

    return true;
}

void ItemAPI::processPriceResults(json data, json response, const QString& optstr, const QString& format)
{
    bool   done  = false;
    size_t start = 0, end = 0;
    auto   flist = response["result"];
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

        QString fetchurl = u_trade_fetch.arg(fcode).arg(QString::fromStdString(response["id"]));

        if (format == "exchange")
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

    endRes["options"] = optstr.toStdString();
    endRes["format"]  = format.toStdString();

    if (format == "simple" || format == "exchange")
    {
        data[p_results] = endRes;

        emit simpleResultsFinished(QString::fromStdString(data.dump()));
    }
    else
    {
        emit priceCheckFinished(QString::fromStdString(endRes.dump()));
    }
}

void ItemAPI::doCurrencySearch(json& data)
{
    Item& item = data[p_item];

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
    if (!c_currencyMap.contains(item[p_type]))
    {
        emit humour(tr("Could not find this currency in the database. See log for details."));
        qWarning() << "Currency not found:" << QString::fromStdString(item[p_type]);
        qWarning() << "If you believe that this is a mistake, please file a bug report on GitHub.";
        return;
    }

    std::string want = c_currencyMap[item[p_type].get<std::string>()].get<std::string>();
    std::string have = p_curr;

    if (want == p_curr)
    {
        have = s_curr;
    }

    query["exchange"]["want"].push_back(want);

    while (true)
    {
        QString options = "To " + QString::fromStdString(have);

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
        processPriceResults(data, resp, options, "exchange");
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

bool ItemAPI::parse(Item& item, QString itemText)
{
    QTextStream stream(&itemText, QIODevice::ReadOnly);
    QString     line;
    int         sections = 0;

    // Check first line for PoE item
    stream.readLineInto(&line);

    if (!line.startsWith("Rarity:"))
    {
        qWarning() << "Parse called on non PoE item text";
        return false;
    }

    // Full original text
    item[p_origtext] = itemText.toStdString();

    // Rarity
    item[p_rarity] = line.section(": ", 1, 1).toStdString();

    // Read name/type
    QString nametype, type;
    stream.readLineInto(&nametype);
    stream.readLineInto(&type);

    if (nametype.startsWith("You cannot"))
    {
        // Item requirements not met msg (why is this needed GGG?!)
        // Ignore it and read the subsequent lines
        stream.readLineInto(&nametype);
        stream.readLineInto(&type);
    }

    if (type.startsWith("---"))
    {
        // nametype has to be item type and not name
        item[p_type] = readType(item, nametype);
        sections++;
    }
    else
    {
        item[p_name] = readName(nametype);
        item[p_type] = readType(item, type);
    }

    // Process category
    if ("Gem" == item[p_rarity].get<std::string>())
    {
        item[p_category] = "gem";
    }
    else if ("Divination Card" == item[p_rarity].get<std::string>())
    {
        item[p_category] = item[p_rarity] = "card";
    }

    if (item[p_type].get<std::string>().ends_with("Map"))
    {
        item[p_category] = "map";
        item[p_mdisc]    = m_mapdisc; // Default map discriminator

        item[p_type] = std::regex_replace(item[p_type].get<std::string>(), std::regex("Elder "), "");
        item[p_type] = std::regex_replace(item[p_type].get<std::string>(), std::regex("Shaped "), "");
    }

    if (!item.contains(p_category) && m_uniques.contains(item[p_type].get<std::string>()))
    {
        auto search = m_uniques.find(item[p_type]);
        if (search != m_uniques.end())
        {
            auto& je = search->second;
            if (je["type"] == "Prophecy")
            {
                // this is a prophecy
                item[p_name]     = item[p_type];
                item[p_type]     = "Prophecy";
                item[p_category] = "prophecy";
            }
        }
    }

    if (!item.contains(p_category))
    {
        auto base = c_baseMap.find(item[p_type].get<std::string>());
        if (base != c_baseMap.end())
        {
            json cat         = base->second;
            item[p_category] = cat["category"];
        }
    }

    // Read the rest of the crap

    while (stream.readLineInto(&line))
    {
        // Skip
        if (line.startsWith("---"))
        {
            m_section.clear();
            sections++;
            continue;
        }

        if (line.contains(":"))
        {
            // parse item prop
            parseProp(item, line);
        }
        else if (sections > 1)
        {
            // parse item stat
            parseStat(item, line, stream);
        }
    }

    // Process special/pseudo rules
    if (item[p_filters].size())
    {
        for (const auto [key, fil] : item[p_filters].items())
        {
            if (c_pseudoRules.contains(key))
            {
                const auto& rules = c_pseudoRules[key];

                for (const auto& r : rules)
                {
                    std::string pid = r["id"].get<std::string>();

                    auto pentry = m_stats_by_id[pid];

                    if (!item[p_pseudos].contains(pid))
                    {
                        json ps_entry = pentry;

                        ps_entry[p_enabled] = false;
                        ps_entry["value"]   = json::array();

                        for (const auto v : fil["value"])
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

                        item[p_pseudos].emplace(pid, ps_entry);
                    }
                    else
                    {
                        auto& ps_entry = item[p_pseudos][pid];

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

    return true;
}

void ItemAPI::fillItemOptions(json& data)
{
    QSettings settings;

    std::string league           = getLeague().toStdString();
    size_t      display_limit    = settings.value(PTA_CONFIG_DISPLAYLIMIT, PTA_CONFIG_DEFAULT_DISPLAYLIMIT).toInt();
    bool        corrupt_override = settings.value(PTA_CONFIG_CORRUPTOVERRIDE, PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE).toBool();
    std::string corrupt_search   = settings.value(PTA_CONFIG_CORRUPTSEARCH, PTA_CONFIG_DEFAULT_CORRUPTSEARCH).toString().toStdString();
    std::string p_curr           = settings.value(PTA_CONFIG_PRIMARY_CURRENCY, PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY).toString().toStdString();
    std::string s_curr           = settings.value(PTA_CONFIG_SECONDARY_CURRENCY, PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY).toString().toStdString();
    bool        onlineonly       = settings.value(PTA_CONFIG_ONLINE_ONLY, PTA_CONFIG_DEFAULT_ONLINE_ONLY).toBool();
    bool        buyoutonly       = settings.value(PTA_CONFIG_BUYOUT_ONLY, PTA_CONFIG_DEFAULT_BUYOUT_ONLY).toBool();
    bool        removedupes      = settings.value(PTA_CONFIG_REMOVE_DUPES, PTA_CONFIG_DEFAULT_REMOVE_DUPES).toBool();
    bool        prefillmin       = settings.value(PTA_CONFIG_PREFILL_MIN, PTA_CONFIG_DEFAULT_PREFILL_MIN).toBool();
    bool        prefillmax       = settings.value(PTA_CONFIG_PREFILL_MAX, PTA_CONFIG_DEFAULT_PREFILL_MAX).toBool();
    bool        prefillnormals   = settings.value(PTA_CONFIG_PREFILL_NORMALS, PTA_CONFIG_DEFAULT_PREFILL_NORMALS).toBool();
    bool        prefillpseudos   = settings.value(PTA_CONFIG_PREFILL_PSEUDOS, PTA_CONFIG_DEFAULT_PREFILL_PSEUDOS).toBool();
    bool        prefillilvl      = settings.value(PTA_CONFIG_PREFILL_ILVL, PTA_CONFIG_DEFAULT_PREFILL_ILVL).toBool();
    bool        prefillbase      = settings.value(PTA_CONFIG_PREFILL_BASE, PTA_CONFIG_DEFAULT_PREFILL_BASE).toBool();

    // app settings
    data[p_settings]["league"]             = league;
    data[p_settings]["display_limit"]      = display_limit;
    data[p_settings]["corrupt_override"]   = corrupt_override;
    data[p_settings]["corrupt_search"]     = corrupt_search;
    data[p_settings]["primary_currency"]   = p_curr;
    data[p_settings]["secondary_currency"] = s_curr;
    data[p_settings]["onlineonly"]         = onlineonly;
    data[p_settings]["buyoutonly"]         = buyoutonly;
    data[p_settings]["removedupes"]        = removedupes;
    data[p_settings]["prefillmin"]         = prefillmin;
    data[p_settings]["prefillmax"]         = prefillmax;
    data[p_settings]["prefillnormals"]     = prefillnormals;
    data[p_settings]["prefillpseudos"]     = prefillpseudos;
    data[p_settings]["prefillilvl"]        = prefillilvl;
    data[p_settings]["prefillbase"]        = prefillbase;

    // search defaults
    data[p_opts]["use_pdps"][p_enabled] = false;
    data[p_opts]["use_edps"][p_enabled] = false;
    data[p_opts]["use_ar"][p_enabled]   = false;
    data[p_opts]["use_ev"][p_enabled]   = false;
    data[p_opts]["use_es"][p_enabled]   = false;

    data[p_opts]["use_sockets"]   = false;
    data[p_opts]["use_links"]     = false;
    data[p_opts]["use_ilvl"]      = prefillilvl;
    data[p_opts]["use_item_base"] = prefillbase;

    if (prefillbase)
    {
        if (data[p_item].contains(p_influences))
        {
            data[p_opts][p_influences] = data[p_item][p_influences];
        }

        if (data[p_item].contains(p_msynth) && data[p_item][p_msynth].get<bool>())
        {
            data[p_opts]["use_synthesis_base"] = prefillbase;
        }
    }
}

bool ItemAPI::trySimplePriceCheck(json& data)
{
    Item& item = data[p_item];

    // If its a currency and the currency is listed in the bulk exchange, try that first
    // Otherwise, try a regular search
    if (item[p_category] == "currency" && c_currencyMap.contains(item[p_type]))
    {
        doCurrencySearch(data);
        return true;
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
    if ((item[p_category] == "map" && item[p_rarity] == "Rare") || !item.contains(p_name))
    {
        is_unique_base = m_uniques.contains(item[p_type]);
        searchToken    = item[p_type];
    }
    else
    {
        is_unique_base = m_uniques.contains(item[p_name]);
        searchToken    = item[p_name];
    }

    // Force rarity if unique
    if (item[p_rarity] == "Unique")
    {
        std::string rarity = item[p_rarity];
        std::transform(rarity.begin(), rarity.end(), rarity.begin(), ::tolower);

        query["query"]["filters"]["type_filters"]["filters"]["rarity"]["option"] = rarity;
    }

    // Force category
    if (item.contains(p_category))
    {
        std::string category = item[p_category];
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
            if (item.contains(p_misc) && item.contains(p_mdisc))
            {
                if (entry["disc"] == item[p_mdisc] && entry["type"] == item[p_type])
                {
                    if (entry.contains("name"))
                    {
                        qe["name"] = {{"discriminator", entry["disc"]}, {"option", entry["name"]}};
                    }

                    qe["type"] = {{"discriminator", entry["disc"]}, {"option", entry["type"]}};

                    break;
                }
            }
            else if (entry["type"] == item[p_type])
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

        QString options = getLeague();

        // Default Gem options
        if (item[p_category] == "gem")
        {
            qe["filters"]["misc_filters"]["filters"]["gem_level"]["min"] = item[p_mglvl];
            qe["filters"]["misc_filters"]["filters"]["quality"]["min"]   = item[p_quality];

            options += ", Lv" + QString::number(item[p_mglvl].get<int>()) + "/" + QString::number(item[p_quality].get<int>()) + "%";
        }

        // Default socket options
        if (item.contains(p_sockets) && item[p_sockets]["total"] == 6)
        {
            qe["filters"]["socket_filters"]["filters"]["sockets"]["min"] = item[p_sockets]["total"];

            options += ", " + QString::number(item[p_sockets]["total"].get<int>()) + "S";
        }

        // Default link options
        if (item.contains(p_sockets) && item[p_sockets]["links"] > 4)
        {
            qe["filters"]["socket_filters"]["filters"]["links"]["min"] = item[p_sockets]["links"];

            options += ", " + QString::number(item[p_sockets]["links"].get<int>()) + "L";
        }

        // Force iLvl
        if (item[p_rarity] != "Unique" && item[p_category] != "card" && item.contains(p_ilvl))
        {
            qe["filters"]["misc_filters"]["filters"]["ilvl"]["min"] = item[p_ilvl];

            options += ", iLvl=" + QString::number(item[p_ilvl].get<int>());
        }

        // Force map tier
        if (item[p_category] == "map" && item.contains(p_mmtier))
        {
            qe["filters"]["map_filters"]["filters"]["map_tier"]["min"] = item[p_mmtier];

            options += ", Map Tier=" + QString::number(item[p_mmtier].get<int>());
        }

        // Note discriminator
        if (item.contains(p_mdisc))
        {
            options += ", Disc=" + QString::fromStdString(item[p_mdisc].get<std::string>());
        }

        // Force Influences
        if (item[p_category] != "card" && item.contains(p_influences))
        {
            for (auto i : item[p_influences])
            {
                std::string inf     = i.get<std::string>();
                std::string inftype = inf + "_item";

                qe["filters"]["misc_filters"]["filters"][inftype]["option"] = true;

                inf[0] = toupper(inf[0]);
                options += ", " + QString::fromStdString(inf) + " Influence";
            }
        }

        // Force Synthesis
        if (item.contains(p_msynth))
        {
            qe["filters"]["misc_filters"]["filters"]["synthesised_item"]["option"] = true;
            options += ", Synthesis Base";
        }

        // Default corrupt options
        bool corrupt_override = settings.value(PTA_CONFIG_CORRUPTOVERRIDE, PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE).toBool();

        // No such thing as corrupted cards or prophecies
        if (item[p_category] != "card" && item[p_category] != "prophecy")
        {
            if (corrupt_override)
            {
                QString corrupt_search = settings.value(PTA_CONFIG_CORRUPTSEARCH, PTA_CONFIG_DEFAULT_CORRUPTSEARCH).toString();

                if (corrupt_search != "Any")
                {
                    qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = (corrupt_search == "Yes");

                    options += ", Corrupted=" + corrupt_search;
                }
                else
                {
                    options += ", Corrupted=Any";
                }

                options += " (override)";
            }
            else
            {
                qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = item.contains(p_corrupted) && item[p_corrupted].get<bool>();

                options += ", Corrupted=";
                options += (item.contains(p_corrupted) && item[p_corrupted].get<bool>()) ? "Yes" : "No";
            }
        }

        options += ", Mods ignored";

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
            processPriceResults(data, resp, options, "simple");
        });

        return true;
    }
    else if (item[p_rarity] != "Magic")
    {
        // poeprices.info

        QString itemText = QString::fromStdString(item[p_origtext]);

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
        connect(req, &QNetworkReply::finished, [=]() mutable {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                emit humour(tr("Error querying poeprices.info. See log for details"));
                qWarning() << "PAPI: Error querying poeprices.info" << req->error() << req->errorString();
                return;
            }

            QByteArray respdata = req->readAll();
            QString    tstr     = QString::fromUtf8(respdata.data(), respdata.size());

            if (!tstr.size())
            {
                emit humour(tr("Error querying poeprices.info. See log for details"));
                qWarning() << "PAPI: Error querying poeprices.info - returned no data";
                return;
            }

            auto resp = json::parse(tstr.toStdString());
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

            // set format
            data["prediction"] = resp;

            // else process the results
            emit simpleResultsFinished(QString::fromStdString(data.dump()));
        });

        return true;
    }

    return false;
}

void ItemAPI::advancedPriceCheck(const QString& str, bool openonsite)
{
    json  data = json::parse(str.toStdString());
    Item& item = data[p_item];

    if (!item.contains(p_filters) || item[p_category] == "map")
    {
        // Cannot advanced search items with no filters
        emit humour(tr("Advanced search is unavailable for this item type"));
        return;
    }

    if (item.contains(p_unidentified))
    {
        emit humour(tr("Advanced search is unavailable for unidentified items"));
        return;
    }

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

    if (item.contains(p_name))
    {
        is_unique_base = m_uniques.contains(item[p_name]);
        searchToken    = item[p_name];
    }
    else
    {
        is_unique_base = m_uniques.contains(item[p_type]);
        searchToken    = item[p_type];
    }

    // Force rarity
    std::string rarity = "nonunique";

    if (item[p_rarity] == "Unique")
    {
        rarity = item[p_rarity];
        std::transform(rarity.begin(), rarity.end(), rarity.begin(), ::tolower);
    }

    query["query"]["filters"]["type_filters"]["filters"]["rarity"]["option"] = rarity;

    // Force category
    if (item.contains(p_category))
    {
        std::string category = item[p_category];
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);

        query["query"]["filters"]["type_filters"]["filters"]["category"]["option"] = category;
    }

    // weapon/armour base mods

    if (data[p_usepdps][p_enabled].get<bool>())
    {
        if (data[p_usepdps][p_min] > 0)
        {
            query["query"]["filters"]["weapon_filters"]["filters"]["pdps"]["min"] = data[p_usepdps][p_min];
        }

        if (data[p_usepdps][p_max] > 0)
        {
            query["query"]["filters"]["weapon_filters"]["filters"]["pdps"]["max"] = data[p_usepdps][p_max];
        }
    }

    if (data[p_useedps][p_enabled].get<bool>())
    {
        if (data[p_useedps][p_min] > 0)
        {
            query["query"]["filters"]["weapon_filters"]["filters"]["edps"]["min"] = data[p_useedps][p_min];
        }

        if (data[p_useedps][p_max] > 0)
        {
            query["query"]["filters"]["weapon_filters"]["filters"]["edps"]["max"] = data[p_useedps][p_max];
        }
    }

    if (data[p_usear][p_enabled].get<bool>())
    {
        if (data[p_usear][p_min] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["ar"]["min"] = data[p_usear][p_min];
        }

        if (data[p_usear][p_max] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["ar"]["max"] = data[p_usear][p_max];
        }
    }

    if (data[p_useev][p_enabled].get<bool>())
    {
        if (data[p_useev][p_min] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["ev"]["min"] = data[p_useev][p_min];
        }

        if (data[p_useev][p_max] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["ev"]["max"] = data[p_useev][p_max];
        }
    }

    if (data[p_usees][p_enabled].get<bool>())
    {
        if (data[p_usees][p_min] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["es"]["min"] = data[p_usees][p_min];
        }

        if (data[p_usees][p_max] > 0)
        {
            query["query"]["filters"]["armour_filters"]["filters"]["es"]["max"] = data[p_usees][p_max];
        }
    }

    // Checked mods
    for (auto& [k, e] : item[p_filters].items())
    {
        // set id
        e["id"] = k;

        if (e[p_enabled] == true)
        {
            e["disabled"] = false;
            e.erase(p_enabled);
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
            if (entry["type"] == item[p_type])
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

    QString options = getLeague();

    // Use sockets
    if (data[p_usesockets].get<bool>())
    {
        qe["filters"]["socket_filters"]["filters"]["sockets"]["min"] = item[p_sockets]["total"];

        options += ", " + QString::number(item[p_sockets]["total"].get<int>()) + "S";
    }

    // Use links
    if (data[p_uselinks].get<bool>())
    {
        qe["filters"]["socket_filters"]["filters"]["links"]["min"] = item[p_sockets]["links"];

        options += ", " + QString::number(item[p_sockets]["links"].get<int>()) + "L";
    }

    // Use iLvl
    if (data[p_useilvl].get<bool>())
    {
        qe["filters"]["misc_filters"]["filters"]["ilvl"]["min"] = item[p_ilvl];

        options += ", iLvl=" + QString::number(item[p_ilvl].get<int>());
    }

    // Use item base
    if (data[p_usebase].get<bool>())
    {
        qe["type"] = item[p_type];

        options += ", Use Base Type";
    }

    // Influences

    if (data.contains(p_useinfluences) && data[p_useinfluences].size())
    {
        for (const auto i : data[p_useinfluences])
        {
            std::string inf    = i.get<std::string>();
            std::string infkey = inf + "_item";

            qe["filters"]["misc_filters"]["filters"][infkey]["option"] = true;

            inf[0] = toupper(inf[0]);
            options += ", " + QString::fromStdString(inf) + " Influence";
        }
    }

    // Synthesis
    if (data.contains(p_usesynth) && data[p_usesynth].get<bool>())
    {
        qe["filters"]["misc_filters"]["filters"]["synthesised_item"]["option"] = true;
        options += ", Synthesis Base";
    }

    // Corrupt
    if (item.contains(p_corrupted))
    {
        qe["filters"]["misc_filters"]["filters"]["corrupted"]["option"] = item[p_corrupted];
        options += ", Corrupted=";
        options += item[p_corrupted].get<bool>() ? "Yes" : "No";
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

        if (openonsite)
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
        processPriceResults(data, resp, options, "advanced");
    });
}

void ItemAPI::openWiki(const Item& item)
{
    QString itemName;

    if (item[p_rarity].get<std::string>() == "Rare")
    {
        itemName = QString::fromStdString(item[p_type].get<std::string>());
    }
    else if (item.contains(p_name))
    {
        itemName = QString::fromStdString(item[p_name].get<std::string>());
    }
    else
    {
        itemName = QString::fromStdString(item[p_type].get<std::string>());
    }

    itemName = itemName.replace(" ", "_");
    if (!QDesktopServices::openUrl(u_poewiki + itemName))
    {
        emit humour(tr("Opening the URL failed"));
    }
}
