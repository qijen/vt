// rsteer.h

#ifndef __PVEHICLE_STEER_H__
#define __PVEHICLE_STEER_H__

#include "vtPhysXBase.h"
#include "pVehicleTypes.h"


class pVehicleSteer
{
  pVehicle *car;

  // Physical
  float lock;              // Maximum angle (radians)
  
  VxVector position;
  float radius;
  float xa;                // Rotation towards the driver's face
  
  // State (output)
  float angle;             // -lock .. +lock
  
  
  // Input
  int  axisInput;          // Controller position (axis)
  
 public:
  pVehicleSteer(pVehicle *car);
  ~pVehicleSteer();
  
  
  float GetAngle(){ return angle; }
  float GetLock(){ return lock; }
  void  SetAngle(float _angle){ angle=_angle; }      // For replays

  void setToDefault();

  void  SetInput(int axisPos);
  float getLock() const { return lock; }
  void setLock(float val) { lock = val; }

  
  void  Integrate();
};

#endif

