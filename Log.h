#ifndef LOG_LOG_H_
#define LOG_LOG_H_

#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "config.h"
#include "Tag.h"

class AbstractSerialInterface;

enum class LogLevel_t {
    Notice,
    Warning,
    Error
};

class Log {
public:
    static Log* getInstance();
    virtual ~Log() {}

    virtual void handler(AbstractSerialInterface* interface) {}

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
    static void ErrorF(LogTag_t tag, const char* format = NULL, uint32_t parameter = 0, ...) {
    	va_list args;
    	va_start(args, parameter);
        Log::getInstance()->addLogFormatted(LogLevel_t::Error, tag, format, parameter, args);
        va_end(args);
    }

    // this can be used to switch interface to BT from USB after BT connection
    virtual void switchSerialInterface(AbstractSerialInterface *interface) {}
protected:
    Log() {}

    // dummy function for disabled logger, override it in actual implementations
    virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0) {}

    void addLogFormatted(LogLevel_t level, LogTag_t tag, const char* format, uint32_t parameter, va_list args) {
    	char buffer[100];
    	vsnprintf(buffer, 100, format, args);
    	this->addLog(level, tag, buffer, parameter);
    }
};

#endif /* LOG_LOG_H_ */
