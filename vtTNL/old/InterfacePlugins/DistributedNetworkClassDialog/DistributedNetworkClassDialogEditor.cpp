// Editor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DistributedNetworkClassDialogEditor.h"
#include "DistributedNetworkClassDialogEditorDlg.h"
#include "DistributedNetworkClassDialogToolbarDlg.h"
#include "DistributedNetworkClassDialogCallback.h"

#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

DistributedNetworkClassDialogEditorDlg*		g_Editor			= NULL;
DistributedNetworkClassDialogToolbarDlg*		g_Toolbar			= NULL;

//----------------------------------------
//The Plugin Info structure that must be filled for Virtools Dev to load effectively the plugin
PluginInfo g_PluginInfo0;

//Returns the number of plugin contained in this dll
//this function must be exported (have a .def file with its name or use __declspec( dllexport )
int GetVirtoolsPluginInfoCount()
{
	return 1;
}

//returns the ptr of the (index)th plugininfo structure of this dll
//this function must be exported (have a .def file with its name or use __declspec( dllexport )
PluginInfo* GetVirtoolsPluginInfo(int index)
{
	switch(index)
	{
	case 0:
		return &g_PluginInfo0;
	}
	return NULL;
}

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogEditorApp

BEGIN_MESSAGE_MAP(DistributedNetworkClassDialogEditorApp, CWinApp)
	//{{AFX_MSG_MAP(DistributedNetworkClassDialogEditorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogEditorApp construction

DistributedNetworkClassDialogEditorApp::DistributedNetworkClassDialogEditorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only DistributedNetworkClassDialogEditorApp object

DistributedNetworkClassDialogEditorApp theApp;

BOOL DistributedNetworkClassDialogEditorApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	strcpy(g_PluginInfo0.m_Name,"DistributedObjects");
	g_PluginInfo0.m_PluginType		= PluginInfo::PT_EDITOR;
	g_PluginInfo0.m_PluginType		= (PluginInfo::PLUGIN_TYPE)(g_PluginInfo0.m_PluginType | PluginInfo::PTF_RECEIVENOTIFICATION);
	g_PluginInfo0.m_PluginCallback	= PluginCallback;
	//EDITOR
	{
		g_PluginInfo0.m_EditorInfo.m_Guid[1]						= 0x5f116b1f;
		g_PluginInfo0.m_EditorInfo.m_Guid[2]						= 0x7771a950;
		strcpy(g_PluginInfo0.m_EditorInfo.m_EditorName,"DistributedNetworkClassDialog");

		g_PluginInfo0.m_EditorInfo.m_CreateEditorDlgFunc			= fCreateEditorDlg;
		g_PluginInfo0.m_EditorInfo.m_CreateToolbarDlgFunc			= fCreateToolbarDlg;

		g_PluginInfo0.m_EditorInfo.m_bUnique						= 1;
		g_PluginInfo0.m_EditorInfo.m_bIndestructible				= false;
		g_PluginInfo0.m_EditorInfo.m_bManageScrollbar				= 0;

		g_PluginInfo0.m_EditorInfo.m_Width							= 800;
		g_PluginInfo0.m_EditorInfo.m_Height							= 800;

		g_PluginInfo0.m_EditorInfo.m_ToolbarHeight					= 21;
		InitImageList();
		g_PluginInfo0.m_EditorInfo.m_IconList						= &m_ImageList;
		g_PluginInfo0.m_EditorInfo.m_IconIndex						= 0;
	}

	return CWinApp::InitInstance();
}

int DistributedNetworkClassDialogEditorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	DeleteImageList();

	return CWinApp::ExitInstance();
}

void DistributedNetworkClassDialogEditorApp::InitImageList()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ImageList.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 2);

	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_EDITORICON));
}

void DistributedNetworkClassDialogEditorApp::DeleteImageList()
{
	m_ImageList.DeleteImageList();
}
