#ifndef __X_DEBUG_TOOLS_H__
 #define __X_DEBUG_TOOLS_H__

	#include "xPlatform.h"
	#include "xAssertion.h"
	#include "xLogger.h"
//	#include <string>
	#include <fstream>

	#define X_IS_BETWEEN(VALUE,MIN,MAX) (VALUE < MAX && VALUE > MIN)
	
	#define X_NEGATE(A) !(A)
	#define _FLT_ASSIGMENT(A,B) ((A=B)==0)


	//----------------------------------------------------------------
	//
	// base types
	//
	

	
	#define D_MSG_BUFFER_MAX 4096



	#ifdef _DEBUG

		#define D_FILE_INFO 1
		#define D_FILE_LINE_INFO 0
	#else
	
		#define D_FILE_INFO 1
		#define D_FILE_LINE_INFO 0
	
	#endif

	//----------------------------------------------------------------
	//
	// constants
	//
	

	//----------------------------------------------------------------
	// hide filed details
	#if D_FILE_INFO==1
		#if !defined(__FILE__)
			extern const char *const __FILE__;
		#endif 
	#else
		#undef __FILE__
		#define __FILE__ ""
	#endif

	
	#if D_FILE_LINE_INFO==1
		#if !defined(__LINE__)
			extern const unsigned int __LINE__;
		#endif
	#else
		#undef __LINE__
		#define __LINE__ 0
	#endif
		
	//----------------------------------------------------------------
	//
	// surround objects  : 
	//
	#if defined(referenceObject)
		#define		DC_OBJECT referenceObject
	#else
		#define		DC_OBJECT NULL
	#endif

	

	//----------------------------------------------------------------
	//
	// Constants
	//
	#define ASSERT_STRING "Assertion"
	
	#define FATAL_STRING "Assertion"

	#define		D_SO_NAME_EX(SourceObjectName) SourceObjectName ? SourceObjectName->GetName() : "none"

#ifdef referenceObject
	#define		D_SO_NAME referenceObject ? referenceObject->GetName() : "none"
#else
	#define D_SO_NAME	"???"
#endif

	
	
	//----------------------------------------------------------------
	//
	// code fragments
	//
	

	#define D_DO_POST(PostAction) PostAction;

	#define D_ASSERT_P_END(PostAction) xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,buffer);\
		D_DO_POST(PostAction);
	
	

	#define xSTR_MERGE(A,B) A##B


	//----------------------------------------------------------------
	//
	// Assertions code fragments
	//

	#define WCASSERT_PREFIX(Assertion,bufferSize) if(!(Assertion)){char buffer[bufferSize];
	

	#define xASSERT_FORMAT_STRING(Assertion) "%s :\n\t Assertion \"" #Assertion "\" failed \n\t : %s : "
		
	//#define xASSERT_FORMAT_OPERATION(Assertion,postMsg,PostAction) _snprintf(buffer,D_MSG_BUFFER_MAX,xASSERT_FORMAT_STRING_PA(Assertion,PostAction),D_SO_NAME,xSTR_MERGE(postMsg,PostAction));
	
	#define xASSERT_FORMAT_OPERATION_EX(Assertion,SourceObjectName,PostMessage,PostAction) _snprintf(buffer,D_MSG_BUFFER_MAX,xASSERT_FORMAT_STRING(Assertion),SourceObjectName, \
			xSTR_MERGE(PostMessage,xSTR_MERGE(" , excecuting : ",#PostAction))); 
	//----------------------------------------------------------------
	//
	// constants
	//
	#if D_FILE_INFO==1

		#define xASSERT_FORMAT_STRING(A) "%s :\n\t Assertion2 \"" #A "\" failed in \n\t : (%s:%d)  :\n\t %s  " // \"" #B "\" "

		#define xASSERT_FORMAT_OPERATION_EX(Assertion,SourceObjectName,PostMessage,PostAction) _snprintf(buffer,D_MSG_BUFFER_MAX,xASSERT_FORMAT_STRING(Assertion),SourceObjectName,__FILE__,__LINE__, \
			xSTR_MERGE(PostMessage,xSTR_MERGE(" , excecuting : ",#PostAction))); 
	#endif
	

	#define WIDEN2(x) L ## x
	#define WIDEN(x) WIDEN2(x)
	#define __WFILE__ WIDEN(__FILE__)
	#define AMACRO( object_name, var_name ) reinterpret_cast<int>((&(((object_name*)0)->var_name))) 





	#define xAssertWPO(Assertion) xVerify(Assertion)

	#define REFRESH_ASSERT_HANDLER(Type,Assertion,PostMessage,PostAction,Result) updateAssertHandlerData(Type,#Assertion,__FILE__,__LINE__,PostMessage,((void*)(PostAction)),Result);

	//----------------------------------------------------------------
	//
	// macros to make an assert, error warning message, and providing a field to correct data
	//
	// prepare meta data : 
	

	#define CREATE_ASSERT_INFO_EX(Assertion,Type,FormatString,SourceObjectName,PostMessage,PostAction) WCASSERT_PREFIX(Assertion,D_MSG_BUFFER_MAX) \
		xASSERT_FORMAT_OPERATION_EX(Assertion,SourceObjectName,PostMessage,PostAction); 
	
	#define xwASSERTEx(Assertion,SourceObjectName,PostMessage,PostAction,Result) CREATE_ASSERT_INFO_EX(Assertion,AFS_ASSERT,xASSERT_FORMAT_STRING(Assertion),SourceObjectName,PostMessage,PostAction,Result);
	
	//REFRESH_ASSERT_HANDLER(AFS_ASSERT,Assertion,buffer,PostAction,Result); 

	#define __xwASSERT(Assertion,SourceObjectName,PostMessage,PostAction,Result) xwASSERTEx(Assertion,SourceObjectName,PostMessage,PostAction,Result)

	#define xAssertW(Assertion,PostAction,PostMessage,SourceObjectName,Result) \
		__xwASSERT(Assertion,SourceObjectName,PostMessage,PostAction,Result) \
			_xAssertHandler(Assertion); \
			 D_ASSERT_P_END(PostAction) \
			Result = false; \
	}
	
/*
	#define xAssertW(Assertion,PostAction,PostMessage,SourceObjectName,Result) \
		__xwASSERT(Assertion,SourceObjectName,PostMessage,PostAction,Result) \
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"start assert");\
		(false || (Assertion) || (xAssertionEx::getErrorHandler() && (xAssertionEx::getErrorHandler()\
		( AFS_ASSERT, #Assertion, __FILE__, __LINE__,"asdasd",NULL,TRUE), true)));\
		xAssertInfo *info = assertFailed();\
		if(info ){\
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"assert failed");\
			D_ASSERT_P_END(PostAction) \
			Result = false; } \
	}*/


		/*	
_xAssertHandler(Assertion); \
			xAssertInfo *info = assertFailed();\
			if(info ){\
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"assert failed");\
			D_ASSERT_P_END(PostAction) \
			Result = false; } \
	}
*/
	#define iAssertW(Assertion,PostAction,PostMessage)\
	{	bool returnValue = false;\
		xAssertW(Assertion,PostAction,PostMessage,D_SO_NAME,returnValue)\
	}

	#define iAssertWR(Assertion,PostAction,Result){\
		xAssertW(Assertion,PostAction,"",D_SO_NAME,Result)\
	}

	

	#define iAssertW1(Assertion,PostAction)\
	{	bool returnValue = false;\
		iAssertW(Assertion,PostAction,"")\
	}

	#define iAssertAndAbortMesg(Assertion,Msg)\
	{	bool returnValue = false;\
		iAssertW(Assertion,NULL,Msg);\
		if(!returnValue)return;\
	}
	#define iAssertAndAbort1(Assertion,POSTACTION)\
	{	bool returnValue = false;\
		iAssertW(Assertion,POSTACTION,"");\
		if(!returnValue)return;\
	}

	#define xError(Message) xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,Message);
	#define xWarning(Message) xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,Message);
	#define xTrace(Message) xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,Message);
	#define xInfo(Message) xLogger::xLog(XL_START,ELOGINFO,E_LI_MANAGER,Message);
	
	void _inline qdbg(const char *fmt,...)
	{
		char buffer[4096];
		unsigned int bufferStart = 0;

		va_list s;
		va_start( s, fmt );
		_vsnprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, fmt, s);
		va_end(s);
		xLogger::xLog(XL_START,ELOGINFO,E_LI_MANAGER,buffer);
	}

	#define xTrace1(Message,Object) { char buffer[D_MSG_BUFFER_MAX];\
		_snprintf(buffer,Message,Object ? Object->GetName() : "");\
		xTrace(buffer)\
	}



