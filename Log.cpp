#include <Log/StdioLog.h>
#include <Log/BufferedLog.h>
#include "Log.h"

Log* Log::getInstance() {
    static Log* instance = NULL;
    if (instance == NULL) {
#if LOG_MODE == LOG_MODE_ITM
        instance = new StdioLog();
#else
  #if LOG_MODE == LOG_MODE_SERIAL
        instance = new BufferedLog(VcpSerialInterface::getInstance());
  #else
        instance = new Log();
  #endif
#endif
    }
    return instance;
}
