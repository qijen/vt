#include "StdAfx2.h"

#include "PCommonDialog.h"
#include "PBodySetup.h"
#include "..\..\include\interface\pbodysetup.h"


#define LAYOUT_STYLE			(WS_CHILD|WS_VISIBLE)
#define LAYOUT_ShaderREE		130



int CPBodyCfg::OnSelect(int before/* =-1 */)
{

	return -1;
}

CParameterDialog* CPBodyCfg::refresh(CKParameter*src)
{	return this;	}

void CPBodyCfg::fillFlags()
{

	BF_Move.SetCheck(true);
}


void CPBodyCfg::fillHullType()
{



	HType.AddString("Sphere");
	HType.AddString("Box");
	HType.AddString("Capsule");
	HType.AddString("Plane");
	HType.AddString("Convex Mesh");
	HType.AddString("Height Field");
	HType.AddString("Wheel");
	HType.AddString("Cloth");
	HType.SetCurSel(0);

}


void CPBodyCfg::initSplitter()
{
	



	//[...] //optional pre initialization steps
	//int opt = mTestViControl.Create(WS_CHILD|WS_VISIBLE,r,pWnd,IDC_TRACKTEST);
	/*if (opt)
	{*/

	//		mTestViControl.SetColors(CZC_176,CZC_BLACK,CZC_176,CZC_WHITE,CZC_128,CZC_200);
	/*
	mTestViControl.SetFont(GetVIFont(VIFONT_NORMAL),FALSE);
	mTestViControl.SetItemHeight(18);*/
	//mTestViControl.SetWindowText("Shader Tree");
	//		mTestViControl.SetStyle(NTVS_DRAWHSEPARATOR);
	//		mTestViControl.SetPreAllocSize(2);

	// Create columns
	//		mTestViControl.SetColumnCount(1);
	//mTestViControl.SetColumn(0, "Shader", 0, 10);

	//HTREEITEM newItem = mTestViControl.InsertItem("asddad", 0,0, mTestViControl.GetFirstVisibleItem() );




	//		newItem->Flags = NTVI_EDITABLE;
	//		newItem->Data = NULL;

	/*HNTVITEM newItem;
	newItem = mTestViControl.InsertItem("asddad", (HNTVITEM)NULL, TRUE);
	newItem->Flags = NTVI_EDITABLE;
	newItem->Data = NULL;*/

	//}

}
void CPBodyCfg::Init(CParameterDialog *parent)
{
	mParent = parent;

}


CPBodyCfg::~CPBodyCfg()
{
	_destroy();
}

void CPBodyCfg::_destroy()
{
	//::CPSharedBase::_destroy();

}
//CPSharedBase(this,Parameter)
CPBodyCfg::CPBodyCfg(CKParameter* Parameter,CK_CLASSID Cid)	:CParameterDialog(Parameter,Cid) 
{
	setEditedParameter(Parameter);
	//int snow =  getNbDialogs();


}


LRESULT CPBodyCfg::OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam)
{

	WORD keyState = 0;
	keyState |= (::GetKeyState(VK_CONTROL) < 0) ? MK_CONTROL : 0;
	keyState |= (::GetKeyState(VK_SHIFT) < 0) ? MK_SHIFT : 0;

	LRESULT lResult;
	HWND hwFocus = ::GetFocus();
	const HWND hwDesktop = ::GetDesktopWindow();

	if (hwFocus == NULL)
		lResult = SendMessage(WM_MOUSEWHEEL, (wParam << 16) | keyState, lParam);
	else
	{
		do {
			lResult = ::SendMessage(hwFocus, WM_MOUSEWHEEL,(wParam << 16) | keyState, lParam);
			hwFocus = ::GetParent(hwFocus);
		}
		while (lResult == 0 && hwFocus != NULL && hwFocus != hwDesktop);
	}
	return lResult;

}

