#if !defined(EA_C2D3E6DE_B1A5_4f07_B3FA_73F108249451__INCLUDED_)
#define EA_C2D3E6DE_B1A5_4f07_B3FA_73F108249451__INCLUDED_


#include "vtPhysXBase.h"
#include "pReferencedObject.h"
#include "pCallbackObject.h"


/** \addtogroup RigidBody
@{
*/

struct pContactModifyData
{

		float minImpulse;			//!< Minimum impulse value that the solver can apply. Normally this should be 0, negative amount gives sticky contacts.
		float maxImpulse;			//!< Maximum impulse value that the solver can apply. Normally this is FLT_MAX. If you set this to 0 (and the min impulse value is 0) then you will void contact effects of the constraint.
		VxVector error;				//!< Error vector. This is the current error that the solver should try to relax.
		VxVector target;				//!< Target velocity. This is the relative target velocity of the two bodies.

		/**
		\brief Constraint attachment point for shape 0. 

		If the shape belongs to a dynamic actor, then localpos0 is relative to the body frame of the actor. 
		Alternatively it is relative to the world frame for a static actor. 
		*/
		VxVector localpos0;

		/**
		\brief Constraint attachment point for shape 1. 

		If the shape belongs to a dynamic actor, then localpos1 is relative to the body frame of the actor. 
		Alternatively it is relative to the world frame for a static actor. 
		*/
		VxVector localpos1;

		/**
		\brief Constraint orientation quaternion for shape 0 relative to shape 0s body frame for dynamic 
		actors and relative to the world frame for static actors. 

		The constraint axis (normal) is along the x-axis of the quaternion. 
		The Y axis is the primary friction axis and the Z axis the secondary friction axis. 
		*/
		VxQuaternion localorientation0;
		

		/**
		\brief Constraint orientation quaternion for shape 1 relative to shape 1s body frame for dynamic 
		actors and relative to the world frame for static actors. 

		The constraint axis (normal) is along the x-axis of the quaternion. 
		The Y axis is the primary friction axis and the Z axis the secondary friction axis. 
		*/
		VxQuaternion localorientation1;

		/**
		\brief Static friction parameter 0.

		\note 0 does not have anything to do with shape 0/1, but is related to anisotropic friction, 
		0 is the primary friction axis.
		*/
		float staticFriction0;

		/**
		\brief Static friction parameter 1.

		\note 1 does not have anything to do with shape 0/1, but is related to anisotropic friction, 
		0 is the primary friction axis.
		*/
		float staticFriction1;

		/**
		\brief Dynamic friction parameter 0.

		\note 0 does not have anything to do with shape 0/1, but is related to anisotropic friction, 
		0 is the primary friction axis.
		*/		
		float dynamicFriction0;

		/**
		\brief Dynamic friction parameter 1.

		\note 1 does not have anything to do with shape 0/1, but is related to anisotropic friction, 
		0 is the primary friction axis.
		*/
		float dynamicFriction1;
		float restitution;			//!< Restitution value.

};

struct pCollisionsEntry
{
	VxVector sumNormalForce;
	VxVector sumFrictionForce;
	VxVector faceNormal;
	VxVector point;
	xU32 faceIndex;
	float pointNormalForce;
	/*float patchNormalForce;*/
	NxActor *actors[2];
	pRigidBody *bodyA;
	pRigidBody *bodyB;
	int eventType;
	float distance;
	CK_ID shapeEntityA;
	CK_ID shapeEntityB;


	pCollisionsEntry(){

		bodyB = bodyA = NULL;
		actors[0]=NULL;
		actors[1]=NULL;
		eventType;
		distance = pointNormalForce = 0.0f;
		shapeEntityA = shapeEntityB  =  0 ;


	}
};
typedef XArray<pCollisionsEntry>CollisionsArray;




