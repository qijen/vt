#include "StdAfx.h"
#include "vtPhysXAll.h"

#include "pDifferential.h"
#include "pVehicleAll.h"


void pDifferential::setToDefault()
{

	type = 0;
	SetRatio(3.95f);
	lockingCoeff = 30.0f;
  
}
pDifferential::pDifferential(pVehicle *_car)
  : pDriveLineComp()
{
  SetName("differential");

  car=_car;
  type=0;
  lockingCoeff=0;
  powerAngle=coastAngle=0;
  clutches=0;
  clutchFactor=0;
  flags=0;
  Reset();
}
pDifferential::~pDifferential()
{
}

void pDifferential::Reset()
{
  torqueIn=0;
  torqueOut[0]=torqueOut[1]=0;
  torqueBrakingOut[0]=torqueBrakingOut[1]=0;
  inertiaIn=0;
  inertiaOut[0]=inertiaOut[1]=0;
  accIn=0;
  accOut[0]=accOut[1]=0;
  torqueLock=0;
  rotVdriveShaft=0;
 
  velASymmetric=0;
  locked=0;

  engine=0;
  wheel[0]=wheel[1]=0;
  
  pDriveLineComp::Reset();
}

float pDifferential::GetTorqueOut(int n)
{
	return torqueOut[n];
}
float pDifferential::GetBreakTorqueOut(int n)
{
	
	return torqueBrakingOut[n];

}


void pDifferential::Lock(int wheel)
// Lock a side (0 or 1)
// The wheel calls this function as soon as it sees rotational
// velocity reversal (the wheel from moving forward suddenly starts
// moving backward due to (mostly) braking or rolling resistance)
// In CalcForces(), the side can be unlocked again if the reaction
// torques exceed the (potential) braking torque.
{
  locked|=(1<<wheel);
}


