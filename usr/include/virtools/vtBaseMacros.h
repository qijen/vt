#ifndef __VT_BASE_MACROS_H__
#define __VT_BASE_MACROS_H__


#include "vtModuleConstants.h"

//################################################################
//
// Universal macros
//

#define VTCX_API_ENTRY(F) VTCX_API_PREFIX##F
#define VTCX_API_CUSTOM_BB_CATEGORY(F) VTCX_API_PREFIX##F	


#ifndef XINLINE 
	#define XINLINE inline
#endif

#endif