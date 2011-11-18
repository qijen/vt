#if !defined(AFX_TABONE_H__4F1DD92C_C67D_48AE_A73F_02D7EDA0580E__INCLUDED_)
#define AFX_TABONE_H__4F1DD92C_C67D_48AE_A73F_02D7EDA0580E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabOne.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PBodyQuickPage dialog

class PBodyQuickPage : public CDialog
{
// Construction
public:
	PBodyQuickPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PBodyQuickPage)
	enum { IDD = IDD_PBCOMMON };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PBodyQuickPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PBodyQuickPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABONE_H__4F1DD92C_C67D_48AE_A73F_02D7EDA0580E__INCLUDED_)
