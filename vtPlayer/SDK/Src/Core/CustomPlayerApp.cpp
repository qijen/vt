
/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   12:08
	filename: 	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer\CustomPlayerApp.cpp
	file path:	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer
	file base:	CustomPlayerApp
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayerDefines.h"
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"

#include "xSplash.h"
// the unique (global) instance of the winapp
CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;

#include <xUtils.h>
#include "CustomPlayerDialog.h"


BEGIN_MESSAGE_MAP(CCustomPlayerApp, CWinApp)
	//{{AFX_MSG_MAP(CCustomPlayerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
//
// CCustomPlayerApp: PUBLIC METHODS
//
////////////////////////////////////////////////////////////////////////////////
//************************************
// Method:    GetInstance
// FullName:  CCustomPlayerApp::GetInstance
// Access:    protected static 
// Returns:   CCustomPlayerApp*
// Qualifier:
//************************************

CCustomPlayerApp*
CCustomPlayerApp::GetInstance()
{

	if (theApp)
	{
		return &theApp;
	} 
	else
	{
		return NULL;
	}

}
CCustomPlayerApp::CCustomPlayerApp()
:	m_MainWindow(0),m_RenderWindow(0),
	m_PlayerClass(MAINWINDOW_CLASSNAME),m_RenderClass(RENDERWINDOW_CLASSNAME),
	m_PlayerTitle(MAINWINDOW_TITLE),
	m_Config(0)
{
}

CCustomPlayerApp::~CCustomPlayerApp()
{
	// simply destroy the windows ...

	if (m_RenderWindow) {
		DestroyWindow(m_RenderWindow);
	}

	if (m_MainWindow) {
		DestroyWindow(m_MainWindow);
	}
}



int CCustomPlayerApp::ExitInstance()
{
	if (thePlayer) {
		delete thePlayer;
		thePlayer = 0;
	}

	return CWinApp::ExitInstance();
}


////////////////////////////////////////////////////////////////////////////////
//
// CCustomPlayerApp: PROTECTED/PRIVATE METHODS
//
////////////////////////////////////////////////////////////////////////////////

void CCustomPlayerApp::_DisplaySplashWindow()
{
	// display the splash windows centered.
/*	RECT rect;
	m_Splash = CreateDialog(theApp.m_hInstance,(LPCTSTR)IDD_SPLASH,NULL,(DLGPROC)_LoadingDlgWndProc); 
	GetWindowRect(m_Splash,&rect);
	SetWindowPos(m_Splash,NULL,(GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))/2,
				 (GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))/2,0,0,SWP_NOZORDER|SWP_NOSIZE);
	ShowWindow(m_Splash, SW_SHOW);
	UpdateWindow(m_Splash);
	*/
}

void CCustomPlayerApp::_PublishingRights()
{
	// IMPORTANT: The following warning should be removed by you (in the source
	// code prior to compilation).
	//
	// This dialog box serves to remind you that publishing rights, a.k.a. runtime
	// fees, are due when building any custom executable (like the player you just
	// compiled). Contact info@virtools.com for more information.

	MessageBox(NULL,PUBLISHING_RIGHT_TEXT,PUBLISHING_RIGHT_TITLE,MB_OK|MB_ICONASTERISK);
}



