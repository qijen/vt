
/********************************************************************
	created:	2007/11/28
	created:	28:11:2007   16:25
	filename: 	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Dialogs\CustomPlayerConfigurationDialog.cpp
	file path:	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Dialogs
	file base:	CustomPlayerConfigurationDialog
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/
#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"
#include "xSplash.h"
#include "SplashScreenEx.h"


//************************************
// Method:    HideSplash
// FullName:  CCustomPlayer::HideSplash
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CCustomPlayer::HideSplash()
{
	if (xSplash::GetSplash())
	{
		xSplash::HideSplash();
	}
}
//************************************
// Method:    SetSplashText
// FullName:  CCustomPlayer::SetSplashText
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const char* text
//************************************
void CCustomPlayer::SetSplashText(const char* text)
{
	if (xSplash::GetSplash())
	{
		xSplash::SetText(text);
	}
	
}

//************************************
// Method:    ShowSplash
// FullName:  CCustomPlayer::ShowSplash
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void CCustomPlayer::ShowSplash()
{
	CWnd *main = CWnd::FromHandle(m_MainWindow);

	xSplash::CreateSplashEx(main,40,10);

	//////////////////////////////////////////////////////////////////////////
	// we modify our splash : 
	CSplashScreenEx *splash  = xSplash::GetSplash();
	/*
	// we set the loading perc to the right bottom corner : 
	splash->SetTextFormat(DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
	
	//we set the splash file and the transparency key : 
	splash->SetBitmap("splash.bmp",255,0,255);

	//font : 
	splash->SetTextFont("MicrogrammaDBolExt",100,CSS_TEXT_NORMAL);*/

	if (xSplash::GetSplash())
	{
		xSplash::ShowSplash();
	}
}