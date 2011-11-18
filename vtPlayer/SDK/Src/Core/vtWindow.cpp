#include <CPStdAfx.h>
#include "vtWindow.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

#include "Manager.h"




/************************************************************************/
/* tools                                                                     */
/************************************************************************/
void
vtWindow::ParseCommandLine(const char*cmdLine)
{
	char Dummy[512]="";
	char Dummy2[512]="";
	char Dummy3[512]="";
	char FileName[512]="";
	char Options[8][128]={"","","","","","","",""};
	char Engine[128]="CK2_3D";	// Default render engine

	XString lpCmdLine(cmdLine);
	lpCmdLine.ToLower(); 


	sscanf(lpCmdLine.Str(),"""%s"" -%s -%s -%s -%s -%s -%s -%s -%s",Dummy,Options[0],Options[1],Options[2],Options[3],Options[4],Options[5],Options[6],Options[7]);
	for (int i=0;i<7;i++)
	{
		switch (Options[i][0])
		{
			case 'd':  sscanf(Options[i],"d=%d",&m_Player->GetPAppStyle()->g_Render); break;
			/*
			case 'w':  sscanf(Options[i],"w=%d",&m_ep.g_Width); break;
			case 'h':  sscanf(Options[i],"h=%d",&m_ep.g_Height);  break;
			case 'b':  sscanf(Options[i],"bpp=%d",&m_ep.g_Bpp); break;
			case  'f' : sscanf(Options[i],"f=%d",&m_ep.g_GoFullScreen); break;
			*/
		}
	}
}

/************************************************************************/
/* player control                                                                     */
/************************************************************************/
void vtWindow::Terminate(int flags)
{

	m_Player->Pause();
	m_Player->Terminate(flags);
	if (m_Player) 
	{
		//PostQuitMessage(0);
		delete m_Player;
		m_Player =0;
	}
}

void vtWindow::Pause()
{
	if (m_Player->m_CKContext)
	{
		m_Player->Pause();
	}
}

void vtWindow::Play()
{
	if (m_Player->m_CKContext)
	{
		m_Player->Play();
	}
}

void vtWindow::LoadCompostion(char *file)
{

	Pause();
	if(m_Player->_Load((const char*)file)!=CK_OK) { // else we load it from a file (iData contains the filename)
		MessageBox(NULL,"Unable to load composition from file.","Initialisation Error",MB_OK|MB_ICONERROR);
	}

	if (!m_Player->_FinishLoad()) 
	{
		MessageBox(NULL,"Unable to start  composition from file.","Initialisation Error",MB_OK|MB_ICONERROR);
	}



}


int vtWindow::WndProc(long *hWnd, int uMsg, long * wParam, long* lParam )
{

	if (m_Player && m_Player->m_CKContext->IsPlaying() )
	{
		m_Player->_MainWindowWndProcStub((HWND)hWnd,(UINT)uMsg,(WPARAM)wParam,(LPARAM)lParam);
	}

	return NULL;
}
/************************************************************************/
/*vt to cshap messaging :                                                                       */
/************************************************************************/
void vtWindow::DeleteMessage(int messageID)
{
	int result  = -1;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			cman->DeleteMessage(messageID);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void vtWindow::CleanMessages()
{
	if (m_Player->m_CKContext)
	{
		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			cman->CleanMessages();
			cman->SetHasMessages(false);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
char*vtWindow::GetMessageValueStr(int messageID,int parameterSubID)
{
	char* result  = NULL;
	
	if (m_Player->m_CKContext)
	{
		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetMessageValueStr(messageID,parameterSubID);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
float vtWindow::GetMessageValueFloat(int messageID,int parameterSubID)
{
	float result  = -1.0f;
	if (m_Player->m_CKContext)
	{
		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetMessageValueFloat(messageID,parameterSubID);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::GetMessageValueInt(int messageID,int parameterSubID)
{
	int result  = -1;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetMessageValueInt(messageID,parameterSubID);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
char* vtWindow::GetMessageName(int messageID)
{
	XString result;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			return cman->GetMessageName(messageID);
		}
	}
	return result.Str();
}

//////////////////////////////////////////////////////////////////////////
int vtWindow::HasMessages()
{
	int result  = -1;
	//MessageBox(NULL,"","",1);
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->HasMessages();
		}
	}
	return result;
}

int vtWindow::GetMessageParameterType(int messageID, int parameterSubID)
{
	int result  = -1;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetMessageParameterType(messageID,parameterSubID);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::GetNumParameters(int messageID)
{
	int result  = -1;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetNumParameters(messageID);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::GetNumMessages()
{
	int result  = -1;
	if (m_Player->m_CKContext)
	{

		CSManager* cman =(CSManager*)m_Player->m_CKContext->GetManagerByGuid(INIT_MAN_GUID);
		if (cman)
		{
			result  = cman->GetNumMessages();
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* window specific functions                                                                     */
/************************************************************************/
int vtWindow::UpdateRenderSize(int w,int h)
{
	Pause();
	//m_Player->Stop();

	if (m_Player->m_RenderContext)
	{
		Sleep(300);
		::SetWindowPos(m_Player->m_MainWindow,NULL,0,0,w,h,SWP_NOMOVE);
		::SetWindowPos(m_Player->m_RenderWindow,NULL,0,0,w,h,SWP_NOMOVE);
		m_Player->m_WindowedWidth = w;
		m_Player->m_WindowedHeight = h;
		Sleep(150);
		m_Player->m_RenderContext->Resize(0,0,w,h);
	}
	Sleep(50);
	Play();
	return 1;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::GetWidth(){	return m_Player->m_WindowedWidth;}
//////////////////////////////////////////////////////////////////////////
int vtWindow::GetHeight(){	return m_Player->m_WindowedHeight;}
//////////////////////////////////////////////////////////////////////////
int vtWindow::Show(int ShowFlags) { 	ShowWindow(m_Player->m_RenderWindow,1);	return 1;}
/************************************************************************/
/* sending msg to vt                                                                     */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
int vtWindow::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,char* value)
{

	if (m_Player)
	{
		return m_Player->SendMessage(targetObject,message,id0,id1,id2,value);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,float value)
{

	//MessageBox(NULL,"","",1);
	if (m_Player)
	{
		return m_Player->SendMessage(targetObject,message,id0,id1,id2,value);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int vtWindow::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,int value)
{

	if (m_Player)
	{
		return m_Player->SendMessage(targetObject,message,id0,id1,id2,value);
	}
	return 0;
}

int vtWindow::CreateAsChild(long * parent)
{

	
	
	HWND win  = (HWND)(parent);
	int ptr =  (int)parent;
	/*XString buffer;
	buffer.Format("id:%d",ptr);
	MessageBox(NULL,buffer.Str(),"",1);*/

	m_Player = &CCustomPlayer::Instance();
	HINSTANCE hinst  = (HINSTANCE)GetModuleHandle("vtWindow.dll");
	
	m_Player->m_hInstance = hinst;
	m_Player->m_hWndParent = win;
	
	m_Player->_RegisterClass();
	m_Player->PLoadEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE);
	m_Player->PLoadEnginePathProfile(CUSTOM_PLAYER_CONFIG_FILE);

	m_Player->PLoadAppStyleProfile(CUSTOM_PLAYER_CONFIG_FILE);
	m_Player->m_AppMode  =  preview;

	//we only create windows when we want to render something !
	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if(!GetPlayer()._CreateWindows()) 
		{
			MessageBox(NULL,FAILED_TO_CREATE_WINDOWS,INIT_ERROR,MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}

	const char* fileBuffer = 0;
	XDWORD fileSize = 0;
	XString filename(GetPlayer().GetEPathProfile()->CompositionFile);
	// initialize the player
	if(!m_Player->InitPlayer(m_Player->m_MainWindow,m_Player->m_RenderWindow,m_Player->m_Config,filename.Length()==0?fileBuffer:filename.CStr(),filename.Length()==0?fileSize:0)) 
	{
		return FALSE;
	}
	
	

	m_Player->m_Config = 0 ;
	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if (m_Player->m_Config&eAutoFullscreen) {
			// we are a auto fullscreen mode
			// so we hide the main window
			ShowWindow(m_Player->m_MainWindow,SW_SHOW);
			UpdateWindow(m_Player->m_MainWindow);
			// we show the render window
			ShowWindow(m_Player->m_RenderWindow,1);
			UpdateWindow(m_Player->m_RenderWindow);
			// and set the focus to it
			SetFocus(m_Player->m_RenderWindow);
		} else {
			// we are in windowed mode
			// so we show the main window
			/*ShowWindow(m_Player->m_MainWindow,1);
			UpdateWindow(m_Player->m_MainWindow);
			// the render window too
			ShowWindow(m_Player->m_RenderWindow,1);
			UpdateWindow(m_Player->m_RenderWindow);
			// and set the focus to it
			SetFocus(m_Player->m_RenderWindow);*/
		}
	}

	// we reset the player to start it
	m_Player->Reset();

	m_Player->RedirectLog();



	return true;
}

void vtWindow::Destroy()
{
	if (m_Player) 
	{
		PostQuitMessage(0);
		delete m_Player;
		m_Player =0;
	}
}

int vtWindow::Run()
{
	if (m_Player->m_hThread)
	{
		m_Player->Stop();
	}

	if (m_Player)
	{
		return m_Player->RunInThread();
	}
	return 1;
}
int
vtWindow::DoFrame()
{
	if (m_Player)
	{
		return m_Player->DoFrame();
	}
	return 1;
}
int
vtWindow::Tick()
{
	return m_Player->Tick();
}


//////////////////////////////////////////////////////////////////////////
vtWindow::vtWindow()
{
	m_Player = NULL;
}
//////////////////////////////////////////////////////////////////////////

int
vtWindow::Init()
{

	m_Player = &CCustomPlayer::Instance();
	HINSTANCE hinst  = (HINSTANCE)GetModuleHandle("vtWindow.dll");
	m_Player->m_hInstance = hinst;

	//////////////////////////////////////////////////////////////////////////




	m_Player->_RegisterClass();
	int LastError  = GetLastError();
	m_Player->PLoadEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE);
	m_Player->PLoadEnginePathProfile(CUSTOM_PLAYER_CONFIG_FILE);

	m_Player->PLoadAppStyleProfile(CUSTOM_PLAYER_CONFIG_FILE);
	m_Player->m_AppMode  =  m_Player->PGetApplicationMode(GetCommandLine()); // sets player.m_AppMode = full;

	ParseCommandLine(GetCommandLine());

	//we only create windows when we want to render something !
	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if(!GetPlayer()._CreateWindows()) 
		{
			MessageBox(NULL,FAILED_TO_CREATE_WINDOWS,INIT_ERROR,MB_OK|MB_ICONERROR);
			return FALSE;
		}
	}
	
	const char* fileBuffer = 0;
	XDWORD fileSize = 0;
	XString filename(GetPlayer().GetEPathProfile()->CompositionFile);
	// initialize the player
	if(!m_Player->InitPlayer(m_Player->m_MainWindow,m_Player->m_RenderWindow,m_Player->m_Config,filename.Length()==0?fileBuffer:filename.CStr(),filename.Length()==0?fileSize:0)) 
	{
		return FALSE;
	}

	m_Player->m_Config = 0 ;
	if (GetPlayer().GetPAppStyle()->IsRenderering())
	{
		if (m_Player->m_Config&eAutoFullscreen) {
			// we are a auto fullscreen mode
			// so we hide the main window
			ShowWindow(m_Player->m_MainWindow,SW_SHOW);
			UpdateWindow(m_Player->m_MainWindow);
			// we show the render window
			ShowWindow(m_Player->m_RenderWindow,1);
			UpdateWindow(m_Player->m_RenderWindow);
			// and set the focus to it
			SetFocus(m_Player->m_RenderWindow);
		} else {
			// we are in windowed mode
			// so we show the main window
			ShowWindow(m_Player->m_MainWindow,1);
			UpdateWindow(m_Player->m_MainWindow);
			// the render window too
			ShowWindow(m_Player->m_RenderWindow,1);
			UpdateWindow(m_Player->m_RenderWindow);
			// and set the focus to it
			SetFocus(m_Player->m_RenderWindow);
		}

	}


	// we reset the player to start it
	m_Player->Reset();

	m_Player->RedirectLog();





    
	return 1;

}