LRESULT CPBodyCfg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

		/*
		case WM_MOUSEWHEEL:
		{
		int cStates = GET_KEYSTATE_WPARAM(wParam);
		int wheelDirection = GET_WHEEL_DELTA_WPARAM(wParam);

		if (wheelDirection>0)
		{
		if ( mTestViControl.GetActiveTabIndex() +1 < mTestViControl.GetTabCount() )
		{
		mTestViControl.SetActiveTab(mTestViControl.GetActiveTabIndex() +1 );
		}else{
		mTestViControl.SetActiveTab( 0  );
		}
		}else
		{
		if ( mTestViControl.GetActiveTabIndex() -1 >=0  )
		{
		mTestViControl.SetActiveTab(mTestViControl.GetActiveTabIndex() - 1 );
		}else{
		mTestViControl.SetActiveTab(mTestViControl.GetTabCount());
		}
		}

		break;
		return NULL;
		}
		*/
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{


			int indexTabUnderMouse = 0;
			indexTabUnderMouse = mTestViControl.GetTabUnderMouse();
			if (indexTabUnderMouse >=0 )
			{
				mTestViControl.SetActiveTab(indexTabUnderMouse);
			}
			//HTREEITEM newItem = mTestViControl.InsertItem("asddad2323",mTestViControl.GetFirstVisibleItem(), mTestViControl.GetFirstVisibleItem() );

			/*HTREEITEM newItem;
			newItem = mTestViControl.InsertItem("asddad", (HTREEITEM)NULL, NULL);
			*/
			/*
			HNTVITEM newItem;
			newItem = mTestViControl.InsertItem("asddad", (HNTVITEM)NULL, TRUE);
			newItem->Flags = NTVI_EDITABLE;
			newItem->Data = NULL;

			*/
			/*
			mTestViControl.UpdateWindow();
			mTestViControl.ShowWindow(1);
			*/

			/*
			mTestViControl.AddItem("asdasd",NULL);
			int a = mTestViControl.GetSubItemCount(0);*/

			/*
			RECT r;
			GetClientRect(&r);

			CDC* pDC=CDC::FromHandle((HDC)wParam);
			pDC->FillSolidRect(&r,RGB(100,200,200));*/

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
						
			fillHullType();
			char temp[64];
			double d;
		} break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPBodyCfg::DoDataExchange(CDataExchange* pDX)
{
	
	//CDialog::DoDataExchange(pDX);
	CParameterDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBodyCfg)

	DDX_Control(pDX, IDC_HULLTYPE, HType);
	DDX_Control(pDX, IDC_LBL_HTYPE, LBL_HType);

	DDX_Control(pDX, IDC_BFLAGS_MOVING,BF_Move);
	DDX_Control(pDX, IDC_BFLAGS_GRAV,BF_Grav);
	DDX_Control(pDX, IDC_BFLAGS_COLL,BF_Collision);
	DDX_Control(pDX, IDC_BFLAGS_COLL_NOTIFY,BF_CollisionNotify);

	DDX_Control(pDX, IDC_BFLAGS_KINEMATIC,BF_Kinematic);
	DDX_Control(pDX, IDC_BFLAGS_TRIGGER,BF_TriggerShape);
	DDX_Control(pDX, IDC_BFLAGS_SLEEP,BF_Sleep);
	DDX_Control(pDX, IDC_BFLAGS_SSHAPE,BF_SubShape);
	DDX_Control(pDX, IDC_BFLAGS_HIERARCHY,BF_Hierarchy);
	DDX_Control(pDX, IDC_BFLAGS_DEFORMABLE,BF_Deformable);
	DDX_Control(pDX, IDC_FLAGS_BG,BF_BG_Rect);
	DDX_Control(pDX, IDC_LBL_FLAGS,LBL_Flags);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CPBodyCfg, CParameterDialog)
	ON_WM_MOUSEMOVE()
	ON_STN_CLICKED(IDC_LBL_FLAGS, OnStnClickedLblFlags)
	ON_STN_CLICKED(IDC_DYNA_FLAGS_RECT, OnStnClickedDynaFlagsRect)
END_MESSAGE_MAP()


void CPBodyCfg::OnStnClickedLblFlags()
{
	// TODO: Add your control notification handler code here
}

void CPBodyCfg::OnStnClickedDynaFlagsRect()
{
	// TODO: Add your control notification handler code here
}
