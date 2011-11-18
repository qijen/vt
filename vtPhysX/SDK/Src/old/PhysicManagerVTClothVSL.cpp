#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

//#include "pVehicle.h"




PhysicManager *ourMan = NULL;
void __newpClothDescr(BYTE *iAdd)
{
	new(iAdd)pClothDesc();
}


void PhysicManager::_RegisterVSLCloth()
{
	
	ourMan = GetPMan();
		
	
	STARTVSLBIND(m_Context)

	DECLAREOBJECTTYPE(pClothDesc)
	
	DECLAREMEMBER(pClothDesc,float,thickness)
	DECLAREMEMBER(pClothDesc,float,density)
	DECLAREMEMBER(pClothDesc,float,bendingStiffness)

	DECLAREMEMBER(pClothDesc,float,stretchingStiffness)
	DECLAREMEMBER(pClothDesc,float,dampingCoefficient)
	DECLAREMEMBER(pClothDesc,float,friction)
	DECLAREMEMBER(pClothDesc,float,pressure)
	DECLAREMEMBER(pClothDesc,float,tearFactor)
	DECLAREMEMBER(pClothDesc,float,collisionResponseCoefficient)
	DECLAREMEMBER(pClothDesc,float,attachmentResponseCoefficient)
	DECLAREMEMBER(pClothDesc,float,attachmentTearFactor)
	DECLAREMEMBER(pClothDesc,float,toFluidResponseCoefficient)
	DECLAREMEMBER(pClothDesc,float,fromFluidResponseCoefficient)
	DECLAREMEMBER(pClothDesc,float,minAdhereVelocity)
	DECLAREMEMBER(pClothDesc,int,solverIterations)
	DECLAREMEMBER(pClothDesc,VxVector,externalAcceleration)
	DECLAREMEMBER(pClothDesc,VxVector,windAcceleration)
	DECLAREMEMBER(pClothDesc,float,wakeUpCounter)
	DECLAREMEMBER(pClothDesc,float,sleepLinearVelocity)
	DECLAREMEMBER(pClothDesc,int,collisionGroup)
	DECLAREMEMBER(pClothDesc,VxBbox,validBounds)
	DECLAREMEMBER(pClothDesc,float,relativeGridSpacing)
	DECLAREMEMBER(pClothDesc,pClothFlag,flags)
	DECLAREMEMBER(pClothDesc,pClothAttachmentFlag,attachmentFlags)
	DECLAREMEMBER(pClothDesc,VxColor,tearVertexColor)
	DECLAREMEMBER(pClothDesc,CK_ID,worldReference)


	DECLAREMETHOD_0(pClothDesc,void,setToDefault)
	DECLARECTOR_0(__newpClothDescr)
	DECLAREPOINTERTYPE(pWorld)

	DECLAREPOINTERTYPE(pRigidBody)
	DECLAREPOINTERTYPEALIAS(pRigidBody,"pBody")


		

		//////////////////////////////////////////////////////////////////////////
		//
		//					Serializer : 
		//
		DECLAREPOINTERTYPE(pSerializer)
		DECLAREFUN_C_0(pSerializer*, GetSerializer)
		DECLAREMETHOD_2(pSerializer,void,overrideBody,pRigidBody*,int)
		DECLAREMETHOD_2(pSerializer,int,loadCollection,const char*,int)
		DECLAREMETHOD_1(pSerializer,int,saveCollection,const char*)
		DECLAREMETHOD_2(pSerializer,void,parseFile,const char*,int)


		//////////////////////////////////////////////////////////////////////////
		//
		//					Joints :
		//

		
		DECLAREPOINTERTYPE(pJoint)
		DECLAREPOINTERTYPE(pJointFixed)
		DECLAREPOINTERTYPE(pJointDistance)
		DECLAREPOINTERTYPE(pJointD6)
		DECLAREPOINTERTYPE(pJointPulley)
		DECLAREPOINTERTYPE(pJointBall)
		DECLAREPOINTERTYPE(pJointRevolute)
		DECLAREPOINTERTYPE(pJointPrismatic)
		DECLAREPOINTERTYPE(pJointCylindrical)
		DECLAREPOINTERTYPE(pJointPointInPlane)
		DECLAREPOINTERTYPE(pJointPointOnLine)

		
		DECLAREINHERITANCESIMPLE("pJoint","pJointDistance")
		DECLAREINHERITANCESIMPLE("pJoint","pJointD6")
		DECLAREINHERITANCESIMPLE("pJoint","pJointFixed")
		DECLAREINHERITANCESIMPLE("pJoint","pJointPulley")
		DECLAREINHERITANCESIMPLE("pJoint","pJointBall")
		DECLAREINHERITANCESIMPLE("pJoint","pJointRevolute")
		DECLAREINHERITANCESIMPLE("pJoint","pJointPrismatic")
		DECLAREINHERITANCESIMPLE("pJoint","pJointCylindrical")
		DECLAREINHERITANCESIMPLE("pJoint","pJointPointOnLine")
		DECLAREINHERITANCESIMPLE("pJoint","pJointPointInPlane")


		
		//////////////////////////////////////////////////////////////////////////
		//
		//					Factory
		//

		DECLAREPOINTERTYPE(pFactory)
		DECLAREFUN_C_0(pFactory,getPFactory)
		


		//////////////////////////////////////////////////////////////////////////
		//
		//					ENUMERATION 
		//

		DECLAREENUM("D6DriveType")

		DECLAREENUMVALUE("D6DriveType", "D6DT_Position" ,1 )
		DECLAREENUMVALUE("D6DriveType", "D6DT_Velocity" ,2 )


		DECLAREENUM("PForceMode")

		DECLAREENUMVALUE("PForceMode", "PFM_Force" , 0)
		DECLAREENUMVALUE("PForceMode", "PFM_Impulse" , 1)
		DECLAREENUMVALUE("PForceMode", "PFM_VelocityChange" , 2)
		DECLAREENUMVALUE("PForceMode", "PFM_SmoothImpulse" , 3)
		DECLAREENUMVALUE("PForceMode", "PFM_SmoothVelocityChange" , 4)
		DECLAREENUMVALUE("PForceMode", "PFM_Acceleration" , 5)
		
		DECLAREENUM("D6MotionMode")

		DECLAREENUMVALUE("D6MotionMode", "D6MM_Locked" , 0)
		DECLAREENUMVALUE("D6MotionMode", "D6MM_Limited" , 1)
		DECLAREENUMVALUE("D6MotionMode", "D6MM_Free" , 2)

		DECLAREENUM("JType")
		DECLAREENUMVALUE("JType", "JT_Any" , -1)
		DECLAREENUMVALUE("JType", "JT_Prismatic" , 0)
		DECLAREENUMVALUE("JType", "JT_Revolute" , 1)
		DECLAREENUMVALUE("JType", "JT_Cylindrical" , 2)
		DECLAREENUMVALUE("JType", "JT_Spherical" , 3)
		DECLAREENUMVALUE("JType", "JT_PointOnLine" , 4)
		DECLAREENUMVALUE("JType", "JT_PointInPlane" , 5)
		DECLAREENUMVALUE("JType", "JT_Distance" , 6)
		DECLAREENUMVALUE("JType", "JT_Pulley" , 7)
		DECLAREENUMVALUE("JType", "JT_Fixed" ,8 )
		DECLAREENUMVALUE("JType", "JT_D6" ,9 )

		
		//////////////////////////////////////////////////////////////////////////
		//
		//		Bindings for pVehicle related classes : 
		//
		//
		DECLAREMETHOD_2(pFactory,pVehicle*,createVehicle,CK3dEntity*,pVehicleDesc)
		

		//DECLAREMETHOD_0(pVehicle,float,getWheelRollAngle)
		//DECLAREMETHOD_0(pVehicle,float,getRpm)



		
		

		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT CREATION
		//
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointDistance*,createDistanceJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,VxVector(),VxVector,VxVector(),float,0.0f,float,0.0f,pSpring,pSpring())
		DECLAREMETHOD_5(pFactory,pJointD6*,createD6Joint,CK3dEntity*,CK3dEntity*,VxVector,VxVector,bool)
		DECLAREMETHOD_2(pFactory,pJointFixed*,createFixedJoint,CK3dEntity*,CK3dEntity*)
		DECLAREMETHOD_3_WITH_DEF_VALS(pFactory,pRigidBody*,createBody,CK3dEntity*,NODEFAULT,pObjectDescr,NODEFAULT,CK3dEntity*,NULL)
		DECLAREMETHOD_6(pFactory,pJointPulley*,createPulleyJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointBall*,createBallJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointRevolute*,createRevoluteJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointPrismatic*,createPrismaticJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointCylindrical*,createCylindricalJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointPointInPlane*,createPointInPlaneJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		DECLAREMETHOD_4(pFactory,pJointPointOnLine*,createPointOnLineJoint,CK3dEntity*,CK3dEntity*,VxVector,VxVector)
		


		DECLAREMETHOD_2(pFactory,pCloth*,createCloth,CK3dEntity*,pClothDesc)



		//////////////////////////////////////////////////////////////////////////
		//
		//					Cloth
		//
		DECLAREMETHOD_4(pCloth,void,attachToCore,CK3dEntity*,float,float,float)
		DECLAREMETHOD_2(pCloth,void,attachToShape,CK3dEntity*,pClothAttachmentFlag)


		//////////////////////////////////////////////////////////////////////////
		//
		//					MANAGER
		//
		DECLAREFUN_C_0(CKGUID, GetPhysicManagerGUID)
		DECLAREOBJECTTYPE(PhysicManager)
		DECLARESTATIC_1(PhysicManager,PhysicManager*,Cast,CKBaseManager* iM)
		DECLAREFUN_C_0(PhysicManager*, GetPhysicManager)
		
		DECLAREMETHOD_0(PhysicManager,pWorld*,getDefaultWorld)
		DECLAREMETHOD_1(PhysicManager,pRigidBody*,getBody,CK3dEntity*)
		DECLAREMETHOD_1(PhysicManager,pWorld*,getWorldByBody,CK3dEntity*)
		DECLAREMETHOD_1(PhysicManager,pWorld*,getWorld,CK_ID)
		DECLAREMETHOD_3_WITH_DEF_VALS(PhysicManager,pJoint*,getJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NULL,JType,JT_Any)

//		DECLAREMETHOD_0(PhysicManager,void,makeDongleTest)


		//////////////////////////////////////////////////////////////////////////
		//
		//					World
		//
		
		DECLAREMETHOD_1(pWorld,pRigidBody*,getBody,CK3dEntity*)
		DECLAREMETHOD_1(pWorld,pVehicle*,getVehicle,CK3dEntity*)
		DECLAREMETHOD_3(pWorld,pJoint*,getJoint,CK3dEntity*,CK3dEntity*,JType)

		DECLAREMETHOD_3(pWorld,void,setFilterOps,pFilterOp,pFilterOp,pFilterOp)
		DECLAREMETHOD_1(pWorld,void,setFilterBool,bool)
		DECLAREMETHOD_1(pWorld,void,setFilterConstant0,const pGroupsMask&)
		DECLAREMETHOD_1(pWorld,void,setFilterConstant1,const pGroupsMask&)




		//
		DECLAREMETHOD_5_WITH_DEF_VALS(pWorld,bool,raycastAnyBounds,const VxRay&,NODEFAULT,pShapesType,NODEFAULT,pGroupsMask,NODEFAULT,int,0xffffffff,float,NX_MAX_F32)
		DECLAREMETHOD_8(pWorld,bool,overlapSphereShapes,CK3dEntity*,const VxSphere&,CK3dEntity*,pShapesType,CKGroup*,int,const pGroupsMask*,BOOL)

		//(const VxRay& worldRay, pShapesType shapesType, pGroupsMask groupsMask,unsigned int groups=0xffffffff, float maxDist=NX_MAX_F32);



		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT :: Revolute
		//
		DECLAREMETHOD_0(pJoint,pJointRevolute*,castRevolute)
		DECLAREMETHOD_1(pJointRevolute,void,setGlobalAnchor,const VxVector&)
		DECLAREMETHOD_1(pJointRevolute,void,setGlobalAxis,const VxVector&)

		
		DECLAREMETHOD_1(pJointRevolute,void,setSpring,pSpring)
		DECLAREMETHOD_1(pJointRevolute,void,setHighLimit,pJointLimit)
		DECLAREMETHOD_1(pJointRevolute,void,setLowLimit,pJointLimit)
		DECLAREMETHOD_1(pJointRevolute,void,setMotor,pMotor)

		DECLAREMETHOD_0(pJointRevolute,pSpring,getSpring)
		DECLAREMETHOD_0(pJointRevolute,pJointLimit,getLowLimit)
		DECLAREMETHOD_0(pJointRevolute,pJointLimit,getHighLimit)
		DECLAREMETHOD_0(pJointRevolute,pMotor,getMotor)
		DECLAREMETHOD_1(pJointRevolute,void,enableCollision,bool)






		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT :: Ball
		//
		DECLAREMETHOD_0(pJoint,pJointBall*,castBall)

		DECLAREMETHOD_1(pJointBall,void,setAnchor,VxVector)
		
		DECLAREMETHOD_1(pJointBall,void,setSwingLimitAxis,VxVector)
		DECLAREMETHOD_1(pJointBall,bool,setSwingLimit,pJointLimit)
		DECLAREMETHOD_1(pJointBall,bool,setTwistHighLimit,pJointLimit)
		DECLAREMETHOD_1(pJointBall,bool,setTwistLowLimit,pJointLimit)

		
		DECLAREMETHOD_0(pJointBall,pJointLimit,getSwingLimit)
		DECLAREMETHOD_0(pJointBall,pJointLimit,getTwistHighLimit)
		DECLAREMETHOD_0(pJointBall,pJointLimit,getTwistLowLimit)
		
		DECLAREMETHOD_1(pJointBall,bool,setSwingSpring,pSpring)
		DECLAREMETHOD_1(pJointBall,bool,setTwistSpring,pSpring)
		DECLAREMETHOD_1(pJointBall,void,setJointSpring,pSpring)

		DECLAREMETHOD_0(pJointBall,pSpring,getSwingSpring)
		DECLAREMETHOD_0(pJointBall,pSpring,getTwistSpring)
		DECLAREMETHOD_0(pJointBall,pSpring,getJointSpring)
		
		DECLAREMETHOD_1(pJointBall,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Prismatic
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPrismatic*,castPrismatic)

		DECLAREMETHOD_1(pJointPrismatic,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPrismatic,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPrismatic,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Cylindrical
		//
		//
		DECLAREMETHOD_0(pJoint,pJointCylindrical*,castCylindrical)

		DECLAREMETHOD_1(pJointCylindrical,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointCylindrical,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointCylindrical,void,enableCollision,int)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Point In Plane
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPointInPlane*,castPointInPlane)

		DECLAREMETHOD_1(pJointPointInPlane,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPointInPlane,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPointInPlane,void,enableCollision,bool)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Point In Plane
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPointOnLine*,castPointOnLine)

		DECLAREMETHOD_1(pJointPointOnLine,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPointOnLine,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPointOnLine,void,enableCollision,bool)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT BASE 
		//
		//
		DECLAREMETHOD_1(pJoint,void,setLocalAnchor0,VxVector)

		DECLAREMETHOD_2(pJoint,void,setBreakForces,float,float)
		DECLAREMETHOD_2(pJoint,void,getBreakForces,float&,float&)
		DECLAREMETHOD_3(pJoint,int,addLimitPlane,VxVector,VxVector,float)
		DECLAREMETHOD_2_WITH_DEF_VALS(pJoint,void,setLimitPoint,VxVector,NODEFAULT,bool,true)
		DECLAREMETHOD_0(pJoint,void,purgeLimitPlanes)
		DECLAREMETHOD_0(pJoint,void,resetLimitPlaneIterator)
		DECLAREMETHOD_0(pJoint,int,hasMoreLimitPlanes)
		DECLAREMETHOD_3(pJoint,int,getNextLimitPlane,VxVector&,float&,float&)
		DECLAREMETHOD_0(pJoint,int,getType)
		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT :: DISTANCE
		//
		DECLAREMETHOD_0(pJoint,pJointDistance*,castDistanceJoint)

		DECLAREMETHOD_1(pJointDistance,void,setMinDistance,float)
		DECLAREMETHOD_1(pJointDistance,void,setMaxDistance,float)
		DECLAREMETHOD_1(pJointDistance,void,setLocalAnchor0,VxVector)
		DECLAREMETHOD_1(pJointDistance,void,setLocalAnchor1,VxVector)
		DECLAREMETHOD_1(pJointDistance,void,setSpring,pSpring)
		DECLAREMETHOD_0(pJointDistance,float,getMinDistance)
		DECLAREMETHOD_0(pJointDistance,float,getMaxDistance)
		DECLAREMETHOD_0(pJointDistance,float,getLocalAnchor0)
		DECLAREMETHOD_0(pJointDistance,float,getLocalAnchor1)
		DECLAREMETHOD_0(pJointDistance,pSpring,getSpring)
		DECLAREMETHOD_1(pJointDistance,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT PULLEY
		//

		DECLAREMETHOD_0(pJoint,pJointPulley*,castPulley)

		DECLAREMETHOD_1(pJointPulley,void,setLocalAnchorA,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setLocalAnchorB,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setPulleyA,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setPulleyB,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setStiffness,float)
		DECLAREMETHOD_1(pJointPulley,void,setRatio,float)
		DECLAREMETHOD_1(pJointPulley,void,setRigid,int)
		DECLAREMETHOD_1(pJointPulley,void,setDistance,float)
		DECLAREMETHOD_1(pJointPulley,void,setMotor,pMotor)
		DECLAREMETHOD_0(pJointPulley,VxVector,getLocalAnchorA)
		DECLAREMETHOD_0(pJointPulley,VxVector,getLocalAnchorB)
		DECLAREMETHOD_0(pJointPulley,VxVector,getPulleyA)
		DECLAREMETHOD_0(pJointPulley,VxVector,getPulleyB)
		DECLAREMETHOD_0(pJointPulley,float,getStiffness)

		DECLAREMETHOD_0(pJointPulley,float,getRatio)
		DECLAREMETHOD_0(pJointPulley,float,getDistance)
		DECLAREMETHOD_1(pJointPulley,void,enableCollision,bool)
		DECLAREMETHOD_0(pJointPulley,pMotor,getMotor)

		//////////////////////////////////////////////////////////////////////////
		//
		//			JOINT D6
		//

		DECLAREMETHOD_0(pJoint,pJointD6*,castD6Joint)

		DECLAREMETHOD_1(pJointD6,void,setTwistMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setSwing1MotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setSwing2MotionMode,D6MotionMode)
		
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getXMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getYMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getZMotion)


		DECLAREMETHOD_1(pJointD6,void,setXMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setYMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setZMotionMode,D6MotionMode)

		DECLAREMETHOD_0(pJointD6,D6MotionMode,getXMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getYMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getZMotion)

		//////////////////////////////////////////////////////////////////////////
		//softwLimits
		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getLinearLimit)
		DECLAREMETHOD_1(pJointD6,int,setLinearLimit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getSwing1Limit)
		DECLAREMETHOD_1(pJointD6,int,setSwing1Limit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getSwing2Limit)
		DECLAREMETHOD_1(pJointD6,int,setSwing2Limit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getTwistHighLimit)
		DECLAREMETHOD_1(pJointD6,int,setTwistHighLimit,pJD6SoftLimit)
		
		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getTwistLowLimit)
		DECLAREMETHOD_1(pJointD6,int,setTwistLowLimit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getXDrive)
		DECLAREMETHOD_1(pJointD6,int,setXDrive,pJD6Drive)
		
		DECLAREMETHOD_0(pJointD6,pJD6Drive,getYDrive)
		DECLAREMETHOD_1(pJointD6,int,setYDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getZDrive)
		DECLAREMETHOD_1(pJointD6,int,setZDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getSwingDrive)
		DECLAREMETHOD_1(pJointD6,int,setSwingDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getTwistDrive)
		DECLAREMETHOD_1(pJointD6,int,setTwistDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getSlerpDrive)
		DECLAREMETHOD_1(pJointD6,int,setSlerpDrive,pJD6Drive)

		DECLAREMETHOD_1(pJointD6,void,setDrivePosition,VxVector)
		DECLAREMETHOD_1(pJointD6,void,setDriveRotation,VxQuaternion)
		
		DECLAREMETHOD_1(pJointD6,void,setDriveLinearVelocity,VxVector)
		DECLAREMETHOD_1(pJointD6,void,setDriveAngularVelocity,VxVector)
		
		DECLAREMETHOD_1(pJointD6,void,enableCollision,bool)




		//////////////////////////////////////////////////////////////////////////
		//
		//				Rigid Body Exports 
		//
		//


		/************************************************************************/
		/* Forces                                                                     */
		/************************************************************************/
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addForce,const VxVector&,NODEFAULT, PForceMode, 0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addTorque,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addLocalForce,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addLocalTorque,const VxVector&,NODEFAULT, PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addForceAtPos,  const VxVector&,NODEFAULT,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addForceAtLocalPos,const VxVector,NODEFAULT, const VxVector&, NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addLocalForceAtPos, const VxVector&, NODEFAULT,const VxVector&,NODEFAULT, PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addLocalForceAtLocalPos, const VxVector&, NODEFAULT,const VxVector&, NODEFAULT,PForceMode,0,bool,true)


		/************************************************************************/
		/* Momentum                                                                     */
		/************************************************************************/
		DECLAREMETHOD_1(pRigidBody,void,setAngularMomentum,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setLinearMomentum,const VxVector&)
		DECLAREMETHOD_0(pRigidBody,VxVector,getAngularMomentum)
		DECLAREMETHOD_0(pRigidBody,VxVector,getLinearMomentum)
		
		/************************************************************************/
		/* Pose :                                                                     */
		/************************************************************************/
		DECLAREMETHOD_1(pRigidBody,void,setPosition,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setRotation,const VxQuaternion&)
		DECLAREMETHOD_1(pRigidBody,void,translateLocalShapePosition,VxVector)

		/************************************************************************/
		/* Velocity :                                                                     */
		/************************************************************************/

		DECLAREMETHOD_1(pRigidBody,void,setLinearVelocity,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setAngularVelocity,const VxVector&)
		DECLAREMETHOD_0(pRigidBody,float,getMaxAngularSpeed)
		DECLAREMETHOD_0(pRigidBody,VxVector,getLinearVelocity)
		DECLAREMETHOD_0(pRigidBody,VxVector,getAngularVelocity)
		
		/************************************************************************/
		/* Mass                                                                     */
		/************************************************************************/
		
		DECLAREMETHOD_1(pRigidBody,void,setMassOffset,VxVector)
		DECLAREMETHOD_1(pRigidBody,void,setMaxAngularSpeed,float)

		/************************************************************************/
		/* Hull  */
		/************************************************************************/
		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setBoxDimensions,const VxVector&,NODEFAULT,CKBeObject*,NULL)


		DECLAREMETHOD_0(pRigidBody,pWorld*,getWorld)
		DECLAREMETHOD_1(pRigidBody,pJoint*,isConnected,CK3dEntity*)
		//DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setBoxDimensions,const VxVector&,NODEFAULT,CKBeObject*,NULL)
		DECLAREMETHOD_1_WITH_DEF_VALS(pRigidBody,float,getMass,CK3dEntity*,NULL)
		DECLAREMETHOD_0(pRigidBody,int,getHullType)

		DECLAREMETHOD_2(pRigidBody,void,setGroupsMask,CK3dEntity*,const pGroupsMask&)

		DECLAREMETHOD_0(pRigidBody,int,getFlags)
		DECLAREMETHOD_1(pRigidBody,VxVector,getPointVelocity,VxVector)
		DECLAREMETHOD_1(pRigidBody,VxVector,getLocalPointVelocity,VxVector)

		DECLAREMETHOD_1(pRigidBody,void,enableCollision,bool)
		DECLAREMETHOD_0(pRigidBody,bool,isCollisionEnabled)

		DECLAREMETHOD_1(pRigidBody,void,setKinematic,int)
		DECLAREMETHOD_0(pRigidBody,int,isKinematic)

		DECLAREMETHOD_1(pRigidBody,void,enableGravity,int)
		DECLAREMETHOD_0(pRigidBody,bool,isAffectedByGravity)

		DECLAREMETHOD_1(pRigidBody,void,setSleeping,int)
		DECLAREMETHOD_0(pRigidBody,int,isSleeping)
		DECLAREMETHOD_1(pRigidBody,void,setLinearDamping,float)
		DECLAREMETHOD_1(pRigidBody,void,setAngularDamping,float)
		DECLAREMETHOD_1(pRigidBody,void,lockTransformation,BodyLockFlags)
		DECLAREMETHOD_1(pRigidBody,int,isBodyFlagOn,int)
		DECLAREMETHOD_1(pRigidBody,void,setSolverIterationCount,int)



		DECLAREMETHOD_1(pRigidBody,void,setCollisionsGroup,int)
		DECLAREMETHOD_0(pRigidBody,int,getCollisionsGroup)
		DECLAREMETHOD_2(pRigidBody,int,updateMassFromShapes,float,float)
		DECLAREMETHOD_5_WITH_DEF_VALS(pRigidBody,int,addSubShape,CKMesh*,NULL,pObjectDescr,NODEFAULT,CK3dEntity*,NULL,VxVector,VxVector(),VxQuaternion,VxQuaternion())
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,int,removeSubShape,CKMesh*,NODEFAULT,float,0.0,float,0.0)




		/*
		DECLAREENUM("WORLD_UPDATE_MODE")
		DECLAREENUMVALUE("WORLD_UPDATE_MODE", "WUM_UPDATE_FROM_ATTRIBUTE" , 0x0001)

		DECLAREENUM("WORLD_UPDATE_FLAGS")
		DECLAREENUMVALUE("WORLD_UPDATE_FLAGS", "WUF_WORLD_SETTINGS" , 0x0001)
		DECLAREENUMVALUE("WORLD_UPDATE_FLAGS", "WUF_DAMPING_PARAMETER" , 0x0002)
		DECLAREENUMVALUE("WORLD_UPDATE_FLAGS", "WUF_SLEEPING_PARAMETER" , 0x0004)
		DECLAREENUMVALUE("WORLD_UPDATE_FLAGS", "WUF_SURFACE_SETTINGS" , 0x0008)
		DECLAREENUMVALUE("WORLD_UPDATE_FLAGS", "WUF_ALL_PARAMETERS" , 0x0010)

		DECLAREENUM("BODY_UPDATE_FLAGS")

		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_PHY_PARAMETER" , 0x0001)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_DAMPING_PARAMETER" , 0x0002)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_SLEEPING_PARAMETER" , 0x0004)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_JOINT_PARAMETERS" , 0x0008)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_ALL_PARAMETERS" , 0x0010)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_GEOMETRY" , 0x0020)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_PIVOT" , 0x0040)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_MASS" , 0x0080)
		DECLAREENUMVALUE("BODY_UPDATE_FLAGS", "BUF_ALL" , 0x0100)


		


		DECLAREENUM("JType")
		DECLAREENUMVALUE("JType", "JT_NONE" , 0)
		DECLAREENUMVALUE("JType", "JT_BALL" , 1)
		DECLAREENUMVALUE("JType", "JT_HINGE" , 2)
		DECLAREENUMVALUE("JType", "JT_SLIDER" , 3)
		DECLAREENUMVALUE("JType", "JT_CONTACT" , 4)
		DECLAREENUMVALUE("JType", "JT_UNIVERSAL" , 5)
		DECLAREENUMVALUE("JType", "JT_HINGE2" , 6)
		DECLAREENUMVALUE("JType", "JT_FIXED" , 7)
		DECLAREENUMVALUE("JType", "JT_MOTOR" ,8 )



		
        		

		DECLAREENUM("J_LIMITPARAMETER");
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JLoStop" , 0);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JHiStop", 1);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JVel"  , 2);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JFMax"  , 3);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JFudgeFactor"  , 4);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JBounce"  , 5);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JCFM"  , 6);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JStopERP"  , 7);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JStopCFM"  , 8);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JSuspensionERP"  , 9);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JSuspensionCFM"  , 10);
		DECLAREENUMVALUE("J_LIMITPARAMETER", "JERP"  , 11);

		DECLAREENUM("J_MOTOR_AXIS_TYPE")
		DECLAREENUMVALUE("J_MOTOR_AXIS_TYPE", "AXIS_GLOBAL_FRAME" , 0)
		DECLAREENUMVALUE("J_MOTOR_AXIS_TYPE", "AXIS_FIRST_BODY" , 1)
		DECLAREENUMVALUE("J_MOTOR_AXIS_TYPE", "AXIS_SECOND_BODY" , 2)
*/

