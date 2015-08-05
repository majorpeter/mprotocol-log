#ifndef LOG_STDIOLOG_H_
#define LOG_STDIOLOG_H_

#include "Log.h"

class StdioLog: public Log {
public:
    StdioLog();
    virtual ~StdioLog();
    virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0);
};

#endif /* LOG_STDIOLOG_H_ */
