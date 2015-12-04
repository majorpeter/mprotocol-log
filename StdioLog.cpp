#include <Log/StdioLog.h>
#include <Time/Timer.h>
#include <stdio.h>

StdioLog::StdioLog() {}

StdioLog::~StdioLog() {}

void StdioLog::addLog(LogLevel_t level, LogTag_t tag, const char* message, uint32_t parameter) {
    switch(level) {
    case LogLevel_t::Notice:  fputs("LN/", stdout); break;
    case LogLevel_t::Warning: fputs("LW/", stdout); break;
    case LogLevel_t::Error:   fputs("LE/", stdout); break;
    }
    fprintf(stdout, "%08lX:", Timer::getTime());
    fprintf(stdout, "%08lX[", parameter);
    fputs(tag, stdout);
    fputc(']', stdout);
    if (message != NULL) {
        fputs(message, stdout);
    }
    fputc('\n', stdout);
}
