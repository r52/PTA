#include "itemapi.h"
#include "pitem.h"
#include "pta_types.h"

#include <unordered_set>

#include <nlohmann/json.hpp>

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QSettings>
#include <QTextStream>

using json = nlohmann::json;

// PoE trade api only allows 10 items at once
constexpr size_t papi_query_limit = 10;

ItemAPI::ItemAPI(QObject* parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    // Download leagues

    auto rp_league = m_manager->get(QNetworkRequest(QUrl("https://www.pathofexile.com/api/trade/data/leagues")));
    connect(rp_league, &QNetworkReply::finished, [=]() {
        rp_league->deleteLater();

        if (rp_league->error() != QNetworkReply::NoError)
        {
            qWarning() << "PAPI: Error downloading leagues" << rp_league->error() << rp_league->errorString();
            return;
        }

        m_leagues = json::parse(rp_league->readAll().toStdString());
    });

    // Download stats

    auto rp_stats = m_manager->get(QNetworkRequest(QUrl("https://www.pathofexile.com/api/trade/data/stats")));
    connect(rp_stats, &QNetworkReply::finished, [=]() {
        rp_stats->deleteLater();

        if (rp_stats->error() != QNetworkReply::NoError)
        {
            qWarning() << "PAPI: Error downloading stats" << rp_stats->error() << rp_stats->errorString();
            return;
        }

        auto obj = json::parse(rp_stats->readAll().toStdString());

        auto& ll = obj["result"];

        for (auto& type : ll)
        {
            auto& el = type["entries"];

            for (auto& et : el)
            {
                m_stats_by_text.insert({{et["text"].get<std::string>(), et}});
                m_stats_by_id.insert({{et["id"].get<std::string>(), et}});
            }
        }
    });

    // Download unique items

    auto rp_uniq = m_manager->get(QNetworkRequest(QUrl("https://www.pathofexile.com/api/trade/data/items")));
    connect(rp_uniq, &QNetworkReply::finished, [=]() {
        rp_uniq->deleteLater();

        if (rp_uniq->error() != QNetworkReply::NoError)
        {
            qWarning() << "PAPI: Error downloading unique items" << rp_uniq->error() << rp_uniq->errorString();
            return;
        }

        // Process data
        auto obj = json::parse(rp_uniq->readAll().toStdString());

        auto& ll = obj["result"];

        for (auto& type : ll)
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
    });
}

int ItemAPI::readPropInt(QString prop)
{
    // Remove augmented tag
    prop.replace(" (augmented)", "");

    QRegularExpression      re("^\\+?(\\d+)%?");
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

    QRegularExpression      re("^\\+?([\\d\\.]+)%?");
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
    // TODO
    return 0;
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

void ItemAPI::parseStat(PItem* item, QString stat)
{
    // Make a copy
    QString orig_stat = stat;

    if (stat == "Unidentified")
    {
        item->f_misc.identified = false;
        return;
    }

    if (stat == "Shaper Item")
    {
        item->f_misc.shaper_item = true;
        return;
    }

    if (stat == "Elder Item")
    {
        item->f_misc.elder_item = true;
        return;
    }

    if (stat == "Corrupted")
    {
        item->f_misc.corrupted = true;
        return;
    }

    bool stat_is_crafted = false;

    if (stat.endsWith("(crafted)"))
    {
        stat_is_crafted = true;
    }

    stat.replace(" (crafted)", "");

    // Get numeric values from stat
    json val = json::array();

    QRegularExpression              re("\\+?(\\d+)");
    QRegularExpressionMatchIterator it = re.globalMatch(stat);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString                 word  = match.captured(1);

        val.push_back(word.toInt());
    }

    if (!val.size())
    {
        // If stat has no values, then it has no variance. Skip
        qDebug() << "Static stat line" << orig_stat;
        return;
    }

    // Craft search token
    stat.replace(re, "#");

    auto stoken = stat.toStdString();

    if (!m_stats_by_text.contains(stoken))
    {
        // Ignored line
        qDebug() << "Ignored/unprocessed line" << orig_stat;
        return;
    }

    json filter;

    auto range = m_stats_by_text.equal_range(stoken);
    for (auto it = range.first; it != range.second; ++it)
    {
        auto& entry = it->second;

        if (stat_is_crafted)
        {
            if (entry["type"] != "crafted")
            {
                // skip this entry
                continue;
            }

            // use crafted stat
            filter["id"]    = entry["id"];
            filter["type"]  = entry["type"];
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

            if (!filter.empty() && filter["type"] == "implicit" && entry["type"] == "explicit")
            {
                // prefer explicit?
                filter["id"]    = entry["id"];
                filter["type"]  = entry["type"];
                filter["value"] = val;
            }
            else if (filter.empty())
            {
                filter["id"]    = entry["id"];
                filter["type"]  = entry["type"];
                filter["value"] = val;
            }
            // else skip
        }
    }

    if (filter.empty())
    {
        qDebug() << "Error parsing stat line" << orig_stat;
        return;
    }

    // TODO: process special/pseudo rules here

    // If the item already has this filter, merge them
    if (item->filters.contains(filter["id"].get<std::string>()))
    {
        auto& efil = item->filters[filter["id"].get<std::string>()];

        auto count = efil["value"].size();

        for (size_t i = 0; i < count; i++)
        {
            efil["value"][i] += filter["value"][i];
        }
    }
    else
    {
        item->filters.insert({filter["id"].get<std::string>(), filter});
    }
}

