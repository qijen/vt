#ifndef __PJOINT_H_
#define __PJOINT_H_

#include "vtPhysXBase.h"

/** \addtogroup Joints
@{
*/

/**
\brief Abstract base class for the different types of joints.

All joints are used to connect two dynamic bodies, or an body and the environment.

A NULL body represents the environment. Whenever the below comments mention two bodies,
one of them may always be the environment (NULL).

*/
class MODULE_API pJoint {

	
	public:
		

		pJoint(){}
		pJoint(CK3dEntity* _e);
		pJoint(pRigidBody* _a,pRigidBody* _b,int _type);
        
		NxJoint *getJoint() { return mJoint;}
		void setJoint(NxJoint*j) { mJoint = j ;}

		//////////////////////////////////////////////////////////////////////////
		protected : 

			pRigidBody* m_SolidA;
			pRigidBody* m_SolidB;
			CK3dEntity* m_vtObjectA;
			

			CK3dEntity* m_vtObjectB;
			
			int m_type;
			
			pWorld *m_pWorld;
			NxJoint *mJoint;
			

			
	
		//////////////////////////////////////////////////////////////////////////
		public :
		
		/**
		\brief Retrieve the type of this joint.
		\return The type of joint.

		\see NxJointType
		*/
		JType getType() const { return (JType)m_type; }
		
		CK3dEntity* GetVTEntA(){ return m_vtObjectA;}
		CK3dEntity* GetVTEntB(){ return m_vtObjectB;}
		pRigidBody *GetSolidA();
		pRigidBody *GetSolidB();

		CK_ID mAID;
		CK_ID mBID;
		
		
		
		pWorld * getWorld() const { return m_pWorld; }
		void setWorld(pWorld * val) { m_pWorld = val; }
		
			
		/**
		\brief Sets the maximum force magnitude that the joint is able to withstand without breaking.

		There are two values, one for linear forces, and one for angular forces. Both values are used directly
		as a value for the maximum impulse tolerated by the joint constraints. 

		Both force values are MaxFloat by default. This setting makes the joint unbreakable. 
		The values should always be nonnegative.

		The distinction between maxForce and maxTorque is dependent on how the joint is implemented internally, 
		which may not be obvious. For example what appears to be an angular degree of freedom may be constrained 
		indirectly by a linear constraint.

		So in most practical applications the user should set both maxTorque and maxForce to low values.

		<b>Sleeping:</b> This call wakes the body(s) if they are sleeping.

		\param[in] maxForce Maximum force the joint can withstand without breaking. <b>Range:</b> (0,inf]
		\param[in] maxTorque Maximum torque the joint can withstand without breaking. <b>Range:</b> (0,inf]

		*/
		void setBreakForces(float maxForce,float maxTorque);

		/**
		\brief Retrieves the max forces of a breakable joint. See #setBreakable().

		\param[out] maxForce Retrieves the maximum force the joint can withstand without breaking.
		\param[out] maxTorque Retrieves the maximum torque the joint can withstand without breaking.
		*/
		void getBreakForces(float& maxForce,float& maxTorque);

		/**
		\brief Adds a limit plane.

		Both of the parameters are given in global coordinates. see setLimitPoint() for the meaning of limit planes.

		The plane is affixed to the	body that does not have the limit point.

		The normal of the plane points toward the positive side of the plane, and thus toward the
		limit point. If the normal points away from the limit point at the time of this call, the
		method returns false and the limit plane is ignored.

		\note This function always returns true and adds the limit plane unlike earlier versions. This behavior
		was changed to allow the joint to be serialized easily.

		<b>Sleeping:</b> This call wakes the body(s) if they are sleeping.

		\param[in] normal Normal for the limit plane in global coordinates. <b>Range:</b> direction vector
		\param[in] pointInPlane Point in the limit plane in global coordinates. <b>Range:</b> position vector
		\param[in] restitution Restitution of the limit plane.
		<b>Range:</b> [0.0, 1.0]
		<b>Default:</b> 0.0
		\return Always true.

		\see setLimitPoint() purgeLimitPlanes() getNextLimitPlane()
		*/
		int addLimitPlane(const VxVector normal, VxVector pointInPlane, float restitution=0.0f);

