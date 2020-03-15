#pragma once

#include "pitem.h"

#include <map>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include <QMap>
#include <QNetworkAccessManager>
#include <QObject>
#include <QTextStream>
#include <QVector>

using json = nlohmann::json;

class ItemAPI : public QObject
{
    Q_OBJECT

public:
    ItemAPI(QNetworkAccessManager* netmanager, QObject* parent = nullptr);

    const json getLeagues() { return m_leagues; }
    QString    getLeague();

    bool parse(Item& item, QString itemText);
    void fillItemOptions(json& data);

    void openWiki(const Item& item);

    bool trySimplePriceCheck(json data, bool openui, bool forcetab);

public slots:
    void advancedPriceCheck(const QString& str, bool openonsite);
    void emitUIReady(const QString& str, bool forcetab);

signals:
    void humour(const QString& msg);
    void openSearchUI(const QString& results);
    void priceCheckFinished(const QString& results);
    void predictionReady(const QString& results);
    void searchUIReady(const QString& str, bool forcetab);

private:
    int         readPropInt(QString prop);
    json        readPropIntRange(QString prop);
    double      readPropFloat(QString prop);
    json        readSockets(QString prop);
    std::string readName(QString name);
    std::string readType(Item& item, QString type);

    void captureNumerics(QString line, QRegularExpression& re, json& val, std::vector<QString>& captured);

    void parseProp(Item& item, QString prop);
    bool parseStat(Item& item, QString stat, QTextStream& stream);

    void processPriceResults(json data, json response, const QString& optstr, const QString& format, bool openui, bool forcetab);

    void doCurrencySearch(json& data, bool openui, bool forcetab);

    bool synchronizedGetJSON(const QNetworkRequest& req, json& result);

    enum filter_type_e : uint8_t
    {
        weapon_filter = 0,
        armour_filter,
        socket_filter,
        req_filter,
        misc_filter,
        special_filter // manual parse
    };

    enum weapon_filters_e : uint8_t
    {
        weapon_filter_aps = 0,
        weapon_filter_crit,
        weapon_filter_pdps,
        weapon_filter_edps
    };

    enum armour_filters_e : uint8_t
    {
        armour_filter_ar = 0,
        armour_filter_ev,
        armour_filter_es,
        armour_filter_block
    };

    enum req_filters_e : uint8_t
    {
        req_filter_lvl = 0,
        req_filter_str,
        req_filter_dex,
        req_filter_int
    };

    enum misc_filters_e : uint8_t
    {
        misc_filter_quality = 0,
        misc_filter_ilvl,
        misc_filter_gem_level,
        misc_filter_gem_level_progress,
        misc_filter_shaper_item,
        misc_filter_elder_item,
        misc_filter_synthesised_item,
        misc_filter_identified,
        misc_filter_corrupted,
        misc_filter_mirrored,
        misc_filter_crafted,
        misc_filter_veiled,
        misc_filter_enchanted,
        misc_filter_talisman_level,
        misc_filter_map_tier
    };

    enum misc_influences_e : uint8_t
    {
        shaper = 0,
        elder,
        crusader,
        redeemer,
        hunter,
        warlord,
        influences_max
    };

    enum class mod_generation_type : uint8_t
    {
        mod_prefix = 0,
        mod_suffix,
        mod_unknown
    };

    const std::array<std::string, influences_max> c_influenceMap = {"shaper", "elder", "crusader", "redeemer", "hunter", "warlord"};

    const QMap<QString, QVector<uint8_t>> c_propMap = {{"Quality", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Elemental Damage)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Caster Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Attack Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Defence Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Life and Mana Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Resistance Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Quality (Attribute Modifiers)", {misc_filter, misc_filter_quality}},
                                                       {"Evasion Rating", {armour_filter, armour_filter_ev}},
                                                       {"Energy Shield", {armour_filter, armour_filter_es}},
                                                       {"Armour", {armour_filter, armour_filter_ar}},
                                                       {"Chance to Block", {armour_filter, armour_filter_block}},
                                                       {"Requirements", {special_filter}},
                                                       {"Level", {special_filter}},
                                                       {"req_level", {req_filter, req_filter_lvl}},
                                                       {"gem_level", {misc_filter, misc_filter_gem_level}},
                                                       {"Str", {req_filter, req_filter_str}},
                                                       {"Dex", {req_filter, req_filter_dex}},
                                                       {"Int", {req_filter, req_filter_int}},
                                                       {"Sockets", {socket_filter}},
                                                       {"Item Level", {misc_filter, misc_filter_ilvl}},
                                                       {"Physical Damage", {weapon_filter, weapon_filter_pdps}},
                                                       {"Critical Strike Chance", {weapon_filter, weapon_filter_crit}},
                                                       {"Attacks per Second", {weapon_filter, weapon_filter_aps}},
                                                       {"Elemental Damage", {weapon_filter, weapon_filter_edps}},
                                                       {"Experience", {misc_filter, misc_filter_gem_level_progress}},
                                                       {"Map Tier", {misc_filter, misc_filter_map_tier}}};

    json                                                             c_baseCat;
    json                                                             c_pseudoRules;
    json                                                             c_enchantRules;
    std::unordered_set<std::string>                                  c_weaponLocals;
    std::unordered_set<std::string>                                  c_armourLocals;
    std::unordered_map<std::string, std::unordered_set<std::string>> c_discriminators;
    json                                                             c_currencyMap;
    std::unordered_set<std::string>                                  c_currencyCodes;
    std::unordered_set<std::string>                                  c_excludes;
    std::unordered_map<std::string, mod_generation_type>             c_mods;

    std::map<std::string, json> c_baseMap;

    QString m_section;

    json                                       m_leagues;
    std::unordered_multimap<std::string, json> m_stats_by_text;
    std::unordered_map<std::string, json>      m_stats_by_id;
    std::unordered_multimap<std::string, json> m_uniques;

    const std::string m_mapdisc = "warfortheatlas"; // default map discriminator

    QNetworkAccessManager* m_manager;
};