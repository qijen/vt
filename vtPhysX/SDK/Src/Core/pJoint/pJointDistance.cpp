#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointDistance::pJointDistance(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Distance)
{
}

void pJointDistance::setMinDistance(float distance)
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.minDistance=distance;

	if (distance !=0.0f)
	{
		descr.flags |=NX_DJF_MIN_DISTANCE_ENABLED; 
	}else
		descr.flags &=~NX_DJF_MIN_DISTANCE_ENABLED;

	joint->loadFromDesc(descr);
}

void pJointDistance::setMaxDistance(float distance)
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.maxDistance=distance;
	if (distance !=0.0f)
	{
		descr.flags |=NX_DJF_MAX_DISTANCE_ENABLED; 
	}else
		descr.flags &=~NX_DJF_MAX_DISTANCE_ENABLED;
	joint->loadFromDesc(descr);
}

void pJointDistance::setLocalAnchor1(VxVector anchor)
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.localAnchor[1] =getFrom(anchor);
	joint->loadFromDesc(descr);
}

void pJointDistance::setLocalAnchor0(VxVector anchor)
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.localAnchor[0] =getFrom(anchor);
	joint->loadFromDesc(descr);
}

void pJointDistance::enableCollision(int collision)
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}
bool pJointDistance::setSpring( pSpring spring )
{
	NxDistanceJointDesc descr;
	NxDistanceJoint *joint  = static_cast<NxDistanceJoint*>(getJoint());
	if (!joint)return false;
	joint->saveToDesc(descr);

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	descr.spring= sLimit;

	if(spring.spring!=0.0f || spring.damper!=0.0f )
		descr.flags|=NX_DJF_SPRING_ENABLED;
	else
		descr.flags &=~NX_DJF_SPRING_ENABLED;
	joint->loadFromDesc(descr);

	return true;
}

VxVector pJointDistance::getLocalAnchor0()
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return getFrom(descr.localAnchor[0]);
}
VxVector pJointDistance::getLocalAnchor1()
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return getFrom(descr.localAnchor[1]);
}

float pJointDistance::getMinDistance()
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	

	if (!joint)return -1.0f;

	joint->saveToDesc(descr);
	return descr.minDistance;
}

float pJointDistance::getMaxDistance()
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	

	if (!joint)return -1.0f;

	joint->saveToDesc(descr);
	return descr.maxDistance;
}

pSpring pJointDistance::getSpring()
{
	NxDistanceJointDesc descr;		
	NxDistanceJoint*joint  = static_cast<NxDistanceJoint*>(getJoint());	
	pSpring result;
	if (!joint)result;
	joint->saveToDesc(descr);

	result.spring = descr.spring.spring;
	result.damper = descr.spring.damper;
	result.targetValue = descr.spring.targetValue;
	return result;
}