//#define iAssertWR(Assertion,PostAction,Result) xVerify(Assertion) 
//#define iAssertW1(Assertion,PostAction) xVerify(Assertion) 

		
	//(xAssertionEx::getErrorHandler() && (xAssertionEx::getErrorHandler()( AFS_ASSERT, #Assertion, __FILE__, __LINE__,"","",FALSE), true)))
//xVerify(Assertion);


	
	//xVerify(Assertion)

	

		
	//xASSERT_FORMAT_OPERATION(Condition,postMsg) \
	//D_ASSERT_P_END(postStep) }
					


		
	

	

	//#define D_VERIFY_O(Condition,obj,postMsg) WASSERT_VERIFY_PREFIX(Condition,D_MSG_BUFFER_MAX)
	//#define D_VERIFY_CORRECT_MOP(Condition) WASSERT_VERIFY_AND_CORRECT_PREFIX(Condition)
	//xASSERT_FORMAT_OPERATION(Condition,postMsg);


	//,postMsg,postStep,Result
	//if(!(Condition)){char buffer[bufferSize];
		//char buffer[bufferSize]; 



	//#define WASSERT_CHECK_PREFIX(Condition,bufferSize) xCheck(Condition); 
		

	//char buffer[bufferSize];	
	//----------------------------------------------------------------
	//
	//	!assert(a) ? : print warning, do post step
	//		
	//

	#define xWASSERT_O_P(Condition,obj,postMsg,postStep) WCASSERT_PREFIX(Condition,D_MSG_BUFFER_MAX)\
		xASSERT_FORMAT_OPERATION(Condition,postMsg) \
		D_ASSERT_P_END(postStep) }
		
	//----------------------------------------------------------------
	//
	// !assert(condition)? print warning and leave
	//
	#define D_WASSERTION_O(Condition,obj,postMsg) WASSERT_CHECK_PREFIX(Condition,D_MSG_BUFFER_MAX) \
		xASSERT_FORMAT_OPERATION(Condition,postMsg);

	
	
	#define D_CHECK_O(Condition,obj,postMsg) WASSERT_CHECK_PREFIX(Condition,D_MSG_BUFFER_MAX)


		//xASSERT_FORMAT_OPERATION(Condition,postMsg);


/*D_ASSERT_P_END(postStep) }*/

	//----------------------------------------------------------------
	//
	// !assert(condition)? print warning and leave
	//
/*	#define eAssertCheckMO(cond,obj,postMsg) WASSERT_CHECK_PREFIX(cond,D_MSG_BUFFER_MAX)\
		xASSERT_FORMAT_OPERATION(cond,postMsg) \
		D_ASSERT_P_END(postStep) }
*/
#endif //D_FILE_INFO

//#endif//file

	

		