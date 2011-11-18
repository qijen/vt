#ifndef __P_WHEEL_H__
#define __P_WHEEL_H__

#include "vtPhysXBase.h"
#include "pDriveline.h"
#include "pReferencedObject.h"
#include "pCallbackObject.h"
#include "pPacejka.h"



class pDifferential;
class pWheelContactModifyData;


//#include "pWheelTypes.h"



/** \addtogroup Vehicle
@{
*/

/**
\brief Base class to handle a vehicle wheel.
*/
class MODULE_API pWheel : public pDriveLineComp
{
public:


	virtual					~pWheel() {}
	
	pWheel(pRigidBody *body,pWheelDescr *descr);
	pRigidBody * getBody() const { return mBody; }
	void setBody(pRigidBody * val) { mBody= val; }

	virtual void			tick(bool handbrake, float motorTorque, float brakeTorque, NxReal dt) = 0;
	virtual NxActor *		getTouchedActor() const;
	virtual VxVector			getWheelPos() const = 0;
	virtual void			setAngle(NxReal angle) = 0;
	virtual float			getRpm() const = 0;
	virtual VxVector			getGroundContactPos() const = 0;
	virtual float			getRadius() const = 0;

	virtual bool			hasGroundContact() const { return getTouchedActor() != NULL; }
	bool			getWheelFlag(WheelFlags flag) const { return (mWheelFlags & flag) != 0; }

	NxActor *getActor() const { return mActor; }
	void setActor(NxActor* val) { mActor = val; }

	pRigidBody *mBody;
	NxActor *mActor;
	
	

	int					mWheelFlags;
	int& getWheelFlags(){ return mWheelFlags; }
	float _wheelRollAngle;
	
	float& getWheelRollAngle()  { return _wheelRollAngle; }
	void setWheelRollAngle(float val) { _wheelRollAngle = val; }

	pWheel1 *castWheel1();
	pWheel2 *castWheel2();

	virtual void _updateVirtoolsEntity(bool position,bool rotation)=0;
	virtual void _updateAgeiaShape(bool position,bool rotation)=0;

	int mEntID;
	int getEntID() const { return mEntID; }
	void setEntID(int val) { mEntID = val; }

	void setFlags(int flags);

	virtual void _tick(float dt){};

	virtual int  _constructWheel(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);

	virtual pWheelContactData* getContact()=0;


	NxMat34 mWheelPose;
	virtual NxMat34& getWheelPose(){ return mWheelPose; }
	void setWheelPose(NxMat34 val) { mWheelPose = val; }

};

class MODULE_API pWheel1 : public pWheel
{
public:


	pWheel1(pRigidBody *body,pWheelDescr *descr);
	pWheel1();

	virtual void			tick(bool handbrake, float motorTorque, float brakeTorque, float dt);
	virtual NxActor *		getTouchedActor();
	virtual VxVector			getWheelPos() const;
	virtual void			setAngle(float angle);
	virtual float			getRpm() const; 
	virtual VxVector			getGroundContactPos() const { return getWheelPos(); }
	virtual float			getRadius() const { return _radius; }

	ContactInfo				*contactInfo;

	void _updateVirtoolsEntity(bool position,bool rotation);
	void _updateAgeiaShape(bool position,bool rotation);

	NxConvexShape* getWheelConvex() const { return wheelConvex; }
	void setWheelConvex(NxConvexShape* val) { wheelConvex = val; }

	NxCapsuleShape* getWheelCapsule() const { return wheelCapsule; }
	void setWheelCapsule(NxCapsuleShape* val) { wheelCapsule = val; }


	int  _constructWheel(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);

	void					getSteeringDirection(NxVec3& dir);
	void					updateContactPosition();
	void					updateAngularVelocity(float lastTimeStepSize, bool handbrake);
	void					setWheelOrientation(const NxMat33& m);

	virtual void _tick(float dt);

	NxCapsuleShape*			wheelCapsule;
	
	NxConvexShape*			wheelConvex;
	
	
	NxMaterial*				material;
	float					_frictionToSide;
	float					_frictionToFront;

	float					_turnAngle;
	float					_turnVelocity;
	float					_radius;
	float					_perimeter;