BOOL CCustomPlayerApp::_LoadInternal(XString& oFilename)
{
	// parameters are:
	//   -disable_keys (or -d)    : disable ALT+ENTER to switch from/to fullscreen
	//                              and ALT+F4 to close the application
	//   -auto_fullscreen (or -f) : the player start automatically in fullscreen mode
	//   -file filename           : specify the file to load
	//   -title title             : specify the title of the main window
	//   -width width             : specify the width in windowed mode
	//   -height height           : specify the height in windowed mode
	//   -fwidth fullscreenwidth  : specify the width in fullscreen mode
	//   -fheight fullscreeneight : specify the height in fullscreen mode
	//   -fbpp fullscreenbpp      : specify the bit per pixel in fullscreen mode
	//   -rasterizer family,flags : specify the rasterizer family (see CKRST_RSTFAMILY)
	//                              and flags to choose the rasterizer (see CKRST_SPECIFICCAPS)
	// default values are:
	//  - title             = Virtools Custom Player
	//  - width             = 640
	//  - height            = 480
	//  - fullscreen width  = 640
	//  - fullscreen height = 480
	//  - fullscreen bpp    = 32
	//  - rasterizer        = CKRST_DIRECTX,CKRST_SPECIFICCAPS_HARDWARE|CKRST_SPECIFICCAPS_DX9

	//	Please note that if there are "space" characters in a parameter value the value must be between " "

// ***********************************************************************************************************************
		
		//	m_Config |= eAutoFullscreen;  // KAM comment to go to windowed mode, uncomment to go directly to full screen

// ***********************************************************************************************************************

		// parameter followed by values

		// -file
		oFilename = "a.vmo";
		// -title
		m_PlayerTitle = "PlayGen Player";
		// -width
		CCustomPlayer::Instance().WindowedWidth() = 800;
		// -height
		CCustomPlayer::Instance().WindowedHeight() = 600;
		// -fwidth
		CCustomPlayer::Instance().FullscreenWidth() = 800;
		// -fheight
		CCustomPlayer::Instance().FullscreenHeight() = 600;
		// -fbpp
		CCustomPlayer::Instance().FullscreenBpp() = 32;

/*		// -rasterizer
		else if (strncmp(token,"rasterizer",XMin((int)(ptr-token),(int)(sizeof("rasterizer")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			int tmp2 = 0;
			if(sscanf(value.CStr(),"%d,%d",&tmp,&tmp2)==2) {
				CCustomPlayer::Instance().RasterizerFamily() = tmp;
				CCustomPlayer::Instance().RasterizerFlags() = tmp2;
*/

	return TRUE; 
}



