
#include "CPStdAfx.h"
#include <stdio.h>


#include "profile/GProfile.h"
	
//used from LoadProfile




/************************************************************************/
/* Enigne Parameter Initiating 												                                                                        */
/************************************************************************/

engine_profile
vtBaseWindow::LoadProfile(const char* _iniFile){

	
	engine_profile res;

	
	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,_iniFile);

	GProfile PProfile(Ini);
	PProfile.Load();

	//////////////////////////////////////////////////////////////////////////
	//Path stuff :
	
	GProfile::Section *pSection = (GProfile::Section*)PProfile.FindSection("PATHS");
	
		strcpy(res.RenderPath,PProfile.FindKey("RenderPath",pSection)->m_strValue);
		strcpy(res.ManagerPath,PProfile.FindKey("ManagerPath",pSection)->m_strValue);
		strcpy(res.BehaviorPath,PProfile.FindKey("BehaviorPath",pSection)->m_strValue);
		strcpy(res.PluginPath,PProfile.FindKey("PluginPath",pSection)->m_strValue);
		strcpy(res.LoadFile,PProfile.FindKey("LoadFile",pSection)->m_strValue);

	//////////////////////////////////////////////////////////////////////////
	//Engine Stuff

	pSection = (GProfile::Section*)PProfile.FindSection("Engine");

		res.g_DisableSwitch = PProfile.GetInt(pSection->m_strName,"DisableSwitch",false);
		res.g_Width = PProfile.GetInt(pSection->m_strName,"Width",false);
		res.g_Height= PProfile.GetInt(pSection->m_strName,"Height",false);
		res.g_Bpp    = PProfile.GetInt(pSection->m_strName,"Bpp",false);
		res.g_FullScreenDriver = PProfile.GetInt(pSection->m_strName,"Driver",false);
		res.g_GoFullScreen = PProfile.GetInt(pSection->m_strName,"FullScreen",false);
		res.g_RefreshRate = PProfile.GetInt(pSection->m_strName,"RefreshRate",false);
		res.g_Mode = PProfile.GetInt(pSection->m_strName,"Mode",false);
		
	PProfile.Destroy();
	

	return res;
	
}