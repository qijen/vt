#ifndef __PRIGIDBODYTYPES_H__
#define __PRIGIDBODYTYPES_H__

#include "vtPhysXBase.h"
#include "pVTireFunction.h"
//#include "pWheelTypes.h"



/** \addtogroup RigidBody
@{
*/

struct pRigidBodyRestoreInfo 
{
	bool hierarchy;
	bool removeJoints;
	pRigidBodyRestoreInfo() : hierarchy(false) ,  removeJoints(false){}

};

/**
\brief Class describing a rigid bodies mass offset.
*/
class MODULE_API pMassSettings
{

	public:
		/**
		\brief Density scale factor of the shapes belonging to the body 
			-	<b>Range:</b> [0,inf)
			-	<b>Default:</b> [0)
		**/
		float newDensity;
		 
		/**
		\brief Total mass of the actor(or zero).
			-	<b>Range:</b> [0,inf)
			-	<b>Default:</b> 0.0f
		**/
		float totalMass;
		/**
		\brief Local position offset
			-	<b>Range:</b> [vector)
			-	<b>Default:</b> (0,0,0)
		**/
		VxVector localPosition;
		/**
		\brief Local orientation offset
		-	<b>Range:</b> [0,inf)
		-	<b>Default:</b> (0,0,0)
		**/
		VxVector localOrientation;
		/**
		\brief Reference object to determine the mass local position. The members "Linear" and "Angular" are being used to transform the final matrix.
		-	<b>Range:</b> [object)
		-	<b>Default:</b> 0
		**/
		CK_ID massReference;

	pMassSettings()
	{
		newDensity = totalMass = 0.0f;
		massReference = 0 ;
	}
};

/**

\brief Class to describe a pivot offset
*/
class MODULE_API pPivotSettings
{


	public:

		bool isValid();
		bool setToDefault();
		bool evaluate(CKBeObject*referenceObject);
		

		/**
		\brief Local position offset. The local position of can be changed by #pRigidBody::setPosition
		-	<b>Range:</b> [vector)
		-	<b>Default:</b> (0,0,0)
		**/
		VxVector localPosition;
		/**
		\brief Local orientation offset. The local orientation of can be changed by #pRigidBody::setRotation
		-	<b>Range:</b> [0,inf)
		-	<b>Default:</b> (0,0,0)
		**/
		VxVector localOrientation;
		/**
		\brief Reference object to determine the shape local matrix. The members "Linear" and "Angular" are being used to transform the final matrix.
		-	<b>Range:</b> [object)
		-	<b>Default:</b> 0
		**/
		CK_ID pivotReference;

	pPivotSettings()
	{
		pivotReference=0;
	}

};


//----------------------------------------------------------------
//
//! \brief Help info to specify a length by an entity reference
//
class MODULE_API pAxisReferencedLength
{
public:
	bool isValid();
	bool setToDefault();
	bool evaluate(CKBeObject *referenceObject);

	float value;
	CKBeObject *reference;
	int referenceAxis;

	pAxisReferencedLength() : 
	value(0.0f) , 
		reference(NULL) , 
		referenceAxis(0) 
	{
	}
};

//----------------------------------------------------------------
//
//! \brief Additional information for deformable objects
//
struct MODULE_API pDeformableSettings
{

public : 

	bool isValid();
	bool setToDefault();
	bool evaluate(CKBeObject*referenceObject);

	float ImpulsThresold;
	float PenetrationDepth;
	float MaxDeform;

	pDeformableSettings()
	{
		ImpulsThresold = 50.0f;
		PenetrationDepth = 0.1f ;
		MaxDeform= 0.1f;
	}
};

//----------------------------------------------------------------
//
//! \brief Additional settings to override a default capsule
//
struct pCapsuleSettings
{

public :

	int localLengthAxis;
	int localRadiusAxis;

	float height;
	float radius;
	pCapsuleSettings()
	{
		localRadiusAxis=0;
		localLengthAxis=0;
		height = 0.0f;
		radius = 0.0f;
	}
};

//----------------------------------------------------------------
//
//! \brief Additional settings to override a default capsule
//
struct MODULE_API pCapsuleSettingsEx
{
public :

	bool isValid();
	bool setToDefault();
	bool evaluate(CKBeObject*referenceObject);

	/**
	\brief Radius specified by value or an objects local box size and an axis
	**/
	pAxisReferencedLength radius;
	
	/**
	\brief Height specified by value or an objects local box size and an axis
	**/
	pAxisReferencedLength height;

