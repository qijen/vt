/******************************************************************************
File : CustomPlayer.h

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#if !defined(CUSTOMPLAYER_H)
#define CUSTOMPLAYER_H

#include "CustomPlayerApp.h"

/*************************************************************************
Summary: This class defines the implementation of the Virtools Runtime/Player.

Description: This class provides member functions for initializing the
virtools runtime and for running it.

Remarks: This class is a singleton. It means there is only one instance of it
and you cannot instanciate it. To get an instance of the class use
CCustomPlayer::Instance.

See also: CCustomPlayerApp, Instance.
*************************************************************************/
class CCustomPlayer
{
public :

	/*************************************************************************
	Summary: Retrieve the unique instance of the player.
	*************************************************************************/
	static CCustomPlayer& Instance();

	/*************************************************************************
	Summary: Release all data which has been created during the initializating
	and the execution of the player.
	*************************************************************************/
	~CCustomPlayer();

	/*************************************************************************
	Summary: Initialize the player.

	Description: This function intialize the virtools engine (ck, render engine, ...) and load
	the composition

	Parameters:
		iMainWindow: the main window of the application.
		iRenderWindow: the render window.
		iConfig: the configuration of the player (see EConfigPlayer).
		iData: pointer to a string (if iDataSize==0) containing the name of the filename
		to load or pointer to the memory where the application is located (if iDataSize!=0).
		iDataSize: Size of the memory where the application is located if it is alredy in memory (can be null).
	*************************************************************************/
	BOOL InitPlayer(HWND iMainWindow, HWND iRenderWindow, int iConfig, const void* iData, int iDataSize);

	
	/*************************************************************************
	Summary: Initialize the engine.

	Description: This function intialize the virtools engine (ck, render engine, ...) 

	*************************************************************************/
	int InitEngine(HWND iMainWindow);

	/*************************************************************************
	Summary: Play the composition.
	*************************************************************************/
	void Play();

	/*************************************************************************
	Summary: Pause the composition.
	*************************************************************************/
	void Pause();

	/*************************************************************************
	Summary: Reset the composition and play it.
	*************************************************************************/
	void Reset();

	/*************************************************************************
	Summary: Process one frame of the compisition
	*************************************************************************/
	BOOL Process(int iConfig);

	/*************************************************************************
	Summary: Switch from fullscreen/windowed to windowed/fullscreen.

	Remarks: The player try to retrieve the resolution (fullscreen or windowed)
	from level attributes before switching.
	*************************************************************************/
	BOOL SwitchFullscreen();

	/*************************************************************************
	Summary: Change the current resolution (fullscreen or windowed

	Remarks: The player try to retrieve the resolution (fullscreen or windowed)
	from level attributes. If nothing has changed nothing is done.
	*************************************************************************/
	BOOL ChangeResolution();
	
	/*************************************************************************
	Summary: Manage the mouse clipping

	Parameters: TRUE to enable the mouse clipping.
	*************************************************************************/
	BOOL ClipMouse(BOOL iEnable);

	/*************************************************************************
	Summary: Manage the WM_PAINT windows event.

	Description: In windowed mode we use the render context to render
	the scene.
	*************************************************************************/
	void OnPaint();

	/*************************************************************************
	Summary: Manage the mouse left button click.

	Description: Send a message (click or double click) to the object "picked"
	by the mouse, if any.
	*************************************************************************/
	void OnMouseClick(int iMessage);

	/*************************************************************************
	Summary: Manage the focus changement.

	Description:
		- if the application is minimized (no focus) we paused it.
		- if the application is no more minimized (but was minimized) we restart it.
		- if the application focus is changin depending of the "Focus Lost Behavior"
		  (see CKContext::GetFocusLostBehavior or the "Focus Lost Behavior" in the
		  variable manager).
	*************************************************************************/
	void OnFocusChange(BOOL iFocus);

	/*************************************************************************
	Summary: Manage the activty of the application.

	Description: If the application is deactivated while it is in fullscreen
	mode, we must switch to windowed mode.
	*************************************************************************/
	void OnActivateApp(BOOL iActivate);

