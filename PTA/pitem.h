#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct mmv_t
{
    int min;
    int max;

    int avg() { return ((min + max) / 2); }

    mmv_t& operator+=(const mmv_t& b)
    {
        min += b.min;
        max += b.max;
        return *this;
    }

    mmv_t operator+(const mmv_t& b) const { return mmv_t{min + b.min, max + b.max}; }
};

struct type_filters_t
{
    std::string category;
    std::string rarity;
};

struct weapon_filters_t
{
    double aps;
    double crit;
    mmv_t  pdps;
    mmv_t  edps;

    bool  search_pdps;
    mmv_t pdps_filter;
    bool  search_edps;
    mmv_t edps_filter;
};

struct armour_filters_t
{
    int ar;
    int ev;
    int es;
    int block;

    bool  search_ar;
    mmv_t ar_filter;
    bool  search_ev;
    mmv_t ev_filter;
    bool  search_es;
    mmv_t es_filter;
};

struct socket_type_t
{
    int r;
    int g;
    int b;
    int w;
    int a;

    int total() { return r + g + b + w + a; }
};

struct socket_filters_t
{
    int           links; // max linked
    socket_type_t sockets;
};

struct req_filters_t
{
    int lvl;
    int str;
    int dex;
    int intl;
};

struct misc_filters_t
{
    int         quality;
    int         ilvl;
    int         gem_level;
    int         gem_level_progress;
    bool        shaper_item;
    bool        elder_item;
    bool        fractured_item;
    bool        synthesised_item;
    bool        identified = true;
    bool        corrupted;
    bool        mirrored;
    bool        crafted;
    bool        veiled;
    bool        enchanted;
    int         talisman_tier;
    int         map_tier;
    std::string disc;
};

struct PItem
{
    std::string m_itemtext; // original item text;

    std::string name;
    std::string type;

    type_filters_t   f_type;
    weapon_filters_t f_weapon;
    armour_filters_t f_armour;
    socket_filters_t f_socket;
    req_filters_t    f_req;
    misc_filters_t   f_misc;

    bool is_weapon;
    bool is_armour;
    bool is_prediction = false;

    bool base_has_implicits;

    json filters;
    json pseudos;

    std::string m_options; // search options
    int         m_sections = 0;
};
