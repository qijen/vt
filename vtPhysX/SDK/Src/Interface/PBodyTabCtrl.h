#ifndef __PBODY_TAB_CONTRL_H__
#define __PBODY_TAB_CONTRL_H__

#include "VIControls.h"


/////////////////////////////////////////////////////////////////////////////
// PBodyTabContrl window

class PBodyTabContrl : public VITabCtrl
{
	DECLARE_DYNCREATE(PBodyTabContrl)

public:
	PBodyTabContrl();
public:
	enum { IDD = IDC_PBODY_TAB_PANE };
	
	PBodyTabContrl(CWnd*win);
	virtual ~PBodyTabContrl();
	CDialog *m_tabPages[1];
	int m_tabCurrent;
	int m_nNumberOfPages;
	void _construct();

// Attributes
public:

// Operations
public:
	void Init();
	void SetRectangle();

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif 
