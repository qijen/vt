#include <StdAfx.h>
#include "vtPhysXAll.h"


int pVehicleMotor::loadNewTorqueCurve(pLinearInterpolation newTCurve)
{

	_torqueCurve.clear();
	_torqueCurve = newTCurve;
	
	NxReal maxTorque = 0;
	NxI32 maxTorquePos = -1;
	for (NxU32 i = 0; i < _torqueCurve.getSize(); i++) 
	{
		NxReal v = _torqueCurve.getValueAtIndex(i);
		if (v > maxTorque) {
			maxTorque = v;
			maxTorquePos = i;
		}
	}

	_maxTorque			= maxTorque;
	_maxTorquePos		= (float)maxTorquePos;
	

	return 1;
}

void pVehicleMotorDesc::setToDefault() 
{
	torqueCurve.clear();
	minRpmToGearDown	= 1000.0f;
	maxRpmToGearUp		= 4000.f;
	maxRpm				= 5000.f;
	minRpm				= 1000.f;
	


	setToCorvette();

}

void pVehicleMotorDesc::setToCorvette() {
	
	// Default should be values for a corvette!
	// These are corresponding numbers for rotations and torque (in rpm and Nm)

	/*	torqueCurve.insert(1000.f, 193.f);
	torqueCurve.insert(2000.f, 234.f);
	torqueCurve.insert(4000.f, 275.f);
	torqueCurve.insert(5000.f, 275.f);
	torqueCurve.insert(6000.f, 166.f);*/
	torqueCurve.insert(1000, 400);
	torqueCurve.insert(3000, 500);
	torqueCurve.insert(5000, 300);
	minRpmToGearDown	= 1500.f;
	maxRpmToGearUp		= 4000.f;
	minRpm				= 1000.f;
	maxRpm				= 5000.f;
}

bool pVehicleMotorDesc::isValid() const
{

	if (torqueCurve.getSize() == 0) {
		fprintf(stderr, "pVehicleMotorDesc::isValid(): Empty TorqueCurve\n");
		return false;
	}
	if (maxRpmToGearUp < minRpmToGearDown) {
		fprintf(stderr, "pVehicleMotorDesc::isValid(): maxRpmToGearUp (%2.3f) is smaller than minRpmToGearDown (%2.3f)\n",
			maxRpmToGearUp, minRpmToGearDown);
		return false;
	}
	return true;

}


int pVehicleMotor::changeGears(const pVehicleGears* gears, float threshold) const 
{
	NxI32 gear = gears->getGear();
	if (_rpm > _maxRpmToGearUp && gear < gears->getMaxGear())
		return 1;
	else if (_rpm < _minRpmToGearDown && gear > 1)
		return -1;
	/*
	NxReal normalTorque = _torqueCurve.getValue(_rpm);

	NxReal lowerGearRatio = gears->getRatio(gear-1);
	NxReal normalGearRatio = gears->getCurrentRatio();
	NxReal upperGearRatio = gears->getRatio(gear+1);
	NxReal lowerGearRpm = _rpm / normalGearRatio * lowerGearRatio;
	NxReal upperGearRpm = _rpm / normalGearRatio * upperGearRatio;
	NxReal lowerTorque = _torqueCurve.getValue(lowerGearRpm);
	NxReal upperTorque = _torqueCurve.getValue(upperGearRpm);
	NxReal lowerWheelTorque = lowerTorque * lowerGearRatio;
	NxReal normalWheelTorque = normalTorque * normalGearRatio;
	NxReal upperWheelTorque = upperTorque * upperGearRatio;
	//printf("%2.3f %2.3f %2.3f\n", lowerWheelTorque, normalWheelTorque, upperWheelTorque);
	*/

	return 0;
}


