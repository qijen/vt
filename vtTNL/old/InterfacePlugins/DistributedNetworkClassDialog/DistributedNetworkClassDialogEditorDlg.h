#pragma once

// DistributedNetworkClassDialogEditorDlg.h : header file
//

//editor dialog creation function, to be called by Virtools Dev
DllEditorDlg*	fCreateEditorDlg(HWND parent);

/////////////////////////////////////////////////////////////////////////////
// EditorDlg dialog

class DistributedNetworkClassDialogEditorDlg : public DllEditorDlg
{
public:
	//called on creation of the dialog by Virtools Dev interface
	//the PluginInterface will be avalaible only when the OnInterfaceInit() has been called
	virtual void	OnInterfaceInit();
	//called on destruction of the dialog by Virtools Dev interface
	virtual void	OnInterfaceEnd();
	//callback for receiving notification
	virtual HRESULT ReceiveNotification(UINT MsgID,DWORD Param1=0,DWORD Param2=0,CKScene* Context=NULL);

	void _SetupSplitters();
	void _SplitterLoadState(VISplitterWnd &splitter, int statebuf[], int num) const;




// Construction
public:
	DistributedNetworkClassDialogEditorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DistributedNetworkClassDialogEditorDlg)
	enum { IDD = IDD_EDITOR };
	VISplitterWnd	m_SplitMain;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DistributedNetworkClassDialogEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DistributedNetworkClassDialogEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

