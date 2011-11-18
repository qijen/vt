#include "stdafx2.h"
#include "vtAgeiaInterfaceCallback.h"


#include "vtAgeiaInterfaceKeyboardShortcuts.h"
#include "vtAgeiaInterfaceMenu.h"

#include "PCommonDialog.h"
#include "vtBodyStructs.h"
#include "PBodySetup.h"




//static plugin interface that allow direct communication with Virtools Dev
PluginInterface*		s_Plugininterface	= NULL;

WIN_HANDLE CKActorUIFunc(CKParameter* param,WIN_HANDLE parent,CKRECT *rect);
WIN_HANDLE CKDoubleUIFunc (CKParameter *param,WIN_HANDLE ParentWindow,CKRECT *rect);

/*
AFX_STATIC UINT _afxMsgMouseWheel = 
(((::GetVersion() & 0x80000000) && LOBYTE(LOWORD(::GetVersion()) == 4)) ||
 (!(::GetVersion() & 0x80000000) && LOBYTE(LOWORD(::GetVersion()) == 3)))
 ? ::RegisterWindowMessage(MSH_MOUSEWHEEL) : 0;
*/

//ON_REGISTERED_MESSAGE(_afxMsgMouseWheel, OnRegisteredMouseWheel)*/
//ON_BN_CLICKED(IDC_BFLAGS_DEFORMABLE, OnBnClickedBflagsDeformable)



WIN_HANDLE CKActorUIFunc (CKParameter *param,WIN_HANDLE ParentWindow,CKRECT *rect) 
{


	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*	
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
*/
//	CMultiDocTemplate* pDocTemplate;

/*	pDocTemplate = new CMultiDocTemplate(IDR_DRAWCLTYPE, RUNTIME_CLASS(CDrawDoc), RUNTIME_CLASS(CSplitFrame), RUNTIME_CLASS(CDrawView));
	pDocTemplate->SetContainerInfo(IDR_DRAWCLTYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);
*/
	
	
	CPBCommonDialog *Dlg = new CPBCommonDialog(param);
	Dlg->m_Context = param->GetCKContext();
	HWND win = (HWND)ParentWindow;
	//Dlg->HType.SetCurSel(0);
	CKRECT &rect2= *rect;
	HWND result = NULL;

	//CPBParentDialog *Dlg = new CPBParentDialog(param,CWnd::FromHandle(win));
	//CPBodyCfg *Dlg = new CPBodyCfg(param);

	if (param && win && Dlg )
	{
		Dlg->Create( IDD_PBCOMMON, CWnd::FromHandle(win));
		result = CreateParameterDialog( win, Dlg, 0, PARAMETER_PICKALL, 0, PARAMETER_NORESIZE);
	}

	if (result)
	{
		return result;
	}
	
	return NULL;

}

//main plugin callback for Virtools Dev
void PluginCallback(PluginInfo::CALLBACK_REASON reason,PluginInterface* plugininterface)
{
	CKContext* ctx = GetCKContext(0);

	CKParameterManager *pm = ctx->GetParameterManager();


	{	//--- Set UI Function of Shader Param
		CKParameterTypeDesc *param_desc = pm->GetParameterTypeDescription(VTF_PHYSIC_BODY_COMMON_SETTINGS);
		if( !param_desc ) return;

		param_desc->UICreatorFunction = CKActorUIFunc;
		//param_desc->UICreatorFunction = CKDoubleUIFunc;
	}

	switch(reason)
	{
	case PluginInfo::CR_LOAD:
		{
			s_Plugininterface	= plugininterface;
			//InitParameters(reason,plugininterface);
			/*InitMenu();
			UpdateMenu();
			RegisterKeyboardShortcutCategory();
			RegisterKeyboardShortcuts();*/

		}break;
	case PluginInfo::CR_UNLOAD:
		{
			//RemoveMenu();
			//UnregisterKeyboardShortcutCategory();
			s_Plugininterface	= NULL;
		}break;
	case PluginInfo::CR_NEWCOMPOSITIONNAME:
		{
			//			InitMenu();

		}break;
	case PluginInfo::CR_NOTIFICATION:
		{
		}break;
	}
}
