#ifndef __P_GEARBOX_H__
#define __P_GEARBOX_H__

#include "vtPhysXBase.h"
#include "XString.h"
#include "pDriveline.h"

#include "pLinearInterpolation.h"


class MODULE_API pGearBox : public pDriveLineComp
{
 public:
  enum Max
  { MAX_GEAR=10                         // #gears possible
  };
  enum Flags
  {
    AUTOMATIC=1                         // Auto shifting
  };

  //----------------------------------------------------------------
  //
  // public access 
  //
  pLinearInterpolation gearRatios;
  pLinearInterpolation gearTensors;
  
  pLinearInterpolation& getGearTensors() { return gearTensors; }
  void setGearTensors(pLinearInterpolation val) { gearTensors = val; }

  pLinearInterpolation& getGearRatios(){ return gearRatios; }
  void setGearRatios(pLinearInterpolation val) { gearRatios = val; }
  
  void setToDefault();

  int getGears() const { return gears; }
  void setGears(int val) { gears = val; }
  
  float getShiftUpRPM() const { return shiftUpRPM; }
  void setShiftUpRPM(float val) { shiftUpRPM = val; }
  float getShiftDownRPM() const { return shiftDownRPM; }
  void setShiftDownRPM(float val) { shiftDownRPM = val; }

  int& getFlags(){return flags;};
  void setFlags(int val);
  
  int getTimeToDeclutch() const { return timeToDeclutch; }
  void setTimeToDeclutch(int val) { timeToDeclutch = val; }
  
  int getTimeToClutch() const { return timeToClutch; }
  void setTimeToClutch(int val) { timeToClutch = val; }
 public:
  // Semi-fixed properties
  pVehicle    *car;                         // The car to which we belong

  // Physical attributes
  int    flags;
  
  float gearRatio[MAX_GEAR];
  int    gears;
  
  float gearInertia[MAX_GEAR];          // Rotational inertia per gear
  // Shifting (static)
  int    timeToDeclutch;                 // Auto-shifting time
  int timeToClutch;
  
  float shiftUpRPM;                     // Automatic transmissions
  float shiftDownRPM;  
   
  

  // State (dynamic output)
  int    curGear;                        // Selected gear
  int    autoShiftStart;                 // Time of auto shift initiation
  int    futureGear;                     // Gear to shift to

 public:
  pGearBox(pVehicle *car);
  ~pGearBox();

  void Reset();                          // Reset all vars

  // Definition
  
  /*
  bool Load(QInfo *info,cstring path=0);
  bool LoadState(QFile *f);
  bool SaveState(QFile *f);
  */

  float endRatio;                       // Final drive ratio

  // Attribs
#ifdef OBS
  // Engine torque generation
  float GetMinTorque(float rpm);
  float GetMaxTorque(float rpm);
  float GetTorqueAtDifferential();
#endif

  //float GetEngineInertia(){ return inertiaEngine; }
  float GetGearInertia(){ return gearInertia[curGear]; }
  float GetInertiaAtDifferential();
  float GetInertiaForWheel(pWheel2 *w);
  float GetTorqueForWheel(pWheel2 *w);


  int     GetGears(){ return gears; }
  int     GetGear(){ return curGear; }
  float  GetGearRatio(int n);
  //float GetEndRatio();
  void    SetGear(int n);
  XString GetGearName(int gear);
  bool    IsNeutral(){ if(curGear==0)return true; return false; }

  void processFutureGear();
 
  // Input
  void SetInput(int ctlThrottle,int ctlClutch);
  
  // Graphics
  void Paint();

  // Physics
  void CalcForces();
  void Integrate();
  void OnGfxFrame();
};

#endif
