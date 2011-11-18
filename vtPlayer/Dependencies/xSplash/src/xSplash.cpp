#include "stdafx.h"
#include "xSplash.h"
#include "splashscreenex.h"
#include "CustomPlayerDefines.h"
	
CSplashScreenEx *splash  = NULL;

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

namespace xSplash
{

	CSplashScreenEx *GetSplash()
	{
		return splash;
	}
	//************************************
	// Method:    HideSplash
	// FullName:  xSplash::HideSplash
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void HideSplash()
	{ 
		if (splash)
		{
			splash->Hide() ;
			delete splash;
			splash = NULL;
		}
		
	}

	//************************************
	// Method:    ShowSplash
	// FullName:  xSplash::ShowSplash
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void ShowSplash()
	{ 
		if (splash)
		{
			splash->Show() ;
		}
		
	}

	//************************************
	// Method:    SetText
	// FullName:  xSplash::SetText
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const char* text
	//************************************
	void SetText(const char* text)
	{
		if(splash)
		{
			splash->SetText(text);
		}
	}

	//************************************
	// Method:    CreateSplashEx
	// FullName:  xSplash::CreateSplashEx
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CWnd *parent
	// Parameter: int w
	// Parameter: int h
	//************************************
	void CreateSplashEx(CWnd *parent,int w,int h)
	{

		splash=new CSplashScreenEx();
		splash->Create(parent,NULL,0,CSS_FADEIN | CSS_CENTERSCREEN | CSS_SHADOW|CSS_FADEOUT|CSS_HIDEONCLICK	);
		splash->SetBitmap(CPF_SPLASH_FILE,255,0,255);
		splash->SetTextFont(CPF_SPLASH_TEXT_TYPE,100,CSS_TEXT_NORMAL);
		int Xpos,Ypos;
		Xpos=(GetSystemMetrics(SM_CXSCREEN)-w)/2;
		Ypos=(GetSystemMetrics(SM_CYSCREEN)-h)/2;

		//splash->SetTextRect(CRect(125,60,291,104));
		splash->SetTextColor(RGB(255,251,185));
		splash->SetTextFormat(CPF_SPLASH_TEXT_FORMAT);
		splash->SetFadeInTime(2000);
		splash->SetFadeOutTime(3000);

	}

	//************************************
	// Method:    CreateSplash
	// FullName:  xSplash::CreateSplash
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: HINSTANCE hinst
	// Parameter: int w
	// Parameter: int h
	//************************************
	void CreateSplash(HINSTANCE hinst,int w,int h)
	{

		splash=new CSplashScreenEx();
		splash->Create(NULL,NULL,0,CSS_FADEIN | CSS_CENTERSCREEN | CSS_SHADOW|CSS_FADEOUT|CSS_HIDEONCLICK	);
		splash->SetBitmap("splash.bmp",255,0,255);

		splash->SetTextFont("MicrogrammaDBolExt",100,CSS_TEXT_NORMAL);
		int Xpos,Ypos;
		Xpos=(GetSystemMetrics(SM_CXSCREEN)-w)/2;
		Ypos=(GetSystemMetrics(SM_CYSCREEN)-h)/2;

		//splash->SetTextRect(CRect(125,60,291,104));
		splash->SetTextColor(RGB(255,251,185));
		splash->SetTextFormat(DT_SINGLELINE | DT_LEFT | DT_BOTTOM);
		splash->Show();

		//splash=CreateDialog(hinst,(LPCTSTR)IDD_DIALOG2, NULL, (DLGPROC)About);  
		/*::GetWindowRect(splash->getw,&rc);
		SetWindowPos(splash,NULL,(GetSystemMetrics(SM_CXSCREEN)-(rc.right-rc.left))/2,
		(GetSystemMetrics(SM_CYSCREEN)-(rc.bottom-rc.top))/2,	0,0,SWP_NOZORDER|SWP_NOSIZE);
		ShowWindow(splash, SW_SHOW);
		UpdateWindow(splash);
		*/
	}


	//************************************
	// Method:    About
	// FullName:  xSplash::About
	// Access:    public 
	// Returns:   LRESULT CALLBACK
	// Qualifier:
	// Parameter: HWND hDlg
	// Parameter: UINT message
	// Parameter: WPARAM wParam
	// Parameter: LPARAM lParam
	//************************************
	LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:

			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
		return FALSE;
	}
}//end of namespace xSplash

