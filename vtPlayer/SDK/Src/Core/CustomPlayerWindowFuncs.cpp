/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"
#include "resourceplayer.h"


#if defined(CUSTOM_PLAYER_STATIC)
// include file used for the static configuration
#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#endif

#include "vtTools.h"

extern CCustomPlayer*	thePlayer;

int CCustomPlayer::_CreateWindows()
{
	RECT mainRect;

	int k = WindowedWidth();
	int k1 = WindowedHeight();

	// compute the main window rectangle, so the window is centered
	mainRect.left = (GetSystemMetrics(SM_CXSCREEN)-WindowedWidth())/2;
	mainRect.right = WindowedWidth()+mainRect.left;
	mainRect.top = (GetSystemMetrics(SM_CYSCREEN)-WindowedHeight())/2;
	mainRect.bottom = WindowedHeight()+mainRect.top;
	BOOL ret = AdjustWindowRect(&mainRect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);

	DWORD dwStyle;

	// create the main window

	switch(PGetApplicationMode())
	{
	case normal:
		{
			m_MainWindow = CreateWindow(m_PlayerClass.CStr(),GetPAppStyle()->g_AppTitle.Str(),WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
				mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
				NULL,NULL,m_hInstance,NULL);
			break;
		}
	case preview:
		{
			RECT rc;
			GetClientRect( m_hWndParent, &rc );
			dwStyle = WS_VISIBLE | WS_CHILD;
			AdjustWindowRect( &rc, dwStyle, FALSE );
			m_MainWindow = m_hWndParent;
			m_WindowedWidth = rc.right - rc.left ; 
			m_WindowedHeight = rc.bottom - rc.top;
			m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_VISIBLE)&~WS_CAPTION,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,m_MainWindow,NULL,m_hInstance,this);
			
		}
		break;
	case popup:
		{
			dwStyle = (WS_OVERLAPPEDWINDOW); 
			//m_MainWindow  = CreateWindowEx( dwStyle , m_PlayerClass.CStr(), GetPAppStyle()->g_AppTitle.Str(), WS_POPUP, mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top , m_MainWindow, NULL,m_hInstance, NULL );
			m_MainWindow = CreateWindow(m_PlayerClass.CStr(),"asd",WS_POPUP,mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,NULL,NULL,m_hInstance,NULL);
			break;
		}
	default:
		m_MainWindow = CreateWindow(m_PlayerClass.CStr(),GetPAppStyle()->g_AppTitle.Str(),WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
			mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
			NULL,NULL,m_hInstance,NULL);
		break;
	}



	//	m_MainWindow = CreateWindow(m_PlayerClass.CStr(),"asd",WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
	//		mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
	//		NULL,NULL,m_hInstance,NULL);

	int errorL  = GetLastError();

	if(!m_MainWindow) {
		return FALSE;
	}

	if(PGetApplicationMode() != preview)
	{
		// create the render window
		if (( GetEWindowInfo()->g_GoFullScreen )) 
		{
			m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_OVERLAPPED|WS_VISIBLE)&~WS_CAPTION,
				0,0,FullscreenWidth(),FullscreenHeight(),m_MainWindow,NULL,m_hInstance,0);
		} else {
			m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_OVERLAPPED|WS_VISIBLE)&~WS_CAPTION,
				0,0,WindowedWidth(),WindowedHeight(),m_MainWindow,NULL,m_hInstance,0);
		}
	}


	if(!m_RenderWindow) 
	{
		return FALSE;
	}


	return TRUE;
}




