/********************************************************************
	created:	2008/01/14
	created:	14:1:2008   12:02
	filename: 	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer\stylepge.h
	file path:	x:\junctions\ProjectRoot\vdev\Sdk\Samples\Runtime\kamplayer
	file base:	stylepge
	file ext:	h
	author:		mc007
	
	purpose:	Displays and stores settings in/from the player.ini 
*********************************************************************/
#pragma once
#include "resourceplayer.h"
#include "afxwin.h"
class CustomPlayerDialogGraphicPage : public CPropertyPage
{
// Construction
public:
	CustomPlayerDialogGraphicPage();

// Dialog Data
	//{{AFX_DATA(CStylePage)
	enum { IDD = IDD_STYLE };
	int     m_nShapeStyle;
	int m_FSSA;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStylePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnOK();
	
	// Generated message map functions
	//{{AFX_MSG(CStylePage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_windowMode;
	CListBox m_Driver;
	CComboBox m_FModes;
	CComboBox m_Bpps;
	CComboBox m_RRates;
	
	BOOL m_FullScreen;
	afx_msg void OnLbnSelchangeDriverList();
	CButton m_FullScreenBtn;
	CComboBox m_CB_FSSA;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCbFmode();
	afx_msg void OnCbnSelchangeCbBpps();
	afx_msg void OnCbnSelchangeCbRrates();
	afx_msg void OnBnClickedCheckbFullscreen();
	afx_msg void OnCbnSelchangeCbFssa();
};
