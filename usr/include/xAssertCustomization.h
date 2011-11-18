#ifndef __X_ASSERT_CUSTOMIZATION_H__
	#define  __X_ASSERT_CUSTOMIZATION_H__

#include "xPlatform.h"

	struct xAssertInfo
	{
	public:
		E_ASSERTION_FAILURE_SEVERITY failureSeverity;
		char *assertionExpression;
		char *assertionFileName;
		int assertionSourceLine;
		char *assertionPostMessage;
		void *postAction;
		bool result;

		xAssertInfo() : failureSeverity(AFS__MAX) ,
            assertionExpression(NULL),
			assertionFileName(NULL),
			assertionSourceLine(0),
			assertionPostMessage(NULL),
			postAction(NULL),
			result(false)
		{

		}

		/*
		xAssertInfo() : 
		{
			failureSeverity			= AFS__MAX;
			assertionExpression		= NULL;
			assertionFileName		= NULL;
			assertionSourceLine		= 0;
			assertionSourceObject	= NULL;
			assertionPostMessage	= NULL;
			postAction				= NULL;
			result					= false;
		}
		*/

		xAssertInfo(
			E_ASSERTION_FAILURE_SEVERITY _failureSeverity,
			char *_assertionExpression,
			char *_assertionFileName,
			int _assertionSourceLine,
			char *_assertionPostMessage,
			void* _postAction,
			bool _result)
		{
            assertionExpression = _assertionExpression;
			assertionFileName = _assertionFileName;
			assertionSourceLine = _assertionSourceLine;
			assertionPostMessage = _assertionPostMessage;
			postAction = _postAction;
			failureSeverity = _failureSeverity;
			result = _result;
		}
	};

#endif