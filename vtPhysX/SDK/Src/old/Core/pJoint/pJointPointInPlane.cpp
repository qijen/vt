#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointPointInPlane::pJointPointInPlane(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_PointInPlane)
{

}

void pJointPointInPlane::setGlobalAnchor(VxVector anchor)
{


	NxPointInPlaneJointDesc descr;		
	NxPointInPlaneJoint*joint  = static_cast<NxPointInPlaneJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAnchor(getFrom(anchor));
	joint->loadFromDesc(descr);
}

void pJointPointInPlane::setGlobalAxis(VxVector axis)
{


	NxPointInPlaneJointDesc descr;		
	NxPointInPlaneJoint*joint  = static_cast<NxPointInPlaneJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAxis(getFrom(axis));
	joint->loadFromDesc(descr);
}



void pJointPointInPlane::enableCollision(int collision)
{
	NxPointInPlaneJointDesc descr;		
	NxPointInPlaneJoint*joint  = static_cast<NxPointInPlaneJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}
