#include "CPStdAfx.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

#include "vtTools.h"

#if defined(CUSTOM_PLAYER_STATIC)

#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#endif

#include "xSplash.h"

extern CCustomPlayer*	thePlayer;


#include <WindowsX.h>

void
CCustomPlayer::Terminate(int flags) 
{

	
	try 
	{
		if (!m_CKContext) 
		{
			return;
		}
		m_CKContext->Pause();
		m_CKContext->Reset();
		m_CKContext->ClearAll();
		if (m_EnginePointers.TheRenderManager && m_RenderContext )
		{
			m_EnginePointers.TheRenderManager->DestroyRenderContext(m_RenderContext);
		}
		m_RenderContext = NULL;
		CKCloseContext(m_CKContext);
		CKShutdown();
		m_CKContext = NULL;

		DestroyWindow(m_RenderWindow);
		DestroyWindow(m_MainWindow);
		
		Stop();
		PostQuitMessage(0);
		exit(0);
	} 
	catch(...) 
	{
	}
	//VxGetCurrentDirectory(cDir);
	//bool res  = RunAndForgetProcess2("playerS.exe",cDir ,&ret);
	//MSGBOX("post quit : now ");
	//PostQuitMessage(0);
	//MSGBOX("post quit");

}

void CCustomPlayer::StartMove(LPARAM lParam)
{
	nMMoveX = LOWORD(lParam);
	nMMoveY = HIWORD(lParam);
}
//////////////////////////////////////////////////////////////////////////
void CCustomPlayer::DoMMove(LPARAM lParam, WPARAM wParam) {

	if(wParam & MK_LBUTTON) { //if mouse is down; window is being dragged.
		RECT wnrect;
		GetWindowRect(this->m_MainWindow,&wnrect); //get window restraints
		MoveWindow(this->m_MainWindow,wnrect.left+(LOWORD(lParam)-nMMoveX),wnrect.top+(HIWORD(lParam)-nMMoveY),wnrect.right - wnrect.left,wnrect.bottom - wnrect.top,true);
	}
}

LRESULT CCustomPlayer::_MainWindowWndProcStub(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){	return thePlayer->_MainWindowWndProc(hWnd,uMsg,wParam,lParam);}
LRESULT CALLBACK CCustomPlayer::_MainWindowWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message) 
	{

		case WM_DROPFILES:		// Load any dropped file
		{

			char FileName[_MAX_PATH];
			HDROP hdrop=(HDROP)wParam;
			DragQueryFile(hdrop,0,FileName,_MAX_PATH);
			Reset();
			_Load(FileName);
			_FinishLoad();
			break;
		}	

	case WM_MOUSEMOVE:
		{
			// we allow window dragging enabled in the player.ini file !
			if (GetPAppStyle()->g_MouseDrag==1)
			{
				DoMMove(lParam,wParam);
			}
		}
	case WM_ACTIVATEAPP:
		{
			OnActivateApp(wParam);
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
				((LPMINMAXINFO)lParam)->ptMinTrackSize.x=MininumWindowedWidth();
				((LPMINMAXINFO)lParam)->ptMinTrackSize.y=MininumWindowedHeight();
			}
		}
		break;

		// Sends a Message "OnClick" or "OnDblClick" if any object is under mouse cursor
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		{
			
			OnMouseClick(message);

			//  [2/18/2008 mc007]
			// we allow window dragging enabled in the player.ini file !
			if (GetPAppStyle()->g_MouseDrag)
			{
				StartMove(lParam);
			}
			
		}
		break;

		// Size and focus management
	case WM_SIZE:
		// if the window is maximized or minimized
		// we get/lost focus.
		{
			if (wParam==SIZE_MINIMIZED) {
				OnFocusChange(FALSE);
			} else if (wParam==SIZE_MAXIMIZED) {
				OnFocusChange(TRUE);
			}
		}
		break;

		// Manage system key (ALT + KEY)
	case WM_SYSKEYDOWN:	
		{
//			return OnSysKeyDownMainWindow(theApp.m_Config,(int)wParam);
		}
		break;

		// Repaint main frame
	case WM_PAINT:
		{
			OnPaint();
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
			OnFocusChange(message==WM_SETFOCUS);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

static DWORD WINAPI runner(LPVOID lpParam){	GetPlayer().DoFrame();	return TRUE;}
void CCustomPlayer::Stop()
{
	if (m_hThread)
	{
		TerminateThread(m_hThread,0);
		Sleep(50);
		m_hThread = NULL;
	}
}

int CCustomPlayer::RunInThread()
{
	DWORD threadId, thirdParam = 0;
	
	m_hThread = CreateThread(
		NULL,			     // no security attributes
		0,			     // use default stack size
		runner,	     // thread function
		&thirdParam,		     // argument to thread function
		0,			     // use default creation flags
		&threadId);		     // returns the thread identifier

	return 123;
}
int CCustomPlayer::Tick()
{

	MSG msg;
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);

	return Process(m_Config);
}
#include <stdio.h>
#include <conio.h>

HRESULT CCustomPlayer::DoFrame() 
{

	BOOL bGotMsg;
	MSG msg;
	msg.message = WM_NULL;

	while ( msg.message != WM_QUIT )
	{		
		if (kbhit())
		{
			char message[400];
			gets(message);
			if (!strcmp(message,"exit"))
			{
                Terminate(0);
			}
			SendMessage("Level","ConsoleMessage",1,1,1,message);
                        
			while (kbhit()){
				getch();
			}
		}

		bGotMsg = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		if( bGotMsg )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

		} else Process(m_Config);
	}
	return S_OK;
}



