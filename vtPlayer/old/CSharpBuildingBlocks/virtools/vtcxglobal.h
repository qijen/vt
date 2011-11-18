/********************************************************************
	created:	2008/02/25
	created:	25:2:2008   18:33
	filename: 	X:\junctions\ProjectRoot\usr\include\virtools\vtcxglobal.h
	file path:	X:\junctions\ProjectRoot\usr\include\virtools
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

#include <virtools/vtCXPrecomp.h>
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Platform specific header switchs : 
//
#ifdef _WIN32
	#include <virtools/vtCXPlatform32.h>
#endif



////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Build switchs :  
//


////////////////////////////////////////////////////////////////////////// 
//	DebugBuild is used to hide private  building blocks, types, attributes,...
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

#define VTCX_API_PREFIX "vt"
#define VTCX_API_ENTRY(F) VTCX_API_PREFIX##F

#define VTCX_API_CUSTOM_BB_CATEGORY(F) VTCX_API_PREFIX##F	
	//	#define MY_BB_CAT  VTCX_API_CUSTOM_BB_CATEGORY(/MyPlug) leads to : "vtCX/MyPlug"


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