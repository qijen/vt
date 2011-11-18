// DistributedNetworkClassDialogToolbarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DistributedNetworkClassDialogEditor.h"
#include "DistributedNetworkClassDialogToolbarDlg.h"

#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

DllToolbarDlg*	fCreateToolbarDlg(HWND parent)
{
	HRESULT r = CreateDllDialog(parent,IDD_TOOLBAR,&g_Toolbar);
	return (DllToolbarDlg*)g_Toolbar;
}

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogToolbarDlg dialog


DistributedNetworkClassDialogToolbarDlg::DistributedNetworkClassDialogToolbarDlg(CWnd* pParent /*=NULL*/)
	: DllToolbarDlg(DistributedNetworkClassDialogToolbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DistributedNetworkClassDialogToolbarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DistributedNetworkClassDialogToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	DllToolbarDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DistributedNetworkClassDialogToolbarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DistributedNetworkClassDialogToolbarDlg, DllToolbarDlg)
	//{{AFX_MSG_MAP(DistributedNetworkClassDialogToolbarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogToolbarDlg message handlers

BOOL DistributedNetworkClassDialogToolbarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	
	return DllToolbarDlg::PreTranslateMessage(pMsg);
}

BOOL DistributedNetworkClassDialogToolbarDlg::OnInitDialog() 
{
	DllToolbarDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	
	ActivateClosebutton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//this is the almost equivalent of OnInitDialog you should use if you want to
//use the PluginInterface with GetInterface or if you want to be sure the editor is present
void DistributedNetworkClassDialogToolbarDlg::OnInterfaceInit()
{
}

//called on WM_DESTROY
void DistributedNetworkClassDialogToolbarDlg::OnInterfaceEnd() 
{
}

HRESULT DistributedNetworkClassDialogToolbarDlg::ReceiveNotification(UINT MsgID,DWORD Param1,DWORD Param2,CKScene* Context)
{
	return 0;
}

