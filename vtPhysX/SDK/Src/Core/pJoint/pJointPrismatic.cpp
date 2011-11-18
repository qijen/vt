#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointPrismatic::pJointPrismatic(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Prismatic)
{

}


void pJointPrismatic::setGlobalAnchor(VxVector anchor)
{


	NxPrismaticJointDesc descr;		
	NxPrismaticJoint*joint  = static_cast<NxPrismaticJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAnchor(getFrom(anchor));
	joint->loadFromDesc(descr);
}

void pJointPrismatic::setGlobalAxis(VxVector axis)
{


	NxPrismaticJointDesc descr;		
	NxPrismaticJoint*joint  = static_cast<NxPrismaticJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAxis(getFrom(axis));
	joint->loadFromDesc(descr);
}

void pJointPrismatic::enableCollision(int collision)
{
	NxPrismaticJointDesc descr;		
	NxPrismaticJoint*joint  = static_cast<NxPrismaticJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}