void pDifferential::CalcForces()
// Calculates accelerations of the 3 sides of the differential
// based on incoming torques and inertia's.
// Also unlocks sides (only the wheels can be locked for now)
// if the torques (engine for example) exceeds the braking torque
// (if that side was locked, see Lock())
{
  // Gregor Veble's naming convention

	float j1,j2,j3,            // Inertia of output1/output2/input
         jw,                  // Total inertia of wheels
         jt,                  // Total inertia of all attached components
         jd;                  // Difference of wheel inertia's
  float m0,                  // Locking torque
         m1,                  // Output1 torque
         m2,                  // Output2 torque
         m3;                  // Input torque (from the engine probably)
  float mt,                  // Total net torque
         md;                  // Asymmetric torque (?)
  float det;

  // Check that everything is ok
  if(!engine)return;
  if(wheel[0]==0||wheel[1]==0)return;
  
  // Note that no ratios are used here; the input and outputs
  // are geared 1:1:1. This makes the formulas easier. To add
  // a differential ratio, the other functions for the input torques
  // take care of this.

  // Retrieve current effective inertia
  // The base is the driveshaft; the accelerations and torques are
  // related to its position in the drivetrain.

#ifdef ND_DRIVELINE
  //inertiaIn=engine->GetInertiaAtDifferential();
  //inertiaOut[0]=wheel[0]->GetRotationalInertia()->x;
  //inertiaOut[1]=wheel[1]->GetRotationalInertia()->x;

#else
  inertiaIn=1;
  inertiaOut[0]=1;
  inertiaOut[1]=1;
#endif
  
  // Retrieve torques at all ends
  // Notice that inside the diff, there can be a ratio. If this is 2 for
  // example, the driveshaft will rotate twice as fast as the wheel axles.

#ifdef ND_DRIVELINE
  torqueIn=engine->GetTorqueAtDifferential();
#else
  torqueIn=1;
#endif
  
 
  
  //torqueOut[0]=wheel[0]->GetTorqueFeedbackTC().x;
  //torqueOut[1]=wheel[1]->GetTorqueFeedbackTC().x;
  
  
  // Retrieve potential braking torque; if bigger than the reaction
  // torque, the output will become unlocked. If not, the output is
  // locked.
  // Note that the braking torque already points in the opposite
  // direction of the output (mostly a wheel) rotation. This is contrary
  // to Gregor Veble's approach, which only calculates the direction
  // in the formulae below.
  
 
	torqueBrakingOut[0]=wheel[0]->GetTorqueBrakingTC().x;
	torqueBrakingOut[1]=wheel[1]->GetTorqueBrakingTC().x;
  

#ifdef LTRACE
	qdbg("  torqueIn=%f, torqueOut0=%f, 1=%f\n",torqueIn,torqueOut[0],torqueOut[1]);
#endif

  // Proceed to Gregor's naming convention and algorithm
  // Determine locking
  switch(type)
  {
    case FREE:
      // No locking; both wheels run free
      m0=0;
      break;
    case VISCOUS:
//      velASymmetric=wheel[1]->GetRotationV()-wheel[0]->GetRotationV();
      m0=-lockingCoeff*velASymmetric;
#ifdef LTRACE
qdbg("  velASymm=%f, lockCoeff=%f => m0=%f\n",velASymmetric,lockingCoeff,m0);
#endif
      break;
    case SALISBURY:
      // Salisbury diff locks based on the ratio of reaction torques on
      // the tires.
      // Calculate torque bias ratio
      if(fabs(torqueOut[1])>D3_EPSILON)
        torqueBiasRatio=torqueOut[0]/torqueOut[1];
      else if(fabs(torqueOut[0])>D3_EPSILON)
        torqueBiasRatio=torqueOut[1]/torqueOut[0];
      else
        torqueBiasRatio=1;          // Both wheels doing pretty much nothing
      // Get a number which always has a ratio>1
      if(torqueBiasRatio<1.0)torqueBiasRatioAbs=1.0f/torqueBiasRatio;
      else                   torqueBiasRatioAbs=torqueBiasRatio;
      // Is the ratio exceeded?
//xxxx continue here
      if(torqueIn>0)
      {
        // Power
        if(torqueBiasRatioAbs>maxBiasRatioPower);
      }
      m0=0; break;
    default:
      m0=0; break;
  }
  m3=torqueIn;            // Entails engine braking already
#ifdef LTRACE
qdbg("  torqueIn=%f, locked=%d\n",m3,locked);
#endif

  j1=inertiaOut[0];
  j2=inertiaOut[1];
  j3=inertiaIn;
  jw=j1+j2;
  jt=jw+j3;
  jd=j1-j2;                // Inertia difference (of outputs)
  // Calculate determinant of 2x2 matrix
  det=4.0f*j1*j2+j3*jw;

  m3=torqueIn;
  switch(locked)
  {
    case 0:                // No outputs locked
      m1=torqueOut[0]+torqueBrakingOut[0];
      m2=torqueOut[1]+torqueBrakingOut[1];
//qdbg("  m1=%f, m2=%f\n",m1,m2);
      break;
    case 1:
      // Output 0 is locked, output 1 is unlocked
      m2=torqueOut[1]+torqueBrakingOut[1];
      m1=(m2*j3-2.0f*m3*j2-m0*(2.0f*j2+j3))/(4.0f*j2+j3);
      if(fabs(m1-torqueOut[0])>fabs(torqueBrakingOut[0]))
        locked=0;
      break;
    case 2:
      // Output 1 is locked, output 0 is unlocked
      m1=torqueOut[0]+torqueBrakingOut[0];
      m2=(m1*j3-2.0f*m3*j1+m0*(2.0f*j1+j3))/(4.0f*j1+j3);
      if(fabs(m2-torqueOut[1])>fabs(torqueBrakingOut[1]))
        locked=0;
      break;
    case 3:
      // Both outputs locked
      m1=-m3/2.0f;
      m2=m1;
      m0=0;
      if(fabs(m1-torqueOut[0])>fabs(torqueBrakingOut[0]))
        locked^=1;
      if(fabs(m2-torqueOut[1])>fabs(torqueBrakingOut[1]))
        locked^=2;
      break;
    default:
      //qerr("Bug: pDifferential locked not in 0..3 (%d)",locked);
      m1=m2=0;
      break;
  }
  mt=m1+m2+m3;
  md=m2-m1+m0;

  // Calculate asymmetric acceleration
  accASymmetric=md/jw;
#ifdef ND_OLD_NAMES
  accASymmetric=(torqueOut[1]-torqueOut[0]+torqueLock)/
    (inertiaOut[0]+inertiaOut[1]);
#endif

  // Calculate total acceleration based on all torques
  // (which is in fact the driveshaft rotational acceleration)
  accIn=mt/jt;
#ifdef ND_OLD_NAMES
  accIn=(torqueIn+torqueOut[0]+torqueOut[1])/
        (inertiaIn+inertiaOut[0]+inertiaOut[1]);
#endif
  
  // Derive from these the acceleration of the 2 output parts
  accOut[1]=accIn+accASymmetric;
  accOut[0]=accIn-accASymmetric;
  
  // Add torque to body because of the accelerating drivetrain
  // This gives a bit of the GPL effect where your car rolls when
  // you throttle with the clutch disengaged.
/*  
  float tr=car->GetEngine()->GetTorqueReaction();
  if(tr>0)
  {
    DVector3 torque(0,0,accIn*inertiaIn*tr);
//qdbg("torque.z=%f\n",torque.z);
    car->GetBody()->AddBodyTorque(&torque);
  }
*/

#ifdef LTRACE
	qdbg("inertia: I%f, O %f, %f\n",inertiaIn,inertiaOut[0],inertiaOut[1]);
	qdbg("torqueBraking: I%f, O %f, %f\n",0,torqueBrakingOut[0],
	torqueBrakingOut[1]);
	qdbg("torque: I%f, O %f, %f, locking %f\n",m3,m1,m2,m0);
	qdbg("Vel: wheel0=%f, wheel1=%f\n",wheel[0]->GetRotationV(),
	wheel[1]->GetRotationV());
	qdbg("Acc: asym %f, in %f, out %f, %f\n",accASymmetric,accIn,
	accOut[0],accOut[1]);
#endif
}