	pCapsuleSettingsEx()
	{
		
	}
};



//----------------------------------------------------------------
//
//! \brief Optional override to describe a convex cylinder.
//
class MODULE_API pConvexCylinderSettings
{

public:


	bool isValid();
	bool setToDefault();
	bool evaluate(CKBeObject*referenceObject);

	
	/**
	\brief Radius specified by value or an objects local box size and an axis
	**/
	pAxisReferencedLength radius;
	/**
	\brief Height specified by value or an objects local box size and an axis
	**/
	pAxisReferencedLength height;

	/**
	\brief Amount of cap segments
	**/
	int approximation;

	/**
	\brief Rotation part 1. If a forward axis reference has been specified, the vector gets transformed
	**/
	VxVector forwardAxis;
	/**
	\brief Reference to specify the "Dir" axis
	**/
	CK_ID forwardAxisRef;

	/**
	\brief Rotation part 2. If a down axis reference has been specified, the vector gets transformed
	**/
	VxVector downAxis;
	/**
	\brief Reference to specify the "Up" axis
	**/
	CK_ID downAxisRef;

	/**
	\brief Rotation part 3. If a right axis reference has been specified, the vector gets transformed
	**/
	VxVector rightAxis;
	/**
	\brief Reference to specify the "Right" axis
	**/
	CK_ID rightAxisRef;

	/**
	\brief Create only a half cylinder
	*/
	bool buildLowerHalfOnly;

	/**
	\brief Flags which describe the format and behavior of a convex mesh. See #pConvexFlags
	*/
	pConvexFlags convexFlags;

	pConvexCylinderSettings() : 
	approximation(0),
		forwardAxisRef(0),
		rightAxisRef(0),
		downAxisRef(0),
		buildLowerHalfOnly(false)
	{
		setToDefault();
		convexFlags=((pConvexFlags)(CF_ComputeConvex));
		

		
	}
};

class MODULE_API pWheelDescr 
{

public :

	//################################################################
	//
	// Wheel Capsule Specific 
	//
	int					wheelApproximation;

	pAxisReferencedLength radius;
	float width;

	//################################################################
	//
	// Wheel Common Attributes 
	//
	float					wheelSuspension;
	float					springRestitution;
	float					springDamping;
	float					springBias;

	float					maxBrakeForce;

	float					frictionToSide;
	float					frictionToFront;

	WheelFlags					wheelFlags;

	//################################################################
	//
	// Wheel Shape Specific 
	//

	WheelShapeFlags					wheelShapeFlags;
	float				inverseWheelMass;
	pConvexCylinderSettings convexCylinder;
	
	pConvexCylinderSettings getConvexCylinderSettings() { return convexCylinder; }
	void setConvexCylinderSettings(pConvexCylinderSettings val) { convexCylinder = val; }
	pTireFunction latFunc;int latFuncXML_Id;
	pTireFunction longFunc;int longFuncXML_Id;

	pWheelDescr(){ setToDefault(); }
	~pWheelDescr(){}

	void*					userData;
	void setToDefault();
	bool isValid() const;

};

//----------------------------------------------------------------
//
//! \brief Container to store joints
//
typedef XArray<pJoint*>JointListType;


class pClothDescr
{

};

/**
!	\brief Describes a rigid body. 

This is used by #pFactory::createBody() and #pRigidBody::addSubShape() only.

*/
class MODULE_API pObjectDescr
{


public : 

	 int mask;


	typedef enum E_OD_VERSION
	{
		OD_DECR_V0,
		OD_DECR_V1,
	};


	CK_ID worlReference;

	int version;
	/**
	\brief The shape type of the body. Default  = HT_Sphere.
	*/
	HullType hullType;
	/**
	\brief The shape type of the body. Default  = 1.0f. 
	*/
	float density;

	/**
	\brief Translates the mass center after the body is created in local space.
	*/
	VxVector massOffset;
	/**
	\brief Translates the pivot after the body is created in local space.
	*/
	VxVector shapeOffset;

	/**
	\brief Translates the pivot after the body is created in local space.
	*/
	VxVector pivotOffsetLinear;
	/**
	\brief Translates the pivot after the body is created in local space.
	*/
	CK_ID pivotOffsetReference;

	/**
	\brief Translates the pivot after the body is created in local space.
	*/
	VxVector pivotOffsetAngular;
	/**
	\brief Describes several properties. See #BodyFlags. Default  = 0. 
	*/
	BodyFlags flags;
	
