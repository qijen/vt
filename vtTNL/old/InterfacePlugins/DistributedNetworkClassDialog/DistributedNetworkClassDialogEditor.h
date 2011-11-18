// DistributedNetworkClassDialogEditor.h : main header file for the EDITOR DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class DistributedNetworkClassDialogEditorDlg;
class DistributedNetworkClassDialogToolbarDlg;

extern DistributedNetworkClassDialogEditorDlg*		g_Editor;
extern DistributedNetworkClassDialogToolbarDlg*		g_Toolbar;

//plugin interface for communication with Virtools Dev
extern PluginInterface*		s_Plugininterface;

/////////////////////////////////////////////////////////////////////////////
// CEditorApp
// See Editor.cpp for the implementation of this class
//

class DistributedNetworkClassDialogEditorApp : public CWinApp
{
protected:
	void	InitImageList();
	void	DeleteImageList();
	CImageList		m_ImageList;

public:
	DistributedNetworkClassDialogEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DistributedNetworkClassDialogCEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(DistributedNetworkClassDialogCEditorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
