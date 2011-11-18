#ifndef __VSL_GLOBAL_FUNCTIONS_H__
#define __VSL_GLOBAL_FUNCTIONS_H__

#include "vtPhysXBase.h"

/************************************************************************************************/
/** @name Joints
*/
//@{


/**
\brief Quick access for #PhysicManager::getJoint

\return pJoint*

@see 
*/
pJoint*getJoint(CK3dEntity *referenceA,CK3dEntity *referenceB=NULL,JType type= JT_Any);


//@}


#endif