ATOM CCustomPlayer::_RegisterClass()
{
	// register window classes

	WNDCLASSEX wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)_MainWindowWndProcStub;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	//wcex.hIconSm       = (HICON) LoadImage(m_hInstance,TEXT("AppIcon"),IMAGE_ICON,16, 16,LR_LOADFROMFILE);
	HICON icon  = NULL;
	icon  = (HICON) LoadImage(NULL,TEXT("app.ico"),IMAGE_ICON,16, 16,LR_LOADFROMFILE);
	if (icon)
	{
		wcex.hIcon = icon;
	}else{
		wcex.hIcon			= LoadIcon( m_hInstance, MAKEINTRESOURCE(IDI_VIRTOOLS) );
	}

	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= m_PlayerClass.CStr();
	wcex.hIconSm		= icon;
    
	WNDCLASS MyRenderClass;
	ZeroMemory(&MyRenderClass,sizeof(MyRenderClass));
	MyRenderClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	MyRenderClass.lpfnWndProc	= (WNDPROC)_MainWindowWndProcStub;
	MyRenderClass.hInstance		= m_hInstance;
	MyRenderClass.lpszClassName	= m_RenderClass.CStr();

	::RegisterClass(&MyRenderClass);

	int le  = GetLastError();

	int result = ::RegisterClassEx(&wcex);

	le  = GetLastError();

	int h = 2 ; 

	return result;

}

void CCustomPlayer::_SetResolutions()
{
	// retrieve the windowed attribute
	CKParameterOut* pout = m_Level->GetAttributeParameter(m_WindowedResolutionAttType);
	if (pout) {
		Vx2DVector res(m_WindowedWidth,m_WindowedHeight);
		// set it
		pout->SetValue(&res);
	}

	// retrieve the fullscreen attribute
	pout = m_Level->GetAttributeParameter(m_FullscreenResolutionAttType);
	if (pout) {
		Vx2DVector res(m_FullscreenWidth,m_FullscreenHeight);
		// set it
		pout->SetValue(&res);
	}

	// retrieve the fullscreen bpp attribute
	pout = m_Level->GetAttributeParameter(m_FullscreenBppAttType);
	if (pout) {
		// set it
		pout->SetValue(&m_FullscreenBpp);
	}
}
BOOL CCustomPlayer::ChangeResolution()
{
	if (!m_RenderContext)
		return FALSE;

	if (m_RenderContext->IsFullScreen()) {
		// we are in fullscreen mode
		if (_GetFullScreenResolution()) {
			// the resolution has changed

			// pause the player
			m_CKContext->Pause();
			// and stop fullscreen
			m_RenderContext->StopFullScreen();

			// return to fullscreen with the new resolution
			m_RenderContext->GoFullScreen(m_FullscreenWidth,m_FullscreenHeight,m_FullscreenBpp,m_Driver);

			VxDriverDesc* Check_API_Desc = m_RenderManager->GetRenderDriverDescription(m_Driver);
			//we have to resize the mainwin to allow correct picking (only in DX)
			if (Check_API_Desc->Caps2D.Family==CKRST_DIRECTX && m_RenderContext->IsFullScreen()) {
				//store current size
				GetWindowRect(m_MainWindow,&m_MainWindowRect);

				//Resize the window
				::SetWindowPos(m_MainWindow,HWND_TOPMOST,0,0,m_FullscreenWidth,m_FullscreenHeight,NULL);

				//Prevent the window from beeing resized
				LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
				st&=~WS_THICKFRAME;
				st&=~WS_SIZEBOX;
				SetWindowLong(m_MainWindow,GWL_STYLE,st);
			}	
			// everything is ok to restart the player
			m_CKContext->Play();
		}
	} else {
		// we are in windowed mode	
		if (_GetWindowedResolution()) {
			// the resolution has changed

			//allow the window to be resized
			LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
			st|=WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_MainWindow,GWL_STYLE,st);

			//reposition the window
			m_MainWindowRect.left = (GetSystemMetrics(SM_CXSCREEN)-m_WindowedWidth)/2;
			m_MainWindowRect.right = m_WindowedWidth+m_MainWindowRect.left;
			m_MainWindowRect.top = (GetSystemMetrics(SM_CYSCREEN)-m_WindowedHeight)/2;
			m_MainWindowRect.bottom = m_WindowedHeight+m_MainWindowRect.top;
			BOOL ret = AdjustWindowRect(&m_MainWindowRect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);
			::SetWindowPos(m_MainWindow,0,m_MainWindowRect.left,m_MainWindowRect.top,m_MainWindowRect.right - m_MainWindowRect.left,m_MainWindowRect.bottom - m_MainWindowRect.top,NULL);

			// and set the position of the render window in the main window
			::SetWindowPos(m_RenderWindow,NULL,0,0,m_WindowedWidth,m_WindowedHeight,SWP_NOMOVE|SWP_NOZORDER);
			m_RenderContext->Resize(0,0,m_WindowedWidth,m_WindowedHeight);
		}
	}

	return TRUE;
}

