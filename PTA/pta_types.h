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

#define PTA_CONFIG_DEFAULT_LOGLEVEL PTA_LOG_DEBUG
