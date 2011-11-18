#include <StdAfx.h>
#include "vtPhysXBase.h"
#include "xAssertion.h"

#include "xAssertCustomization.h"

//----------------------------------------------------------------
//
// global data
//
CAssertionFailedProcedure xAssertionEx::g_fnAssertFailureHandler = NULL;
xErrorHandlerFn xAssertionEx::g_fnAssertHandler = NULL;

xAssertionEx* gAssertionEx = NULL;

//----------------------------------------------------------------
//
// 
//

xAssertionEx*xAssertionEx::Instance()
{
	if (!gAssertionEx)
		gAssertionEx = &xAssertionEx();

	return gAssertionEx;
}

xAssertionEx::xAssertionEx()
{
	gAssertionEx = this;
}

/*
xErrorHandlerFn xAssertionEx::getErrorHandler()
{
	return g_fnAssertHandler;
}

void xAssertionEx::setErrorHandler(xErrorHandlerFn fnFailureProcedure)
{
	g_fnAssertHandler = fnFailureProcedure;
}
*/
//----------------------------------------------------------------
//
// 
//
void assert_notify_failure(const char* str)
{
	xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,str);
	//std::cerr << str << " failed" << std::endl;
}

