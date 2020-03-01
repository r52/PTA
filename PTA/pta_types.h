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

// constexpr auto PTA_CONFIG_PRICE_TEMPLATE = "ui/pricetemplate";

constexpr auto PTA_CONFIG_SIMPLE_CHECK_HOTKEY         = "hotkey/simplecheck";
constexpr auto PTA_CONFIG_SIMPLE_CHECK_HOTKEY_ENABLED = "hotkey/simplecheck/enabled";
constexpr auto PTA_CONFIG_ADV_CHECK_HOTKEY            = "hotkey/advcheck";
constexpr auto PTA_CONFIG_ADV_CHECK_HOTKEY_ENABLED    = "hotkey/advcheck/enabled";
constexpr auto PTA_CONFIG_WIKI_HOTKEY                 = "hotkey/wiki";
constexpr auto PTA_CONFIG_WIKI_HOTKEY_ENABLED         = "hotkey/wiki/enabled";
constexpr auto PTA_CONFIG_CTRL_SCROLL_HOTKEY_ENABLED  = "hotkey/cscroll/enabled";

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
constexpr auto PTA_CONFIG_PREFILL_RANGE      = "pricecheck/prefillrange";
constexpr auto PTA_CONFIG_PREFILL_NORMALS    = "pricecheck/prefillnormals";
constexpr auto PTA_CONFIG_PREFILL_PSEUDOS    = "pricecheck/prefillpseudos";
constexpr auto PTA_CONFIG_PREFILL_ILVL       = "pricecheck/prefillilvl";
constexpr auto PTA_CONFIG_PREFILL_BASE       = "pricecheck/prefillbase";

constexpr auto PTA_CONFIG_CUSTOM_MACROS = "macro/list";

constexpr auto PTA_CONFIG_CLIENTLOG_PATH = "client/path";

// defaults
constexpr auto PTA_CONFIG_DEFAULT_PRICE_TEMPLATE  = "search/dist/index.html";
constexpr auto PTA_CONFIG_DEFAULT_TEMPLATE_WIDTH  = 820;
constexpr auto PTA_CONFIG_DEFAULT_TEMPLATE_HEIGHT = 870;

constexpr auto PTA_CONFIG_DEFAULT_SIMPLE_CHECK_HOTKEY = "Ctrl+D";
constexpr auto PTA_CONFIG_DEFAULT_ADV_CHECK_HOTKEY    = "Ctrl+Alt+D";
constexpr auto PTA_CONFIG_DEFAULT_WIKI_HOTKEY         = "Ctrl+Alt+G";

constexpr auto PTA_CONFIG_DEFAULT_LEAGUE             = 0;
constexpr auto PTA_CONFIG_DEFAULT_DISPLAYLIMIT       = 20;
constexpr auto PTA_CONFIG_DEFAULT_CORRUPTOVERRIDE    = false;
constexpr auto PTA_CONFIG_DEFAULT_CORRUPTSEARCH      = "Any";
constexpr auto PTA_CONFIG_DEFAULT_PRIMARY_CURRENCY   = "chaos";
constexpr auto PTA_CONFIG_DEFAULT_SECONDARY_CURRENCY = "exa";
constexpr auto PTA_CONFIG_DEFAULT_ONLINE_ONLY        = true;
constexpr auto PTA_CONFIG_DEFAULT_BUYOUT_ONLY        = true;
constexpr auto PTA_CONFIG_DEFAULT_REMOVE_DUPES       = true;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_MIN        = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_MAX        = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_RANGE      = 0;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_NORMALS    = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_PSEUDOS    = true;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_ILVL       = false;
constexpr auto PTA_CONFIG_DEFAULT_PREFILL_BASE       = false;

enum MacroType : uint8_t
{
    MACRO_TYPE_CHAT = 0,
    MACRO_TYPE_URL,
    MACRO_TYPE_MAX
};

#ifdef _DEBUG
#    define PTA_CONFIG_DEFAULT_LOGLEVEL PTA_LOG_DEBUG
#else
#    define PTA_CONFIG_DEFAULT_LOGLEVEL PTA_LOG_INFO
#endif
