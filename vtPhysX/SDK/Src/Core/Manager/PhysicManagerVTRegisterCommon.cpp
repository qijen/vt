#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"


#include "pVehicleAll.h"



PhysicManager*GetPhysicManager();
pRigidBody *getBody(CK3dEntity*ent);
CKGUID GetPhysicManagerGUID() { 	return GUID_MODULE_MANAGER;}

typedef ForceMode PForceMode;
typedef D6MotionMode PJMotion;
typedef D6DriveType PDriveType;


pFactory* getPFactory()
{
	return pFactory::Instance();
}


void __newpObjectDescr(BYTE *iAdd) 
{
	new (iAdd) pObjectDescr();
}


void __newpRaycastHit(BYTE *iAdd)
{
	new(iAdd)pRaycastHit();
}

void __newpGroupsMask(BYTE *iAdd)
{
	new(iAdd)pGroupsMask();
}

void __newpOptimization(BYTE *iAdd)
{
	new(iAdd)pOptimization();
}
void __newpMassSettings(BYTE *iAdd)
{
	new(iAdd)pMassSettings();
}
void __newpCCD(BYTE *iAdd)
{
	new(iAdd)pCCDSettings();
}
void __newpCollisionSettings(BYTE *iAdd)
{
	new(iAdd)pCollisionSettings();
}

void __newpPivotSettings(BYTE *iAdd)
{
	new(iAdd)pPivotSettings();
}

void __newpMaterial(BYTE *iAdd)
{
	new(iAdd)pMaterial();
}
void __newpAxisReferenceLength(BYTE *iAdd)
{
	new(iAdd)pAxisReferencedLength();
}


void __newpCapsuleSettingsEx(BYTE *iAdd)
{
	new(iAdd)pCapsuleSettingsEx();
}
void __newpConvexCylinder(BYTE *iAdd)
{
	new(iAdd)pConvexCylinderSettings();
}

void __newpWheelDescr(BYTE *iAdd)
{
	new (iAdd)pWheelDescr();
}

void __newpTireFunction(BYTE *iAdd)
{
	new (iAdd)pTireFunction();
}


void __newpInterpolation(BYTE *iAdd)
{
	new (iAdd)pLinearInterpolation();
}

CKGUID getRigidBodyParameterType()
{
	return VTS_PHYSIC_ACTOR;
}

//////////////////////////////////////////////////////////////////////////

#define TESTGUID CKGUID(0x2c5c47f6,0x1d0755d9)

void logWarning(const char*errMessage)
{
	if (!errMessage || !strlen(errMessage))
		return;

	xLogger::xLog(XL_START,ELOGWARNING,E_VSL,errMessage);


}

