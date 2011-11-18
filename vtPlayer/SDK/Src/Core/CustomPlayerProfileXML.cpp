/********************************************************************
	created:	2007/11/28
	created:	28:11:2007   15:43
	filename: 	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Misc\CustomPlayerProfileXML.cpp
	file path:	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Misc
	file base:	CustomPlayerProfileXML
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/
#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"

#include "CustomPlayerDefines.h"

xEApplicationMode 
CCustomPlayer::PGetApplicationMode()
{
	return m_AppMode;
}
xSApplicationWindowStyle *
CCustomPlayer::GetPAppStyle()
{
	return m_AWStyle;
}

xSEngineWindowInfo *
CCustomPlayer::GetEWindowInfo()
{
	return m_EngineWindowInfo;
}

/*USHORT 
CCustomPlayer::PLoadEnginePathProfile(const char* configFile)
{
    return m_EPaths;
}*/
xSEnginePaths* 
CCustomPlayer::GetEPathProfile()
{
	return m_EPaths;
}

//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////
extern CCustomPlayer*	thePlayer;


USHORT
CCustomPlayer::PLoadResourcePaths(const char* configFile,const char*section,int resourceType)
{

	if (!this->m_CKContext)return -1;
	
	CKPathManager *pm = (CKPathManager*)this->m_CKContext->GetPathManager();
	if (!pm)return -1;


	USHORT error = CPE_OK;

	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);


	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *tsection = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}

	if ((tsection = config.GetSubSection((char*)section, FALSE)) != NULL)
	{
        
		ConstEntryIt it  = tsection->BeginChildEntry();
		VxConfigurationEntry *sEntry  = NULL;

	
		char newPath[MAX_PATH];
		while (sEntry=tsection->GetNextChildEntry(it))
		{
			if (sEntry!=NULL)
			{
				const char * value = sEntry->GetValue();
				sprintf(newPath,"%s%s%s",drive,dir,value);
				pm->AddPath(resourceType,XString(newPath));
			}
		}

	}



}

//************************************
// Method:    PLoadAppStyleProfile
// FullName:  CCustomPlayerApp::PLoadAppStyleProfile
// Access:    protected 
// Returns:   USHORT
// Qualifier:
// Parameter: const char* configFile
// Parameter: vtPlayer::Structs::xEApplicationWindowStyle& result
//************************************

USHORT
CCustomPlayer::PLoadAppStyleProfile(const char* configFile)
{
	USHORT error = CPE_OK;

	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);
	

	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *section = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}

	if ((section = config.GetSubSection("ApplicationConfiguration", FALSE)) != NULL)
	{

		
		//////////////////////////////////////////////////////////////////////////
		// MouseDragsWindow
		entry = section->GetEntry("MouseDragsWindow");
		if (entry != NULL)
		{


			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_MouseDrag = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		// MouseDragsWindow
		entry = section->GetEntry("OwnerDrawed");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_OwnerDrawed = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		// HasRenderWindow
		entry = section->GetEntry("Render");
		if (entry != NULL)
		{


			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_Render = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

			printf(value);

		}
		//////////////////////////////////////////////////////////////////////////
		entry = section->GetEntry("AlwayOnTop");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_AOT = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		// CaptionBar
		entry = section->GetEntry("Title");
		if (entry != NULL)
		{
			m_AWStyle->g_AppTitle = entry->GetValue();
		}
		//////////////////////////////////////////////////////////////////////////
		// Show Loading Progress : 
		entry = section->GetEntry("ShowLoadingProcess");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_ShowLoadingProcess= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//splash
		entry = section->GetEntry("UseSplash");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_UseSplash = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//////////////////////////////////////////////////////////////////////////
		//ShowDialog
		entry = section->GetEntry("ShowDialog");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_ShowDialog = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//////////////////////////////////////////////////////////////////////////
		//ShowAboutTab
		entry = section->GetEntry("ShowAboutTab");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_ShowAboutTab = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		//ShowConfigTab
		entry = section->GetEntry("ShowConfigTab");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_ShowConfigTab= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		//MinDirectXVersion
		entry = section->GetEntry("MinDirectXVersion");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_MinimumDirectXVersion= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//////////////////////////////////////////////////////////////////////////
		//MinRAM 
		entry = section->GetEntry("MinRAM");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_AWStyle->g_MiniumumRAM= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}


	}

	return error;
}

