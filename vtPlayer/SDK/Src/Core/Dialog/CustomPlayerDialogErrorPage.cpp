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
#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"
#include "resourceplayer.h"

#include "CustomPlayerDialogErrorPage.h"
#include "CustomPlayerDialog.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CErrorPage dialog



CustomPlayerDialogErrorPage::CustomPlayerDialogErrorPage() : CPropertyPage(CustomPlayerDialogErrorPage::IDD)
{
	//{{AFX_DATA_INIT(CErrorPage)
	//}}AFX_DATA_INIT
}

void CustomPlayerDialogErrorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ERROR_RICHT_TEXT, m_ErrorRichText);
}



BEGIN_MESSAGE_MAP(CustomPlayerDialogErrorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CErrorPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_ERROR_RICHT_TEXT, OnEnSetfocusErrorRichtText)
	ON_NOTIFY(EN_LINK, IDC_ERROR_RICHT_TEXT, OnEnLinkErrorRichtText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorPage message handlers

void CustomPlayerDialogErrorPage::OnEnSetfocusErrorRichtText()
{
	if (m_ErrorRichText)
	{
		CRect rc;
		m_ErrorRichText.GetWindowRect(rc);
		ScreenToClient(rc);
		DWORD newStyle= ES_CENTER;
		DWORD style= m_ErrorRichText.GetStyle();
		style&= ~ES_CENTER;
		style|= newStyle;
		style|= WS_VSCROLL;

		DWORD exstyle= m_ErrorRichText.GetExStyle();
		m_ErrorRichText.SetAutoURLDetect();
		m_ErrorRichText.SetEventMask(ENM_LINK);
		m_ErrorRichText.SetFont(GetFont());

		m_ErrorRichText.SetWindowText(errorText);
		m_ErrorRichText.UpdateData();
		m_ErrorRichText.UpdateWindow();
		m_ErrorRichText.HideCaret();

	}
	
}

void CustomPlayerDialogErrorPage::OnEnLinkErrorRichtText(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	ENLINK *el= reinterpret_cast<ENLINK *>(pNMHDR);
	*pResult = 0;

	if (el->msg == WM_LBUTTONDOWN)
	{
		CString link;
		m_ErrorRichText.GetTextRange(el->chrg.cpMin, el->chrg.cpMax, link);

		ShellExecute(*this, NULL, link, NULL, _T(""), SW_SHOWNORMAL);
	}
}
