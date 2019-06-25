#pragma once

#include "pitem.h"

#include <map>
#include <memory>

#include <nlohmann/json.hpp>

#include <QObject>
#include <QVariant>

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)

using json = nlohmann::json;

class PAPI : public QObject
{
    Q_OBJECT

public:
    PAPI(QObject* parent = nullptr);

    json& getStats() { return m_stats; }

    void simplePriceCheck(std::shared_ptr<PItem> item);

signals:
    void humour(QString msg);
    void priceCheckFinished(std::shared_ptr<PItem> item, QString results);

private:
    QNetworkAccessManager* m_manager;

    json                        m_leagues;
    json                        m_stats;
    std::map<std::string, json> m_uniques;
};
