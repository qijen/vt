#include "vtPhysXAll.h"

#include "pEngine.h"
#include <xDebugTools.h>


// Local trace?
//#define LTRACE

#define DEF_SIZE  .25
#define DEF_MAXRPM 5000
#define DEF_MAXPOWER 100
#define DEF_FRICTION 0
#define DEF_MAXTORQUE 340          // ~ F1 Jos Verstappen

#define DEF_TORQUE    100          // In case no curve is present

//#define LTRACE


// If USE_HARD_REVLIMIT is used, any rpm above the max. RPM
// returns a 0 engine torque. Although this works, it gives quite
// hard rev limits (esp. when in 1st gear). Better is to supply
// a curve which moves down to 0 torque when rpm gets above maxRPM,
// so a more natural (smooth) balance is obtained (and turn
// this define off)
//#define USE_HARD_REVLIMIT

pEngine::pEngine(pVehicle *_car)
  : pDriveLineComp()
{
  SetName("engine");
  car=_car;
  driveLine=car->GetDriveLine();
  crvTorque=0;
  Init();
}

pEngine::~pEngine()
{
  /*
  if(quad)gluDeleteQuadric(quad);
  QDELETE(crvTorque);
  */
}

void pEngine::Init()
// Init all member variables
{

  flags=0;
  size=DEF_SIZE;
  
  mass=0;
  torqueReaction=0;
  maxRPM=DEF_MAXRPM;
  idleRPM=500;
  stallRPM=400;
  startRPM=500;
  autoClutchRPM=idleRPM*1.5f;
  //autoClutch=0;
  starterTorque=50;
  idleThrottle=0.1f;
  throttleRange=1000.0-idleThrottle*1000.0;
  friction=0;
  brakingCoeff=0;
  position.Set(0,0,0);

  maxTorque=0;
  throttle=0;
  brakes=0;
  
  //QDELETE(crvTorque);

  Reset();
}

void pEngine::Reset()
// Reset engine before usage (after Shift-R for example)
{
  torqueReaction=0;

  pDriveLineComp::Reset();

  // No automatic clutch engaged
  //flags&=~AUTOCLUTCH_ACTIVE;
  flags|=AUTOCLUTCH_ACTIVE;

  // Start engine (or not) and make it stable; AFTER RDLC:Reset(), since that
  // sets the rotational velocity to 0.
  if(flags&START_STALLED)
  {
    // Start with the engine off
    EnableStall();
    SetRPM(0);
  } else
  {
    // Pre-start engine
    DisableStall();
    SetRPM(idleRPM);
  }
}

/**********
* Attribs *
**********/
void pEngine::SetRPM(float rpm)
{
  // Convert to radians and seconds
  rotV=rpm*2*PI/60.0f;
}


/*************
* Definition *
*************/
void pEngine::PreCalculate()
// Precalculate known numbers to speed up calculations during gameplay
{
  float minT,maxT;

  // Calculate idle throttle based on desired idle RPM
  maxT=GetMaxTorque(idleRPM);
  minT=GetMinTorque(idleRPM);
  if(fabs(minT)<D3_EPSILON)
  {
    // There's no engine braking, hm
    //qwarn("pEngine: engine braking torque at rpm=%.f is 0;"" that's not realistic",idleRPM);
    idleThrottle=0;
  } else
  {
    // Calculate throttle at which resulting engine torque would be 0
    idleThrottle=-minT/(maxT-minT);
  }

  // Calculate effective throttle range so that the throttle axis
  // can be more quickly converted to a 0..1 number.
  // This also includes converting from the controllers integer 0..1000
  // range to 0..1 floating point.
  throttleRange=(1.0-idleThrottle)/1000.0;

  // Reset engine
  Reset();
//qdbg("  idleThrottle=%f, idleRPM=%f\n",idleThrottle,idleRPM);
//qdbg("  brakingCoeff=%f, minT=%f/maxT=%f\n",brakingCoeff,minT,maxT);
}


/*
void pEngine::Paint()
{
#ifdef OBS_NO_MOTOR
  car->SetDefaultMaterial();
  
  glPushMatrix();
  
  // Location
  glTranslatef(position.GetX(),position.GetY(),position.GetZ());
  
#ifdef OBS
  // Stats
  char buf[80];
  float p;
  // Calc generated power in Watts (J/s)
  // Note also that 1hp = 746W
  p=2*PI*(rpm/60)*torque;
  sprintf(buf,"RPM=%.f, Torque=%.f Nm, Power %.3fhp, Gear-ratio %.2f",
    rpm,torque,p/746,gearRatio[curGear]*endRatio);
  GfxText3D(position.GetX(),position.GetY(),position.GetZ(),buf);
#endif
  
  // Discs are painted at Z=0
  //glRotatef(90,0,1,0);
  
  // Center point for quad
  //glTranslatef(0,0,-size/2);
  
  float colMotor[]={ .8,.8,.82 };
  int   slices=6;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,colMotor);
  // Draw a box (=cylinder with 4 slices)
  gluCylinder(quad,size,size,size,slices,1);
  
  // Caps
  gluQuadricOrientation(quad,GLU_INSIDE);
  gluDisk(quad,0,size,slices,1);
  gluQuadricOrientation(quad,GLU_OUTSIDE);
  glTranslatef(0,0,size);
  gluDisk(quad,0,size,slices,1);
  
  glPopMatrix();
#endif
}
*/

