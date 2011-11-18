#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pConfig.h"

#include <xTime.h>

int demoTimerExpired=0;

float absTime = 0.0f;
float lastAbsTime = 0.0f;
#include "Timing.h"

static float			gTimestepMultiplier	= 1.0f;

float getCurrentTime()
{
	unsigned int currentTime = timeGetTime();
	return (float)(currentTime)*0.001f;
}


float getElapsedTime()
{
	static LARGE_INTEGER previousTime;
	static LARGE_INTEGER freq;
	static bool init = false;
	if(!init){
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&previousTime);
		init=true;
	}
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	unsigned long long elapsedTime = currentTime.QuadPart - previousTime.QuadPart;
	previousTime = currentTime;
	return (float)(elapsedTime)/(freq.QuadPart);
}


int PhysicManager::_checkResetList()
{

	int result = 0 ;
	pRestoreMapIt it  = _getRestoreMap()->Begin();
	while(it != _getRestoreMap()->End())
	{
		CK_ID id = it.GetKey();
		CK3dEntity * ent  = (CK3dEntity*)GetPMan()->GetContext()->GetObject(id);
		if(ent)
		{
			pRigidBody* body = GetPMan()->getBody(ent);
			if (body)
			{
				body->onICRestore(ent,*it);
				_getRestoreMap()->Remove(it.GetKey());
				//_getRestoreMap()->Remove(it.GetKey());
				it =_getRestoreMap()->Begin();
				result++;
				continue;
			}
		}else
		{
			_getRestoreMap()->Remove(it.GetKey());
			it =_getRestoreMap()->Begin();
			result++;
			continue;
		}
		it++;
	}
	//_getRestoreMap()->Clear();

	/*
	for (CK3dEntity** it = resetList.Begin(); it != resetList.End(); ++it)
	{	
		pRigidBody* body = GetPMan()->getBody(*it);
		if (body)
		{
			body->onICRestore(*it,true);
		}
	}
	*/
//	resetList.Clear();

	return result;
}

float PhysicManager::getLastTimeStep(int flags)
{

	NxF32 elapsedTime = getElapsedTime();
	elapsedTime*= GetContext()->GetTimeManager()->GetTimeScaleFactor();
	
	elapsedTime*=10.0f;
	if(elapsedTime <= 0)
		elapsedTime = 0;

	return elapsedTime;

}
void PhysicManager::advanceTime(float time)
{
	
	timer +=time;
	float timeNow = timer;

	mLastStepTime = time;

	int op= 30 ;
	op++;
	/*
	float TimeStep = 1.0f / 60.0f;
	if(gFixedStep) gScene->setTiming(TimeStep, 1, NX_TIMESTEP_FIXED);
	else gScene->setTiming(TimeStep, 1, NX_TIMESTEP_VARIABLE);

	*/

}

void PhysicManager::update(float stepsize)
{
	advanceTime(stepsize);
/*	float a= time->GetSpanMS();
	float b= time->GetRealTime();
	time->Update();
*/
	int    lastTime;
	int    curTime,diffTime,maxTime;

	// Calculate time from last to current situation
//	time->Update();
	curTime=time->GetRealTime();
	lastTime=time->GetLastSimTime();
	diffTime=curTime-lastTime;
	//maxTime=lastTime+maxSimTimePerFrame;

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


	//CKTimeProfiler(const char* iTitle, CKContext* iContext, int iStartingCount = 4):
	//CKTimeProfiler MyProfiler("PhysX Step",GetContext(),8);



	float elapsedTime = getLastTimeStep(0);//(0.5)
	float msTimeNow = lastStepTimeMS;//(50.0)

	//_checkResetList();
	
	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		
		w->step(elapsedTime);
		//w->step(lastStepTimeMS);
		it++;
	}

	/*
	if (checkPhysics)
	{
		checkWorlds();
		checkPhysics = false;
	}
	*/

	

	/*if (checkPhysics)
	{
		_checkObjectsByAttribute(GetContext()->GetCurrentLevel()->GetCurrentScene());
		checkPhysics = false;
	}*/
	//float pTime  = MyProfiler
	//float a = pTime;
}

CKERROR PhysicManager::PostProcess()
{
	advanceTime(lastStepTimeSec);

	if(sceneWasChanged)
	{


		//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"sceneWasChanged");
		sceneWasChanged = 0;

	}


	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		w->onPostProcess();
		it++;
	}
	
	cleanAttributePostObjects();
	_cleanTriggers();
	getJointFeedbackList().Clear();
	return CK_OK;
}


CKERROR PhysicManager::PreProcess()
{
	

	float timeCtx  = GetContext()->GetTimeManager()->GetLastDeltaTimeFree();
	update(timeCtx);

	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		w->onPreProcess();
		it++;
	}
	
	return CK_OK;
}

void PhysicManager::_cleanTriggers()
{
	/*
	int nbEntries = getTriggers().Size() ; 
	for (int i  = 0 ; i < getTriggers().Size(); i++ )
	{
	
		pTriggerEntry &entry  = *getTriggers().At(i);
		{
			if (entry.triggered)
			{
				getTriggers().RemoveAt(i);
				i = 0;
			}
		}
	}
	*/
	getTriggers().Clear();
}

