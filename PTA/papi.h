#pragma once

#include <QJsonObject>
#include <QObject>
#include <QVariant>

QT_FORWARD_DECLARE_CLASS(QNetworkAccessManager)

class PItem;

class PAPI : public QObject
{
    Q_OBJECT

public:
    PAPI(QObject* parent = nullptr);

    QVariantMap& getStats() { return m_stats; }

    void simplePriceCheck(PItem* item);

private:
    QNetworkAccessManager* m_manager;

    QVariantMap                m_leagues;
    QVariantMap                m_stats;
    QMap<QString, QJsonObject> m_uniques;
};