void PhysicManager::_RegisterVSLCommon()
{
			
	STARTVSLBIND(m_Context)

	//----------------------------------------------------------------
	//
	// manager related
	//

	{

		DECLAREENUM("pSDKParameter")
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_SkinWidth" ,1 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_DefaultSleepLinVelSquared" ,2 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_DefaultSleepAngVelSquared" ,3 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_BounceThreshold" ,4 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_DynFrictScaling" , 5)
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_StaFrictionScaling" ,6 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_MaxAngularVelocity" ,7 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ContinuousCD" ,8 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_AdaptiveForce" ,68 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_CollVetoJointed" ,69 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_TriggerTriggerCallback" ,70 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_SelectHW_Algo" ,71 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_CCDEpsilon" ,73 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_SolverConvergenceThreshold" ,74 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_BBoxNoiseLevel" ,75 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ImplicitSweepCacheSize" , 76)
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_DefaultSleepEnergy" ,77 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ConstantFluidMaxPackets" ,78 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ConstantFluidMaxParticlesPerStep" ,79 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_AsynchronousMeshCreation" ,96 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ForceFieldCustomKernelEpsilon" ,97 )
		DECLAREENUMVALUE("pSDKParameter", "pSDKP_ImprovedSpringSolver" ,98 )
	
	}


	REGISTERCONST("FLT_MAX",BIND_FLOAT,FLT_MAX)
	REGISTERCONST("PTYPE_RIGID_BODY",BIND_FLOAT,FLT_MAX)

	//----------------------------------------------------------------
	//
	// common types
	//
	DECLAREPOINTERTYPE(pFactory)
	DECLAREFUN_C_0(pFactory,getPFactory)
	
	
	
	DECLAREFUN_C_0(CKGUID, GetPhysicManagerGUID)
	DECLAREOBJECTTYPE(PhysicManager)
	DECLARESTATIC_1(PhysicManager,PhysicManager*,Cast,CKBaseManager* iM)
	DECLAREFUN_C_0(PhysicManager*, GetPhysicManager)

	DECLAREPOINTERTYPE(pRigidBody)
	DECLAREPOINTERTYPE(pPacejka)

	DECLAREPOINTERTYPEALIAS(pRigidBody,"pBody")
	
	DECLAREFUN_C_0(CKGUID,getRigidBodyParameterType)
	DECLAREFUN_C_1(void,logWarning,const char*)

	DECLAREPOINTERTYPE(pWorld)
	DECLAREPOINTERTYPE(pCloth)
	DECLAREPOINTERTYPE(pVehicle)
	DECLAREPOINTERTYPE(pWheel)
	DECLAREPOINTERTYPE(pVehicle)
	DECLAREPOINTERTYPE(pVehicleMotor)
	DECLAREPOINTERTYPE(pVehicleGears)

	DECLAREPOINTERTYPE(pWheel1)
	DECLAREPOINTERTYPE(pWheel2)

	//----------------------------------------------------------------
	//
	// new vehicle pack
	//
	DECLAREPOINTERTYPE(pEngine)
	DECLAREPOINTERTYPE(pGearBox)
	DECLAREPOINTERTYPE(pWheelContactData)

	DECLAREPOINTERTYPE(pLinearInterpolation)
	//DECLARECTOR_0(__newpInterpolation)







	DECLAREINHERITANCESIMPLE("pWheel","pWheel1")
	DECLAREINHERITANCESIMPLE("pWheel","pWheel2")

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
	//----------------------------------------------------------------
	//
	// help structures collision
	//
	
	DECLAREOBJECTTYPE(pGroupsMask)
	DECLARECTOR_0(__newpGroupsMask)
	DECLAREMEMBER(pGroupsMask,int,bits0)
	DECLAREMEMBER(pGroupsMask,int,bits1)
	DECLAREMEMBER(pGroupsMask,int,bits2)
	DECLAREMEMBER(pGroupsMask,int,bits3)


	DECLAREOBJECTTYPE(pCollisionSettings)
	DECLARECTOR_0(__newpCollisionSettings)
	DECLAREMEMBER(pCollisionSettings,int,collisionGroup)
	DECLAREMEMBER(pCollisionSettings,pGroupsMask,groupsMask)
	DECLAREMEMBER(pCollisionSettings,float,skinWidth)


	DECLAREOBJECTTYPE(pRaycastHit)
	DECLARECTOR_0(__newpRaycastHit)
	DECLAREMEMBER(pRaycastHit,float,distance)
	DECLAREMEMBER(pRaycastHit,CK3dEntity*,shape)
	DECLAREMEMBER(pRaycastHit,VxVector,worldImpact)
	DECLAREMEMBER(pRaycastHit,VxVector,worldNormal)
	DECLAREMEMBER(pRaycastHit,int,faceID)
	DECLAREMEMBER(pRaycastHit,int,internalFaceID)
	DECLAREMEMBER(pRaycastHit,float,u)
	DECLAREMEMBER(pRaycastHit,float,v)
	DECLAREMEMBER(pRaycastHit,int,materialIndex)
	DECLAREMEMBER(pRaycastHit,int,flags)

	DECLAREENUM("pFilterOp")
	DECLAREENUMVALUE("pFilterOp", "FO_And" , 0)
	DECLAREENUMVALUE("pFilterOp", "FO_Or" , 1)
	DECLAREENUMVALUE("pFilterOp", "FO_Xor" ,2)
	DECLAREENUMVALUE("pFilterOp", "FO_Nand",3 )
	DECLAREENUMVALUE("pFilterOp", "FO_Nor" , 4)
	DECLAREENUMVALUE("pFilterOp", "FO_NXor" ,5 )

	DECLAREENUM("pShapesType")
	DECLAREENUMVALUE("pShapesType", "ST_Static" , 1)
	DECLAREENUMVALUE("pShapesType", "ST_Dynamic" , 2)
	DECLAREENUMVALUE("pShapesType", "ST_All" , 3)

	DECLAREENUM("pTriggerFlags")
	DECLAREENUMVALUE("pTriggerFlags", "TF_Disable" , 8)
	DECLAREENUMVALUE("pTriggerFlags", "TF_OnEnter" , 1)
	DECLAREENUMVALUE("pTriggerFlags", "TF_OnLeave" , 2)
	DECLAREENUMVALUE("pTriggerFlags", "TF_OnStay" , 4)

	DECLAREENUM("pContactModifyMask")
	DECLAREENUMVALUE("pContactModifyMask", "CMM_None" , 0)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_MinImpulse" , 1)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_MaxImpulse" , 2)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_Error" , 4)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_Target" , 8)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_LocalPosition0" , 16)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_LocalPosition1" , 32)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_LocalOrientation0" , 64)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_LocalOrientation1" , 128)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_StaticFriction0" , 256)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_StaticFriction1" , 512)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_DynamicFriction0" , 1024)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_DynamicFriction1" , 2048)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_Restitution" , 8196)
	DECLAREENUMVALUE("pContactModifyMask", "CMM_Force32" ,2147483648 )

	DECLAREENUM("pCollisionEventMask")
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_IgnorePair" , 1)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnStartTouch" , 2)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnEndTouch" , 4)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnTouch" , 8)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnImpact" , 16)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnRoll" , 32)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnSlide" , 64)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_Forces" , 128)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnStartTouchForceThreshold" , 256)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnEndTouchForceThreshold" , 512)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_OnTouchForceThreshold" , 1024)
	DECLAREENUMVALUE("pCollisionEventMask", "CPF_ContactModification" , 65536)



	//----------------------------------------------------------------
	//
	// help structures Rigid body
	//
	
	DECLAREENUM("BodyFlags")
	DECLAREENUMVALUE("BodyFlags", "BF_Moving" , 1)
	DECLAREENUMVALUE("BodyFlags", "BF_Gravity" , 2)
	DECLAREENUMVALUE("BodyFlags", "BF_Collision" , 4)
	DECLAREENUMVALUE("BodyFlags", "BF_Kinematic" , 8)
	DECLAREENUMVALUE("BodyFlags", "BF_SubShape" , 16)
	DECLAREENUMVALUE("BodyFlags", "BF_Hierarchy" , 32)
	DECLAREENUMVALUE("BodyFlags", "BF_Attributes" , 64)
	DECLAREENUMVALUE("BodyFlags", "BF_TriggerShape" , 128)
	DECLAREENUMVALUE("BodyFlags", "BF_Sleep" , 4096)
	DECLAREENUMVALUE("BodyFlags", "BF_CollisionNotify" , 512)
	DECLAREENUMVALUE("BodyFlags", "BF_CollisionsForce" , 1024)
	DECLAREENUMVALUE("BodyFlags", "BF_ContactModify" , 2048)

	DECLAREENUM("BodyLockFlags")
	DECLAREENUMVALUE("BodyLockFlags", "BF_LPX" , 2)
	DECLAREENUMVALUE("BodyLockFlags", "BF_LPY" , 4)
	DECLAREENUMVALUE("BodyLockFlags", "BF_LPZ" , 8)
	DECLAREENUMVALUE("BodyLockFlags", "BF_LRX" , 16)
	DECLAREENUMVALUE("BodyLockFlags", "BF_LRY" , 32)
	DECLAREENUMVALUE("BodyLockFlags", "BF_LRZ" , 64)



	DECLAREENUM("HullType")
	DECLAREENUMVALUE("HullType", "HT_Sphere" , 0)
	DECLAREENUMVALUE("HullType", "HT_Box" , 1)
	DECLAREENUMVALUE("HullType", "HT_Capsule" , 2)
	DECLAREENUMVALUE("HullType", "HT_Plane" , 3)
	DECLAREENUMVALUE("HullType", "HT_Mesh" , 4)
	DECLAREENUMVALUE("HullType", "HT_ConvexMesh" , 5)
	DECLAREENUMVALUE("HullType", "HT_HeightField" , 6)
	DECLAREENUMVALUE("HullType", "HT_Wheel" , 7)
	DECLAREENUMVALUE("HullType", "HT_ConvexCylinder" , 9)

	


	DECLAREENUM("CombineMode")
	DECLAREENUMVALUE("CombineMode", "CM_Average" , 0)
	DECLAREENUMVALUE("CombineMode", "CM_Min" , 1)
	DECLAREENUMVALUE("CombineMode", "CM_Multiply" , 2)
	DECLAREENUMVALUE("CombineMode", "CM_Max" , 3)

	DECLAREENUM("pObjectDescrMask")
	DECLAREENUMVALUE("pObjectDescrMask", "OD_XML" , 1)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Pivot" , 2)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Mass" , 4)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Collision" , 8)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_CCD" , 16)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Material" , 32)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Optimization" , 64)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Capsule" , 128)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_ConvexCylinder" , 256)
	DECLAREENUMVALUE("pObjectDescrMask", "OD_Wheel" , 512)


	DECLAREENUM("DIRECTION")
	DECLAREENUMVALUE("DIRECTION", "DIR_X" , 0)
	DECLAREENUMVALUE("DIRECTION", "DIR_Y" , 1)
	DECLAREENUMVALUE("DIRECTION", "DIR_Z" , 2)


	//----------------------------------------------------------------
	//
	// Wheel related types
	//


	DECLAREOBJECTTYPE(pTireFunction)
	DECLARECTOR_0(__newpTireFunction)
	DECLAREMEMBER(pTireFunction,float,extremumSlip)
	DECLAREMEMBER(pTireFunction,float,extremumValue)
	DECLAREMEMBER(pTireFunction,float,asymptoteSlip)
	DECLAREMEMBER(pTireFunction,float,stiffnessFactor)
	DECLAREMEMBER(pTireFunction,int,xmlLink)
	DECLAREMETHOD_0(pTireFunction,void,setToDefault)



	
	
	DECLAREENUM("WheelFlags")
	DECLAREENUMVALUE("WheelFlags", "WF_SteerableInput" , 1)
	DECLAREENUMVALUE("WheelFlags", "WF_SteerableAuto" , 2)
	DECLAREENUMVALUE("WheelFlags", "WF_AffectedByHandbrake" , 4)
	DECLAREENUMVALUE("WheelFlags", "WF_Accelerated" , 8)
	DECLAREENUMVALUE("WheelFlags", "WF_BuildLowerHalf" , 256)
	DECLAREENUMVALUE("WheelFlags", "WF_UseWheelShape" , 512)
	DECLAREENUMVALUE("WheelFlags", "WF_VehicleControlled" , 1024)


	DECLAREENUM("WheelShapeFlags")
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_WheelAxisContactNormal" , 1)
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_InputLatSlipVelocity" , 1)
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_InputLongSlipVelocity" , 2)
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_UnscaledSpringBehavior" , 4)
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_EmulateLegacyWheel" , 8)
	DECLAREENUMVALUE("WheelShapeFlags", "WSF_ClampedFriction" , 16)

	
	
	DECLAREENUM("pConvexFlags")
	DECLAREENUMVALUE("pConvexFlags", "CF_FlipNormals" , CF_FlipNormals)
	DECLAREENUMVALUE("pConvexFlags", "CF_16BitIndices" , CF_16BitIndices)
	DECLAREENUMVALUE("pConvexFlags", "CF_ComputeConvex" , CF_ComputeConvex)
	DECLAREENUMVALUE("pConvexFlags", "CF_InflateConvex" , CF_InflateConvex)
	DECLAREENUMVALUE("pConvexFlags", "CF_UncompressedNormals" , CF_UncompressedNormals)



	DECLAREOBJECTTYPE(pAxisReferencedLength)
	DECLARECTOR_0(__newpAxisReferenceLength)
	DECLAREMEMBER(pAxisReferencedLength,float,value)
	DECLAREMEMBER(pAxisReferencedLength,CKBeObject*,reference)
	DECLAREMEMBER(pAxisReferencedLength,int,referenceAxis)
	DECLAREMETHOD_0(pAxisReferencedLength,bool,isValid)
	DECLAREMETHOD_0(pAxisReferencedLength,void,setToDefault)

	DECLAREOBJECTTYPE(pWheelDescr)
	DECLARECTOR_0(__newpWheelDescr)
	DECLAREMEMBER(pWheelDescr,float,springRestitution)
	DECLAREMEMBER(pWheelDescr,pAxisReferencedLength,radius)
	DECLAREMEMBER(pWheelDescr,float,wheelSuspension)
	DECLAREMEMBER(pWheelDescr,float,springDamping)
	DECLAREMEMBER(pWheelDescr,float,springDamping)
	DECLAREMEMBER(pWheelDescr,float,maxBrakeForce)
	DECLAREMEMBER(pWheelDescr,float,frictionToFront)
	DECLAREMEMBER(pWheelDescr,float,frictionToSide)
	DECLAREMEMBER(pWheelDescr,float,inverseWheelMass)
	DECLAREMEMBER(pWheelDescr,WheelFlags,wheelFlags)
	DECLAREMEMBER(pWheelDescr,WheelShapeFlags,wheelShapeFlags)
	DECLAREMEMBER(pWheelDescr,pTireFunction,latFunc)
	DECLAREMEMBER(pWheelDescr,pTireFunction,longFunc)
	DECLAREMETHOD_0(pWheelDescr,void,setToDefault)
	DECLAREMETHOD_0(pWheelDescr,bool,isValid)






	



	DECLAREOBJECTTYPE(pConvexCylinderSettings)
	DECLARECTOR_0(__newpCollisionSettings)
	DECLAREMEMBER(pConvexCylinderSettings,pAxisReferencedLength,radius)
	DECLAREMEMBER(pConvexCylinderSettings,pAxisReferencedLength,height)
	DECLAREMEMBER(pConvexCylinderSettings,int,approximation)
	DECLAREMEMBER(pConvexCylinderSettings,VxVector,forwardAxis)
	DECLAREMEMBER(pConvexCylinderSettings,CK3dEntity*,forwardAxisRef)
	DECLAREMEMBER(pConvexCylinderSettings,VxVector,downAxis)
	DECLAREMEMBER(pConvexCylinderSettings,CK3dEntity*,downAxisRef)
	DECLAREMEMBER(pConvexCylinderSettings,VxVector,rightAxis)
	DECLAREMEMBER(pConvexCylinderSettings,CK3dEntity*,rightAxisRef)
	DECLAREMEMBER(pConvexCylinderSettings,bool,buildLowerHalfOnly)
	DECLAREMEMBER(pConvexCylinderSettings,pConvexFlags,convexFlags)
	DECLAREMETHOD_0(pConvexCylinderSettings,bool,isValid)
	DECLAREMETHOD_0(pConvexCylinderSettings,void,setToDefault)

	DECLAREOBJECTTYPE(pCapsuleSettingsEx)
	DECLARECTOR_0(__newpCapsuleSettingsEx)
	DECLAREMEMBER(pCapsuleSettingsEx,pAxisReferencedLength,radius)
	DECLAREMEMBER(pCapsuleSettingsEx,pAxisReferencedLength,height)


	DECLAREOBJECTTYPE(pMaterial)
	DECLARECTOR_0(__newpMaterial)
	DECLAREMEMBER(pMaterial,int,flags)
	DECLAREMEMBER(pMaterial,float,dynamicFriction)
	DECLAREMEMBER(pMaterial,float,staticFriction)
	DECLAREMEMBER(pMaterial,float,restitution)
	DECLAREMEMBER(pMaterial,float,dynamicFrictionV)
	DECLAREMEMBER(pMaterial,float,staticFrictionV)
	DECLAREMEMBER(pMaterial,CombineMode,frictionCombineMode)
	DECLAREMEMBER(pMaterial,CombineMode,restitutionCombineMode)
	DECLAREMEMBER(pMaterial,VxVector,dirOfAnisotropy)
	DECLAREMEMBER(pMaterial,int,xmlLinkID)


	DECLAREOBJECTTYPE(pOptimization)
	DECLARECTOR_0(__newpOptimization)
	DECLAREMEMBER(pOptimization,BodyLockFlags,transformationFlags)
	DECLAREMEMBER(pOptimization,float,linDamping)
	DECLAREMEMBER(pOptimization,float,angDamping)
	DECLAREMEMBER(pOptimization,float,linSleepVelocity)
	DECLAREMEMBER(pOptimization,float,angSleepVelocity)
	DECLAREMEMBER(pOptimization,float,sleepEnergyThreshold)
	DECLAREMEMBER(pOptimization,int,dominanceGroup)
	DECLAREMEMBER(pOptimization,int,solverIterations)
	DECLAREMEMBER(pOptimization,int,compartmentGroup)

	DECLAREOBJECTTYPE(pMassSettings)
	DECLARECTOR_0(__newpMassSettings)
	DECLAREMEMBER(pMassSettings,float,newDensity)
	DECLAREMEMBER(pMassSettings,float,totalMass)
	DECLAREMEMBER(pMassSettings,VxVector,localPosition)
	DECLAREMEMBER(pMassSettings,VxVector,localOrientation)
	DECLAREMEMBER(pMassSettings,CK_ID,massReference)

	DECLAREOBJECTTYPE(pPivotSettings)
	DECLARECTOR_0(__newpPivotSettings)
	DECLAREMEMBER(pPivotSettings,VxVector,localPosition)
	DECLAREMEMBER(pPivotSettings,VxVector,localOrientation)
	DECLAREMEMBER(pPivotSettings,CK_ID,pivotReference)


	DECLAREOBJECTTYPE(pCCDSettings)
	DECLARECTOR_0(__newpCCD)
	DECLAREMEMBER(pCCDSettings,float,motionThresold)
	DECLAREMEMBER(pCCDSettings,int,flags)
	DECLAREMEMBER(pCCDSettings,CK_ID,meshReference)
	DECLAREMEMBER(pCCDSettings,float,scale)



	DECLAREOBJECTTYPE(pObjectDescr)
	DECLARECTOR_0(__newpObjectDescr)
	DECLAREMEMBER(pObjectDescr,HullType,hullType)
	DECLAREMEMBER(pObjectDescr,float,density)
	DECLAREMEMBER(pObjectDescr,BodyFlags,flags)
	DECLAREMEMBER(pObjectDescr,VxVector,massOffset)
	DECLAREMEMBER(pObjectDescr,VxVector,shapeOffset)
	DECLAREMEMBER(pObjectDescr,float,skinWidth)
	DECLAREMEMBER(pObjectDescr,float,newDensity)
	DECLAREMEMBER(pObjectDescr,float,totalMass)
	DECLAREMEMBER(pObjectDescr,int,collisionGroup)
	DECLAREMEMBER(pObjectDescr,int,hirarchy)

	DECLAREMEMBER(pObjectDescr,int,mask)
	
	

	DECLAREMEMBER(pObjectDescr,pCollisionSettings,collision)
		DECLAREMETHOD_0(pObjectDescr,pCollisionSettings&,getCollision)
		DECLAREMETHOD_1(pObjectDescr,void,setCollision,pCollisionSettings)
	
	DECLAREMEMBER(pObjectDescr,pMaterial,material)
		DECLAREMETHOD_0(pObjectDescr,pMaterial&,getMaterial)
		DECLAREMETHOD_1(pObjectDescr,void,setMaterial,pMaterial)
	
	DECLAREMEMBER(pObjectDescr,pMassSettings,mass)
		DECLAREMETHOD_0(pObjectDescr,pMassSettings&,getMass)
		DECLAREMETHOD_1(pObjectDescr,void,setMass,pMassSettings)
	
	DECLAREMEMBER(pObjectDescr,pCCDSettings,ccd)
		DECLAREMETHOD_0(pObjectDescr,pCCDSettings&,getCollision)
		DECLAREMETHOD_1(pObjectDescr,void,setCollision,pCCDSettings)

	DECLAREMEMBER(pObjectDescr,pOptimization,optimization)
		DECLAREMETHOD_0(pObjectDescr,pOptimization&,getOptimization)
		DECLAREMETHOD_1(pObjectDescr,void,setOptimization,pOptimization)
	
	DECLAREMEMBER(pObjectDescr,pPivotSettings,pivot)
		DECLAREMETHOD_0(pObjectDescr,pPivotSettings&,getPivot)
		DECLAREMETHOD_1(pObjectDescr,void,setPivot,pPivotSettings)
	
	DECLAREMEMBER(pObjectDescr,pCapsuleSettingsEx,capsule)
		DECLAREMETHOD_0(pObjectDescr,pCapsuleSettingsEx&,getCapsule)
		DECLAREMETHOD_1(pObjectDescr,void,setCapsule,pCapsuleSettingsEx)
	
	DECLAREMEMBER(pObjectDescr,pConvexCylinderSettings,convexCylinder)
		DECLAREMETHOD_0(pObjectDescr,pConvexCylinderSettings&,getConvexCylinder)
		DECLAREMETHOD_1(pObjectDescr,void,setConvexCylinder,pConvexCylinderSettings)


	DECLAREMETHOD_1(CK3dEntity,CKParameterOut*,GetAttributeParameter,int)


		//#define REGISTERVSLGUID(iGUID, iClassName)		VSLM->RegisterGUID(iGUID, iClassName);





	







	STOPVSLBIND
	

}


PhysicManager*GetPhysicManager()
{
	return GetPMan();
}
