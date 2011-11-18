#ifndef __xDOStructs_h
#define __xDOStructs_h

#include "xNetTypes.h"
#include "xPoint.h"

struct x3DPositionState
{

	Point3F state;
	F32 stateDeltaTime;
	Point3F stateVelocity;

	x3DPositionState()
	{
		state  = Point3F(0,0,0);
		stateDeltaTime = 0.0f;
		stateVelocity = Point3F(0,0,0);
	}
};

struct  x3DObjectData
{
	x3DPositionState *prevState;
	x3DPositionState *currentState;
	//////////////////////////////////////////////////////////////////////////
	//Position
    Point3F lastPostion;
	Point3F currentPosition;
	
	//////////////////////////////////////////////////////////////////////////
	float dT;
	float currentTime;
	float lastTime;
	int warpCount;

	int UpdateFlags;
	//////////////////////////////////////////////////////////////////////////
	x3DObjectData() : currentPosition(Point3F(0,0,0))
	{
		currentTime = 0.0f;
		lastTime = 0.0f;
		dT = 0.0f;
		prevState  = new x3DPositionState();
		currentState  = new x3DPositionState();
		UpdateFlags = 0 ; 
		warpCount = 0;

	}
};

#endif
