#pragma once

#include "StdAfx2.h"
#include "VIControls.h"
#include "ParameterDialog.h"
//#include "CUIKNotificationReceiver.h"

//--- Include "GenericObjectParameterDialog.h" from CK2UI define IDDs to mak it compile


#include "resource.h"
#include "PCommonDialog.h"
#include "afxwin.h"


//--- Some constants
#define MFC_NAME_OF_DIALOG "#32770"
#define CHECK_MATERIAL_TIMER 57

class CPBodyCfg : public CParameterDialog
{

public:

	//	virtual void PreSubclassWindow();

	int					m_paramType;

	CPBodyCfg(CKParameter* Parameter,CK_CLASSID Cid=CKCID_OBJECT);
	virtual ~CPBodyCfg();
	void _destroy();

	//virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd);
	virtual void Init(CParameterDialog *parent);
	
	CParameterDialog* refresh(CKParameter*src);
	void initSplitter();

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
	enum { IDD = IDD_PBCOMMON };


	virtual int OnSelect(int before=-1);
	
	
	
	//{{AFX_VIRTUAL(CPBodyCfg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	//{{AFX_MSG(CPBodyCfg)
	
	//}}AFX_MSG

public:

	CParameterDialog *dlgDeformableSettings;
	//VITrackCtrl mTestViControl;
	//VITreeCtrl mTestViControl;
	VITabCtrl mTestViControl;
	CParameterDialog *mParent;


	/************************************************************************/
	/* Low Level passes                                                                     */
	/************************************************************************/
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);



	/************************************************************************/
	/* Members                                                                      */
	/************************************************************************/


	//		Hull Type 

	VIComboBox HType;		VIStaticText  LBL_HType;
	
	void fillHullType();
	
	

	//		Body main Flags 

	VIStaticText  LBL_Flags;	

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

	void fillFlags();

	//			Transformation Lock Flags

	VIStaticText  LBL_TransformationLockFlags;
	VICheckButton			TF_POS;
	VICheckButton			TF_ROT;
	VICheckButton			TF_PX;		VICheckButton			TF_RX;
	VICheckButton			TF_PY;		VICheckButton			TF_RY;
	VICheckButton			TF_PZ;		VICheckButton			TF_RZ;


	void fillTransformationFlags();


public:


	CKParameter *mParameter;
	VIEdit editValue;
	VIStaticText textValue;
	VIComboBox			type;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox UserLevel;
	afx_msg void OnCbnSelchangeHulltype2();
	afx_msg void OnStnClickedLblFlags();
	afx_msg void OnStnClickedDynaFlagsRect();
};

