// PBRootForm.cpp : implementation file
//

#include "stdafx2.h"

#include "PBRootForm.h"


// PBRootForm

IMPLEMENT_DYNCREATE(PBRootForm, CFormView)

PBRootForm::PBRootForm()
	: CFormView(PBRootForm::IDD)
{

}

PBRootForm::~PBRootForm()
{
}

void PBRootForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(PBRootForm, CFormView)
END_MESSAGE_MAP()


// PBRootForm diagnostics

#ifdef _DEBUG
void PBRootForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void PBRootForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// PBRootForm message handlers
