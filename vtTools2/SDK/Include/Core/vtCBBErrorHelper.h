#ifndef __VT_C_BB_ERROR_HELPER_H__
	#define __VT_C_BB_ERROR_HELPER_H__

#ifndef __X_LOGGER_H__
	#include <xLogger.h>
#endif

#ifndef __P_CONSTANTS_H__
	#include "pConstants.h"	
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

#endif