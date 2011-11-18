/********************************************************************
	created:	2009/02/16
	created:	16:2:2009   9:07
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common\ModulePrerequisites.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common
	file base:	ModulePrerequisites
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Include of all prerequisites
*********************************************************************/
#ifndef __PREREQUISITES_ALL_H__
#define __PREREQUISITES_ALL_H__

//################################################################
//
//	Common forward declarations :
//

//################################################################
//
// Virtools related forward declarations :
//
#include "Prerequisites_Virtools.h"

//################################################################
//
// 3th party forward declarations : 
//
//	+ TinyXML
//	+ NXUStream
//
#include "Prerequisites_3thParty.h"

//################################################################
//
// Main library forward declarations :
//
#include "Prerequisites_PhysX.h"

//################################################################
//
// Forward declarations for this module itself : 
//
#include "Prerequisites_Module.h"

#include "Timing.h"




#endif

