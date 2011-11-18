/********************************************************************
created:	2008/01/14
created:	14:1:2008   12:14
filename: 	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer\CustomPlayerDialogErrorPage.cpp
file path:	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer
file base:	CustomPlayerDialogErrorPage
file ext:	cpp
author:		mc007

purpose:	Displays an Error Tab, the text is set by CustomPlayerDialog.InitDialog !
*********************************************************************/
// CErrorPage dialog
#pragma once
#include "resourceplayer.h"
#include "afxwin.h"
#include "afxcmn.h"
class CustomPlayerDialogErrorPage : public CPropertyPage
{
// Construction
public:
	CustomPlayerDialogErrorPage();

// Dialog Data
	//{{AFX_DATA(CErrorPage)
	enum { IDD = IDD_ERROR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CErrorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CErrorPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	CRichEditCtrl m_ErrorRichText;
	afx_msg void OnEnSetfocusErrorRichtText();
	CString errorText;

	afx_msg void OnEnLinkErrorRichtText(NMHDR *pNMHDR, LRESULT *pResult);
};
