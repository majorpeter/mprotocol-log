#ifndef LOG_TAG_H_
#define LOG_TAG_H_

typedef const char* LogTag_t;

#define LOG_TAG(_NAME_) static LogTag_t _NAME_ = #_NAME_

#endif /* LOG_TAG_H_ */
