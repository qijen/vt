/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

/*
int le = GetLastError();
XString buf;
buf.Format("le:%d",le);
MessageBox(NULL,buf.Str(),"",1);
*/

#include "CPStdAfx.h"
#include "CustomPlayer.h"
#include "CustomPlayerApp.h"
#include "CustomPlayerDefines.h"
#include "CustomPlayerDialog.h"

#if defined(CUSTOM_PLAYER_STATIC)
// include file used for the static configuration
#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#include "CustomPlayerRegisterDllsExtra.h"
#endif



extern CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;


extern CKERROR LoadCallBack(CKUICallbackStruct& loaddata,void*);
int
CCustomPlayer::InitEngine(HWND iMainWindow)
{

	m_MainWindow = iMainWindow;
	// start the Virtools Engine
	CKStartUp();

	// retrieve the plugin manager ...
	CKPluginManager* pluginManager = CKGetPluginManager();

	//  ... to intialize plugins ...
	if (!_InitPlugins(*pluginManager)) {
		MessageBox(NULL,UNABLE_TO_INIT_PLUGINS,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	// ... and the render engine.
	int renderEngine = _InitRenderEngines(*pluginManager);
	if (renderEngine==-1) {
		MessageBox(NULL,UNABLE_TO_LOAD_RENDERENGINE,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	XString inifile = CKGetStartPath();
	inifile << "CompositionPrefs_R_Hi.ini";
	CKERROR res = CKCreateContext(&m_CKContext,m_MainWindow,inifile.Str());
	if (res!=CK_OK) {
		MessageBox(NULL,UNABLE_TO_INIT_CK,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	// retrieve the main managers which will be used by the player
	m_MessageManager	= m_CKContext->GetMessageManager();
	m_RenderManager		= m_CKContext->GetRenderManager();
	m_TimeManager		= m_CKContext->GetTimeManager();
	m_AttributeManager	= m_CKContext->GetAttributeManager();

	m_InputManager		= (CKInputManager*)m_CKContext->GetManagerByGuid(INPUT_MANAGER_GUID);
	if (!m_MessageManager || !m_RenderManager || !m_TimeManager || !m_AttributeManager || !m_InputManager) {
		MessageBox(NULL,UNABLE_TO_INIT_MANAGERS,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	// initialize the display driver using the player configuration (resolution, rasterizer, ...)
	if (!_InitDriver()) {		
		MessageBox(NULL,UNABLE_TO_INIT_DRIVER,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}



BOOL CCustomPlayer::InitPlayer(HWND iMainWindow, HWND iRenderWindow, int iConfig, const void* iData,int iDataSize)
{

	// keep a reference on the main/render window
	m_MainWindow = iMainWindow;
	m_RenderWindow = iRenderWindow;



	// start the Virtools Engine
	//CKInitCustomPlayer(false);
	CKStartUp();


	




	//////////////////////////////////////////////////////////////////////////
	// we have unmet requirements ? 
	XString errorText;
	int hasError  = DoSystemCheck(errorText);

	//////////////////////////////////////////////////////////////////////////
	//  show splash when we have no error and we are not in the explicit configuration mode ( invoked by : "CustomPlayer.exe -c ") : 
	if( GetPAppStyle()->UseSplash() && !hasError && PGetApplicationMode() != config )
	{
		ShowSplash();
	}


	// retrieve the plugin manager ...
	CKPluginManager* pluginManager = CKGetPluginManager();

	//////////////////////////////////////////////////////////////////////////
	//  ... to intialize plugins ...
	if (!_InitPlugins(*pluginManager)) {
		MessageBox(NULL,UNABLE_TO_INIT_PLUGINS,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ... and the render engine.
	int renderEngine = _InitRenderEngines(*pluginManager);
	if (renderEngine==-1) 
	{
		MessageBox(NULL,UNABLE_TO_LOAD_RENDERENGINE,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// now create the CK context
	XString inifile = CKGetStartPath();
	inifile << "CompositionPrefs_R_Hi.ini";
	CKERROR res = CKCreateContext(&m_CKContext,m_MainWindow,inifile.Str());
	if (res!=CK_OK) {
		MessageBox(NULL,UNABLE_TO_INIT_CK,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	





#if defined(CUSTOM_PLAYER_STATIC)
	// if the player is not static

	char BehaviorPath[_MAX_PATH];
	char szPath[_MAX_PATH];
	VxGetModuleFileName(NULL,szPath,_MAX_PATH);
	CKPathSplitter ps(szPath);
	sprintf(BehaviorPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->BehaviorPath.Str());
	pluginManager->ParsePlugins(BehaviorPath);

#endif
	//////////////////////////////////////////////////////////////////////////
	//  adding load callback
	if(GetPAppStyle()->UseSplash() && !m_LastError && PGetApplicationMode() != config )
	{
		if (GetPAppStyle()->ShowLoadingProcess())
		{
			
			m_CKContext->SetInterfaceMode(FALSE,LoadCallBack,NULL);
		}
	}

	// retrieve the main managers which will be used by the player
	m_MessageManager	= m_CKContext->GetMessageManager();
	m_RenderManager		= m_CKContext->GetRenderManager();
	m_TimeManager		= m_CKContext->GetTimeManager();
	m_AttributeManager	= m_CKContext->GetAttributeManager();
	
	m_EnginePointers.TheMessageManager = m_MessageManager;
	m_EnginePointers.TheRenderManager =  m_RenderManager;
	m_EnginePointers.TheTimeManager = m_TimeManager;
	m_EnginePointers.TheCKContext  = m_CKContext;
	

	//We have an error,  show our dialog :  
	if (hasError)
	{
		AfxInitRichEdit2();
		CustomPlayerDialog k(NULL,CString(errorText.Str()));
		k.DoModal();
		if (CPA_ABORT_ON_ERROR)
		{
			return FALSE;
		}
	}
	



	m_InputManager		= (CKInputManager*)m_CKContext->GetManagerByGuid(INPUT_MANAGER_GUID);
	if (!m_MessageManager || !m_RenderManager || !m_TimeManager || !m_AttributeManager || !m_InputManager) {
		MessageBox(NULL,UNABLE_TO_INIT_MANAGERS,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}


	// initialize the display driver using the player configuration (resolution, rasterizer, ...)
	if (!_InitDriver()) {		
		MessageBox(NULL,UNABLE_TO_INIT_DRIVER,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}


	PLoadResourcePaths(CUSTOM_PLAYER_CONFIG_FILE,"Textures",0);
	PLoadResourcePaths(CUSTOM_PLAYER_CONFIG_FILE,"Data",1);
	PLoadResourcePaths(CUSTOM_PLAYER_CONFIG_FILE,"Music",2);



	// now load the composition
	if (iDataSize) { // if iDataSize is not null it means the composition is already in memory
		if (_Load(iData,iDataSize)!=CK_OK) {
			MessageBox(NULL,"Unable to load composition from memory.","Initialization Error",MB_OK|MB_ICONERROR);
			return FALSE;
		}
	} else if (_Load((const char*)iData)!=CK_OK) { // else we load it from a file (iData contains the filename)
		MessageBox(NULL,"Unable to load composition from file.","Initialization Error",MB_OK|MB_ICONERROR);
		return FALSE;
	}

	//GetPlayer().HideSplash();

	//show our dialog  ? 
	if (GetPAppStyle()->g_ShowDialog)
	{
		AfxInitRichEdit2();
		CustomPlayerDialog k(NULL,CString(errorText.Str()));
		k.DoModal();
	}

	


	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	// update of our member variables
	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	m_WindowedHeight  = ewinfo->g_HeightW;
	m_WindowedWidth = ewinfo->g_WidthW;
	
	m_FullscreenWidth  = ewinfo->g_Width;
	m_FullscreenHeight  = ewinfo->g_Height;

	if(ewinfo->g_GoFullScreen)
		m_Driver  =  ewinfo->g_FullScreenDriver;
	else
		m_Driver  =  ewinfo->g_WindowedDriver;

	m_FullscreenBpp  = ewinfo->g_Bpp;
	
	//////////////////////////////////////////////////////////////////////////

	CKVariableManager *vm = (CKVariableManager *)m_EnginePointers.TheCKContext->GetVariableManager();
	vm->SetValue("CK2_3D/Antialias", GetEWindowInfo()->FSSA * 2 );
    

	if(PGetApplicationMode() != config)
	{
		  
		if (GetPlayer().GetPAppStyle()->IsRenderering())
		{
			
			//////////////////////////////////////////////////////////////////////////
			// create the render context
			if (GetEWindowInfo()->g_GoFullScreen) 
			{
				// in fullscreen we specify the rendering size using a rectangle (CKRECT)
				CKRECT rect;
				rect.left = 0;
				rect.top = 0;
				rect.right = m_FullscreenWidth;
				rect.bottom = m_FullscreenHeight;
				// create the render context
			
				m_RenderContext = m_RenderManager->CreateRenderContext(m_RenderWindow,GetEWindowInfo()->g_FullScreenDriver,&rect,TRUE,m_FullscreenBpp);

				// set the position of the render window

				::SetWindowPos(m_RenderWindow,NULL,0,0,m_FullscreenWidth,m_FullscreenHeight,SWP_NOMOVE|SWP_NOZORDER);

				// resize the render context
				if (m_RenderContext) {
					m_RenderContext->Resize(0,0,m_FullscreenWidth,m_FullscreenHeight);
				}
			} else {
				
				//////////////////////////////////////////////////////////////////////////
				//  [2/18/2008 mc007]
                //  we only repositioning the window when we are not displayed by a hosting application like
				//  a charp panel !
				if (!GetPAppStyle()->g_OwnerDrawed)
				{
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

					// set the position of the render window
					::SetWindowPos(m_RenderWindow,NULL,0,0,m_WindowedWidth,m_WindowedHeight,SWP_NOMOVE|SWP_NOZORDER);
				}
				// create the render context
				m_RenderContext = m_RenderManager->CreateRenderContext(m_RenderWindow,m_Driver,0,FALSE);

				// resize the render context
				if (m_RenderContext) {
					m_RenderContext->Resize(0,0,m_WindowedWidth,m_WindowedHeight);
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// when somebody changed any resolution in the configuration tab , we should update our windows : 
			//store current size
			/*
			GetWindowRect(m_MainWindow,&m_MainWindowRect);

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
			m_RenderContext->Resize(0,0,GetEWindowInfo()->g_GoFullScreen ? m_FullscreenWidth : m_WindowedWidth, GetEWindowInfo()->g_GoFullScreen ? m_FullscreenHeight :  m_WindowedHeight);*/


			if (!m_RenderContext) {
				MessageBox(NULL,UNABLE_TO_CREATE_RENDERCONTEXT,INIT_ERROR,MB_OK|MB_ICONERROR);
				return FALSE;
			}


			
			//////////////////////////////////////////////////////////////////////////
			// clear the render view
			m_RenderContext->Clear();
			m_RenderContext->BackToFront();
			m_RenderContext->Clear();
		}


		// finalize the loading
		if (!_FinishLoad()) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCustomPlayer::_InitDriver()
{
	int count = m_RenderManager->GetRenderDriverCount();
	int i = 0;

	// first, we try to get exactly what is required
	int checkFlags = eFamily | eDirectXVersion | eSoftware;
	for (i=0;i<count;++i) {
		VxDriverDesc* desc = m_RenderManager->GetRenderDriverDescription(i);
		if (!desc) {
			continue;
		}

		if (_CheckDriver(desc,checkFlags)) {
			m_Driver = i;
			_CheckFullscreenDisplayMode(TRUE);
			return TRUE;
		}
	}

	if (m_RasterizerFamily == CKRST_OPENGL) {
		return FALSE;
	}

	// if we did not find a driver,
	// we only check family and software flags
	checkFlags &= ~eDirectXVersion;
	for (i=0;i<count;++i) {
		VxDriverDesc* desc = m_RenderManager->GetRenderDriverDescription(i);
		if (!desc) {
			continue;
		}

		if (_CheckDriver(desc,checkFlags)) {
			m_Driver = i;
			_CheckFullscreenDisplayMode(TRUE);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCustomPlayer::_InitPlugins(CKPluginManager& iPluginManager)
{

	char szPath[_MAX_PATH];
	char BehaviorPath[_MAX_PATH];
	

#if !defined(CUSTOM_PLAYER_STATIC)
	// if the player is not static

	
	char PluginPath[_MAX_PATH];
	char RenderPath[_MAX_PATH];
	char ManagerPath[_MAX_PATH];

	VxGetModuleFileName(NULL,szPath,_MAX_PATH);
	CKPathSplitter ps(szPath);
	sprintf(PluginPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->PluginPath.Str());
	sprintf(RenderPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->RenderPath.Str());
	sprintf(ManagerPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->ManagerPath.Str());
	sprintf(BehaviorPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->BehaviorPath.Str());

	// we initialize plugins by parsing directories
	iPluginManager.ParsePlugins(RenderPath);
	iPluginManager.ParsePlugins(ManagerPath);
	iPluginManager.ParsePlugins(BehaviorPath);
	iPluginManager.ParsePlugins(PluginPath);

#else
	// else if the player is static
	// we initialize plugins by manually register them
	// for an exemple look at CustomPlayerRegisterDlls.h
	CustomPlayerRegisterRenderEngine(iPluginManager);
	CustomPlayerRegisterReaders(iPluginManager);
	CustomPlayerRegisterManagers(iPluginManager);
	CustomPlayerRegisterBehaviors(iPluginManager);
	CustomPlayerRegisterBehaviorsExtra(iPluginManager);


	VxGetModuleFileName(NULL,szPath,_MAX_PATH);
	CKPathSplitter ps(szPath);
	sprintf(BehaviorPath,"%s%s%s",ps.GetDrive(),ps.GetDir(),GetEPathProfile()->BehaviorPath.Str());
	//iPluginManager.ParsePlugins(BehaviorPath);

#endif

	return TRUE;
}

int CCustomPlayer::_InitRenderEngines(CKPluginManager& iPluginManager)
{
	// here we look for the render engine (ck2_3d)
	int count = iPluginManager.GetPluginCount(CKPLUGIN_RENDERENGINE_DLL);
	for (int i=0;i<count;i++) {
		CKPluginEntry* desc = iPluginManager.GetPluginInfo(CKPLUGIN_RENDERENGINE_DLL,i); 
		CKPluginDll*   dll  = iPluginManager.GetPluginDllInfo(desc->m_PluginDllIndex); 
#if !defined(CUSTOM_PLAYER_STATIC)
		XWORD pos = dll->m_DllFileName.RFind(DIRECTORY_SEP_CHAR);
		if (pos==XString::NOTFOUND)
			continue;
		XString str = dll->m_DllFileName.Substring(pos+1);
		if (strnicmp(str.CStr(),"ck2_3d",strlen("ck2_3d"))==0)
			return i;
#else
		if (dll->m_DllFileName.ICompare("ck2_3d")==0)
			return i;
#endif
	}

	return -1;
}


