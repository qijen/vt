/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

#if defined(CUSTOM_PLAYER_STATIC)
// include file used for the static configuration
#include "CustomPlayerStaticLibs.h"
#include "CustomPlayerRegisterDlls.h"
#endif

extern CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;

BOOL CCustomPlayer::_FinishLoad()
{

	// retrieve the level
	m_Level = m_CKContext->GetCurrentLevel();
	if (!m_Level) {
		MessageBox(NULL,CANNOT_FIND_LEVEL,INIT_ERROR,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	if(m_RenderContext)
	{
		// Add the render context to the level
		m_Level->AddRenderContext(m_RenderContext,TRUE);

		// Take the first camera we found and attach the viewpoint to it.
		// (in case it is not set by the composition later)
		const XObjectPointerArray cams = m_CKContext->GetObjectListByType(CKCID_CAMERA,TRUE);
		if(cams.Size()) {
			m_RenderContext->AttachViewpointToCamera((CKCamera*)cams[0]);
		}

		// Hide curves ?
		int curveCount = m_CKContext->GetObjectsCountByClassID(CKCID_CURVE);
		CK_ID* curve_ids = m_CKContext->GetObjectsListByClassID(CKCID_CURVE);
		for (int i=0;i<curveCount;++i) {
			CKMesh* mesh = ((CKCurve*)m_CKContext->GetObject(curve_ids[i]))->GetCurrentMesh();
			if(mesh)
				mesh->Show(CKHIDE);
		}

		// retrieve custom player attributes
		// from an exemple about using this attributes see sample.cmo which is delivered with this player sample
		// simply set the "Quit" attribute to quit the application
		m_QuitAttType = m_AttributeManager->GetAttributeTypeByName("Quit");
		// simply set the "Switch Fullscreen" attribute to make the player switch between fullscreen and windowed mode
		m_SwitchFullscreenAttType = m_AttributeManager->GetAttributeTypeByName("Switch Fullscreen");
		// simply set the "Switch Resolution" attribute to make the player change its resolution according
		// to the "Windowed Resolution" or "Fullscreen Resolution" (depending on the current mode)
		m_SwitchResolutionAttType = m_AttributeManager->GetAttributeTypeByName("Switch Resolution");
		// simply set the "Switch Mouse Clipping" attribute to make the player clip/unclip the mouse to the render window
		m_SwitchMouseClippingAttType = m_AttributeManager->GetAttributeTypeByName("Switch Mouse Clipping");
		// the windowed resolution
		m_WindowedResolutionAttType = m_AttributeManager->GetAttributeTypeByName("Windowed Resolution");
		// the fullscreen resolution
		m_FullscreenResolutionAttType = m_AttributeManager->GetAttributeTypeByName("Fullscreen Resolution");
		// the fullscreen bpp
		m_FullscreenBppAttType = m_AttributeManager->GetAttributeTypeByName("Fullscreen Bpp");

		// remove attributes (quit,switch fullscreen and switch resolution) if present
		if (m_Level->HasAttribute(m_QuitAttType)) {
			m_Level->RemoveAttribute(m_QuitAttType);
		}
		if (m_Level->HasAttribute(m_SwitchFullscreenAttType)) {
			m_Level->RemoveAttribute(m_SwitchFullscreenAttType);
		}
		if (m_Level->HasAttribute(m_SwitchResolutionAttType)) {
			m_Level->RemoveAttribute(m_SwitchResolutionAttType);
		}
		if (m_Level->HasAttribute(m_SwitchMouseClippingAttType)) {
			m_Level->RemoveAttribute(m_SwitchMouseClippingAttType);
		}

		// set the attributes so it match the current player configuration
		_SetResolutions();

		// set a fake last cmo loaded
		// we build a filename using the exe full filename
		// remplacing exe by vmo.
		{
			char path[MAX_PATH];
			if (::GetModuleFileName(0,path,MAX_PATH)) {
				char drive[MAX_PATH];
				char dir[MAX_PATH];
				char filename[MAX_PATH];
				char ext[MAX_PATH];
				_splitpath(path,drive,dir,filename,ext);
				_makepath(path,drive,dir,filename,"vmo");
				m_CKContext->SetLastCmoLoaded(path);

			}
		}
	}


	// we launch the default scene
	m_Level->LaunchScene(NULL);

	// ReRegister OnClick Message in case it changed
	m_MsgClick = m_MessageManager->AddMessageType("OnClick");
	m_MsgDoubleClick = m_MessageManager->AddMessageType("OnDblClick");

	// render the first frame
	if (m_RenderContext)
		m_RenderContext->Render();

	return TRUE;
}

CKERROR CCustomPlayer::_Load(const char* str) 
{
	// the composition from a file

	// validate the filename
	if(!str || !(*str) || strlen(str)<=0) {
		return FALSE;
	}

	// here we decompose the loading from a file to manage the missing guids case

	// create a ckfile
	CKFile *f = m_CKContext->CreateCKFile();	
	XString resolvedfile = str;	
	// resolve the filename using the pathmanager
	{
		CKPathManager *pm = m_CKContext->GetPathManager();	
		if(pm) {
			pm->ResolveFileName(resolvedfile,0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// dbo - modification - displaying a bitmap during the load process
	// init - BEGIN
	//////////////////////////////////////////////////////////////////////////
	CKSprite* loadingScreen = NULL;


	//////////////////////////////////////////////////////////////////////////
	// dbo mod - init - END
	//////////////////////////////////////////////////////////////////////////

	// open the file
	DWORD res = CKERR_INVALIDFILE;
	res = f->OpenFile(resolvedfile.Str(),(CK_LOAD_FLAGS) (CK_LOAD_DEFAULT | CK_LOAD_CHECKDEPENDENCIES));
	if (res!=CK_OK) {
		// something failed
		if (res==CKERR_PLUGINSMISSING) {
			// log the missing guids
			_MissingGuids(f,resolvedfile.CStr());
		}
		//m_CKContext->DeleteCKFile(f);
		//return res;
	}

	CKPathSplitter ps(resolvedfile.Str());
	CKPathMaker mp(ps.GetDrive(),ps.GetDir(),NULL,NULL);
	if(strcmp(mp.GetFileName(),CKGetStartPath())){
		CKPathManager *pm = m_CKContext->GetPathManager();
		XString XStr = XString(mp.GetFileName());
		pm->AddPath(BITMAP_PATH_IDX,XStr);
		XStr = XString(mp.GetFileName());
		pm->AddPath(DATA_PATH_IDX,XStr);
		XStr = XString(mp.GetFileName());
		pm->AddPath(SOUND_PATH_IDX,XStr);			
	}

	CKObjectArray *array = CreateCKObjectArray();	
	res = f->LoadFileData(array);	

	//////////////////////////////////////////////////////////////////////////
	// loading is done either as failed, or ok. Let's deleted the loading sprite
	//////////////////////////////////////////////////////////////////////////
	/*if (loadingScreen)
	{
		m_CKContext->DestroyObject(loadingScreen->GetID());
		m_Level->RemoveRenderContext(m_RenderContext);
		m_CKContext->DestroyObject(m_Level->GetID());

		// in case we changed it, it might have side effects, so we restore to black
		m_RenderContext->GetBackgroundMaterial()->SetDiffuse(VxColor(0,0,0).GetRGB());
	}*/
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////


	if(res != CK_OK) {
		m_CKContext->DeleteCKFile(f);
		DeleteCKObjectArray(array);
		return res;
	}

	m_CKContext->DeleteCKFile(f);

	DeleteCKObjectArray(array);	

	return CK_OK;
}

CKERROR CCustomPlayer::_Load(const void* iMemoryBuffer,int iBufferSize)
{
	CKFile *f = m_CKContext->CreateCKFile();	

	DWORD res = CKERR_INVALIDFILE;
	res = f->OpenMemory((void*)iMemoryBuffer,iBufferSize,(CK_LOAD_FLAGS) (CK_LOAD_DEFAULT | CK_LOAD_CHECKDEPENDENCIES));
	if (res!=CK_OK) {
		if (res==CKERR_PLUGINSMISSING) {
			_MissingGuids(f,0);
		}
		//m_CKContext->DeleteCKFile(f);
		//return res;
	}

	CKObjectArray *array = CreateCKObjectArray();	
	res = f->LoadFileData(array);	
	if(res != CK_OK) {
		m_CKContext->DeleteCKFile(f);
		return res;
	}

	m_CKContext->DeleteCKFile(f);

	DeleteCKObjectArray(array);	
	return CK_OK;
}
