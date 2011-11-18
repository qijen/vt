#pragma once



// PBRootForm form view

class PBRootForm : public CFormView
{
	DECLARE_DYNCREATE(PBRootForm)

protected:
	PBRootForm();           // protected constructor used by dynamic creation
	virtual ~PBRootForm();

public:
	enum { IDD = PBRootMDI };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


