#ifndef __PVEHICLETYPES_H__
#define __PVEHICLETYPES_H__

#include "pVTireFunction.h"
#include "pLinearInterpolation.h"

/** \addtogroup Vehicle
@{
*/

typedef enum FrictionCircleMethod
{
	FC_GENTA,                   // Genta (page 52?)
	FC_VECTOR,                  // Vector reduction to fit circle
	FC_VOID,                    // Don't touch forces (for testing)
	FC_SLIPVECTOR               // Gregor's method of combined Pacejka
};


#define BREAK_TABLE_ENTRIES 10

class pVehicleBrakeTable
{
public: 
	float brakeEntries[BREAK_TABLE_ENTRIES];
};

class MODULE_API pVehicleDesc
{

public:

	VxVector					position;

	pVehicleGearDesc *gearDescription;
	pVehicleGearDesc * getGearDescription() { return gearDescription; }

	pVehicleMotorDesc *motorDescr;
	pVehicleMotorDesc * getMotorDescr() { return motorDescr; }




	float					mass;
	float					motorForce;
	float					transmissionEfficiency;
	float					differentialRatio;

	VxVector					steeringTurnPoint;
	VxVector					steeringSteerPoint;
	float					steeringMaxAngle;

	VxVector					centerOfMass;

	float					digitalSteeringDelta;

	float					maxVelocity;
	float					digitalSteeringDeltaVelocityModifier;

	int xmlLinkID;
	XString xmlName;
	
	pRigidBody *body;


	void*					userData;

	//----------------------------------------------------------------
	//
	// Engine Related Parameters
	//
	float engineInertia;
	float engineFriction;
	float engineBrakingCoefficient;
	float engineMaximumTorque;

	//-RPMs
	float engineRpmMaximum;
	float engineRpmIdle;
	float engineRpmAutoClutch;
	float engineRpmStart;
	float engineRpmStall;

	int engineFlags;

	//-Engine Torque Curve
	pLinearInterpolation torqueCurve;
	
	int processFlags;



	pVehicleDesc();
	void setToDefault();
	bool isValid() const;
};



typedef std::vector<pWheel*>WheelArrayType;

/** @} */

#include "pLinearInterpolation.h"





#endif // __PVEHICLETYPES_H__