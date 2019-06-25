#include "itemparser.h"

#include "pitem.h"

#include <nlohmann/json.hpp>

#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>

using json = nlohmann::json;

ItemParser::ItemParser(QObject* parent) : QObject(parent)
{
    // TODO load api data
}

int ItemParser::readPropInt(QString prop)
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

mmv_t ItemParser::readPropIntRange(QString prop)
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

double ItemParser::readPropFloat(QString prop)
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

socket_filters_t ItemParser::readSockets(QString prop)
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

int ItemParser::readPropExp(QString prop)
{
    // TODO
    return 0;
}

void ItemParser::parseProp(PItem* item, QString prop)
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
                    item->f_misc.gem_level_progress = readPropInt(v);
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

void ItemParser::parseStat(PItem* item, QString stat)
{
    // TODO
}

PItem* ItemParser::parse(QString itemText)
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
        item->m_name = nametype.toStdString();
        item->setType(nametype.toStdString());
    }
    else
    {
        item->m_name = nametype.toStdString();
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

    if (item->m_type.ends_with("Map"))
    {
        item->f_type.category = "Map";
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

QString ItemParser::toJson(PItem* item)
{
    json j;

    j["name"] = item->m_name;

    auto cls = item->f_type.rarity;

    // process category
    if (item->f_type.category == "Gem" || item->f_type.category == "Card" || item->f_type.category == "Prophecy")
    {
        cls = item->f_type.category;
    }

    j["class"] = cls;

    if (item->m_name != item->m_type)
    {
        j["type"] = item->m_type;
    }

    j["sockets"] = item->f_socket.sockets.total();
    j["links"]   = item->f_socket.links;

    j["ilvl"]    = item->f_misc.ilvl;
    j["quality"] = item->f_misc.quality;

    if (item->f_type.category == "Gem")
    {
        j["gem_level"] = item->f_misc.gem_level;
    }

    if (item->f_misc.elder_item)
    {
        j["elder_item"] = true;
    }

    if (item->f_misc.shaper_item)
    {
        j["shaper_item"] = true;
    }

    j["identified"] = item->f_misc.identified;

    if (!item->m_options.empty())
    {
        j["options"] = item->m_options;
    }

    return QString::fromStdString(j.dump());
}
