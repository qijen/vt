#ifndef __P_VEHICLE_GEARS_H__

#define __P_VEHICLE_GEARS_H__

#include "pVehicleTypes.h"
#include "NxArray.h"


#define NX_VEHICLE_MAX_NB_GEARS 32

/** \addtogroup Vehicle
@{
*/

class MODULE_API pVehicleGearDesc {
public:
	pVehicleGearDesc() { setToDefault(); }
	void setToDefault();
	void setToCorvette();
	bool isValid() const;
	int									getMaxNumOfGears() const { return NX_VEHICLE_MAX_NB_GEARS; }


	//NxArray<pLinearInterpolation>	forwardGears;
	//NxArray<float>							forwardGearRatios;
	int									nbForwardGears;
	pLinearInterpolation			forwardGears[NX_VEHICLE_MAX_NB_GEARS];
	float									forwardGearRatios[NX_VEHICLE_MAX_NB_GEARS];

	//NxLinearInterpolationValues				backwardGear;
	float									backwardGearRatio;
};

/**
\brief A abstract base class for a vehicle gear box.
*/
class MODULE_API pVehicleGears {

public:
	//NxArray<NxLinearInterpolationValues>	_forwardGears;
	//NxArray<float>							_forwardGearRatios;
	int									_nbForwardGears;
	pLinearInterpolation				_forwardGears[NX_VEHICLE_MAX_NB_GEARS];
	float									_forwardGearRatios[NX_VEHICLE_MAX_NB_GEARS];

	//NxLinearInterpolationValues				_backwardGear;
	float									_backwardGearRatio;

	int									_curGear;


public:
	pVehicleGears(): _curGear(1) { }

	//static NxVehicleGears* createVehicleGears(const NxVehicleGearDesc& gearDesc);

	float					getCurrentRatio() const;
	float					getRatio(int gear) const;
	int					getGear() const { return _curGear; }
	int					getMaxGear() const { return _nbForwardGears; }
	void					gearUp() { _curGear = NxMath::min(_curGear+1, (int)_nbForwardGears); }
	void					gearDown() { _curGear = NxMath::max(_curGear-1, -1); }
};

/** @} */

#endif