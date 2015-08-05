#include <Log/StdioLog.h>
#include <stdio.h>

#define TARGET_STREAM stderr

StdioLog::StdioLog() {}

StdioLog::~StdioLog() {}

void StdioLog::addLog(LogLevel_t level, LogTag_t tag, const char* message, uint32_t parameter) {
    switch(level) {
    case LogLevel_t::Notice:  fputs("N/", TARGET_STREAM); break;
    case LogLevel_t::Warning: fputs("W/", TARGET_STREAM); break;
    case LogLevel_t::Error:   fputs("E/", TARGET_STREAM); break;
    }
    //TODO fprintf(TARGET_STREAM, "%d ", time);
    fprintf(TARGET_STREAM, "%08lX[", parameter);
    fputs(tag, TARGET_STREAM);
    fputc(']', TARGET_STREAM);
    if (message != NULL) {
        fputs(message, TARGET_STREAM);
    }
    fputc('\n', TARGET_STREAM);
}