BOOL CCustomPlayer::ClipMouse(BOOL iEnable)
{
	// manage the mouse clipping

	if (!GetPlayer().GetPAppStyle()->IsRenderering())
		return false;
	if(iEnable==FALSE) {
		// disable the clipping
		return ClipCursor(NULL);
	}

	if (!m_RenderContext) {
		return FALSE;
	}

	// retrieve the render window rectangle
	VxRect r;
	m_RenderContext->GetWindowRect(r,TRUE);

	RECT rect;
	rect.top = (LONG)r.top;
	rect.left = (LONG)r.left;
	rect.bottom = (LONG)r.bottom;
	rect.right = (LONG)r.right;

	// to clip the mouse in it.
	return ClipCursor(&rect);
}

BOOL CCustomPlayer::SwitchFullscreen(BOOL value)
{

	if (!m_RenderContext || !m_FullscreenEnabled) {
		return FALSE;
	}

	// mark eat display change to true
	// so we cannot switch the display during this function.
	m_EatDisplayChange = TRUE;

	if ( !value && m_RenderContext->IsFullScreen() ) {
		// siwtch to windowed mode

		// retrieve the windowed resolution from the attributes
		_GetWindowedResolution();
		// we pause the player
		m_CKContext->Pause();
		// stop the fullscreen
		m_RenderContext->StopFullScreen();

		//restore the main window size (only in DirectX rasterizer->m_MainWindowRect.bottom not modified)
		if (m_MainWindowRect.bottom!=0 && !m_RenderContext->IsFullScreen()) {
			//allow the window to be resized
			LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
			st|=WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_MainWindow,GWL_STYLE,st);			
		}

		//reposition the window
		m_MainWindowRect.left = (GetSystemMetrics(SM_CXSCREEN)-m_WindowedWidth)/2;
		m_MainWindowRect.right = m_WindowedWidth+m_MainWindowRect.left;
		m_MainWindowRect.top = (GetSystemMetrics(SM_CYSCREEN)-m_WindowedHeight)/2;
		m_MainWindowRect.bottom = m_WindowedHeight+m_MainWindowRect.top;
		BOOL ret = AdjustWindowRect(&m_MainWindowRect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);
		::SetWindowPos(m_MainWindow,HWND_NOTOPMOST,m_MainWindowRect.left,m_MainWindowRect.top,m_MainWindowRect.right - m_MainWindowRect.left,m_MainWindowRect.bottom - m_MainWindowRect.top,NULL);

		// now we can show the main widnwos
		ShowWindow(m_MainWindow,SW_SHOW);
		SetFocus(m_MainWindow);

		// and set the position of the render window in the main window
		::SetWindowPos(m_RenderWindow,NULL,0,0,m_WindowedWidth,m_WindowedHeight,SWP_NOMOVE|SWP_NOZORDER);

		m_RenderContext->Resize(0,0,m_WindowedWidth,m_WindowedHeight);

		// and give the focus to the render window
		SetFocus(m_RenderWindow);

		// everything is ok to restart the player
		m_CKContext->Play();
	} 
	else if(value && !m_RenderContext->IsFullScreen())
	{
		// switch to fullscreen mode

		// retrieve the fullscreen resolution from the attributes
		_GetFullScreenResolution();
		// we pause the player
		m_CKContext->Pause();
		// and go fullscreen
		m_RenderContext->GoFullScreen(m_FullscreenWidth,m_FullscreenHeight,m_FullscreenBpp,m_Driver);

		// everything is ok to restart the player
		m_CKContext->Play();

		VxDriverDesc* Check_API_Desc = m_RenderManager->GetRenderDriverDescription(m_Driver);
		//we have to resize the mainwin to allow correct picking (only in DX)
		if (Check_API_Desc->Caps2D.Family==CKRST_DIRECTX && m_RenderContext->IsFullScreen()) {
			//store current size
			GetWindowRect(m_MainWindow,&m_MainWindowRect);

			//Resize the window
			::SetWindowPos(m_MainWindow,HWND_TOPMOST,0,0,m_FullscreenWidth,m_FullscreenHeight,NULL);

			//Prevent the window from beeing resized
			LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
			st&=~WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_MainWindow,GWL_STYLE,st);
		}
	} 

	// mark eat display change to false
	// as we have finished
	m_EatDisplayChange = FALSE;

	ClipMouse(m_MouseClipped);

	return TRUE;
}

