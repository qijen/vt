#pragma once
#include "StdAfx2.h"
#include "VIControls.h"
#include "ParameterDialog.h"
#include "CKShader.h"
//#include "CUIKNotificationReceiver.h"

//--- Include "GenericObjectParameterDialog.h" from CK2UI define IDDs to mak it compile
#define IDD_GENOBJECTDIALOG             2011
#define IDD_BASEPARAMDIALOG             2000
#include "Parameters\GenericObjectParameterDialog.h"

#include "resource.h"
#include "PCommonDialog.h"


//--- Some constants
#define MFC_NAME_OF_DIALOG "#32770"
#define CHECK_MATERIAL_TIMER 57
class CPBXMLSetup : public CParameterDialog ,public CPSharedBase
{

public:

	//	virtual void PreSubclassWindow();

	int					m_paramType;

	CPBXMLSetup(CKParameter* Parameter,CWnd* parent = NULL);
	CPBXMLSetup(CKParameter* Parameter,CWnd *parent,CK_CLASSID Cid=CKCID_OBJECT);
	CPBXMLSetup(CKParameter* Parameter,CK_CLASSID Cid=CKCID_OBJECT);
	virtual ~CPBXMLSetup();
	void _destroy();

	//BOOL Create(CKParameter* Parameter,UINT nIDTemplate, CWnd* pParentWnd);
	//BOOL Init(CKParameter* Parameter,UINT nIDTemplate,CParameterDialog *parent);
	
	//BOOL OnInitDialog();


	CPBXMLSetup* refresh(CKParameter*src);
	/************************************************************************/
	/* Overrides 
	*/
	/************************************************************************/
	void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);


	/************************************************************************/
	/* Accessors                                                                      */
	/************************************************************************/
	CKParameter * getEditedParameter() const { return mParameter; }
	void setEditedParameter(CKParameter * val) { mParameter= val; }

	/************************************************************************/
	/* Virtools mParameter transfer callbacks :                                                                      */
	/************************************************************************/
	virtual BOOL On_UpdateFromParameter(CKParameter* p){

		//		if(!p) p = (CKParameterOut *)CKGetObject(m_EditedParameter);
		//		if(!p) return FALSE;

		return TRUE;
	}

	virtual BOOL On_UpdateToParameter(CKParameter* p)
	{

		/*
		if(!p) p = (CKParameterOut *)CKGetObject(m_EditedParameter);
		if(!p) return FALSE;
		CString cstr;*/
		return TRUE;

	}
	


public:

	
	/************************************************************************/
	/* Low Level passes                                                                     */
	/************************************************************************/
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);



	/************************************************************************/
	/* Logical Actions                                                                     */
	/************************************************************************/
	virtual int OnSelect(int before=-1);

	void fillXMLLinks();

	HWND getDlgWindowHandle(UINT templateID);

	/************************************************************************/
	/* Members                                                                      */
	/************************************************************************/

	//		Hull Type 

	VIComboBox XMLInternLink;	
	VIStaticText  XMLInternLinkLbl;
	VIComboBox XMLExternLink;	
	VIStaticText  XMLExternLinkLbl;
	CKParameter *mParameter;
	VIEdit editValue;
	VIStaticText textValue;
	VIComboBox			type;

	
	enum { IDD = IDD_PB_XML_PARENT };
	
	//{{AFX_VIRTUAL(CPBXMLSetup)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPBXMLSetup)
	BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
public:
	afx_msg void OnStnClickedXmlMainView();
};
