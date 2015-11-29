/*
 * BufferedLog.h
 *
 *  Created on: 2015. okt. 26.
 *      Author: peti
 */

#ifndef BUFFEREDLOG_H_
#define BUFFEREDLOG_H_

#include "Log.h"
#include "Nodes/Node.h"
#include "Time/Timer.h"

class AbstractSerialInterface;

typedef struct {
	LogLevel_t level;
	LogTag_t tag;
	Timer::ticks_t tick;
	char *message;
	uint16_t messageLength;
	uint32_t parameter;
} LogEntry_t;

class BufferedLog: public Log, public Node {
	DECLARE_PROP_UINT32_RO(EntryOverrun);
	DECLARE_PROP_UINT32_RO(MessageOverrun);
	DECLARE_PROP_METHOD(TestLog);
public:
	BufferedLog(AbstractSerialInterface *serialIface);
	virtual ~BufferedLog();
	virtual void addLog(LogLevel_t level, LogTag_t tag, const char* message = NULL, uint32_t parameter = 0);
	virtual void handler();
    virtual void switchSerialInterface(AbstractSerialInterface *interface);
private:
	AbstractSerialInterface *serialIface;
	LogEntry_t *entries;
	/// entries circular buffer management
	uint8_t firstEntryIndex, nextEntryIndex;
	/// circular buffer for text messages
	char *messageBuffer;
	char *messagesBegin, *messagesEnd;

	uint16_t entryOverrunCount, messageOverrunCount;
};

#endif /* BUFFEREDLOG_H_ */
