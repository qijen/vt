/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

#if defined(CUSTOM_PLAYER_STATIC)
// include file used for the static configuration
#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#endif

#include "vtTools.h"

extern CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;

BOOL CCustomPlayerApp::_CreateWindows()
{
	RECT mainRect;
	int k = CCustomPlayer::Instance().WindowedWidth();
	int k1 = CCustomPlayer::Instance().WindowedHeight();

	// compute the main window rectangle, so the window is centered
	mainRect.left = (GetSystemMetrics(SM_CXSCREEN)-CCustomPlayer::Instance().WindowedWidth())/2;
	mainRect.right = CCustomPlayer::Instance().WindowedWidth()+mainRect.left;
	mainRect.top = (GetSystemMetrics(SM_CYSCREEN)-CCustomPlayer::Instance().WindowedHeight())/2;
	mainRect.bottom = CCustomPlayer::Instance().WindowedHeight()+mainRect.top;
	BOOL ret = AdjustWindowRect(&mainRect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);

	DWORD dwStyle;

	// create the main window

	switch(GetPlayer().PGetApplicationMode())
	{
		case normal:
			{
				m_MainWindow = CreateWindow(m_PlayerClass.CStr(),GetPlayer().GetPAppStyle()->g_AppTitle.Str(),(WS_EX_ACCEPTFILES|WS_OVERLAPPEDWINDOW) & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
					mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
					NULL,NULL,m_hInstance,NULL);
				//m_MainWindow = CreateWindowEx(WS_EX_ACCEPTFILES|WS_EX_TOPMOST,m_PlayerClass.CStr(),GetPlayer().GetPAppStyle()->g_AppTitle.Str(),WS_VISIBLE,mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,NULL,NULL,m_hInstance,this);
				break;
			}
		case preview:
			{
				RECT rc;
				GetClientRect( GetPlayer().m_hWndParent, &rc );
				dwStyle = WS_VISIBLE | WS_CHILD;
				AdjustWindowRect( &rc, dwStyle, FALSE );
				m_MainWindow = GetPlayer().m_hWndParent;
				GetPlayer().m_WindowedWidth = rc.right - rc.left ; 
				GetPlayer().m_WindowedHeight = rc.bottom - rc.top;
				m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_VISIBLE)&~WS_CAPTION,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,m_MainWindow,NULL,m_hInstance,this);
			}
			break;
		case popup:
			{
				dwStyle = (WS_OVERLAPPEDWINDOW); 
				//m_MainWindow  = CreateWindowEx( dwStyle , m_PlayerClass.CStr(), GetPlayer().GetPAppStyle()->g_AppTitle.Str(), WS_POPUP, mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top , m_MainWindow, NULL,m_hInstance, NULL );
				m_MainWindow = CreateWindow(m_PlayerClass.CStr(),"asd",WS_POPUP,mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,NULL,NULL,m_hInstance,NULL);
				break;
			}
		default:
				m_MainWindow = CreateWindow(m_PlayerClass.CStr(),GetPlayer().GetPAppStyle()->g_AppTitle.Str(),WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
					mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
					NULL,NULL,m_hInstance,NULL);
		break;
	}



	//	m_MainWindow = CreateWindow(m_PlayerClass.CStr(),"asd",WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX|WS_MAXIMIZEBOX),
	//		mainRect.left,mainRect.top,mainRect.right-mainRect.left,mainRect.bottom-mainRect.top,
	//		NULL,NULL,m_hInstance,NULL);

	if(!m_MainWindow) {
		return FALSE;
	}

	if(GetPlayer().PGetApplicationMode() != preview)
	{

		int  w = CCustomPlayer::Instance().FullscreenWidth();
		int  h = CCustomPlayer::Instance().FullscreenHeight();

		// create the render window
		if (( GetPlayer().GetEWindowInfo()->g_GoFullScreen )) 
		{
			m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_OVERLAPPED|WS_VISIBLE)&~WS_CAPTION,
				0,0,CCustomPlayer::Instance().FullscreenWidth(),CCustomPlayer::Instance().FullscreenHeight(),m_MainWindow,NULL,m_hInstance,0);
		} else {
			m_RenderWindow = CreateWindowEx(WS_EX_TOPMOST,m_RenderClass.CStr(),"",(WS_CHILD|WS_OVERLAPPED|WS_VISIBLE)&~WS_CAPTION,
				0,0,CCustomPlayer::Instance().WindowedWidth(),CCustomPlayer::Instance().WindowedHeight(),m_MainWindow,NULL,m_hInstance,0);
		}
	}


	if(!m_RenderWindow) 
	{
		return FALSE;
	}


	return TRUE;
}
