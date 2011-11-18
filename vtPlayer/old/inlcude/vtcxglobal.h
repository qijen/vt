/********************************************************************
	created:	2007/12/12
	created:	12:12:2007   11:55
	filename: 	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude\vtcxglobal.h
	file path:	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude
	file base:	vtcxglobal
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/



#ifndef __VTCXGLOBAL_H_
	#define __VTCXGLOBAL_H_

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Platform Headers
//

#include <vtCXPrecomp.h>
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Platform specific header switchs : 
//
#ifdef _WIN32
	#include <vtCXPlatform32.h>
#endif



////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Build switchs :  
//


////////////////////////////////////////////////////////////////////////// 
//	GBLDebugBuild is used to hide GBL - private  building blocks, types, attributes,...
#ifdef NDEBUG
	static const bool vtCXDebugBuild = true;
#else
	static const bool vtCXDebugBuild = false;
#endif

//////////////////////////////////////////////////////////////////////////
// dll directives :
#ifndef VTCX_API_EXPORT
	#define VTCX_API_EXPORT __declspec(dllexport)
#endif

#ifndef VTCX_API_INLINE
	#define VTCX_API_INLINE __inline
#endif

#ifndef VTCX_API_sCALL
	#define VTCX_API_sCALL __stdcall
#endif

#ifndef VTCX_API_cDECL
	#define VTCX_API_cDECL __cdecl
#endif


////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						API Specific Constants  :
//

#define VTCX_API_PREFIX "vtCX"
#define VTCX_API_ENTRY(F) VTCX_API_PREFIX##F

#define VTCX_API_CUSTOM_BB_CATEGORY(F) VTCX_API_PREFIX##F	
	//	#define MY_BB_CAT  VTCX_API_CUSTOM_BB_CATEGORY(/Configurable Information) leads to : "GBL/Configurable Information"


#ifndef VTCX_AUTHOR
	#define	VTCX_AUTHOR				"Guenter Baumgart"
#endif

#ifndef VTCX_AUTHOR_GUID
	#define	VTCX_AUTHOR_GUID			CKGUID(0x79ba75dd,0x41d77c63)
#endif

//////////////////////////////////////////////////////////////////////////
//
//						Error Identifiers : 
//

//Error code to identify the GBL-COMMON Component :
#define VTCX_ERROR_ID_VTCX_COMMON 10


////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						File System Specific : 						
//

#if defined (_LINUX)
	#define VTCX_FS_PATH_SEPERATOR '/'
	#define VTCX_FS_PATH_DRIVE_SEPARATOR  ':'
	#define VTCX_FS_EOL  "\n"			//(0x0D)
#endif

#ifdef _WIN32
	#define VTCX_FS_PATH_SEPERATOR '\\'
	#define VTCX_FS_PATH_DRIVE_SEPARATOR 
	#define VTCX_FS_EOL  "\r\n"		//(0x0A 0x0D)
#endif

#if defined (macintosh)
	#define VTCX_FS_PATH_SEPERATOR '/'
	#define VTCX_FS_PATH_DRIVE_SEPARATOR 
	#define VTCX_FS_EOL  "\r"		//(0x0A)
#endif

#define VTCX_FS_PATH_EXT_SEPARATOR '.'

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//

 		


#endif