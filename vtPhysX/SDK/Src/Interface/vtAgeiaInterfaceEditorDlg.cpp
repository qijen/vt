// vtAgeiaInterfaceEditorDlg.cpp : implementation file
//

#include "stdafx2.h"
#include "vtAgeiaInterfaceEditor.h"
#include "vtAgeiaInterfaceEditorDlg.h"
#include "vtAgeiaInterfaceKeyboardShortcuts.h"
#include "vtAgeiaInterfaceCallback.h"



#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

extern vtAgeiaInterfaceEditorApp theApp;


DllEditorDlg* fCreateEditorDlg(HWND parent)
{
	HRESULT r = CreateDllDialog(parent,IDD_EDITOR,&g_Editor);
	return (DllEditorDlg*)g_Editor;
}

/////////////////////////////////////////////////////////////////////////////
// vtAgeiaInterfaceEditorDlg dialog


vtAgeiaInterfaceEditorDlg::vtAgeiaInterfaceEditorDlg(CWnd* pParent /*=NULL*/)
	: DllEditorDlg(vtAgeiaInterfaceEditorDlg::IDD, pParent)
{
	mContext = NULL;
	pMan = NULL;

	//{{AFX_DATA_INIT(vtAgeiaInterfaceEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void vtAgeiaInterfaceEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	DllEditorDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(vtAgeiaInterfaceEditorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(vtAgeiaInterfaceEditorDlg, DllEditorDlg)
	//{{AFX_MSG_MAP(vtAgeiaInterfaceEditorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// vtAgeiaInterfaceEditorDlg message handlers

LRESULT vtAgeiaInterfaceEditorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	switch (message)
	{
	case WM_KEYDOWN:
		{
			KeyboardShortcutManager* ksm = GetInterface()->GetKeyboardShortcutManager();
			/*int commandID	= ksm->TestKS(STR_CATEGORY,wParam);
			if (commandID)
				OnGlobalKeyboardShortcut(commandID);
			*/
		}break;
	}
	
	return DllEditorDlg::WindowProc(message, wParam, lParam);
}

BOOL vtAgeiaInterfaceEditorDlg::SaveData(CKInterfaceObjectManager* iom)
{

	if( !iom ) return FALSE;

	const int chunkCount = iom->GetChunkCount();

	CKStateChunk* chunk = iom->GetChunk( 0 );
	if( !chunk ) return FALSE;


	int a  = 0 ;
	return TRUE;
}

BOOL vtAgeiaInterfaceEditorDlg::LoadData(CKInterfaceObjectManager* iom)
{


	if( !iom ) return FALSE;

	const int chunkCount = iom->GetChunkCount();

	CKStateChunk* chunk = iom->GetChunk( 0 );
	if( !chunk ) return FALSE;

	chunk->StartRead();

	chunk->CloseChunk();
	return TRUE;


}
BOOL vtAgeiaInterfaceEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	/*if (
		pMsg->message >= WM_MOUSEFIRST &&
		pMsg->message <= WM_MOUSELAST)
	{
		MSG msg;
		::CopyMemory(&msg, pMsg, sizeof(MSG));
		m_wndToolTip.RelayEvent(pMsg);
	}
	*/
	/*switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			break;
		}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			
		}break;
	}*/

	return DllEditorDlg::PreTranslateMessage(pMsg);
}

