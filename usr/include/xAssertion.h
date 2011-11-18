/********************************************************************
	created:	2009/05/01
	created:	1:5:2009   19:35
	filename: 	x:\ProjectRoot\svn\local\usr\include\xAssertion.h
	file path:	x:\ProjectRoot\svn\local\usr\include 
	file base:	xAssertion
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	
*********************************************************************/
#ifndef __X_ASSERTION_H__
#define __X_ASSERTION_H__



#include "xPlatform.h"
#include "xLogger.h"
#include "xAssertCustomization.h"

extern void assert_notify_failure(const char* str); // may also take
extern xAssertInfo* getLastAssertInfo();
extern xAssertInfo* assertFailed();


bool TestCustomizations_Asserts();
bool customizeAsserts();

#include "vtPhysXBase.h"




#ifdef _DEBUG
	#include <assert.h>
#endif




void xCONVENTION_CALLBACK updateAssertHandlerData(
	E_ASSERTION_FAILURE_SEVERITY _failureSeverity,
	char *_assertionExpression,
	char *_assertionFileName,
	int _assertionSourceLine,
	char *_assertionPostMessage,
	void* _postAction,
	bool _result);

void xCONVENTION_CALLBACK tickAssertHandlerData();




//----------------------------------------------------------------
//
//	Assertion checks customization
//
typedef void (xCONVENTION_CALLBACK *CAssertionFailedProcedure)(
	E_ASSERTION_FAILURE_SEVERITY fsFailureSeverity, 
	const char *szAssertionExpression, 
	const char *szAssertionFileName, 
	unsigned int uiAssertionSourceLine
	);

//typedef void (xCONVENTION_CALLBACK *CPostCallback)(); 

typedef void (xCONVENTION_CALLBACK *xErrorHandlerFn)
	(
	E_ASSERTION_FAILURE_SEVERITY m_fsFailureSeverity,
	char *m_szAssertionExpression,
	char *m_szAssertionFileName,
	int m_uiAssertionSourceLine,
	char *szAssertionPostMessage,
	void* postAction,
	bool result
	);




class xAssertionEx
{
public:

	static CAssertionFailedProcedure getAssertFailureCustomHandler()
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"invoked");
//		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Null");
	return g_fnAssertFailureHandler;
	}
	static void CustomizeAssertionChecks(CAssertionFailedProcedure fnAssertionFailureProcedure)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"invoked");
//		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Null");
		g_fnAssertFailureHandler = fnAssertionFailureProcedure;
	}


	
	//xErrorHandlerFn  xCONVENTION_API getErrorHandler();


	//----------------------------------------------------------------
	//
	// 
	//
	static  xErrorHandlerFn getErrorHandler()
	{
		///xAssertInfo *testData  = getLastAssertInfo();
		//xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"invoked");
		return g_fnAssertHandler;
	}

	static void updateErrorHandler(xErrorHandlerFn fnFailureProcedure)
	{
		g_fnAssertHandler = fnFailureProcedure;
		//xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"invoked");

	}
	
	//void xCONVENTION_API setErrorHandler(xErrorHandlerFn fnFailureProcedure);


	static void swapBuffer(xAssertInfo&newData);
	static xAssertionEx *Instance();
	xAssertionEx();

private:
	
	static CAssertionFailedProcedure g_fnAssertFailureHandler;
	static xErrorHandlerFn g_fnAssertHandler;


};	

#define xAssertRouted(Assertion) if(Assertion){}else assert_notify_failure(#Assertion)

