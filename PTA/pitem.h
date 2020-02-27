#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using jptr = json::json_pointer;
using Item = json;

/*

JSON Item Schema

{
    origtext: string,
    rarity: string,
    type: string,
    name: string,
    category: string,
    quality: integer,
    ilvl: integer,
    unidentified: bool,
    corrupted: bool,

    influences: ["shaper", "crusader", ...],

    requirements: {
        lvl: integer,
        str: integer,
        dex: integer,
        int: integer
    },

    weapon: {
        aps: float,
        crit: float,
        pdps: {
            min: integer,
            max: integer
        },
        edps: {
            min: integer,
            max: integer
        }
    },

    armour: {
        ar: integer,
        ev: integer,
        es: integer,
        block: integer
    },

    sockets: {
        links: int,
        total: int,
        R: int,
        G: int,
        B: int,
        W: int,
        A: int
    },

    misc: {
        disc: string,
        synthesis: bool,
        gem_level: int,
        gem_progress: string,
        map_tier: integer
    },

    filters: {
        ...PoE mod filters...
    },

    pseudos: {
        ...PoE mod filters...
    }


}

*********************************

JSON Data Schema

{
    item: Item,

    settings: {
        ...PTA settings...
    }

    searchopts: {
        use_pdps: bool,
        use_edps: bool,
        use_ar: bool,
        use_ev: bool,
        use_es: bool,
        use_sockets: bool,
        use_links: bool,
        use_ilvl: bool,
        use_item_base: bool,
        influences: Array
    }
}

*/

constexpr auto p_enabled = "enabled";
constexpr auto p_item    = "item";
constexpr auto p_results = "results";

constexpr auto p_min = "min";
constexpr auto p_max = "max";

constexpr auto p_origtext     = "origtext";
constexpr auto p_rarity       = "rarity";
constexpr auto p_type         = "type";
constexpr auto p_name         = "name";
constexpr auto p_category     = "category";
constexpr auto p_sockets      = "sockets";
constexpr auto p_quality      = "quality";
constexpr auto p_ilvl         = "ilvl";
constexpr auto p_unidentified = "unidentified";
constexpr auto p_corrupted    = "corrupted";
constexpr auto p_influences   = "influences";
constexpr auto p_filters      = "filters";
constexpr auto p_pseudos      = "pseudos";

constexpr auto    p_requirements = "requirements";
inline const auto p_reqlvl       = "/requirements/lvl"_json_pointer;
inline const auto p_reqstr       = "/requirements/str"_json_pointer;
inline const auto p_reqdex       = "/requirements/dex"_json_pointer;
inline const auto p_reqint       = "/requirements/int"_json_pointer;

constexpr auto    p_misc   = "misc";
inline const auto p_mdisc  = "/misc/disc"_json_pointer;
inline const auto p_msynth = "/misc/synthesis"_json_pointer;
inline const auto p_mglvl  = "/misc/gem_level"_json_pointer;
inline const auto p_mgexp  = "/misc/gem_progress"_json_pointer;
inline const auto p_mmtier = "/misc/map_tier"_json_pointer;

constexpr auto    p_weapon = "weapon";
inline const auto p_waps   = "/weapon/aps"_json_pointer;
inline const auto p_wcrit  = "/weapon/crit"_json_pointer;
inline const auto p_wpdps  = "/weapon/pdps"_json_pointer;
inline const auto p_wedps  = "/weapon/edps"_json_pointer;

constexpr auto    p_armour = "armour";
inline const auto p_aar    = "/armour/ar"_json_pointer;
inline const auto p_aev    = "/armour/ev"_json_pointer;
inline const auto p_aes    = "/armour/es"_json_pointer;
inline const auto p_ablock = "/armour/block"_json_pointer;

constexpr auto p_settings = "settings";

constexpr auto    p_opts          = "searchopts";
inline const auto p_usepdps       = "/searchopts/use_pdps"_json_pointer;
inline const auto p_useedps       = "/searchopts/use_edps"_json_pointer;
inline const auto p_usear         = "/searchopts/use_ar"_json_pointer;
inline const auto p_useev         = "/searchopts/use_ev"_json_pointer;
inline const auto p_usees         = "/searchopts/use_es"_json_pointer;
inline const auto p_usesockets    = "/searchopts/use_sockets"_json_pointer;
inline const auto p_uselinks      = "/searchopts/use_links"_json_pointer;
inline const auto p_useilvl       = "/searchopts/use_ilvl"_json_pointer;
inline const auto p_usebase       = "/searchopts/use_item_base"_json_pointer;
inline const auto p_useinfluences = "/searchopts/influences"_json_pointer;
inline const auto p_usesynth      = "/searchopts/use_synthesis_base"_json_pointer;