CKERROR LogRedirect(CKUICallbackStruct& loaddata,void*) 
{
	if(loaddata.Reason == CKUIM_OUTTOCONSOLE || 
		loaddata.Reason 	== CKUIM_OUTTOINFOBAR ||
		loaddata.Reason 	== CKUIM_DEBUGMESSAGESEND	 )  
	{ 

		
		//printf ("oldString : %s\n",GetPlayer().m_oldText.CStr());
		//printf ("newString : %s\n",loaddata.ConsoleString);
		if (GetPlayer().m_oldText.Compare(loaddata.ConsoleString))
		{
			printf ("%s\n",loaddata.ConsoleString);
			GetPlayer().m_oldText = loaddata.ConsoleString;

			//GetPlayer().m_CKContext->OutputToConsoleEx("log:%s",GetPlayer().m_oldText);

		}
		//char buffer[MAX_PATH];
		/*if (GetPlayer().m_oldText.Compare(loaddata.ConsoleString))
		{
			//printf (loaddata.ConsoleString);
			GetPlayer().m_oldText = loaddata.ConsoleString;
		}*/
	} 

	return CK_OK; 
}
void CCustomPlayer::RedirectLog()
{


	m_oldText ="";
	m_CKContext->SetInterfaceMode(FALSE,LogRedirect,NULL);
}
CKERROR LoadCallBack(CKUICallbackStruct& loaddata,void*) { 

	if (GetPlayer().GetPAppStyle()->UseSplash() && GetPlayer().GetPAppStyle()->ShowLoadingProcess())
	{
		if(loaddata.Reason == CKUIM_LOADSAVEPROGRESS ){ 
			if (loaddata.NbObjetsLoaded % 10  == 0)
			{
				float progress = (static_cast<float>(loaddata.NbObjetsLoaded) / static_cast<float>(loaddata.NbObjetsToLoad)) ; 
				progress *=100;
				int out = static_cast<int>(progress);
				XString percStr;
				percStr.Format("Load... %d %s",out,"%");
				if (xSplash::GetSplash())
				{
					GetPlayer().SetSplashText(percStr.Str());
					if (progress > 99.9F)
					{
						GetPlayer().HideSplash();
						//Sleep(3000);
					}
				}
			}
		}
	}
	return CK_OK; 
}
void CCustomPlayer::Reset()
{
	// mark the player as playing
	m_State = ePlaying;
	// reset
	m_CKContext->Reset();
	// and play
	m_CKContext->Play();
}
void CCustomPlayer::Pause()
{
	// mark the play as paused
	m_State = ePlaused;
	// pause
	m_CKContext->Pause();
}

void CCustomPlayer::Play()
{
	// mark the player as playing
	m_State = ePlaying;
	// play
	m_CKContext->Play();
}

BOOL CCustomPlayer::Process(int iConfig)
{
	// just to be sure we check the player is ready and playing
	if (!m_CKContext || !m_CKContext->IsPlaying()) {
		Sleep(10);
		return TRUE;
	}

	float beforeRender	= 0;
	float beforeProcess	= 0;
	// retrieve the time to wait in milliseconds to perfom a rendering or a process loop
	// so that the time manager limits are respected.
	m_TimeManager->GetTimeToWaitForLimits(beforeRender,beforeProcess);

	if (beforeProcess<=0) {
		// ok to process
		m_TimeManager->ResetChronos(FALSE,TRUE);
		m_CKContext->Process();
	}

	if (beforeRender<=0) {
		// ok to render
		m_TimeManager->ResetChronos(TRUE,FALSE);
		if (GetPlayer().GetPAppStyle()->IsRenderering())
			m_RenderContext->Render();
	}

	// now we check if the composition has not change the state of a "control" attribute

	// quit has been set
	if (m_Level->HasAttribute(m_QuitAttType)) {
		// we remove it (so it is not "here" the next frame)
		m_Level->RemoveAttribute(m_QuitAttType);
//		MessageBox(NULL,"","a",1);
		// and return FALSE to quit
		return FALSE;
	}

	// switch fullscreen has been set
	if (m_Level->HasAttribute(m_SwitchFullscreenAttType)) 
	{
		//bool goFS;// = vtTools::AttributeTools::GetValueFromAttribute<bool>(m_Level,m_SwitchFullscreenAttType);
		/*CKParameterOut* pout = m_Level->GetAttributeParameter(m_SwitchFullscreenAttType);
		if (!pout) {
			return FALSE;
		}*/
		// we remove it (so it is not "here" the next frame)
		m_Level->RemoveAttribute(m_SwitchFullscreenAttType);
		// and switch fullscreen (ie goes from fullscreen to windowed
		// or to windowed from fullscreen)
//		if(goFS)
		if (GetPlayer().GetPAppStyle()->IsRenderering())
			SwitchFullscreen();

	}

	// switch resolution has been set
	if (m_Level->HasAttribute(m_SwitchResolutionAttType)) {
		// we remove it (so it is not "here" the next frame)
		m_Level->RemoveAttribute(m_SwitchResolutionAttType);
		// and we change the resolution
		ChangeResolution();
	}

	// switch resolution has been set
	if (m_Level->HasAttribute(m_SwitchMouseClippingAttType)) {
		// we remove it (so it is not "here" the next frame)
		m_Level->RemoveAttribute(m_SwitchMouseClippingAttType);
		m_MouseClipped = !m_MouseClipped;
		ClipMouse(m_MouseClipped);
	}

	return TRUE;
}

