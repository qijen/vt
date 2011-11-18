#ifndef _VTLOG_TOOLS_H_
#define _VTLOG_TOOLS_H_

#include "xLogger.h"

//////////////////////////////////////////////////////////////////////////
//
//
//	We different output channels for logging,debugging or assertions : 
//		+	console
//		+	log file
//		+	std::err

class CKContext;
class CKGUID;

namespace vtTools
{

	class BehaviorInfoTools
	{
		public :
			static const char*getBuildingBlockName(CKContext *ctx,CKGUID* guid);
	};

}

 
#define XL_BB_NAME  beh->GetPrototype()->GetName()
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

#define bbErrorMesg(F) 	xLogger::xLog(ELOGERROR,E_BB,F); \
	XLOG_BB_INFO;

#endif