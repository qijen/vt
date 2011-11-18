// racer/driveline.h

#ifndef __P_DRIVELINE_H__
#define __P_DRIVELINE_H__

#include "vtPhysXBase.h"
#include "XString.h"

/*
#ifdef RR_FIXED_TIMESTEP
#define RR_TIMESTEP       0.01f
#else
#define RR_TIMESTEP       0.01f
#endif
*/

#define D3_EPSILON   (0.00001f)

// Generic constants
#ifndef PI
#define PI 3.14159265358979
#endif


// Clutch must be smoothed out a lot when applying starts
#define DEFAULT_CLUTCH_LINEARITY 0.3f

// Threshold at which force an unlock of engine and gearbox.
// This can happen if the driver just throws in a gear,
// without clutching (and if autoclutch is off).
// We should penalize this with a gear scratch sound and
// damage.
#define DELTA_VEL_THRESHOLD 1.0f

class pDriveLine;

class MODULE_API pDriveLineComp
// Driveline component base class
{
 protected:
  // Administration
  XString name;                   // For easier debugging
  pDriveLineComp *parent,         // Bi-directional tree
                 *child[2];
  pDriveLine *driveLine;          // Part of which driveline


 private:
  // Static data
  float inertia;                 // Inertia of this component
  float ratio;                   // Gearing ratio (gearbox/differential)
  float invRatio;                // 1/ratio (optimized multiplication)

  // Semi-static data (recalculated when gear is changed)
  float effectiveInertiaDownStream;  // Inertia of component + children
  float cumulativeRatio;         // Ratio at this point of the driveline

  // Dynamic data
 protected:
  float tReaction,               // Reaction torque
         tBraking,                // Braking torque (also a reaction torque)
         tEngine;                 // Torque from the engine side
  // State
  float rotV,                    // Rotational velocity
         rotA;                    // Rotational acceleration

 public:
  pDriveLineComp();
  virtual ~pDriveLineComp();

  // Attribs
  XString GetName(){ return name.CStr(); }
  void    SetName(XString s){ name=s; }
  float GetInertia(){ return inertia; }
  void   SetInertia(float i){ inertia=i; }
  float GetRatio(){ return ratio; }
  void   SetRatio(float r);
  float GetInverseRatio(){ return invRatio; }
  float GetEffectiveInertia(){ return effectiveInertiaDownStream; }
  float GetCumulativeRatio(){ return cumulativeRatio; }
  pDriveLine *GetDriveLine();
  void        SetDriveLine(pDriveLine *dl){ driveLine=dl; }
  pDriveLineComp *GetParent(){ return parent; }
  pDriveLineComp *GetChild(int n);
  float GetReactionTorque(){ return tReaction; }
  float GetBrakingTorque(){ return tBraking; }
  float GetEngineTorque(){ return tEngine; }

  float GetRotationVel(){ return rotV; }
  void   SetRotationVel(float v){ rotV=v; }
  float GetRotationAcc(){ return rotA; }
  void   SetRotationAcc(float a){ rotA=a; }

  // Attaching to other components
  void AddChild(pDriveLineComp *comp);
  void SetParent(pDriveLineComp *comp);

  // Reset for new use
  virtual void Reset();


  // Precalculation
  void CalcEffectiveInertia();
  void CalcCumulativeRatio();

  // Physics
  void CalcReactionForces();
  void CalcEngineForces();
  virtual void CalcForces();
  virtual void CalcAccelerations();
  virtual void Integrate();


  // Debugging
  void DbgPrint(int depth,XString s);
};

class MODULE_API pDriveLine
// A complete driveline
// Includes the clutch and the handbrakes
{
 public:
  pDriveLineComp *root;           // Root of driveline tree; the engine
  pDriveLineComp *gearbox;        // The gearbox is always there
  pVehicle *car;

  // Static driveline data
  int diffs;                      // Number of differentials

  // Semi-static driveline data (changes when gear is changed)
  float preClutchInertia,        // Total inertia before clutch (engine)
         postClutchInertia,       // After clutch (gearbox, diffs)
         totalInertia;            // Pre and post clutch inertia

  // Clutch
  float clutchApplication,       // 0..1 (how much clutch is applied)
         clutchLinearity,         // A linear clutch was too easy to stall
         clutchMaxTorque,         // Clutch maximum generated torque (Nm)
         clutchCurrentTorque;     // Clutch current torque (app*max)
  bool   autoClutch;              // Assist takes over clutch?

  // Handbrakes
  float handbrakeApplication;    // 0..1 (control)

  // Dynamic
  bool   prepostLocked;           // Engine is locked to rest of drivetrain?
  float tClutch;                 // Directed clutch torque (+/-clutchCurT)

 public:
  pDriveLine(pVehicle *car);
  ~pDriveLine();

  // Attribs
  void SetRoot(pDriveLineComp *comp);
  pDriveLineComp *GetRoot(){ return root; }
  pDriveLineComp *GetGearBox(){ return gearbox; }
  void SetGearBox(pDriveLineComp *c){ gearbox=c; }
  bool IsSingleDiff(){ return diffs==1; }
  int  GetDifferentials(){ return diffs; }
  void SetDifferentials(int n){ diffs=n; }
  bool IsPrePostLocked(){ return prepostLocked; }
  void LockPrePost(){ prepostLocked=true; }
  void UnlockPrePost(){ prepostLocked=false; }
  float GetClutchTorque(){ return tClutch; }

  // Precalculation
  void CalcCumulativeRatios();
  void CalcEffectiveInertiae();
  void CalcPreClutchInertia();
  void CalcPostClutchInertia();

  // Clutch
  float GetClutchApplication(){ return clutchApplication; }
  float GetClutchMaxTorque(){ return clutchMaxTorque; }
  float GetClutchCurrentTorque(){ return clutchCurrentTorque; }
  void SetClutchApplication(float app);
  bool IsAutoClutchActive(){ return autoClutch; }
  void EnableAutoClutch(){ autoClutch=true; }
  void DisableAutoClutch(){ autoClutch=false; }

  // Handbrakes
  float GetHandBrakeApplication(){ return handbrakeApplication; }

  // Definition

  // Input
  void SetInput(int ctlClutch,int ctlHandbrake);

  // Physics
  void Reset();
  void CalcForces();
  void CalcAccelerations();
  void Integrate();

  // Debugging
  void DbgPrint(XString& s);
};

#endif

