#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include <string.h>
#include <stdint.h>

#include "config.h"
#include "Tag.h"

enum class LogLevel_t {
    Notice,
    Warning,
    Error
};

class Log {
protected:
    Log() {}
public:
    static Log* getInstance();
    virtual ~Log() {}

    virtual void handler() {}

    // dummy function for disabled logger
    virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0) {}

    // these functions are inline calls, much like defines in C
    static void Notice(LogTag_t tag, const char* message = NULL, uint32_t parameter = 0) __attribute__((always_inline)) {
        Log::getInstance()->addLog(LogLevel_t::Notice, tag, message, parameter);
    }
    static void Warning(LogTag_t tag, const char* message = NULL, uint32_t parameter = 0) __attribute__((always_inline)) {
        Log::getInstance()->addLog(LogLevel_t::Warning, tag, message, parameter);
    }
    static void Error(LogTag_t tag, const char* message = NULL, uint32_t parameter = 0) __attribute__((always_inline)) {
        Log::getInstance()->addLog(LogLevel_t::Error, tag, message, parameter);
    }
};

#endif /* LOG_LOG_H_ */