	/**
	\brief Will parse the entities hierarchy for additional sub shapes. Child objects needs to have the physic object attribute attached. Default  = false. 
	*/
	bool hirarchy;
	/**
	\brief If there are sub shape attached, #pRigidBodie::addSubShape will call #pRigidBody::updateMassFromShapes(). Default  = 0.0f. 
	*/
	float newDensity;
	/**
	\brief If there are sub shape attached, #pRigidBodie::addSubShape will call #pRigidBody::updateMassFromShapes().  Default  = 0.0f. 
	*/
	float totalMass; 
	
	int subEntID;
	int transformationFlags;

	int internalXmlID;
	int externalXmlID;
	int xmlImportFlags;
	VxVector pivotAngularOffset;


	/************************************************************************************************/
	/** @name Collision
	*/
	//@{

		/**
			\brief Sets 128-bit mask used for collision filtering. See comments for ::pGroupsMask
		*/
		pGroupsMask groupsMask;

		/**
		\brief Adds an additional offset for collision response.
		*/
		float skinWidth;

		/**
		\brief The collision group the body belongs to. Default  = 0. 
		*/
		int collisionGroup;

	
	//@}
	

	/************************************************************************************************/
	/** @name CCD Settings
	*/
	//@{

		float ccdMotionThresold;
		int ccdFlags;
		CK_ID ccdMeshReference;
		float ccdScale;

	
	//@}

	

	/************************************************************************************************/
	/** @name Damping
	*/
	//@{

	/**
	\brief the linear damping.
	*/
	float linearDamping;

	/**
	\brief the angular damping.
	*/
	float angularDamping;


	//@}

	/************************************************************************************************/
	/** @name Sleeping Settings
	*/
	//@{

		float linearSleepVelocity;
		float angularSleepVelocity;
		float sleepingEnergyThresold;

	
	//@}


	/************************************************************************************************/
	/** @name Optimization
	*/
	//@{

		int solverIterations;
		int dominanceGroups;
		int compartmentID;

		
	//@}

	/************************************************************************************************/
	/** @name Mass
	*/
	//@{

		VxVector massOffsetLinear;
		VxVector massOffsetAngular;
		CK_ID massOffsetReference;
	
	//@}


	pOptimization optimization;
		pOptimization& getOptimization() { return optimization; }
		void setOptimization(pOptimization val) { optimization = val; }
	
	pMaterial material;
		pMaterial& getMaterial()  { return material; }
		void setMaterial(pMaterial val) { material = val; }
	
	pCCDSettings ccd;
		pCCDSettings& getCCD()  { return ccd; }
		void setCCD(pCCDSettings val) { ccd = val; }
	
	pCapsuleSettingsEx capsule;
		pCapsuleSettingsEx& getCapsule()  { return capsule; }
		void setCapsule(pCapsuleSettingsEx val) { capsule = val; }
	
	pConvexCylinderSettings convexCylinder;
		pConvexCylinderSettings& getConvexCylinder()  { return convexCylinder; }
		void setConvexCylinder(pConvexCylinderSettings val) { convexCylinder = val; }
	
	pMassSettings mass;
		pMassSettings& getMass()  { return mass; }
		void setMass(pMassSettings val) { mass = val; }
	
	pPivotSettings pivot;
		pPivotSettings& getPivot()  { return pivot; }
		void setPivot(pPivotSettings val) { pivot = val; }

	pCollisionSettings collision;
		pCollisionSettings& getCollision()  { return collision; }
		void setCollision(pCollisionSettings val) { collision = val; }

	pWheelDescr wheel;
		pWheelDescr& getWheel()  { return wheel; }
		void setWheel(pWheelDescr val) { wheel = val; }

	pObjectDescr()
	{
		setToDefault();
	}

	bool isValid();
	bool setToDefault();

};

//----------------------------------------------------------------
//
//! \brief	Meta data to track the relation between a NxShape and an CK3dEntity.
//!			Also used to track a wheel object in the NxShapes user data.
//
struct pSubMeshInfo
{
	pObjectDescr initDescription;
	NxCCDSkeleton *ccdSkeleton;
	int meshID;
	CKBeObject *mesh;

	int entID;
	CKBeObject *refObject;
	CK_ID ccdReference;

	pWheel *wheel;

	pSubMeshInfo()
	{
		wheel = NULL;
		meshID = -1;
		entID = -1 ;
		mesh  = NULL;
		refObject = NULL;
		ccdSkeleton = NULL;
		ccdReference = 0;
	}
};


/** @} */



#endif // __PRIGIDBODYTYPES_H__