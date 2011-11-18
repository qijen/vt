/********************************************************************
	created:	2007/12/15
	created:	15:12:2007   22:33
	filename: 	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\plugins\xUtils\src\xUtils.cpp
	file path:	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\plugins\xUtils\src
	file base:	xUtils
	file ext:	cpp
	author:		Günter Baumgart
	
	purpose:	
*********************************************************************/

#include <pch.h>		///  DWORD,
#include <tchar.h>
#include "xUtils.h"




#include "d3d9.h"

#include "3d/DXDiagNVUtil.h"

//************************************
// Method:    GetPhysicalMemoryInMB
// FullName:  xUtils::system::GetPhysicalMemoryInMB
// Access:    public 
// Returns:   int
// Qualifier:
//************************************
int
xUtils::system::GetPhysicalMemoryInMB()
{

	DXDiagNVUtil nvutil;
	nvutil.InitIDxDiagContainer();


	float ret = -1;
	nvutil.GetPhysicalMemoryInMB(&ret);
	nvutil.FreeIDxDiagContainer();

	return static_cast<int>(ret);
}
//is defined in GetDXVer.cpp : 
extern HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

//************************************
// Method:    GetDirectVersion
// FullName:  xUtils::GetDirectVersion
// Access:    private static 
// Returns:   void
// Qualifier:
// Parameter: char*&version
// Parameter: int& minor
// Description : 
//************************************
HRESULT
xUtils::system::GetDirectVersion(char*&version,DWORD& minor)
{

	HRESULT hr;
	TCHAR strResult[128];
	DWORD dwDirectXVersion = 0;
	TCHAR strDirectXVersion[10];

	hr = GetDXVersion( &minor,strResult, 10 );
	version  = new char[strlen(strResult)];
	strcpy(version,strResult);
	return hr;
	//strcpy(version,strDirectXVersion);
}

int GetPhysicalMemoryInMB()
{
	DXDiagNVUtil nvutil;
	nvutil.InitIDxDiagContainer();


	float ret = -1;
	nvutil.GetPhysicalMemoryInMB(&ret);
	nvutil.FreeIDxDiagContainer();

	return ret;
}
