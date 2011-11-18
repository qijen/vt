#ifndef __base_macros_h__
#define __base_macros_h__


#ifndef API_EXPORT
	#define API_EXPORT __declspec(dllexport)
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

#endif