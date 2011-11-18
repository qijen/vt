#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

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
void PhysicManager::_RegisterVSLVehicle()
{
	STARTVSLBIND(m_Context)

	//----------------------------------------------------------------
	//
	// vehicle base types
	//

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





	STOPVSLBIND
	

}