		/**
		\brief deletes all limit planes added to the joint.

		Invalidates limit plane iterator.

		<b>Sleeping:</b> Does <b>NOT</b> wake the associated body up automatically.

		\see addLimitPlane() getNextLimitPlane()
		*/
		void purgeLimitPlanes();
		/**
		\brief Restarts the limit plane iteration.

		Call before starting to iterate. This method may be used together with
		the below two methods to enumerate the limit planes.
		This iterator becomes invalid when planes
		are added or removed, or the plane iterator mechanism is
		invoked on another joint.

		\see hasMoreLimitPlanes() getNextLimitPlane()
		*/
		void resetLimitPlaneIterator();
		/**
		\brief Returns true until the iterator reaches the end of the set of limit planes.

		Adding or removing elements does not reset the iterator.

		\return True if the iterator has not reached the end of the sequence of limit planes.

	
		\see resetLimitPlaneIterator() getNextLimitPlane()
		*/
		int hasMoreLimitPlanes ();

		/**
		\brief Returns the next element pointed to by the limit plane iterator, and increments the iterator.

		Places the global frame plane equation (consisting of normal and d, the 4th
		element) coefficients in the argument references. The plane equation is of the form:

		dot(n,p) + d == 0 (n = normal, p = a point on the plane)

		\param[out] planeNormal Used to store the plane normal.
		\param[out] planeD Used to store the plane 'D'.
		\param[out] restitution Optional, used to store restitution of the limit plane.
		\return Returns true if the limit plane is satisfied.

		\see resetLimitPlaneIterator() hasMoreLimitPlanes()
		*/
		int getNextLimitPlane (VxVector &planeNormal, float &planeD,float *restitution=NULL);
		/**
		\brief Sets the limit point.

		The point is specified in the global coordinate frame.

		All types of joints may be limited with the same system:
		You may elect a point attached to one of the two bodies to act as the limit point.
		You may also specify several planes attached to the other body.

		The points and planes move together with the body they are attached to.

		The simulation then makes certain that the pair of bodies only move relative to each other 
		so that the limit point stays on the positive side of all limit planes.

		the default limit point is (0,0,0) in the local frame of actor2.
		Calling this deletes all existing limit planes.

		<b>Sleeping:</b> This call wakes the body(s) if they are sleeping.

		\param[in] point The limit reference point defined in the global frame. <b>Range:</b> position vector
		\param[in] pointIsOnActor2 if true the point is attached to the second body. Otherwise it is attached to the first.

		\sa getLimitPoint() addLimitPlane()
		*/
		void setLimitPoint(VxVector point,bool pointIsOnActor2=true);
		
		/**
		\brief Retrieves the global space limit point.

		Returns true if the point is fixed on actor2.

		\param[out] worldLimitPoint Used to store the global frame limit point. 
		\return True if the point is fixed to body 2 otherwise the point is fixed to body 1.

		\see setLimitPoint() addLimitPlane()
		*/
		bool getLimitPoint(VxVector & worldLimitPoint);


		bool IsValid();
		CKContext *context;

		pJointD6 *castD6Joint();
		pJointFixed*castFixed();
		pJointDistance*castDistanceJoint();
		pJointBall *castBall();
		pJointPulley *castPulley();
		pJointRevolute *castRevolute();
		pJointPrismatic *castPrismatic();
		pJointCylindrical *castCylindrical();
		pJointPointInPlane *castPointInPlane();
		pJointPointOnLine *castPointOnLine();

		void setLocalAnchor0(VxVector anchor0);
		void setLocalAnchor1(VxVector anchor1);

		VxVector getGlobalAxis();
		void setGlobalAxis(VxVector axis);

		VxVector getGlobalAnchor();
		void setGlobalAnchor(VxVector anchor);


