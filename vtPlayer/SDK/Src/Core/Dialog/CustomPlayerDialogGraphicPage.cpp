#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"
#include "resourceplayer.h"

#include "CustomPlayerDialogGraphicPage.h"
#include "CustomPlayerDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern void _UpdateResolutionLists(HWND hwndDlg,int DriverId);


/////////////////////////////////////////////////////////////////////////////
// CStylePage dialog



CustomPlayerDialogGraphicPage::CustomPlayerDialogGraphicPage() : CPropertyPage(CustomPlayerDialogGraphicPage::IDD)
, m_FullScreen(FALSE)
, m_FSSA(0)
{
	//{{AFX_DATA_INIT(CStylePage)
	//}}AFX_DATA_INIT
}

void CustomPlayerDialogGraphicPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStylePage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CB_WMODES, m_windowMode);
	DDX_Control(pDX, IDC_DRIVER_LIST, m_Driver);
	DDX_Control(pDX, IDC_CB_FMODE, m_FModes);
	DDX_Control(pDX, IDC_CB_BPPS, m_Bpps);
	DDX_Control(pDX, IDC_CB_RRATES, m_RRates);
	
	DDX_Control(pDX, IDC_CHECKB_FULLSCREEN, m_FullScreenBtn);
	DDX_Control(pDX, IDC_CB_FSSA, m_CB_FSSA);

}



BEGIN_MESSAGE_MAP(CustomPlayerDialogGraphicPage, CPropertyPage)
	//{{AFX_MSG_MAP(CStylePage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_DRIVER_LIST, OnLbnSelchangeDriverList)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_CB_FMODE, OnCbnSelchangeCbFmode)
	ON_CBN_SELCHANGE(IDC_CB_BPPS, OnCbnSelchangeCbBpps)
	ON_CBN_SELCHANGE(IDC_CB_RRATES, OnCbnSelchangeCbRrates)
	ON_BN_CLICKED(IDC_CHECKB_FULLSCREEN, OnBnClickedCheckbFullscreen)
	ON_CBN_SELCHANGE(IDC_CB_FSSA, OnCbnSelchangeCbFssa)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStylePage message handlers

void CustomPlayerDialogGraphicPage::OnLbnSelchangeDriverList()
{
    m_Bpps.ResetContent();
	m_RRates.ResetContent();
	m_windowMode.ResetContent();
	m_FModes.ResetContent();
	
	_UpdateResolutionLists(NULL, m_Driver.GetCurSel());
	

	SetModified();
	
	// TODO: Add your control notification handler code here
}

void CustomPlayerDialogGraphicPage::OnCbnSelchangeCombo1()
{

	SetModified();

}

void CustomPlayerDialogGraphicPage::OnCbnSelchangeCbFmode()
{
	SetModified();
}

void CustomPlayerDialogGraphicPage::OnCbnSelchangeCbBpps()
{

	SetModified();
}

void CustomPlayerDialogGraphicPage::OnCbnSelchangeCbRrates()
{

	SetModified();
}

void CustomPlayerDialogGraphicPage::OnBnClickedCheckbFullscreen()
{
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	ewinfo->g_GoFullScreen = m_FullScreenBtn.GetCheck();

	if (ewinfo->g_GoFullScreen)
	{
		if (ewinfo->g_FullScreenDriver <  m_Driver.GetCount() )
		{
			m_Driver.SetCurSel( ewinfo->g_FullScreenDriver );
		}
	}else
	{
		if (ewinfo->g_WindowedDriver <  m_Driver.GetCount() )
		{
			m_Driver.SetCurSel( ewinfo->g_WindowedDriver );
		}
	}
	SetModified();
}

void CustomPlayerDialogGraphicPage::OnCbnSelchangeCbFssa()
{
	SetModified();
}

void CustomPlayerDialogGraphicPage::OnOK()
{
	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();

	//////////////////////////////////////////////////////////////////////////
	CString bpp;
	m_Bpps.GetLBText(m_Bpps.GetCurSel(),bpp);
	XString oBpp(bpp);
	ewinfo->g_Bpp = oBpp.ToInt();

	//////////////////////////////////////////////////////////////////////////
	CString rr;
	m_RRates.GetLBText(m_RRates.GetCurSel(),rr);
	XString oRR(rr);
	ewinfo->g_RefreshRate= oRR.ToInt();
	//////////////////////////////////////////////////////////////////////////
	CString fssa;
	m_CB_FSSA.GetLBText(m_CB_FSSA.GetCurSel(),fssa);
	XString oFSSA(fssa);
	ewinfo->FSSA= oFSSA.ToInt();

	//////////////////////////////////////////////////////////////////////////
	ewinfo->g_GoFullScreen = m_FullScreenBtn.GetCheck();
	if (ewinfo->g_GoFullScreen)
	{
		ewinfo->g_FullScreenDriver  = m_Driver.GetCurSel();
	}else
		ewinfo->g_WindowedDriver = m_Driver.GetCurSel();


	//////////////////////////////////////////////////////////////////////////
	CString fmode;
	m_FModes.GetLBText(m_FModes.GetCurSel(),fmode);



	XStringTokenizer	tokizer(fmode, "x");
	const char*tok = NULL;
	tok = tokizer.NextToken(tok);
	XString width(tok);
	tok = tokizer.NextToken(tok);
	XString height(tok);
	ewinfo->g_Width = width.ToInt();
	ewinfo->g_Height = height.ToInt();

	{

		CString wmode;
		m_windowMode.GetLBText(m_windowMode.GetCurSel(),wmode);
		XStringTokenizer	tokizer(wmode, "x");
		const char*tok = NULL;
		tok = tokizer.NextToken(tok);
		XString width(tok);
		tok = tokizer.NextToken(tok);
		XString height(tok);
		ewinfo->g_WidthW = width.ToInt();
		ewinfo->g_HeightW = height.ToInt();
	}

	//GetApp()->PSaveEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE,*GetPlayer().GetEngineWindowInfo());
	
}