	float					_angle;
	float					_wheelWidth;
	float					_maxSuspension;
	//NxReal					_rpm;
	VxVector					_maxPosition;
	pWheelContactData *getContact();

};
class MODULE_API pWheel2 :  
	public pWheel , 
	public xEngineObjectAssociation<CK3dEntity*>,
	public pCallbackObject
{
	public:
	
		pWheel2();
		pWheel2(pRigidBody *body,pWheelDescr *descr,CK3dEntity *wheelShapeReference);

		//----------------------------------------------------------------
		//
		// vehicle 
		//

		//----------------------------------------------------------------
		//
		// new vehicle code : differential
		//
		pDifferential *differential;
		int            differentialSide;      // Which output from the diff
		void setDifferential(pDifferential *diff,int side);
		pDifferential *getDifferential(){ return differential; }
		int getDifferentialSide(){ return differentialSide; }
		
		pPacejka pacejka;                    
		
		pPacejka *getPacejka()  { return &pacejka; }
		void setPacejka(pPacejka val) { pacejka = val; }


		// Translation
		VxVector position;                    // Position in CC wrt the body (!)
		VxVector velocity;                    // Velocity in wheel coords acceleration;   
		VxVector acceleration;                // For the suspension
			
		VxVector GetVelocity(){ return velocity; }
		VxVector GetAcceleration(){ return acceleration; }
		VxVector GetSlipVectorCC(){ return slipVectorCC; }
		VxVector GetPosContactWC(){ return posContactWC; }
		
		// Rotation
		VxVector rotation;                    // Current rotation
		VxVector rotationV;
		VxVector rotationA;         // Rotation speed and acceleration

		// Braking
		float   maxBrakingTorque;		// Max. force when braking 100%
		float   brakingFactor;		// For example .58 for fronts, .42 back
		float   lock;                        // Max angle of heading (radians)

		int      stateFlags;
		float   lowSpeedFactor;              // How slow it is spinning
		float   slipAngle;                   // Vel. vs. wheel dir in radians
		float   slipRatio;                   // Ratio of wheel speed vs. velocity

		// Pre-calculated
		float   distCGM;                     // Distance to center of geometry
		float   angleCGM;                    // Angle (around car Y) of wheel
		float   distCM;                      // Distance to center of mass

		float GetSlipAngle(){ return slipAngle; }
		float GetSlipRatio(){ return slipRatio; }
		float GetHeading(){ return rotation.y; }
		float GetRotation(){ return rotation.x; }
		float GetRotationV(){ return rotationV.x; }



		
		float GetLock(){ return lock; }

		// End forces
		VxVector forceVerticalCC;             // Result of suspension+gravity

		float   differentialSlipRatio;       // SR calculated by differential eq.
		float   tanSlipAngle;                // From this, 'slipAngle' is derived
		float   relaxationLengthLat;         // 'b' in SAE 950311
		float   lastU;                       // To detect switching of long. speed
		float   signU;                       // Sign of longitudinal speed
		float   relaxationLengthLong;        // 'B' in SAE 950311
		float   dampingFactorLong;           // Damping at low speed
		float   dampingFactorLat;            // Damping laterally
		// Combined slip
		float   csSlipLen;                   // Combined slip vector length
		float   load;                        // Load on the wheel (N)
		
		float   mass;                        // Mass of wheel+tyre
		
		float   radius,
			width;
		float   rollingCoeff;                // Rolling resistance coefficient
		
		float   toe;                         // Pre-rotating the wheel
		float   ackermanFactor;              // Scaling the steering angle

		float   optimalSR,optimalSA;         // Optimal slip ratio/angle (in rad)
		float   tanOptimalSA;    

		// Spring values
		float   tireRate;			// Spring vertical rate of tire
		
		// SAE 950311
		float   dampingCoefficientLat,       // Amount of damping at low speed
			dampingCoefficientLong;
		float   dampingSpeed;                // Speed at which to start damping


		float getTireRate() const { return tireRate; }
		void setTireRate(float val) { tireRate = val; }


		void preCalculate();
		void preAnimate();
		void postAnimate();
		void calcForces();

			void calcVerticalForces();
			void calcLongForces();
			void calcLatForces();
			void calcBreakForces();
			
		void Integrate();
		void postStep();
		void cleanup();


		// Physics
		void CalcSlipRatio(VxVector *velWheelCC);
		void CalcSlipAngle();
		void CalcSlipVelocity();
		void CalcLoad();
		void CalcPacejka();
		void CalcDamping();
		void CalcWheelPosWC();
		
		void CalcWheelAngAcc();
		void CalcBodyForce();
		void CalcAccelerations();

		void updateSteeringPose(float rollangle,float steerAngle,float dt);
		void updatePosition();


		void setSteering(float angle);
		float pWheel2::getSteerAngle();



		void calculatePose(float dt);

		
		


		// Other state vars
		//rfloat   load;                        // Load on the wheel (N)
		float   radiusLoaded;                // Rl = radius of loaded wheel to gnd
		float   aligningTorque;              // Mz in the SAE system


		//----------------------------------------------------------------
		//
		// 
		//
		VxVector torqueTC;                    // 3-dimensions of torque
		VxVector torqueBrakingTC;             // Total braking torque (incl. rr)
		VxVector torqueRollingTC;             // Rolling resistance
		VxVector torqueFeedbackTC;            // Feedback (road reaction)
		VxVector forceRoadTC;                 // Because of slip ratio/angle
		VxVector forceDampingTC;              // Damping if low-speed (SAE950311)
		VxVector forceBrakingTC;		// Brakes
		VxVector forceBodyCC;                 // Force on body (for acc.)
		VxVector forceGravityCC;		// Gravity that pulls wheel down
		float   velMagnitude;                // Length of slip velocity vector (obs)
		VxVector velWheelCC,                  // Real velocity of wheel
		velWheelTC;                  // Velocity in tire coords
		// End forces

		VxVector posContactWC;		// Position of contact patch in WC
		VxVector slipVectorCC,                // Diff of tire vs wheel velocity
			slipVectorTC;                // In tire coords
		float   slipVectorLength;            // Magnitude of slipVector?C

		pLinearInterpolation crvSlipTraction;             // Traction curve for slip ratio
		pLinearInterpolation crvSlipBraking;              // Braking curve
		pLinearInterpolation crvLatForce;                 // Slip angle -> normalized lat. force
		pLinearInterpolation  crvSlip2FC;                  // Effect of slip vel on frict. circle


		// Status
		VxVector slipVector;                  // Diff. of theorial v vs ACTUAL v
		float   frictionCoeff;               // Current friction coefficient
		float   skidFactor;                  // How much skidding? 0..1
		float   slip2FCVelFactor;            // Scaling of 'crvSlip2FC' X axis



		
		// Forces
		VxVector GetForceRoadTC(){ return forceRoadTC; }
		VxVector GetForceBodyCC(){ return forceBodyCC; }
		VxVector GetTorqueTC(){ return torqueTC; }
		// Feedback torque is the road reaction torque
		VxVector GetTorqueFeedbackTC(){ return torqueFeedbackTC; }
		VxVector GetTorqueBrakingTC(){ return torqueBrakingTC; }
		VxVector GetTorqueRollingTC(){ return torqueRollingTC; }

		float getCsSlipLen() const { return csSlipLen; }
		void setCsSlipLen(float val) { csSlipLen = val; }
		VxVector getVelWheelCC() const { return velWheelCC; }
		void setVelWheelCC(VxVector val) { velWheelCC = val; }
		VxVector getVelWheelTC() const { return velWheelTC; }
		void setVelWheelTC(VxVector val) { velWheelTC = val; }
		float getRollingCoeff() { return rollingCoeff; }
		void setRollingCoeff(float val) { rollingCoeff = val; }
		
		float getMass();
		void setMass(float val) { mass = val; }


		bool hadContact;
		
		//NxWheelContactData *lastContactData;
		pWheelContactData *lastContactData;// = *wheel2->getContact();



		 virtual void			tick(bool handbrake, float motorTorque, float brakeTorque, float dt);
		 virtual NxActor *		getTouchedActor() const;
		 virtual VxVector			getWheelPos() const;
		 virtual void			setAngle(float angle);
		 virtual float			getRpm() const;
		 
		 VxVector	getGroundContactPos() const;
		 bool hasGroundContact() const ;
		 float			getRadius()const;
		 
		 NxWheelShape * getWheelShape() const { return mWheelShape; }
		 void setWheelShape(NxWheelShape * val) { mWheelShape = val; }

		 void setAxleSpeed(float speed);
		 void setMotorTorque(float torque);
		 void setBreakTorque(float torque);
		 
		 float getSuspensionTravel() const;
		 float getAxleSpeed()const;
		 
		 void _updateVirtoolsEntity(bool position,bool rotation);
		 void _updateAgeiaShape(bool position,bool rotation);

		 bool setSuspensionSpring(const pSpring& spring);
		 pSpring getSuspensionSpring();

		 void setSuspensionTravel(float travel);
		 int  _constructWheel(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation){ return 1;}

		 pWheelContactData *getContact();
		 bool getContact(pWheelContactData&dst);
		 virtual void _tick(float dt);
		 pVehicle * getVehicle() const { return mVehicle; }
		 void setVehicle(pVehicle * val) { mVehicle = val; }

		 //----------------------------------------------------------------
		 //
		 // manager calls 
		 //
		 int onPreProcess();
		 int onPostProcess();
		 
		 void processPostScript();
		 void processPreScript();

		 virtual bool onWheelContact(CK3dEntity* wheelShapeReference, VxVector& contactPoint, VxVector& contactNormal, float& contactPosition, float& normalForce, CK3dEntity* otherShapeReference, int& otherShapeMaterialIndex);
		 virtual void setWheelContactScript(int val);
		 virtual bool onWheelContactModify(int& changeFlags,pWheelContactModifyData *contact);

		 pWheelContactModify *wheelContactModifyCallback;

		 void _createInternalContactModifyCallback();

		 //----------------------------------------------------------------
		 //
		 // physics
		 //
		 float getEndBrakingTorqueForWheel();
		 float getEndTorqueForWheel();
		 float getEndAccForWheel();

		 float getWheelTorque();
		 float getWheelBreakTorque();

		 bool isAxleSpeedFromVehicle();
		 bool isTorqueFromVehicle();
		 
		 void applyTorqueToPhysics();
		 void applyAxleSpeedToPhysics();



		 
		 CK3dEntity * getEntity() const { return entity; }
		 void setEntity(CK3dEntity * val) { entity = val; }
	private:

		NxWheelShape * mWheelShape;
		pVehicle *mVehicle;
		CK3dEntity *entity;
		
};

/** @} */

#endif