BOOL vtAgeiaInterfaceEditorDlg::OnInitDialog() 
{
	DllEditorDlg::OnInitDialog();
	
	// TODO: Add extra initialization here

	//EnableToolTips(TRUE);
	//CreateTooltip();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//this is the almost equivalent of OnInitDialog you should use if you want to
//use the PluginInterface with GetInterface or if you want to be sure the toolbar is present
void vtAgeiaInterfaceEditorDlg::OnInterfaceInit()
{
	if (theApp.mContext && theApp.pMan)
	{
		init(theApp.mContext,theApp.pMan);
	}
	//sample code : here we ask to listen to the _CKFILELOADED notification
	//which is send when a file is loaded from Virtools Dev's user interface
	ObserveNotification(CUIK_NOTIFICATION_CKFILELOADED);
	
	ObserveNotification(CUIK_NOTIFICATION_LEVEL_LOADED);
	ObserveNotification(CUIK_NOTIFICATION_CKFILELOADED);
	ObserveNotification(CUIK_NOTIFICATION_PRECKFILELOADED);
	
	ObserveNotification(CUIK_NOTIFICATION_OBJECTROTCHANGED);
	ObserveNotification(CUIK_NOTIFICATION_OBJECTPOSCHANGED);
	ObserveNotification(CUIK_NOTIFICATION_SELECTIONCHANGED);
	ObserveNotification(CUIK_NOTIFICATION_UPDATESETUP);
	ObserveNotification(CUIK_NOTIFICATION_OBJECTPARAMSCHANGED);




}

void vtAgeiaInterfaceEditorDlg::objectSelChanged(DWORD par1,DWORD par2)
{

	/*
	if (getContext())
	{

		getContext()->OutputToConsoleEx("sel changed :");
		return ;
		CKBeObject *object  = (CKBeObject*)mContext->GetObject(par1);
		if (object)
		{
			CKSTRING name = object->GetName();
			getContext()->OutputToConsoleEx("sel changed : %s",name);

		}
	}*/
}
void vtAgeiaInterfaceEditorDlg::objectPosChanged(CK_ID objID)
{
	int id = objID;
	///MessageBox("asdasd","asdasd",1);
/*

	if (mContext)
	{
		CKBeObject *object  = (CKBeObject*)mContext->GetObject(objID);
		if (objID)
		{
			int a = 0 ; 
			CKSTRING name = object->GetName();
			CKSTRING name2 = object->GetName();


		}
	}

	*/
}
void vtAgeiaInterfaceEditorDlg::init(CKContext *ctx,PhysicManager *pm)
{
	pMan = pm;
	mContext = ctx;

}

//called on WM_DESTROY
void vtAgeiaInterfaceEditorDlg::OnInterfaceEnd() 
{
}

HRESULT vtAgeiaInterfaceEditorDlg::ReceiveNotification(UINT MsgID,DWORD Param1,DWORD Param2,CKScene* Context)
{
	switch(MsgID)
	{
	//sample code : 
	case CUIK_NOTIFICATION_CKFILELOADED:
		{
		}break;
	case CUIK_NOTIFICATION_OBJECTPOSCHANGED:
	{
		//objectPosChanged(Param1);
	}
	case CUIK_NOTIFICATION_SELECTIONCHANGED:
		{

			//objectSelChanged(Param1,Param2);
			break;
		}
	case CUIK_NOTIFICATION_UPDATESETUP:
	{
			//objectSelChanged(Param1,Param2);
			break;
	}
	case CUIK_NOTIFICATION_OBJECTPARAMSCHANGED:
		{
			//objectSelChanged(Param1,Param2);
			break;
		}
	break;
	}
	return 0;
}

void vtAgeiaInterfaceEditorDlg::CreateTooltip()
{
	m_wndToolTip.Create(this);
	m_wndToolTip.Activate(TRUE);

	//delay after which the tooltip apppear, value of 1 is immediate, 0 is default (that is 500 for windows)
	m_wndToolTip.SetDelayTime(500);

	//change tooltip back color
	//m_wndToolTip.SetTipBkColor(CZC_176);	
	//change tooltip text color
	//m_wndToolTip.SetTipTextColor(CZC_BLACK);

	//for each control you want to add a tooltip on, add the following line
	//m_wndToolTip.AddTool(CWnd* target,const char* tooltip_string);

	//delay after which the tooltip will auto close itself
	m_wndToolTip.SetDelayTime(TTDT_AUTOPOP,5000);

	m_wndToolTip.SetWindowPos(&(CWnd::wndTopMost),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	//if you want multiline tooltip, you must add the following line (note that this represent the max tooltip width in pixel)
	m_wndToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 500);
}
int vtAgeiaInterfaceEditorDlg::OnGlobalKeyboardShortcut(int commandID)
{
	return 0;	//return 1 if successfull/keyboard shortcut has been processed
}

int vtAgeiaInterfaceEditorDlg::OnLocalKeyboardShortcut(int commandID)
{
	return 0;	//return 1 if successfull/keyboard shortcut has been processed
}
void vtAgeiaInterfaceEditorDlg::OnCustomMenu(CMenu* menu,int startingCommandID,int endingCommandID)
{
	menu->AppendMenu(0,0 /*base ID*/ + startingCommandID,"Sample Command 1");
	menu->AppendMenu(0,1 /*base ID*/ + startingCommandID,"Sample Command 2");
}

void vtAgeiaInterfaceEditorDlg::OnCustomMenu(int commandID)
{
	switch(commandID) {
	case 0:
		AfxMessageBox("sample command 1 called");
		break;
	case 1:
		AfxMessageBox("sample command 2 called");
		break;
	}
}
