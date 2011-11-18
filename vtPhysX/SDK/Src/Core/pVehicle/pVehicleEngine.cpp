#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"

#include <xDebugTools.h>
#include "NxArray.h"

#include "pEngine.h"
#include "pGearbox.h"
#include "pDifferential.h"
#include "pSteer.h"


float pVehicle::getClutch()
{
	if (isValidEngine())
	{
		return getDriveLine()->GetClutchApplication();
	}
	return 0.0f;
}

void pVehicle::setClutch(float clutch)
{
	if (isValidEngine())
	{
		getDriveLine()->SetClutchApplication(clutch);
	}
}

void pVehicle::preAnimate()
{
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel *cW = _wheels[i];
		pWheel2* wheel2 = (pWheel2*)cW;
		if ( wheel2->isAxleSpeedFromVehicle() || wheel2->isTorqueFromVehicle()  )
			wheel2->preAnimate();
	}
}



void pVehicle::preCalculate()
{
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel *cW = _wheels[i];
		pWheel2* wheel2 = (pWheel2*)cW;

		if ( wheel2->isAxleSpeedFromVehicle() || wheel2->isTorqueFromVehicle()  ) 
		{
			//wheel2->preAnimate();
		}
	}
}

void pVehicle::addDifferential(pDifferential *diff)
{
	if(differentials==MAX_DIFFERENTIAL)
	{
		xWarning("RCar::addDifferential(); maximum (%d) exceeded");
		return;
	}
	differential[differentials]=diff;
	differentials++;
}


int pVehicle::doEngine(int flags,float dt)
{

	if (engine && gearbox && driveLine )
	{
		engine->updateUserControl(_cAcceleration);
		engine->CalcForces();

		driveLine->CalcForces();
		driveLine->CalcAccelerations();

		driveLine->Integrate();

		for(int i=0;i<differentials;i++)
		{
			differential[i]->Integrate();
		}


		gearbox->processFutureGear();

	}
	return 0;
}

int pVehicle::initEngine(int flags)
{

	driveLine=new pDriveLine(this);
	//driveLine->EnableAutoClutch();

	engine=new pEngine(this);
	engine->setToDefault();

	gearbox=new pGearBox(this);
	gearbox->setToDefault();

	driveLine->SetRoot(engine);
	engine->AddChild(gearbox);

	driveLine->SetGearBox(gearbox);

	steer  = new pVehicleSteer(this);
	steer->setToDefault();

	steer->setLock(getMaxSteering());



	//----------------------------------------------------------------
	//
	// control values 
	//
	_cShiftStateDown = _cShiftStateUp = 0;


	//----------------------------------------------------------------
	//
	// setup differential , a single one for the first
	//

	differentials = 0 ;


	
	pDifferential *d =  new pDifferential(this);
	d->setToDefault();
	d->engine = engine;
	addDifferential(d);



	pWheel2 *w1 = NULL;int w1Index = -1;
	pWheel2 *w2 = NULL;int w2Index = -1;
	
	findDifferentialWheels(w1Index,w2Index);


	
	w1 = (w1Index !=-1) ? (pWheel2*)_wheels[w1Index] : NULL;
	w2 = (w2Index !=-1) ? (pWheel2*)_wheels[w2Index] : NULL;
	if ( !w1||!w2 || ( !w1&&!w2 ) )
	{
		xError("Couldn't find differential wheels");
		return -1;
	}

	
	d->wheel[0]=w1; w1->setDifferential(d,0);
	d->wheel[1]=w2; w2->setDifferential(d,1);

	
	CK3dEntity *w1E  = (CK3dEntity*)GetPMan()->GetContext()->GetObject(w1->mEntID);
	CK3dEntity *w2E  = (CK3dEntity*)GetPMan()->GetContext()->GetObject(w2->mEntID);


	// Add differentials and wheels to the driveline
	// Note this code does NOT work for 3 diffs, need more work for that.
	driveLine->SetDifferentials(differentials);
	if(differentials>0)
	{
		// Hook first diff to the gearbox
		gearbox->AddChild(differential[0]);
	}
	// Make the wheels children of the differentials
	// A diff with 2 diffs as children is not yet supported.
	for(int i=0;i<differentials;i++)
	{
		differential[i]->AddChild(differential[i]->wheel[0]);
		differential[i]->AddChild(differential[i]->wheel[1]);
	}


	driveLine->CalcPreClutchInertia();
//	gearbox->SetGear(2);
	gearbox->SetGear(0);

	driveLine->SetInput(1000.0f,_cHandbrake);

	return 0;
}

void pVehicle::PreCalcDriveLine()
{
	driveLine->CalcCumulativeRatios();
	driveLine->CalcEffectiveInertiae();
	driveLine->CalcPostClutchInertia();

}
float pVehicle::getTorque(int component)
{
	if (isValidEngine())
	{
		return getEngine()->getTorque();
	}
	return -1.0f;
}

bool pVehicle::isStalled()
{
	if (isValidEngine())
	{
		return getEngine()->IsStalled();
	}
	return false;
}
float pVehicle::getRPM()
{
	if (isValidEngine())
	{
		return getEngine()->getRPM();
	}
	return -1.0f;
}
int pVehicle::getGear()
{
	if (isValidEngine())
	{
		return getGearBox()->GetGear();
	}
}

bool pVehicle::hasDifferential()
{
	return differentials;
}

bool pVehicle::isValidEngine()
{
	return engine && gearbox && driveLine && differentials && steer;
}
