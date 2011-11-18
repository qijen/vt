/********************************************************************
	created:	2009/02/17
	created:	17:2:2009   8:26
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\include\core\Common\pTypes.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\include\core\Common 
	file base:	pTypes
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Type definitions, Function pointers 
*********************************************************************/
#ifndef __P_TYPES_H__
#define __P_TYPES_H__

#include "vtPhysXBase.h"
#include "CKAll.h"


//################################################################
//
// Common used types for rigid bodies and joints
//


/**
\brief Describes a joint spring. The spring is implicitly integrated, so even high spring and damper coefficients should be robust.
pSpring is registered as custom structure #pJSpring and can be accessed or modified through parameter operations.<br>

*/
class pSpring
{

public:
	/**
	Damper coefficient
	*/
	float damper;
	/**
	Spring coefficient
	*/
	float spring;
	/**
	Target value (angle/position) of spring where the spring force is zero.
	*/
	float targetValue;
	pSpring()
	{
		damper = 0.0f;
		spring = 0.0f;
		targetValue = 0.0f;
	}
	pSpring(float _damper,float _spring,float _targetValue)
	{

		damper = _damper;
		spring = _spring;
		targetValue = _targetValue;
	}
};





/** \addtogroup Collision
@{
*/

/**
\brief 128-bit mask used for collision filtering.

The collision filtering equation for 2 shapes S0 and S1 is:

<pre> (G0 op0 K0) op2 (G1 op1 K1) == b </pre>

with

<ul>
<li> G0 = pGroupsMask for shape S0. See ::setGroupsMask </li>
<li> G1 = pGroupsMask for shape S1. See ::setGroupsMask </li>
<li> K0 = filtering constant 0. See ::setFilterConstant0 </li>
<li> K1 = filtering constant 1. See ::setFilterConstant1 </li>
<li> b = filtering boolean. See ::setFilterBool </li>
<li> op0, op1, op2 = filtering operations. See ::setFilterOps </li>
</ul>

If the filtering equation is true, collision detection is enabled.

@see pWorld::setFilterOps()
*/
class pGroupsMask
{
public:
	API_INLINE	pGroupsMask()	{}
	API_INLINE	~pGroupsMask()	{}
	int		bits0, bits1, bits2, bits3;
};



class pRay
{

public:
	VxVector orig;
	VxVector dir;

};

class pRaycastHit
{
public:
	float distance;
	CK3dEntity *shape;
	VxVector worldImpact;
	VxVector worldNormal;
	int faceID;
	int internalFaceID;
	float u;
	float v;
	int materialIndex;
	int flags;

	pRaycastHit()
	{ 
		distance = 0.0;
		shape = NULL;
		u = 0.0f;
		v= 0.0f;
		faceID = 0 ; 
		materialIndex = 0 ; 
		flags = 0;

	}

};

/**
\brief Class for describing rigid bodies ccd settings.
*/
class pCCDSettings
{
public : 
	
	bool isValid();
	bool setToDefault();
	bool evaluate(CKBeObject*referenceObject);

	float motionThresold;
	int flags;
	CK_ID meshReference;
	float scale;

	pCCDSettings()
	{
		motionThresold = 0.0;
		flags = 0 ;
		meshReference =0;
		scale = 1.0f;
	}
};

/**

\brief Class for describing rigid bodies or its sub shapes collisions settings.

\sa #pRigidBody::updateCollisionSettings()
*/
class pCollisionSettings
{
	public:
		
		bool isValid();
		bool setToDefault();
		/**
		\brief	collisions group the shape belongs to
			-	<b>Range:</b> [0,32]
			-	<b>Default:</b> 0
		*/
		int collisionGroup;
		/**
		\brief 128-bit mask used for collision filtering
		-	<b>Range:</b> [0,4x4 bits]
		-	<b>Default:</b> all off
		*/
		pGroupsMask groupsMask;
		/**
		\brief Additional collisions offset
		-	<b>Range:</b> [0,1.0f]
		-	<b>Default:</b>0.25f
		-	Can NOT altered after creation
		*/
		float skinWidth;

	pCollisionSettings()
	{
		collisionGroup = 0;
		skinWidth = 0.025f;
	}
};

//@}






typedef std::vector<NxRaycastHit*>pRayCastHits;


/**
\brief Class for describing rigid bodies optimization.
*/
class MODULE_API pOptimization 
{
	public : 

		bool isValid();
		bool setToDefault();

		
		