		int getNbLimitPlanes();


		virtual void enableCollision(int collision){}

		virtual ~pJoint(){}

};
/**
\brief A prismatic joint permits relative translational movement between two bodies along
an axis, but no relative rotational movement.

\image html prismJoint.png

<h3>Creation</h3>

\include NxPrismaticJoint_Create.cpp

\sa pFactory::createPrismaticJoint()
*/
class MODULE_API pJointPrismatic : public pJoint
{

public:
	pJointPrismatic(pRigidBody* _a,pRigidBody* _b);


	void setGlobalAxis(VxVector axis);
	void setGlobalAnchor(VxVector anchor);
	void enableCollision(int collision);


};
/**
\brief Cylindrical Joints permit relative translational movement between two bodies along
an axis, and also relative rotation along the axis.

\image html cylinderJoint.png


\sa pFactory::createCylindricalJoint()
*/
class MODULE_API pJointCylindrical: public pJoint
{

public:
	pJointCylindrical(pRigidBody* _a,pRigidBody* _b);

	void setGlobalAxis(VxVector axis);
	void setGlobalAnchor(VxVector anchor);
	void enableCollision(int collision);


};

/**
\brief A point in plane joint constrains a point on one actor to only move inside a plane attached to another actor. The point attached to the plane is defined by the anchor point. The joint's axis specifies the plane normal.

	DOFs removed: 1 <br>
	DOFs remaining: 5<br>

\image html pointInPlaneJoint.png


\sa pFactory::createPointInPlaneJoint()
*/
class MODULE_API pJointPointInPlane: public pJoint
{

public:
	pJointPointInPlane(pRigidBody* _a,pRigidBody* _b);

	/**
	\brief Sets the global axis. 
	\param[in] axis The new axis.
	*/
	void setGlobalAxis(VxVector axis);
	/**
	\brief Sets the global anchor. 
	\param[in] anchor The new anchor.
	*/
	void setGlobalAnchor(VxVector anchor);
	/**
	\brief Enables collision between both bodies.
	\param[in] collision Collision is enabled.	
	*/
	void enableCollision(int collision);


};
/**
\brief A point on line joint constrains a point on one actor to only move along a line attached to another actor. The point attached to the line is the anchor point for the joint. The line through this point is specified by its direction (axis) vector.

DOFs removed: 2 <br>
DOFs remaining: 4<br>

\image html pointOnLineJoint.png


\sa pFactory::createPointInPlaneJoint()
*/
class MODULE_API pJointPointOnLine: public pJoint
{

public:
	pJointPointOnLine(pRigidBody* _a,pRigidBody* _b);

	/**
	\brief Sets the global axis. 
	\param[in] axis The new axis.
	*/
	void setGlobalAxis(VxVector axis);
	/**
	\brief Sets the global anchor. 
	\param[in] anchor The new anchor.
	*/
	void setGlobalAnchor(VxVector anchor);
	/**
	\brief Enables collision between both bodies.
	\param[in] collision Collision is enabled.	
	*/
	void enableCollision(int collision);

};

