/********************************************************************
	created:	2009/02/16
	created:	16:2:2009   11:07
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common\vtPhysXBase.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common 
	file base:	vtPhysXBase
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Minimal includes for the whole component. 
				
				+ Generic macros and constants, functions
				+ Virtools specific forward decalarations
				+ Generic base types such as std::vector
				+ Component specific forward decalarations
				+ Disabling Module specific Visual Studio compiler Warnings
				+ Prerequisites
				+ Virtools Base Types ( XString, CKGUID, HashTable )
				+ Constants
	
	warning:	The order of the includes must stay !		

	remarks:	- This module is using the concept of forward declaration 
				- This header is the base for all other headers.
				- Do not introduce any platform specific dependencies( ie: windows.h )

*********************************************************************/



#ifndef __VT_PHYSX_BASE_H__
#define __VT_PHYSX_BASE_H__

//################################################################
//
// Generic 
//

//----------------------------------------------------------------
//
//	Include of base types, not involving external dependencies to std,etc..
//	
#include <xBaseTypes.h>


//----------------------------------------------------------------
//
//	Class to encapsulate a set of flags in a word, using shift operators
//
#include <xBitSet.h>


//----------------------------------------------------------------
//
//	Macros for generic DLL exports 
//
#include <BaseMacros.h>

//----------------------------------------------------------------
//
//	Include of Virtools related macros
//
#include <vtBaseMacros.h>


//################################################################
//
// Component specific constants,strings, error codes
//

//----------------------------------------------------------------
//
//			+	API Prefix 
//			+	Error Codes 
//			+	Error Strings
//
#include "vtModuleConstants.h"

//----------------------------------------------------------------
//
//	Virtools Guids of the plug-in ( manager + building block only !!! )
//	
//	GUIDS for custom enumerations, structures are included by the managers 
//	parameter_x.cpp explicitly ! 
//
#include "vtModuleGuids.h"

//----------------------------------------------------------------
//
//	Enumerations to identifier a custom structure's sub item
//
#include "vtParameterSubItemIdentifiers_All.h"


//----------------------------------------------------------------
//
//	Enumerations used by the SDK and the Virtools Interface ( Schematics, VSL)
//
#include "vtInterfaceEnumeration.h"


//################################################################
//
// Compiler specific warnings 
//
#include "vcWarnings.h"


//################################################################
//
// Prerequisites 
//
#include "Prerequisites_All.h"





#endif