/**
\brief pRigidBody is the main simulation object in the physics SDK.

The body is owned by and contained in a pWorld.


<h3>Creation</h3>
Instances of this class are created by calling #pFbodyy::createBody() and deleted with #NxScene::deleteBody().

See #pObjectDescr for a more detailed description of the parameters which can be set when creating a body.

//class MODULE_API pRigidBody : public xEngineObjectAssociation<CK3dEntity*>

class MODULE_API pRigidBody : public pStoredObjectAssociation<CK3dEntity*,NxActor*>

*/
class MODULE_API pRigidBody : 
	public xEngineObjectAssociation<CK3dEntity*>,
	public pCallbackObject
{

public:
	pRigidBody(CK3dEntity* _e);
	pRigidBody(CK3dEntity* _e,pWorld *world);
	virtual ~pRigidBody(){}

	void test();

	pObjectDescr *mInitialDescription;
	
	pObjectDescr * getInitialDescription() const { return mInitialDescription; }
	void setInitialDescription(pObjectDescr * val) { mInitialDescription = val; }
	void onICRestore(CK3dEntity* parent,pRigidBodyRestoreInfo *restoreInfo);

	bool hasBrokenJoint;


	int onJointBreak(pBrokenJointEntry *entry);

	/************************************************************************************************/
	/** @name Callback handler
	*/
	//@{
    

	bool onSubShapeTransformation(bool fromPhysicToVirtools=true,bool position=true,bool rotation=true,CK3dEntity*parent=NULL,bool children=true);
	bool onMove(bool position,bool rotation,VxVector pos,VxQuaternion quad);

	void processScriptCallbacks();


	//----------------------------------------------------------------
	//
	// collision notification
	//
	void onContactNotify(pCollisionsEntry *collisionData);
	void setContactScript(int behaviorID,int eventMask);

	/**
	\brief Sets the force threshold for contact reports.

	See #getContactReportThreshold().

	The actor must be dynamic.

	\param[in] threshold Force threshold for contact reports. 
		-	<b>Range:</b> (0,inf)

	@see getContactReportThreshold getContactReportFlags pContactPairFlag 
	*/
	void setContactReportThreshold(float threshold);
	/**
	\brief Retrieves the force threshold for contact reports.

	The contact report threshold is a force threshold. If the force between 
	two actors exceeds this threshold for either of the two actors, a contact report 
	will be generated according to the union of both body' contact report threshold flags.
	See #getContactReportFlags().

	The body must be dynamic. The threshold used for a collision between a dynamic actor
	and the static environment is the threshold of the dynamic actor, and all contacts with
	static actors are summed to find the total normal force.

	\return Force threshold for contact reports.

	@see setContactReportThreshold getContactReportFlags pContactPairFlag 
	*/
	float getContactReportThreshold();


	void setContactReportFlags(pContactPairFlags flags);
	int getContactReportFlags();

	//////////////////////////////////////////////////////////////////////////
	//
	//	joint break events
	//	
	void setJointBreakScript(int behaviorID,CK3dEntity *shapeReference = NULL);


	//----------------------------------------------------------------
	//
	// trigger notification
	//
	void setTriggerScript(int behaviorID,int eventMask,CK3dEntity *shapeReference = NULL);

	int onTrigger(pTriggerEntry *report);
	//----------------------------------------------------------------
	//
	// contact modification
	//
	void setContactModificationScript(int behaviorID);

	bool onContactConstraint(int& changeFlags,CK3dEntity *sourceObject,CK3dEntity *otherObject,pContactModifyData *data);
	//----------------------------------------------------------------
	//
	// raycast hit
	//
	virtual void setRayCastScript(int val);
	virtual bool onRayCastHit(NxRaycastHit *report);

	//@}

	/************************************************************************************************/
	/** @name Collision
	*/
	//@{

	void setTriggerFlags(pTriggerFlags flags,CKBeObject *shapeReference=NULL);
	pTriggerFlags getTriggerFlags(CKBeObject *shapeReference=NULL);

	int handleContactPair(NxContactPair* pair,int shapeIndex);
	int handleContactPairWheel(NxContactPair* pair,int shapeIndex);

	/**
	\brief Sets 128-bit mask used for collision filtering. See comments for ::pGroupsMask

	<b>Sleeping:</b> Does <b>NOT</b> wake the associated body up automatically.

	\param[in] shape Reference The sub shape reference object. Leave blank to set the bodies initial shapes groups mask. 
	\param[in] mask The group mask to set for the shape.

	@see getGroupsMask()
	*/
	void					setGroupsMask(CK3dEntity *shapeReference,const pGroupsMask& mask);

	pGroupsMask getGroupsMask(CK3dEntity *shapeReference);



	//@}

	/************************************************************************************************/
	/** @name Velocity
	*/
	//@{

	/**
	\brief Retrieves the angular velocity of a rigid body. 
	
	\return    Vector
	
	@see setAngularVelocity() getLinearVelocity()
	
	\warning The body must be dynamic.
	
	*/
	VxVector getAngularVelocity()const;
	/**
		\brief Retrieves the linear velocity of a rigid body. 
		\return    Vector
		\sa setLinearVelocity() getAngularVelocity()
		\warning The body must be dynamic.
	*/
	VxVector getLinearVelocity()const;
	/**
	\brief Retrieves the maximum angular velocity permitted for this body.
	\return    float
	\sa setMaxAngularVelocity
	\warning The body must be dynamic.
	*/
	float getMaxAngularSpeed()const;
	/**
	\brief Lets you set the maximum angular velocity permitted for this body.
	
	Because for various internal computations, very quickly rotating bodies introduce error 
	into the simulation, which leads to undesired results.

	With PhysicManager::setParameter(EX_MAX_ANGULAR_VELOCITY) you can set the default maximum velocity for bodies created
	after the call. Bodies' high angular velocities are clamped to this value. 

	However, because some bodies, such as car wheels, should be able to rotate quickly, you can override the default setting
	on a per-body basis with the below call. Note that objects such as wheels which are approximated with spherical or 
	other smooth collision primitives can be simulated with stability at a much higher angular velocity than, say, a box that
	has corners.

	Note: The angular velocity is clamped to the set value <i>before</i> the solver, which means that
	the limit may still be momentarily exceeded.
	
	\param[in] val Max allowable angular velocity for body. <b>Range:</b> (0,inf)
	
	\sa  getMaxAngularVelocity()
	
	\warning The body must be dynamic.
	*/
	void setMaxAngularSpeed(float val);
	/**
	\brief Computes the velocity of a point given in body local coordinates as if it were attached to the 
	body and moving with it.
	
	\param[in] point Point we wish to determine the velocity of, defined in the body local frame. <b>Range:</b> position vector
	
	\return The velocity, in the global frame, of the point.

	\sa getLocalPointVelocity()
	
	\warning The body must be dynamic.
	*/
	VxVector getPointVelocity(const VxVector& point)const;
	/**
	\brief Computes the velocity of a point given in body local coordinates as if it were attached to the 
	body and moving with it.

	\param[in] point Point we wish to determine the velocity of, defined in the body local frame. <b>Range:</b> position vector

	\return The velocity, in the global frame, of the point.

	\sa getPointVelocity()

	\warning The body must be dynamic.
	*/
	VxVector getLocalPointVelocity(const VxVector& point)const;
	/**
	\brief Sets the angular velocity of the body.

	\note Note that if you continuously set the angular velocity of an body yourself, 
	forces such as friction will not be able to rotate the body, because forces directly influence only the velocity/momentum.

	\param[in] angVel New angular velocity of body. <b>Range:</b> angular velocity vector

	\sa getAngularVelocity() setLinearVelocity() 

	\warning 
	The body must be dynamic.<br>	
	<b>Sleeping:</b> This call wakes the body if it is sleeping.
	*/
	void setAngularVelocity(const VxVector& angVel);
	/**
	\brief Sets the linear velocity of the body.

	\note Note that if you continuously set the velocity of an body yourself, 
	forces such as gravity or friction will not be able to manifest themselves, because forces directly
	influence only the velocity/momentum of an body.

	\param[in] linVel New linear velocity of body. <b>Range:</b> velocity vector
	\sa getLinearVelocity() setAngularVelocity()

	\warning 
	The body must be dynamic.<br>	
	<b>Sleeping:</b> This call wakes the body if it is sleeping.
	*/
	void setLinearVelocity(const VxVector& linVel);

	//@}


	/************************************************************************************************/

	/** @name Mass Manipulation
	*/
	//@{


	/**
	\brief The setCMassOffsetLocal*() methods set the pose of the center of mass relative to the actor.

	See ::setCMassOffsetLocalPose() for more information.

	\note Setting an unrealistic center of mass which is a long way from the body can make it difficult for
	the SDK to solve constraints. Perhaps leading to instability and jittering bodies.

	The actor must be dynamic.

	<b>Sleeping:</b> This call wakes the actor if it is sleeping.

	\param[in] vec Mass frame offset relative to the actor frame. <b>Range:</b> position vector
	@see setCMassOffsetLocalPose() setCMassOffsetLocalOrientation() setCMassOffsetGlobalPose()
	*/
	//void			setCMassOffsetLocalPosition(VxVector vec);


	/**
	\brief The setCMassOffsetGlobal*() methods set the pose of the center of mass relative to world space.

	See ::setCMassOffsetGlobalPose() for more information.

	\note Setting an unrealistic center of mass which is a long way from the body can make it difficult for
	the SDK to solve constraints. Perhaps leading to instability and jittering bodies.

	The rigid body must be dynamic.

	<b>Sleeping:</b> This call wakes the rigid body if it is sleeping.

	\param[in] vec Mass frame offset relative to the global frame. <b>Range:</b> position vector

	@see setCMassOffsetGlobalPose() setCMassOffsetGlobalOrientation()

	*/
	void			setCMassOffsetGlobalPosition(VxVector vec);

	/**
	\brief The setCMassGlobal*() methods move the rigid body by setting the pose of the center of mass.

	See ::setCMassGlobalPose() for more information.

	The rigid body must be dynamic.

	<b>Sleeping:</b> This call wakes the rigid body if it is sleeping.

	\param[in] vec rigid bodys new position, from the transformation of the mass frame to the global frame. <b>Range:</b> position vector
	@see setCMassGlobalPose() setCMassGlobalOrientation() getCMassLocalPose()
	*/
	void			setCMassGlobalPosition(VxVector vec);

	/**
	\brief The getCMassLocal*() methods retrieve the center of mass pose relative to the rigid body.

	The rigid body must be dynamic.

	\return The center of mass position relative to the rigid body.

	@see getCMassLocalPose() getCMassLocalOrientation() getCMassGlobalPose()
	*/
	VxVector 			getCMassLocalPosition(); 


	/**
	\brief The getCMassGlobal*() methods retrieve the center of mass pose in world space.

	The rigid body must be dynamic.

	\return The position of the center of mass relative to the global frame.
	@see getCMassGlobalPose() getCMassGlobalOrientation() getCMassLocalPose()
	
	*/
	VxVector 			getCMassGlobalPosition();


	/**
	\brief Sets the inertia tensor, using a parameter specified in mass space coordinates.

	Note that such matrices are diagonal -- the passed vector is the diagonal.

	If you have a non diagonal world/rigid body space inertia tensor(3x3 matrix). Then you need to
	diagonalize it and set an appropriate mass space transform. See #setCMassOffsetLocalPose().

	The rigid body must be dynamic.

	<b>Sleeping:</b> Does <b>NOT</b> wake the rigid body up automatically.

	\param[in] m New mass space inertia tensor for the rigid body. <b>Range:</b> inertia vector
	@see NxBodyDesc.massSpaceInertia getMassSpaceInertia() setMass() setCMassOffsetLocalPose()
	*/
	void			setMassSpaceInertiaTensor(VxVector m);

	/**
	\brief  Retrieves the diagonal inertia tensor of the rigid body relative to the mass coordinate frame.

	This method retrieves a mass frame inertia vector. If you want a global frame inertia tensor(3x3 matrix),
	then see #getGlobalInertiaTensor().

	The rigid body must be dynamic.

	\return The mass space inertia tensor of this rigid body.

	@see NxBodyDesc.massSpaceInertia setMassSpaceInertiaTensor() setMass() CMassOffsetLocalPose()
	*/
	VxVector			getMassSpaceInertiaTensor();

	/**
	\brief Retrieves the mass of the body.

	Zero represents no damping. The damping coefficient must be nonnegative.

	\param[in] angDamp Angular damping coefficient. <b>Range:</b> [0,inf)

	\sa setMass()

	\warning Static bodies will always return 0.
	*/
	float getMass();
	/**
	\brief Sets the mass of a dynamic body.

	The mass must be positive and the body must be dynamic.

	setMass() does not update the inertial properties of the body, to change the inertia tensor
	use setMassSpaceInertiaTensor() or updateMassFromShapes().

	<b>Sleeping:</b> Does <b>NOT</b> wake the body up automatically.

	\param[in] mass New mass value for the body. <b>Range:</b> (0,inf)

	\sa setMass()

	\warning The mass must be positive and the body must be dynamic.
	*/
	void setMass(float mass,CKBeObject *shapeReference=NULL);
	/**
	\brief The setCMassOffsetLocal*() methods set the pose of the center of mass relative to the body.

	Methods that automatically compute the center of mass such as updateMassFromShapes() as well as computing
	the mass and inertia using the bodies shapes, will set this pose automatically.

	\note Setting an unrealistic center of mass which is a long way from the body can make it difficult for
	the SDK to solve constraints. Perhaps leading to instability and jittering bodies.

	<b>Sleeping:</b> This call wakes the body if it is sleeping.

	\param[in] vec Mass frame offset relative to the body frame. <b>Range:</b> position vector

	\warning The body must be dynamic.
	*/
	void setCMassOffsetLocalPosition(VxVector offset);
	/**
	\brief Recomputes a dynamic body's mass properties from its shapes

	Given a constant density or total mass, the bodies mass properties can be recomputed
	using the shapes attached to the body. If the body has no shapes, then only the totalMass
	parameter can be used. If all shapes in the body are trigger shapes (non-physical), the call
	will fail.

	The mass of each shape is either the shape's local density (as specified in the #NxShapeDesc; default 1.0) 
	multiplied by the shape's volume or a directly specified shape mass. 

	The inertia tensor, mass frame and center of mass will always be recomputed. If there are no
	shapes in the body, the mass will be totalMass, and the mass frame will be set to the center
	of the body.

	If you supply a non-zero total mass, the body's mass and inertia will first be computed as
	above and then scaled to fit this total mass.

	If you supply a non-zero density, the body's mass and inertia will first be computed as above
	and then scaled by this fbody.

	Either totalMass or density must be non-zero.

	\param[in] density Density scale fbody of the shapes belonging to the body. <b>Range:</b> [0,inf)
	\param[in] totalMass Total mass of the body(or zero). <b>Range:</b> [0,inf)

	\sa setMass()

	\warning 
		The body must be dynamic.<br>
		<b>Sleeping:</b> Does <b>NOT</b> wake the body up automatically.

	*/
	int updateMassFromShapes( float density,  float totalMass );
//@}
	


	/************************************************************************************************/


/** @name Forces
	*/
	//@{

	/**
	\brief Applies a force (or impulse) defined in the global coordinate frame to the body.

	Methods that automatically compute the center of mass such as updateMassFromShapes() as well as computing
	the mass and inertia using the bodies shapes, will set this pose automatically.

	
	\param[in] force Force/Impulse to apply defined in the global frame. <b>Range:</b> force vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode).Default  = #FM_Force.
	\param[in] wakeUp Specify if the call should wake up the body.Default  = true.

	\sa addForceAtPos() addForceAtLocalPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addForce(const VxVector& force,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies a force (or impulse) defined in the global coordinate frame, acting at a particular 
	point in global coordinates, to the body. 


	Note that if the force does not act along the center of mass of the body, this
	will also add the corresponding torque. Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.

	\param[in] force Force/impulse to add, defined in the global frame. <b>Range:</b> force vector
	\param[in] point Position in the global frame to add the force at. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode)
	\param[in] wakeUp Specify if the call should wake up the body.
	
	\sa ForceMode
	\sa addForceAtLocalPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce() addLocalForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addForceAtPos(const VxVector& force,const VxVector& point,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies a force (or impulse) defined in the global coordinate frame, acting at a particular 
	point in local coordinates, to the body. 


	\note Note that if the force does not act along the center of mass of the body, this
	will also add the corresponding torque. Because forces are reset at the end of every timestep, you can maintain a
	total external force on an object by calling this once every frame.
	
	::ForceMode determines if the force is to be conventional or impulsive.
	
	\param[in] force Force/impulse to add, defined in the global frame. <b>Range:</b> force vector
	\param[in] point Position in the local frame to add the force at. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode)
	\param[in] wakeUp Specify if the call should wake up the body.

	\sa ForceMode
	\sa addForceAtPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce() addLocalForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addForceAtLocalPos(const VxVector& force,const VxVector& point,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies a force (or impulse) defined in the body local coordinate frame to the body.
	
	::ForceMode determines if the force is to be conventional or impulsive.

	\param[in] force Force/Impulse to apply defined in the local frame. <b>Range:</b> force vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode)
	\param[in] wakeUp Specify if the call should wake up the body.

	\sa ForceMode
	\sa addForceAtPos() addForceAtLocalPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addLocalForce(const VxVector& force,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies a force (or impulse) defined in the body local coordinate frame, acting at a 
	particular point in global coordinates, to the body. 

	\note Note that if the force does not act along the center of mass of the body, this
	will also add the corresponding torque. Because forces are reset at the end of every timestep, you can maintain a
	total external force on an object by calling this once every frame.

	::ForceMode determines if the force is to be conventional or impulsive.

	\param[in] force Force/impulse to add, defined in the local frame. <b>Range:</b> force vector
	\param[in] point Position in the global frame to add the force at. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode)
	\param[in] wakeUp Specify if the call should wake up the body.

	\sa ForceMode
	\sa addForceAtPos() addForceAtLocalPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addLocalForceAtPos(const VxVector& force,const VxVector& point,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies a force (or impulse) defined in the body local coordinate frame, acting at a 
	particular point in local coordinates, to the body. 

	\note Note that if the force does not act along the center of mass of the body, this
	will also add the corresponding torque. Because forces are reset at the end of every timestep, you can maintain a
	total external force on an object by calling this once every frame.

	::ForceMode determines if the force is to be conventional or impulsive.

	\param[in] force Force/impulse to add, defined in the local frame. <b>Range:</b> force vector
	\param[in] point Position in the local frame to add the force at. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode)
	\param[in] wakeUp Specify if the call should wake up the body.

	\sa ForceMode
	\sa addForceAtPos() addForceAtLocalPos() addLocalForceAtPos() addLocalForceAtLocalPos() addForce()


	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addLocalForceAtLocalPos(const VxVector& force,const VxVector& point,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies an impulsive torque defined in the global coordinate frame to the body.

	::ForceMode determines if the force is to be conventional or impulsive.


	\param[in] torque Torque to apply defined in the global frame.
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode).
	\param[in] wakeUp Specify if the call should wake up the body.

	\sa addLocalTorque() addForce()

	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addTorque(const VxVector& torque,ForceMode mode=FM_Force,bool wakeUp=true);
	/**
	\brief Applies an impulsive torque defined in the body local coordinate frame to the body.

	::ForceMode determines if the force is to be conventional or impulsive.

	\param[in] torque Torque to apply defined in the local frame.
	\param[in] mode The mode to use when applying the force/impulse(see #ForceMode).
	\param[in] wakeUp Specify if the call should wake up the body.
	\sa addLocalTorque() addForce()

	\warning 
	The body must be dynamic.<br>	
	<b>This will not induce a torque</b>.<br>
	<b>Sleeping:</b> This call wakes the body if it is sleeping and the wakeup parameter is true (default).
	*/
	void addLocalTorque(const VxVector& torque,ForceMode mode=FM_Force,bool wakeUp=true);

	//@}


	/************************************************************************************************/

	/** @name Momentum
	*/
	//@{
	
	/**
	\brief Sets the angular momentum of the body. 

	\note Note that if you continuously set the linear momentum of an body yourself, 
	forces such as gravity or friction will not be able to manifest themselves, because forces directly
	influence only the velocity/momentum of a body.

	\param[in] angMoment New angular momentum. <b>Range:</b> angular momentum vector
		
	\sa getAngularMomentum()

	\warning 
	The body must be dynamic.<br>	
	*/
	void setAngularMomentum(const VxVector& angMoment);
	/**
	\brief Sets the linear momentum of the body. 

	\note Note that if you continuously set the linear momentum of an body yourself, 
	forces such as gravity or friction will not be able to manifest themselves, because forces directly
	influence only the velocity/momentum of a body.
	
	\param[in] linMoment New linear momentum. <b>Range:</b> momentum vector

	\sa getLinearMomentum() 

	\warning 
	The body must be dynamic.<br>	
	*/
	void setLinearMomentum(const VxVector& linMoment);
	/**
	\brief Retrieves the angular momentum of an body.

	The angular momentum is equal to the angular velocity times the global space inertia tensor.

	\return The angular momentum for the body.

	\sa setLinearMomentum() getAngularMomentum()
	

	\warning 
	The body must be dynamic.<br>	
	*/
	VxVector getAngularMomentum()const;
	/**
	\brief Retrieves the linear momentum of an body.

	The momentum is equal to the velocity times the mass.

	\return The linear momentum for the body.

	\sa setLinearMomentum() getAngularMomentum()

	\warning 
	The body must be dynamic.<br>	
	*/
	VxVector getLinearMomentum()const;

	//@}

	/************************************************************************************************/

	/** @name Pose
	*/
	//@{

	/**
	\brief Sets a dynamic body's position in the world.

	\param[in] pos New position for the bodies frame relative to the global frame. <b>Range:</b> position vector
	\param[in] subShapeReference Reference object specifing a subshape for the case the body is a compound object. Must be a mesh or  an 3D-entity. Default = Null 

	\sa getPosition()

	*/
	void setPosition(const VxVector& pos,CK3dEntity *subShapeReference=NULL);
	/**
	\brief Sets a dynamic body's orientation in the world.
	
	\param[in] rot New orientation for the bodies frame.
	\param[in] subShapeReference Reference object specifying a sub shape for the case the body is a compound object. Must be a mesh or  an 3D-entity. Default = Null 

	\sa getLinearMomentum() setAngularMomentum()
	*/
	void setRotation(const VxQuaternion& rot,CK3dEntity *subShapeReference=NULL);
	//@}


	/************************************************************************************************/
	/** @name Collision
	*/
	//@{

	
	/**
	\brief Enables/disable collision detection. I.e. the body will not collide with other objects. Please note that you might need to wake 
	the body up if it is sleeping, this depends on the result you wish to get when using this flag. (If a body is asleep it 
	will not start to fall through objects unless you activate it).

	\param[in] enable. Flag to determining collisions response for the body.
	to collision detect with each other.
	\param[in] subShapeReference Reference object specifing a subshape for the case the body is a compound object. Must be a mesh or  an 3D-entity. Default = Null 

	\Note	: Also excludes the body from overlap tests!

	\sa isCollisionEnabled()
	
	<b>Sleeping:</b> Does <b>NOT</b> wake the associated body up automatically.
	*/
	void enableCollision(bool enable,CK3dEntity* subShapeReference=NULL);

	bool isCollisionEnabled(CK3dEntity* subShapeReference=NULL) ;


	void enableCollisionsNotify(bool enable);
	bool isCollisionsNotifyEnabled();

	void enableContactModification(bool enable);
	void enableCollisionForceCalculation(bool enable,CK3dEntity* subShapeReference=NULL);
	void enableTriggerShape(bool enable,CK3dEntity* subShapeReference=NULL);
	bool isTriggerShape(CK3dEntity* subShapeReference=NULL);
	/**
	\brief Sets the collisions group the body belongs too.

	\param[in] index The new group index. Default group is 0. Maximum possible group is 31.Collision groups are sets of shapes which may or may not be set
	to collision detect with each other.
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\note If you pass a sub shape reference then its only setting the group on the sub shape and not for all sub shapes.

	\sa getCollisionsGroup()
	\warning 
	The body must be dynamic.<br>	
	<b>Sleeping:</b> Does <b>NOT</b> wake the associated body up automatically.
	*/
	void setCollisionsGroup(int index,CK3dEntity* subShapeReference=NULL);

	/**
	\brief Retrieves the collisions group which this body or a sub shape of it is part of.

	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\return The collision group this body or sub shape belongs to.

	\sa setCollisionsGroup()

	\warning 
	The body must be dynamic.<br>	
	*/
	int getCollisionsGroup(CK3dEntity* subShapeReference=NULL);

	//@}



	/************************************************************************************************/
	/** @name Shape
	*/
	//@{

	/**
	\brief Updates the box dimension of the initial shape or a sub shape. 

	\param[in] dimension New dimension. <b>Range:</b> dimension vector
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa getBoxDimensions()

	\warning 
		The call doesnt updates the bodies mass.Use updateMassFromShapes()<br>	

	*/
	void setBoxDimensions(const VxVector&dimension,CKBeObject* subShapeReference=NULL);
	/**
	\brief Retrieves the box dimension of the initial shape or sub shape. 

	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa setBoxDimensions()
	*/
	VxVector getBoxDimensions(CKBeObject* subShapeReference=NULL);
	/**
	\brief Updates the radius of the initial shape or a sub shape. 

	\param[in] radius New radius.
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa getSphereRadius()

	\warning 
	The call doest updates the bodies mass.Use updateMassFromShapes()<br>	

	*/
	void setSphereRadius(float radius,CKBeObject* subShapeReference=NULL);
	/**
	\brief Retrieves the radius of the initial shape or sub shape. 

	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa setSphereRadius() .  
	*/
	float getSphereRadius(CKBeObject* subShapeReference=NULL);
	/**
	\brief Updates the capsule parameter of the initial shape or a sub shape. 

	\param[in] radius New radius.
	\param[in] length New length.
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa getCapsuleDimensions()

	\warning 
	The call doesnt updates the bodies mass.Use updateMassFromShapes()<br>	

	*/
	void setCapsuleDimensions(float radius,float length,CKBeObject* subShapeReference=NULL);
	/**
	\brief Retrieves the capsule parameters of the initial shape or sub shape. 

	\param[out] radius The radius of the capsule.
	\param[out] length The length of the capsule.
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.
	\sa setCapsuleDimensions() .  
	*/
	void getCapsuleDimensions(float& radius,float& length,CKBeObject* subShapeReference=NULL);
	/**
	\brief Retrieves the hull type of the initial shape or sub shape. 
	\return The hull type.
	\param[in] subShapeReference Sub shape reference. Default = Null.
	*/
	HullType getShapeType(CKBeObject* subShapeReference=NULL);

	/**
	\brief Retrieves the skin width of the initial shape or a sub shape. 
	\return The skin with.
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.
	\sa setSkinWidth()
	*/
	float getSkinWidth(CKBeObject* subShapeReference=NULL);

	/**
	\brief Updates the skin width of the initial shape or a sub shape. 
	
	\param[in] skinWidth The new skin width. <b>Range:</b> (0,inf)
	\param[in] subShapeReference Sub shape reference. Must be a mesh or  an 3D-entity. Default = Null.

	\sa setSkinWidth()
	\warning
	<b>Sleeping:</b> Does <b>NOT</b> wake the associated body up automatically.
	*/
	void setSkinWidth(const float skinWidth,CKBeObject* subShapeReference=NULL);

	//@}

	

	/************************************************************************************************/
	/** @name Optimization
	*/
	//@{


		/**
		\brief The solver iteration count determines how accurately joints and contacts are resolved. <br>
		If you are having trouble with jointed bodies oscillating and behaving erratically, then
		setting a higher solver iteration count may improve their stability.

		\param[in] count Number of iterations the solver should perform for this body. <br>
			-	<b>Range:</b> [1,255]

		*/
		void setSolverIterationCount(int count);

		/**
		
		\brief Assigns dynamic bodies a dominance group identifier.<br>
		
		The dominance group is a 5 bit group identifier (legal range from 0 to 31).

		The #pWorld::setDominanceGroupPair() lets you set certain behaviors for pairs of dominance groups.
		By default every body is created in group 0.  Static bodies must stay in group 0; thus you can only 
		call this on dynamic bodys.

		<b>Sleeping:</b> Changing the dominance group does <b>NOT</b> wake the body up automatically.

		@see getDominanceGroup() NxScene::setDominanceGroupPair()
		*/
		void setDominanceGroup(int dominanceGroup);

		/**
		\brief Retrieves the value set with setDominanceGroup().<br>

		\return The dominance group of this body.

		@see setDominanceGroup() pWorld::setDominanceGroupPair()
		*/
		int	getDominanceGroup() const;


		/**
		\brief Returns the linear velocity below which an body may go to sleep.<br>

		Bodies whose linear velocity is above this threshold will not be put to sleep.

		The body must be dynamic.

		@see isSleeping

		\return The threshold linear velocity for sleeping.


		@see isGroupSleeping() isSleeping() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepLinearVelocity() setSleepEnergyThreshold() getSleepEnergyThreshold()
		*/
		float			getSleepLinearVelocity() const ;

		/**
		\brief Sets the linear velocity below which an body may go to sleep.<br>

		Bodies whose linear velocity is above this threshold will not be put to sleep.

		If the threshold value is negative,	the velocity threshold is set using the NxPhysicsSDK's 
		NX_DEFAULT_SLEEP_LIN_VEL_SQUARED parameter.

		Setting the sleep angular/linear velocity only makes sense when the NX_BF_ENERGY_SLEEP_TEST is not set. In
		version 2.5 and later a new method is used by default which uses the kinetic energy of the body to control
		sleeping.


		The body must be dynamic.

		\param[in] threshold Linear velocity below which an body may sleep. <b>Range:</b> (0,inf]

		@see isGroupSleeping() isSleeping() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepEnergyThreshold() getSleepEnergyThreshold()
		*/
		void			setSleepLinearVelocity(float threshold);


		/**
		\brief Returns the angular velocity below which an body may go to sleep.<br>

		Bodies whose angular velocity is above this threshold will not be put to sleep. 

		The body must be dynamic.

		\return The threshold angular velocity for sleeping.

		@see isGroupSleeping() isSleeping() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepAngularVelocity() setSleepEnergyThreshold() getSleepEnergyThreshold()
		*/
		float			getSleepAngularVelocity() const;


		/**
		\brief Sets the angular velocity below which an body may go to sleep.<br>

		Bodies whose angular velocity is above this threshold will not be put to sleep.

		If the threshold value is negative,	the velocity threshold is set using the NxPhysicsSDK's 
		NX_DEFAULT_SLEEP_LIN_VEL_SQUARED parameter.

		Setting the sleep angular/linear velocity only makes sense when the NX_BF_ENERGY_SLEEP_TEST is not set. In
		version 2.5 and later a new method is used by default which uses the kinetic energy of the body to control 	sleeping.

		The body must be dynamic.

		\param[in] threshold Angular velocity below which an body may go to sleep. 
			-	<b>Range:</b> (0,inf]

		
		@see isGroupSleeping() isSleeping() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepLinearVelocity() setSleepEnergyThreshold() getSleepEnergyThreshold()
		*/
		void			setSleepAngularVelocity(float threshold);
		


		/**
		\brief Sets the energy threshold below which an body may go to sleep.<br>

		Bodies whose kinematic energy is above this threshold will not be put to sleep.

		If the threshold value is negative,	the velocity threshold is set using the NxPhysicsSDK's 
		NX_DEFAULT_SLEEP_ENERGY parameter.

		Setting the sleep energy threshold only makes sense when the NX_BF_ENERGY_SLEEP_TEST is set. There
		are also other types of sleeping that uses the linear and angular velocities directly instead of the 
		energy.

		The body must be dynamic.

		\param[in] threshold Energy below which an actor may go to sleep. <br>
		-	<b>Range:</b> (0,inf]

		@see isGroupSleeping() isSleeping() getSleepEnergyThreshold() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepLinearVelocity() setSleepAngularVelocity() 
		*/
		void setSleepEnergyThreshold(float threshold);

		/**
		\brief Returns the energy below which an body may go to sleep.<br>

		Bodies whose energy is above this threshold will not be put to sleep. 

		The body must be dynamic.

		\return The energy threshold for sleeping.

		@see isGroupSleeping() isSleeping() getSleepLinearVelocity() getSleepAngularVelocity() wakeUp() putToSleep() setSleepAngularVelocity()
		*/
		float				getSleepEnergyThreshold() const;


		/**
		\brief Retrieves the linear damping coefficient.<br>

		\return The linear damping coefficient associated with this body.

		\sa  getAngularDamping()

		\warning The body must be dynamic.
		*/
		float getLinearDamping()const;
		/**
		\brief Retrieves the angular damping coefficient.<br>


		\return The angular damping coefficient associated with this body.

		\sa setAngularDamping() 

		\warning The body must be dynamic.
		*/
		float getAngularDamping()const;
		/**
		\brief Sets the linear damping coefficient.<br>

		Zero represents no damping. The damping coefficient must be nonnegative.

		\param[in] linDamp Linear damping coefficient. <b>Range:</b> [0,inf)

		\sa getLinearDamping()

		\warning The body must be dynamic.
		*/
		void setLinearDamping(float linDamp);
		/**
		\brief Sets the angular damping coefficient.<br>

		Zero represents no damping. The damping coefficient must be nonnegative.

		\param[in] angDamp Angular damping coefficient. <b>Range:</b> [0,inf)

		\sa getLinearDamping()

		\warning The body must be dynamic.
		*/
		void setAngularDamping(float angDamp);


		





	//@}


	/************************************************************************************************/
	/** @name Conditions
	*/
	//@{


	
		/**
		\brief Sets the body to kinematic.
		\param[in] enabled Enable kinematic mode. <b>Range:</b> (0,inf)
		\sa isKinematic()
		*/
		void setKinematic(bool enabled);
		/**
		\brief Retrieves the bodies kinematic state.
		\return the kinematic state.
		*/
		bool isKinematic()const;

		/**
		\brief Checks whether the body is affected by the worlds gravity.
		\return Is affected by gravity.
		\sa enableGravity()
		*/
		bool isAffectedByGravity()const;
		/**
		\brief Enables gravity on the body.
		\param[in] enable The gravity state.  
		\sa isAffectedByGravity()
		*/
		void enableGravity(bool enable);

	//@}

	/************************************************************************************************/
	/** @name Sleeping
	*/
	//@{

		/**
		\brief Checks whether the body is in a sleeping state.
		\return True if sleeping.
		\sa setSleeping()
		*/
		bool isSleeping()const;
		/**
		\brief  Forces the body to sleep. 

		The body will stay asleep until the next call to simulate, and will not wake up until then even when otherwise 
		it would (for example a force is applied to it). It can however wake up during
		the next simulate call.

		\param[in] sleeping The sleeping state.  
		\sa setSleeping()
		\warning 
			The body must be dynamic.<br>
		*/
		void setSleeping(bool sleeping);

		/**
		\brief  Wakes up the body if it is sleeping.  

		The wakeCounterValue determines how long until the body is put to sleep, a value of zero means 
		that the body is sleeping. wakeUp(0) is equivalent to NxActor::putToSleep().

		\param[in] wakeCounterValue New sleep counter value. Default = (20.0f*0.02f) . <b>Range:</b> [0,inf]
		\sa setSleeping() isSleeping() 
		\warning 
		The body must be dynamic.<br>
		*/
		void wakeUp(float wakeCounterValue=pSLEEP_INTERVAL);

	//@}


	/************************************************************************************************/
	/** @name Sub shapes
	*/
	//@{

		/*
		 * \brief Adds an additional shape to the body.  
		 *
		 *  
		 *
		 * \return    int
		 * \param		CKMesh * mesh
		 * \param		pObjectDescr objectDescr
		 * \param		CK3dEntity * srcRefEntity
		 * \param		VxVector localPosition
		 * \param		VxQuaternion localRotation
		 * \note
		 * \sa
		 * \warning
		 */
		int addSubShape(CKMesh *mesh,pObjectDescr& objectDescr,CK3dEntity*srcRefEntity=NULL,VxVector localPosition=VxVector(),VxQuaternion localRotation=VxQuaternion());
		
		int addCollider(pObjectDescr objectDescr,CK3dEntity*srcRefEntity);

		int removeSubShape(CKBeObject *reference,float newensity=0.0f,float totalMass=0.0f);
		NxShape *_getSubShape(CK_ID meshID);
		NxShape *_getSubShapeByEntityID(CK_ID id);
		bool isSubShape(CKBeObject *object);
		int updateSubShapes(bool fromPhysicToVirtools= true,bool position=true,bool rotation=true,CK3dEntity *childObject=NULL);
		int updateSubShape(bool fromPhysicToVirtools=true,bool position=true,bool rotation=true,CK3dEntity *childObject=NULL,bool hierarchy=true);


		NxShape * getMainShape() const { return mMainShape; }
		void setMainShape(NxShape * val) { mMainShape = val; }
		
		NxShape *getShapeByIndex(int index=0);
		NxShape* getSubShape(CK3dEntity*shapeReference=NULL);




	//@}
	
	
	int _initMainShape(const pObjectDescr oDescr,NxActorDesc&bodyDesc);
	int _initMainShape(const pObjectDescr oDescr,NxActorDesc* bodyDesc);



	
	bool isWheel(CKBeObject *object);
	bool isVehicle(CKBeObject *object);
	bool isCharacter(CKBeObject *object);
	

	int getNbJoints();
	pJoint* getJointAtIndex(int index);
	
	
	pJoint* getJoint(CK3dEntity*_b,JType type);
	void deleteJoint(pJoint*joint);
	void deleteJoint(CK3dEntity*_b,JType type);


	/************************************************************************************************/
	/** @name Serialization
	*/
	//@{

	void readFrom(NXU::NxActorDesc *desc,int flags);
	void writeTo(const char *filename,int flags);

	//@}

	int getFlags();
	void recalculateFlags(int _flags);
	void updateFlags(int _flags,CK3dEntity*shapeReference=NULL);
	void setFlags(int _flags);
	
	
	int getHullType();
	void setHullType(int _type);

	//////////////////////////////////////////////////////////////////////////
	//geometry related :
	
	

	bool isValid()const;
	
	//////////////////////////////////////////////////////////////////////////
	//maintainence :
	
	CK3dEntity* GetVT3DObject();
	void SetVT3DObject(CK3dEntity* _obj);
	
	CK_ID getEntID() const { return mEntID; }
	void setEntID(CK_ID val) { mEntID = val; }
	void destroy();

	//////////////////////////////////////////////////////////////////////////
	//optimization settings :
	
	pSleepingSettings* getSleepingSettings() { return m_SleepingSettings; }
	void setSleepingSettings(pSleepingSettings* val) { m_SleepingSettings = val; }
	

	void Init();
	void retrieveSettingsFromAttribute();
	
	void UpdateGeometry(int flags=0);
	
	
	//////////////////////////////////////////////////////////////////////////
	//pivot manipulation,no affects to mass !
	
	void setLocalShapePosition(VxVector relative,CK3dEntity*shapeReference=NULL);
	//void SetPivotOrientation(VxQuaternion relative);
	
	
	pWorld * getWorld() const { return m_pWorld; }
	void setWorld(pWorld * val) { m_pWorld = val; }

	void clean(int flags=0);

	//////////////////////////////////////////////////////////////////////////
	//joint related 
	pJoint* isConnected(CK3dEntity*);
	pJoint* isConnected(CK3dEntity*,int type);
	int JGetNumJoints();
	
	
	JointListType& GetJoints(){ return m_Joints; }


	//////////////////////////////////////////////////////////////////////////
	//composite geometries 
	
	VxVector getMassOffset() const { return massOffset; }
	void setMassOffset(VxVector val) { massOffset = val; }
	void setPivotOffset(VxVector val) { pivotOffset = val; }
	VxVector getPivotOffset(CK3dEntity*shapeReference);

	void translateLocalShapePosition(VxVector vec);
	float getDensity() const { return mDensity; }
	void setDensity(float val) { mDensity = val; }

	
	//////////////////////////////////////////////////////////////////////////
	//prototype : 

	xBitSet& getDataFlags()  { return mDataFlags; }
	void setDataFlags(xBitSet val) { mDataFlags = val; }

	void checkDataFlags();
	void checkForOptimization();
	
	void updateOptimizationSettings(pOptimization optimization);

	
	void checkForCCDSettings(CK3dEntity *shapeReference=NULL);

	void updateCCDSettings(pCCDSettings ccd,CK3dEntity*shapeReference=NULL);
	void updateCollisionSettings(const pObjectDescr oDescr,CK3dEntity*shapeReference=NULL);
	void updateCollisionSettings(pCollisionSettings collision,CK3dEntity*shapeReference=NULL);
	void updatePivotSettings(pPivotSettings pivot,CK3dEntity*shapeReference=NULL);
	void updateMaterialSettings(pMaterial& material,CK3dEntity*shapeReference=NULL);
	void updateMassSettings(pMassSettings massSettings);
	void saveToAttributes(pObjectDescr* oDescr);

	//////////////////////////////////////////////////////////////////////////
	//Surface : 
	

	void InitSurfaceMaterials();
	//////////////////////////////////////////////////////////////////////////
	
	/************************************************************************/
	/* Material                                                                     */
	/*********************************	***************************************/

	pMaterial& getShapeMaterial(CK3dEntity *shapeReference=NULL);
	void setShapeMaterial(pMaterial&material,CK3dEntity*shapeReference=NULL);
	void setShapeMaterialFrom(CKBeObject*src,CK3dEntity*shapeReference=NULL);

	NxMaterial* getMaterial() const { return mMaterial; }
	void setMaterial(NxMaterial* val) { mMaterial = val; }


	NxActor* getActor() const { return mActor; }
	void setActor(NxActor* val) { mActor = val; }
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	

	pCloth *mCloth;
	pCloth * getCloth() const { return mCloth; }
	void setCloth(pCloth * val) { mCloth = val; }
	bool isDeformable();

	//////////////////////////////////////////////////////////////////////////
	void lockTransformation(int flags);
	int getTransformationsLockFlags();
	int isBodyFlagOn(int flags);

	/************************************************************************/
	/* Sub shapes :                                                                      */
	/************************************************************************/

	CollisionsArray mCollisions;
	CollisionsArray&getCollisions(){return mCollisions;}
	
	pTriggerArray mTriggers;
	pTriggerArray& getTriggers() { return mTriggers; }



	xBitSet mCollisionFlags;
	xBitSet&getCollisionFlags();
	

	
	pVehicle * getVehicle() const { return mVehicle; }
	void setVehicle(pVehicle * val) { mVehicle = val; }
	pWheel* getWheel(CK3dEntity* subShapeReference);
	pWheel2* getWheel2(CK3dEntity* subShapeReference);



	int getTriggerState() const { return mTriggerState; }
	void setTriggerState(int val) { mTriggerState = val; }

	int getNbWheels();
	void updateWheels(float step);
	void _transformWheels();
	bool hasWheels();


	int getNbSubShapes();
	int getNbSubBodies();


	protected :
	
		DWORD m_DataFlags;//deprecated !
		xBitSet mDataFlags;
		


		CK_ID mEntID;
		CKContext* context;
		int m_sFlags;
		int m_HullType;
		float m_friction;
		float m_restitution;
		float mDensity;
		
		VxVector massOffset;
		VxVector pivotOffset;
		
		CK3dEntity* mVTObject;
		pWorld * m_pWorld;
		pSleepingSettings* m_SleepingSettings;
		JointListType m_Joints;

		NxActor* mActor;
		NxMaterial*mMaterial;
		float mSkinWidth;
		NxShape *mMainShape;

		

		pVehicle *mVehicle;

		int mTriggerState;

		public:
			void _checkForNewSubShapes();
			void _checkForRemovedSubShapes();


		
};

/** @} */


#endif // !defined(EA_C2D3E6DE_B1A5_4f07_B3FA_73F108249451__INCLUDED_)