#define XASSERT_HANDLER(Assertion) (false || (Assertion) \
	|| (xAssertionEx::getAssertFailureCustomHandler() \
	&& (xAssertionEx::getAssertFailureCustomHandler()( \
	AFS_ASSERT, #Assertion, __FILE__, __LINE__), true)))

#define xCHECK_HANDLER(Assertion) ((bConditionValue = false || (Assertion)\
	|| (xAssertionEx::getAssertFailureCustomHandler() \
	&& (xAssertionEx::getAssertFailureCustomHandler()( \
	AFS_CHECK,  #Assertion, __FILE__, __LINE__), true)))



#define _xAssertHandler(Assertion) (false || (Assertion) || \
	(xAssertionEx::getErrorHandler() &&\
	(xAssertionEx::getErrorHandler()( AFS_ASSERT, #Assertion, __FILE__, __LINE__,"asdasd",NULL,TRUE), true)))


/*#define _xAssert(Assertion) (false || (Assertion) 
	|| (xAssertionEx::getErrorHandler() \
	&& (xAssertionEx::getErrorHandler()( \
	AFS_ASSERT, #Assertion, __FILE__, __LINE__,"","",FALSE), true)))
*/


#define xAssert(Assertion) xAssertRouted(XASSERT_HANDLER(Assertion))

#define xVerify(Assertion) xAssert(Assertion)

#define xCheck(Assertion) { \
	bool bConditionValue; \
	assert(xCHECK_HANDLER(Assertion)); \
	(void)(bConditionValue || (*(int *)0 = 0)); \


#define xVerify(Assertion) _xAssertHandler(Assertion)


/*
#define xVerify(Assertion) { \
	bool bConditionValue=False; \
	xAssert(xVerifyHandlerEx(Assertion)); \
	(void)(bConditionValue || (*(int *)0 = 0)); \
}

*/


/*

#define xVerify(Assertion) { \
	bool bConditionValue; \
	assert(xVerifyHandlerEx(Assertion)); \
	(void)(bConditionValue || (*(int *)0 = 0)); \
}




//#define xAssert(Assertion) assert(XASSERT_HANDLER(Assertion))

//#define xVerify(Assertion) assert(xVerifyHandlerEx(Assertion))

/*




/*	

//extern xErrorHandlerFn xAssertionEx::g_fnAssertHandler;

*/

/*



#define xASSERT_HANDLER(Condition) (false || (Condition) \
|| (xAssertionEx::GetAssertFailureCustomHandler() \
&& (xAssertionEx::GetAssertFailureCustomHandler()( \
AFS_ASSERT, #Condition, __FILE__, __LINE__), true)))

#define xAssert(Condition) XASSERT(xASSERT_HANDLER(Condition))
*/
//#define xVerify(Condition) xAssert(Condition)

//#define XASSERT(Condition) assert(xASSERT_HANDLER(Condition))


/*
#define xVerifyHandlerEx(Condition,Message,Object,PostAction,Result) (false || (Condition,Message,Object,PostAction,Result) \
|| (xAssertionEx::CVerifyChecks2() \
&& (xAssertionEx::CVerifyChecks2()( \
AFS_ASSERT, #Condition, __FILE__, __LINE__,Message,Object,PostAction,Result), true)))
*/
/*
#define xVerifyAndCorrect(Condition,Message,Object,PostAction,Result) { \
bool bConditionValue; \
xAssert(xVerifyHandlerEx(Condition,Message,Object,PostAction,Result)); \
(void)(bConditionValue || (*(int *)0 = 0)); \
}
*/

/*
#define xCheck(Condition) { \
bool bConditionValue; \
XASSERT(xCHECK_HANDLER(Condition)); \
(void)(bConditionValue || (*(int *)0 = 0)); \
}
*/

/*
#define xVerifyHandlerEx(Conditio1n,Message,Object,PostAction,Result) (false || (Condition,Message,Object,PostAction,Result) \
|| (xAssertionEx::CVerifyChecks2() \
&& (xAssertionEx::CVerifyChecks2()( \
AFS_ASSERT, #Condition, __FILE__, __LINE__,Message,Object,PostAction,Result), true)))
*/


/*
#define xVerifyHandler(Condition) (false || (Condition) \
|| (xAssertionEx::GetAssertFailureCustomHandler() \
&& (xAssertionEx::GetAssertFailureCustomHandler()( \
AFS_ASSERT, #Condition, __FILE__, __LINE__), true)))
*/
/*
#define xVerifyHandler(Condition,Message,Object,PostAction,Result) (false || (Condition,Message,Object,PostAction,Result) \
|| (xAssertionEx::CVerifyChecks() \
&& (xAssertionEx::CVerifyChecks()(\
AFS_ASSERT, #Condition ,__FILE__, __LINE__,Message,Object,PostAction,Result), true)))
*/

/*
#define xVerifyHandler(Condition,Message,Object,PostAction,Result) (false || (Condition
|| (xAssertionEx::CVerifyChecks() \
&& (xAssertionEx::CVerifyChecks()( AFS_ASSERT, #Condition ,\
__FILE__, __LINE__,Message,Object,PostAction,Result), true)))
*/

/*
#define xCHECK_HANDLER(Condition) ((bConditionValue = false || (Condition)) \
|| (xAssertionEx::GetAssertFailureCustomHandler() \
&& (xAssertionEx::GetAssertFailureCustomHandler()( \
AFS_CHECK,  #Condition, __FILE__, __LINE__), true)))
*/

/*	
#define xCHECK_HANDLER_MO(Condition) ((bConditionValue = false || (Condition)) \
|| (xAssertionEx::GetAssertFailureCustomHandler() \
&& (xAssertionEx::GetAssertFailureCustomHandler()( \
AFS_CHECK,  #Condition, __FILE__, __LINE__), true)))
*/





/*



/*


#define xCheckMO(Condition) { \
bool bConditionValue; \
XASSERT(xCHECK_HANDLER(Condition)); \
(void)(bConditionValue || (*(int *)0 = 0)); \
}
*/




//----------------------------------------------------------------
//
// Release Code :
//
//#define xAssert(Condition) ((void)0)

//#define xVerify(Condition) ((void)(Condition))




/*
#define xVerifyAndCorrect(Condition) { \
	bool bConditionValue = true; \
	xAssert(xVerifyHandlerEx(Condition)); \
	(void)(bConditionValue || (*(int *)0 = 0)); }

*/
/*
#define xVerifyHandlerEx(Condition) (false||(Condition) \
	|| (xAssertionEx::GetVerifyCustomHandler() \
	&& (xAssertionEx::GetVerifyCustomHandler()( \
	AFS_ASSERT, #Condition, __FILE__, __LINE__), true)))
*/






/*
#define xVerifyHandlerEx(Condition) (false || (Condition)) \
	|| (xAssertionEx::CVerifyChecks2() \
	&& (xAssertionEx::CVerifyChecks2()( \
	AFS_ASSERT, #Condition, __FILE__, __LINE__), true)))
*/





//#define xVerifyAndCorrect(Condition,Message,Object,PostAction,Result) xAssert(XASSERT_HANDLER(Condition,Message,Object,PostAction,Result))
//#define xVerify(Condition) xAssert(Condition)

/*#define xVerifyAndCorrect(Condition,Message,Object,PostAction,Result)xAssert(Condition)*/








#endif