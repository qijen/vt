#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"

#include <xDebugTools.h>
#include "NxArray.h"

#include "pEngine.h"
#include "pGearbox.h"
#include "pDifferential.h"

void pVehicle::AddDifferential(pDifferential *diff)
{
	if(differentials==MAX_DIFFERENTIAL)
	{
		xWarning("RCar::AddDifferential(); maximum (%d) exceeded");
		return;
	}
	differential[differentials]=diff;
	differentials++;
}


int pVehicle::doEngine(int flags,float dt)
{

	if (engine && gearbox && driveLine )
	{
		engine->SetInput(_cAcceleration);
		engine->CalcForces();

		driveLine->CalcForces();
		driveLine->CalcAccelerations();

		driveLine->Integrate();

		for(int i=0;i<differentials;i++)
		{
			differential[i]->Integrate();
		}


	}
	return 0;
}

int pVehicle::initEngine(int flags)
{

	driveLine=new pDriveLine(this);

	engine=new pEngine(this);
	engine->setToDefault();

	gearbox=new pGearBox(this);
	gearbox->setToDefault();

	driveLine->SetRoot(engine);
	engine->AddChild(gearbox);

	driveLine->SetGearBox(gearbox);

	differentials = 0 ; 

	//----------------------------------------------------------------
	//
	// setup differential , a single one for the first
	//

	pDifferential *d =  new pDifferential(this);
	d->setToDefault();

	pWheel *w1 = NULL;int w1Index = -1;
	pWheel *w2 = NULL;int w2Index = -1;
	

	findDifferentialWheels(w1Index,w2Index);
	w1 = (w1Index !=-1) ? _wheels[w1Index] : NULL;
	w2 = (w2Index !=-1) ? _wheels[w2Index] : NULL;

	if ( !w1||!w2 || ( !w1&&!w2 ) )
	{
		xError("Couldn't find differential wheels");
		return -1;
	}

	d->wheel[0]=w1; w1->setDifferential(d,0);
	d->wheel[1]=w2; w2->setDifferential(d,1);

	d->engine = engine;
	AddDifferential(d);

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
	gearbox->SetGear(0);

	return 0;
}

//----------------------------------------------------------------
//
// 
//

float pVehicle::getTorque(int component)
{
	if (isValidEngine())
	{
		return GetEngine()->GetTorque();
	}
	return -1.0f;
}

bool pVehicle::isStalled()
{
	if (isValidEngine())
	{
		return GetEngine()->IsStalled();
	}
	return false;
}
float pVehicle::getRPM()
{
	if (isValidEngine())
	{
		return GetEngine()->GetRPM();
	}
	return -1.0f;
}
int pVehicle::getGear()
{
	if (isValidEngine())
	{
		return GetGearBox()->GetGear();
	}
}

bool pVehicle::hasDifferential()
{
	return differentials;
}

bool pVehicle::isValidEngine()
{
	return engine && gearbox && driveLine && differentials;
}
