/********************************************************************
	created:	2007/12/15
	created:	15:12:2007   22:28
	filename: 	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\plugins\xUtils\include\xUtils.h
	file path:	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\plugins\xUtils\include
	file base:	xUtils
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#ifndef __X_UTILS_H_
#define __X_UTILS_H_

#include <BaseMacros.h>

namespace xUtils
{

	namespace system

	{

	
		 MODULE_API HRESULT GetDirectVersion(char*&version,DWORD& minor);
		 MODULE_API int GetPhysicalMemoryInMB();
	
	}

}

#endif

