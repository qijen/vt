#ifndef __BASEMACROS_H
	#define __BASEMACROS_H

////////////////////////////////////////////////////////////////////////// 
//
//						Platform Headers
//
////////////////////////////////////////////////////////////////////////// 
//
//						Platform specific header switchs : 
//

////////////////////////////////////////////////////////////////////////// 
//
//						Build switchs :  
//

////////////////////////////////////////////////////////////////////////// 
//	DebugBuild is used to hide  private  building blocks, types, attributes,...
#ifdef NDEBUG
				static const bool DebugBuild = true;
#else
				static const bool DebugBuild = false;
#endif

//////////////////////////////////////////////////////////////////////////
// dll directives :
#ifdef _WIN32
	#define API_EXPORT __declspec(dllexport)
#else
	
// Unix needs no export, but for name mangling, keep the function name
// clean. If you omit the 'extern "C"', the .so names will be compiler dependent.
	#define API_EXPORT extern "C"
#endif


#ifndef API_INLINE
	#define API_INLINE __inline
#endif

#ifndef API_sCALL
	#define API_sCALL __stdcall
#endif

#ifndef API_cDECL
	#define API_cDECL __cdecl
#endif


#define DLLEXPORT			__declspec( dllexport )
#define DLLIMPORT			__declspec( dllimport )



#if defined(MODULE_STATIC)
	#define MODULE_IMPORT_API
	#define MODULE_EXPORT_API
#else
	#define MODULE_IMPORT_API DLLIMPORT
	#define MODULE_EXPORT_API DLLEXPORT
#endif

#if defined( MODULE_BASE_EXPORTS )
	#define MODULE_API MODULE_EXPORT_API
#else
	#define MODULE_API MODULE_IMPORT_API
#endif

//////////////////////////////////////////////////////////////////////////
//
//						Error Identifiers : 
//

////////////////////////////////////////////////////////////////////////// 
//
//						File System Specific : 						
//

#if defined (_LINUX)
	#define FS_PATH_SEPERATOR '/'
	#define FS_PATH_DRIVE_SEPARATOR  ':'
	#define FS_EOL  "\n"			//(0x0D)
#endif

#ifdef _WIN32
	#define FS_PATH_SEPERATOR '\\'
	#define FS_PATH_DRIVE_SEPARATOR 
	#define FS_EOL  "\r\n"		//(0x0A 0x0D)
#endif

#if defined (macintosh)
	#define FS_PATH_SEPERATOR '/'
	#define FS_PATH_DRIVE_SEPARATOR 
	#define FS_EOL  "\r"		//(0x0A)
#endif

#define FS_PATH_EXT_SEPARATOR '.'
		

#endif