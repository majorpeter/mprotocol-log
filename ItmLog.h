#ifndef LOG_ITMLOG_H_
#define LOG_ITMLOG_H_

#include "Log.h"

class ItmLog: public Log {
public:
    ItmLog();
    virtual ~ItmLog();
    virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0);
};

#endif /* LOG_ITMLOG_H_ */
