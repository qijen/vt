#pragma once

#include "StdAfx2.h"
#include "VIControls.h"
#include "ParameterDialog.h"
#include "CKShader.h"
#include "ParameterDialog.h"
//#include "CUIKNotificationReceiver.h"

//--- Include "GenericObjectParameterDialog.h" from CK2UI define IDDs to mak it compile
#define IDD_GENOBJECTDIALOG             2011
#define IDD_BASEPARAMDIALOG             2000
#include "Parameters\GenericObjectParameterDialog.h"

#include "resource.h"


//--- Some constants
#define MFC_NAME_OF_DIALOG "#32770"
#define CHECK_MATERIAL_TIMER 57

class CPBCommonDialog : public CParameterDialog
{

public:
	

	bool InitChildWin( CDialog* pDlg, UINT iWinID,int otherID );
	LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);


	
	//virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */);

	VIComboBox			HType;
	VIStaticText  LBL_HType;
	VIStaticText  LBL_Flags;
	VIStaticText  LBL_DFlags;

		
	VICheckButton			BF_Move;
	VICheckButton			BF_Grav;
	VICheckButton			BF_Collision;
	VICheckButton			BF_CollisionNotify;
	
	VICheckButton			BF_Kinematic;
	VICheckButton			BF_TriggerShape;
	VICheckButton			BF_SubShape;
	VICheckButton			BF_Sleep;
	VICheckButton			BF_Hierarchy;

	VICheckButton			BF_Deformable;

	VIStaticRectangle		BF_BG_Rect;
	VIStaticRectangle		BF_FLEX_Rect;

	CButton		FlexButton;
	CStatic mPlaceHolder;


	VIStaticRectangle mDynaFlagsRect;


	VICheckButton			TF_POS;
	VICheckButton			TF_ROT;
	


	VICheckButton			TF_PX;		VICheckButton			TF_RX;
	VICheckButton			TF_PY;		VICheckButton			TF_RY;
	VICheckButton			TF_PZ;		VICheckButton			TF_RZ;


	CToolTipCtrl *m_tt;


	int					m_paramType;

	CPBCommonDialog(CKParameter* Parameter,CK_CLASSID Cid=CKCID_OBJECT) : CParameterDialog(Parameter,Cid) 
	{		
		setEditedParameter(Parameter); 	
		m_tt =NULL;

		


	}
	virtual ~CPBCommonDialog() {	}

	CKParameter *m_EditedParameter;
	
	CKParameter * getEditedParameter() const { return m_EditedParameter; }
	void setEditedParameter(CKParameter * val) { m_EditedParameter = val; }
	virtual CKBOOL On_Init();

	// associated resource id :
	enum { IDD = IDD_PBCOMMON };
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPBCommonDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void fillHullType();
	void fillFlags();
	void fillTransformationFlags();

public:

	
	
	virtual BOOL On_UpdateFromParameter(CKParameter* p){

//		if(!p) p = (CKParameterOut *)CKGetObject(m_EditedParameter);
//		if(!p) return FALSE;

		return TRUE;
	}

	virtual BOOL On_UpdateToParameter(CKParameter* p){


/*		if(!p) p = (CKParameterOut *)CKGetObject(m_EditedParameter);
		if(!p) return FALSE;

		CString cstr;
/*
		
		
*/
		return TRUE;
	}

		
	
	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	

public:


	CKParameter *parameter;
	VIEdit editValue;
	VIStaticText textValue;
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnStnClickedDynaFlagsRect();
};