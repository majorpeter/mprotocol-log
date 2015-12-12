#include <Log/StdioLog.h>
#include <Log/BufferedLog.h>
#include "Log.h"

#if LOG_MODE == LOG_MODE_STDIO
#include "Protocol/StdioSerialInterface.h"
#endif

#if LOG_MODE == LOG_MODE_SERIAL
#include "Protocol/VcpSerialInterface.h"
#endif

#include "BufferedLog.h"

Log* Log::getInstance() {
    static Log* instance = NULL;
    if (instance == NULL) {
#if LOG_MODE == LOG_MODE_STDIO
        instance = new BufferedLog(new StdioSerialInterface());
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
