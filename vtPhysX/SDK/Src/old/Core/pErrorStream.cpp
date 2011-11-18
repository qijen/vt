#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pErrorStream.h"

namespace vtAgeia
{


	
void pErrorStream::reportError(NxErrorCode e, const char* message, const char* file, int line)
{

	xLogger::xLog(ELOGERROR,E_LI_AGEIA,message);

}
NxAssertResponse pErrorStream::reportAssertViolation(const char* message, const char* file, int line)
{
	return NX_AR_CONTINUE;

}
void pErrorStream::print(const char* message)
{

	xLogger::xLog(ELOGERROR,E_LI_AGEIA,message);

}

}
