// vtAgeiaInterfaceToolbarDlg.cpp : implementation file
//

#include "stdafx2.h"
#include "vtAgeiaInterfaceEditor.h"
#include "vtAgeiaInterfaceToolbarDlg.h"

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
// vtAgeiaInterfaceToolbarDlg dialog


vtAgeiaInterfaceToolbarDlg::vtAgeiaInterfaceToolbarDlg(CWnd* pParent /*=NULL*/)
	: DllToolbarDlg(vtAgeiaInterfaceToolbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(vtAgeiaInterfaceToolbarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void vtAgeiaInterfaceToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	DllToolbarDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(vtAgeiaInterfaceToolbarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(vtAgeiaInterfaceToolbarDlg, DllToolbarDlg)
	//{{AFX_MSG_MAP(vtAgeiaInterfaceToolbarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// vtAgeiaInterfaceToolbarDlg message handlers

BOOL vtAgeiaInterfaceToolbarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (
		pMsg->message >= WM_MOUSEFIRST &&
		pMsg->message <= WM_MOUSELAST)
	{
		MSG msg;
		::CopyMemory(&msg, pMsg, sizeof(MSG));
		m_wndToolTip.RelayEvent(pMsg);
	}
	
	return DllToolbarDlg::PreTranslateMessage(pMsg);
}

BOOL vtAgeiaInterfaceToolbarDlg::OnInitDialog() 
{
	DllToolbarDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
//	EnableToolTips(TRUE);
//	CreateTooltip();
	
	ActivateClosebutton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//this is the almost equivalent of OnInitDialog you should use if you want to
//use the PluginInterface with GetInterface or if you want to be sure the editor is present
void vtAgeiaInterfaceToolbarDlg::OnInterfaceInit()
{
}

//called on WM_DESTROY
void vtAgeiaInterfaceToolbarDlg::OnInterfaceEnd() 
{
}

HRESULT vtAgeiaInterfaceToolbarDlg::ReceiveNotification(UINT MsgID,DWORD Param1,DWORD Param2,CKScene* Context)
{
	return 0;
}

void vtAgeiaInterfaceToolbarDlg::CreateTooltip()
{
	//m_wndToolTip.Create(this);
//	m_wndToolTip.Activate(TRUE);

	//delay after which the tooltip apppear, value of 1 is immediate, 0 is default (that is 500 for windows)
	//m_wndToolTip.SetDelayTime(500);

	//change tooltip back color
	//m_wndToolTip.SetTipBkColor(CZC_176);	
	//change tooltip text color
	//m_wndToolTip.SetTipTextColor(CZC_BLACK);

	//for each control you want to add a tooltip on, add the following line
	//m_wndToolTip.AddTool(CWnd* target,const char* tooltip_string);

	//delay after which the tooltip will auto close itself
	/*
	m_wndToolTip.SetDelayTime(TTDT_AUTOPOP,5000);

	m_wndToolTip.SetWindowPos(&(CWnd::wndTopMost),0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	//if you want multiline tooltip, you must add the following line (note that this represent the max tooltip width in pixel)
	m_wndToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 500);
	*/
}
