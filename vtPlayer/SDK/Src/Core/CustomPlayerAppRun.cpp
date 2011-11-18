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

#include "vtTools.h"

#if defined(CUSTOM_PLAYER_STATIC)
// include file used for the static configuration
#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#endif

#include "xSplash.h"


extern CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;

BOOL CCustomPlayerApp::InitInstance()
{

	// register the windows class the main and the render window
	_RegisterClass();


	// read the configuration

	const char* fileBuffer = 0;
	XDWORD fileSize = 0;

	// read the configuration
	//XString filename;

	// retrieve the unique instance of the player (CCustomPlayer class)
	CCustomPlayer& player = CCustomPlayer::Instance();




	player.PLoadEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE);
	player.PLoadEnginePathProfile(CUSTOM_PLAYER_CONFIG_FILE);

	player.PLoadAppStyleProfile(CUSTOM_PLAYER_CONFIG_FILE);


	player.m_AppMode  =  player.PGetApplicationMode(GetCommandLine()); // sets player.m_AppMode = full;


	//m_Config |= eAutoFullscreen;
	// create the main and the render window

	//we only create windows when we want to render something !
	
	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if(!_CreateWindows()) 
		{
			MessageBox(NULL,FAILED_TO_CREATE_WINDOWS,INIT_ERROR,MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	XString filename(GetPlayer().GetEPathProfile()->CompositionFile);



	if(!_ReadConfig(filename,fileBuffer,fileSize)) {
		/*MessageBox(NULL,CANNOT_READ_CONFIG,INIT_ERROR,MB_OK|MB_ICONERROR);*/
		//return FALSE;
	}


	//XString filename("tetris1.cmo");

	// initialize the player
	if(!player.InitPlayer(m_MainWindow,m_RenderWindow,m_Config,filename.Length()==0?fileBuffer:filename.CStr(),filename.Length()==0?fileSize:0)) 
	{
		return FALSE;
	}



	//  [12/17/2007 mc007] 
	// as the player is ready we destroy the splash screen window
	//DestroyWindow(m_Splash);


	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if (GetPlayer().GetEWindowInfo()->g_GoFullScreen) 
		{
			// we are a auto fullscreen mode
			// so we hide the main window
			ShowWindow(m_MainWindow,SW_HIDE);
			UpdateWindow(m_MainWindow);
			// we show the render window
			ShowWindow(m_RenderWindow,theApp.m_nCmdShow);
			UpdateWindow(m_RenderWindow);
			// and set the focus to it
			SetFocus(m_RenderWindow);
			GetPlayer().m_EatDisplayChange = TRUE;
		} else {
			// we are in windowed mode
			// so we show the main window
			ShowWindow(m_MainWindow,theApp.m_nCmdShow);
			UpdateWindow(m_MainWindow);
			// the render window too
			ShowWindow(m_RenderWindow,theApp.m_nCmdShow);
			UpdateWindow(m_RenderWindow);
			// and set the focus to it
			SetFocus(m_RenderWindow);
		}
	}
	// we reset the player to start it
	player.Reset();

	///////////////////////////////////////////////////////////////////////////
	// NOTE: other important things to read after initialization
	//   - the main loop of the player is executed by CCustomPlayerApp::Run
	//   - the windowproc of the main and render window is
	//     CCustomPlayerApp::_MainWindowWndProc
	//   - but the main part of the application is implemented by the
	//     CCustomPlayer class.
	///////////////////////////////////////////////////////////////////////////

	return TRUE;
}

int CCustomPlayerApp::Run() 
{
	MSG msg; 

	while(1)  {
		if(PeekMessage(&msg, NULL,0,0,PM_REMOVE)) {
			if(msg.message==WM_QUIT) {
				return CWinApp::Run();
			}
			else if(!TranslateAccelerator(msg.hwnd,m_hAccelTable,&msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			// process the player
			if(!CCustomPlayer::Instance().Process(m_Config)) {
				PostQuitMessage(0);
				return CWinApp::Run();
			}
		}
	}

	return msg.wParam;
}