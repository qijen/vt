#ifndef __VTMODULES_GUIDS_H__
 #define __VTMODULES_GUIDS_H__

#include "vtBaseMacros.h"

//----------------------------------------------------------------
//
//! \brief The guid of the modules manager. Used 
//
//
//! \brief Manager Guid, used in plug-in registration, building blocks and many core components 
//
#define GUID_MODULE_MANAGER		CKGUID(0x1c0f04e8,0x442e20e5)

//----------------------------------------------------------------
//
//! \brief ::	The guid of the modules building blocks
//				If defined "WebPack", its using the guid of the built-in camera plug in.   
#ifdef WebPack
	#define GUID_MODULE_BUILDING_BLOCKS	CKGUID(0x12d94eba,0x47057415)
#else
	#define GUID_MODULE_BUILDING_BLOCKS	CKGUID(0x36834d9e,0x63664944)
#endif

#endif