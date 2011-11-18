#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointPointOnLine::pJointPointOnLine(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_PointOnLine)
{
}

void pJointPointOnLine::setGlobalAnchor(VxVector anchor)
{
	NxPointOnLineJointDesc descr;		
	NxPointOnLineJoint*joint  = static_cast<NxPointOnLineJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAnchor(getFrom(anchor));
	joint->loadFromDesc(descr);
}

void pJointPointOnLine::setGlobalAxis(VxVector axis)
{
	NxPointOnLineJointDesc descr;		
	NxPointOnLineJoint*joint  = static_cast<NxPointOnLineJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.setGlobalAxis(getFrom(axis));
	joint->loadFromDesc(descr);
}

void pJointPointOnLine::enableCollision(int collision)
{
	NxPointOnLineJointDesc descr;		
	NxPointOnLineJoint*joint  = static_cast<NxPointOnLineJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}