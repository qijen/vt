#ifndef __VT_C_BB_ERROR_HELPER_H__
	#define __VT_C_BB_ERROR_HELPER_H__

#ifndef __X_LOGGER_H__
	#include <xLogger.h>
#endif


#define CERROR_STRING(F) sBBErrorStrings[F]


#define bbSErrorME(A)	{	xLogger::xLog(XL_START,ELOGERROR,E_BB,CERROR_STRING(A));\
	XLOG_BB_INFO;\
	beh->ActivateOutput(0);\
	return CKBR_PARAMETERERROR ; }

#define bbErrorME(A)	{ xLogger::xLog(XL_START,ELOGERROR,E_BB,A);\
	XLOG_BB_INFO;\
	beh->ActivateOutput(0);\
	return CKBR_PARAMETERERROR ; }

#define bbWarning(A){ xLogger::xLog(XL_START,ELOGWARNING,E_BB,A);\
	XLOG_BB_INFO;\
}



/*#define XL_BB_NAME  beh->GetPrototype()->GetName()
#define XL_BB_OWNER_SCRIPT beh->GetOwnerScript()->GetName()
#define XL_BB_OWNER_OBJECT beh->GetOwner() ? beh->GetOwner()->GetName() : "none"
#define XL_BB_SIGNATURE ("\n\tScript : %s\n\tBuildingBlock : %s \n\tObject :%s Error :") 


#define XLOG_FMT(msg,extro) msg##extro
#define XLOG_MERGE(var, fmt) (#var##fmt )
#define XLOG_MERGE2(var,fmt) (var##fmt)

#define XLOG_BB_INFO xLogger::xLogExtro(0,XL_BB_SIGNATURE,XL_BB_OWNER_SCRIPT,XL_BB_NAME,XL_BB_OWNER_OBJECT)

#define VTERROR_STRING(F) sErrorStrings[F]
#define bbError(F) 	XLOG_BB_INFO; \
	Error(beh,F,BB_OP_ERROR,VTERROR_STRING(F),TRUE,BB_O_ERROR,TRUE)
#define bbNoError(F) Error(beh,F,BB_OP_ERROR,VTERROR_STRING(F),FALSE,BB_O_ERROR,FALSE)
*/



#endif