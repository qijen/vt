#include <StdAfx.h>
#include "vtPhysXAll.h"

void pVehicleGearDesc::setToCorvette() {

	forwardGearRatios[0] = 1.66f;
	forwardGearRatios[1] = 1.78f;
	forwardGearRatios[2] = 1.30f;
	forwardGearRatios[3] = 1;
	forwardGearRatios[4] = 0.74f;
	forwardGearRatios[5] = 0.50f;
	nbForwardGears = 6;

	backwardGearRatio = -2.90f;

}


void pVehicleGearDesc::setToDefault() 
{
	//forwardGears.clear();
}

bool pVehicleGearDesc::isValid() const 
{
	if (nbForwardGears > getMaxNumOfGears()) {
		fprintf(stderr, "NxVehicleGearDesc::isValid(): nbForwardGears(%d) is bigger than max (%d)\n",
			nbForwardGears, getMaxNumOfGears());
		return false;
	}
	if (nbForwardGears <= 0) {
		fprintf(stderr, "NxVehicleGearDesc::isValid(): nbForwardGears(%d) smaller or equal 0\n", nbForwardGears);
		return false;
	}
	if (backwardGearRatio > 0) {
		fprintf(stderr, "NxVehilceGearDesc::isValid(): backwardGearRatio(%2.3f) is bigger than 0, make it negative\n", backwardGearRatio);
		return false;
	}
	for (int i = 0; i < nbForwardGears; i++) 
	{
		if (forwardGearRatios[i] < 0) 
		{
			fprintf(stderr, "NxVehilceGearDesc::isValid(): forwardGearRatios[%d] (%2.3f) has value smaller 0\n", i, forwardGearRatios[i]);
			return false;
		}
	}
	return true;
}


float pVehicleGears::getCurrentRatio() const {
	return getRatio(_curGear);
}

float pVehicleGears::getRatio(NxI32 gear) const {
	if (gear > 0)
		return _forwardGearRatios[gear-1];
		//return _forwardGears[gear-1];
	if (gear == -1)
		return _backwardGearRatio;
	return 0;
}