/********
* Input *
********/
void pEngine::SetInput(int ctlThrottle)
// Controller input from 0..1000
// Calculates resulting throttle.
{
  // Convert throttle to 0..1 (mind idle throttle offset)
  throttle=((float)ctlThrottle)*throttleRange+idleThrottle;
  if(throttle>1)throttle=1;
  else if(throttle<0)throttle=0;

//qdbg("pEngine:SetInput(); ctlT=%d, throttle=%f\n",ctlThrottle,throttle);
//qdbg("  idleThrottle=%f, idleRPM=%f\n",idleThrottle,idleRPM);
//qdbg("pEngine:SetInput(ctlClutch=%d)\n",ctlClutch);
#ifdef ND_AUTO_CLUTCH
  // Only update clutch value when the car is not auto-shifting
  if(!autoShiftStart)
  {
    clutch=((float)ctlClutch)/1000.0f;
    if(clutch>1)clutch=1;
    else if(clutch<0)clutch=0;
  }
#else
  // Pass clutch on to driveline
  //driveLine->SetClutchApplication(((float)ctlClutch)/1000.0f);
  // Directly set clutch
  //clutch=((float)ctlClutch)/1000.0f;
  //if(clutch>1)clutch=1;
  //else if(clutch<0)clutch=0;
#endif
}

/********************
* Torque generation *
********************/
float pEngine::GetMaxTorque(float rpm)
// Returns the maximum torque generated at 'rpm'
{
#ifdef USE_HARD_REVLIMIT
  // Clip hard at max rpm (return 0 torque if rpm gets too high)
  if(rpm>maxRPM)
    return 0;
#endif
  if(!crvTorque)
  {
    // No curve available, use a less realistic constant torque
    return DEF_TORQUE;
  } else
  {
    float t;
    // Find normalized torque in RPM-to-torque curve
//    t=(float)crvTorque->GetValue(rpm);
//qdbg("pEngine: rpm=%.2f => T=%.2f, (curve)maxTorque=%.2f\n",rpm,t,maxTorque);
    return t*maxTorque;
  }
}
// Returns the minimum torque generated at 'rpm' (engine braking).
float pEngine::GetMinTorque(float rpm)

{
  // If 0% throttle, this is the minimal torque which is generated
  // by the engine
  return -brakingCoeff*rpm/60.0f;
}


/**************
* Calc Forces *
**************/
void pEngine::CalcForces()
{
  float minTorque,maxTorque;
  float rpm=GetRPM();
  static int starterDelay;          // Temp crap to avoid multiple starter smps

#ifdef LTRACE
	qdbg("pEngine::CalcForces()\n");
	qdbg("  rpm=%f, throttle=%.2f\n",rpm,throttle);
#endif

  if(starterDelay>0)
    starterDelay--;

  // Check starter; this section assumes CalcForces() is called
  // only once per simulation step. Although this may not be
  // the case in the future, it should not harm that much.
  if(IsStalled())
  {
    // There's only some engine braking
    // Note this skips calculating min/maxTorque
    tEngine=GetMinTorque(rpm);
//qdbg("Stalled; check starter=%d\n",RMGR->controls->control[RControls::T_STARTER]->value);
    bool starter = true;
	if(starter)
    {
	
      // Use the starter
      tEngine+=starterTorque;
      // Raise the starter sample volume
      if(starterDelay==0)
      {
        starterDelay=1000/car->_lastDT;
      }
     

#ifdef LTRACE
	qdbg("  starting; T_starter=%f, tEngine=%f\n",starterTorque,tEngine);
#endif
	  

    }
#ifdef LTRACE
	qdbg("Stalled engine torque: %.3f\n",tEngine);
#endif
  } else
  {
    // Engine is running
    // Calculate minimal torque (if 0% throttle)
    minTorque=GetMinTorque(rpm);
  
    // Calculate maximum torque (100% throttle situation)
    maxTorque=GetMaxTorque(rpm);
  
    // The throttle accounts for how much of the torque is actually
    // produced.
    // NOTE: The output power then is 2*PI*rps*outputTorque
    // (rps=rpm/60; rotations per second). Nothing but a statistic now.
    tEngine=(maxTorque-minTorque)*throttle+minTorque;
#ifdef LTRACE
	qdbg("minTorque=%f, maxTorque=%.f, throttle=%f => torque=%f\n",minTorque,maxTorque,throttle,tEngine);
#endif
	

  }
}

