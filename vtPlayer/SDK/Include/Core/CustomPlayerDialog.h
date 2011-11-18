/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   11:57
	filename: 	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer\CustomPlayerDialog.h
	file path:	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer
	file base:	CustomPlayerDialog
	file ext:	h
	author:		mc007
	
	purpose:	 Dialog for the Custom Player : 
		-	sets the values in the player.ini
		-	contains an about tab
		-	contains an error tab when necessary 

*********************************************************************/
#pragma once

#include "CustomPlayerDialogGraphicPage.h"
#include "CustomPlayerDialogErrorPage.h"
#include "CustomPlayerDialogAboutPage.h"

class CustomPlayerDialog : public CPropertySheet
{
public:
	DECLARE_DYNAMIC(CustomPlayerDialog)
	//standard constructor, not used !
	//CustomPlayerDialog(CWnd* pWndParent = NULL);


	// explicit constructor, the only one we use !, an error string is passed here, when strlen(errorText) >0 then we have an error tab !
	CustomPlayerDialog(CWnd* pWndParent = NULL,CString errorText="");

	CustomPlayerDialogGraphicPage m_GraphicPage;
	CustomPlayerDialogErrorPage m_errorPage;
	CustomPlayerDialogAboutPage m_aboutPage;
	CString m_ErrorText;
	
	// Overrides : 

	//
	//************************************
	// Method:    OnInitDialog
	// FullName:  CustomPlayerDialog::OnInitDialog
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Summary : 
	//			- fills the configuration tab 

	//************************************
	virtual BOOL OnInitDialog();

	// Message Handlers
protected:
	//{{AFX_MSG(CModalShapePropSheet)
	afx_msg void OnApplyNow();
	
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
public:
	
};
