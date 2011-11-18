#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

#include "pVehicleAll.h"


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

	
	//----------------------------------------------------------------
	//
	// vehicle specific 
	//

	DECLAREMETHOD_1(pVehicle,void,setPreScript,int)
	DECLAREMETHOD_1(pVehicle,void,setPostScript,int)
	DECLAREMETHOD_1(pVehicle,void,setOverrideMask,int)



	DECLAREMETHOD_1(pVehicle,int,initEngine,int)
	
	DECLAREMETHOD_0(pVehicle,BOOL,isValidEngine)

	DECLAREMETHOD_0(pVehicle,pEngine*,getEngine)
	DECLAREMETHOD_0(pVehicle,pGearBox*,getGearBox)

	DECLAREMETHOD_0(pVehicle,int,getStateFlags)
	
	DECLAREMETHOD_1(pVehicle,void,setClutch,float)
	DECLAREMETHOD_0(pVehicle,float,getClutch)

	//----------------------------------------------------------------
	//
	// egine
	//
	DECLAREMETHOD_1(pEngine,void,SetInertia,float)
	DECLAREMETHOD_1(pEngine,void,setIdleRPM,float)
	DECLAREMETHOD_1(pEngine,void,setStallRPM,float)
	DECLAREMETHOD_1(pEngine,void,setStartRPM,float)

	DECLAREMETHOD_1(pEngine,void,setTimeScale,float)
	DECLAREMETHOD_1(pEngine,void,setEndRotationalFactor,float)


	DECLAREMETHOD_1(pEngine,void,setForceFeedbackScale,float)
	DECLAREMETHOD_0(pEngine,float,getForceFeedbackScale)

	//----------------------------------------------------------------
	//
	// engine
	//
	DECLAREMETHOD_0(pVehicle,pEngine*,getEngine)

	DECLAREMETHOD_0(pEngine,pLinearInterpolation,getTorqueCurve)
	DECLAREMETHOD_1(pEngine,void,setTorqueCurve,pLinearInterpolation)

	DECLAREMETHOD_0(pEngine,float,getRPM)
	DECLAREMETHOD_0(pEngine,float,getTorque)
	DECLAREMETHOD_0(pEngine,void,setToDefault)
	DECLAREMETHOD_1(pEngine,void,updateUserControl,int)

	DECLAREMETHOD_1(pEngine,void,setMaxTorque,float)
	DECLAREMETHOD_1(pEngine,void,setMaxRPM,float)
	DECLAREMETHOD_1(pEngine,void,setIdleRPM,float)

	DECLAREMETHOD_1(pEngine,void,setBrakingCoeff,float)
	DECLAREMETHOD_0(pEngine,float,getBrakingCoeff)

	DECLAREMETHOD_1(pEngine,void,setFriction,float)
	DECLAREMETHOD_0(pEngine,float,getFriction)

	DECLAREMETHOD_1(pEngine,void,SetInertia,float)

	DECLAREMETHOD_1(pEngine,void,setStartRPM,float)
	DECLAREMETHOD_0(pEngine,float,getStartRPM)

	DECLAREMETHOD_0(pEngine,int,GetGears)
	DECLAREMETHOD_0(pVehicle,void,PreCalcDriveLine)

	//DECLAREMETHOD_1(pEngine,float,GetGearRatio,int)


	//----------------------------------------------------------------
	//
	// interpolation curve, used for for torque and gears
	//
	DECLAREMETHOD_0(pLinearInterpolation,int,getSize)
	DECLAREMETHOD_1(pLinearInterpolation,int,isValid,float)
	DECLAREMETHOD_1(pLinearInterpolation,float,getValue,float)
	DECLAREMETHOD_1(pLinearInterpolation,int,getValueAtIndex,int)
	DECLAREMETHOD_2(pLinearInterpolation,void,insert,float,float)


	//----------------------------------------------------------------
	//
	// gearbox 
	//

	DECLAREMETHOD_1(pGearBox,float,GetTorqueForWheel,pWheel2*)

	DECLAREMETHOD_0(pGearBox,pLinearInterpolation,getGearRatios)
	DECLAREMETHOD_1(pGearBox,void,setGearRatios,pLinearInterpolation)

	DECLAREMETHOD_0(pGearBox,pLinearInterpolation,getGearTensors)
	DECLAREMETHOD_1(pGearBox,void,setGearTensors,pLinearInterpolation)


	//----------------------------------------------------------------
	//
	// wheel2
	//
	DECLAREMETHOD_1(pWheel2,void,setPreScript,int)
	DECLAREMETHOD_1(pWheel2,void,setPostScript,int)
	DECLAREMETHOD_1(pWheel2,void,setOverrideMask,int)

	DECLAREMETHOD_0(pWheel2,float,getEndBrakingTorqueForWheel)
	DECLAREMETHOD_0(pWheel2,float,getEndTorqueForWheel)
	DECLAREMETHOD_0(pWheel2,float,getEndAccForWheel)
	
	DECLAREMETHOD_0(pWheel2,float,getWheelTorque)
	DECLAREMETHOD_0(pWheel2,float,getWheelBreakTorque)
	DECLAREMETHOD_0(pWheel2,float,getAxleSpeed)
	
	DECLAREMETHOD_0(pWheel2,VxVector,GetForceRoadTC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetForceBodyCC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetTorqueTC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetTorqueFeedbackTC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetTorqueBrakingTC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetTorqueRollingTC)

	DECLAREMETHOD_0(pWheel2,float,GetSlipAngle)
	DECLAREMETHOD_0(pWheel2,float,GetSlipRatio)
	DECLAREMETHOD_0(pWheel2,float,GetHeading)
	DECLAREMETHOD_0(pWheel2,float,GetRotation)
	DECLAREMETHOD_0(pWheel2,float,GetRotationV)
	DECLAREMETHOD_0(pWheel2,float,GetAcceleration)

	DECLAREMETHOD_0(pWheel2,VxVector,GetVelocity)
	//DECLAREMETHOD_0(pWheel2,VxVector,GetAcceleration)
	DECLAREMETHOD_0(pWheel2,VxVector,GetSlipVectorCC)
	DECLAREMETHOD_0(pWheel2,VxVector,GetPosContactWC)


	DECLAREMETHOD_0(pWheel2,float,getEndBrakingTorqueForWheel)
	DECLAREMETHOD_0(pWheel2,float,getEndTorqueForWheel)
	DECLAREMETHOD_0(pWheel2,float,getEndAccForWheel)
	DECLAREMETHOD_0(pWheel2,float,getWheelTorque)
	DECLAREMETHOD_0(pWheel2,float,getWheelBreakTorque)
	DECLAREMETHOD_0(pWheel2,bool,isAxleSpeedFromVehicle)
	DECLAREMETHOD_0(pWheel2,bool,isTorqueFromVehicle)




	DECLAREMEMBER(pWheel2,float,radius)
	
	
	DECLAREMEMBER(pWheel2,VxVector,forceGravityCC)
	DECLAREMEMBER(pWheel2,VxVector,forceBrakingTC)



	DECLAREMEMBER(pWheel2,VxVector,torqueTC)
	DECLAREMEMBER(pWheel2,VxVector,forceRoadTC)

	DECLAREMEMBER(pWheel2,VxVector,rotation)
	DECLAREMEMBER(pWheel2,VxVector,rotationA)
	DECLAREMEMBER(pWheel2,VxVector,rotationV)
	DECLAREMEMBER(pWheel2,VxVector,velWheelTC)
	DECLAREMEMBER(pWheel2,VxVector,velWheelCC)

	DECLAREMETHOD_1(pWheel2,void,setMass,float)
	DECLAREMETHOD_1(pWheel2,void,setTireRate,float)
	
	DECLAREMETHOD_0(pWheel2,float,getCsSlipLen)
	DECLAREMETHOD_1(pWheel2,void,setRollingCoeff,float)

	


	

	DECLAREMETHOD_0(pWheel2,float,getSuspensionTravel)

	//----------------------------------------------------------------
	//
	// pwheel contactdata
	//
	DECLAREMETHOD_0(pWheel2,pWheelContactData*,getContact)
	
	DECLAREMETHOD_1(pWheel2,bool,getContact,pWheelContactData&dst)

	DECLAREMEMBER(pWheel2,bool,hadContact)

	DECLAREMEMBER(pWheelContactData,VxVector,contactPoint)
	DECLAREMEMBER(pWheelContactData,VxVector,contactNormal)
	DECLAREMEMBER(pWheelContactData,VxVector,longitudalDirection)
	DECLAREMEMBER(pWheelContactData,VxVector,lateralDirection)
	DECLAREMEMBER(pWheelContactData,CK3dEntity*,contactEntity)
	DECLAREMEMBER(pWheelContactData,float,contactForce,)
	DECLAREMEMBER(pWheelContactData,float,longitudalSlip,)
	DECLAREMEMBER(pWheelContactData,float,lateralSlip)
	DECLAREMEMBER(pWheelContactData,float,longitudalImpulse)
	DECLAREMEMBER(pWheelContactData,float,lateralImpulse)
	DECLAREMEMBER(pWheelContactData,float,contactPosition)
	DECLAREMEMBER(pWheelContactData,int,otherShapeMaterialIndex)




	
	//----------------------------------------------------------------
	//
	// 
	//
	DECLAREMETHOD_0(pWheel2,pPacejka*,getPacejka)
	
	DECLAREMETHOD_1(pPacejka,void,SetFx,float)
	DECLAREMETHOD_1(pPacejka,void,SetFy,float)
	DECLAREMETHOD_1(pPacejka,void,SetMz,float)
	DECLAREMETHOD_1(pPacejka,void,SetCamber,float)
	DECLAREMETHOD_1(pPacejka,void,SetSlipAngle,float)
	DECLAREMETHOD_1(pPacejka,void,SetSlipRatio,float)
	DECLAREMETHOD_1(pPacejka,void,SetNormalForce,float)
	DECLAREMETHOD_0(pPacejka,void,setToDefault)
	DECLAREMETHOD_0(pPacejka,void,Calculate)
	DECLAREMETHOD_0(pPacejka,float,GetMaxLongForce)
	DECLAREMETHOD_0(pPacejka,float,GetMaxLatForce)
	DECLAREMETHOD_1(pPacejka,void,SetMaxLongForce,float)
	DECLAREMETHOD_1(pPacejka,void,SetMaxLatForce,float)
	DECLAREMETHOD_0(pPacejka,float,GetLongitudinalStiffness)
	DECLAREMETHOD_0(pPacejka,float,GetCorneringStiffness)
	DECLAREMETHOD_0(pPacejka,float,GetFx)
	DECLAREMETHOD_0(pPacejka,float,GetFy)
	DECLAREMETHOD_0(pPacejka,float,GetMz)


	DECLAREMETHOD_1(pRigidBody,pWheel2*,getWheel2,CK3dEntity*)

	DECLAREMETHOD_0(pVehicle,void,PreCalcDriveLine)

	//----------------------------------------------------------------
	//
	// 
	//
	
	/*
	DECLAREMETHOD_1(pVehicle,void,updateVehicle,float)
	DECLAREMETHOD_5(pVehicle,void,setControlState,float,bool,float,bool,bool)


	DECLAREMETHOD_1(pVehicle,pWheel*,getWheel,CK3dEntity*)
	DECLAREMETHOD_0(pVehicle,pVehicleMotor*,getMotor)
	DECLAREMETHOD_0(pVehicle,pVehicleGears*,getGears)

	DECLAREMETHOD_1(pVehicle,void,setPreProcessingScript,int)
	DECLAREMETHOD_1(pVehicle,void,setPostProcessingScript,int)

	DECLAREMETHOD_0(pVehicle,void,gearUp)
	DECLAREMETHOD_0(pVehicle,void,gearDown)
	DECLAREMETHOD_0(pVehicleGears,int,getGear)
	*/


	//////////////////////////////////////////////////////////////////////////
	//motor : 
	
	/*DECLAREMETHOD_0(pVehicleMotor,float,getRpm)
	DECLAREMETHOD_0(pVehicleMotor,float,getTorque)


	DECLAREMETHOD_0(pWheel,pWheel1*,castWheel1)
	DECLAREMETHOD_0(pWheel,pWheel2*,castWheel2)

	DECLAREMETHOD_0(pWheel,float,getWheelRollAngle)
	DECLAREMETHOD_0(pWheel2,float,getRpm)
	DECLAREMETHOD_0(pWheel2,float,getAxleSpeed)
	DECLAREMETHOD_0(pWheel2,float,getSuspensionTravel)
	DECLAREMETHOD_0(pWheel2,VxVector,getGroundContactPos)
*/







	STOPVSLBIND
	

}