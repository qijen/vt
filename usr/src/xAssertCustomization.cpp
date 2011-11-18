#include <StdAfx.h>
#include <xAssertion.h>
#include <xAssertCustomization.h>

#include <xDebugTools.h>

#include "vtPhysXBase.h"



//#define WASSERT_VERIFY_PREFIX(Condition,bufferSize) xVerifyAndCorrect(Condition)

static const xAssertInfo g_fiAssertInvalidInfo(AFS_ASSERT,NULL,NULL, 0 , NULL ,NULL,false);
static xAssertInfo g_fiAssertLastInfo;

void xCONVENTION_CALLBACK testAssertsAssertionFailure(E_ASSERTION_FAILURE_SEVERITY fsFailureSeverity, 
														 char *szAssertionExpression, 
														 char *szAssertionFileName, 
														  int uiAssertionSourceLine,
														 char *assertionPostMessage,
														 void* postAction,
														 bool result
														 ){
	g_fiAssertLastInfo.failureSeverity= fsFailureSeverity;
	g_fiAssertLastInfo.assertionExpression = szAssertionExpression;
	g_fiAssertLastInfo.assertionFileName = szAssertionFileName;
	g_fiAssertLastInfo.assertionSourceLine = uiAssertionSourceLine;
	g_fiAssertLastInfo.assertionPostMessage = assertionPostMessage;
	g_fiAssertLastInfo.postAction = postAction;
	g_fiAssertLastInfo.result = result;
}

xAssertInfo* assertFailed()
{
/*

	if (g_fiAssertLastInfo.assertionExpression == NULL
		|| g_fiAssertLastInfo.assertionFileName == NULL
		|| strcmp(g_fiAssertLastInfo.assertionFileName, __FILE__) != 0
		|| g_fiAssertLastInfo.assertionSourceLine == 0
		|| !g_fiAssertLastInfo.result)
	{*/

	if (!g_fiAssertLastInfo.result)
	{
		return NULL;
	}

	return &g_fiAssertLastInfo;

}
xAssertInfo *getLastAssertInfo(){ return &g_fiAssertLastInfo; }

void xCONVENTION_CALLBACK tickAssertHandlerData()
{

	bool bResult = false;

	xErrorHandlerFn fnAssertOldHandler = xAssertionEx::getErrorHandler();
	xAssertionEx::updateErrorHandler(&updateAssertHandlerData);



	return;
	/*

	do
	{
#if !defined(NDEBUG)
		// Only callback invocation is checked here.
		// Availability of functionality depending on preprocessor defines
		// is verified in OST_ASSERT subsystem.

		OU_ASSERT(false); // const unsigned int uiAssertToVerifyLines = 14; -- see further in code

		if (g_fiAssertLastInfo.m_fsFailureSeverity != AFS_ASSERT
			|| g_fiAssertLastInfo.m_szAssertionExpression == NULL
			|| g_fiAssertLastInfo.m_szAssertionFileName == NULL
			|| strcmp(g_fiAssertLastInfo.m_szAssertionFileName, __FILE__) != 0
			|| g_fiAssertLastInfo.m_uiAssertionSourceLine == 0)
		{
			break;
		}

		CTestCustomizations_Asserts_FailureInfo fiAssertFailureInfoSave = g_fiAssertLastInfo;
		g_fiAssertLastInfo = g_fiAssertInvalidInfo;

		OU_VERIFY(false); const unsigned int uiAssertToVerifyLines = 14;

		if (g_fiAssertLastInfo.m_fsFailureSeverity != AFS_ASSERT
			|| g_fiAssertLastInfo.m_szAssertionExpression == NULL
			|| strcmp(g_fiAssertLastInfo.m_szAssertionExpression, fiAssertFailureInfoSave.m_szAssertionExpression) != 0
			|| g_fiAssertLastInfo.m_szAssertionFileName == NULL
			|| strcmp(g_fiAssertLastInfo.m_szAssertionFileName, __FILE__) != 0
			|| g_fiAssertLastInfo.m_uiAssertionSourceLine != fiAssertFailureInfoSave.m_uiAssertionSourceLine + uiAssertToVerifyLines)
		{
			break;
		}

		g_fiAssertLastInfo = g_fiAssertInvalidInfo;


#endif // #if !defined(NDEBUG)

		 -- can't verify OU_CHECK() as it crashes the application on failure
		OU_CHECK(false);

		if (g_fiAssertLastInfo.m_fsFailureSeverity != AFS_CHECK
		|| g_fiAssertLastInfo.m_szAssertionExpression == NULL
		|| g_fiAssertLastInfo.m_szAssertionFileName == NULL
		|| strcmp(g_fiAssertLastInfo.m_szAssertionFileName, __FILE__) != 0
		|| g_fiAssertLastInfo.m_uiAssertionSourceLine == 0)
		{
		break;
		}
		
		bResult = true;
	}
	while (false);

	CAssertionCheckCustomization::CustomizeAssertionChecks(fnAssertOldHandler);

	return bResult;
	*/

}