float pDifferential::CalcLockingTorque()
// Calculates the locking torque of the differential
{
  float m0;

  switch(type)
  {
    case FREE:
      // No locking; both wheels run free
      m0=0;
      break;
    case VISCOUS:
      //velASymmetric=wheel[1]->GetRotationV()-wheel[0]->GetRotationV();
      m0=-lockingCoeff*velASymmetric;
#ifdef LTRACE
qdbg("  velASymm=%f, lockCoeff=%f => m0=%f\n",velASymmetric,lockingCoeff,m0);
#endif
      break;
    case SALISBURY:
      // Salisbury diff locks based on the ratio of reaction torques on
      // the tires.
      // Calculate torque bias ratio
      if(fabs(torqueOut[1])>D3_EPSILON)
        torqueBiasRatio=torqueOut[0]/torqueOut[1];
      else if(fabs(torqueOut[0])>D3_EPSILON)
        torqueBiasRatio=torqueOut[1]/torqueOut[0];
      else
        torqueBiasRatio=1;          // Both wheels doing pretty much nothing
      // Get a number which always has a ratio>1
      if(torqueBiasRatio<1.0)torqueBiasRatioAbs=1.0f/torqueBiasRatio;
      else                   torqueBiasRatioAbs=torqueBiasRatio;
      // Is the ratio exceeded?
//xxxx continue here
      if(torqueIn>0)
      {
        // Power
        if(torqueBiasRatioAbs>maxBiasRatioPower);
      }
      m0=0; break;
    default:
      //qwarn("pDifferential:CalcLockingTorque(); unknown diff type");
      m0=0; break;
  }
  return m0;
}

