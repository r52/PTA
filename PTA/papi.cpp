#include "papi.h"

#include "pitem.h"

#include <QApplication>
#include <QClipboard>
#include <QNetworkAccessManager>
#include <QNetworkReply>

PAPI::PAPI(QObject* parent) : QObject(parent)
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

        m_stats = json::parse(rp_stats->readAll().toStdString());
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

        // TODO FIX THIS FOR DUPLICATES
        for (auto& type : ll)
        {
            auto& el = type["entries"];

            for (auto& et : el)
            {
                if (et.contains("name"))
                {
                    if (m_uniques.contains(et["name"]))
                    {
                        // Debug: Entry already exists???
                        qDebug() << "Duplicate unique entry:" << QString::fromStdString(et["name"].get<std::string>());
                    }

                    m_uniques[et["name"]] = et;
                }
                else if (et.contains("disc"))
                {
                    // only check against newest maps
                    if (et["disc"] == "warfortheatlas")
                    {
                        if (m_uniques.contains(et["type"]))
                        {
                            // Debug: Entry already exists???
                            qDebug() << "Duplicate unique entry:" << QString::fromStdString(et["type"].get<std::string>());
                        }

                        m_uniques[et["type"]] = et;
                    }
                }
                else
                {
                    if (m_uniques.contains(et["type"]))
                    {
                        // Debug: Entry already exists???
                        qDebug() << "Duplicate unique entry:" << QString::fromStdString(et["type"].get<std::string>());
                    }

                    // gems and stuff
                    m_uniques[et["type"]] = et;
                }
            }
        }
    });
}

void PAPI::simplePriceCheck(std::shared_ptr<PItem> item)
{
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

    bool        is_unique = false;
    std::string searchToken;

    if (item->f_type.category == "Map" && item->f_type.rarity != "Unique")
    {
        is_unique   = m_uniques.contains(item->m_type);
        searchToken = item->m_type;
    }
    else
    {
        is_unique   = m_uniques.contains(item->m_name);
        searchToken = item->m_name;
    }

    // Force rarity
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
    if (is_unique)
    {
        auto& entry = m_uniques[searchToken];

        auto& qe = query["query"];

        if (entry.contains("disc"))
        {
            // map entry
            if (entry.contains("name"))
            {
                qe["name"] = {{"discriminator", entry["disc"]}, {"option", entry["name"]}};
            }

            qe["type"] = {{"discriminator", entry["disc"]}, {"option", entry["type"]}};
        }
        else if (entry.contains("name"))
        {
            // generic unique with name
            qe["name"] = entry["name"];
            qe["type"] = entry["type"];
        }
        else
        {
            // only type
            qe["type"] = entry["type"];
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

        item->m_options += ", Corrupted=any, Mods ignored";

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
                qWarning() << "PAPI: Error querying trade site";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            QStringList fetchcodes;
            auto&       flist = resp["result"];
            auto        len   = flist.size();

            if (len == 0)
            {
                emit humour("No results found.");
                qDebug() << "No results";
                return;
            }

            if (len > 10)
            {
                // Take the first 10 results
                len = 10;
            }

            for (int i = 0; i < len; i++)
            {
                fetchcodes.append(QString::fromStdString(flist.at(i)));
            }

            QString fcode = fetchcodes.join(',');

            QString resUrl = QString("https://www.pathofexile.com/api/trade/fetch/%1?query=%2").arg(fcode).arg(QString::fromStdString(resp["id"]));

            auto pricereq = m_manager->get(QNetworkRequest(QUrl(resUrl)));
            connect(pricereq, &QNetworkReply::finished, [=]() {
                pricereq->deleteLater();

                if (pricereq->error() != QNetworkReply::NoError)
                {
                    qWarning() << "PAPI: Error getting prices" << pricereq->error() << pricereq->errorString();
                    return;
                }

                auto pricesj = pricereq->readAll();
                auto res     = QString::fromUtf8(pricesj);

                emit priceCheckFinished(item, res);
            });
        });
    }
    else
    {
        // TODO
        emit humour("Simple price check for rare items is unimplemented");
        qWarning() << "Unimplemented";
    }
}
