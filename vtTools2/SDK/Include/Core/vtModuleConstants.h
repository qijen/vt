#ifndef __vtModuleConstants_h__
#define __vtModuleConstants_h__


//----------------------------------------------------------------
//
//		Include of system headers 
//

#include <float.h> // float max 

//################################################################
//
// Component specific names, prefixes,etc....
//

//----------------------------------------------------------------
//
//! \brief Global API prefix
//
#define VTCX_API_PREFIX "vt"

//----------------------------------------------------------------
//
//! \brief Module name, merged with module suffix "vt" with see #VTCX_API_PREFIX
//
#define VTCMODULE_NAME VTCX_API_PREFIX("Physic")


//----------------------------------------------------------------
//
//! \brief Modules attribute category prefix , using module name above
//
#define VTCMODULE_ATTRIBUTE_CATAEGORY VTCMODULE_NAME

//----------------------------------------------------------------
//
//! \brief Error enumerations
//
#include "vtModuleErrorCodes.h"

//----------------------------------------------------------------
//
//! \brief Error strings
//
#include "vtModuleErrorStrings.h"

//----------------------------------------------------------------
//
//! \brief Guids of the plug-in it self 
//
#include "vtModuleGuids.h"



//----------------------------------------------------------------
//
//! \brief Math oriented values
//

#define pSLEEP_INTERVAL (20.0f*0.02f)
#define pFLOAT_MAX FLT_MAX


//----------------------------------------------------------------
//
//! \brief Constants for building blocks
//
#ifndef VTCX_AUTHOR
	#define	VTCX_AUTHOR				"Guenter Baumgart"
#endif

#ifndef VTCX_AUTHOR_GUID
	#define	VTCX_AUTHOR_GUID			CKGUID(0x79ba75dd,0x41d77c63)
#endif

#endif // vtModuleConstants_h__