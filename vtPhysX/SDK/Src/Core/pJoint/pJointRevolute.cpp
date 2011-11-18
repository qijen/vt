#include <StdAfx.h>
#include "vtPhysXAll.h"


pJointRevolute::pJointRevolute(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Revolute)
{

}
bool pJointRevolute::setHighLimit(pJointLimit limit)
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());
	if (!joint)return false;
	joint->saveToDesc(descr);

	NxJointLimitDesc sLimit;	sLimit.value  = limit.value;sLimit.restitution = limit.restitution;sLimit.hardness = limit.hardness;
	if (!sLimit.isValid())return false;
	descr.limit.high= sLimit;

	if (sLimit.hardness!=0.0f || sLimit.restitution!=0.0f || sLimit.value !=0.0f )
	{
		descr.flags |= NX_RJF_LIMIT_ENABLED;
	}else
		descr.flags &=~NX_RJF_LIMIT_ENABLED;

	int v = descr.isValid();


	joint->loadFromDesc(descr);
	return true;
}
bool pJointRevolute::setLowLimit(pJointLimit limit)
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());
	if (!joint)return false;
	joint->saveToDesc(descr);

	NxJointLimitDesc sLimit;	sLimit.value  = limit.value;sLimit.restitution = limit.restitution;sLimit.hardness = limit.hardness;
	if (!sLimit.isValid())return false;
	descr.limit.low= sLimit;
	if (sLimit.hardness!=0.0f || sLimit.restitution!=0.0f || sLimit.value !=0.0f )
	{
		descr.flags |= NX_RJF_LIMIT_ENABLED;
	}else
		descr.flags &=~NX_RJF_LIMIT_ENABLED;

	bool ret = descr.isValid();
	joint->loadFromDesc(descr);
	return ret;
}

pSpring pJointRevolute::getSpring()
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());

	joint->saveToDesc(descr);
	return pSpring (descr.spring.damper,descr.spring.spring,descr.spring.targetValue);

}

bool pJointRevolute::setSpring(pSpring spring)
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	joint->saveToDesc(descr);
	if (!joint)return false ;
	joint->saveToDesc(descr);

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	descr.spring= sLimit;

	if (spring.damper!=0.0f || !spring.spring!=0.0f || !spring.targetValue !=0.0f )
	{
		descr.flags |= NX_RJF_SPRING_ENABLED;
	}else
		descr.flags &=~NX_RJF_SPRING_ENABLED;


	joint->loadFromDesc(descr);
	return false;
}



pMotor pJointRevolute::getMotor()
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	joint->saveToDesc(descr);
	
	NxMotorDesc mDescr = descr.motor;
	pMotor result;
	result.freeSpin = mDescr.freeSpin;
	result.targetVelocity= mDescr.velTarget;
	result.maximumForce = mDescr.maxForce;
	return result;
}

bool pJointRevolute::setMotor(pMotor motor)
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	joint->saveToDesc(descr);

	NxMotorDesc mDescr = descr.motor;

	if (motor.maximumForce!=0.0f && motor.targetVelocity !=0.0f )
	{
		mDescr.freeSpin = motor.freeSpin;
		mDescr.velTarget= motor.targetVelocity;
		mDescr.maxForce= motor.maximumForce;
		descr.flags |= NX_RJF_MOTOR_ENABLED;
		joint->setMotor(mDescr);
		descr.motor = mDescr;
	}else{
		descr.flags &=~NX_RJF_MOTOR_ENABLED;
	}
	joint->loadFromDesc(descr);

	return descr.isValid();
}
void pJointRevolute::enableCollision(bool collision)
{

	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);

}


void pJointRevolute::setGlobalAnchor(const VxVector& anchor)
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	joint->saveToDesc(descr);
	//descr.setGlobalAnchor(pMath::getFrom(anchor));
	//descr.localAnchor[0] = NxVec3(0,-40,0);
	//	joint->loadFromDesc(descr);

	
	joint->setGlobalAnchor(pMath::getFrom(anchor));

}

void pJointRevolute::setGlobalAxis(const VxVector& axis)
{
	NxRevoluteJointDesc descr;	
	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());
	//joint->saveToDesc(descr);
	//descr.setGlobalAxis(pMath::getFrom(axis));
	//joint->loadFromDesc(descr);

	joint->setGlobalAxis(pMath::getFrom(axis));
}


pJointLimit pJointRevolute::getHighLimit()
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());

	joint->saveToDesc(descr);
	return pJointLimit (descr.limit.high.hardness,descr.limit.high.restitution,descr.limit.high.value);

}

pJointLimit pJointRevolute::getLowLimit()
{
	NxRevoluteJointDesc descr;	
	NxRevoluteJoint*joint  = static_cast<NxRevoluteJoint*>(getJoint());

	joint->saveToDesc(descr);
	return pJointLimit (descr.limit.low.hardness,descr.limit.low.restitution,descr.limit.low.value);

}
void pJointRevolute::setProjectionMode(ProjectionMode mode)
{
	NxRevoluteJointDesc descr;		NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionMode = (NxJointProjectionMode)mode;
	joint->loadFromDesc(descr);
}

void pJointRevolute::setProjectionDistance(float distance)
{
	NxRevoluteJointDesc descr;		NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionDistance= distance;
	joint->loadFromDesc(descr);
}

void pJointRevolute::setProjectionAngle(float angle)
{
	NxRevoluteJointDesc descr;		NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionAngle= angle;
	int s = descr.isValid();
	joint->loadFromDesc(descr);
}