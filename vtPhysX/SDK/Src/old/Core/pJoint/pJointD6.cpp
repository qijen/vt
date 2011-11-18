#include <StdAfx.h>
#include "vtPhysXAll.h"


using namespace vtAgeia;

pJointD6::pJointD6(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_D6)
{

}

void pJointD6::setDriveAngularVelocity(VxVector angVel)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return;	joint->saveToDesc(descr);
	//joint->setDriveAngularVelocity(pMath::getFrom(angVel));
	descr.driveAngularVelocity  = (pMath::getFrom(angVel));
	joint->loadFromDesc(descr);
}
//////////////////////////////////////////////////////////////////////////
void pJointD6::setDriveLinearVelocity(VxVector linVel)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	//joint->setDriveLinearVelocity(pMath::getFrom(linVel));
	descr.driveLinearVelocity = (pMath::getFrom(linVel));
	joint->loadFromDesc(descr);
}
//////////////////////////////////////////////////////////////////////////
void pJointD6::setDriveRotation(VxQuaternion rot)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	joint->setDriveOrientation(pMath::getFrom(rot));
	joint->loadFromDesc(descr);
}
//////////////////////////////////////////////////////////////////////////
void pJointD6::setDrivePosition(VxVector pos)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	joint->setDrivePosition(pMath::getFrom(pos));
	descr.drivePosition = (pMath::getFrom(pos));
	joint->loadFromDesc(descr);
}

//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getSlerpDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.slerpDrive.damping,descr.slerpDrive.spring,descr.slerpDrive.forceLimit,descr.slerpDrive.driveType.bitField);
	return result;
}

int pJointD6::setSlerpDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;	sdrive.driveType=drive.driveType;	descr.slerpDrive = sdrive;
	descr.flags |=NX_D6JOINT_SLERP_DRIVE;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getTwistDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.twistDrive.damping,descr.twistDrive.spring,descr.twistDrive.forceLimit,descr.twistDrive.driveType);
	return result;
}

int pJointD6::setTwistDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;		sdrive.driveType=drive.driveType;	descr.twistDrive = sdrive;

	joint->loadFromDesc(descr);
	return 1;
}

//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getSwingDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.swingDrive.damping,descr.swingDrive.spring,descr.swingDrive.forceLimit,descr.swingDrive.driveType);
	return result;
}

int pJointD6::setSwingDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;		sdrive.driveType=drive.driveType;	descr.swingDrive = sdrive;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getZDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.zDrive.damping,descr.zDrive.spring,descr.zDrive.forceLimit,descr.zDrive.driveType);
	return result;
}

int pJointD6::setZDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;		sdrive.driveType=drive.driveType;	descr.zDrive = sdrive;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getYDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.yDrive.damping,descr.yDrive.spring,descr.yDrive.forceLimit,descr.yDrive.driveType);
	return result;
}

int pJointD6::setYDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;		sdrive.driveType=drive.driveType;	descr.yDrive = sdrive;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
pJD6Drive pJointD6::getXDrive()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6Drive result(descr.xDrive.damping,descr.xDrive.spring,descr.xDrive.forceLimit,descr.xDrive.driveType);
	return result;
}


int pJointD6::setXDrive(pJD6Drive drive)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return -1 ;	joint->saveToDesc(descr);
	NxJointDriveDesc sdrive;		sdrive.damping  = drive.damping;		sdrive.spring  = drive.spring;		sdrive.forceLimit  = drive.forceLimit;		sdrive.driveType=drive.driveType;	descr.xDrive = sdrive;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////


pJD6SoftLimit pJointD6::getTwistLowLimit()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6SoftLimit result(descr.twistLimit.low.damping,descr.twistLimit.low.spring,descr.twistLimit.low.value,descr.twistLimit.low.restitution);
	return result;
}

int pJointD6::setTwistLowLimit(pJD6SoftLimit limit)
{
	NxD6JointDesc descr;	
	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return -1 ;
	joint->saveToDesc(descr);

	NxJointLimitSoftDesc sLimit;	sLimit.value  = limit.value;	sLimit.spring  = limit.spring;	sLimit.damping  = limit.damping;	sLimit.restitution = limit.restitution;
	if (!sLimit.isValid())return -1;
	descr.twistLimit.low= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
pJD6SoftLimit pJointD6::getTwistHighLimit()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6SoftLimit result(descr.twistLimit.high.damping,descr.twistLimit.high.spring,descr.twistLimit.high.value,descr.twistLimit.high.restitution);
	return result;
}


int pJointD6::setTwistHighLimit(pJD6SoftLimit limit)
{
	NxD6JointDesc descr;	
	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return -1 ;
	joint->saveToDesc(descr);

	NxJointLimitSoftDesc sLimit;	sLimit.value  = limit.value;	sLimit.spring  = limit.spring;	sLimit.damping  = limit.damping;	sLimit.restitution = limit.restitution;
	if (!sLimit.isValid())return -1;
	descr.twistLimit.high= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////

pJD6SoftLimit pJointD6::getSwing2Limit()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6SoftLimit result(descr.swing2Limit.damping,descr.swing2Limit.spring,descr.swing2Limit.value,descr.swing2Limit.restitution);
	return result;
}

