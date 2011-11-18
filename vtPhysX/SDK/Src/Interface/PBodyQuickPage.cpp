#include "stdafx2.h"
#include "PBodyTabCtrl.h"
#include "PBodyQuickPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PBodyQuickPage dialog


PBodyQuickPage::PBodyQuickPage(CWnd* pParent /*=NULL*/)
	: CDialog(PBodyQuickPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(PBodyQuickPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void PBodyQuickPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PBodyQuickPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PBodyQuickPage, CDialog)
	//{{AFX_MSG_MAP(PBodyQuickPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PBodyQuickPage message handlers
