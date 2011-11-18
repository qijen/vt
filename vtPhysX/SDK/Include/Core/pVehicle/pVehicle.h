#ifndef __P_VEHICLE_H__
#define __P_VEHICLE_H__

#include "vtPhysXBase.h"

#include "pWheel.h"
#include "NxArray.h"
#include "NxUserContactReport.h"
#include "pVehicleGears.h"
#include "pVTireFunction.h"
#include "pReferencedObject.h"
#include "pCallbackObject.h"


class pEngine;
class pGearBox;
class pDifferential;
class pDriveLine;
class pVehicleSteer;



/** \addtogroup Vehicle
@{
*/
typedef std::vector<pWheel*>WheelArrayType;

/**
\brief A abstract base class for a vehicle object.
*/

typedef enum pVehicleControlStateItem
{
	E_VCS_ACCELERATION,
	E_VCS_STEERING,
	E_VCS_HANDBRAKE,
	E_VCS_BRAKING,
	E_VCS_GUP,
	E_VCS_GDOWN,
	E_VCS_TCLUTCH,
	E_VCS_STARTER,
	E_VCS_BRAKES,
};
enum ControlTypes
// Which kind of controls do we have?
{
	T_STEER_LEFT,T_STEER_RIGHT,
	T_THROTTLE,T_BRAKES,
	T_SHIFTUP,T_SHIFTDOWN,
	T_CLUTCH,
	T_HANDBRAKE,
	T_STARTER,
	T_HORN,
	MAX_CONTROLLER_TYPE
};

typedef enum pVehicleControlStateItemMode
{
	E_VCSM_ANALOG,
	E_VCSM_DIGITAL,
};

typedef enum pVehicleBreakCase
{
	BC_NoUserInput,
	BC_DirectionChange,
	BC_UserBreak,
	BC_Handbrake,
	BC_NumBreakCases,
};

typedef enum pVehicleBreakLevel
{
	BL_NoBreak=-1,
	BL_Small,
	BL_Medium,
	BL_High,
	BL_NumBreakLevels,
};

typedef enum
{
	VBF_UseTable=(1<<0),
	VBF_Autobreak=(1<<1),

};
enum VehicleStatus
{
	VS_IsMoving=(1<<0),
	VS_IsAccelerated=(1<<1),
	VS_IsAcceleratedForward=(1<<2),
	VS_IsAcceleratedBackward=(1<<3),
	VS_AllWheelsOnGround=(1<<4),
	VS_IsFalling=(1<<5),
	VS_Handbrake=(1<<6),
	VS_IsBraking=(1<<7),
	VS_IsSteering=(1<<8),
	VS_HasGearBox=(1<<9),
	VS_HasMotor=(1<<10),
	VS_IsRollingForward=(1<<11),
	VS_IsRollingBackward=(1<<12),
};