/**
\brief A joint which behaves in a similar way to a hinge or axle.

\image html revoluteJoint.png

A hinge joint removes all but a single rotational degree of freedom from two objects.
The axis along which the two bodies may rotate is specified with a point and a direction
vector.

An example for a revolute joint is a door hinge. Another example would be using a revolute 
joint to attach rotating fan blades to a ceiling. The revolute joint could be motorized, 
causing the fan to rotate.

<h4>Revolute Joint Limits</h4>

A revolute joint allows limits to be placed on how far it rotates around the joint axis. For example, a hinge on a door cannot rotate through 360 degrees; rather, it can rotate between 20 degrees and 180 degrees.
The angle of rotation is measured using the joints normal (axis orthogonal to the joints axis). This is the angle reported by NxRevoluteJoint::getAngle(). The limits are specified as a high and low limit, which must satisfy the condition -Pi < low < high <Pi degrees.
Below are valid revolute joint limits in which the joint is able to move between low and high:

\image html revoluteJointLimits.png
<br>

Note : The white region represents the allowable rotation for the joint.

<h4>Limitations of Revolute Joint Limits</h4>
As shown below, it is not possible to specify certain limit configurations without rotating the joint axes, due to the restrictions on the values of low and high: 
\image html revoluteLimitLimitation.png

To achieve this configuration, it is necessary to rotate the joint counter-clockwise so that low is below the 180 degree line. 

NOTE: If the angular region that is prohibited by the twist limit (as in the above figures) is very small, only a few degrees or so, then the joint may "push through" the limit and out on the other side if the relative angular velocity is large enough in relation to the time step. Care must be taken to make sure the limit is "thick" enough for the typical angular velocities it will be subjected to.

\sa pFactory::createRevoluteJoint()
*/
class MODULE_API pJointRevolute : public pJoint
{

public:
	pJointRevolute(pRigidBody* _a,pRigidBody* _b);



	/**
	\brief Sets the global axis. 
	\param[in] axis The new axis.
	*/
	void setGlobalAxis(const VxVector& axis);
	/**
	\brief Sets the global anchor. 
	\param[in] anchor The new anchor.
	*/
	void setGlobalAnchor(const VxVector& anchor);

	/**
	\brief Retrieves the optional spring.
	\return The spring.
	\sa setSpring()
	*/
	pSpring getSpring();
	/**
	\brief Sets an optional spring.
	\param[in] spring The spring.
	\return TRUE if spring was valid.
	\sa getSpring()
	*/
	bool setSpring(pSpring spring);


	/**
	\brief Retrieves an optional high limit for angular motion of the joint.
	\sa setHighLimit() 
	*/
	pJointLimit getHighLimit();
	/**
	\brief Sets an optional high limit for angular motion of the joint.
	\param[in] limit The new high limit.
	\return True if the limit was valid.
	\sa getLowLimit() 
	*/
	bool setHighLimit(pJointLimit limit);
	/**
	\brief Retrieves an optional low limit for angular motion of the joint.
	\sa setLowLimit() 
	*/
	pJointLimit getLowLimit();
	/**
	\brief Sets an optional high limit for angular motion of the joint.
	\param[in] limit The new low limit.
	\return True if the limit was valid.
	\sa setLowLimit() 
	*/
	bool setLowLimit(pJointLimit limit);

	/**
	\brief Sets a motor.
	\param[in] motor The new motor settings.
	\return True if the motor was valid.
	\sa getMotor() 
	*/
	bool setMotor(pMotor motor);
	/**
	\brief Retrieves a motor.
	\return The motor settings
	\sa setMotor() 
	*/
	pMotor getMotor();
	/**
	\brief Enables collision between both bodies.
	\param[in] collision Collision is enabled.	
	*/
	void enableCollision(bool collision);
	void setProjectionMode(ProjectionMode mode);
	void setProjectionDistance(float distance);
	void setProjectionAngle(float angle);




protected:
private:

};


/*!
 * \brief A pulley joint simulates a rope between two objects passing over 2 pulleys. 
 * 
 *	\image html pulleyJoint.png 
 *
 * 
 The pulley joint simulates a rope that can be thrown across a pair of pulleys. In this way, it is similar to the 
 distance joint (the length of the rope is the distance) but the rope doesn't connect the two bodies along the 
 shortest path, rather it leads from the connection point on one actor to the pulley point (fixed in world space), 
 then to the second pulley point, and finally to the other actor.
 
 The pulley joint can also be used to simulate a rope around a single point by making the pulley points coincide.


 <b>Note</b> that a setup where either object attachment point coincides with its corresponding pulley suspension point in 
	world space is invalid. In this case, the simulation would be unable to determine the appropriate direction in which to pull the 
	object and a random direction would result. The simulation will be unstable. Note that it is also invalid to allow the simulation 
	to end up in such a state. 

*/
class MODULE_API pJointPulley : public pJoint
{

public:
	pJointPulley(pRigidBody* _a,pRigidBody* _b);

