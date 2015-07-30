#include "ItmLog.h"
#include <stdio.h>

ItmLog::ItmLog() {}

ItmLog::~ItmLog() {}

void ItmLog::addLog(LogLevel_t level, LogTag_t tag, const char* message, uint32_t parameter) {
    switch(level) {
    case LogLevel_t::Notice:  fputs("N/", stdout); break;
    case LogLevel_t::Warning: fputs("W/", stdout); break;
    case LogLevel_t::Error:   fputs("E/", stdout); break;
    }
    //TODO fprintf(stdout, "%d ", time);
    fprintf(stdout, "%08lX[", parameter);
    fputs(tag, stdout);
    fputc(']', stdout);
    if (message != NULL) {
        fputs(message, stdout);
    }
    fputc('\n', stdout);
}