BOOL CCustomPlayer::SwitchFullscreen()
{
		if (!m_RenderContext || !m_FullscreenEnabled) {
		return FALSE;
	}

	// mark eat display change to true
	// so we cannot switch the display during this function.
	m_EatDisplayChange = TRUE;

	if (m_RenderContext->IsFullScreen()) {
		// siwtch to windowed mode

		// retrieve the windowed resolution from the attributes
		_GetWindowedResolution();
		// we pause the player
		m_CKContext->Pause();
		// stop the fullscreen
		m_RenderContext->StopFullScreen();

		//restore the main window size (only in DirectX rasterizer->m_MainWindowRect.bottom not modified)
		if (m_MainWindowRect.bottom!=0 && !m_RenderContext->IsFullScreen()) {
			//allow the window to be resized
			LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
			st|=WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_MainWindow,GWL_STYLE,st);			
		}

		//reposition the window
		m_MainWindowRect.left = (GetSystemMetrics(SM_CXSCREEN)-m_WindowedWidth)/2;
		m_MainWindowRect.right = m_WindowedWidth+m_MainWindowRect.left;
		m_MainWindowRect.top = (GetSystemMetrics(SM_CYSCREEN)-m_WindowedHeight)/2;
		m_MainWindowRect.bottom = m_WindowedHeight+m_MainWindowRect.top;
		BOOL ret = AdjustWindowRect(&m_MainWindowRect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);
		::SetWindowPos(m_MainWindow,HWND_NOTOPMOST,m_MainWindowRect.left,m_MainWindowRect.top,m_MainWindowRect.right - m_MainWindowRect.left,m_MainWindowRect.bottom - m_MainWindowRect.top,NULL);

		// now we can show the main widnwos
		ShowWindow(m_MainWindow,SW_SHOW);
		SetFocus(m_MainWindow);

		// and set the position of the render window in the main window
		::SetWindowPos(m_RenderWindow,NULL,0,0,m_WindowedWidth,m_WindowedHeight,SWP_NOMOVE|SWP_NOZORDER);

		// and give the focus to the render window
		SetFocus(m_RenderWindow);

		// everything is ok to restart the player
		m_CKContext->Play();
	} else {
		// switch to fullscreen mode

		// retrieve the fullscreen resolution from the attributes
		_GetFullScreenResolution();
		// we pause the player
		m_CKContext->Pause();
		// and go fullscreen
		m_RenderContext->GoFullScreen(m_FullscreenWidth,m_FullscreenHeight,m_FullscreenBpp,m_Driver);

		// everything is ok to restart the player
		m_CKContext->Play();

		VxDriverDesc* Check_API_Desc = m_RenderManager->GetRenderDriverDescription(m_Driver);
		//we have to resize the mainwin to allow correct picking (only in DX)
		if (Check_API_Desc->Caps2D.Family==CKRST_DIRECTX && m_RenderContext->IsFullScreen()) {
			//store current size
			GetWindowRect(m_MainWindow,&m_MainWindowRect);

			//Resize the window
			::SetWindowPos(m_MainWindow,HWND_TOPMOST,0,0,m_FullscreenWidth,m_FullscreenHeight,NULL);

			//Prevent the window from beeing resized
			LONG st = GetWindowLong(m_MainWindow,GWL_STYLE);
			st&=~WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_MainWindow,GWL_STYLE,st);
		}
	} 

	// mark eat display change to false
	// as we have finished
	m_EatDisplayChange = FALSE;

	ClipMouse(m_MouseClipped);

	return TRUE;
}


