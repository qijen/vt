#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pConfig.h"

int demoTimerExpired=0;


void PhysicManager::advanceTime(float time)
{
	
	timer +=time;
	float timeNow = timer;

	mLastStepTime = time;

}

void PhysicManager::update(float stepsize)
{

	advanceTime(stepsize);

#ifdef SESSION_LIMIT
	#ifndef REDIST
		
		if (timer >(SESSION_MAX))
		{
			if (demoTimerExpired==0)
			{
				xLogger::xLog(ELOGERROR,E_LI_MANAGER,"Demo expires after 15 mins");
				demoTimerExpired = 1;
			}
			return ;
		}
	#endif
#endif
	

#ifdef REDIST

	if (m_Context->IsInInterfaceMode())
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"This is a redist Dll");
		return;
	}
	#pragma message("-------------------------------PManager ::Update REDIST" )
#endif
	
#if PROFILER
	//CKTimeProfiler(const char* iTitle, CKContext* iContext, int iStartingCount = 4):
	//CKTimeProfiler MyProfiler("PhysX Step",GetContext(),8);
#endif
	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		w->step(stepsize);
		it++;
	}

	//float pTime  = MyProfiler
	//float a = pTime;
}

CKERROR PhysicManager::PostProcess()
{

	
	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		//w->getCollisions().Clear();
		it++;
	}

	cleanAttributePostObjects();

	return CK_OK;
}

CKERROR PhysicManager::PreProcess()
{

	float timeCtx  = GetContext()->GetTimeManager()->GetLastDeltaTimeFree();
	update(timeCtx);
	return CK_OK;
}