//************************************
// Method:    PLoadEngineWindowProperties
// FullName:  CCustomPlayerApp::PLoadEngineWindowProperties
// Access:    protected 
// Returns:   USHORT
// Qualifier:
// Parameter: const char *configFile
// Parameter: vtPlayer::Structs::xEEngineWindowInfo& result
//************************************

USHORT
CCustomPlayer::PLoadEngineWindowProperties(const char *configFile)
{



	USHORT error = CPE_OK;

	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);

	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *section = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}

	if ((section = config.GetSubSection("VideoSettings", FALSE)) != NULL)
	{

		// WindowWidth (optional)
		entry = section->GetEntry("WindowWidth");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_WidthW = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		// WindowHeight (optional)
		entry = section->GetEntry("WindowHeight");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_HeightW = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		// FullscreenWidth (optional)
		entry = section->GetEntry("FullscreenWidth");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_Width = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}


		// FullscreenHeight
		entry = section->GetEntry("FullscreenHeight");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_Height= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		// FullscreenBpp : 
		entry = section->GetEntry("Bpp");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_Bpp= tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		// FullscreenDriver : 
		entry = section->GetEntry("FullScreenDriver");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_FullScreenDriver  = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}


		// FullscreenDriver : 
		entry = section->GetEntry("WindowedDriver");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_WindowedDriver = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		// RefreshRate : 
		entry = section->GetEntry("RefreshRate");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_RefreshRate = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//ScreenMode : 
		entry = section->GetEntry("Mode");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_Mode = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//AntiAlias : 
		entry = section->GetEntry("AntiAlias");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->FSSA = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//Fullscreen : 
		entry = section->GetEntry("FullScreen");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_GoFullScreen = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//HasResolutionMask : 
		entry = section->GetEntry("HasResolutionMask");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			int tmp = 0;
			if(sscanf(value,"%d",&tmp)==1) 
			{
				m_EngineWindowInfo->g_HasResMask = tmp;
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//HasResolutionMaskX : 
		entry = section->GetEntry("XResolutions");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();
			
			if(value) 
			{
				XStringTokenizer	tokizer(value, ",");
				const char*tok = NULL;
				while (tokizer.NextToken(tok))
				{
					tok = tokizer.NextToken(tok);
					XString xtok(tok);
					m_EngineWindowInfo->g_ResMaskArrayX.PushBack(xtok.ToInt());
				}
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
		//HasResolutionMaskY : 
		entry = section->GetEntry("YResolutions");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();

			if(value) 
			{
				XStringTokenizer	tokizer(value, ",");
				const char*tok = NULL;
				while (tokizer.NextToken(tok))
				{
					tok = tokizer.NextToken(tok);
					XString xtok(tok);
					m_EngineWindowInfo->g_ResMaskArrayY.PushBack(xtok.ToInt());
				}
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}

		//Allowed OpenGL versions ; 
		entry = section->GetEntry("OpenGLVersions");
		if (entry != NULL)
		{
			const char * value = entry->GetValue();

			if(value) 
			{
				XStringTokenizer	tokizer(value, ",");
				const char*tok = NULL;
				while (tokizer.NextToken(tok))
				{
					tok = tokizer.NextToken(tok);
					XString xtok(tok);
					m_EngineWindowInfo->g_OpenGLMask.PushBack(xtok);
				}
			}
			else
				error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//todo : 
	m_FullscreenBpp = GetEngineWindowInfo()->g_Bpp;
	m_FullscreenHeight= GetEngineWindowInfo()->g_Height;
	m_FullscreenWidth = GetEngineWindowInfo()->g_Width;
	m_FullscreenRefreshRate = GetEngineWindowInfo()->g_RefreshRate;

	m_WindowedWidth = GetEngineWindowInfo()->g_WidthW;
	m_WindowedHeight = GetEngineWindowInfo()->g_HeightW;

	return error;
}

//************************************
// Method:    PLoadEnginePathProfile
// FullName:  vtBaseWindow::PLoadEnginePathProfile
// Access:    public 
// Returns:   USHORT
// Qualifier:
// Parameter: const char* configFile
//************************************


USHORT
CCustomPlayer::PLoadEnginePathProfile(const char* configFile)
{

	USHORT error = CPE_OK;

	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);

	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *section = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}

	if ((section = config.GetSubSection("Paths", FALSE)) != NULL)
	{

		// Disable hotkeys (optional)
		entry = section->GetEntry("RenderEngines");
		if (entry != NULL)
		{
			GetEPathProfile()->RenderPath = entry->GetValue();
		}

		entry = section->GetEntry("ManagerPath");
		if (entry != NULL)
		{
			GetEPathProfile()->ManagerPath = entry->GetValue() ;
		}

		entry = section->GetEntry("BehaviorPath");
		if (entry != NULL)
		{
			GetEPathProfile()->BehaviorPath = entry->GetValue();
		}

		entry = section->GetEntry("PluginPath");
		if (entry != NULL)
		{
			GetEPathProfile()->PluginPath = entry->GetValue();
		}

		entry = section->GetEntry("LoadFile");
		if (entry != NULL)
		{
			GetEPathProfile()->CompositionFile = entry->GetValue();
		}

		entry = section->GetEntry("DecompressFile");
		if (entry != NULL)
		{
			GetEPathProfile()->DecompressFile = entry->GetValue();
		}
		entry = section->GetEntry("Installdirectory");
		if (entry != NULL)
		{
			GetEPathProfile()->InstallDirectory = entry->GetValue();
		}

	}

	section->Clear();
	config.Clear();
	return error;
}