		/**
		\brief Flags to lock a rigid body in certain degree of freedom. See also #pRigidBody::lockTransformation
		*/
		BodyLockFlags transformationFlags;
		/**
		\brief Sets the linear damping coefficient. Zero represents no damping. The damping coefficient must be nonnegative. See pRigidBody::setLinearDamping
		*/
		float linDamping;
		/**
		\brief Sets the angular damping coefficient.Zero represents no damping. The damping coefficient must be nonnegative.See pRigidBody::setAngularDamping
		*/
		float angDamping;
		/**
		\copydoc pRigidBody::setSolverIterationCount
		*/
		int solverIterations;
		/**
		\copydoc pRigidBody::setDominanceGroup
		*/
		int dominanceGroup;
		/**
		\brief Not Implemented
		*/
		int compartmentGroup;
		/**
		\copydoc pRigidBody::setSleepEnergyThreshold
		*/
		float sleepEnergyThreshold;
		/**
		\copydoc pRigidBody::setSleepLinearVelocity
		*/
		float linSleepVelocity;
		/**
		\copydoc pRigidBody::setSleepAngularVelocity
		*/
		float angSleepVelocity;

		pOptimization()
		{

			transformationFlags = (BodyLockFlags)0;

			linDamping  = angDamping = 0.0f;
			solverIterations = 24 ;
			dominanceGroup=0;
			compartmentGroup=0;
			sleepEnergyThreshold =0.0f;
			linSleepVelocity = angSleepVelocity = 0.0;
		}

};

/**
\brief Class for describing a shape's surface properties.
*/
class MODULE_API pMaterial 
{
public : 

	bool isValid();

	/**
	\brief	Flags, a combination of the bits defined by the enum ::pMaterialFlag <br>
		-	<b>Default:</b> 0

	@see MaterialFlags
	*/
	int flags;

	/**
	\brief	Coefficient of dynamic friction. If set to greater than staticFriction, <br>
			the effective value of staticFriction will be increased to match. if flags & MF_Anisotropic is set, then this value is used for the primary direction of anisotropy (U axis)<br>
		-	<b>Range:</b> [0,+inf)
		-	<b>Default:</b> [0)
	*/
	float                                          dynamicFriction;
	/**
	\brief 	Coefficient of static friction. If flags & MF_Anisotropic is set,<br>
			then this value is used for the primary direction of anisotropy (U axis)
	*/
	float                                          staticFriction;
	/**
	\brief Coefficient of restitution. Makes the object bounce as little as possible, higher values up to 1.0 result in more bounce. Note that values close to or above 1 may cause stability problems and/or increasing energy.
		-	<b>Range:</b> [0,1)
		-	<b>Default:</b> [0)
	*/
	float                                          restitution;
	/**
	\brief Anisotropic dynamic friction coefficient for along the secondary (V) axis of anisotropy. This is only used if flags & MF_Anisotropic is set.
		-	<b>Range:</b> [0,inf)
		-	<b>Default:</b> [0)
	*/
	float                                          dynamicFrictionV;
	/**
	\brief Anisotropic static friction coefficient for along the secondary (V) axis of anisotropy. This is only used if flags & MF_Anisotropic is set. 
		-	<b>Range:</b> [0,inf)
		-	<b>Default:</b> [0)
	*/
	float                                          staticFrictionV;
	/**
	\brief Friction combine mode. See the enum CombineMode .
		-	<b>Range:</b> [CombineMode)
		-	<b>Default:</b> [CM_Average)
	*/
	CombineMode                                  frictionCombineMode;
	/**
	\brief Restitution combine mode. See the enum CombineMode .
		-	<b>Range:</b> [CombineMode)
		-	<b>Default:</b> [CM_Average)
	*/
	CombineMode                                  restitutionCombineMode;
	/**
	\brief shape space direction (unit vector) of anisotropy. This is only used if flags & MF_Anisotropic is set. 
		-	<b>Range:</b> [vector)
		-	<b>Default:</b> [1.0f,0.0f,0.0f)
	*/
	VxVector                                         dirOfAnisotropy;

	/**
	\brief unique name of the material 
	*/
	const char*									name;

	/**
	\brief Enumeration to identify a material setup from PhysicDefaults.xml. Is being populated automatically on reset.
		-	<b>Range:</b> [0,Number of material setups in xml file)
		-	<b>Default:</b> [NONE)
	*/
	int													xmlLinkID;

	int getNxMaterialID() const { return mNxMaterialID; }
	void setNxMaterialID(int val) { mNxMaterialID = val; }

	pMaterial()
	{
		setToDefault();
	}
	__inline void setToDefault()
	{
		dynamicFriction	= 0.0f;
		staticFriction	= 0.0f;
		restitution		= 0.0f;
		dynamicFrictionV= 0.0f;
		staticFrictionV = 0.0f;
		dirOfAnisotropy = VxVector(1,0,0);
		flags = 0;
		frictionCombineMode = CM_Average;
		restitutionCombineMode = CM_Average;
		xmlLinkID =0;
		name = "";
	}
	
	

private :

	int mNxMaterialID;
};



#endif