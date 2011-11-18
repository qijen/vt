#ifndef __P_ATTRIBUTE_HELPER_H__
#define __P_ATTRIBUTE_HELPER_H__

#define PHYSIC_BODY_CAT "Physic"

#define ATT_FUNC_TABLE_SIZE 12

#include "vtParameterGuids.h"
#include "pManagerTypes.h"

//################################################################
//
// Declaration of rigid body related attribute callback function
//
int registerRigidBody(CK3dEntity *target,int attributeType,bool set,bool isPostJob);

//################################################################
//
// Declaration of various joint attribute callback functions 
//
int registerJDistance(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJFixed(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJBall(CK3dEntity *target,int attributeType,bool set,bool isPostJob);

int registerJPrismatic(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJCylindrical(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJPointInPlane(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJPointOnLine(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJRevolute(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJD6(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJD6Drive(CK3dEntity *target,int attributeType,bool set,bool isPostJob);
int registerJLimitPlane(CK3dEntity *target,int attributeType,bool set,bool isPostJob);

//----------------------------------------------------------------
//
//! \brief The global map of parameter type and registration function
//
static ObjectRegistration attributeFunctionMap[]  = 
{
	ObjectRegistration(VTS_PHYSIC_ACTOR,registerRigidBody),
	ObjectRegistration(VTS_JOINT_DISTANCE,registerJDistance),
	ObjectRegistration(VTS_JOINT_FIXED,registerJFixed),
	ObjectRegistration(VTS_JOINT_BALL,registerJBall),
	ObjectRegistration(VTS_JOINT_PRISMATIC,registerJPrismatic),
	ObjectRegistration(VTS_JOINT_POINT_IN_PLANE,registerJPointInPlane),
	ObjectRegistration(VTS_JOINT_POINT_ON_LINE,registerJPointOnLine),
	ObjectRegistration(VTS_JOINT_CYLINDRICAL,registerJCylindrical),
	ObjectRegistration(VTS_JOINT_REVOLUTE,registerJRevolute),
	ObjectRegistration(VTS_JOINT_D6,registerJD6),
	ObjectRegistration(VTS_JOINT_D6_DRIVES,registerJD6Drive),
	ObjectRegistration(VTS_PHYSIC_JLIMIT_PLANE,registerJLimitPlane),
};


//################################################################
//
// Misc prototypes
//

//----------------------------------------------------------------
//
//! \brief	This is the attribute callback function which is expected from Virtools.
//			We only use this as dispatcher function because we have our own sub set.
//
void	PObjectAttributeCallbackFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg);


//################################################################
//
// OLD 
//



//----------------------------------------------------------------
//
//! \brief this has become obselete 
//
void	recheckWorldsFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg);		// --> old !

void rigidBodyAttributeCallback(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg);	//-->new !



#endif