/*
 * ExceptionDump.h
 *
 *  Created on: 2015. nov. 30.
 *      Author: peti
 */

#ifndef LOG_EXCEPTIONDUMP_H_
#define LOG_EXCEPTIONDUMP_H_

#include <stdint.h>
#include "cortexm/ExceptionHandlers.h"

#ifdef __cplusplus
extern "C" {
#endif

void logExceptionStack(const char* cause, ExceptionStackFrame* frame, uint32_t cfsr, uint32_t mmfar, uint32_t bfar, uint32_t lr);

#ifdef __cplusplus
}
#endif

#endif /* LOG_EXCEPTIONDUMP_H_ */
