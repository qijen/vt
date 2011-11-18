/********************************************************************
	created:	2009/02/17
	created:	17:2:2009   10:21
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\vtPhysXAll.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core 
	file base:	vtPhysXAll
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Common header for all definitions. Includes ALL !
*********************************************************************/
#ifndef __VTPHYSXALL_H__
#define __VTPHYSXALL_H__

#include "pTypes.h"


//################################################################
//
// Help types
//

//----------------------------------------------------------------
//
//! \brief Include of necessary meta and extra structures. 
//

#include "pTypes.h"

#include "pWorldTypes.h"
#include "pManagerTypes.h"
#include "pJointTypes.h"
#include "pRigidBodyTypes.h"


#include "pVehicleTypes.h"
#include "pWheelTypes.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"

#include "pClothTypes.h"


//----------------------------------------------------------------
//
//! \brief Logger 
//
#include <xLogger.h>


//################################################################
//
// Implementation Objects
//
#include "PhysicManager.h"
#include "pRigidBody.h"
#include "pFactory.h"
#include "pWorld.h"
#include "pJoint.h"
#include "pSerializer.h"

#include "pWheel.h"
#include "pCloth.h"


//----------------------------------------------------------------
//
//! \brief Vehicle Based Types 
//
#include "pVehicle.h"


//################################################################
//
// Implementation Helper
//
#include "pMisc.h"


//################################################################
//
// Parameter Guids
//
#include "vtParameterGuids.h"


//################################################################
//
// NVDIA PhysX Objects
//
#include "NxPhysics.h"
#include "NxUserOutputStream.h"
#include "NxSceneDesc.h"
#include "NxClothDesc.h"
#include "NxFluidDesc.h"
#include "NxFluidEmitterDesc.h"
#include "NxConvexShapeDesc.h"
#include "NxScene.h"
#ifdef VTPX_HAS_CHARACTER_CONTROLLER
	#include "NxControllerManager.h"
	#include "NxCharacter.h"
	#include "NxBoxController.h"
	#include "NxCapsuleController.h"
	#include "pBoxController.h"
#endif



//################################################################
//
// Generic Virtools Helpers
//
#include <virtools/vtTools.h>



//################################################################
//
// Math conversions
//
#include "pMathTools.h"
using namespace pMath;



#endif // __VTPHYSXALL_H__