void xCONVENTION_CALLBACK rotateAssertInfo(

										 E_ASSERTION_FAILURE_SEVERITY failureSeverity,
										 char *assertionExpression,
										 char *assertionFileName,
										 int assertionSourceLine,
										 char *assertionPostMessage,
										 void* postAction,
										 bool result)
{
	g_fiAssertLastInfo.failureSeverity		= failureSeverity;
	g_fiAssertLastInfo.assertionExpression	= assertionExpression;
	g_fiAssertLastInfo.assertionFileName	= assertionFileName;
	g_fiAssertLastInfo.assertionSourceLine	= assertionSourceLine;

	g_fiAssertLastInfo.assertionPostMessage	= assertionPostMessage;
	g_fiAssertLastInfo.postAction	= postAction;


	xErrorHandlerFn fnAssertOldHandler = xAssertionEx::Instance()->getErrorHandler();
	xAssertionEx::Instance()->updateErrorHandler(&updateAssertHandlerData);




}

void xCONVENTION_CALLBACK updateAssertHandlerData(
	E_ASSERTION_FAILURE_SEVERITY _failureSeverity,
	char *_assertionExpression,
	char *_assertionFileName,
	int _assertionSourceLine,
	char *_assertionPostMessage,
	void* _postAction,
	bool _result)
{

	g_fiAssertLastInfo.failureSeverity = _failureSeverity;
	g_fiAssertLastInfo.assertionExpression = _assertionExpression;
	g_fiAssertLastInfo.assertionFileName = _assertionFileName;
	g_fiAssertLastInfo.assertionSourceLine = _assertionSourceLine;
	g_fiAssertLastInfo.assertionPostMessage = _assertionPostMessage;
	g_fiAssertLastInfo.postAction = _postAction;
	g_fiAssertLastInfo.result = _result;

}

xAssertInfo createAssertInfo(
							 E_ASSERTION_FAILURE_SEVERITY failureSeverity,
							 char *assertionExpression,
							 char *assertionFileName,
							 int assertionSourceLine,
							 char *assertionPostMessage,
							 void* postAction,
							 bool result)
{
	return xAssertInfo(failureSeverity,assertionExpression,assertionFileName,assertionSourceLine,assertionPostMessage,postAction,result);
} 



bool customizeAsserts()
{



	bool bResult = false;

	xErrorHandlerFn fnAssertOldHandler = xAssertionEx::getErrorHandler();
	xAssertionEx::updateErrorHandler(&testAssertsAssertionFailure);

	return bResult;


}

bool TestCustomizations_Asserts()
{
	

	bool bResult = false;
	xErrorHandlerFn fnAssertOldHandler = xAssertionEx::getErrorHandler();
	xAssertionEx::updateErrorHandler(&testAssertsAssertionFailure);


	do
	{

		// Only callback invocation is checked here.
			// Availability of functionality depending on preprocessor defines
			// is verified in OST_ASSERT subsystem.

			float a = 10;
			//xAssert(1); // const unsigned int uiAssertToVerifyLines = 14; -- see further in code
			int test=0;
			bool resultAssert=false;
			xAssertW( a < 5,test=10,"reset value",D_SO_NAME,resultAssert);


			if (g_fiAssertLastInfo.failureSeverity != AFS_ASSERT
				|| g_fiAssertLastInfo.assertionExpression == NULL
				|| g_fiAssertLastInfo.assertionFileName == NULL
				|| strcmp(g_fiAssertLastInfo.assertionFileName, __FILE__) != 0
				|| g_fiAssertLastInfo.assertionSourceLine == 0)
			{
				break;
			}

			xAssertInfo fiAssertFailureInfoSave = g_fiAssertLastInfo;
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,fiAssertFailureInfoSave.assertionExpression);



			g_fiAssertLastInfo = g_fiAssertInvalidInfo;

	}
	while (false);


	xAssertionEx::updateErrorHandler(fnAssertOldHandler);

	
	return false;
	
}

/*			xAssertInfo *last = getLastAssertInfo();
if (last)
{
xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,last->assertionExpression);
}
*/
/*			xVerify(false); const unsigned int uiAssertToVerifyLines = 14;

if (g_fiAssertLastInfo.failureSeverity != AFS_ASSERT
|| g_fiAssertLastInfo.assertionExpression == NULL
|| strcmp(g_fiAssertLastInfo.assertionExpression, fiAssertFailureInfoSave.assertionExpression) != 0
|| g_fiAssertLastInfo.assertionFileName == NULL
|| strcmp(g_fiAssertLastInfo.assertionFileName, __FILE__) != 0
|| g_fiAssertLastInfo.assertionSourceLine != fiAssertFailureInfoSave.assertionSourceLine + uiAssertToVerifyLines)
{
break;
}
*/
//g_fiAssertLastInfo = g_fiAssertInvalidInfo;


/*xAssertInfo *last = getLastAssertInfo();
if (last)
{
xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,last->assertionExpression);
}*/

/*
#endif 
//#if !defined(NDEBUG)


//		 -- can't verify OU_CHECK() as it crashes the application on failure
//xCheck(false);
/*
if (g_fiAssertLastInfo.failureSeverity != AFS_CHECK
|| g_fiAssertLastInfo.assertionExpression == NULL
|| g_fiAssertLastInfo.assertionFileName == NULL
|| strcmp(g_fiAssertLastInfo.assertionFileName, __FILE__) != 0
|| g_fiAssertLastInfo.assertionSourceLine == 0)
{
break;
}*/