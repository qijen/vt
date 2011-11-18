#include "pGearbox.h"
#include "vtPhysXAll.h"

#include <xDebugTools.h>

#define DEF_SIZE  .25
#define DEF_MAXRPM 5000
#define DEF_MAXPOWER 100
#define DEF_FRICTION 0
#define DEF_MAXTORQUE 340          // ~ F1 Jos Verstappen

#define DEF_TORQUE    100          // In case no curve is present

// If USE_HARD_REVLIMIT is used, any rpm above the max. RPM
// returns a 0 engine torque. Although this works, it gives quite
// hard rev limits (esp. when in 1st gear). Better is to supply
// a curve which moves down to 0 torque when rpm gets above maxRPM,
// so a more natural (smooth) balance is obtained (and turn
// this define off)
//#define USE_HARD_REVLIMIT


void pGearBox::SetGear(int gear)
{

/*#ifdef LTRACE
qdbg("pGearBox:SetGear(%d)\n",gear);
#endif
*/
  //QASSERT_V(gear>=0&&gear<gears);
  curGear=gear;

  // Calculate resulting ratio
  SetRatio(gearRatio[curGear]);
  SetInertia(gearInertia[curGear]);

  // Recalculate driveline inertias and ratios
  //car->PreCalcDriveLine();
}



void pGearBox::CalcForces()
{

/*#ifdef LTRACE
//qdbg("pGearBox::CalcForces()\n");
#endif
	*/
}

/************
* Integrate *
************/
void pGearBox::Integrate()
// Based on current input values, adjust the engine
{
  int t;
  bool ac=true;// RMGR->IsEnabled(RManager::ASSIST_AUTOCLUTCH);

  pDriveLineComp::Integrate();

  // The shifting process
  if(autoShiftStart)
  {

	
    //t=RMGR->time->GetSimTime()-autoShiftStart;
	t=car->_lastDT;
    if(ac)car->GetDriveLine()->EnableAutoClutch();
	
    // We are in a shifting operation
    if(curGear!=futureGear)
    {
	
      // We are in the pre-shift phase
      if(t>=timeToDeclutch)
      {
		  
		//qdbg("Shift: declutch ready, change gear\n");
        // Declutch is ready, change gear
        SetGear(futureGear);
        // Trigger gear shift sample
        //car->GetRAPGear()->GetSample()->Play();
        if(ac)
			car->GetDriveLine()->SetClutchApplication(1.0f);
      } else
      {
        // Change clutch
        if(ac)
          car->GetDriveLine()->SetClutchApplication((t*1000/timeToDeclutch)/1000.0f);
      }
	  
    } else
    {
      // We are in the post-shift phase
      if(t>=timeToClutch+timeToDeclutch)
      {
//qdbg("Shift: clutch ready, end shift\n");
        // Clutch is ready, end shifting process
#ifdef OBS_MANUAL_CLUTCH
        clutch=1.0f;
#endif
        // Conclude the shifting process
        autoShiftStart=0;
        if(ac)
        {
		
          car->GetDriveLine()->SetClutchApplication(0.0f);
          car->GetDriveLine()->DisableAutoClutch();
        }
      } else
      {
        // Change clutch
        
		  if(ac)car->GetDriveLine()->SetClutchApplication(
            ((t-timeToClutch)*1000/timeToDeclutch)/1000.0f);
		
      }
    }
  }
}
void pGearBox::OnGfxFrame()
{
	/*
//qdbg("pGearBox:OnGfxFrame()\n");

  if(autoShiftStart==0)
  {
    // No shift in progress; check shift commands from the controllers
    if(RMGR->controls->control[RControls::T_SHIFTUP]->value)
    {
//qdbg("Shift Up!\n");
      if(curGear<gears)
      {
        autoShiftStart=RMGR->time->GetSimTime();
        switch(curGear)
        {
          case 0:  futureGear=2; break;   // From neutral to 1st
          case 1:  futureGear=0; break;   // From reverse to neutral
          default: futureGear=curGear+1; break;
        }
      }
    } else if(RMGR->controls->control[RControls::T_SHIFTDOWN]->value)
    {
      autoShiftStart=RMGR->time->GetSimTime();
      if(curGear!=1)                      // Not in reverse?
      {
        switch(curGear)
        {
          case 0:  futureGear=1; break;   // From neutral to reverse
          case 2:  futureGear=0; break;   // From 1st to neutral
          default: futureGear=curGear-1; break;
        }
      }
qdbg("Switch back to futureGear=%d\n",futureGear);
    }
  }
  */
}
/*
bool pGearBox::LoadState(QFile *f)
{
RDriveLineComp::LoadState(f);
f->Read(&curGear,sizeof(curGear));
f->Read(&autoShiftStart,sizeof(autoShiftStart));
f->Read(&futureGear,sizeof(futureGear));
return TRUE;
}
bool pGearBox::SaveState(QFile *f)
{
RDriveLineComp::SaveState(f);
f->Write(&curGear,sizeof(curGear));
f->Write(&autoShiftStart,sizeof(autoShiftStart));
f->Write(&futureGear,sizeof(futureGear));
return TRUE;
}

*/
/*
bool pGearBox::Load(QInfo *info,cstring path)
// 'path' may be 0, in which case the default "engine" is used
{
char   buf[128];
int    i;

if(!path)path="engine";

// Shifting (still in the 'engine' section for historical reasons)
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

//qdbg("declutch=%d, clutch=%d (buf=%s)\n",timeToDeclutch,timeToClutch,buf);

// Gearbox
path="gearbox";                     // !
sprintf(buf,"%s.gears",path);
gears=info->GetInt(buf,4);
if(gears>=MAX_GEAR-1)
{ qwarn("Too many gears defined (%d, max=%d)",gears,MAX_GEAR-1);
gears=MAX_GEAR-1;
}
for(i=0;i<gears;i++)
{
sprintf(buf,"%s.gear%d.ratio",path,i);
gearRatio[i+1]=info->GetFloat(buf,1.0);
sprintf(buf,"%s.gear%d.inertia",path,i);
gearInertia[i+1]=info->GetFloat(buf);
}

return TRUE;
}
*/



