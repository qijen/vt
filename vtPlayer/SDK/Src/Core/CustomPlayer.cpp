/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayer.h"
#include "CustomPlayerApp.h"
#include "CustomPlayerDefines.h"

#if defined(CUSTOM_PLAYER_STATIC)
	// include file used for the static configuration
	#include "CustomPlayerStaticLibs.h"
	#include "CustomPlayerRegisterDlls.h"
#endif

extern CCustomPlayerApp theApp;
CCustomPlayer*	thePlayer=0;

int& CCustomPlayer::RasterizerFamily()
{
	return m_RasterizerFamily;
}

int& CCustomPlayer::RasterizerFlags()
{
	return m_RasterizerFlags;
}

int& CCustomPlayer::WindowedWidth()
{
	return m_WindowedWidth;
}

int& CCustomPlayer::WindowedHeight()
{
	return m_WindowedHeight;
}

int& CCustomPlayer::MininumWindowedWidth()
{
	return m_MinWindowedWidth;
}

int& CCustomPlayer::MininumWindowedHeight()
{
	return m_MinWindowedHeight;
}

int& CCustomPlayer::FullscreenWidth()
{
	return m_FullscreenWidth;
}

int& CCustomPlayer::FullscreenHeight()
{
	return m_FullscreenHeight;
}

int CCustomPlayer::Driver()
{
	return m_Driver;
}

int& CCustomPlayer::FullscreenBpp()
{
	return m_FullscreenBpp;
}

CKRenderContext* CCustomPlayer::GetRenderContext()
{
	return m_RenderContext;
}

extern CKERROR LoadCallBack(CKUICallbackStruct& loaddata,void*);

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
	return m_EngineWindowInfo;
}

////////////////////////////////////////////////////////////////////////////////
//
// CCustomPlayer: PUBLIC STATIC METHODS
//
////////////////////////////////////////////////////////////////////////////////

CCustomPlayer&
CCustomPlayer::Instance()
{
	if (thePlayer==0) {
		thePlayer = new CCustomPlayer();
	}
	return *thePlayer;
}

////////////////////////////////////////////////////////////////////////////////
//
// CCustomPlayer: PUBLIC METHODS
//
////////////////////////////////////////////////////////////////////////////////

CCustomPlayer::~CCustomPlayer()
{
	// here we stop/release/clear the Virtools Engine

	try {
		if (!m_CKContext) {
			return;
		}

		// clear the CK context
		m_CKContext->Reset();
		m_CKContext->ClearAll();

		// destroy the render engine
		if (m_RenderManager && m_RenderContext) {
			m_RenderManager->DestroyRenderContext(m_RenderContext);
		}
		m_RenderContext = NULL;

		// close the ck context
		CKCloseContext(m_CKContext);

		m_CKContext = NULL;

		// shutdown all
		CKShutdown();

		m_CKContext = NULL;
		
	

		if (m_AWStyle)
		{
			delete m_AWStyle;
			m_AWStyle =NULL;
		}
				

	} catch(...) 
	{
	}
	
}



CCustomPlayer::CCustomPlayer()
	: m_State(eInitial), m_MainWindow(0),m_RenderWindow(0),
	m_CKContext(0),m_RenderContext(0),
	m_MessageManager(0),m_RenderManager(0),m_TimeManager(0),
	m_AttributeManager(0),m_InputManager(0),
	m_Level(0),m_QuitAttType(-1),m_SwitchResolutionAttType(-1),m_SwitchMouseClippingAttType(-1),
	m_WindowedResolutionAttType(-1),m_FullscreenResolutionAttType(-1),m_FullscreenBppAttType(-1),
	m_MsgClick(0),m_MsgDoubleClick(0),
	m_RasterizerFamily(CKRST_DIRECTX),m_RasterizerFlags(CKRST_SPECIFICCAPS_HARDWARE|CKRST_SPECIFICCAPS_DX9),
	m_WindowedWidth(640),m_WindowedHeight(480),
	m_MinWindowedWidth(400),m_MinWindowedHeight(300),
	m_FullscreenWidth(640),m_FullscreenHeight(480),m_FullscreenBpp(32),
	m_Driver(-1),m_FullscreenEnabled(FALSE),
	m_EatDisplayChange(FALSE),m_MouseClipped(FALSE),m_LastError(0),
	m_PlayerClass(MAINWINDOW_CLASSNAME),m_RenderClass(RENDERWINDOW_CLASSNAME)
{
	m_EngineWindowInfo = new xSEngineWindowInfo();
	m_EPaths	 = new xSEnginePaths();
//	m_AWStyle  = NULL;
	m_AWStyle  = new xSApplicationWindowStyle();
	m_AppMode  = normal;
	m_hThread = NULL;

}

BOOL CCustomPlayer::_CheckDriver(VxDriverDesc* iDesc, int iFlags)
{
	// check the rasterizer family
	if (iFlags & eFamily) { 
		if (iDesc->Caps2D.Family!=m_RasterizerFamily) {
			return FALSE;
		}
	}

	// test directx version
	if ( (iDesc->Caps2D.Family==CKRST_DIRECTX) && (iFlags & eDirectXVersion) ) {
		if ((iDesc->Caps3D.CKRasterizerSpecificCaps&0x00000f00UL)!=(m_RasterizerFlags&0x00000f00UL)) {
			return FALSE;
		}
	}

	// test hardware/software
	if (iFlags & eSoftware) {
		if (((int)iDesc->Caps3D.CKRasterizerSpecificCaps&CKRST_SPECIFICCAPS_SOFTWARE)!=(m_RasterizerFlags&CKRST_SPECIFICCAPS_SOFTWARE)) {
			return FALSE;
		}	
	}

	return TRUE;
}

void CCustomPlayer::_MissingGuids(CKFile* iFile, const char* iResolvedFile)
{
	// here we manage the error CKERR_PLUGINSMISSING when loading a composition failed

	// create missing guids log filename
	char fp[_MAX_PATH];
	{
		GetTempPath(_MAX_PATH,fp);
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
				
		_splitpath(fp,drive,dir,fname,ext);
		_makepath(fp,drive,dir,MISSINGUIDS_LOG,NULL);
	}

	// retrieve the list of missing plugins/guids
	XClassArray<CKFilePluginDependencies> *p = iFile->GetMissingPlugins();
	CKFilePluginDependencies*it = p->Begin();	

	FILE *logf = NULL;	
	char str[64];
	for(CKFilePluginDependencies* it=p->Begin();it!=p->End();it++) {
		int count = (*it).m_Guids.Size();
		for(int i=0;i<count;i++) {
			if (!((*it).ValidGuids[i])) {
				if(!logf) {
					logf = fopen(fp,"wt");
					if (!logf) {
						return;
					}
					if (iResolvedFile) {
						fprintf(logf,"File Name : %s\nMissing GUIDS:\n",iResolvedFile);
					}
				}

				sprintf(str,"%x,%x\n",(*it).m_Guids[i].d1,(*it).m_Guids[i].d2);
				fprintf(logf,"%s",str);
			}
		}
	}
			
	fclose(logf);
}