BOOL CCustomPlayerApp::_ReadCommandLine(const char* iArguments, XString& oFilename)
{
	// parameters are:
	//   -disable_keys (or -d)    : disable ALT+ENTER to switch from/to fullscreen
	//                              and ALT+F4 to close the application
	//   -auto_fullscreen (or -f) : the player start automatically in fullscreen mode
	//   -file filename           : specify the file to load
	//   -title title             : specify the title of the main window
	//   -width width             : specify the width in windowed mode
	//   -height height           : specify the height in windowed mode
	//   -fwidth fullscreenwidth  : specify the width in fullscreen mode
	//   -fheight fullscreeneight : specify the height in fullscreen mode
	//   -fbpp fullscreenbpp      : specify the bit per pixel in fullscreen mode
	//   -rasterizer family,flags : specify the rasterizer family (see CKRST_RSTFAMILY)
	//                              and flags to choose the rasterizer (see CKRST_SPECIFICCAPS)
	// default values are:
	//  - title             = Virtools Custom Player
	//  - width             = 640
	//  - height            = 480
	//  - fullscreen width  = 640
	//  - fullscreen height = 480
	//  - fullscreen bpp    = 32
	//  - rasterizer        = CKRST_DIRECTX,CKRST_SPECIFICCAPS_HARDWARE|CKRST_SPECIFICCAPS_DX9

	//	Please note that if there are "space" characters in a parameter value the value must be between " "

	XStringTokenizer st(iArguments,"-");
	const char* token = 0;
	const char* ptr = 0;
	const char* ptr2 = 0;
	while (token=st.NextToken(token)) {
		ptr = _NextBlank(token);
		ptr2 = _SkipBlank(ptr);

		// the parameter is not followed by a value
		if (*ptr2=='\0') {
			// -disable_keys (or -d)
			if (strncmp(token,"d",XMin((int)(ptr-token),(int)(sizeof("d")-1)))==0 ||
				strncmp(token,"disable_keys",XMin((int)(ptr-token),(int)(sizeof("disable_keys")-1)))==0) {
				m_Config |= eDisableKeys;
			}
			// -auto_fullscreen (or -f)
			else if (strncmp(token,"f",XMin((int)(ptr-token),(int)(sizeof("f")-1)))==0 ||
					   strncmp(token,"auto_fullscreen",XMin((int)(ptr-token),(int)(sizeof("auto_fullscreen")-1)))==0) {
				m_Config |= eAutoFullscreen;
			} else {
				// unknow parameter
				return FALSE;
			}

			continue;
		}

		// parameter followed by values

		// -file
		if (strncmp(token,"file",XMin((int)(ptr-token),(int)(sizeof("file")-1)))==0) {
			if (!_ComputeParamValue(ptr2,oFilename)) {
				return FALSE;
			}
		}
		// -title
		else if (strncmp(token,"title",XMin((int)(ptr-token),(int)(sizeof("title")-1)))==0) {
			if (!_ComputeParamValue(ptr2,m_PlayerTitle)) {
				return FALSE;
			}
		}
		// -width
		else if (strncmp(token,"width",XMin((int)(ptr-token),(int)(sizeof("width")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			if(sscanf(value.CStr(),"%d",&tmp)==1) {
				CCustomPlayer::Instance().WindowedWidth() = tmp;
			}
		}
		// -height
		else if (strncmp(token,"height",XMin((int)(ptr-token),(int)(sizeof("height")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			if(sscanf(value.CStr(),"%d",&tmp)==1) {
				CCustomPlayer::Instance().WindowedHeight() = tmp;
			}
		}
		// -fwidth
		else if (strncmp(token,"fwidth",XMin((int)(ptr-token),(int)(sizeof("fwidth")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			if(sscanf(value.CStr(),"%d",&tmp)==1) {
				CCustomPlayer::Instance().FullscreenWidth() = tmp;
			}
		}
		// -fheight
		else if (strncmp(token,"fheight",XMin((int)(ptr-token),(int)(sizeof("fheight")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			if(sscanf(value.CStr(),"%d",&tmp)==1) {
				CCustomPlayer::Instance().FullscreenHeight() = tmp;
			}
		}
		// -fbpp
		else if (strncmp(token,"fbpp",XMin((int)(ptr-token),(int)(sizeof("fbpp")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			if(sscanf(value.CStr(),"%d",&tmp)==1) {
				CCustomPlayer::Instance().FullscreenBpp() = tmp;
			}
		}
		// -rasterizer
		else if (strncmp(token,"rasterizer",XMin((int)(ptr-token),(int)(sizeof("rasterizer")-1)))==0) {
			XString value;
			if (!_ComputeParamValue(ptr2,value)) {
				return FALSE;
			}
			int tmp = 0;
			int tmp2 = 0;
			if(sscanf(value.CStr(),"%d,%d",&tmp,&tmp2)==2) {
				CCustomPlayer::Instance().RasterizerFamily() = tmp;
				CCustomPlayer::Instance().RasterizerFlags() = tmp2;
			}
		} else {
			// unknow parameter
			return FALSE;
		}
	}

	return TRUE; 
}

BOOL CCustomPlayerApp::_ReadConfig(XString& oFilename, const char*& oBufferFile, XDWORD& oSize)
{
	const char* cmdLine = GetCommandLine();

	const char* ptr = 0;
	// the command line start with a '"' (it means the first parameters
	// contains at least one space)
	if (*cmdLine=='"') {
		// the first parameter is something like
		// "e:\directory name\customplayer.exe"
		// here we look for the second '"' to be after
		// the first parameter
		ptr = strchr(cmdLine+1,'"');
		if (!ptr) {
			// cannot find it.
			// the command line is invalid
			return FALSE;
		}
		// move on the character after the second '"'
		ptr++;

		ptr = _SkipBlank(ptr);
	} else {
		// if there is no space in the first parameter, the
		// first space we can found is the one after the first parameter
		// if any
		ptr = strchr(cmdLine,' ');
		if (ptr) {
			ptr = _SkipBlank(ptr);
		}
	}

	/*
	if (ptr==0 || *ptr=='\0') {
		// there is no parameter on the command line (excepted the name of the exe)
		// try to read the "internal" configuration
		return _ReadInternalConfig(oBufferFile,oSize);
	}
	*/

	// any argument must begin with a '-'
	if (*ptr!='-') {
		return FALSE;
	}

	// read the command line
	return _ReadCommandLine(ptr,oFilename);
}

BOOL CCustomPlayerApp::_ReadInternalConfig(const char*& oBufferFile, XDWORD& oSize)
{
	// NOTE: the internal configuration is not supported at this time
	// The idea behind "internal" configuration is to embed all data needed by the player
	// in the executable itself. The vmo and the parameters can be happend to the executable file.
	// In ouput oBufferFile must contains the address where the vmo is located in memory and
	// oSize must contains the size of the vmo in memory.
	return FALSE;
}

ATOM CCustomPlayerApp::_RegisterClass()
{
	// register window classes


	WNDCLASSEX wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)_MainWindowWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	HICON icon  = NULL;
	icon  = (HICON) LoadImage(NULL,TEXT("app.ico"),IMAGE_ICON,16, 16,LR_LOADFROMFILE);
	if (icon)
	{
		wcex.hIcon = icon;
	}else{
		wcex.hIcon			= LoadIcon(IDI_VIRTOOLS);
	}
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= m_PlayerClass.CStr();
	wcex.hIconSm		= icon;


	WNDCLASS MyRenderClass;
	ZeroMemory(&MyRenderClass,sizeof(MyRenderClass));
	MyRenderClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	MyRenderClass.lpfnWndProc	= (WNDPROC)_MainWindowWndProc;
	MyRenderClass.hInstance		= m_hInstance;
	MyRenderClass.lpszClassName	= m_RenderClass.CStr();

	::RegisterClass(&MyRenderClass);
	return ::RegisterClassEx(&wcex);
}

#include <WindowsX.h>

void 
CCustomPlayerApp::StartMove(LPARAM lParam)
{
	nMMoveX = LOWORD(lParam);
	nMMoveY = HIWORD(lParam);
}
//////////////////////////////////////////////////////////////////////////
void 
CCustomPlayerApp::DoMMove(LPARAM lParam, WPARAM wParam) {

	if(wParam & MK_LBUTTON) { //if mouse is down; window is being dragged.
		RECT wnrect;
		GetWindowRect(this->m_MainWindow,&wnrect); //get window restraints
		MoveWindow(this->m_MainWindow,wnrect.left+(LOWORD(lParam)-nMMoveX),wnrect.top+(HIWORD(lParam)-nMMoveY),wnrect.right - wnrect.left,wnrect.bottom - wnrect.top,true);
	}
}



LRESULT CALLBACK CCustomPlayerApp::_MainWindowWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message) 
	{

	case WM_DROPFILES:		// Load any dropped file
		{

			/*char FileName[_MAX_PATH];
			HDROP hdrop=(HDROP)wParam;
			DragQueryFile(hdrop,0,FileName,_MAX_PATH);

			GetPlayer().Reset();
			GetPlayer().m_CKContext->ClearAll();
			if (GetPlayer.m_RenderContext)
			{
				//GetPlayer.m_RenderContext->Clear();
			}
			
			
			GetPlayer()._Load(FileName);
			GetPlayer()._FinishLoad();
			break;*/
		}	
		case WM_MOUSEMOVE:
			{
				if (GetPlayer().GetPAppStyle()->g_MouseDrag==1)
				{
					GetApp()->DoMMove(lParam,wParam);
				}
			}
		case WM_ACTIVATEAPP:
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				player.OnActivateApp(wParam);
			}
			break;

		// Minimum size of the player window
		case WM_GETMINMAXINFO:
			// this message is not very useful because
			// the main window of the player is not resizable ...
			// but perhaps it will change so we manage this message.
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				if((LPMINMAXINFO)lParam) {
					((LPMINMAXINFO)lParam)->ptMinTrackSize.x=player.MininumWindowedWidth();
					((LPMINMAXINFO)lParam)->ptMinTrackSize.y=player.MininumWindowedHeight();
				}
			}
			break;

		// Sends a Message "OnClick" or "OnDblClick" if any object is under mouse cursor
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				player.OnMouseClick(message);
				if (GetPlayer().GetPAppStyle()->g_MouseDrag==1)
				{
					GetApp()->StartMove(lParam);
				}
			}
			break;
			
		// Size and focus management
		case WM_SIZE:
			// if the window is maximized or minimized
			// we get/lost focus.
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				if (wParam==SIZE_MINIMIZED) {
					player.OnFocusChange(FALSE);
				} else if (wParam==SIZE_MAXIMIZED) {
					player.OnFocusChange(TRUE);
				}
			}
			break;

		// Manage system key (ALT + KEY)
		case WM_SYSKEYDOWN:	
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				return player.OnSysKeyDownMainWindow(theApp.m_Config,(int)wParam);
			}
			break;

		// Repaint main frame
		case WM_PAINT:
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				player.OnPaint();
			}
			break;

		// The main windows has been closed by the user
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		// Focus management
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
			{
				CCustomPlayer& player = CCustomPlayer::Instance();
				player.OnFocusChange(message==WM_SETFOCUS);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK CCustomPlayerApp::_LoadingDlgWndProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