USHORT
CCustomPlayer::PSaveEngineWindowProperties(const char *configFile,const vtPlayer::Structs::xSEngineWindowInfo& input)
{

	USHORT error = CPE_OK;


	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,configFile);


	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *section = NULL;
	VxConfigurationEntry *entry = NULL;

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}

	if ((section = config.GetSubSection("VideoSettings", FALSE)) != NULL)
	{
		// WindowWidth (optional)
		entry = section->GetEntry("WindowWidth");
		if (entry != NULL)
		{
			entry->SetValue(input.g_WidthW);			
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;



		// WindowHeight (optional)
		entry = section->GetEntry("WindowHeight");
		if (entry != NULL)
		{
			entry->SetValue(input.g_HeightW);			
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;



		// FullscreenHeight
		entry = section->GetEntry("FullscreenHeight");
		if (entry != NULL)
		{
			entry->SetValue(input.g_Height);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// FullscreenWidth
		entry = section->GetEntry("FullscreenWidth");
		if (entry != NULL)
		{
			entry->SetValue(input.g_Width);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// FSSA
		entry = section->GetEntry("AntiAlias");
		if (entry != NULL)
		{
			entry->SetValue(input.FSSA);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// Bpp
		entry = section->GetEntry("Bpp");
		if (entry != NULL)
		{
			entry->SetValue(input.g_Bpp);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// Driver
		entry = section->GetEntry("FullScreenDriver");
		if (entry != NULL)
		{
			entry->SetValue(input.g_FullScreenDriver);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// Driver
		entry = section->GetEntry("WindowedDriver");
		if (entry != NULL)
		{
			entry->SetValue(input.g_WindowedDriver);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;


		// RefreshRate
		entry = section->GetEntry("RefreshRate");	if (entry != NULL)
		{
			entry->SetValue(input.g_RefreshRate);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// Mode
		entry = section->GetEntry("Mode");	if (entry != NULL)
		{
			entry->SetValue(input.g_Mode);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;

		// FullScreen
		entry = section->GetEntry("FullScreen");	if (entry != NULL)
		{
			entry->SetValue(input.g_GoFullScreen);
		}else
			error = CPE_PROFILE_ERROR_ENTRY_INCORRECT;
	}

	config.SaveToFile(Ini);

	return error;
}