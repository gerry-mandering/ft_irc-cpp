#pragma once

#if LOGGING && LOG_LEVEL <= DEBUG_LEVEL_TRACE
#define kprintf_trace(...) printf(__FILE__ ":" TO_STR(__LINE__) " [TRACE] " __VA_ARGS__)
#else
#define kprintf_trace(...)
#endif

#if LOGGING && LOG_LEVEL <= DEBUG_LEVEL_DEBUG
#define kprintf_debug(...) printf(__FILE__ ":" TO_STR(__LINE__) " [DEBUG] " __VA_ARGS__)
#else
#define kprintf_debug(...)
#endif

#if LOGGING && LOG_LEVEL <= DEBUG_LEVEL_INFO
#define kprintf_info(...) printf(__FILE__ ":" TO_STR(__LINE__) " [INFO] " __VA_ARGS__)
#else
#define kprintf_info(...)
#endif

#if LOGGING && LOG_LEVEL <= DEBUG_LEVEL_WARN
#define kprintf_warn(...) printf(__FILE__ ":" TO_STR(__LINE__) " [WARN] " __VA_ARGS__)
#else
#define kprintf_warn(...)
#endif

#if LOGGING && LOG_LEVEL <= DEBUG_LEVEL_ERROR
#define kprintf_error(...) printf(__FILE__ ":" TO_STR(__LINE__) " [ERROR] " __VA_ARGS__)
#else
#define kprintf_error(...)
#endif