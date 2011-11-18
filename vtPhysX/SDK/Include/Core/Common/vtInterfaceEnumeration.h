/********************************************************************
	created:	2009/02/16
	created:	16:2:2009   20:25
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common\vtInterfaceEnumeration.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common 
	file base:	vtInterfaceEnumeration
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Declaration of enumerations with bindings in the interface :
				+ VSL 
				+ Custom Enumeration

	Remarks		The identifiers described here need to be consistent with types
				registered for the interface. All types are used by this SDK

*********************************************************************/
#ifndef __VT_INTERFACE_ENUMERATION_H__
#define __VT_INTERFACE_ENUMERATION_H__




	/** \addtogroup Callbacks
	@{
	*/
	typedef enum pCallback
	{
		CB_OnDelete				=	(1<<0),
		CB_OnCopy				=	(1<<1),
		CB_OnPreProcess			=	(1<<2),
		CB_OnPostProcess		=	(1<<3),
		CB_OnContactNotify		=	(1<<4),
		CB_OnContactModify		=	(1<<5),
		CB_OnRayCastHit			=	(1<<6),
		CB_OnWheelContactModify	=	(1<<7),
		CB_OnTrigger			=	(1<<8),
		CB_OnJointBreak			=	(1<<9),
		CB_Max,
	};
	typedef enum pWheelContactModifyFlags
	{
		CWCM_ContactPoint		=	(1<<0),
		CWCM_ContactNormal		=	(1<<1),
		CWCM_ContactPosition	=	(1<<2),
		CWCM_NormalForce		=	(1<<3),
		CWCM_OtherMaterialIndex	=	(1<<4),
	};
	/** @} */
	

	/** \addtogroup Collision
	@{
	*/

	/**
	\brief Contact pair flags.

	@see NxUserContactReport.onContactNotify() NxActor::setContactReportThreshold
	*/
	typedef enum pContactPairFlags
	{
		CPF_IgnorePair								= (1<<0),	//!< disable contact generation for this pair

		CPF_OnStartTouch					= (1<<1),	//!< pair callback will be called when the pair starts to be in contact
		CPF_OnEndTouch						= (1<<2),	//!< pair callback will be called when the pair stops to be in contact
		CPF_OnTouch							= (1<<3),	//!< pair callback will keep getting called while the pair is in contact
		CPF_OnImpact							= (1<<4),	//!< [not yet implemented] pair callback will be called when it may be appropriate for the pair to play an impact sound
		CPF_OnRoll							= (1<<5),	//!< [not yet implemented] pair callback will be called when the pair is in contact and rolling.
		CPF_OnSlide							= (1<<6),	//!< [not yet implemented] pair callback will be called when the pair is in contact and sliding (and not rolling).
		CPF_Forces							= (1<<7),	//!< the (summed total) friction force and normal force will be given in the nxcontactpair variable in the contact report.
		CPF_OnStartTouchForceThreshold	= (1<<8),	//!< pair callback will be called when the contact force between two actors exceeds one of the actor-defined force thresholds
		CPF_OnEndTouchForceThreshold		= (1<<9),	//!< pair callback will be called when the contact force between two actors falls below the actor-defined force thresholds
		CPF_OnTouchForceThreshold			= (1<<10),	//!< pair callback will keep getting called while the contact force between two actors exceeds one of the actor-defined force thresholds

		CPF_ContactModification				= (1<<16),	//!< generate a callback for all associated contact constraints, making it possible to edit the constraint. this flag is not included in CPFall for performance reasons. \see nxusercontactmodify
	};

	typedef enum pContactModifyMask
	{

		CMM_None					= 0,		//!< No changes made

		CMM_MinImpulse			= (1<<0),	//!< Min impulse value changed
		CMM_MaxImpulse			= (1<<1),	//!< Max impulse value changed
		CMM_Error				= (1<<2),	//!< Error vector changed
		CMM_Target				= (1<<3),	//!< Target vector changed

		CMM_LocalPosition0		= (1<<4),	//!< Local attachment position in shape 0 changed
		CMM_LocalPosition1		= (1<<5),	//!< Local attachment position in shape 1 changed
		CMM_LocalOrientation0	= (1<<6),	//!< Local orientation (normal, friction direction) in shape 0 changed
		CMM_LocalOrientation1	= (1<<7),	//!< Local orientation (normal, friction direction) in shape 1 changed

		CMM_StaticFriction0		= (1<<8),	//!< Static friction parameter 0 changed. (Note: 0 does not have anything to do with shape 0/1)
		CMM_StaticFriction1		= (1<<9),	//!< Static friction parameter 1 changed. (Note: 1 does not have anything to do with shape 0/1)
		CMM_DynamicFriction0		= (1<<10),	//!< Dynamic friction parameter 0 changed. (Note: 0 does not have anything to do with shape 0/1)
		CMM_DynamicFriction1		= (1<<11),	//!< Dynamic friction parameter 1 changed. (Note: 1 does not have anything to do with shape 0/1)
		CMM_Restitution			= (1<<12),	//!< Restitution value changed.

		CMM_Force32				= (1<<31)	//!< Not a valid flag value, used by the enum to force the size to 32 bits.

	};
	
	/**
	Specifies which informations should be generated(when used as hint flags for ray casting methods).
	*/
	enum pRaycastBit
	{
		RCH_Shape		= (1<<0),								//!< "shape" member of #NxRaycastHit is valid
		RCH_Impact		= (1<<1),								//!< "worldImpact" member of #NxRaycastHit is valid
		RCH_Normal		= (1<<2),								//!< "worldNormal" member of #NxRaycastHit is valid
		RCH_FaceIndex	= (1<<3),								//!< "faceID" member of #NxRaycastHit is valid
		RCH_Distance		= (1<<4),								//!< "distance" member of #NxRaycastHit is valid
		RCH_UV	= (1<<5),								//!< "u" and "v" members of #NxRaycastHit are valid
		RCH_FaceNormal	= (1<<6),								//!< Same as RCH_NORMAL but computes a non-smoothed normal
		RCH_Material= (1<<7),								//!< "material" member of #NxRaycastHit is valid
	};

	/**
	\brief Collision filtering operations.

	@see pGroupsMask
	*/
	enum pFilterOp
	{
		FO_And,
		FO_Or,
		FO_Xor,
		FO_Nand,
		FO_Nor,
		FO_NXor,
		FO_SwapAnd
	};
	/**
	\brief Used to specify which types(static or dynamic) of shape to test against when used with raycasting and overlap test methods in pWorld.

	*/
	typedef enum pShapesType
	{
		/**
		\brief Hits static shapes. 
		*/
		ST_Static= 1,

		/**
		\brief Hits dynamic shapes. 
		*/
		ST_Dynamic= 2, 
		
		/**
		\brief Hits dynamic and static shapes. 
		*/
		ST_All					= ST_Dynamic|ST_Static, 
	};


	/**
	\brief Flags which affect the behavior of NxShapes.

	@see pRigidBody.setTriggerFlags()
	*/
	typedef enum pTriggerFlags
	{
		/**
		\brief Disables trigger callback.
		*/
		TF_Disable				= (1<<3),
		/**
		\brief Trigger callback will be called when a shape enters the trigger volume.
		*/
		TF_OnEnter				= (1<<0),

		/**
		\brief Trigger callback will be called after a shape leaves the trigger volume.
		*/
		TF_OnLeave				= (1<<1),

		/**
		\brief Trigger callback will be called while a shape is intersecting the trigger volume.
		*/
		TF_OnStay				= (1<<2)

	};
	/** @} */
	
	typedef enum E_ENTITY_DATA_FLAGS
	{
		EDF_MATERIAL_PARAMETER,
		EDF_SLEEPING_PARAMETER,
		EDF_DAMPING_PARAMETER,
		EDF_DEFORMABLE_PARAMETER,
		EDF_OPTIMIZATION_PARAMETER,
	};

	typedef enum WORLD_DATA_FLAGS
	{
		WDF_HAS_SURFACE_PARAMETER = 0x0001,
		WDF_HAS_SLEEPING_PARAMETER = 0x0002,
		WDF_HAS_DAMPING_PARAMETER = 0x0004
	};

	typedef enum WORLD_UPDATE_MODE
	{
		WUM_UPDATE_FROM_ATTRIBUTE = 0x0001
	};

	typedef enum WORLD_UPDATE_FLAGS
	{
		WUF_WORLD_SETTINGS = 0x0001,
		WUF_DAMPING_PARAMETER = 0x0002,
		WUF_SLEEPING_PARAMETER = 0x0004,
		WUF_SURFACE_SETTINGS = 0x0008,
		WUF_ALL_PARAMETERS = 0x0010,
	};

	typedef enum BODY_UPDATE_FLAGS
	{
		BUF_PHY_PARAMETER = 0x0001,
		BUF_DAMPING_PARAMETER = 0x0002,
		BUF_SLEEPING_PARAMETER = 0x0004,
		BUF_JOINT_PARAMETERS = 0x0008,
		BUF_SURFACE_PARAMETERS = 0x0010,
		BUF_ALL_PARAMETERS = 0x0020,
		BUF_GEOMETRY = 0x0040,
		BUF_PIVOT = 0x0080,
		BUF_MASS = 0x0100,
		BUF_ALL = 0x0200
	};

	/** \addtogroup Joints
	@{
	*/
	//! Identifies each type of joint. 
	//! This enum is registered as a custom enumeration for the schematic interface as #pJointType.<br>
	typedef enum JType
	{
		JT_Any =-1,/*!<None*/
		JT_Prismatic= 0,/*!<Permits a single translational degree of freedom. See #pJointPrismatic.*/
		JT_Revolute,/*!<Also known as a hinge joint, permits one rotational degree of freedom. See #pJointRevolute.*/
		JT_Cylindrical,/*!<Formerly known as a sliding joint, permits one translational and one rotational degree of freedom. See #pJointCylindrical.*/
		JT_Spherical,/*!<Also known as a ball or ball and socket joint. See #pJointBall.*/
		JT_PointOnLine,/*!<A point on one actor is constrained to stay on a line on another. */
		JT_PointInPlane,/*!<A point on one actor is constrained to stay on a plane on another.*/
		JT_Distance,/*!<A point on one actor maintains a certain distance range to another point on another actor. See #pJointDistance.	*/
		JT_Pulley,/*!<A pulley joint. See #pJointPulley.*/
		JT_Fixed,/*!<A "fixed" connection. See #pJointFixed. */
		JT_D6,/*!<	A 6 degree of freedom joint. See #pJointD6.*/
	};
	/** @} */

	//////////////////////////////////////////////////////////////////////////
	// BodyFlags
	typedef enum E_DEBUG_FLAGS
	{
		E_DEBUG_0,
		E_DEBUG_1,					//ode-body-pos to virtools-entity pos
		E_DEBUG_2,					//updates changes in dev´s interface mode inda geom space
		E_DEBUG_3,
		E_DEBUG_4,
		E_DEBUG_5
	};
	

	/** \addtogroup RigidBody
	@{
	*/
	//! Enumeration is used : <br>
	//! -during body registration. <br>
	//! -through invoking of #pRigidBody::addSubShape().This needs to have BF_SubShape enabled then!.<br>
	//! -as member of #pObjectDescr.<br><br>
	//! This enum is registered as a custom flags for the schematic : #pBFlags.<br>
	typedef enum BodyFlags{
		BF_Moving=1,/*!< Makes the body movable. this can not be altered after creation */
		BF_Gravity=2,/*!< Enables gravity. See #pRigidBody::enableGravity() or \ref PBSetPar   */
		BF_Collision=4,/*!< Enables collisions response. See #pRigidBody::enableCollision() or \ref PBSetPar */
		BF_Kinematic=8,/*!< Act as kinematic object. See #pRigidBody::setKinematic() or \ref PBSetPar  */
		BF_SubShape=16,/*!<Entities with a physic object attribute become physical on scene play. If flag enabled, the holding entity is understood as sub shape of the parent object */
		BF_Hierarchy=32,/*!<Parse the entities hierarchy */
		BF_AddAttributes=64,/*!<Adds the physic attribute on the entity.*/
		BF_TriggerShape=128,/*!<Sets the trigger flag enabled(TF_OnEnter,TF_OnStay and TF_OnLeave). This can only be used if it is a sub shape ! This can be changed by \ref PCSetTriggerMask too*/
		BF_Deformable=256,/*!<Creates a metal cloth. A deformable object can not be a sub shape ! */
		BF_CollisionNotify=512,/*!< Enables using of collisions building blocks. Use or \ref PBSetPar to alter this */
		BF_CollisionsForce=1024,/*!< Enables using of collisions building blocks. Use or \ref PBSetPar to alter this */
		BF_ContactModify=2048,/*!< Enables using of collisions building blocks. Use or \ref PBSetPar to alter this */
		BF_Sleep=4096/*!< Puts the rigid body to sleep */
	};

	/**
	\brief   Flags which describe the format and behavior of a convex mesh. 
	*/
	enum pConvexFlags
	{
		/**
		\brief Used to flip the normals if the winding order is reversed. The PhysX libraries assume that the face normal of a triangle with vertices [a,b,c] can be computed as: edge1 = b-a edge2 = c-a face_normal = edge1 x edge2. Note: this is the same as counterclockwise winding in a right handed graphics coordinate system.
		*/
		CF_FlipNormals              = (1<<0),
		/**
		\brief Denotes the use of 16-bit vertex indices (otherwise, 32-bit indices are used)
		*/
		CF_16BitIndices           = (1<<1),
		/**
		\brief Automatically recomputes the hull from the vertices. If this flag is not set, you must provide the entire geometry manually.
		*/
		CF_ComputeConvex           = (1<<2),
		/**
		\brief Inflates the convex object according to skin width. Note: This flag is only used in combination with CF_ComputeConvex. 
		*/
		CF_InflateConvex           = (1<<3),
		/**
		\brief Instructs cooking to save normals uncompressed. The cooked hull data will be larger, but will load faster.
		*/
		CF_UncompressedNormals = (1<<6),
	};

	/*\brief
		Enable/disable freezing for this body. 

		\note This is an EXPERIMENTAL feature which doesn't always work on in all situations, e.g. 
		for actors which have joints connected to them.

		To freeze an actor is a way to simulate that it is static. The actor is however still simulated
		as if it was dynamic, it's position is just restored after the simulation has finished. A much
		more stable way to make an actor temporarily static is to raise the #BF_Kinematic flag.
	*/
	typedef enum BodyLockFlags
	{
		BF_LPX=2,
		BF_LPY=4,
		BF_LPZ=8,
		BF_LRX=16,
		BF_LRY=32,
		BF_LRZ=64,
	};

	//! Flags which control the behavior of a material.
	//!	@see pMaterial 
	typedef enum MaterialFlags
	{
		/**
		\brief Flag to enable anisotropic friction computation. 

		For a pair of actors, anisotropic friction is used only if at least one of the two bodies materials are anisotropic.
		The anisotropic friction parameters for the pair are taken from the material which is more anisotropic (i.e. the difference
		between its two dynamic friction coefficients is greater).

		The anisotropy direction of the chosen material is transformed to world space:

		dirOfAnisotropyWS = shape2world * dirOfAnisotropy

		Next, the directions of anisotropy in one or more contact planes (i.e. orthogonal to the contact normal) have to be determined. 
		The two directions are:

		uAxis = (dirOfAnisotropyWS ^ contactNormal).normalize()
		vAxis = contactNormal ^ uAxis

		This way [uAxis, contactNormal, vAxis] forms a basis.

		It may happen, however, that (dirOfAnisotropyWS | contactNormal).magnitude() == 1 
		and then (dirOfAnisotropyWS ^ contactNormal) has zero length. This happens when 
		the contactNormal is coincident to the direction of anisotropy. In this case we perform isotropic friction. 

		@see pMaterial.dirOfAnisotropy
		*/
		MF_Anisotropic=1,
		/**
		If this flag is set, friction computations are always skipped between shapes with this material and any other shape.
		It may be a good idea to use this when all friction is to be performed using the tire friction model (see ::pWheel2).

		@see pWheel2
		*/
		MF_DisableFriction = 1 << 4,
		/**
		The difference between "normal" and "strong" friction is that the strong friction feature
		remembers the "friction error" between simulation steps. The friction is a force trying to
		hold objects in place (or slow them down) and this is handled in the solver. But since the
		solver is only an approximation, the result of the friction calculation can include a small
		"error" - e.g. a box resting on a slope should not move at all if the static friction is in
		action, but could slowly glide down the slope because of a small friction error in each 
		simulation step. The strong friction counter-acts this by remembering the small error and
		taking it to account during the next simulation step.

		However, in some cases the strong friction could cause problems, and this is why it is
		possible to disable the strong friction feature by setting this flag. One example is
		raycast vehicles, that are sliding fast across the surface, but still need a precise
		steering behavior. It may be a good idea to reenable the strong friction when objects
		are coming to a rest, to prevent them from slowly creeping down inclines.

		Note: This flag only has an effect if the MF_DisableFriction bit is 0.

		@see pWheel2
		*/
		MF_DisableStrongFriction = 1 << 5,
	};

	//! Enumeration is used : <br>
	//! -during body registration. <br>
	//! -as member of #pObjectDescr.<br><br>
	//! This enum is registered as a custom flags for the schematic : #pBHullType.<br>
	typedef enum HullType {
		HT_Sphere   = 0,
		/*!<A spherical shape type defined by the meshes radius.
		<br>
		*/
		HT_Box         = 1, 
		/*!<A box shape. The entities rotation is set to 0,0,0 (degree) temporary in order to determine the box dimension. It assumes the bodies pivot is aligned to the world frame. 
		<br>
		*/
		HT_Capsule         = 2,
		/*!<A capsule shape defined by length and  radius. <br>
				Assuming bodies pivot is aligned to the world frame, the entities rotation is set to 0,0,0 (degree) temporary in order to determine the capsule parameters whereas :<br>
				capsule length = boxMeshSize.y - boxMeshSize.x and <br>
				capsule radius = boxMeshSize.x / 2  <br>
		<br>
		*/
		HT_Plane     = 3,
		/*!<A plane shape.
		\todo : not implemented yet !
		<br>
		*/
		HT_Mesh       =4,
		/*!<A mesh shape.Its utilizing the NxTrianglemesh.
		\note The number of vertices is unlimited. <br>
		\note This mesh type will not create contact points with another trieangle meshs. Avoid this at any cost, otherwise Virtools will crash!<br>
		<br>
		*/
		HT_ConvexMesh       =5,
		/*!<A convex mesh shape.
		\note The number of vertices is limited to 256 vertices. This is because of Ageia. <br>
		\note This type will create contact points with all other types!
		<br>
		*/
		HT_Heightfield=6,
		/*!<A plane shape.
		\todo : not implemented yet !
		<br>
		*/
		HT_Wheel=7,
		/*!<A wheel shape.
		\todo : not implemented yet !
		<br>
		*/
		HT_Cloth=8,
		/*!<A cloth shape.
		\todo : not implemented yet !
		<br>
		*/
		HT_ConvexCylinder=9,
		/*!<A convex cylinder shape.
		<br>
		*/
		HT_Unknown,
		/*!<Indicates a unspecified type.
		<br>
		*/
	};
	/** @} */

	typedef enum E_LOG_ITEMS
	{
		E_LI_AGEIA,
		E_LI_MANAGER,
		E_VSL,
		E_BB,
	};

	typedef enum E_PHYSIC_ERROR
	{
		E_PE_OK,
		E_PE_AGEIA_ERROR,
		E_PE_INVALID_PARAMETER,
		E_PE_INVALID_OPERATION,
	};

	typedef enum E_MANAGER_FLAGS
	{
		E_MF_OK,
		E_MF_PSDK_LOADED,
		E_MF_PSDK_FAILED,
		E_MF_DEFAULT_WORLD_CREATED,
		E_MF_DEFAULT_CONFIG_LOADED,
		E_MF_LOADING_DEFAULT_CONFIG_FAILED,
		E_MF_FACTORY_CREATED,
	};
	typedef enum E_MANAGER_INIT_FLAGS
	{
		E_MFI_LOAD_CONFIG,
		E_MFI_CREATE_FACTORY,
		E_MFI_CREATE_DEFAULT_WORLD,
		E_MFI_USE_XML_WORLD_SETTINGS
	};


	/** \addtogroup RigidBody
	@{
	*/
	//! This enum is registered as a custom flags for the schematic : #pBForceMode.<br>
	//! Enumeration to force related calls.
	//*!	Is used for force related calls. Registered as custom enumeration #pBForceMode. */
	enum ForceMode
	{
		FM_Force,                   /*!< parameter has unit of mass * distance/ time^2, i.e. a force*/
		FM_Impulse,                /*!< parameter has unit of mass * distance /time */
		FM_VelocityChange,			/*!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.*/
		FM_SmoothImpulse,          /*!< same as FM_Impulse but the effect is applied over all sub steps. Use this for motion controllers that repeatedly apply an impulse.*/
		FM_SmoothVelocityChange,	/*!< same as FM_VelocityChange but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.*/
		FM_Acceleration				/*!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.*/
	};

	//! This enum is registered as custom hidden enumeration for pCCDSettings-
	enum CCDFlags
	{
		CCD_Shared,                   /*!< Is reusing a ccd skeleton*/
	};

	/** @} */

	/** \addtogroup RigidBody
	@{
	*/
	//! Flag that determines the combine mode. When two bodies come in contact with each other, 
	//!	 they each have materials with various coefficients, but we only need a single set of coefficients for the pair. 
	//! Physics doesn't have any inherent combinations because the coefficients are determined empirically on a case by case basis. However, simulating this with a pairwise lookup table is often impractical. For this reason the following combine 
	//! behaviors are available: CM_Average CM_Min CM_Multiply CM_Max 
	//! The effective combine mode for the pair is max(material0.combineMode, material1.combineMode).
	enum CombineMode
	{
		CM_Average,/*!<Average: (a + b)/2.*/
		CM_Min,/*!<Minimum: min(a,b). */
		CM_Multiply,/*!<Multiply: a*b. */
		CM_Max,/*!<Maximum: max(a,b).*/
	};



	/** @} */


	typedef enum E_BODY_CREATION_FLAGS
	{
		E_BCF_USE_WORLD_DAMPING,
		E_BCF_USE_WORLD_SLEEP_SETTINGS,
		E_BCF_USE_WORLD_MATERIAL,
		E_BCF_CREATE_ATTRIBUTES
	};


	/** \addtogroup D6
	@{
	*/
	//! Enumeration is used to specify the range of motions allowed for a DOF in a D6 joint. 
	//! Registered as custom enumeration #pJD6MotionMode.
	typedef enum D6MotionMode
	{
		D6MM_Locked,/*!<The DOF is locked, it does not allow relative motion. 	*/
		D6MM_Limited,/*!<The DOF is limited, it only allows motion within a specific range.	*/
		D6MM_Free,/*!<The DOF is free and has its full range of motions. */
	};
	/** @} */

	
	/** \addtogroup D6
	@{
	*/
	//! Enumeration is used to specify a particular drive method. i.e. Having a position based goal or a velocity based goal. 
	//! Registered as custom enumeration #pJD6DriveType.
	typedef enum D6DriveType
	{
		D6DT_Position  = 1 <<0,	
		/*!<Used to set a position goal when driving.<br>	
		\note The appropriate target positions/orientations should be set.
		*/
		D6DT_Velocity  = 1 << 1,
		/*!<Used to set a velocity goal when driving. <br>
		\note The appropriate target velocities should beset.*/
	};
	/** @} */


	/** \addtogroup D6
	@{
	*/
	//! Enumeration is used to specify a particular degree of freedom.Registered as custom enumeration #pJD6Axis.
	typedef enum D6MotionAxis
	{

		D6MA_Twist,/*!<Rotational axis.*/
		D6MA_Swing1,/*!<Rotational axis.*/
		D6MA_Swing2,/*!<Rotational axis.	*/
		D6MA_X,/*!<Linear axis.*/
		D6MA_Y,/*!<Linear axis.*/
		D6MA_Z/*!<	Linear axis.*/
	};
	/** @} */

	/** \addtogroup D6
	@{
	*/
	//! Enumeration is used to specify a particular axis for a soft limit. Registered as custom enumeration #pJD6LimitAxis.
	typedef enum D6LimitAxis
	{
		D6LA_Linear,/*!<Linear axis.*/
		D6LA_Swing1,/*!<Rotational axis.*/
		D6LA_Swing2,/*!<Rotational axis.	*/
		D6LA_TwistHigh,/*!<Rotational axis.	*/
		D6LA_TwistLow/*!<Rotational axis.	*/

	};
	/** @} */

	/** \addtogroup D6
	@{
	*/
	//! Enumeration is used to specify a particular axis for drives. Registered as custom enumeration #pJD6DriveAxis.
	typedef enum D6DriveAxis
	{

		D6DA_Twist=0,/*!<Rotational axis.Twist is defined as the rotation about the x-axis	*/
		D6DA_Swing=1,/*!<Rotational axis.Swing is defined as the rotation of the x-axis with respect to the y- and z-axis. */
		D6DA_Slerp=2,/*!<Rotational axis.	*/
		D6DA_X=3,/*!<Linear axis = joint axis */
		D6DA_Y=4,/*!<Linear axis = joint normal axis. */
		D6DA_Z=5,/*!<Linear axis = x-axis cross y-axis. */
	};
	/** @} */

	/** \addtogroup Joints
	@{
	*/
	/**
	Joint projection is a method for correcting large joint errors. Joint errors occur when a joint's constraint is violated - imagine the ball being pulled out of the socket in a spherical joint. Under normal circumstances, joint errors are small yet unavoidable due to the imprecise nature of floating point math and numerical integrators. The SDK applies minimal correcting forces to the simulation to try and reduce joint errors over time.
	However, if a joint error becomes very large, more drastic measures are necessary, such as joint projection. The SDK can, in some situations, project (or change) the position of objects directly to fix the joint error. But when projecting the joint it is not desirable to completely remove the joint error because the correcting forces, which also affect velocity, would become zero.
	*/
	typedef enum ProjectionMode
	{
		PM_None,/*!<Disables projection.*/
		PM_MinPointDist,/*!<Performs both linear and angular projection. */
		PM_MinLinearDist,/*!<Only projects linear distances, for improved performance.*/
	};
	/** @} */
	

	/** \addtogroup Vehicle
	@{
	*/



	/**
		Flags to describe the internal used shape types behavior.  This can be done at any time with : <br>
			- building block \ref PVWSet
			- #pWheel::setShapeFlags

	*/
	/**
	Flags to describe the wheels control facility.  This can be done at any time with : <br>
	- building block \ref PVWSet
	- #pWheel::setWheelFlags
	*/
	typedef enum VehicleFlags
	{
		VF_UseAdvance			= (1 << 0),
	};
	typedef enum WheelShapeFlags
	{
		/**
		\brief Determines whether the suspension axis or the ground contact normal is used for the suspension constraint.
		*/
		WSF_WheelAxisContactNormal = 1 << 0,

		/**
		\brief If set, the lateral slip velocity is used as the input to the tire function, rather than the slip angle.

		*/
		WSF_InputLatSlipVelocity = 1 << 1,

		/**
		\brief If set, the longitudinal slip velocity is used as the input to the tire function, rather than the slip ratio.  
		*/
		WSF_InputLongSlipVelocity = 1 << 2,

		/**
		\brief If set, does not factor out the suspension travel and wheel radius from the spring force computation.  This is the legacy behavior from the raycast capsule approach.
		*/
		WSF_UnscaledSpringBehavior = 1 << 3, 

		/**
		\brief If set, the axle speed is not computed by the simulation but is rather expected to be provided by the user every simulation step via NxWheelShape::setAxleSpeed().
		*/
		WSF_AxleSpeedOverride = 1 << 4,
		/**
		\brief If set, the wheels shape will emulate the legacy raycast capsule based wheel.
		See #pVWheelFunction
		*/
		WSF_EmulateLegacyWheel = 1 << 5,

		/**
		\brief If set, the shape will clamp the force in the friction constraints.
		See #pVWheelFunction
		*/
		WSF_ClampedFriction = 1 << 6,
	};

	/**
	Flags to describe the wheels control facility.  This can be done at any time with : <br>
	- building block \ref PVWSet
	- #pWheel::setWheelFlags
	*/
	typedef enum WheelFlags
	{

			/**
			\brief If set, the wheels shape will emulate the legacy raycast capsule based wheel.
			See #pVWheelFunction
			*/
			WF_SteerableInput			= (1 << 0),
			WF_SteerableAuto			= (1 << 1),
			WF_AffectedByHandbrake		= (1 << 2),
			WF_Accelerated				= (1 << 3),
			WF_VehicleControlled		= (1 << 4),
			WF_AffectedByDifferential	= (1 << 5),
			WF_IgnoreTireFunction		= (1 << 6),

			WF_AllWheelFlags		= WF_SteerableInput
			| WF_SteerableAuto
			| WF_AffectedByHandbrake
			| WF_Accelerated
			| WF_VehicleControlled
			| WF_AffectedByDifferential,

	};

	typedef enum E_VEHICLE_STATE_FLAGS
	{
		E_VSF_HAS_GEARS=(1<<0),
		E_VSF_HAS_MOTOR=(1<<1),
		E_VSF_HAS_GROUND=(1<<2),
		E_VSF_IS_BRAKING=(1<<3),
		E_VSF_IS_BREAKPEDAL=(1<<4),
		E_VSF_BREAKPEDAL_CHANGED=(1<<5),
		E_VSF_ACC_PEDAL=(1<<6),
		E_VSF_RELEASING_BRAKE_PEDAL=(1<<7),
	};

		
	/** @} */

	typedef enum pParticleRenderType
	{

		PRT_Point,
		PRT_Sprite,

	};

	typedef enum E_OBJECT_CREATION_FLAGS
	{

		E_OCF_ROTATION,
		E_OFC_POSITION,
		E_OFC_DIMENSION,
		E_OFC_PERISITENT,

	};

	typedef enum xManagerCallMask
	{
		XCM_PreProcess =  1 << 0 ,
		XCM_PostProcess =  1 << 0
	};



#endif // __VTINTERFACEENUMERATION_H__