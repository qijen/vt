#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointCylindrical::pJointCylindrical(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Cylindrical)
{

}

void pJointCylindrical::setGlobalAnchor(VxVector anchor)
{


	NxCylindricalJointDesc descr;		
	NxCylindricalJoint*joint  = static_cast<NxCylindricalJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAnchor(getFrom(anchor));
	joint->loadFromDesc(descr);
}

void pJointCylindrical::setGlobalAxis(VxVector axis)
{


	NxCylindricalJointDesc descr;		
	NxCylindricalJoint*joint  = static_cast<NxCylindricalJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAxis(getFrom(axis));
	joint->loadFromDesc(descr);
}

void pJointCylindrical::enableCollision(int collision)
{
	NxCylindricalJointDesc descr;		
	NxCylindricalJoint*joint  = static_cast<NxCylindricalJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}