/*
		DECLAREOBJECTTYPE(pSleepingSettings)
		DECLARECTOR_0(__newvtSleepingSettings)
	
		DECLAREMETHODC_0(pSleepingSettings,int,SleepSteps)
		DECLAREMETHOD_1(pSleepingSettings,void,SleepSteps,int)
		DECLAREMETHODC_0(pSleepingSettings,float,AngularThresold)
		DECLAREMETHOD_1(pSleepingSettings,void,AngularThresold,float)
		DECLAREMETHODC_0(pSleepingSettings,float,LinearThresold)
		DECLAREMETHOD_1(pSleepingSettings,void,LinearThresold,float)
		DECLAREMETHODC_0(pSleepingSettings,int,AutoSleepFlag)
		DECLAREMETHOD_1(pSleepingSettings,void,AutoSleepFlag,int)

	
		DECLAREOBJECTTYPE(pWorldSettings)
		DECLARECTOR_0(__newvtWorldSettings)
		DECLAREMETHODC_0(pWorldSettings,VxVector,Gravity)
		DECLAREMETHOD_1(pWorldSettings,void,Gravity,VxVector)
		DECLAREMETHODC_0(pWorldSettings,float,ContactSurfaceLayer)
		DECLAREMETHOD_1(pWorldSettings,void,ContactSurfaceLayer,float)
		DECLAREMETHODC_0(pWorldSettings,float,ERP)
		DECLAREMETHOD_1(pWorldSettings,void,ERP,float)
		DECLAREMETHODC_0(pWorldSettings,float,CFM)
		DECLAREMETHOD_1(pWorldSettings,void,CFM,float)
		DECLAREMETHODC_0(pWorldSettings,float,MaximumContactCorrectVelocity)
		DECLAREMETHOD_1(pWorldSettings,void,MaximumContactCorrectVelocity,float)

		DECLAREOBJECTTYPE(pJointSettings)
		DECLARECTOR_0(__newvtJointSettings)






		DECLAREFUN_C_1(int,TestWS,pWorldSettings)
			
		
		DECLAREMETHODC_0(pWorld,pSleepingSettings*,SleepingSettings)
		DECLAREMETHOD_1(pWorld,void,SleepingSettings,pSleepingSettings*)
		
		DECLAREMETHODC_0(pWorld,pWorldSettings*,WorldSettings)
		DECLAREMETHOD_1(pWorld,void,WorldSettings,pWorldSettings*)
		DECLAREMETHOD_0(pWorld,int,NumJoints)
*/
		//////////////////////////////////////////////////////////////////////////
		//collision