	/*************************************************************************
	Summary: Manage system keys (ALT + KEY)

	Description: If system keys are not diabled (see eDisableKeys)
		- ALT + ENTER -> SwitchFullscreen
		- ALT + F4    -> Quit the application
	*************************************************************************/
	LRESULT	OnSysKeyDownMainWindow(int iConfig, int iKey);
	/*************************************************************************
	Summary: Manage the activty of the application.

	Description: If the application is deactivated while it is in fullscreen
	mode, we must switch to windowed mode.
	*************************************************************************/
	int DoSystemCheck();

	void _InitContextAnd();
	void _InitManagers();


	void SetMainWindow(HWND _main){ m_MainWindow = _main;}

	vtPlayer::Structs::xEApplicationMode  m_AppMode;


	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////

	//  [11/28/2007 mc007]
	
	xSEngineWindowInfo m_EngineWindowInfo;
	USHORT PLoadEngineWindowProperties(const char *configFile);
	xSEngineWindowInfo& GetEWindowInfo(){return m_EngineWindowInfo;}

	xSEnginePaths m_EPaths;
	USHORT PLoadEnginePathProfile(const char* configFile);
	xSEnginePaths& GetEPathProfile(){return m_EPaths;}

	xSApplicationWindowStyle m_AWStyle;

	USHORT PLoadAppStyleProfile(const char* configFile);	
	xSApplicationWindowStyle&GetPAppStyle(){ return m_AWStyle;}

	xEApplicationMode PGetApplicationMode(const char* pstrCommandLine);
	xEApplicationMode PGetApplicationMode(){return m_AppMode;}
	 
	API_INLINE vtPlayer::Structs::xSEnginePointers *GetEnginePointers();
	API_INLINE vtPlayer::Structs::xSEngineWindowInfo *GetEngineWindowInfo();
	
	void ShowSplash();
	void SetSplashText(const char* text);
	void HideSplash();



	////////////////////////////////////////
	// accessors

	int& RasterizerFamily();
	int& RasterizerFlags();
	int& WindowedWidth();
	int& WindowedHeight();
	int& MininumWindowedWidth();
	int& MininumWindowedHeight();
	int& FullscreenWidth();
	int& FullscreenHeight();
	int Driver();
	int& FullscreenBpp();
	CKRenderContext* GetRenderContext();

protected:

	enum DriverFlags 
	{
		eFamily			= 1,
		eDirectXVersion	= 2,
		eSoftware		= 4
	};
	

	enum PlayerState
	{
		eInitial	= 0,
		ePlaying	= 1,
		ePlaused	= 2,
		eFocusLost	= 3,
		eMinimized	= 4
	};


	BOOL	_FinishLoad();
	BOOL	_InitPlugins(CKPluginManager& iPluginManager);
	int		_InitRenderEngines(CKPluginManager& iPluginManager);
	CKERROR	_Load(const char* str);
	CKERROR	_Load(const void* iMemoryBuffer, int iBufferSize);
	void	_MissingGuids(CKFile* iFile, const char* iResolvedFile);
	BOOL	_GetWindowedResolution();
	BOOL	_GetFullScreenResolution();
	void	_SetResolutions();

	BOOL	_InitDriver();
	BOOL	_CheckDriver(VxDriverDesc* iDesc, int iFlags);
	BOOL	_CheckFullscreenDisplayMode(BOOL iDoBest);

	int					m_State;
	public:
	HWND				m_MainWindow;
	HWND				m_RenderWindow;
	// ck objects (context, managers, ...)
	
	CKContext*			m_CKContext;
	CKRenderContext*	m_RenderContext;
	CKMessageManager*	m_MessageManager;
	CKRenderManager*	m_RenderManager;
	CKTimeManager*		m_TimeManager;
	CKAttributeManager*	m_AttributeManager;
	CKInputManager*		m_InputManager;

	vtPlayer::Structs::xSEnginePointers m_EnginePointers;