	/**
	\brief Sets the attachment point of joint in bodie[0]'s space.
	
	\param[in] VxVector anchor
	
	\return void
	
	@see pJointPulley::setLocalAnchorB
	*/
	void setLocalAnchorA(VxVector anchor);
	/**
	\brief Sets the attachment point of joint in bodie[1]'s space.
	\param[in] VxVector anchor

	\return void

	@see pJointPulley::setLocalAnchorB
	*/
	void setLocalAnchorB(VxVector anchor);

	/**
	\brief Returns the attachment point of joint in bodie[0]'s space.

	\return VxVector

	@see pJointPulley::setLocalAnchorB
	*/
	VxVector getLocalAnchorA();
	/**
	\brief Returns the attachment point of joint in bodie[0]'s space.

	\param[in] VxVector anchor

	\return VxVector

	@see pJointPulley::setLocalAnchorA
	*/
	VxVector getLocalAnchorB();

	/**
	\brief Sets the suspension point of joint in world space.

	\param[in] VxVector pulley

	\return void

	@see pJointPulley::setPulleyB
	*/
	void setPulleyA(VxVector pulley);
	/**
	\brief Sets the suspension point of joint in world space.

	\param[in] VxVector pulley

	\return void

	@see pJointPulley::setPulleyA
	*/
	void setPulleyB(VxVector pulley);

	/**
	\brief Returns the suspension point of joint in world space.

	\return VxVector

	@see pJointPulley::getPulleyB
	*/
	VxVector getPulleyA();
	/**
	\brief Returns the suspension point of joint in world space.

	\return VxVector

	@see pJointPulley::getPulleyA
	*/
	VxVector getPulleyB();

	/**
	\brief Sets how stiff the constraint is, between 0 and 1 (stiffest).
	
	\param[in] float stiffness
		-	<b>Range:</b> [0,1.0f)  <br>
		-	<b>Default:</b> 1.0f
	
	\return void
	
	@see getStiffness()
	*/
	void setStiffness(float stiffness);

	/**
	\brief Returns how stiff the constraint is, between 0 and 1 (stiffest).
	
	\return float
	
	@see setStiffness()
	*/
	float getStiffness();
	

	/**
	\brief Sets transmission ratio.
	
	\param[in] float ratio
		-	<b>Range:</b> [0,1.0f)  <br>
		-	<b>Default:</b> 1.0f

	\return void
	
	@see ::getRatio()
	*/
	void setRatio(float ratio);

	/**
	\brief  Gets transmission ratio.
	
	\return float
	
	@see setRatio
	*/
	float getRatio();


	/**
	\brief Set true if the joint also has to maintain a minimum distance, not just a maximum.
	 
	\param[in] bool rigid
	-	<b>Range:</b> [true,false)  <br>
	-	<b>Default:</b>false	
	\return void
	
	@see ::isRigid()
	*/
	void setRigid(bool rigid);


	/**
	\brief Returns true if the joint also has to maintain a minimum distance, not just a maximum.
	
	\return bool
	
	@see 
	*/
	bool isRigid();

	/**
	\brief 
	
	Sets the rest length of the rope connecting the two objects.

	The distance is computed as ||(pulley0 - anchor0)|| + ||(pulley1 - anchor1)|| * ratio.

	\param[in] float distance
	-	<b>Range:</b> [0,inf)  <br>
	-	<b>Default:</b> 0.0f
	
	\return void
	
	@see getDistance()
	*/
	void setDistance(float distance);

	/**
	\brief Returns the rest length of the rope connecting the two objects.

	\return float
	
	@see setDistance()
	*/
	float getDistance();
	

	/**
	\brief Sets motor parameters for the joint. 
	
	\param[in] pMotor motor
	-	<b>Range:</b> [#pMotor)  <br>
	-	<b>Default:</b> no motor

	\return void
	
	@see getMotor()
	*/
	void setMotor(pMotor motor);
	
