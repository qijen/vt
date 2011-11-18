#pragma once

#include "StdAfx2.h"
#include "VIControls.h"
#include "ParameterDialog.h"
#include "CKShader.h"
#include "ParameterDialog.h"
#include "MultiParamEditDlg.h"

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

class CPBXMLSetup : public MultiParamEditDlg , public CPSharedBase
{

public:

	//	virtual void PreSubclassWindow();

	int					m_paramType;

	CPBXMLSetup(CKContext* context,CWnd* parent = NULL);
	CPBXMLSetup(CKParameter* Parameter,CK_CLASSID Cid=CKCID_OBJECT);
	virtual ~CPBXMLSetup();
	void _destroy();

	//virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd);
	virtual void Init(CParameterDialog *parent);

	CParameterDialog* refresh(CKParameter*src);
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



	// associated resource id :
	enum { IDD = IDD_PBCOMMON_DEFORMABLE };



	/************************************************************************/
	/* MFC	                                                                     */
	/************************************************************************/
	//{{AFX_VIRTUAL(CPBXMLSetup)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CPBXMLSetup)
	//ON_REGISTERED_MESSAGE(_afxMsgMouseWheel, OnRegisteredMouseWheel);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//}}AFX_MSG

public:


	/************************************************************************/
	/* Low Level passes                                                                     */
	/************************************************************************/
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);



	/************************************************************************/
	/* Members                                                                      */
	/************************************************************************/


public:


	CKParameter *mParameter;
	VIEdit editValue;
	VIStaticText textValue;
	VIComboBox			type;

	DECLARE_MESSAGE_MAP()

	
};
