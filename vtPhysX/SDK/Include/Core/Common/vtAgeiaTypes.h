/********************************************************************
	created:	2007/11/23
	created:	23:11:2007   12:21
	filename: 	e:\ProjectRoot\current\vt_plugins\vtOpenDynamics\Manager\vtOdeTypes.h
	file path:	e:\ProjectRoot\current\vt_plugins\vtOpenDynamics\Manager
	file base:	vtOdeTypes
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#ifndef __VTODE_TYPES_H_
	#define __VTODE_TYPES_H_


#include "pSleepingSettings.h"
#include "pWorldSettings.h"

typedef CK3dEntity* vt3DObjectType;
typedef VxVector xVector3;

struct vtHeightFieldData
{

	/*
	int wS,dS,warp;
	float w,d,thickness,scale;
	int ID,tID;
	float rF,gF,bF,aF;
	float maxH;
	CK_ID maxHReference;
	dHeightfieldDataID heightFieldData;
	vtHeightFieldData(float _w,float _d,float _thickness,float _scale,int _wS,int _dS,int _warp,int _ID,int _texID,float _rF,float _gF,float _bF,float _aF) :
		w(_w) , d(_d) , thickness(_thickness) , scale(_scale ) ,warp(_warp) , ID(_ID) , wS(_wS) , dS(_dS) ,tID(_texID) , rF(_rF) , gF(_gF) ,bF(_bF) , aF(_aF)
		{
            heightFieldData = NULL;
		}
		
	float lastH;
	Vx2DVector lastHCoord;
	int lastColor;
*/

};
struct vtWorldInfo
{ 

/*	VxVector Gravity;
	float CFM;
	float ERP;
	float StepResFactor;
	int MaxIterations;
	int AutoEnableDepth;
	float MaximumContactCorrectVelocity;
	float ContactSurfaceLayer;
	float rF,gF,bF,aF;
	
	vtWorldInfo()
	{
		Gravity.x = 0.0f;
		Gravity.y = -10.0f;
		Gravity.z = 0.0f;
		CFM = 0.00000001f;
		ERP = 0.9f;
		AutoEnableDepth = 1;
		MaxIterations = 100;
		StepResFactor = 0.0085f;
	}
	
	vtWorldInfo(VxVector _g,float _CFM,float _ERP,int _Auto,int _Max,float _StepResFactor,float _rF,float _gF,float _bF,float _aF)
	 : Gravity(_g) , CFM(_CFM) , ERP(_ERP) , AutoEnableDepth(_Auto) , MaxIterations(_Max) , StepResFactor(_StepResFactor) , rF(_rF) , gF(_gF),bF(_bF ),aF(_aF)
	{
	}
*/	
};
//////////////////////////////////////////////////////////////////////////
struct vtIntersectionInfo 
{
	/*
	vt3DObjectType m_ObjectPart;
	vt3DObjectType m_TouchedObstacle;
	vt3DObjectType m_TouchedSubObstacle;
	xVector3 m_position;
	xVector3 m_Normal;
	float	m_Depth;
	int m_num; 
	*/
};

#endif