BOOL CCustomPlayer::_CheckFullscreenDisplayMode(BOOL iDoBest)
{
	VxDriverDesc* desc = m_RenderManager->GetRenderDriverDescription(m_Driver);

	// try to find the correct fullscreen display mode
	VxDisplayMode* displayMode = NULL;
	for (displayMode=desc->DisplayModes.Begin();displayMode!=desc->DisplayModes.End();displayMode++) {
		if (displayMode->Width==m_FullscreenWidth	&&
			displayMode->Height==m_FullscreenHeight	&&
			displayMode->Bpp==m_FullscreenBpp) {
				m_FullscreenEnabled = TRUE;
				return TRUE;
			}
	}

	if (!iDoBest) {
		return FALSE;
	}

	// we did not find a display mode
	// try 640x480x32
	m_FullscreenWidth = 640;
	m_FullscreenHeight = 480;
	m_FullscreenBpp = 32;

	for (displayMode=desc->DisplayModes.Begin();displayMode!=desc->DisplayModes.End();displayMode++) {
		if (displayMode->Width==m_FullscreenWidth	&&
			displayMode->Height==m_FullscreenHeight	&&
			displayMode->Bpp==m_FullscreenBpp) {
				m_FullscreenEnabled = TRUE;
				return TRUE;
			}
	}

	// we did not find a display mode
	// try 640x480x16
	m_FullscreenBpp = 16;

	for (displayMode=desc->DisplayModes.Begin();displayMode!=desc->DisplayModes.End();displayMode++) {
		if (displayMode->Width==m_FullscreenWidth	&&
			displayMode->Height==m_FullscreenHeight	&&
			displayMode->Bpp==m_FullscreenBpp) {
				m_FullscreenEnabled = TRUE;
				return TRUE;
			}
	}

	m_FullscreenEnabled = FALSE;
	return FALSE;
}


BOOL CCustomPlayer::_GetFullScreenResolution()
{
	// retrieve the fullscreen resolution from attribute

	// retrieve the attribute (resolution width and height)
	CKParameterOut* paramRes = m_Level->GetAttributeParameter(m_FullscreenResolutionAttType);
	if (!paramRes) {
		return FALSE;
	}

	// save old values
	int oldWidth = m_FullscreenWidth;
	int oldHeight = m_FullscreenHeight;
	int oldBpp = m_FullscreenBpp;

	// retrieve the attribute (bpp)
	CKParameterOut* paramBpp = m_Level->GetAttributeParameter(m_FullscreenBppAttType);
	if (paramBpp) {
		paramBpp->GetValue(&m_FullscreenBpp);
	}

	Vx2DVector res(m_FullscreenWidth,m_FullscreenHeight);
	paramRes->GetValue(&res);
	m_FullscreenWidth = (int)res.x;
	m_FullscreenHeight = (int)res.y;

	// check the resolution is compatible with the fullscreen mode
	if (!_CheckFullscreenDisplayMode(FALSE)) {
		// else ...
		m_FullscreenWidth = oldWidth;
		m_FullscreenHeight = oldHeight;
		m_FullscreenBpp = oldBpp;
		// ... reset attributes with old values
		_SetResolutions();
	}

	// returns TRUE if at least one value has changed
	return (m_FullscreenWidth!=oldWidth || m_FullscreenHeight!=oldHeight || m_FullscreenBpp!=oldBpp);
}

BOOL CCustomPlayer::_GetWindowedResolution()
{	
	// retrieve the windowed resolution from attribute

	// retrieve the attribute (resolution width and height)
	CKParameterOut* pout = m_Level->GetAttributeParameter(m_WindowedResolutionAttType);
	if (!pout) {
		return FALSE;
	}

	// save old values
	int oldWidth = m_WindowedWidth;
	int oldHeight = m_WindowedHeight;

	Vx2DVector res(m_WindowedWidth,m_WindowedHeight);
	pout->GetValue(&res);
	m_WindowedWidth = (int)res.x;
	m_WindowedHeight = (int)res.y;

	// returns TRUE if at least one value has changed
	return (m_WindowedWidth!=oldWidth || m_WindowedHeight!=oldHeight);
}