void pDifferential::CalcSingleDiffForces(float torqueIn,float inertiaIn)
// Special version in case there is only 1 differential.
// Differences with regular operating:
// - 'torqueIn' is directly passed in from the driveline root (engine).
// - 'inertiaIn' is directly passed from the driveline (engine's eff. inertia)
// Calculates accelerations of the 3 sides of the differential
// based on incoming torques and inertia's.
// Also unlocks sides (only the wheels can be locked for now)
// if the engine torques exceeds the reaction/braking torque
// (if that side was locked, see Lock())
{
  // Gregor Veble's naming convention
  float j1,j2,j3,            // Inertia of output1/output2/input
         jw,                  // Total inertia of wheels
         jt,                  // Total inertia of all attached components
         jd;                  // Difference of wheel inertia's
  float m0,                  // Locking torque
         m1,                  // Output1 torque
         m2,                  // Output2 torque
         m3;                  // Input torque (from the engine probably)
  float mt,                  // Total net torque
         md;                  // Asymmetric torque (?)
  float det;

  // Check that everything is ok
  if(!engine)return;
  if(wheel[0]==0||wheel[1]==0)return;
  
  // Note that no ratios are used here; the input and outputs
  // are geared 1:1:1. This makes the formulas easier. To add
  // a differential ratio, the other functions for the input torques
  // take care of this.
#ifdef LTRACE
qdbg("RDiff:CalcSingleDiffForces(%.2f)\n",torqueIn);
#endif
  // Retrieve current effective inertia
  // The base is the driveshaft; the accelerations and torques are
  // related to its position in the drivetrain.
  
	//inertiaIn=engine->GetInertiaAtDifferential();
	//float inertiaIn2 =engine->GetInertiaAtDifferential();



	inertiaOut[0]=wheel[0]->GetInertia();
	inertiaOut[1]=wheel[1]->GetInertia();
 
  // Retrieve torques at all ends
  // Notice that inside the diff, there can be a ratio. If this is 2 for
  // example, the drive shaft will rotate twice as fast as the wheel axles.
  
	//float torqueIn2 =engine->GetTorqueAtDifferential();



	torqueOut[0]=wheel[0]->torqueFeedbackTC.x;
	torqueOut[1]=wheel[1]->torqueFeedbackTC.x;



  // Retrieve potential braking torque; if bigger than the reaction
  // torque, the output will become unlocked. If not, the output is
  // locked.
  // Note that the braking torque already points in the opposite
  // direction of the output (mostly a wheel) rotation. This is contrary
  // to Gregor Veble's approach, which only calculates the direction
  // in the formula below.
  torqueBrakingOut[0]=wheel[0]->torqueBrakingTC.x;
  torqueBrakingOut[1]=wheel[1]->torqueBrakingTC.x;


  // Proceed to Gregor's naming convention and algorithm
  // Determine locking
  m0=CalcLockingTorque();



#ifdef OBS
  switch(type)
  {
    case FREE:
      // No locking; both wheels run free
      m0=0;
      break;
    case VISCOUS:
      velASymmetric=wheel[1]->GetRotationV()-wheel[0]->GetRotationV();
      m0=-lockingCoeff*velASymmetric;
#ifdef LTRACE
qdbg("  velASymm=%f, lockCoeff=%f => m0=%f\n",velASymmetric,lockingCoeff,m0);
#endif
      break;
    case SALISBURY:
      // Salisbury diff locks based on the ratio of reaction torques on
      // the tires.
      // Calculate torque bias ratio
      if(fabs(torqueOut[1])>D3_EPSILON)
        torqueBiasRatio=torqueOut[0]/torqueOut[1];
      else if(fabs(torqueOut[0])>D3_EPSILON)
        torqueBiasRatio=torqueOut[1]/torqueOut[0];
      else
        torqueBiasRatio=1;          // Both wheels doing pretty much nothing
      // Get a number which always has a ratio>1
      if(torqueBiasRatio<1.0)torqueBiasRatioAbs=1.0f/torqueBiasRatio;
      else                   torqueBiasRatioAbs=torqueBiasRatio;
      // Is the ratio exceeded?
//xxxx continue here
      if(torqueIn>0)
      {
        // Power
        if(torqueBiasRatioAbs>maxBiasRatioPower);
      }
      m0=0; break;
    default:
      m0=0; break;
  }
#endif

  m3=torqueIn;            // Entails engine braking already

#ifdef LTRACE
qdbg("  torqueIn=%f, locked=%d\n",m3,locked);
#endif

  j1=inertiaOut[0];
  j2=inertiaOut[1];
  j3=inertiaIn;
  jw=j1+j2;
  jt=jw+j3;
  jd=j1-j2;                // Inertia difference (of outputs)
  // Calculate determinant of 2x2 matrix
  det=4.0f*j1*j2+j3*jw;

  m3=torqueIn;
  switch(locked)
  {
    case 0:                // No outputs locked
      m1=torqueOut[0]+torqueBrakingOut[0];
      m2=torqueOut[1]+torqueBrakingOut[1];//qdbg("  m1=%f, m2=%f\n",m1,m2);
      break;
    case 1:
      // Output 0 is locked, output 1 is unlocked
      m2=torqueOut[1]+torqueBrakingOut[1];
      m1=(m2*j3-2.0f*m3*j2-m0*(2.0f*j2+j3))/(4.0f*j2+j3);
      if(fabs(m1-torqueOut[0])>fabs(torqueBrakingOut[0]))
        locked=0;
      break;
    case 2:
      // Output 1 is locked, output 0 is unlocked
      m1=torqueOut[0]+torqueBrakingOut[0];
      m2=(m1*j3-2.0f*m3*j1+m0*(2.0f*j1+j3))/(4.0f*j1+j3);
      if(fabs(m2-torqueOut[1])>fabs(torqueBrakingOut[1]))
        locked=0;
      break;
    case 3:
      // Both outputs locked
      m1=-m3/2.0f;
      m2=m1;
      m0=0;
      if(fabs(m1-torqueOut[0])>fabs(torqueBrakingOut[0]))
        locked^=1;
      if(fabs(m2-torqueOut[1])>fabs(torqueBrakingOut[1]))
        locked^=2;
      break;
    default:
      //qerr("Bug: pDifferential locked not in 0..3 (%d)",locked);
      m1=m2=0;
      break;
  }
  mt=m1+m2+m3;
  md=m2-m1+m0;

  // Calculate asymmetric acceleration
  accASymmetric=md/jw;


  // Calculate total acceleration based on all torques
  // (which is in fact the driveshaft rotational acceleration)
  accIn=mt/jt;


  
  // Derive from these the acceleration of the 2 output parts
  accOut[1]=accIn+accASymmetric;
  accOut[0]=accIn-accASymmetric;
  
  // Add torque to body because of the accelerating drivetrain
  // This gives a bit of the GPL effect where your car rolls when
  // you throttle with the clutch disengaged.
  /*
  float tr=car->GetEngine()->GetTorqueReaction();
  if(tr>0)
  {
    DVector3 torque(0,0,accIn*inertiaIn*tr);
//qdbg("torque.z=%f\n",torque.z);
    car->GetBody()->AddBodyTorque(&torque);
  }
  */

#ifdef LTRACE
	qdbg("inertia: I%f, O %f, %f\n",inertiaIn,inertiaOut[0],inertiaOut[1]);
	qdbg("torqueBraking: I%f, O %f, %f\n",0,torqueBrakingOut[0],torqueBrakingOut[1]);
	qdbg("torque: I%f, O %f, %f, locking %f\n",m3,m1,m2,m0);
	qdbg("Vel: wheel0=%f, wheel1=%f\n",wheel[0]->GetRotationV(),wheel[1]->GetRotationV());
	qdbg("Acc: asym %f, in %f, out %f, %f\n",accASymmetric,accIn,accOut[0],accOut[1]);
#endif


}

