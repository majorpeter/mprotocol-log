#ifndef LOG_ITMLOG_H_
#define LOG_ITMLOG_H_

#include "Log.h"

/**
 * simple log interface that uses library functions to print data to STDOUT stream
 * @note this is ITM in the embedded project
 */
class StdioLog: public Log {
public:
    StdioLog();
    virtual ~StdioLog();
    virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0);
};

#endif /* LOG_ITMLOG_H_ */
