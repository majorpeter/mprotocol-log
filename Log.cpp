#include <Log/StdioLog.h>
#include "Log.h"

Log* Log::getInstance() {
    static Log* instance = NULL;
    if (instance == NULL) {
#if LOG_MODE == LOG_MODE_ITM
        instance = new StdioLog();
#else
        instance = new Log();
#endif
    }
    return instance;
}