void pDifferential::Integrate()
// Maintain differential objects rotations
{
  float rotAds;
  
  // Check that everything is ok
  if(!engine)return;
  if(wheel[0]==0||wheel[1]==0)return;
  
  // Driveshaft rotation
  rotAds=accIn;


  if (car->getProcessOptions() & pVPO_Wheel_DiffDirect)
  {
	  rotVdriveShaft+=rotAds*lastStepTimeSec;
  }
   rotVdriveShaft=(wheel[0]->rotationV.x+wheel[1]->rotationV.x)/2.0f;

}


/*
bool pDifferential::LoadState(QFile *f)
{
	RDriveLineComp::LoadState(f);
	f->Read(&rotVdriveShaft,sizeof(rotVdriveShaft));
	f->Read(&locked,sizeof(locked));
	return TRUE;
}
bool pDifferential::SaveState(QFile *f)
{
	RDriveLineComp::SaveState(f);
	f->Write(&rotVdriveShaft,sizeof(rotVdriveShaft));
	f->Write(&locked,sizeof(locked));
	return TRUE;
}

bool pDifferential::Load(QInfo *info,cstring path)
// Read settings from the car file
{
	char buf[256];

	sprintf(buf,"%s.type",path);
	type=info->GetInt(buf);
	// Read the ratio
	sprintf(buf,"%s.ratio",path);
	if(info->PathExists(buf))
	{
		SetRatio(info->GetFloat(buf));
		//qdbg("Diff ratio (%s) = %.2f\n",buf,ratio);
	} else
	{
		// Backward compatible (v0.4.9 and before); use old gearbox setting
		qwarn("No differential ratio in car.ini; using gearbox.end_ratio instead");
		qwarn("(this is obsolete; use differential.diff<x>.ratio from now on)");
		SetRatio(info->GetFloat("gearbox.end_ratio"));
	}
	sprintf(buf,"%s.inertia",path);
	SetInertia(info->GetFloat(buf));

	// Type-specific parameters
	if(type==VISCOUS)
	{
		// Viscous locking differential
		sprintf(buf,"%s.locking_coeff",path);
		lockingCoeff=info->GetFloat(buf);
	} else if(type==SALISBURY)
	{
		// Salisbury (known from GPL)
		sprintf(buf,"%s.power_angle",path);
		powerAngle=info->GetFloat(buf)/RR_RAD2DEG;
		sprintf(buf,"%s.coast_angle",path);
		coastAngle=info->GetFloat(buf)/RR_RAD2DEG;
		sprintf(buf,"%s.clutches",path);
		clutches=info->GetInt(buf);
		sprintf(buf,"%s.clutch_factor",path);
		clutchFactor=info->GetFloat(buf);

		// Calculate resulting constants
		maxBiasRatioPower=cos(powerAngle)*(1.0+2.0*clutches)*clutchFactor;
		maxBiasRatioCoast=cos(coastAngle)*(1.0+2.0*clutches)*clutchFactor;
	}
	return TRUE;
}
*/