class MODULE_API pVehicle : 
	public xEngineObjectAssociation<CK3dEntity*>,
	public pCallbackObject
{

public:
	

	//----------------------------------------------------------------
	//
	// new engine code 
	//
	enum Max
	{
		MAX_WHEEL=8,
		MAX_DIFFERENTIAL=3,                 // Enough for a 4WD Jeep
		MAX_WING=10,                        // Front, rear and perhaps others
		MAX_CAMERA=10			// Max #camera's around car
	};
	pEngine *engine;
	pGearBox *gearbox;
	pDifferential  *differential[MAX_DIFFERENTIAL];
	int             differentials;
	pDriveLine     *driveLine;
	pVehicleSteer *steer;
	

	float motorTorque;
	float breakTorque;



	//----------------------------------------------------------------
	//
	// public access
	//
	pDifferential *getDifferential(int n){ return differential[n]; }
	pEngine       *getEngine(){ return engine; }
	pDriveLine    *getDriveLine(){ return driveLine; }
	pGearBox      *getGearBox(){ return gearbox; }
	pVehicleSteer * getSteer() { return steer; }
	void setSteer(pVehicleSteer * val) { steer = val; }

	void findDifferentialWheels(int& wheel1Index,int& wheel2Index);

	
	
	int            getNbDifferentials(){ return differentials; }
	void           addDifferential(pDifferential *diff);

	float getRPM();
	bool isStalled();
	bool isAutoClutch();
	bool isAutoShifting();
	int getGear();
	bool isValidEngine();
	bool hasDifferential();
	bool shiftUp();
	bool shiftDown();
	int setGear(int gear);
	float getTorque(int component);
	
	//----------------------------------------------------------------
	//
	// control 
	//
	void setControlState(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake);
	void setControlState(int parameter,float value);
	void setControlMode(int parameter,int mode);
	void getControlState(int parameter,float &value,int &mode);


	//----------------------------------------------------------------
	//
	// internal 
	//
	int initEngine(int flags);
	int doEngine(int flags,float dt);
	void preCalculate();
	void calcForces();
	void integrate();
	void postStep();
	void cleanup();
	
	void PreCalcDriveLine();

	void calculateForces();


	void preAnimate();
	void postAnimate();

	void setClutch(float clutch);
	float getClutch();





	//----------------------------------------------------------------
	//
	// manager calls
	//
	void step(float dt);


	void advanceTime(float lastDeltaMS);
	
	//----------------------------------------------------------------
	//
	// manager calls 
	//
	int onPreProcess();
	int onPostProcess();

	void processPostScript();
	void processPreScript();


	XString debugPrint(int mask);

	//----------------------------------------------------------------
	//
	// Break functions
	//

	bool useBreakTable;
	int breakFlags;
	int breakConditionLevels[BC_NumBreakCases];
	float mBreakPressures[BL_NumBreakLevels];
	float             mBrakeTable[BL_NumBreakLevels][BREAK_TABLE_ENTRIES]; //!< Brake Table to take advantage of a certain brake lvl at any one time
	
	pVehicleBrakeTable mSmallBrakeTable;
	pVehicleBrakeTable mMediumBrakeTable;
	pVehicleBrakeTable mHighBrakeTable;

	bool mBreakLastFrame;

	float mTimeBreaking;
	float mBrakeMediumThresold;
	float mBrakeHighThresold;



	void setBreakPressure(int breakLevel,float pressure);
	float getBreakPressure(int breakLevel);
	int getBreakFlags() const { return breakFlags; }
	void setBreakFlags(int val) { breakFlags = val; }

	bool isUsingBreakTable() const { return useBreakTable; }
	void setUsingBreakTable(bool val) { useBreakTable = val; }
	pVehicleBreakCase calculateBreakCase(int currentAccelerationStatus);


	void setBreakCaseLevel(pVehicleBreakCase breakCase,pVehicleBreakLevel level);
	pVehicleBreakLevel getBreaklevelForCase(pVehicleBreakCase breakCase) { return (pVehicleBreakLevel)breakConditionLevels[breakCase]; }

	float getBrakeTorque();

	void setMeasurementWheel(CK3dEntity*wheelReference);
	float getBrakeAmount(pVehicleBreakLevel brakeLevel);
	float getBrakeAmountFromTable(pVehicleBreakLevel brakeLevel);

	

	float       getBrakeSmallPressure()    {return mBreakPressures[BL_Small];}      
	float       getBrakeMediumPressure()   {return mBreakPressures[BL_Medium];}     
	float       getBrakeHighPressure()  {return mBreakPressures[BL_High];}    

	float       getBrakeMediumApplyTime()  {return mBrakeMediumThresold;}     
	float       getBrakeHighApplyTime() {return mBrakeHighThresold;}    

	//----------------------------------------------------------------
	//
	// misc
	//
	
	int mVSFlags;
	float _cSteering;
	float _cAcceleration;
	int _cShiftStateUp;
	int _cShiftStateDown;
	bool _cAnalogSteering;
	bool _cAnalogAcceleration;
	bool _cHandbrake;

	
	NxArray<pWheel*>		_wheels;
	

	

	
	//WheelArrayType _wheels;
	//NxArray<pWheel*> getWheels() const { return _wheels; }


	NxArray<pWheel*>& getWheels()  { return _wheels; }


	int& getStateFlags() { return mVSFlags; }
	void setVSFlags(int val) { mVSFlags = val; }


	void printAllToConsole();

	//----------------------------------------------------------------
	//
	// old code
	//
	pVehicleGears*			_vehicleGears;
	pVehicleMotor*			_vehicleMotor;


	pVehicleMotor* getMotor() const { return _vehicleMotor; }
	void setMotor(pVehicleMotor* val) { _vehicleMotor = val; }
	
	pVehicleGears* getGears() const { return _vehicleGears; }
	void setGears(pVehicleGears* val) { _vehicleGears = val; }

	float					_steeringWheelState;
	float					_accelerationPedal;
	float					_brakePedal;
	bool					_brakePedalChanged;
	bool					_handBrake;
	float					_acceleration;

	float					_digitalSteeringDelta;
	float					_mass;

	float					_lastDT;
	VxVector _centerOfMass;

	VxVector				_steeringTurnPoint;

	VxVector				_steeringSteerPoint;
	float getMass();
	void setMass(float val) { _mass = val; }

	
	
	VxVector getCenterOfMass() const { return _centerOfMass; }
	void setCenterOfMass(VxVector val) { _centerOfMass = val; }

	
	float getDigitalSteeringDelta() const { return _digitalSteeringDelta; }
	void setDigitalSteeringDelta(float val) { _digitalSteeringDelta = val; }
	
	


	VxVector getSteeringTurnPoint() const { return _steeringTurnPoint; }
	void setSteeringTurnPoint(VxVector val) { _steeringTurnPoint = val; }
	
	
	VxVector getSteeringSteerPoint() const { return _steeringSteerPoint; }
	void setSteeringSteerPoint(VxVector val) { _steeringSteerPoint = val; }
	
	float getMotorForce() const { return _motorForce; }
	void setMotorForce(float val) { _motorForce = val; }

	float					_transmissionEfficiency;
	float					_steeringMaxAngleRad;

	float					_motorForce;
	NxVec3					_localVelocity;
	bool					_braking;
	bool					_releaseBraking;
	float					_maxVelocity;
	NxMaterial*				_carMaterial;
	float					_cameraDistance;


	NxActor*				_mostTouchedActor;
	bool mAutomaticMode;

	float					_differentialRatio;


	float getTransmissionEfficiency() const { return _transmissionEfficiency; }
	void setTransmissionEfficiency(float val) { _transmissionEfficiency = val; }
	
	
	float getDifferentialRatio() const { return _differentialRatio; }
	void setDifferentialRatio(float val) { _differentialRatio = val; }

	
	
	float getMaxVelocity() const { return _maxVelocity; }
	void setMaxVelocity(float val) { _maxVelocity = val; }

	
	bool getAutomaticMode() const { return mAutomaticMode; }
	

	float getMPH(int type=0);

	void setAutomaticMode(bool autoMode);

	void					_computeMostTouchedActor();
	void					_computeLocalVelocity();
	float					_computeAxisTorque();
	float					_computeAxisTorqueV2();
	float					_computeRpmFromWheels();
	float					_computeMotorRpm(float rpm);

	void					_updateRpms();

	float					_getGearRatio();

	void					_controlSteering(float steering, bool analogSteering);
	void					_controlAcceleration(float acceleration, bool analogAcceleration);

	
	
	float getMaxSteering() const { return _steerMax; }
	void setMaxSteering(float val) { _steerMax = val; 	}

	float					_steerMax;
	int _nbTouching;
	int _nbNotTouching;
	int _nbHandbrakeOn;
	int _currentStatus;

	int flags;

	int _calculateCurrentStatus();
	int _calculateReaction();
	int _calculateNextSteering();
	int _performSteering(float dt);
	void doSteering();
	int _performAcceleration(float dt);

	
	float calculateBraking(float dt);
	
	pRigidBody *mBody;
	NxActor *mActor;
	

	
	public : 
		pVehicle();
		pVehicle(pVehicleDesc descr,CK3dEntity *referenceObject);

		pRigidBody * getBody() const { return mBody; }
		void setBody(pRigidBody * val) { mBody = val; }
		void setToDefault();

		int initWheels(int flags);

		void updateWheels(int flags= 0);

		void					handleContactPair(NxContactPair* pair, int carIndex);
		void updateVehicle(float lastTimeStepSize);
		void					control (float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake);
		void					updateControl(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake);
		void					gearUp();
		void					gearDown();
		void					standUp();
		float					getDriveVelocity();
		float					getMaxVelocity() { return _maxVelocity; }
		int					getNbWheels() { return _wheels.size(); }
		const pWheel*			getWheel(int i);
		pWheel*getWheel(CK3dEntity* wheelReference);

		
		NxActor *getActor();
		void setActor(NxActor * val) { mActor = val; }
		int load(const pVehicleDesc& src );

};






/** @} */

#endif