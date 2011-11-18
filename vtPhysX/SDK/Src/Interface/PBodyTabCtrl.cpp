#include "stdafx2.h"
#include "resource.h"
#include "PBodyTabCtrl.h"
#include "PBodyQuickPage.h"




/*#include "TabOne.h"
#include "TabTwo.h"
#include "TabThree.h"
*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PBodyTabContrl

IMPLEMENT_DYNCREATE(PBodyTabContrl,VITabCtrl)

PBodyTabContrl::PBodyTabContrl(CWnd*win)
{


	//PBodyTabContrl
	m_tabPages[0]=new PBodyQuickPage();
	if (m_tabPages[0])
	{
		m_nNumberOfPages=1;

	}
	/*m_tabPages[1]=new CTabTwo;
	m_tabPages[2]=new CTabThree;
	*/

	m_nNumberOfPages=1;
}

PBodyTabContrl::PBodyTabContrl() : VITabCtrl()
{

	//PBodyTabContrl
	m_tabPages[0]=new PBodyQuickPage();
	if (m_tabPages[0])
	{
		m_nNumberOfPages=1;

	}
	/*m_tabPages[1]=new CTabTwo;
	m_tabPages[2]=new CTabThree;
*/

	m_nNumberOfPages=1;
}

PBodyTabContrl::~PBodyTabContrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}



void PBodyTabContrl::_construct()
{

	VITabCtrl::EnableWindow(true);
	VITabCtrl::EnableAutomation();
	VITabCtrl::ShowWindow(SW_SHOW);

	AFX_MANAGE_STATE(AfxGetStaticModuleState());



	m_tabCurrent=0;
	//m_tabPages[0]->Create(IDD_PBCOMMON, this);
	//m_tabPages[1]->Create(IDD_PBCOMMON, this);
	
	//m_tabPages[0]->ShowWindow(SW_SHOW);
	//m_tabPages[1]->ShowWindow(SW_SHOW);
	//m_tabPages[1]->ShowWindow(SW_SHOW);

	//m_tabPages[2]->ShowWindow(SW_HIDE);

	m_nNumberOfPages = 1;

	SetRectangle();
}
void PBodyTabContrl::Init()
{	
//VITabCtrl::UpdateWindow();

}

void PBodyTabContrl::SetRectangle()
{
	
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	CRect r;


	((CTabCtrl *)(this))->GetItemRect(0, &itemRect);



	//TabCtrl_GetItemRect(0, &itemRect);
	//GetItemRect

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;
/*
	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}*/
	
}

BEGIN_MESSAGE_MAP(PBodyTabContrl, VITabCtrl)
	//{{AFX_MSG_MAP(PBodyTabContrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PBodyTabContrl message handlers

/*
void PBodyTabContrl::OnLButtonDown(UINT nFlags, CPoint point) 
{


	CTabCtrl &tabContr = ((CTabCtrl*)(this));
	tabContr::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != (CTabCtrl*)GetCurFocus()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
	
	
}
*/