/*********************
* Calc accelerations *
*********************/
void pEngine::CalcAccelerations()
{
#ifdef LTRACE
	qdbg("pEngine:CalcAcc()\n");
#endif

  if(!driveLine->IsPrePostLocked())
  {
    // Engine moves separately from the rest of the driveLine
    rotA=(tEngine-driveLine->GetClutchTorque())/GetInertia();
#ifdef LTRACE
	qdbg("  tEngine=%.2f, tClutch=%.2f, rotA=%.2f\n",tEngine,driveLine->GetClutchTorque(),rotA);
#endif
  } // else rotA is calculated in the driveline (passed up
    // from the wheels on to the root - the engine)
}

/************
* Integrate *
************/
void pEngine::Integrate()
// Step the engine.
// Also looks at the stall state (you can kickstart the engine when driving).
{
  float rpm=GetRPM();
//qdbg("pEngine:Integrate()\n");
//qdbg("  rotV=%.2f, rotA=%.2f\n",rotV,rotA);

  // This is normally done by RDriveLineComp, but avoid the function call
  rotV+=rotA*RR_TIMESTEP;

  // Deduce state of engine (stalling)
  if(IsStalled())
  {
    // Turning back on?
    if(rpm>=startRPM)
    {
//qdbg("UNStalling engine!\n");
      DisableStall();
    }
  } else
  {
    // Stalling?
    if(rpm<stallRPM)
    {
      EnableStall();
      // Trigger sample?
    }
  }

  int autoClutch = 1;
  // Auto-clutch assist
  if(autoClutch)
  {
  	
	if(rpm<autoClutchRPM)
    {
      // Engage clutch (is picked up by the driveline)
      float autoClutch=(rpm-idleRPM)/(autoClutchRPM-idleRPM);
//qdbg("Autoclutch %f\n",autoClutch);
      car->GetDriveLine()->EnableAutoClutch();
      //flags|=AUTOCLUTCH_ACTIVE;
      if(autoClutch<0)autoClutch=0;
      else if(autoClutch>1)autoClutch=1;
      car->GetDriveLine()->SetClutchApplication(autoClutch);
    } else
    {
      // Turn off auto-clutch
      car->GetDriveLine()->DisableAutoClutch();
    }
  
  }
  
//qdbg("  post rotV=%.2f\n",rotV);
}


void pEngine::setToDefault()
{

		
	char   buf[128],fname[128];
	position.Set(0,0,0);
	size = 0.0f;
	maxRPM = 8000;
	idleRPM = 1110;
	mass = 0.0f;

	// Physical attribs
#ifdef OBS
	sprintf(buf,"%s.rolling_friction_coeff",path);
	rollingFrictionCoeff=info->GetFloat(buf);
#endif
	
	torqueReaction=1.0f;
	maxTorque=468;
	SetInertia(0.35f);
	
#ifdef OBS
	sprintf(buf,"%s.inertia.final_drive",path);
	inertiaDriveShaft=info->GetFloat(buf);
#endif
	
	friction=0.0f;
	brakingCoeff=3.3f;
	//flags|=HAS_STARTER;
//	if(info->GetInt(buf,1))
//	flags|=START_STALLED;
//	sprintf(buf,"%s.starter_torque",path);
	starterTorque=90.0f;
	stallRPM=400.0f;
	stallRPM=1250.0f;
	autoClutchRPM=1250.0f;

	//flags|=AUTOMATIC;
	flags|=AUTOCLUTCH_ACTIVE;

	PreCalculate();


	

}

