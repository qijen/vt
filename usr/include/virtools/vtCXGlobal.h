/********************************************************************
	created:	2009/01/05
	created:	5:1:2009   18:18
	filename: 	X:\ProjectRoot\svn\local\usr\include\virtools\vtCXGlobal.h
	file path:	X:\ProjectRoot\svn\local\usr\include\virtools
	file base:	vtCXGlobal
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/
#ifndef __VTCXGLOBAL_H__
	#define __VTCXGLOBAL_H__

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
//
//						Platform Headers
//

//#include <virtools/vtCXPrecomp.h>
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