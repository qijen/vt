#ifndef __PJOINTTYPES_H__
#define __PJOINTTYPES_H__


/** \addtogroup Joints
@{
*/


/**
\brief Describes a joint motor. Some joints can be motorized, this allows them to apply a force to cause attached actors to move. Joints which can be motorized: #pJointPulley #pJointRevolute is used for a similar purpose with pJointD6.

*/
class pMotor
{

public :

	/**
	The relative velocity the motor is trying to achieve.Default = 0.0f.
	*/
	float targetVelocity;
	/**
	The maximum force (or torque) the motor can exert.Default = 0.0f.
	*/
	float maximumForce;
	/**
	If true, motor will not brake when it spins faster than velTarget.Default = false.
	*/
	bool freeSpin;

	pMotor()
	{
		targetVelocity = 0.0f;
		maximumForce =0.0f;
		freeSpin = false;
	}

};
/**
\brief Describes a joint limit. pJointLimit is registered as custom structure #pJLimit and can be accessed or modified through parameter operations.<br>

This is used for ball joints. 
\sa \ref PJBall.

*/
class pJointLimit
{

public:

	/**
	[not yet implemented!] Limit can be made softer by setting this to less than 1. Default = 0.0f;
	*/
	float hardness;
	/**
	The angle / position beyond which the limit is active. Default = 0.0f;
	*/
	float value;
	/**
	The limit bounce. Default = 0.0f;
	*/
	float restitution;
	pJointLimit()
	{
		hardness = 0.0f;
		value = 0.0f;
		restitution  = 0.0f;
	}

	pJointLimit(float _h,float _r, float _v)
	{
		hardness  = _h;
		restitution  = _r;
		value = _v;
	}

};



/**
\brief Describes a joint soft limit for D6 usage. pJD6SoftLimit is registered as custom structure #pJD6SLimit and can be accessed or modified through parameter operations.<br>
\sa #PJD6.
*/
class pJD6SoftLimit
{



public:
	/**\brief If spring is greater than zero, this is the damping of the spring. 
	*/
	float damping;
	/**\brief If greater than zero, the limit is soft, i.e. a spring pulls the joint back to the limit.

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 0.0
	*/
	float spring;
	/**\brief The angle or position beyond which the limit is active. 

	Which side the limit restricts depends on whether this is a high or low limit.

	<b>Unit:</b> Angular: Radians
	<b>Range:</b> Angular: (-PI,PI)<br>
	<b>Range:</b> Positional: [0.0,inf)<br>
	<b>Default:</b> 0.0
	*/
	float value;
	/**\brief Controls the amount of bounce when the joint hits a limit. 
	<br>
	<br>
	A restitution value of 1.0 causes the joint to bounce back with the velocity which it hit the limit. A value of zero causes the joint to stop dead.

	In situations where the joint has many locked DOFs (e.g. 5) the restitution may not be applied correctly. This is due to a limitation in the solver which causes the restitution velocity to become zero as the solver enforces constraints on the other DOFs.

	This limitation applies to both angular and linear limits, however it is generally most apparent with limited angular DOFs.

	Disabling joint projection and increasing the solver iteration count may improve this behavior to some extent.

	Also, combining soft joint limits with joint motors driving against those limits may affect stability.

	<b>Range:</b> [0,1]<br>
	<b>Default:</b> 0.0

	*/
	float restitution;
	pJD6SoftLimit()
	{
		damping = 0.0f;
		spring = 0.0f;
		value = 0.0f;
		restitution  = 0.0f;
	}
	pJD6SoftLimit(float _damping,float _spring,float _value,float _restitution)
	{

		damping = _damping;
		spring = _spring;
		value = _value;
		restitution = _restitution;
	}

};
/**
\brief Class used to describe drive properties for a #pJointD6.
*/
class pJD6Drive
{

public:

	/**
	Damper coefficient
	<b>Default:</b> 0
	<b>Range:</b> [0,inf)
	*/
	float damping;
	/**
	Spring coefficient

	<b>Default:</b> 0
	<b>Range:</b> (-inf,inf)
	*/
	float spring;
	/**
	The maximum force (or torque) the drive can exert.

	<b>Default:</b> FloatMax
	<b>Range:</b> [0,inf)
	*/
	float forceLimit;
	/**
	Type of drive to apply.See #D6DriveType.
	<b>Default:</b> FloatMax
	<b>Range:</b> [0,inf)
	*/
	int driveType;

	pJD6Drive()
	{
		damping = 0.0f;
		spring = 0.0f;
		forceLimit = 3.402823466e+38F;
		driveType = 0;

	}
	pJD6Drive(float _damping,float _spring,float _forceLimit,int _driveType)
	{
		damping = _damping;
		spring = _spring;
		forceLimit  = _forceLimit;
		driveType = _driveType;
	}

};
/** @} */

#endif // __PJOINTTYPES_H__