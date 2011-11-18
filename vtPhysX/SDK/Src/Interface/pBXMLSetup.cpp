#include <StdAfx2.h>
#include "PCommonDialog.h"
#include "PBodySetup.h"
#include "PBXMLSetup.h"


#include "resource.h"
#include "VITabCtrl.h"
#include "VIControl.h"
#include "..\..\include\interface\pbxmlsetup.h"


void CPBXMLSetup::fillXMLLinks()
{

	XMLExternLink.AddString("None");
	XMLInternLink.AddString("None");

	XMLExternLink.SetCurSel(0);
	XMLInternLink.SetCurSel(0);


}

int CPBXMLSetup::OnSelect(int before/* =-1 */)
{
	return -1;
}


BOOL CPBXMLSetup::OnInitDialog()
{

		
	fillXMLLinks();
	CDialog::OnInitDialog();
	/*getXMLSetup().SetMode(MultiParamEditDlg::MODELESSEDIT|MultiParamEditDlg::USER_CUSTOM_BUTTONS|MultiParamEditDlg::AUTOCLOSE_WHEN_MODELESS|MultiParamEditDlg::CREATEPARAM);
	//getXMLSetup().AttachControlSite(this,IDC_MAIN_VIEW_XML);

	CRect rcValue;
	GetWindowRect( &rcValue ); // Use picture box position.
	ScreenToClient( &rcValue );
*/
	return TRUE;
}





CPBXMLSetup::~CPBXMLSetup(){	_destroy();}
void CPBXMLSetup::_destroy(){	::CPSharedBase::_destroy();}

CPBXMLSetup::CPBXMLSetup(CKParameter* Parameter,CK_CLASSID Cid)	: 
CParameterDialog(Parameter,Cid) , CPSharedBase(this,Parameter)
{
	setEditedParameter(Parameter);
}

CPBXMLSetup::CPBXMLSetup(CKParameter* Parameter,CWnd *parent,CK_CLASSID Cid) : 
CParameterDialog(Parameter,Cid) , CPSharedBase(this,Parameter)
{
	setEditedParameter(Parameter);
}

void CPSharedBase::_dtrBodyXMLDlg(){}
CPBXMLSetup*CPBXMLSetup::refresh(CKParameter*src){	return this;}


HWND CPBXMLSetup::getDlgWindowHandle(UINT templateID)
{
	HWND result  = NULL;
	GetDlgItem(templateID,&result);
	return result;
}
LRESULT CPBXMLSetup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{

			break;

		}
	case WM_ERASEBKGND:
		{
			/*RECT r;
			GetClientRect(&r);
			CDC* pDC=CDC::FromHandle((HDC)wParam);
			pDC->FillSolidRect(&r,RGB(200,200,200));
			return 1;*/
		}break;
	case CKWM_OK:
		{
			//CEdit *valueText = (CEdit*)GetDlgItem(IDC_EDIT);
			/*CString l_strValue;
			valueText->GetWindowText(l_strValue);

			double d;
			if (sscanf(l_strValue,"%Lf",&d)) {
			parameter->SetValue(&d);
			}*/
		} break;

	case CKWM_INIT:
		{

			RECT r;
			GetClientRect(&r);
			fillXMLLinks();
			
		} break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}



void CPBXMLSetup::DoDataExchange(CDataExchange* pDX)
{

	//CDialog::DoDataExchange(pDX);
	CParameterDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBXMLSetup)
	DDX_Control(pDX, IDC_XINTERN_LINK2, XMLInternLink);
	DDX_Control(pDX, IDC_XEXTERN_LINK, XMLExternLink);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CPBXMLSetup, CParameterDialog)

	ON_STN_CLICKED(IDC_XML_MAIN_VIEW, OnStnClickedXmlMainView)
END_MESSAGE_MAP()


void CPBXMLSetup::OnStnClickedXmlMainView()
{
	// TODO: Add your control notification handler code here
}