	/**
	\brief Returns motor parameter for the joint.

	\return pMotor
	
	@see setMotor()
	*/
	pMotor getMotor();


	/**
	\brief For convenience only. For the case the pulley hooks are moving, the SDK corrects the these points according to	reference entities.
	
	\return void
	
	@see 
	*/
	void setFollowPulleyReferences(bool follow);

	/**
	\brief Sets the first pulley.
		
	\return void
	
	@see setPulleyBReference()
	*/
	void setPulleyAReference(CK3dEntity*ent);

	/**
	\brief Sets the second pulley.

	\return void

	@see setPulleyAReference()
	*/
	void setPulleyBReference(CK3dEntity*ent);


	/**
	\brief Returns the first pulley reference.

	\return void

	@see setPulley1Ref()
	*/
	CK3dEntity *getPulleyAReference();

	/**
	\brief Sets the second pulley reference

	\return void

	@see setPulley1Ref()
	*/
	CK3dEntity *getPulleyBReference();

	/**
	\brief Enables collision between both bodies.

	\return void

	@see setPulley1Ref()
	*/
	void enableCollision(bool collision);


private :
	CK_ID m_Pulley0Reference0;
	CK_ID m_Pulley0Reference1;
	bool mFollowPulleyReferences;




	
	public :
};

/**
\brief A D6 joint is a general constraint between two actors. 

	It allows the user to individually define the linear and rotational degrees of freedom. 
	It also allows the user to configure the joint with limits and driven degrees of freedom as they wish.

*/
class MODULE_API pJointD6 : public pJoint
{

public:
		pJointD6(pRigidBody* _a,pRigidBody* _b);


		/**
		\brief Sets the motion mode for the twist axis.
		\param[in] mode The new mode.
		\sa getTwistMotionMode() 
		*/
		void setTwistMotionMode(D6MotionMode mode);	
		D6MotionMode getTwist();
		/**
		\brief Sets the motion mode for the swing1 axis.
		\param[in] mode The new mode.
		\sa getSwing1MotionMode() 
		*/
		void setSwing1MotionMode(D6MotionMode mode);	
		D6MotionMode getSwing1();
		/**
		\brief Sets the motion mode for the swing2 axis.
		\param[in] mode The new mode.
		\sa getSwing2MotionMode() 
		*/
		void setSwing2MotionMode(D6MotionMode mode);	
		D6MotionMode getSwing2();
		/**
		\brief Sets the motion mode for the linear x axis.
		\param[in] mode The new mode
		\sa getXMotionMode() 
		*/
		void setXMotionMode(D6MotionMode mode);	
		D6MotionMode getXMotion();
		/**
		\brief Sets the motion mode for the linear y axis.
		\param[in] mode The new mode
		\sa getZMotionMode() 
		*/
		void setYMotionMode(D6MotionMode mode);	
		D6MotionMode getYMotion();
		/**
		\brief Sets the motion mode for the linear z axis.
		\param[in] mode The new mode
		\sa getZMotionMode() 
		*/
		void setZMotionMode(D6MotionMode mode);	
		D6MotionMode getZMotion();

		
		int setLinearLimit(pJD6SoftLimit limit);				pJD6SoftLimit getLinearLimit();
		int setSwing1Limit(pJD6SoftLimit limit);				pJD6SoftLimit getSwing1Limit( );
		int setSwing2Limit(pJD6SoftLimit limit);				pJD6SoftLimit getSwing2Limit();
		int setTwistLowLimit(pJD6SoftLimit value);		pJD6SoftLimit getTwistLowLimit();
		int setTwistHighLimit(pJD6SoftLimit value);		pJD6SoftLimit getTwistHighLimit();

		pJD6Drive getXDrive();										int setXDrive(pJD6Drive drive);
		pJD6Drive getYDrive();										int setYDrive(pJD6Drive drive);
		pJD6Drive getZDrive();										int setZDrive(pJD6Drive drive);

