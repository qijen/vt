#include "PCommonDialog.h"
#include "PBodySetup.h"
#include "PBXMLSetup.h"
#include "resource.h"
#include "VITabCtrl.h"

#include "VIControl.h"


#define LAYOUT_STYLE			(WS_CHILD|WS_VISIBLE)
#define LAYOUT_ShaderREE		130

/*
MultiParamEditDlg* CPBXMLSetup::refresh(CKParameter*src)
{	return this;	}
*/
void CPBXMLSetup::Init(CParameterDialog *parent){}
CPBXMLSetup::~CPBXMLSetup(){	_destroy();}
void CPBXMLSetup::_destroy(){	::CPSharedBase::_destroy();}

/*
CPBXMLSetup::CPBXMLSetup(
					 CKParameter* Parameter,
					 CK_CLASSID Cid)	: 
CParameterDialog(Parameter,Cid) , 
CPSharedBase(this,Parameter)
{

	setEditedParameter(Parameter);
}
*/


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
			/*
			CDC* pDC=CDC::FromHandle((HDC)wParam);*/
			//initSplitter();
			char temp[64];
			double d;
		} break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPBXMLSetup::DoDataExchange(CDataExchange* pDX)
{

	//CDialog::DoDataExchange(pDX);
	CParameterDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBXMLSetup)

	//}}AFX_DATA_MAP

}



BEGIN_MESSAGE_MAP(CPBXMLSetup, CParameterDialog)
END_MESSAGE_MAP()