	CKLevel*			m_Level;
	// attributes
	// from an exemple about using this attributes see sample.cmo which is delivered with this player sample
	int					m_QuitAttType;					// attribut without type, name is "Quit"
	int					m_SwitchFullscreenAttType;		// attribut without type, name is "Switch Fullscreen"
	int					m_SwitchResolutionAttType;		// attribut without type, name is "Switch Resolution"
	int					m_SwitchMouseClippingAttType;	// attribut without type, name is "Switch Mouse Clipping"
	int					m_WindowedResolutionAttType;	// attribut which type is Vector 2D, name is "Windowed Resolution"
	int					m_FullscreenResolutionAttType;	// attribut which type is Vector 2D, name is "Fullscreen Resolution"
	int					m_FullscreenBppAttType;			// attribut which type is Integer, name is "Fullscreen Bpp"
	// messages
	int					m_MsgClick;
	int					m_MsgDoubleClick;
	// display configuration
	int					m_RasterizerFamily;		// see CKRST_RSTFAMILY
	int					m_RasterizerFlags;		// see CKRST_SPECIFICCAPS
	int					m_WindowedWidth;		// windowed mode width
	int					m_WindowedHeight;		// windowed mode height
	int					m_MinWindowedWidth;		// windowed mode minimum width
	int					m_MinWindowedHeight;	// windowed mode minumum height
	int					m_FullscreenWidth;		// fullscreen mode width
	int					m_FullscreenHeight;		// fullscreen mode height
	int					m_FullscreenBpp;		// fullscreen mode bpp
	int					m_Driver;				// index of the current driver
	BOOL				m_FullscreenEnabled;	// is fullscreen enable
	BOOL				m_EatDisplayChange;		// used to ensure we are not switching mode will we are already switching
	BOOL				m_MouseClipped;			// used to know if the mouse is acutally cliped
	RECT				m_MainWindowRect;

	int					m_FullscreenRefreshRate;		// fullscreen mode bpp
	int m_LastError;
	XString m_LastErrorText;

private:

	// {secret}
	CCustomPlayer();
	// {secret}
	CCustomPlayer(const CCustomPlayer&);
	// {secret}
	CCustomPlayer& operator=(const CCustomPlayer&);

};











//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
//  [11/28/2007 mc007]

#define  GetPlayer() CCustomPlayer::Instance()


//////////////////////////////////////////////////////////////////////////


//************************************
// Method:    GetEnginePointers
// FullName:  CCustomPlayer::GetEnginePointers
// Access:    public 
// Returns:   vtPlayer::Structs::xSEnginePointers*
// Qualifier:
//************************************
vtPlayer::Structs::xSEnginePointers*
CCustomPlayer::GetEnginePointers()
{
	return &m_EnginePointers;
}

//************************************
// Method:    GetEngineWindowInfo
// FullName:  CCustomPlayer::GetEngineWindowInfo
// Access:    public 
// Returns:   vtPlayer::Structs::xSEngineWindowInfo*
// Qualifier:
//************************************
vtPlayer::Structs::xSEngineWindowInfo*
CCustomPlayer::GetEngineWindowInfo()
{
	return &m_EngineWindowInfo;
}
//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////











//////////////////////////////////////////////////////////////////////////
inline int& CCustomPlayer::RasterizerFamily()
{
	return m_RasterizerFamily;
}

inline int& CCustomPlayer::RasterizerFlags()
{
	return m_RasterizerFlags;
}

inline int& CCustomPlayer::WindowedWidth()
{
	return m_WindowedWidth;
}

inline int& CCustomPlayer::WindowedHeight()
{
	return m_WindowedHeight;
}

inline int& CCustomPlayer::MininumWindowedWidth()
{
	return m_MinWindowedWidth;
}

inline int& CCustomPlayer::MininumWindowedHeight()
{
	return m_MinWindowedHeight;
}

inline int& CCustomPlayer::FullscreenWidth()
{
	return m_FullscreenWidth;
}

inline int& CCustomPlayer::FullscreenHeight()
{
	return m_FullscreenHeight;
}

inline int CCustomPlayer::Driver()
{
	return m_Driver;
}

inline int& CCustomPlayer::FullscreenBpp()
{
	return m_FullscreenBpp;
}

inline CKRenderContext* CCustomPlayer::GetRenderContext()
{
	return m_RenderContext;
}


#endif // CUSTOMPLAYER_H