		pJD6Drive getSwingDrive();								int setSwingDrive(pJD6Drive drive);
		pJD6Drive getTwistDrive();int setTwistDrive(pJD6Drive drive);
		pJD6Drive getSlerpDrive();int setSlerpDrive(pJD6Drive drive);

		/**\brief If the type of xDrive (yDrive,zDrive) is #D6DT_Position, drivePosition defines the goal position.

			<b>Range:</b> position vector<br>
			<b>Default:</b> Zero
		*/
		void setDrivePosition(VxVector pos);
	
		/**\brief If the type of swingDrive or twistDrive is #D6DT_Position, driveOrientation defines the goal orientation.

			<b>Range:</b> position vector<br>
			<b>Default:</b> Zero
		*/
		void setDriveRotation(VxQuaternion rot);
	
		/**\brief If the type of xDrive (yDrive,zDrive) is D6DT_Velocity, driveLinearVelocity defines the goal linear velocity.

		<b>Range:</b> unit quaternion<br>
		<b>Default:</b> Identity Quaternion

		*/
		void setDriveLinearVelocity(VxVector linVel);    
	
		/**\brief  If the type of swingDrive or twistDrive is D6DT_Velocity, driveAngularVelocity defines the goal angular velocity.
		
		- driveAngularVelocity.x - goal angular velocity about the twist axis
		- driveAngularVelocity.y - goal angular velocity about the swing1 axis
		- driveAngularVelocity.z - goal angular velocity about the swing2 axis


		<b>Range:</b> angular velocity vector<br>
		<b>Default:</b> Zero
		
		*/
		void setDriveAngularVelocity(VxVector angVel);

		/**
		\brief Enables collision between the two bodies.
		\param[in] Collide or not. 
		*/
		void enableCollision(bool value);
		/**
		\brief Sets the global anchor. 
		\param[in] anchor The new anchor.
		*/
		void setGlobalAnchor(VxVector anchor);
		/**
		\brief Sets the global axis. 
		\param[in] axis The new axis.
		*/
		void setGlobalAxis(VxVector axis);
		void setRatio(float ratio);
		void setProjectionMode(ProjectionMode mode);
		void setProjectionDistance(float distance);
		void setProjectionAngle(float angle);

		protected:
		private:

};



/**
\brief A distance joint maintains a certain distance between two points on two bodies.

\image html distanceJoint.png

\sa pFactory::createDistanceJoint()
*/
class MODULE_API pJointDistance : public pJoint
{

	public:
		pJointDistance(pRigidBody* _a,pRigidBody* _b);

		/**
		\brief Sets the minimum rest length of the rope or rod between the two anchor points.
		\param[in] distance The new rest length. The value must be non-zero!
		\sa getMinDistance()
		*/
		void setMinDistance(float distance);
		/**
		\brief Sets the maximum rest length of the rope or rod between the two anchor points.
		\param[in] distance The new rest length.The value must be non-zero!
		\sa getMaxDistance()
		*/
		void setMaxDistance(float distance);
		/**
		\brief Sets the attachment point of the joint in bodies[0] space.
		\param[in] anchor The new anchor.
		\sa getLocalAnchor0()
		*/
		void setLocalAnchor0(VxVector anchor);
		/**
		\brief Sets the attachment point of the joint in bodies[1] space.
		\param[in] anchor The new anchor.
		\sa getLocalAnchor1()
		*/
		void setLocalAnchor1(VxVector anchor);
		/**
		\brief Retrieves the attachment point of the joint in bodies[1] space.
		\return anchor The local anchor 0 .
		\sa setLocalAnchor0()
		*/
		VxVector getLocalAnchor0();
		/**
		\brief Retrieves the attachment point of the joint in bodies[1] space.
		\return The local anchor 1.
		\sa setLocalAnchor1()
		*/
		VxVector getLocalAnchor1();
		/**
		\brief Retrieves the minimum rest length of the rope or rod between the two anchor points.
		\return The minimum distance amongst both bodies. 
		\sa setMinDist()
		*/
		float getMinDistance();
		/**
		\brief Retrieves the maximum rest length of the rope or rod between the two anchor points.
		\return The maximum distance amongst both bodies. 
		\sa setMaxDist()
		*/
		float getMaxDistance();
		/**
		\brief Retrieves the spring which keeps both bodies springy.
		\return The spring.
		\sa setSpring()
		*/
		pSpring getSpring();
		/**
		\brief Makes the joint springy. 
		\param[in] spring The new rest length. The spring.targetValue field is not used.
		\sa getSpring()
		*/
		bool setSpring(pSpring spring);

