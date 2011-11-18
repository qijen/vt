// racer/pacejka.h

#ifndef __P_PACEJKA_H__
#define __P_PACEJKA_H__

#include "vtPhysXBase.h"

#define RR_RAD_DEG_FACTOR	57.29578f	// From radians to degrees"
#define RR_RAD2DEG              57.29578f	// A bit more descriptive

#ifndef PI
#define PI	3.14159265358979f
#endif

// Near-zero definitions
#define RR_EPSILON_VELOCITY	0.001		// Wheel velocity


class MODULE_API pPacejka
// A Pacejka calculation engine
// Uses SAE axis system for reference
// (SAE X=Racer Z, SAE Y=Racer X, SAE Z=Racer -Y)
{
 public:
  // Fx longitudinal force
  float a0,a1,a2,a3,a4,a5,a6,a7,a8,
         a9,a10,a111,a112,a12,a13;
  // Fy lateral force
  float b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10;
  // Mz aligning moment
  float c0,c1,c2,c3,c4,c5,c6,
         c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17;

 protected:
  // Input parameters
  float camber,                // Angle of tire vs. surface (in degrees!)
         sideSlip,              // Slip angle (in degrees!)
         slipPercentage,        // Percentage slip ratio (in %)
         Fz;                    // Normal force (in kN!)
  // Output
  float Fx,Fy,Mz;              // Longitudinal/lateral/aligning moment
  float longStiffness,         // Longitudinal tire stiffness
         latStiffness;          // Lateral or cornering stiffness
  VxVector maxForce;            // Max. available tire force (friction ellipse)

 public:
  pPacejka();
  ~pPacejka();

  
  void setToDefault();

  // Attribs
  void SetCamber(float _camber){ camber=_camber*RR_RAD2DEG; }
  void SetSlipAngle(float sa){ sideSlip=sa*RR_RAD2DEG; }
  void SetSlipRatio(float sr){ slipPercentage=sr*100.0f; }
  void SetNormalForce(float force){ Fz=force/1000.0f; }

  // Physics
  void   Calculate();
 protected:
  float CalcFx();
  float CalcFy();
  float CalcMz();
 public:
  float GetFx(){ return Fx; }
  float GetFy(){ return Fy; }
  float GetMz(){ return Mz; }
  // Adjust (used in combined slip)
  void   SetFx(float v){ Fx=v; }
  void   SetFy(float v){ Fy=v; }
  void   SetMz(float v){ Mz=v; }

  // Extras
  float GetMaxLongForce(){ return maxForce.x; }
  float GetMaxLatForce(){ return maxForce.y; }
  // Adjust (used for surface and quick-adjust modifications)
  void   SetMaxLongForce(float v){ maxForce.x=v; }
  void   SetMaxLatForce(float v){ maxForce.x=v; }
  float GetLongitudinalStiffness(){ return longStiffness; }
  float GetCorneringStiffness(){ return latStiffness; }
};

#endif
