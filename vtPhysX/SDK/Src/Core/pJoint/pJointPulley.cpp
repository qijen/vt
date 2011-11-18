#include <StdAfx.h>
#include "vtPhysXAll.h"

pJointPulley::pJointPulley(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Pulley)
{

}

void pJointPulley::setLocalAnchorA(VxVector anchor)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.localAnchor[0] = pMath::getFrom(anchor);
	joint->loadFromDesc(descr);
}

VxVector pJointPulley::getLocalAnchorA()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return pMath::getFrom(descr.localAnchor[0]);
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::setLocalAnchorB(VxVector anchor)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.localAnchor[1] = pMath::getFrom(anchor);
	joint->loadFromDesc(descr);
}


VxVector pJointPulley::getLocalAnchorB()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return pMath::getFrom(descr.localAnchor[1]);
}

//////////////////////////////////////////////////////////////////////////
void pJointPulley::setPulleyA(VxVector pulley)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.pulley[0] = pMath::getFrom(pulley);
	joint->loadFromDesc(descr);
}

VxVector pJointPulley::getPulleyA()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return pMath::getFrom(descr.pulley[0]);
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::setPulleyB(VxVector pulley)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.pulley[1] = pMath::getFrom(pulley);
	joint->loadFromDesc(descr);
}
VxVector pJointPulley::getPulleyB()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return VxVector();
	joint->saveToDesc(descr);
	return pMath::getFrom(descr.pulley[1]);
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::setStiffness(float stiffness)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.stiffness= stiffness;
	joint->loadFromDesc(descr);
}
float pJointPulley::getStiffness()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return -1.0f;
	joint->saveToDesc(descr);
	return descr.stiffness;
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::setRatio(float ratio)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.ratio= ratio;
	joint->loadFromDesc(descr);
}
float pJointPulley::getRatio()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return -1.0f;
	joint->saveToDesc(descr);
	return descr.ratio;
}
//////////////////////////////////////////////////////////////////////////

void pJointPulley::setDistance(float distance)
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	descr.distance= distance;
	joint->loadFromDesc(descr);
}
float pJointPulley::getDistance()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return -1.0f;
	joint->saveToDesc(descr);
	return descr.distance;
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::setRigid( bool rigid )
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (rigid)
	{
		descr.flags|=NX_PJF_IS_RIGID;
	}else{
		descr.flags&=~NX_PJF_IS_RIGID;

	}
	joint->loadFromDesc(descr);
}
bool pJointPulley::isRigid()
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return -1.0f;
	joint->saveToDesc(descr);

	return (  descr.flags  & NX_PJF_IS_RIGID );
}
//////////////////////////////////////////////////////////////////////////


pMotor pJointPulley::getMotor()
{
	NxPulleyJointDesc descr;
	NxPulleyJoint *joint  = static_cast<NxPulleyJoint*>(getJoint());joint->saveToDesc(descr);
	NxMotorDesc mDescr = descr.motor;
	pMotor result;
	result.freeSpin = mDescr.freeSpin;
	result.targetVelocity= mDescr.velTarget;
	result.maximumForce = mDescr.maxForce;
	return result;
}
void pJointPulley::setMotor(pMotor motor)
{
	NxPulleyJointDesc descr;
	NxPulleyJoint *joint  = static_cast<NxPulleyJoint*>(getJoint());joint->saveToDesc(descr);

	NxMotorDesc mDescr = descr.motor;

	if (motor.maximumForce!=0.0f && motor.targetVelocity !=0.0f )
	{
		mDescr.freeSpin = motor.freeSpin;
		mDescr.velTarget= motor.targetVelocity;
		mDescr.maxForce= motor.maximumForce;
		descr.flags |= NX_PJF_MOTOR_ENABLED;
		joint->setMotor(mDescr);
		descr.motor = mDescr;
	}else{
		descr.flags &=~NX_PJF_MOTOR_ENABLED;
	}

	int flagsNow = descr.flags;
	int isValid  = descr.isValid();

	joint->loadFromDesc(descr);
}
//////////////////////////////////////////////////////////////////////////
void pJointPulley::enableCollision( bool collision )
{
	NxPulleyJointDesc descr;		
	NxPulleyJoint*joint  = static_cast<NxPulleyJoint*>(getJoint());	
	if (!joint)return ;	joint->saveToDesc(descr);
	if (collision)
	{
		descr.jointFlags  |= NX_JF_COLLISION_ENABLED;
	}else
		descr.jointFlags  &= ~NX_JF_COLLISION_ENABLED;
	joint->loadFromDesc(descr);
}
