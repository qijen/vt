#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointBall::pJointBall(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Spherical)
{

}

pJointLimit pJointBall::getSwingLimit()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pJointLimit();
	joint->saveToDesc(descr);
	return pJointLimit (descr.swingLimit.hardness,descr.swingLimit.restitution,descr.swingLimit.value);
}

pJointLimit pJointBall::getTwistHighLimit()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pJointLimit();
	joint->saveToDesc(descr);
	return pJointLimit (descr.twistLimit.high.hardness,descr.twistLimit.high.restitution,descr.twistLimit.high.value);
}

pJointLimit pJointBall::getTwistLowLimit()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pJointLimit();
	joint->saveToDesc(descr);
	return pJointLimit (descr.twistLimit.low.hardness,descr.twistLimit.low.restitution,descr.twistLimit.low.value);
}

pSpring pJointBall::getSwingSpring()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pSpring();
	joint->saveToDesc(descr);
	return pSpring (descr.swingSpring.damper,descr.swingSpring.spring,descr.swingSpring.targetValue);
}
pSpring pJointBall::getTwistSpring()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pSpring();
	joint->saveToDesc(descr);
	return pSpring (descr.twistSpring.damper,descr.twistSpring.spring,descr.twistSpring.targetValue);
}

pSpring pJointBall::getJointSpring()
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return pSpring();
	joint->saveToDesc(descr);
	return pSpring (descr.jointSpring.damper,descr.jointSpring.spring,descr.jointSpring.targetValue);
}
void pJointBall::enableFlag(int flag,bool enable)
{

	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return;
	joint->saveToDesc(descr);

	if (enable)
		descr.flags |=flag;
	else
		descr.flags &=~flag;

	joint->loadFromDesc(descr);
	return ;

}
void pJointBall::setSwingLimitAxis( const VxVector& swingLimitAxis )
{
	NxSphericalJointDesc descr;		
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.swingAxis = pMath::getFrom(swingLimitAxis);
	joint->loadFromDesc(descr);

}

void pJointBall::enableCollision( bool collision )
{

	NxSphericalJointDesc descr;		
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);
}

void pJointBall::setProjectionMode(ProjectionMode mode)
{
	NxSphericalJointDesc descr;		NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionMode = (NxJointProjectionMode)mode;
	joint->loadFromDesc(descr);
}

void pJointBall::setProjectionDistance(float distance)
{
	NxSphericalJointDesc descr;		NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionDistance= distance;
	joint->loadFromDesc(descr);
}

bool pJointBall::setSwingLimit( pJointLimit limit )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false ;
	joint->saveToDesc(descr);

	if ( limit.hardness != 0.0f || limit.restitution != 0.0f || limit.value !=0.0f )
	{
		enableFlag(NX_SJF_SWING_LIMIT_ENABLED,1);
		NxJointLimitDesc sLimit;	sLimit.value  = limit.value;sLimit.restitution = limit.restitution;sLimit.hardness = limit.hardness;
		if (!sLimit.isValid())return false;
		descr.swingLimit= sLimit;
		joint->loadFromDesc(descr);
		return true;
	}else
	{
		enableFlag(NX_SJF_SWING_LIMIT_ENABLED,0);
	}
	return false;
}
bool pJointBall::setTwistHighLimit( pJointLimit limit )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false ;
	joint->saveToDesc(descr);
	if ( limit.hardness != 0.0f || limit.restitution != 0.0f || limit.value !=0.0f )
	{

		enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,1);

		NxJointLimitDesc sLimit;	sLimit.value  = limit.value;sLimit.restitution = limit.restitution;sLimit.hardness = limit.hardness;
		if (!sLimit.isValid())return false;
		descr.twistLimit.high= sLimit;
		joint->loadFromDesc(descr);
		return true;
	}
	else
	{
		enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,0);
	}
	return 1;
}
bool pJointBall::setTwistLowLimit( pJointLimit limit )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false ;
	joint->saveToDesc(descr);

	if ( limit.hardness != 0.0f || limit.restitution != 0.0f || limit.value !=0.0f )
	{

		enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,1);

		NxJointLimitDesc sLimit;	sLimit.value  = limit.value;sLimit.restitution = limit.restitution;sLimit.hardness = limit.hardness;
		if (!sLimit.isValid())return false;
		descr.twistLimit.low= sLimit;
		joint->loadFromDesc(descr);
		return true;
	}
	else
	{
		enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,0);
	}
	return 1;
}
void pJointBall::setAnchor( const VxVector& anchor )
{
	NxSphericalJointDesc descr;		
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	joint->setGlobalAnchor(pMath::getFrom(anchor));
}

bool pJointBall::setSwingSpring( pSpring spring )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false;
	joint->saveToDesc(descr);

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	descr.swingSpring= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
bool pJointBall::setTwistSpring( pSpring spring )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false ;
	joint->saveToDesc(descr);

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	descr.twistSpring= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
bool pJointBall::setJointSpring( pSpring spring )
{
	NxSphericalJointDesc descr;	
	NxSphericalJoint *joint  = static_cast<NxSphericalJoint*>(getJoint());
	if (!joint)return false ;
	joint->saveToDesc(descr);

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	descr.jointSpring= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
