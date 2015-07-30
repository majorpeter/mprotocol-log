#include "Log.h"
#include "ItmLog.h"

Log* Log::getInstance() {
    static Log* instance = NULL;
    if (instance == NULL) {
#if LOG_MODE == LOG_MODE_ITM
        instance = new ItmLog();
#else
        instance = new Log();
#endif
    }
    return instance;
}