		/**
		\brief Enables collision between the two bodies.
		\param[in] Collide or not. 
		*/
		void enableCollision(int collision);


	protected:
	private:

};
class MODULE_API pJointFixed : public pJoint
{
public:

	pJointFixed(pRigidBody* _a,pRigidBody* _b);
	protected:
	private:
};

/**
\brief A sphere joint constrains two points on two bodies to coincide.

This point, specified in world space (this guarantees that the points coincide 
to start with) is the only parameter that has to be specified.

\image html sphericalJoint.png


*/
class MODULE_API pJointBall : public pJoint
{
	public:

		pJointBall(pRigidBody* _a,pRigidBody* _b);
		pJointBall(pRigidBody* _a,pRigidBody* _b,VxVector anchor);
		
		/**
		\brief Retrieves the global space anchor.
		\return The joints anchor.
		\sa getAnchor() 
		*/
		VxVector  getAnchor();
		/**
		\brief Sets the global space anchor.
		\param[in] worldLimitPoint Used to store the global frame limit point. 
		\sa getAnchor() 
		*/
		void setAnchor(const VxVector& anchor);
		/**
		\brief Sets the limit axis defined in the joint space of body a.
		\param[in] swingLimitAxis The new limit axis.
		\sa getAnchor() 
		*/
		void setSwingLimitAxis(const VxVector& swingLimitAxis);
		/**
		\brief Sets the swing limit of the twist axis.
		\param[in] limit The new swing limit axis.
		\return True if the limit was valid.
		\sa getSwingLimit() 
		*/
		bool setSwingLimit(pJointLimit limit);
		/**
		\brief Sets the high rotation limit around the twist axis.
		\param[in] limit The new twist high limit.
		\return True if the limit was valid.
		\sa getTwistLowLimit() 
		*/
		bool setTwistHighLimit(pJointLimit limit);
		/**
		\brief Sets the high rotation limit around the twist axis.
		\param[in] limit The new twist low limit.
		\return True if the limit was valid.
		\sa getTwistLowLimit() 
		*/
		bool setTwistLowLimit(pJointLimit limit);
		
		/**
		\brief Sets a spring that works against swinging.
		\param[in] spring The new spring.
		\return True if the spring was valid.
		\sa getSwingSpring() 
		*/
		bool setSwingSpring(pSpring spring);
		/**
		\brief Sets a spring that works against twisting.
		\param[in] spring The new spring.
		\return True if the spring was valid.
		\sa getTwistSpring() 
		*/
		bool setTwistSpring(pSpring spring);
		/**
		\brief Sets a spring that lets the joint get pulled apart.
		\param[in] spring The new spring.
		\return True if the spring was valid.
		\sa getJointSpring() 
		*/
		bool setJointSpring(pSpring spring);

		pJointLimit getSwingLimit();
		pJointLimit getTwistHighLimit();
		pJointLimit getTwistLowLimit();

		pSpring getSwingSpring();
		pSpring getTwistSpring();
		pSpring getJointSpring();
		void enableFlag(int flag,bool enable);

		/**
		\brief Enables collision between the two bodies.
		\param[in] collision Collide or not. 
		*/
		void enableCollision(bool collision);

		void setProjectionMode(ProjectionMode mode);
		void setProjectionDistance(float distance);
		void setProjectionAngle(float angle);




	protected:
	private:
};

/** @} */

#endif