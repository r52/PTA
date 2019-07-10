#pragma once

enum pta_log_level_t
{
    PTA_LOG_DEBUG,   //!< Debug level.
    PTA_LOG_INFO,    //!< Info level.
    PTA_LOG_WARNING, //!< Warning level.
    PTA_LOG_CRITICAL //!< Critical level.
};

// Config defs
constexpr auto PTA_CONFIG_LOGLEVEL = "global/loglevel";
constexpr auto PTA_CONFIG_LOGFILE  = "global/logfile";

constexpr auto PTA_CONFIG_PRICE_TEMPLATE  = "ui/pricetemplate";
constexpr auto PTA_CONFIG_TEMPLATE_WIDTH  = "ui/tempwidth";
constexpr auto PTA_CONFIG_TEMPLATE_HEIGHT = "ui/tempheight";

constexpr auto PTA_CONFIG_SIMPLE_CHECK_HOTKEY         = "hotkey/simplecheck";
constexpr auto PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED = "hotkey/simplecheck/enabled";
constexpr auto PTA_CONFIG_ADV_CHECK_HOTKEY            = "hotkey/advcheck";
constexpr auto PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED    = "hotkey/advcheck/enabled";

constexpr auto PTA_CONFIG_LEAGUE             = "pricecheck/league";
constexpr auto PTA_CONFIG_DISPLAYLIMIT       = "pricecheck/displaylimit";
constexpr auto PTA_CONFIG_CORRUPTOVERRIDE    = "pricecheck/corruptoverride";
constexpr auto PTA_CONFIG_CORRUPTSEARCH      = "pricecheck/corruptsearch";
constexpr auto PTA_CONFIG_PRIMARY_CURRENCY   = "pricecheck/primarycurrency";
constexpr auto PTA_CONFIG_SECONDARY_CURRENCY = "pricecheck/secondarycurrency";
constexpr auto PTA_CONFIG_ONLINE_ONLY        = "pricecheck/online";
constexpr auto PTA_CONFIG_BUYOUT_ONLY        = "pricecheck/buyout";
constexpr auto PTA_CONFIG_REMOVE_DUPES       = "pricecheck/duplicates";
constexpr auto PTA_CONFIG_PREFILL_MIN        = "pricecheck/prefillmin";
constexpr auto PTA_CONFIG_PREFILL_MAX        = "pricecheck/prefillmax";
constexpr auto PTA_CONFIG_PREFILL_WEAPON     = "pricecheck/prefillweapon";
constexpr auto PTA_CONFIG_PREFILL_ARMOUR     = "pricecheck/prefillarmour";
constexpr auto PTA_CONFIG_PREFILL_NORMALS    = "pricecheck/prefillnormals";
constexpr auto PTA_CONFIG_PREFILL_PSEUDOS    = "pricecheck/prefillpseudos";
constexpr auto PTA_CONFIG_PREFILL_ILVL       = "pricecheck/prefillilvl";
constexpr auto PTA_CONFIG_PREFILL_BASE       = "pricecheck/prefillbase";

// defaults
constexpr auto PTA_CONFIG_DEFAULT_PRICE_TEMPLATE  = "templates/price/index.html";
constexpr auto PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH  = 600;
constexpr auto PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT = 450;

constexpr auto PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY = "Ctrl+D";
constexpr auto PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY    = "Ctrl+Alt+D";

constexpr auto PTA_CONFIG_DEFAULT_LEAGUE             = 0;
constexpr auto PTA_CONFIG_DEFAULT_DISPLAYLIMIT       = 12;
constexpr auto PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE    = false;
constexpr auto PTA_CONFIG_DEFAULT_CORRUPTSEARCH      = "Any";
constexpr auto PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY   = "chaos";
constexpr auto PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY = "exa";
constexpr auto PTA_CONFIG_DEFAULT_ONLINE_ONLY        = true;
constexpr auto PTA_CONFIG_DEFAULT_BUYOUT_ONLY        = true;
constexpr auto PTA_CONFIG_DEFAULT_REMOVE_DUPES       = true;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_MIN        = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_MAX        = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_WEAPON     = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_ARMOUR     = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_NORMALS    = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_PSEUDOS    = true;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_ILVL       = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_BASE       = false;
#define PTA_CONFIG_DEFAULT_LOGLEVEL PTA_LOG_DEBUG
