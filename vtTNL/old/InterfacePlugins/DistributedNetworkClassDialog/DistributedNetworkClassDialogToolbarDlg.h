#pragma once

// DistributedNetworkClassDialogToolbarDlg.h : header file
//

//toolbar dialog creation function, to be called by Virtools Dev
DllToolbarDlg*	fCreateToolbarDlg(HWND parent);

/////////////////////////////////////////////////////////////////////////////
// ToolbarDlg dialog

class DistributedNetworkClassDialogToolbarDlg : public DllToolbarDlg
{
public:
	//called on creation of the dialog by Virtools Dev interface
	//the PluginInterface will be avalaible only when the OnInterfaceInit() has been called
	virtual void OnInterfaceInit();
	//called on destruction of the dialog by Virtools Dev interface
	virtual void OnInterfaceEnd();
	//callback for receiving notification
	virtual HRESULT ReceiveNotification(UINT MsgID,DWORD Param1=0,DWORD Param2=0,CKScene* Context=NULL);


// Construction
public:
	DistributedNetworkClassDialogToolbarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DistributedNetworkClassDialogToolbarDlg)
	enum { IDD = IDD_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DistributedNetworkClassDialogToolbarDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DistributedNetworkClassDialogToolbarDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