void ItemAPI::processPriceResults(std::shared_ptr<PItem> item, json results)
{
    bool   done  = false;
    size_t start = 0, end = 0;
    auto   flist = results["result"];
    auto   total = flist.size();
    json   endRes;

    std::unordered_set<std::string> accounts;

    QSettings settings;
    size_t    display_limit = settings.value(PTA_CONFIG_DISPLAYLIMIT, PTA_CONFIG_DEFAULT_DISPLAYLIMIT).toInt();

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

        QString resUrl = QString("https://www.pathofexile.com/api/trade/fetch/%1?query=%2").arg(fcode).arg(QString::fromStdString(results["id"]));

        QEventLoop loop;
        connect(m_manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

        auto pricereq = m_manager->get(QNetworkRequest(QUrl(resUrl)));
        loop.exec();

        pricereq->deleteLater();

        if (pricereq->error() != QNetworkReply::NoError)
        {
            qWarning() << "PAPI: Error getting prices" << pricereq->error() << pricereq->errorString();
            return;
        }

        auto pricesb = pricereq->readAll();

        auto dat = json::parse(pricesb.toStdString());

        auto& rj = dat["result"];

        // Delete duplicate accounts
        // TODO make this a setting
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

PItem* ItemAPI::parse(QString itemText)
{
    QTextStream stream(&itemText);
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

    if (type.startsWith("-"))
    {
        // nametype has to be item type and not name
        item->setType(nametype.toStdString());
    }
    else
    {
        item->setName(nametype.toStdString());
        item->setType(type.toStdString());
    }

    // Process category
    if ("Gem" == item->f_type.rarity)
    {
        item->f_type.category = "Gem";
    }
    else if ("Divination Card" == item->f_type.rarity)
    {
        item->f_type.category = item->f_type.rarity = "Card";
    }

    if (item->type.ends_with("Map"))
    {
        item->f_type.category = "Map";
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
                item->name = item->type;
                item->type = item->f_type.category = "Prophecy";
            }
        }
    }

    // Read the rest of the crap

    while (stream.readLineInto(&line))
    {
        // Skip
        if (line.startsWith('-'))
        {
            m_section.clear();
            continue;
        }

        if (line.contains(":"))
        {
            // parse item prop
            parseProp(item, line);
        }
        else
        {
            // parse item stat
            parseStat(item, line);
        }
    }

    return item;
}

QString ItemAPI::toJson(PItem* item)
{
    json j;

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

    j["ilvl"]    = item->f_misc.ilvl;
    j["quality"] = item->f_misc.quality;

    if (item->f_type.category == "Gem")
    {
        j["gem_level"] = item->f_misc.gem_level;
    }

    j["elder_item"]  = item->f_misc.elder_item;
    j["shaper_item"] = item->f_misc.shaper_item;
    j["identified"]  = item->f_misc.identified;
    j["corrupted"]   = item->f_misc.corrupted;

    if (!item->m_options.empty())
    {
        j["options"] = item->m_options;
    }

    return QString::fromStdString(j.dump());
}

void ItemAPI::simplePriceCheck(std::shared_ptr<PItem> item)
{
    if (item->f_type.rarity == "Currency")
    {
        emit humour("Currency search is unimplemented");
        return;
    }

    QSettings settings;

    // TODO: default settings
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

            if (item->f_type.category == "Map")
            {
                // Use default map discriminator
                if (entry["disc"] == m_mapdisc)
                {
                    // use this entry
                    if (entry.contains("name"))
                    {
                        qe["name"] = {{"discriminator", entry["disc"]}, {"option", entry["name"]}};
                    }

                    qe["type"] = {{"discriminator", entry["disc"]}, {"option", entry["type"]}};

                    break;
                }
            }
            else
            {
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

        // TODO league
        item->m_options = "Legion";

        // Default Gem options
        if (item->f_type.category == "Gem")
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

        item->m_options += ", Mods ignored";

        auto qba = query.dump();

        // TODO: Fix league
        QNetworkRequest request;
        request.setUrl(QUrl("https://www.pathofexile.com/api/trade/search/Legion"));
        request.setRawHeader("Content-Type", "application/json");

        auto req = m_manager->post(request, QByteArray::fromStdString(qba));
        connect(req, &QNetworkReply::finished, [=]() {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                qWarning() << "PAPI: Error querying trade site" << req->error() << req->errorString();
                return;
            }

            auto respdata = req->readAll();
            auto resp     = json::parse(respdata.toStdString());
            if (!resp.contains("result") || !resp.contains("id"))
            {
                emit humour("Error querying trade site");
                qWarning() << "PAPI: Error querying trade site";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            if (resp["result"].size() == 0)
            {
                emit humour("No results found.");
                qDebug() << "No results";
                return;
            }

            // else process the results
            processPriceResults(item, resp);
        });
    }
    else
    {
        // TODO
        emit humour("Simple price check for rare items is unimplemented");
        qWarning() << "Unimplemented";
    }
}

void ItemAPI::advancedPriceCheck(std::shared_ptr<PItem> item)
{
    if (item->filters.empty())
    {
        // Cannot advanced search items with no filters
        emit humour("Advanced search is unavailable for this item type");
        return;
    }

    emit humour("Advanced search is unimplemented");
    qWarning() << "Unimplemented";
}