#ifdef OBS

float pGearBox::GetInertiaAtDifferential()
// Returns effective inertia as seen at the input of the differential.
// This may be used as the input for the differential.
// Notice the gearing from differential to engine.
{
	float  totalInertia;
	float  ratio,ratioSquared;

	// From differential towards engine
	// First, the driveshaft
	ratio=endRatio;
	ratioSquared=ratio*ratio;
	totalInertia=inertiaDriveShaft*ratioSquared;

	// Gearing and engine
	// Both engine and the faster rotating part of the gearbox are at
	// the engine side of the clutch. Therefore, both interia's from those
	// 2 objects must be scaled to get the effective inertia.
	// Note this scaling factor is squared, for reasons
	// I explain on the Racer website.
	ratio=gearRatio[curGear]*endRatio;
	ratioSquared=ratio*ratio;
	totalInertia+=clutch*(gearInertia[curGear]+inertiaEngine)*ratioSquared;
	return totalInertia;
}

float pGearBox::GetInertiaForWheel(RWheel *w)
// Return effective inertia as seen in the perspective of wheel 'w'
// Takes into account any clutch effects
{
	float  totalInertia;
	float  inertiaBehindClutch;
	float  NtfSquared,NfSquared;
	//float  rotationA;
	RWheel *wheel;
	int     i;

	// Calculate total ratio multiplier; note the multipliers are squared,
	// and not just a multiplier for the inertia. See Gillespie's book,
	// 'Fundamentals of Vehicle Dynamics', page 33.
	NtfSquared=gearRatio[curGear]*endRatio;
	NtfSquared*=NtfSquared;

	// Calculate total inertia that is BEHIND the clutch
	NfSquared=endRatio*endRatio;
	inertiaBehindClutch=gearInertia[curGear]*NtfSquared+
		inertiaDriveShaft*NfSquared;
	// Add inertia of attached and powered wheels
	// This is a constant, so it should be cached actually (except
	// when a wheel breaks off)

	/*  for(i=0;i<car->GetWheels();i++)
	{
	wheel=car->GetWheel(i);
	if(wheel->IsPowered()&&wheel->IsAttached())
	inertiaBehindClutch+=wheel->GetRotationalInertia()->x;
	}
	*/    
	// Add the engine's inertia based on how far the clutch is engaged
	/*
	totalInertia=inertiaBehindClutch+clutch*inertiaEngine*NtfSquared;
	*/
	return totalInertia;
}
// Return effective torque for wheel 'w'
// Takes into account any clutch effects
float pGearBox::GetTorqueForWheel(pWheel *w)

{
	//qdbg("clutch=%f, T=%f, ratio=%f\n",clutch,torque,gearRatio[curGear]*endRatio);

	//return clutch*torque*gearRatio[curGear]*endRatio;
}


#endif



void pGearBox::setToDefault()
{

	gears = 7;
	gearRatio[0]=-2.59f;
	gearInertia[0]=0.1f;

	gearRatio[1]=2.94f;
	gearInertia[1]=0.06f;

	gearRatio[2]=2.056f;
	gearInertia[2]=0.05f;

	gearRatio[3]=1.520f;
	gearInertia[3]=0.04f;

	gearRatio[4]=1.179f;
	gearInertia[4]=0.03f;

	gearRatio[5]=1.030f;
	gearInertia[5]=0.02f;

	gearRatio[6]=0.914f;
	gearInertia[6]=0.01f;

	autoShiftStart = 1;
	timeToClutch=25.0f;
	timeToDeclutch=352.0f;
	shiftDownRPM= 3000.0f;
	shiftUpRPM=7000.0f;



}

pGearBox::pGearBox(pVehicle *_car)
: pDriveLineComp()
{
	//SetName("gearbox");

	car=_car;
	Reset();
}
pGearBox::~pGearBox()
{
}

void pGearBox::Reset()
// Reset all variables
{
	int i;

	flags=0;
	timeToDeclutch=timeToClutch=0;
	autoShiftStart=1;
	futureGear=0;
	for(i=0;i<MAX_GEAR;i++)
	{ gearRatio[i]=1;
	gearInertia[i]=0;
	}
	curGear=0;          // Neutral
	gears=4;
}

XString pGearBox::GetGearName(int gear)
// Returns a name for a gear
// Convenience function.
{
	switch(gear)
	{
	case 0: return "N";
	case 1: return "R";
	case 2: return "1st";
	case 3: return "2nd";
	case 4: return "3rd";
	case 5: return "4th";
	case 6: return "5th";
	case 7: return "6th";
	case 8: return "7th";
	case 9: return "8th";
	default: return "G??";
	}
}



void pGearBox::Paint()
{
}
