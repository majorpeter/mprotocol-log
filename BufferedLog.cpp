/*
 * BufferedLog.cpp
 *
 *  Created on: 2015. okt. 26.
 *      Author: peti
 */

#include "BufferedLog.h"
#include <Protocol/AbstractSerialInterface.h>
#include <Nodes/RootNode.h>
#include <stdio.h>

#define BUFFERED_LOG_ENTRIES ((uint8_t) 20)
#define MESSAGE_BUFFER_SIZE  0x200

MK_PROP_UINT32_RO(BufferedLog, EntryOverrun, "Count of log entries lost because of full circular buffer.");
MK_PROP_UINT32_RO(BufferedLog, MessageOverrun, "Count of messages lost because of full circular buffer");
MK_PROP_METHOD(BufferedLog, TestLog, "Generate log message for testing.");

PROP_ARRAY(props) = {
		PROP_ADDRESS(BufferedLog, EntryOverrun),
		PROP_ADDRESS(BufferedLog, MessageOverrun),
		PROP_ADDRESS(BufferedLog, TestLog)
};

LOG_TAG(LOG);

BufferedLog::BufferedLog(): Node("LOG") {
	entries = new LogEntry_t[BUFFERED_LOG_ENTRIES];
	firstEntryIndex = nextEntryIndex = 0;	// no entries for now
	messageBuffer = new char[MESSAGE_BUFFER_SIZE];
	messagesBegin = messagesEnd = messageBuffer;
	entryOverrunCount = messageOverrunCount = 0;

	// set up node and attach to root
	NODE_SET_PROPS(props);
	RootNode::getInstance()->addChild(this);
}

BufferedLog::~BufferedLog() {
	delete[] entries;
	delete[] messageBuffer;
}

void BufferedLog::addLog(LogLevel_t level, LogTag_t tag, const char* message, uint32_t parameter) {
	// exit if the entry array is already full
	if ((((nextEntryIndex + 1) < BUFFERED_LOG_ENTRIES) ? nextEntryIndex + 1 : 0) == firstEntryIndex) {
		entryOverrunCount++;
		return;
	}

	// put message in the circular buffer
	bool dropMsg = false;
	size_t messageLength = strlen(message);
	if (messageLength >= MESSAGE_BUFFER_SIZE) {
		// drop message if too long for circular buffer
		dropMsg = true;
	} else {
		// check if circular buf is already full
		char *p = messagesEnd + messageLength;

		if (p < messageBuffer + MESSAGE_BUFFER_SIZE) {
			if (messagesBegin > messagesEnd && messagesBegin <= p)
				dropMsg = true;
		} else {
			p -= MESSAGE_BUFFER_SIZE;
			if (p > messagesBegin)
				dropMsg = true;
		}
	}

	char *storedMessage;
	if (dropMsg) {
		storedMessage = NULL;
		messageLength = 0;
		messageOverrunCount++;
	} else {
		storedMessage = messagesEnd;
		size_t bytesToCopy = messageLength;
		if ((messageBuffer + MESSAGE_BUFFER_SIZE) - messagesEnd >= (int) bytesToCopy) {
			memcpy(messagesEnd, message, bytesToCopy);
		} else {
			size_t bytesAtEndOfBuf = (messageBuffer + MESSAGE_BUFFER_SIZE) - messagesEnd;
			memcpy(messagesEnd, message, bytesAtEndOfBuf);
			bytesToCopy -= bytesAtEndOfBuf;
			message += bytesAtEndOfBuf;
			memcpy(messageBuffer, message, bytesToCopy);
		}
		messagesEnd += messageLength;
		if (messagesEnd >= messageBuffer + MESSAGE_BUFFER_SIZE) {
			messagesEnd -= MESSAGE_BUFFER_SIZE;
		}
	}

	entries[nextEntryIndex].level = level;
	entries[nextEntryIndex].tag = tag;
	entries[nextEntryIndex].tick = Timer::getTime();
	entries[nextEntryIndex].message = storedMessage;
	entries[nextEntryIndex].messageLength = (uint16_t) messageLength;
	entries[nextEntryIndex].parameter = parameter;
	nextEntryIndex = (uint8_t) (((nextEntryIndex + 1) < BUFFERED_LOG_ENTRIES) ? nextEntryIndex + 1 : 0);
}

void BufferedLog::handler(AbstractSerialInterface* interface) {
	for (; firstEntryIndex != nextEntryIndex; firstEntryIndex = (uint8_t) (((firstEntryIndex + 1) < BUFFERED_LOG_ENTRIES) ? firstEntryIndex + 1 : 0)) {
		switch (entries[firstEntryIndex].level) {
		case LogLevel_t::Notice:  interface->writeBytes((uint8_t*) "LN/", 3); break;
		case LogLevel_t::Warning: interface->writeBytes((uint8_t*) "LW/", 3); break;
		case LogLevel_t::Error:   interface->writeBytes((uint8_t*) "LE/", 3); break;
		}
		char buffer[10];
		sprintf(buffer, "%08lX:", entries[firstEntryIndex].tick);
		interface->writeBytes((uint8_t*) buffer, 9);
		sprintf(buffer, "%08lX[", entries[firstEntryIndex].parameter);
		interface->writeBytes((uint8_t*) buffer, 9);
		interface->writeString(entries[firstEntryIndex].tag);
		interface->writeBytes((uint8_t*)"]", 1);
		if (entries[firstEntryIndex].message != NULL) {
			if (entries[firstEntryIndex].message + entries[firstEntryIndex].messageLength < messageBuffer + MESSAGE_BUFFER_SIZE) {
				interface->writeBytes((uint8_t*) entries[firstEntryIndex].message, entries[firstEntryIndex].messageLength);
				messagesBegin += entries[firstEntryIndex].messageLength;
			} else {
				size_t bytesAtEndOfBuf = (messageBuffer + MESSAGE_BUFFER_SIZE) - entries[firstEntryIndex].message;
				interface->writeBytes((uint8_t*) entries[firstEntryIndex].message, (uint16_t) bytesAtEndOfBuf);
				entries[firstEntryIndex].messageLength = (uint16_t) (entries[firstEntryIndex].messageLength - bytesAtEndOfBuf);
				interface->writeBytes((uint8_t*) messageBuffer, entries[firstEntryIndex].messageLength);
				messagesBegin = messageBuffer + entries[firstEntryIndex].messageLength;
			}
		}
		interface->writeBytes((uint8_t*)"\n", 1);
	}
	interface->handler();
}

ProtocolResult_t BufferedLog::getEntryOverrun(uint32_t *value) const {
	*value = entryOverrunCount;
	return ProtocolResult_Ok;
}

ProtocolResult_t BufferedLog::getMessageOverrun(uint32_t *value) const {
	*value = messageOverrunCount;
	return ProtocolResult_Ok;
}

ProtocolResult_t BufferedLog::invokeTestLog(const char*) {
	Log::Notice(LOG, "test", firstEntryIndex);
	return ProtocolResult_Ok;
}
