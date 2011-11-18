/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   12:06
	filename: 	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer\AboutPage.h
	file path:	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer
	file base:	AboutPage
	file ext:	h
	author:		mc007
	
	purpose:	Show an about text via a rich text control given from the projects resource file about.txt !
*********************************************************************/
#pragma once
#include "resourceplayer.h"
#include "afxwin.h"
#include "afxcmn.h"
class CustomPlayerDialogAboutPage : public CPropertyPage
{
// Construction
public:
	CustomPlayerDialogAboutPage();

// Dialog Data
	//{{AFX_DATA(CAboutPage)
	enum { IDD = IDD_ABOUTP };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAboutPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CAboutPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	CRichEditCtrl m_ErrorRichText;
	afx_msg void OnEnSetfocusErrorRichtText();
	CString errorText;

	afx_msg void OnEnLinkErrorRichtText(NMHDR *pNMHDR, LRESULT *pResult);
};
