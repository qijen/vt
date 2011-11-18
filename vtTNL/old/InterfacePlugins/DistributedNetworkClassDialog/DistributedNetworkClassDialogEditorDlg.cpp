// DistributedNetworkClassDialogEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DistributedNetworkClassDialogEditor.h"
#include "DistributedNetworkClassDialogEditorDlg.h"
#include ".\distributednetworkclassdialogeditordlg.h"

#ifdef _MFCDEBUGNEW
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

DllEditorDlg* fCreateEditorDlg(HWND parent)
{
	HRESULT r = CreateDllDialog(parent,IDD_EDITOR,&g_Editor);
	return (DllEditorDlg*)g_Editor;
}

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogEditorDlg dialog


DistributedNetworkClassDialogEditorDlg::DistributedNetworkClassDialogEditorDlg(CWnd* pParent /*=NULL*/)
	: DllEditorDlg(DistributedNetworkClassDialogEditorDlg::IDD, pParent),m_SplitMain(3,1)
{
	//{{AFX_DATA_INIT(DistributedNetworkClassDialogEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DistributedNetworkClassDialogEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	DllEditorDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DistributedNetworkClassDialogEditorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DistributedNetworkClassDialogEditorDlg, DllEditorDlg)
	//{{AFX_MSG_MAP(DistributedNetworkClassDialogEditorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DistributedNetworkClassDialogEditorDlg message handlers

LRESULT DistributedNetworkClassDialogEditorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	
	return DllEditorDlg::WindowProc(message, wParam, lParam);
}

BOOL DistributedNetworkClassDialogEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	

	return DllEditorDlg::PreTranslateMessage(pMsg);
}

BOOL DistributedNetworkClassDialogEditorDlg::OnInitDialog() 
{
	DllEditorDlg::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//this is the almost equivalent of OnInitDialog you should use if you want to
//use the PluginInterface with GetInterface or if you want to be sure the toolbar is present
void DistributedNetworkClassDialogEditorDlg::OnInterfaceInit()
{
	//sample code : here we ask to listen to the _CKFILELOADED notification
	//which is send when a file is loaded from Virtools Dev's user interface
	ObserveNotification(CUIK_NOTIFICATION_CKFILELOADED);
	

	_SetupSplitters();

}


BOOL DistributedNetworkClassDialogEditorDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	MSGFILTER* filter = (MSGFILTER*)lParam;

	
	return DllEditorDlg::OnNotify(wParam, lParam, pResult);
}

//-----------------------------------------------------------------------------
void DistributedNetworkClassDialogEditorDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (m_SplitMain.m_hWnd)
	{
		RECT rect;
		GetClientRect(&rect);
		m_SplitMain.MoveWindow(&rect);
		//m_SplitEditorTitle.SetHorizontalSplitbarPosition(0, LAYOUT_EDITORTITLE-5);
	}

	DllEditorDlg::OnSize(nType, cx, cy);
}
void
DistributedNetworkClassDialogEditorDlg::_SetupSplitters()
{


	RECT rect;
	GetClientRect(&rect);
	int	width = 1140;
	int height = 381;

	//		int								: column count
	//		int								: row count
	//		int								: width of window RECT at saving time
	//		int								: height of window RECT at saving time
	//		array of int (column count-1)	: SplitBars X positions (in pixel or percentage if percent==TRUE)
	//		array of int (row count-1)		: SplitBars Y positions (in pixel or percentage if percent==TRUE)
	int SplitMainState[] = { 3, 1, width, height, 0, 0};
	_SplitterLoadState(m_SplitMain, SplitMainState, sizeof(SplitMainState) / sizeof(int));
	m_SplitMain.Create(-1, rect, this, IDC_SPLIT_MAIN);


}
void DistributedNetworkClassDialogEditorDlg::_SplitterLoadState(VISplitterWnd &splitter, int statebuf[], int num) const
{
	XArray<int> state;
	for (int i = 0; i < num; ++i)
		state.PushBack(statebuf[i]);
	splitter.LoadState(state);
}


//called on WM_DESTROY
void DistributedNetworkClassDialogEditorDlg::OnInterfaceEnd() 
{
}

HRESULT DistributedNetworkClassDialogEditorDlg::ReceiveNotification(UINT MsgID,DWORD Param1,DWORD Param2,CKScene* Context)
{
	switch(MsgID)
	{
	//sample code : 
	case CUIK_NOTIFICATION_CKFILELOADED:
		{
		}break;
	}
	return 0;
}