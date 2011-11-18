#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

//#include "pVehicle.h"




PhysicManager *ourMan = NULL;

CKGUID GetPhysicManagerGUID() { 	return GUID_MODULE_MANAGER;}

void __newpVehicleDescr(BYTE *iAdd)
{

		new (iAdd)pVehicleDesc();
}

void __newpVehicleMotorDesc(BYTE *iAdd)
{
	new (iAdd)pVehicleMotorDesc();
}

void __newpVehicleGearDesc(BYTE *iAdd)
{
	new(iAdd)pVehicleGearDesc();
}

//////////////////////////////////////////////////////////////////////////

#define TESTGUID CKGUID(0x2c5c47f6,0x1d0755d9)


void PhysicManager::_RegisterVSL()
{
	
	ourMan = GetPMan();

	int z = D6DT_Position;
	int y = D6DT_Velocity;
		
	
	STARTVSLBIND(m_Context)

	DECLAREPOINTERTYPE(pVehicleMotorDesc)
	DECLAREMEMBER(pVehicleMotorDesc,float,maxRpmToGearUp)
	DECLAREMEMBER(pVehicleMotorDesc,float,minRpmToGearDown)
	DECLAREMEMBER(pVehicleMotorDesc,float,maxRpm)
	DECLAREMEMBER(pVehicleMotorDesc,float,minRpm)
	DECLAREMETHOD_0(pVehicleMotorDesc,void,setToCorvette)

	DECLAREPOINTERTYPE(pVehicleGearDesc)
	DECLAREMEMBER(pVehicleGearDesc,int,nbForwardGears)
	DECLAREMETHOD_0(pVehicleGearDesc,void,setToDefault)
	DECLAREMETHOD_0(pVehicleGearDesc,void,setToCorvette)
	DECLAREMETHOD_0(pVehicleGearDesc,bool,isValid)

	

	DECLAREOBJECTTYPE(pWheelDescr)
	DECLARECTOR_0(__newpWheelDescr)

	DECLAREOBJECTTYPE(pVehicleDesc)
	DECLARECTOR_0(__newpVehicleDescr)
	DECLAREMEMBER(pVehicleDesc,float,digitalSteeringDelta)
	DECLAREMEMBER(pVehicleDesc,VxVector,steeringSteerPoint)
	DECLAREMEMBER(pVehicleDesc,VxVector,steeringTurnPoint)
	DECLAREMEMBER(pVehicleDesc,float,steeringMaxAngle)
	DECLAREMEMBER(pVehicleDesc,float,transmissionEfficiency)
	DECLAREMEMBER(pVehicleDesc,float,differentialRatio)
	DECLAREMEMBER(pVehicleDesc,float,maxVelocity)
	DECLAREMEMBER(pVehicleDesc,float,motorForce)

	DECLAREMETHOD_0(pVehicleDesc,pVehicleGearDesc*,getGearDescription)
	DECLAREMETHOD_0(pVehicleDesc,pVehicleMotorDesc*,getMotorDescr)
	DECLAREMETHOD_0(pVehicleDesc,void,setToDefault)
		
	DECLAREPOINTERTYPE(pWheel)
	DECLAREPOINTERTYPE(pVehicle)
	DECLAREPOINTERTYPE(pVehicleMotor)
	DECLAREPOINTERTYPE(pVehicleGears)

	DECLAREPOINTERTYPE(pWheel1)
	DECLAREPOINTERTYPE(pWheel2)

	DECLAREINHERITANCESIMPLE("pWheel","pWheel1")
	DECLAREINHERITANCESIMPLE("pWheel","pWheel2")


	//////////////////////////////////////////////////////////////////////////
	//
	//					Vehicle : 
	//
	DECLAREMETHOD_1(pVehicle,void,updateVehicle,float)
	DECLAREMETHOD_5(pVehicle,void,setControlState,float,bool,float,bool,bool)


	DECLAREMETHOD_1(pVehicle,pWheel*,getWheel,CK3dEntity*)
	DECLAREMETHOD_0(pVehicle,pVehicleMotor*,getMotor)
	DECLAREMETHOD_0(pVehicle,pVehicleGears*,getGears)

	DECLAREMETHOD_0(pVehicle,void,gearUp)
	DECLAREMETHOD_0(pVehicle,void,gearDown)
	
	
	DECLAREMETHOD_0(pVehicleGears,int,getGear)



	//////////////////////////////////////////////////////////////////////////
	//motor : 
	DECLAREMETHOD_0(pVehicleMotor,float,getRpm)
	DECLAREMETHOD_0(pVehicleMotor,float,getTorque)


	DECLAREMETHOD_0(pWheel,pWheel1*,castWheel1)
	DECLAREMETHOD_0(pWheel,pWheel2*,castWheel2)

	DECLAREMETHOD_0(pWheel,float,getWheelRollAngle)
	DECLAREMETHOD_0(pWheel2,float,getRpm)
	DECLAREMETHOD_0(pWheel2,float,getAxleSpeed)
	DECLAREMETHOD_0(pWheel2,float,getSuspensionTravel)
	DECLAREMETHOD_0(pWheel2,VxVector,getGroundContactPos)



	DECLAREMETHOD_2(pFactory,pVehicle*,createVehicle,CK3dEntity*,pVehicleDesc)
	
	
	

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