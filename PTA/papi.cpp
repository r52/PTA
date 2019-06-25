#include "papi.h"

#include "pitem.h"

#include <QApplication>
#include <QClipboard>
#include <QJsonArray>
#include <QJsonDocument>
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

        m_leagues = QJsonDocument::fromJson(rp_league->readAll()).object().toVariantMap();
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

        m_stats = QJsonDocument::fromJson(rp_stats->readAll()).object().toVariantMap();
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
        auto obj = QJsonDocument::fromJson(rp_uniq->readAll()).object();

        auto ll = obj["result"].toArray();

        for (auto type : ll)
        {
            auto tob = type.toObject();
            auto el  = tob["entries"].toArray();

            for (auto et : el)
            {
                auto etob = et.toObject();

                if (etob.contains("name"))
                {
                    m_uniques[etob["name"].toString()] = etob;
                }
                else if (etob.contains("disc"))
                {
                    // only check against newest maps
                    if (etob["disc"].toString() == "warfortheatlas")
                    {
                        m_uniques[etob["type"].toString()] = etob;
                    }
                }
                else
                {
                    // gems and stuff
                    m_uniques[etob["type"].toString()] = etob;
                }
            }
        }
    });
}

void PAPI::simplePriceCheck(std::shared_ptr<PItem> item)
{
    QByteArray q = QByteArrayLiteral("{"
                                     "\"query\": {"
                                     "\"status\": {"
                                     "\"option\": \"online\""
                                     "},"
                                     "\"stats\": [{"
                                     "\"type\": \"and\","
                                     "\"filters\": []"
                                     "}]"
                                     "},"
                                     "\"sort\": {"
                                     "\"price\" : \"asc\""
                                     "}"
                                     "}");

    QJsonObject query = QJsonDocument::fromJson(q).object();

    // Check for unique items
    if (m_uniques.contains(item->m_name))
    {
        auto entry = m_uniques[item->m_name];

        auto qe = query["query"].toObject();

        if (entry.contains("disc"))
        {
            // map entry
            if (entry.contains("name"))
            {
                qe["name"] = QJsonObject{{"discriminator", entry["disc"].toString()}, {"option", entry["name"].toString()}};
            }

            qe["type"] = QJsonObject{{"discriminator", entry["disc"].toString()}, {"option", entry["type"].toString()}};
        }
        else if (entry.contains("name"))
        {
            // generic unique with name
            qe["name"] = entry["name"].toString();
            qe["type"] = entry["type"].toString();
        }
        else
        {
            // only type
            qe["type"] = entry["type"].toString();
        }

        query["query"] = qe;

        // auto qba = QJsonDocument(query).toJson();
        auto qba = QJsonDocument(query).toJson();

        // TODO: Fix league
        QNetworkRequest request;
        request.setUrl(QUrl("https://www.pathofexile.com/api/trade/search/Legion"));
        request.setRawHeader("Content-Type", "application/json");

        auto req = m_manager->post(request, qba);
        connect(req, &QNetworkReply::finished, [=]() {
            req->deleteLater();

            if (req->error() != QNetworkReply::NoError)
            {
                qWarning() << "PAPI: Error querying trade site" << req->error() << req->errorString();
                return;
            }

            auto respdata = req->readAll();
            auto resp     = QJsonDocument::fromJson(respdata).object();
            if (!resp.contains("result") || !resp.contains("id"))
            {
                qWarning() << "PAPI: Error querying trade site";
                qWarning() << "PAPI: Site responded with" << respdata;
                return;
            }

            QStringList fetchcodes;
            auto        flist = resp["result"].toArray();
            auto        len   = flist.count();

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
                fetchcodes.append(flist.at(i).toString());
            }

            QString fcode = fetchcodes.join(',');

            QString resUrl = QString("https://www.pathofexile.com/api/trade/fetch/%1?query=%2").arg(fcode).arg(resp["id"].toString());

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
        qWarning() << "Unimplemented";
    }
}
