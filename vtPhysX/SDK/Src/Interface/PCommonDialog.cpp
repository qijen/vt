#include "PCommonDialog.h"
#include "resource.h"
#include "VITabCtrl.h"

#include "VIControl.h"


#define LAYOUT_STYLE			(WS_CHILD|WS_VISIBLE)
#define LAYOUT_ShaderREE		130

RECT	_GetNullRect() {
	return RECT(); 
}


LRESULT CPBCommonDialog::OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	return lResult;

}

void CPBCommonDialog::fillFlags()
{

	BF_Move.SetCheck(true);
}


void CPBCommonDialog::fillHullType()
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


namespace vtAgeia
{

}


CKBOOL CPBCommonDialog::On_Init()
{	

	fillHullType();


	return true;


}

LRESULT CPBCommonDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{


	case WM_MOUSEWHEEL:
		{

			
			break;

		}
	case WM_ERASEBKGND:
		{
			/*RECT r;
			GetClientRect(&r);
			CDC* pDC=CDC::FromHandle((HDC)wParam);
			pDC->FillSolidRect(&r,RGB(100,8,100));
			*/

		}break;
	case CKWM_OK:
		{

		} break;

	case CKWM_INIT:
		{
			fillHullType();
		} break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}


void CPBCommonDialog::DoDataExchange(CDataExchange* pDX)
{

	//CDialog::DoDataExchange(pDX);

	CParameterDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPBCommonDialog)
	//DDX_Control(pDX, IDC_DYNA_FLAGS_RECT,mDynaFlagsRect);
	DDX_Control(pDX, IDC_HULLTYPE, HType);
	DDX_Control(pDX, IDC_LBL_HTYPE, LBL_HType);


	DDX_Control(pDX, IDC_BFLAGS_MOVING,BF_Move);
	DDX_Control(pDX, IDC_BFLAGS_GRAV,BF_Grav);
	DDX_Control(pDX, IDC_BFLAGS_HIERARCHY,BF_Hierarchy);
	
	DDX_Control(pDX, IDC_BFLAGS_COLL,BF_Collision);
	DDX_Control(pDX, IDC_BFLAGS_COLL_NOTIFY,BF_CollisionNotify);

	DDX_Control(pDX, IDC_BFLAGS_KINEMATIC,BF_Kinematic);
	DDX_Control(pDX, IDC_BFLAGS_TRIGGER,BF_TriggerShape);
	DDX_Control(pDX, IDC_BFLAGS_SLEEP,BF_Sleep);
	DDX_Control(pDX, IDC_BFLAGS_SSHAPE,BF_SubShape);
	
	DDX_Control(pDX, IDC_BFLAGS_DEFORMABLE,BF_Deformable);
	DDX_Control(pDX, IDC_LBL_LFLAGS,LBL_Flags);
	DDX_Control(pDX, IDC_LBL_FLAGS,LBL_Flags);
	DDX_Control(pDX, IDC_LBL_DYNAFLAGS,LBL_DFlags);


	DDX_Control(pDX, IDC_TLFLAGS_POS,TF_POS);
	DDX_Control(pDX, IDC_TLFLAGS_ROT,TF_ROT);
	
	DDX_Control(pDX, IDC_TLFLAGS_POS_X,TF_PX);
	DDX_Control(pDX, IDC_TLFLAGS_POS_Y,TF_PY);
	DDX_Control(pDX, IDC_TLFLAGS_POS_Z,TF_PZ);


	DDX_Control(pDX, IDC_TLFLAGS_ROT_X,TF_RX);
	DDX_Control(pDX, IDC_TLFLAGS_ROT_Y,TF_RY);
	DDX_Control(pDX, IDC_TLFLAGS_ROT_Z,TF_RZ);

	//}}AFX_DATA_MAP

}
// DoubleParamDialog dialog

BEGIN_MESSAGE_MAP(CPBCommonDialog, CParameterDialog)
	ON_STN_CLICKED(IDC_DYNA_FLAGS_RECT, OnStnClickedDynaFlagsRect)
END_MESSAGE_MAP()


#include "xEnumerations.h" 
#include "..\..\include\interface\pcommondialog.h"


/*IMPLEMENT_DYNAMIC(CPBCommonDialog, CDialog)
CPBCommonDialog::CPBCommonDialog(CKParameter *param, CWnd* pParent)
: CDialog(CPBCommonDialog::IDD, pParent)
{
	parameter = param;
}*/




BOOL CPBCommonDialog::OnInitDialog()
{
	CParameterDialog::OnInitDialog();
	m_tt = new CToolTipCtrl();
	m_tt->Create(this);
	// Add tool tips to the controls, either by hard coded string 
	// or using the string table resource
	m_tt->AddTool( &BF_Move, _T("This is a tool tip!"));
	/*m_tt->AddTool( &mDynaFlagsRect, _T("This is a tool tip!"));
	//m_ToolTip.AddTool( &m_myEdit, IDS_MY_EDIT);
	m_tt->Activate(TRUE);
*/
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPBCommonDialog::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) 
		m_tt->RelayEvent(pMsg); 


	// TODO: Add your specialized code here and/or call the base class

	return CParameterDialog::PreTranslateMessage(pMsg);
}

void CPBCommonDialog::OnStnClickedDynaFlagsRect()
{
	
	m_tt->Activate(TRUE);

	LPRECT rect;
	m_tt->GetWindowRect(rect);
	m_tt->GetWindowRect(rect);


}
