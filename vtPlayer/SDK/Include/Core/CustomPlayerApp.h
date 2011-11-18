#if !defined(CUSTOMPLAYERAPP_H)
#define CUSTOMPLAYERAPP_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "CustomPlayerStructs.h"
#include "BaseMacros.h"

/*************************************************************************
Summary: This class defines the Windows application object.

Description: This class provides member functions for initializing the
application and for running the application.

See also: CCustomPlayerApp::InitInstance, CCustomPlayerApp::Run,
CCustomPlayer.
*************************************************************************/
class CCustomPlayerApp : public CWinApp
{
public:

	/*************************************************************************
	Summary: Initialize class members.
	*************************************************************************/
	CCustomPlayerApp();

	/*************************************************************************
	Summary: Release window handles.
	*************************************************************************/
	~CCustomPlayerApp();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomPlayerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

public:

	//{{AFX_MSG(CCustomPlayerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	HWND		m_MainWindow;	// the main window (visible in windowed mode).
	HWND		m_RenderWindow;	// the render window (used for windowed and fullscreen mode).
	
	//HWND		m_Splash;		// the window used to display the splash screen.
	XString		m_PlayerClass;	// the name of the windows class used for the main window.
	XString		m_RenderClass;	// the name of the windows class used for the render window.
	XString		m_PlayerTitle;	// the string display in the title bar of the main window.
	int			m_Config;		// the configuration of the player (see EConfigPlayer).
	HACCEL		m_hAccelTable;

	

	////////////////////////////////////////
	// initialization function

	ATOM	_RegisterClass();
	int	_CreateWindows();
	void	_DisplaySplashWindow();
	void	_PublishingRights();

	////////////////////////////////////////
	// configurations management

	BOOL		_ReadConfig(XString& oFilename, const char*& oBufferFile,XDWORD& oSize);
	BOOL		_ReadInternalConfig(const char*& oBufferFile, XDWORD& oSize);
	BOOL		_ReadCommandLine(const char* iArguments, XString& oFilename);
	BOOL		_LoadInternal(XString& oFilename);









	//////////////////////////////////////////////////////////////////////////

public:

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
//	USHORT PSaveEngineWindowProperties(const char *configFile,const vtPlayer::Structs::xSEngineWindowInfo& input);
	

	//  [11/28/2007 mc007]-	Dialog functions to modify the render settings of the application
	//void DoConfig();
	//INT_PTR CALLBACK ConfigureDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
	

	//////////////////////////////////////////////////////////////////////////
	//
	//	this is just for handy writing : 
	static CCustomPlayerApp* GetInstance();
	


	//////////////////////////////////////////////////////////////////////////





	// _NextBlank, _SkipBlank and _ComputeParamValue are tools used
	// while parsing the command line.
	const char*	_NextBlank(const char* iStr);
	const char* _SkipBlank(const char* iPtr);
	BOOL		_ComputeParamValue(const char* iPtr, XString& oValue);


	////////////////////////////////////////
	// windocprocs

	// main windowproc
	static LRESULT CALLBACK _MainWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// splash screen windowproc
	static LRESULT CALLBACK _LoadingDlgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void StartMove(LPARAM lParam);
	void DoMMove(LPARAM lParam, WPARAM wParam);
	int nMMoveX, nMMoveY; //initial mouse position from window origin.

};


////////////////////////////////////////
// inlines

// _NextBlank, _SkipBlank and _ComputeParamValue are tools used
// while parsing the command line.
inline const char* CCustomPlayerApp::_NextBlank(const char* iStr)
{
	const char* ptr = iStr;
	while(*ptr!='\0' && *ptr!=' ' && *ptr!='\t') {
		ptr++;
	}
	return ptr;
}

inline const char* CCustomPlayerApp::_SkipBlank(const char* iPtr)
{
	while(*iPtr!='\0' && (*iPtr==' ' || *iPtr=='\t')) {
		iPtr++;
	}
	return iPtr;
}

inline BOOL CCustomPlayerApp::_ComputeParamValue(const char* iPtr, XString& oValue)
{
	const char* tmp = 0;
	if (*iPtr=='"') {
		tmp = strchr(iPtr+1,'"');
		if (tmp==0) {
			return FALSE;
		}
		++iPtr;
	} else {
		tmp = _NextBlank(iPtr);
	}

	oValue.Create(iPtr,tmp-iPtr);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////

#define  GetApp() CCustomPlayerApp::GetInstance()

#endif // CUSTOMPLAYERAPP_H
