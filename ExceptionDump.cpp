/*
 * ExceptionDump.cpp
 *
 *  Created on: 2015. nov. 30.
 *      Author: peti
 */

#include "ExceptionDump.h"
#include <core_cm4.h>
#include <diag/Trace.h>
#include "Log.h"
#include "../Protocol/ProtocolParser.h"
#include <LCD/Console.h>

#if defined(TRACE)

LOG_TAG(EXCEPTION);

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

// The values of BFAR and MMFAR stay unchanged if the BFARVALID or
// MMARVALID is set. However, if a new fault occurs during the
// execution of this fault handler, the value of the BFAR and MMFAR
// could potentially be erased. In order to ensure the fault addresses
// accessed are valid, the following procedure should be used:
// 1. Read BFAR/MMFAR.
// 2. Read CFSR to get BFARVALID or MMARVALID. If the value is 0, the
//    value of BFAR or MMFAR accessed can be invalid and can be discarded.
// 3. Optionally clear BFARVALID or MMARVALID.
// (See Joseph Yiu's book).

void logExceptionStack(const char* cause, ExceptionStackFrame* frame, uint32_t cfsr,
		uint32_t mmfar, uint32_t bfar, uint32_t lr) {
  Log::Error(EXCEPTION, cause);
  Log::Error(EXCEPTION, "Stack frame:");
  Log::ErrorF(EXCEPTION, " R0 =  %08X", 0, frame->r0);
  Log::ErrorF(EXCEPTION, " R1 =  %08X", 0, frame->r1);
  Log::ErrorF(EXCEPTION, " R2 =  %08X", 0, frame->r2);
  Log::ErrorF(EXCEPTION, " R3 =  %08X", 0, frame->r3);
  Log::ErrorF(EXCEPTION, " R12 = %08X", 0, frame->r12);
  Log::ErrorF(EXCEPTION, " LR =  %08X", 0, frame->lr);
  Log::ErrorF(EXCEPTION, " PC =  %08X", 0, frame->pc);
  Log::ErrorF(EXCEPTION, " PSR = %08X", 0, frame->psr);
  Log::Error(EXCEPTION, "FSR/FAR:");
  Log::ErrorF(EXCEPTION, " CFSR =  %08X", 0, cfsr);
  Log::ErrorF(EXCEPTION, " HFSR =  %08X", 0, SCB->HFSR);
  Log::ErrorF(EXCEPTION, " DFSR =  %08X", 0, SCB->DFSR);
  Log::ErrorF(EXCEPTION, " AFSR =  %08X", 0, SCB->AFSR);

  if (cfsr & (1UL << 7))
    {
      Log::ErrorF(EXCEPTION, " MMFAR = %08X", 0, mmfar);
    }
  if (cfsr & (1UL << 15))
    {
      Log::ErrorF(EXCEPTION, " BFAR =  %08X", 0, bfar);
    }
  Log::Error(EXCEPTION, "Misc");
  Log::ErrorF(EXCEPTION, " LR/EXC_RETURN= %08X", 0, lr);
  // dump everything
  Log::getInstance()->handler(ProtocolParser::getExistingInstance()->getInterface());
  Console::getInstance()->writeLine(cause);
}

#endif // defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

#endif // defined(TRACE)
