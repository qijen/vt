#pragma once

#include "PhysicManager.h"


// vtAgeiaInterfaceEditorDlg.h : header file
//

//editor dialog creation function, to be called by Virtools Dev
DllEditorDlg*	fCreateEditorDlg(HWND parent);

/////////////////////////////////////////////////////////////////////////////
// EditorDlg dialog

class vtAgeiaInterfaceEditorDlg : public DllEditorDlg
{
public:
	//called on creation of the dialog by Virtools Dev interface
	//the PluginInterface will be avalaible only when the OnInterfaceInit() has been called
	virtual void	OnInterfaceInit();
	//called on destruction of the dialog by Virtools Dev interface
	virtual void	OnInterfaceEnd();
	//callback for receiving notification
	virtual HRESULT ReceiveNotification(UINT MsgID,DWORD Param1=0,DWORD Param2=0,CKScene* Context=NULL);

	int				OnGlobalKeyboardShortcut(int commandID);
	int				OnLocalKeyboardShortcut(int commandID);
	virtual void	OnCustomMenu(CMenu* menu,int startingCommandID,int endingCommandID);	//fill custom menu, use commandID= baseID+startingCommandID
	virtual void	OnCustomMenu(int commandID);	//callback when custom menu used, commandID==baseID (without the startingCommandID)
	//Create a CTooltipCtrl for tooltip management
	void CreateTooltip();

	CKContext *mContext;
	PhysicManager *pMan;
	PhysicManager *getPMan(){return pMan;}
	CKContext *getContext(){return mContext;}

	void init(CKContext *ctx,PhysicManager *pm);
	void objectPosChanged(CK_ID objID);
	void objectSelChanged(DWORD par1,DWORD par2);

	virtual BOOL LoadData(CKInterfaceObjectManager* iom);
	virtual BOOL SaveData(CKInterfaceObjectManager* iom);


protected:
	//tooltip
	CToolTipCtrl	m_wndToolTip;

// Construction
public:
	vtAgeiaInterfaceEditorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(vtAgeiaInterfaceEditorDlg)
	enum { IDD = IDD_EDITOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(vtAgeiaInterfaceEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(vtAgeiaInterfaceEditorDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