/*
		DECLAREMETHOD_0(pRigidBody,float,GetFriction)
		DECLAREMETHOD_1(pRigidBody,void,SetFriction,float)
		DECLAREMETHOD_1(pRigidBody,void,SetPosition,VxVector)
		DECLAREMETHOD_1(pRigidBody,void,SetQuaternion,  VxQuaternion)
		DECLAREMETHOD_0(pRigidBody,VxBbox,GetAABB)

		
		DECLAREMETHOD_0(pRigidBody,float,GetLastHFHeight)
		DECLAREMETHOD_0(pRigidBody,int,GetLastHFColor)
		DECLAREMETHOD_0(pRigidBody,Vx2DVector,GetLastHCoord)
*/

		// Velocity :
		//DECLAREMETHOD_1(pRigidBody,void,setLinearVel,VxVector)
		//DECLAREMETHOD_1(pRigidBody,void,setAngularVel,  VxVector)

		/*
		//DECLAREMETHOD_0(pRigidBody,VxVector,GetLinearVel)
		//DECLAREMETHOD_0(pRigidBody,VxVector,GetAngularVel)
		// Forces :	
		*/
		
		//DECLAREMETHOD_2(pRigidBody,void,addForce,  VxVector , PForceMode )
		




		


		//////////////////////////////////////////////////////////////////////////
		//rotation axis   : 

		/*




		DECLAREMETHOD_2(pFactory,pWorldSettings*,CreateWorldSettings,const char *,const char*)
		DECLAREMETHOD_2_WITH_DEF_VALS(pFactory,pWorldSettings*,CreateWorldSettings,const char *,"Default",const char*,"PhysicDefaults.xml")
		
		DECLAREMETHOD_2(pFactory,pSleepingSettings*,CreateSleepingSettings,const char *,const char*)

		//////////////////////////////////////////////////////////////////////////
		//world : 
		DECLAREMETHOD_3(pFactory,pWorld*,CreateWorld,CK3dEntity*,pWorldSettings*,pSleepingSettings*)
		

		//////////////////////////////////////////////////////////////////////////
		//bodies :
		DECLAREMETHOD_3(pFactory,pRigidBody*,CreateRigidBody,CK3dEntity*,pWorld*,pSleepingSettings*)
		DECLAREMETHOD_3(pFactory,pRigidBody*,CreateRigidBody,CK3dEntity*,CK3dEntity*,pSleepingSettings*)

		DECLAREMETHOD_3_WITH_DEF_VALS(pFactory,pRigidBody*,CreateBall,CK3dEntity*,NULL,CK3dEntity*,NULL,pSleepingSettings*,NULL)
		DECLAREMETHOD_3(pFactory,pRigidBody*,CreateRigidBodyFull,CK3dEntity*,pWorld*,pSleepingSettings*)
		DECLAREMETHOD_8_WITH_DEF_VALS(pFactory,pRigidBody*,CreateBody,CK3dEntity*,NULL,CK3dEntity*,NULL,pSleepingSettings*,NULL,int,(BodyFlags)(BF_MOVING|BF_P2V|BF_WORLD_GRAVITY|BF_ENABLED|BF_COLLISION),int,HT_BOX,float,1.0f,float,0.0f,float,1.0f)


		//////////////////////////////////////////////////////////////////////////
		//joint

		DECLAREMETHOD_2(pRigidBody,pJoint*,IsConnected, CK3dEntity*,int)

		//////////////////////////////////////////////////////////////////////////
		//collision :  

		DECLAREMETHOD_4(pWorld,CK3dEntity*,CIsInCollision,CK3dEntity*,VxVector&,VxVector&,float&)
		DECLAREMETHOD_5(pWorld,CK3dEntity*,CIsInCollision,CK3dEntity*,CKGroup*,VxVector&,VxVector&,float&)
		DECLAREMETHOD_5(pWorld,bool,CIsInCollision,CK3dEntity*,CK3dEntity*,VxVector&,VxVector&,float&)
		DECLAREMETHOD_8(pWorld,CK3dEntity*,CRayCollision,VxVector ,CK3dEntity*,VxVector,CK3dEntity*,float,bool,VxVector&, VxVector&)
		DECLAREMETHOD_2(pWorld,int,CIsInCollision,CK3dEntity*,CK3dEntity*)

		//	DECLAREMETHOD_7(PhysicManager,int,CTestRayCollision,CKGroup*,VxVector,VxVector,float,VxVector*,VxVector*,CK3dEntity**)

		

	

*/

		

		/*

		DECLAREMETHODC_0(PhysicManager,pWorldSettings,DefaultWorldSettings)
		DECLAREMETHOD_1(PhysicManager,void,DefaultWorldSettings,pWorldSettings)

		DECLAREMETHODC_0(PhysicManager,pSleepingSettings,DefaultSleepingSettings)
		DECLAREMETHOD_1(PhysicManager,void,DefaultSleepingSettings,pSleepingSettings)

		DECLAREMETHOD_0(PhysicManager,void,CheckWorlds)
		DECLARESTATIC_0(pWorld,pWorld*,GetDefault)
		*/



	STOPVSLBIND
	

}


PhysicManager*GetPhysicManager()
{
	return GetPMan();
}

/*
void __newvtWorldSettings(BYTE *iAdd) 
{
new (iAdd) pWorldSettings();
}
void __newvtSleepingSettings(BYTE *iAdd) 
{
new (iAdd) pSleepingSettings();
}
void __newvtJointSettings(BYTE *iAdd) 
{
new (iAdd) pJointSettings();
}


int TestWS(pWorldSettings pWS)
{
VxVector grav = pWS.Gravity();
return 2;
}

pFactory* GetPFactory();


pFactory* GetPFactory()
{
return pFactory::Instance();
}


extern pRigidBody*getBody(CK3dEntity*ent);
*/