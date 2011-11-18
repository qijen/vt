#include "StdAfx.h"
#include "pGearbox.h"
#include "vtPhysXAll.h"

#include "pVehicleAll.h"

#include <xDebugTools.h>

#define DEF_SIZE  .25
#define DEF_MAXRPM 5000
#define DEF_MAXPOWER 100
#define DEF_FRICTION 0
#define DEF_MAXTORQUE 340          // ~ F1 Jos Verstappen
#define DEF_TORQUE    100          // In case no curve is present
#define USE_HARD_REVLIMIT


float pGearBox::GetInertiaForWheel(pWheel2 *w)
// Return effective inertia as seen in the perspective of wheel 'w'
// Takes into account any clutch effects
{
	float  totalInertia;
	
	float  inertiaBehindClutch;
	float  NtfSquared,NfSquared;
	//rfloat  rotationA;
	pWheel *wheel;
	int     i;

	// Calculate total ratio multiplier; note the multipliers are squared,
	// and not just a multiplier for the inertia. See Gillespie's book,
	// 'Fundamentals of Vehicle Dynamics', page 33.
	NtfSquared=gearRatio[curGear]*endRatio;
	NtfSquared*=NtfSquared;

	/*
	// Calculate total inertia that is BEHIND the clutch
	NfSquared=endRatio*endRatio;
	inertiaBehindClutch=gearInertia[curGear]*NtfSquared+inertiaDriveShaft*NfSquared;
	
	/*
	// Add inertia of attached and powered wheels
	// This is a constant, so it should be cached actually (except
	// when a wheel breaks off)
	for(i=0;i<car->GetWheels();i++)
	{
		wheel=car->GetWheel(i);
		if(wheel->IsPowered()&&wheel->IsAttached())
			inertiaBehindClutch+=wheel->GetRotationalInertia()->x;
	}

	// Add the engine's inertia based on how far the clutch is engaged
	totalInertia=inertiaBehindClutch+clutch*inertiaEngine*NtfSquared;*/
	return totalInertia;
}

float pGearBox::GetTorqueForWheel(pWheel2 *w)
// Return effective torque for wheel 'w'
// Takes into account any clutch effects
{
	float clutch =car->getDriveLine()->GetClutchApplication();
	
	float torque  = car->getDifferential(0)->GetTorqueOut(w->differentialSide);
	endRatio  = car->getEngine()->GetCumulativeRatio();




	//car->getDriveLine()->

	
	


	//qdbg("clutch=%f, T=%f, ratio=%f\n",clutch,torque,gearRatio[curGear]*endRatio);
	return clutch*torque*gearRatio[curGear]*endRatio;
}


void pGearBox::SetGear(int gear)
{
	curGear=gear;
	float cRatio = gearRatio[curGear];
	float cInertia = gearInertia[curGear];
	SetRatio(gearRatio[curGear]);
	SetInertia(gearInertia[curGear]);
	car->PreCalcDriveLine();
}
void pGearBox::CalcForces()
{

}
void pGearBox::Integrate()
// Based on current input values, adjust the engine
{
  int t;
  bool ac=true;// RMGR->IsEnabled(RManager::ASSIST_AUTOCLUTCH);

  pDriveLineComp::Integrate();

  // The shifting process
  if(autoShiftStart)
  {

	
    t=GetPMan()->GetContext()->GetTimeManager()->GetAbsoluteTime()-autoShiftStart;
	
	if(ac){
		car->getDriveLine()->EnableAutoClutch();
	}
	
    // We are in a shifting operation
    if(curGear!=futureGear)
    {
	
      // We are in the pre-shift phase
      if(t>=timeToDeclutch)
      {
        // Declutch is ready, change gear
        SetGear(futureGear);
        // Trigger gear shift sample
        if(ac && (car->_currentStatus & VS_IsAccelerated) )
			car->getDriveLine()->SetClutchApplication(1.0f);
      } else
      {
        // Change clutch
        if(ac && (car->_currentStatus & VS_IsAccelerated) )
          car->getDriveLine()->SetClutchApplication((t*1000/timeToDeclutch)/1000.0f);
      }
	  
    } else
    {
      // We are in the post-shift phase
      if(t>=timeToClutch+timeToDeclutch)
      {
        // Clutch is ready, end shifting process

        // Conclude the shifting process
        autoShiftStart=0;
        if(ac)
        {
		
          car->getDriveLine()->SetClutchApplication(0.0f);
          car->getDriveLine()->DisableAutoClutch();
        }
      } else
      {
        // Change clutch
        
		  if(ac && (car->_currentStatus & VS_IsAccelerated))
			  car->getDriveLine()->SetClutchApplication(((t-timeToClutch)*1000/timeToDeclutch)/1000.0f);
		
      }
    }
  }
}

void pGearBox::processFutureGear()
{

	//qdbg("pGearBox:OnGfxFrame()\n");

	if(autoShiftStart==0)
	{
		// No shift in progress; check shift commands from the controllers
		if(car->_cShiftStateUp)
		{
			//qdbg("Shift Up!\n");
			if(curGear<gears)
			{
				autoShiftStart=GetPMan()->GetContext()->GetTimeManager()->GetAbsoluteTime();
				switch(curGear)
				{
					case 0:  futureGear=2; break;   // From neutral to 1st
					case 1:  futureGear=0; break;   // From reverse to neutral
					default: futureGear=curGear+1; break;
				}
			}
		}else if(car->_cShiftStateDown)
		{
			autoShiftStart=GetPMan()->GetContext()->GetTimeManager()->GetAbsoluteTime();
			if(curGear!=1)                      // Not in reverse?
			{
				switch(curGear)
				{
					case 0:  futureGear=1; break;   // From neutral to reverse
					case 2:  futureGear=0; break;   // From 1st to neutral
					default: futureGear=curGear-1; break;
				}
			}
			//qdbg("Switch back to futureGear=%d\n",futureGear);
		}
	}
}

void pGearBox::setToDefault()
{

	gears = 6;
	
	gearRatio[0]=1.0f;
	gearInertia[0]=0.0f;

	gearRatio[1]=-3.59f;
	gearInertia[1]=0.9f;
	
	gearRatio[2]=3.61f;
	gearInertia[2]=0.13f;

	gearRatio[3]=4.36f;
	gearInertia[3]=0.4f;

	gearRatio[4]=1.69f;
	gearInertia[4]=0.07f;

	gearRatio[5]=1.29f;
	gearInertia[5]=0.05f;

	gearRatio[6]=1.03f;
	gearInertia[6]=0.04f;

	gearRatios.insert(0,-3.61f);
	gearTensors.insert(0,0.13f);
	
	gearRatios.insert(1,3.61f);
	gearTensors.insert(1,0.13f);
	
	gearRatios.insert(2,2.36f);
	gearTensors.insert(2,0.09f);

	gearRatios.insert(3,1.69f);
	gearTensors.insert(3,0.07f);


	gearRatios.insert(4,1.29f);
	gearTensors.insert(4,0.05f);

	gearRatios.insert(5,1.03f);
	gearTensors.insert(5,0.04f);


	autoShiftStart = 0;
	timeToClutch=300;
	timeToDeclutch=300.0f;
	shiftDownRPM= 1110.0f;
	shiftUpRPM=5500.0f;
	futureGear = 0;
	




}

pGearBox::pGearBox(pVehicle *_car)
: pDriveLineComp()
{
	SetName("gearbox");
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
	autoShiftStart=0;
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
