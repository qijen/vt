#ifndef STDAFX_H
#define STDAFX_H


	#if defined(_MSC_VER) && (_MSC_VER <= 1200)
		#   pragma warning(disable : 4099)
	#endif


	#ifdef USEDIRECTX9
		#include <d3d9.h>
		#include <d3dx9math.h>
		#include <d3dx9.h>
		#if defined(DEBUG) || defined(_DEBUG)
		#ifndef V
		#define V(x)           { hr = x; if( FAILED(hr) ) { DXTrace( __FILE__, (DWORD)__LINE__, hr, #x, true ); } }
		#endif
		#ifndef V_RETURN
		#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) {  return DXTrace( __FILE__, (DWORD)__LINE__, hr, #x, true ); }  }
		#endif
		#else
		#ifndef V
		#define V(x)           { hr = x; }
		#endif
		#ifndef V_RETURN
		#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
		#endif
		#endif
	#else
//		#include <d3dx9math.h>
	#endif


	#include <WTypes.h>
	#include <stdlib.h>
	#include <map>
	#include <vector>

	#ifdef VIRTOOLS_USER_SDK
		#include "CKAll.h"
	#endif

#ifdef NVIDIA_PHYSX_SDK

//	#include "cooking.h"
//	#include "NxPhysicsSDK.h"
//	#include "NxPMap.h"
//	#include "PhysXLoader.h"

#endif

#ifdef VIRTOOLS_USER_SDK
	#include <CKAll.h>
#endif




/*	#ifdef xGUIForwards
		#include <xGUI_ForwardsRefs.h>
	#endif
*/

/*
	#ifdef USE_ENIGMA
		#define ENIGMA_CLIENTSIDE		// Set to import headers
		#include "Enigma/Source/Precompiled.h"
		#include "Enigma/Source/Enigma.h"
	#endif

*/
	#ifdef D3D_DEBUG_INFO

		#ifndef DEBUG_VS   // Uncomment this line to debug vertex shaders 
			#define DEBUG_VS   // Uncomment this line to debug pixel shaders 
		#endif

		#ifndef DEBUG_PS   // Uncomment this line to debug vertex shaders 
			#define DEBUG_PS   // Uncomment this line to debug pixel shaders 
		#endif


		#ifdef D3D_DEBUG_INFO
			//const static long TryAtFirst  = 0x00000080L;
			//const static bool ShaderDebugger  = true ; 
		#else
			//const static long TryAtFirst  = 0x00000040L;
			//const static bool ShaderDebugger  = false; 
		#endif

	#endif

#endif