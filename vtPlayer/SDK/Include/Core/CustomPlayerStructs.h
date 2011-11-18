/********************************************************************
	created:	2007/12/12
	created:	12:12:2007   11:55
	filename: 	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude\CustomPlayerStructs.h
	file path:	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude
	file base:	CustomPlayerStructs
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#ifndef __CUSTOM_PLAYER_STRUCTS_H_
#define __CUSTOM_PLAYER_STRUCTS_H_

namespace vtPlayer
{

	namespace Structs
	{
		//////////////////////////////////////////////////////////////////////////
		//managers,... : 
		struct xSEnginePointers
		{

			CKContext*		 TheCKContext;
			CKTimeManager*	 TheTimeManager;
			CKMessageManager* TheMessageManager;
			CKRenderManager* TheRenderManager;
			CKRenderContext* TheRenderContext;
			CKPluginManager* ThePluginManager;
			
			xSEnginePointers() : TheCKContext(0),TheTimeManager(0),TheMessageManager(0),
				TheRenderManager(0),TheRenderContext(0),ThePluginManager(0)
			{
			
			}
		};
		//////////////////////////////////////////////////////////////////////////
		//player modies : 
		enum xEApplicationMode
		{
			config,         // Config dialog box
			preview,        // Mini preview window in Display Properties dialog
			normal,           // Full-on screensaver mode
			passwordchange,
			java,
			decompress,
			static_res,
			popup
		};
		//////////////////////////////////////////////////////////////////////////
		//Paths : 
		class xSEnginePaths
		{
		public:
			XString RenderPath;
			XString ManagerPath;
			XString BehaviorPath;
			XString PluginPath;
			XString CompositionFile;
			XString DecompressFile;
			XString ConfigFile;
			XString ApplicationProfileFile;
			XString InstallDirectory;

			virtual ~xSEnginePaths(){}


		};
		//////////////////////////////////////////////////////////////////////////
		//Engine Parameters:
		struct xSEngineWindowInfo{

			int				 g_WindowedDriver;
			int				 g_FullScreenDriver;
			int				 g_Mode;
			BOOL	g_DisableSwitch;
			BOOL	g_NoContextMenu;
			BOOL	g_GoFullScreen;
			int	g_RefreshRate;
			int	g_WidthW;
			int	g_HeightW;
			int	g_Width;
			int	g_Height;
			int	g_Bpp;
			int FSSA;
			BOOL g_HasResMask;

			typedef XArray<int> xSResMaskArray;
			xSResMaskArray g_ResMaskArrayX;
			xSResMaskArray g_ResMaskArrayY;

			typedef XArray<XString> xSAllowedOpenGLVersions;
			xSAllowedOpenGLVersions g_OpenGLMask;

			xSEngineWindowInfo() : 
				g_FullScreenDriver(0),
				g_WindowedDriver(0),
				g_Mode(0),
				g_DisableSwitch(false),
				g_NoContextMenu(false),
				g_GoFullScreen(false),
				g_RefreshRate(g_Width),
				g_Width(1024),
				g_Height(768),
				g_Bpp(32),
				FSSA(0){}

		};
		//////////////////////////////////////////////////////////////////////////
		//Application Style
		class xSApplicationWindowStyle
		{

		public:
			int g_MouseDrag;
			int g_OwnerDrawed;
			int g_Render;
			int IsRenderering() const { return g_Render; }
			int g_Sound;
			int g_CaptionBar;
			int g_IsResizeable;
			int g_SizeBox;
			int g_AOT;
			XString g_AppTitle;
			XString AppTitle() const { return g_AppTitle; }
			int g_SSMode;
			int g_MMT;
			BOOL g_UseSplash;
			BOOL g_ShowLoadingProcess;
			BOOL g_AllowEscExit;
			int g_ShowDialog;
			int g_ShowAboutTab;
			int g_ShowConfigTab;
			int g_MinimumDirectXVersion;
			int g_MiniumumRAM;
			BOOL ShowLoadingProcess() const { return g_ShowLoadingProcess; }
			BOOL UseSplash() const { return g_UseSplash; }

			xSApplicationWindowStyle() : 
			g_Render(0),
			g_CaptionBar(0),
			g_IsResizeable(0),
			g_SizeBox(0),
            g_AOT(0),
			g_SSMode(0),
			g_MMT(0),
			g_ShowDialog(0),
			g_ShowAboutTab(0),
			g_ShowConfigTab(0),
			g_MinimumDirectXVersion(9),
			g_MiniumumRAM(256),
			g_UseSplash(false),
			g_AllowEscExit(false)
			{

			}

			virtual ~xSApplicationWindowStyle()
			{

			}
		};
	}	//end of namespace structs
}	//end of namespace structs

using namespace vtPlayer::Structs;

#endif