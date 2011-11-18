#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"
#include "resourceplayer.h"


#include "CustomPlayerDialogAboutPage.h"
#include "CustomPlayerDialog.h"

#include "commonhelpers.h"
#include "CustomPlayerDefines.h"
#include "mdexceptions.h"




#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutPage dialog


namespace
{
	enum
	{
		RE_CONTROL = 4711	// Id of the RichEdit Control
	};

	// Retrieve the GPL text from our resources
	CString GetTextResource(UINT id)
	{
		CString s;

		HGLOBAL hresource = NULL;
		try
		{
			HRSRC hrsrc= FindResource(NULL, MAKEINTRESOURCE(id), _T("TEXT"));
			if (hrsrc == NULL)
				MdThrowLastWinerror();

			DWORD dwSize= SizeofResource(AfxGetInstanceHandle(), hrsrc);
			if (dwSize == 0)
				MdThrowLastWinerror();

			hresource= LoadResource(NULL, hrsrc);
			const BYTE *pData= (const BYTE *)LockResource(hresource);

			CComBSTR bstr(dwSize, (LPCSTR)pData);

			s= bstr;
		}
		catch (CException *pe)
		{
			pe->ReportError();
			pe->Delete();
		}

		if (hresource != NULL)
			FreeResource(hresource);

		return s;
	}
}

CustomPlayerDialogAboutPage::CustomPlayerDialogAboutPage() : CPropertyPage(CustomPlayerDialogAboutPage::IDD)
{
	//{{AFX_DATA_INIT(CAboutPage)
	//}}AFX_DATA_INIT
}

void CustomPlayerDialogAboutPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ERROR_RICHT_TEXT, m_ErrorRichText);
}



BEGIN_MESSAGE_MAP(CustomPlayerDialogAboutPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS(IDC_ERROR_RICHT_TEXT, OnEnSetfocusErrorRichtText)
	ON_NOTIFY(EN_LINK, IDC_ERROR_RICHT_TEXT, OnEnLinkErrorRichtText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutPage message handlers

void CustomPlayerDialogAboutPage::OnEnSetfocusErrorRichtText()
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


		CString text;

		CFile aboutFile;
		CFileException ex;

		char Ini[MAX_PATH];
		char drive[MAX_PATH];
		char dir[MAX_PATH];
		char szPath[MAX_PATH];
        

		GetModuleFileName(NULL,szPath,_MAX_PATH);
		_splitpath(szPath, drive, dir, NULL, NULL );
		sprintf(Ini,"%s%s%s",drive,dir,"playerinfo.dat");

		if(aboutFile.Open(Ini,CFile::modeNoTruncate|CFile::modeReadWrite|CFile::shareExclusive,&ex))
		{
			char pbuf[16000];
			DWORD length  = aboutFile.Read(pbuf,16000);
			XString b(pbuf);
			XString bufferOut  = b.Create(b.Str(),length);

			text.SetString(bufferOut.Str());
			aboutFile.Close();

		}else
		{
			text  = GetTextResource(IDR_ABOUT);
		}

		  
		m_ErrorRichText.SetWindowText(text);
		m_ErrorRichText.UpdateData();
		m_ErrorRichText.UpdateWindow();
		m_ErrorRichText.HideCaret();

	}
	
}

void CustomPlayerDialogAboutPage::OnEnLinkErrorRichtText(NMHDR *pNMHDR, LRESULT *pResult)
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
