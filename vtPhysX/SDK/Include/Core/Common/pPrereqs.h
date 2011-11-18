#ifndef __PPREREQS_H_
#define __PPREREQS_H_

class PhysicManager;

//################################################################
//
// Physic Types 
//
class pContactReport;
class pTriggerReport;
class pRayCastReport;
	
class pRigidBody;
class pObjectDescr;

class pJointSettings;
class pJoint;
class pJointBall;
class pJointFixed;
class pJointPulley;

class pJointRevolute;
class pJointDistance;
class pJointD6;
class pJointPrismatic;
class pJointCylindrical;
class pClothDesc;
class pCloth;

class pFluid;
class pFluidDesc;
class pFluidEmitterDesc;
class pFluidEmitter;
class pFluidRenderSettings;

	
class pSerializer;
class pWorld;
class pFactory;
class pSoftBody;


class pWheelDescr;
class pWheel;
class pWheel1;
class pWheel2;

class pVecicleDescr;
class pVehicle;

class pVehicleMotor;
class pVehicleGears;
class pVehicleMotorDesc;
class pVehicleGearDesc;


class pBoxController;


namespace vtAgeia
{
	class pWorldSettings;
	class pSleepingSettings;
	class pShape;
	class pErrorStream;
	class pCollisionsListener;
}

class pLogger;


using namespace vtAgeia;

#endif