LRESULT CCustomPlayer::OnSysKeyDownMainWindow(int iConfig, int iKey)
{
	// Manage system key (ALT + KEY)
	// system keys can be disable using eDisableKeys

	switch(iKey)
	{
	case VK_RETURN:
		if (!(iConfig&eDisableKeys)) 
		{
			// ALT + ENTER -> SwitchFullscreen
			SwitchFullscreen( !m_RenderContext->IsFullScreen() );
		}
		break;

	case VK_F4:
		if (!(iConfig&eDisableKeys)) {
			// ALT + F4 -> Quit the application
			PostQuitMessage(0);
			return 1;
		}
		break;
	}
	return 0;
}
void CCustomPlayer::OnActivateApp(BOOL iActivate)
{
	// if
	// - the application is being activated (iActivate == TRUE)
	// - the render context is in fullscreen
	// - and the player is not already switching fullscreen (m_EatDisplayChange == FALSE)
	if (iActivate==FALSE && m_RenderContext && m_RenderContext->IsFullScreen() && !m_EatDisplayChange) {
		// we switch fullscreen because the application is deactivated
		//SwitchFullscreen();
		
	}
}

void CCustomPlayer::OnFocusChange(BOOL iFocus)
{
	// here we manage the focus change

	if (!m_CKContext) {
		return;
	}

	///////////////////////
	// First, check minimize/restore
	///////////////////////

	if ( (m_State==ePlaying) && IsIconic(m_MainWindow) ) { // we must pause process
		// the application is minimized
		m_State = eMinimized;
		// so we pause the player
		m_CKContext->Pause();
		return;
	}

	if ( (m_State==eMinimized) && !IsIconic(m_MainWindow) ) { // we must restart process
		// the application is no longer minimized
		m_State = ePlaying;
		// so we restart the player
		m_CKContext->Play();
		return;
	}

	///////////////////////
	// then check focus lost behavior
	///////////////////////

	CKDWORD pauseMode = m_CKContext->GetFocusLostBehavior();

	if(m_State==ePlaying || m_State==eFocusLost) {
		switch (pauseMode)
		{
			// if the composition is configured to pause
			// the input manager when the focus is lost
			// note: for a stand alone player
			// CK_FOCUSLOST_PAUSEINPUT_MAIN and CK_FOCUSLOST_PAUSEINPUT_PLAYER
			// is the same thing. there is a difference only for the webplayer
		case CK_FOCUSLOST_PAUSEINPUT_MAIN:
		case CK_FOCUSLOST_PAUSEINPUT_PLAYER:
			// we pause/unpause it depending on the focus
			m_InputManager->Pause(!iFocus);
			break;

			// if the composition is configured to pause
			// the player (pause all) when the focus is lost
		case CK_FOCUSLOST_PAUSEALL:
			if (!iFocus) {
				// focus lost
				m_State = eFocusLost;
				// pause the player
				m_CKContext->Pause();	
			} else {
				// else
				m_State = ePlaying;
				// play
				m_CKContext->Play();
			}
			break;
		}
	}
}

void CCustomPlayer::OnMouseClick(int iMessage)
{
	// here we manage the mouse click

	if (!m_RenderContext) {
		return;
	}

	// retrieve the cursor position
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_RenderWindow,&pt);

	// convert the windows message to the virtools message
	int msg = (iMessage==WM_LBUTTONDOWN)?m_MsgClick:m_MsgDoubleClick;

	// retrieve information about object "under" the mouse
	VxIntersectionDesc desc;
	CKObject* obj = m_RenderContext->Pick(pt.x,pt.y,&desc);
	if(obj && CKIsChildClassOf(obj,CKCID_BEOBJECT)) {
		// send a message to the beobject
		m_MessageManager->SendMessageSingle(msg,(CKBeObject*)obj);
	}

	if (desc.Sprite) {
		// send a message to the sprite
		m_MessageManager->SendMessageSingle(msg,(CKBeObject *)desc.Sprite);
	}
}

void CCustomPlayer::OnPaint()
{
	if (!m_RenderContext || m_RenderContext->IsFullScreen()) {
		return;
	}
	// in windowed mode call render when WM_PAINT
	m_RenderContext->Render();
}
