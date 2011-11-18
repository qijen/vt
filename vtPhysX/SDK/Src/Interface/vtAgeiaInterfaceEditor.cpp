// Editor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx2.h"
#include "vtAgeiaInterfaceEditor.h"
#include "vtAgeiaInterfaceEditorDlg.h"
#include "vtAgeiaInterfaceToolbarDlg.h"
#include "vtAgeiaInterfaceCallback.h"

#include "resource.h"
#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

vtAgeiaInterfaceEditorDlg*		g_Editor			= NULL;
vtAgeiaInterfaceToolbarDlg*		g_Toolbar			= NULL;

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
// vtAgeiaInterfaceEditorApp

BEGIN_MESSAGE_MAP(vtAgeiaInterfaceEditorApp, CWinApp)
	//{{AFX_MSG_MAP(vtAgeiaInterfaceEditorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// vtAgeiaInterfaceEditorApp construction

vtAgeiaInterfaceEditorApp::vtAgeiaInterfaceEditorApp()
{
	mContext = NULL;
	pMan =NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only vtAgeiaInterfaceEditorApp object

vtAgeiaInterfaceEditorApp theApp;






BOOL vtAgeiaInterfaceEditorApp::InitInstance() 
{
	AfxOleInit();
	// TODO: Add your specialized code here and/or call the base class
	if (GetCKContext(0))
	{
		/*theApp.mContext = GetCKContext(0);
		PhysicManager *im = (PhysicManager*)getContext()->GetManagerByGuid(PhysicManager_GUID);
		if (im)
		{
			pMan = im;
		}*/
	}
	
	strcpy(g_PluginInfo0.m_Name,"vtAgeiaInterface");
	g_PluginInfo0.m_PluginType		= PluginInfo::PT_EDITOR;
	//g_PluginInfo0.m_PluginType		= (PluginInfo::PLUGIN_TYPE)(g_PluginInfo0.m_PluginType | PluginInfo::PTF_RECEIVENOTIFICATION);
	
	
	//EDITOR
	{
		g_PluginInfo0.m_EditorInfo.m_Guid[1]						= 0xd07cc06a;
		g_PluginInfo0.m_EditorInfo.m_Guid[2]						= 0x7ca4fc86;
		strcpy(g_PluginInfo0.m_EditorInfo.m_EditorName,"vtAgeiaEditor");

		g_PluginInfo0.m_EditorInfo.m_CreateEditorDlgFunc			= fCreateEditorDlg;
		g_PluginInfo0.m_EditorInfo.m_CreateToolbarDlgFunc			= fCreateToolbarDlg;

		g_PluginInfo0.m_EditorInfo.m_bUnique						= 1;
		g_PluginInfo0.m_EditorInfo.m_bIndestructible				= 0;
		g_PluginInfo0.m_EditorInfo.m_bManageScrollbar				= 0;

		g_PluginInfo0.m_EditorInfo.m_Width							= 400;
		g_PluginInfo0.m_EditorInfo.m_Height							= 200;

		g_PluginInfo0.m_EditorInfo.m_ToolbarHeight					= 21;
		//InitImageList();
		g_PluginInfo0.m_EditorInfo.m_IconList						= &m_ImageList;
		g_PluginInfo0.m_EditorInfo.m_IconIndex						= 0;
		
		g_PluginInfo0.m_PluginCallback	= PluginCallback;
	}
	

	return CWinApp::InitInstance();
}

int vtAgeiaInterfaceEditorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	DeleteImageList();

	return CWinApp::ExitInstance();
}

void vtAgeiaInterfaceEditorApp::InitImageList()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ImageList.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 2);

//	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_EDITORICON));
}

void vtAgeiaInterfaceEditorApp::DeleteImageList()
{
	m_ImageList.DeleteImageList();
}


/*

CWnd* thisSubParentWindow = CWnd::FromHandle((HWND)ParentWindow);
MultiParamEditDlg	pedit(GetCKContext(0), thisSubParentWindow);
int s = pedit.GetMode();
pedit.SetMode(MultiParamEditDlg::AUTOCLOSE_WHEN_MODELESS|MultiParamEditDlg::MODELESSEDIT);

CKParameterOut *commonParameter= GetParameterFromStruct(param,PS_COMMON_SETTINGS);
pedit.AddParameter(commonParameter);
//for (int i = 0; i < mfx->m_Params.Size(); ++i)
//	pedit.AddParameter(mfx->m_Params[i]);
pedit.SetTitle("Material Shader Fx Params");
pedit.DoModal();

return pedit.GetSafeHwnd();

char name[256];
if (oriUIFunc && param)
{

	//CWnd* thisSubParentWindow = CWnd::FromHandle((HWND)ParentWindow);
	CWnd* thisSubParentWindow = CWnd::FromHandle((HWND)ParentWindow);
	CString strMain;
	thisSubParentWindow->GetWindowText(strMain);



	if (thisSubParentWindow !=NULL )
	{
		CWnd* bigParentWindow = thisSubParentWindow;
		for( CWnd* subParentWindow = bigParentWindow->GetWindow(GW_CHILD); subParentWindow ; subParentWindow = subParentWindow->GetWindow(GW_HWNDNEXT) )
		{


			CString str;
			subParentWindow->GetWindowText(str);

			GetClassName( subParentWindow->m_hWnd, name, sizeof(name) );

			XString _name(name);
			//if( strcmp(name,MFC_NAME_OF_DIALOG) ) continue;
			for( CWnd* child = subParentWindow->GetWindow(GW_CHILD) ; child ; child = child->GetWindow(GW_HWNDNEXT) )
			{


				CString str2;
				child->GetWindowText(str);


				//GetClassName( child->m_hWnd, name, sizeof(name) );
				//XString _name2(name);

				//if( strcmp(name,MFC_NAME_OF_DIALOG) ) continue;

				//CParameterDialog* otherParamDialog = (CParameterDialog*)(child->SendMessage( CKWM_GETPARAMDIALOG, 0, 0 ));
				//if( !otherParamDialog ) continue;

				CKParameterOut *commonParameter= GetParameterFromStruct(param,PS_COMMON_SETTINGS);
				if (commonParameter)
				{
					int p2=2;
					p2++;
					//SetParameterStructureValue<int>(commonParameter,PS_HULL_TYPE,HT_Mesh,false);
				}


				//--- From this point it should be a CParameterDialog...
				//CParameterDialog* otherParamDialog = (CParameterDialog*)(child->SendMessage( CKWM_GETPARAMDIALOG, 0, 0 ));
				//if( !otherParamDialog ) continue;

			}





		}
	}


	//MultiParamEditDlg test(param->GetCKContext(),ParentWindow);


	CKParameterOut *commonParameter= GetParameterFromStruct(param,PS_COMMON_SETTINGS);
	if (commonParameter)
	{
	//SetParameterStructureValue<int>(commonParameter,PS_HULL_TYPE,HT_Mesh,false);

	}
	//return (*oriUIFunc)(param,ParentWindow,rect);
	return ParentWindow;
	double d = 0.0;
}

*/