/*
bool pEngine::Load(QInfo *info,cstring path)
// 'path' may be 0, in which case the default "engine" is used
{
	char   buf[128],fname[128];
	QInfo *infoCurve;

	if(!path)path="engine";

	// Location
	sprintf(buf,"%s.x",path);
	position.x=info->GetFloat(buf);
	sprintf(buf,"%s.y",path);
	position.y=info->GetFloat(buf);
	sprintf(buf,"%s.z",path);
	position.z=info->GetFloat(buf);
	sprintf(buf,"%s.size",path);
	size=info->GetFloat(buf,DEF_SIZE);

	// Physical attribs
#ifdef OBS
	sprintf(buf,"%s.rolling_friction_coeff",path);
	rollingFrictionCoeff=info->GetFloat(buf);
#endif
	sprintf(buf,"%s.mass",path);
	mass=info->GetFloat(buf);

	// Power/torque
	sprintf(buf,"%s.max_rpm",path);
	maxRPM=info->GetFloat(buf,DEF_MAXRPM);
	sprintf(buf,"%s.idle_rpm",path);
	idleRPM=info->GetFloat(buf,1000);

	// Torque reaction
	sprintf(buf,"%s.torque_reaction",path);
	torqueReaction=info->GetFloat(buf);

	// Torque curve or constant (curve is preferred)
	//sprintf(buf,"%s.constant_torque",path);
	sprintf(buf,"%s.max_torque",path);
	maxTorque=info->GetFloat(buf,DEF_MAXTORQUE);
	crvTorque=new QCurve();
	sprintf(buf,"%s.curve_torque",path);
	info->GetString(buf,fname);

	//qdbg("fname_torque='%s', maxTorque=%.2f\n",fname,maxTorque);
	if(fname[0])
	{
		
		infoCurve=new QInfo(RFindFile(fname,car->GetDir()));
		crvTorque->Load(infoCurve,"curve");
		QDELETE(infoCurve);
		

	} else
	{ // No torque curve
		//QDELETE(crvTorque);
	}

	// Check torque curve in that it makes sense and is usable
	if(!crvTorque)
	{
		//qwarn("No torque curve (torque.crv?) present; you really should have one");
	} else
	{
		// Make sure it ends at 0 (assuming no engine will rev above 100,000 rpm)
		if(fabs(crvTorque->GetValue(100000))>D3_EPSILON)
		{
			//qwarn("The torque curve needs to end at 0 torque (is now %.2f)",   crvTorque->GetValue(100000));
		}
	}

	sprintf(buf,"%s.inertia.engine",path);
	SetInertia(info->GetFloat(buf));
	//inertiaEngine=info->GetFloat(buf);
#ifdef OBS
	sprintf(buf,"%s.inertia.final_drive",path);
	inertiaDriveShaft=info->GetFloat(buf);
#endif
	sprintf(buf,"%s.friction",path);
	friction=info->GetFloat(buf,DEF_FRICTION);
	sprintf(buf,"%s.braking_coeff",path);
	brakingCoeff=info->GetFloat(buf);

	// Starter engine
	sprintf(buf,"%s.starter",path);
	if(info->GetInt(buf,1))
		flags|=HAS_STARTER;
	sprintf(buf,"%s.start_stalled",path);
	if(info->GetInt(buf,1))
		flags|=START_STALLED;
	sprintf(buf,"%s.starter_torque",path);
	starterTorque=info->GetFloat(buf);
	sprintf(buf,"%s.stall_rpm",path);
	stallRPM=info->GetFloat(buf);
	sprintf(buf,"%s.start_rpm",path);
	startRPM=info->GetFloat(buf);
	sprintf(buf,"%s.autoclutch_rpm",path);
	autoClutchRPM=info->GetFloat(buf);

#ifdef OBS
	// Shifting
	sprintf(buf,"%s.shifting.automatic",path);
	if(info->GetInt(buf))
		flags|=AUTOMATIC;
	//qdbg("Autoshift: flags=%d, buf='%s'\n",flags,buf);
	sprintf(buf,"%s.shifting.shift_up_rpm",path);
	shiftUpRPM=info->GetFloat(buf,3500);
	sprintf(buf,"%s.shifting.shift_down_rpm",path);
	shiftDownRPM=info->GetFloat(buf,2000);
	sprintf(buf,"%s.shifting.time_to_declutch",path);
	timeToDeclutch=info->GetInt(buf,500);
	sprintf(buf,"%s.shifting.time_to_clutch",path);
	timeToClutch=info->GetInt(buf,500);

	// Gearbox
	path="gearbox";                     // !
	sprintf(buf,"%s.gears",path);
	gears=info->GetInt(buf,4);
	for(i=0;i<gears;i++)
	{ sprintf(buf,"%s.gear%d.ratio",path,i);
	gearRatio[i]=info->GetFloat(buf,1.0);
	sprintf(buf,"%s.gear%d.inertia",path,i);
	gearInertia[i]=info->GetFloat(buf);
	}
	sprintf(buf,"%s.end_ratio",path);
	endRatio=info->GetFloat(buf,3.0);
#endif

	// Calculate static facts
	PreCalculate();

	return TRUE;
}
*/

/*
void pEngine::OnGfxFrame()
{
}

void pEngine::DbgPrint(cstring s)
{
qdbg("pEngine state (%s):\n",s);
qdbg("  rpm=%.2f, stalled: %d, idleThrottle: %.2f, Treaction=%.2f\n",
GetRPM(),IsStalled(),idleThrottle,torqueReaction);
}


bool pEngine::LoadState(QFile *f)
{
RDriveLineComp::LoadState(f);
return TRUE;
}
bool pEngine::SaveState(QFile *f)
{
RDriveLineComp::LoadState(f);
return TRUE;
}
*/
