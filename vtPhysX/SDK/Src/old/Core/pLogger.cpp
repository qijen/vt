#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pLogger.h"
#include "pErrorStream.h"


static pErrorStream gErrorStream;

pLogger::pLogger()
{

	mErrorStream = &gErrorStream;

}

