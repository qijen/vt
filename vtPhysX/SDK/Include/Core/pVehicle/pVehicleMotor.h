#ifndef __P_VEHICLE_MOTOR_H__
#define __P_VEHICLE_MOTOR_H__

#include "vtPhysXBase.h"
#include "pVehicleTypes.h"
#include "pDriveline.h"
#include "pEngine.h"


/** \addtogroup Vehicle
@{
*/

class MODULE_API pVehicleMotorDesc 
{
public:
	pLinearInterpolation torqueCurve;
	float						maxRpmToGearUp;
	float						minRpmToGearDown;
	float						maxRpm;
	float						minRpm;

	void setToDefault();
	pVehicleMotorDesc() { setToDefault(); }
	void setToCorvette();
	bool isValid() const;
};

class MODULE_API pVehicleMotor {

public:

	pLinearInterpolation _torqueCurve;
	
	float						_rpm;
	
	
	
	
	float getMaxTorquePos() const { return _maxTorquePos; }
	void setMaxTorquePos(float val) { _maxTorquePos = val; }

	
	
	float getMaxRpmToGearUp() const { return _maxRpmToGearUp; }
	void setMaxRpmToGearUp(float val) { _maxRpmToGearUp = val; }
	
	float getMinRpmToGearDown() const { return _minRpmToGearDown; }
	void setMinRpmToGearDown(float val) { _minRpmToGearDown = val; }
	
	
	float getMaxTorque() const { return _maxTorque; }
	void setMaxTorque(float val) { _maxTorque = val; }



	float						_maxTorquePos;
	float						_maxTorque;

	float						_maxRpm;
	
	

	
	float						_minRpm;
	
	
	
	float						_minRpmToGearDown;
	float						_maxRpmToGearUp;



	pVehicleMotor() : _rpm(0) { }

	void setMaxRpm(float val) { _maxRpm = val; }
	void setMinRpm(float val) { _minRpm = val; }

	
	void		setRpm(float rpm) { _rpm = rpm; }
	float		getRpm() const { return _rpm; }

	float		getMinRpm() const { return _minRpm; }
	float		getMaxRpm() const { return _maxRpm; }


	int		changeGears(const pVehicleGears* gears, float threshold) const;
	float		getTorque() const { return _torqueCurve.getValue(_rpm); }

	int loadNewTorqueCurve(pLinearInterpolation newTCurve);


	int reload(pVehicleMotorDesc* descr);


};



/** @} */

#endif