int pJointD6::setSwing2Limit(pJD6SoftLimit limit)
{
	NxD6JointDesc descr;	
	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return -1 ;
	joint->saveToDesc(descr);

	NxJointLimitSoftDesc sLimit;	sLimit.value  = limit.value;	sLimit.spring  = limit.spring;	sLimit.damping  = limit.damping;	sLimit.restitution = limit.restitution;
	if (!sLimit.isValid())return -1;
	descr.swing2Limit= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////


pJD6SoftLimit pJointD6::getSwing1Limit()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6SoftLimit result(descr.swing1Limit.damping,descr.swing1Limit.spring,descr.swing1Limit.value,descr.swing1Limit.restitution);
	return result;
}

int pJointD6::setSwing1Limit(pJD6SoftLimit limit)
{
	NxD6JointDesc descr;	
	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return -1 ;
	joint->saveToDesc(descr);

	NxJointLimitSoftDesc sLimit;	sLimit.value  = limit.value;	sLimit.spring  = limit.spring;	sLimit.damping  = limit.damping;	sLimit.restitution = limit.restitution;
	if (!sLimit.isValid())return -1;
	descr.swing1Limit= sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////

pJD6SoftLimit pJointD6::getLinearLimit()
{
	NxD6JointDesc descr;NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());joint->saveToDesc(descr);
	pJD6SoftLimit result(descr.linearLimit.damping,descr.linearLimit.spring,descr.linearLimit.value,descr.linearLimit.restitution);
	return result;
}

int pJointD6::setLinearLimit(pJD6SoftLimit limit)
{
	NxD6JointDesc descr;	
	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return -1 ;
	joint->saveToDesc(descr);

	NxJointLimitSoftDesc sLimit;	sLimit.value  = limit.value;	sLimit.spring  = limit.spring;	sLimit.damping  = limit.damping;	sLimit.restitution = limit.restitution;
	if (!sLimit.isValid())return -1;
	descr.linearLimit = sLimit;
	joint->loadFromDesc(descr);
	return 1;
}
//////////////////////////////////////////////////////////////////////////
void pJointD6::setTwistMotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.twistMotion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
void pJointD6::setSwing1MotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.swing1Motion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
void pJointD6::setSwing2MotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.swing2Motion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
void pJointD6::setXMotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.xMotion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
void pJointD6::setYMotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.yMotion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
void pJointD6::setZMotionMode(D6MotionMode mode)
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());if (!joint)return;joint->saveToDesc(descr);
	descr.zMotion = (NxD6JointMotion)mode;
	joint->loadFromDesc(descr);
}
D6MotionMode pJointD6::getTwist()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.twistMotion;
}
D6MotionMode pJointD6::getSwing1()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.swing1Motion;
}
D6MotionMode pJointD6::getSwing2()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.swing2Motion;
}
D6MotionMode pJointD6::getXMotion()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.xMotion;
}
D6MotionMode pJointD6::getYMotion()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.yMotion;
}
D6MotionMode pJointD6::getZMotion()
{
	NxD6JointDesc descr;	NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());
	if (!joint)return D6MM_Locked;
	joint->saveToDesc(descr);
	return (D6MotionMode)descr.zMotion;
}


void pJointD6::setGlobalAnchor(VxVector anchor)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	joint->setGlobalAnchor(pMath::getFrom(anchor));
}
void pJointD6::setGlobalAxis(VxVector axis)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	joint->setGlobalAxis(pMath::getFrom(axis));
	//joint->setGlobalA(pMath::getFrom(anchor));
}
void pJointD6::setRatio(float ratio)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	if (ratio!=0.0f)
	{
		descr.jointFlags|=NX_D6JOINT_GEAR_ENABLED;
		descr.gearRatio = ratio;
	}else
	{
		descr.jointFlags&=~NX_D6JOINT_GEAR_ENABLED;
	}
	joint->loadFromDesc(descr);
}

void pJointD6::enableCollision( bool value )
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	if (value)
	{
		descr.jointFlags|=NX_JF_COLLISION_ENABLED;
	}else
	{
		descr.jointFlags&=~NX_JF_COLLISION_ENABLED;
	}
	joint->loadFromDesc(descr);
}

void pJointD6::setProjectionMode(ProjectionMode mode)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionMode = (NxJointProjectionMode)mode;
	joint->loadFromDesc(descr);
}

void pJointD6::setProjectionDistance(float distance)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionDistance= distance;
	joint->loadFromDesc(descr);
}

void pJointD6::setProjectionAngle(float angle)
{
	NxD6JointDesc descr;		NxD6Joint *joint  = static_cast<NxD6Joint*>(getJoint());	if (!joint)return ;	joint->saveToDesc(descr);
	descr.projectionAngle= angle;
	joint->loadFromDesc(descr);
}