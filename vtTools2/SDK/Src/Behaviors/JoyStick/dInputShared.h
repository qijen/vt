/********************************************************************
	created:	2009/06/01
	created:	1:6:2009   14:19
	filename: 	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick\dInputShared.h
	file path:	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick 
	file base:	dInputShared
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	shared functions for joystick
*********************************************************************/
#ifndef __DINPUT_SHARED_H__
#define __DINPUT_SHARED_H__

#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#include <dinput.h>

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDialogProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK EnumFFDevicesCallback2( LPCDIDEVICEINSTANCE pDDI, VOID* pvRef );
BOOL CALLBACK EnumAndCreateEffectsCallback2( LPCDIFILEEFFECT pDIFileEffect, VOID* pvRef );

HRESULT InitDirectInput2( HWND hDlg );
HRESULT FreeDirectInput2();
VOID    EmptyEffectList2();
HRESULT OnReadFile2( HWND hDlg,const char*file);
HRESULT OnPlayEffects2( HWND hDlg );
HRESULT SetDeviceForcesXY(float x,float y);
#endif