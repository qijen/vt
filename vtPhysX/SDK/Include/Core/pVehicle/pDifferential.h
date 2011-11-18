// racer/differential.h

#ifndef __RACER_DIFFERENTIAL_H
#define __RACER_DIFFERENTIAL_H

#include "vtPhysXBase.h"
#include "pDriveline.h"

class pVehicle;
class pWheel;
class pEngine;

class MODULE_API pDifferential : public pDriveLineComp
// A differential that has 3 parts; 1 'input', and 2 'outputs'.
// Actually, all parts work together in deciding what happens.
{
 public:
  enum Type
  {
    FREE=0,                // Free/open differential
    VISCOUS=1,             // Viscous locking differential
    SALISBURY=2            // Grand Prix Legends type clutch locking
  };
  enum Flags
  {
    //LINEAR_LOCKING=1       // Viscous diff is of linear (expensive) type
  };

  void setToDefault();

 protected:

  // Definition (static input)
  int    type;
  int    flags;            // Behavior flags
  float lockingCoeff;     // Coefficient for viscous diff
  // Salisbury diff
  float powerAngle,coastAngle;    // Ramp angles
  int    clutches;                 // Number of clutches in use
  float clutchFactor;             // Scaling the effect of the clutches
  float maxBiasRatioPower;        // Resulting max. bias ratio
  float maxBiasRatioCoast;        // Resulting max. bias ratio

  // Input

  // Torque on the 3 parts
  float torqueIn;
  float torqueOut[2];
  float torqueBrakingOut[2];        // Potential braking torque
  // Inertia of objects
  float inertiaIn;
  float inertiaOut[2];
  // Limited slip differentials add a locking torque
  float torqueLock;

  // State
  float velASymmetric;              // Difference in wheel rotation speed
  float torqueBiasRatio;            // Relative difference in reaction T's
  float torqueBiasRatioAbs;         // Always >=1
  int    locked;                     // Mask of locked ends

  // Output

  // Resulting accelerations
  float accIn,
         accASymmetric;
  float accOut[2];
  float rotVdriveShaft;             // Speed of driveshaft (in)

  pVehicle *car;

 public:
  // Input
  pEngine *engine;
  // Output members
  pWheel2  *wheel[2];

 public:
  pDifferential(pVehicle *car);
  ~pDifferential();

  void   Reset();
  //----------------------------------------------------------------
  //
  // experimental
  //
  
  void   SetTorqueIn(float torque);
  
  float GetTorqueOut(int n);
  float GetBreakTorqueOut(int n);

  void   SetInertiaIn(float inertia);
  void   SetInertiaOut(int n,float inertia);


  //----------------------------------------------------------------
  //
  // standard
  //
  

  float GetAccIn() const { return accIn; }
  float GetAccASymmetric() const { return accASymmetric; }
  float GetAccOut(int n) const { return accOut[n]; }
  // Salisbury info
  float GetTorqueBiasRatio(){ return torqueBiasRatio; }
  float GetMaxBiasRatioPower(){ return maxBiasRatioPower; }
  float GetMaxBiasRatioCoast(){ return maxBiasRatioCoast; }
  float GetPowerAngle(){ return powerAngle; }
  float GetCoastAngle(){ return powerAngle; }
  int    GetClutches(){ return clutches; }
  float GetClutchFactor(){ return clutchFactor; }
  // Other info
  float GetRotationalVelocityIn() const { return rotVdriveShaft; }

  void   Lock(int wheel);
  bool   IsLocked(int wheel)
  { if(locked&(1<<wheel))return true; return false; }

  float CalcLockingTorque();
  void   CalcSingleDiffForces(float torqueIn,float inertiaIn);
  void   CalcForces();
  void   Integrate();
};

#endif
