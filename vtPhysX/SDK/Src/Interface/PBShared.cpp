#include "StdAfx2.h"
#include "PCommonDialog.h"
#include "PBodySetup.h"
#include "PBodyTabCtrl.h"
#include "PBXMLSetup.h"
#include "resource.h"
#include "VITabCtrl.h"


#include "resource.h"
#include "VITabCtrl.h"
#include "VIControl.h"
#include "..\..\include\interface\pcommondialog.h"



/************************************************************************/
/* Global vars                                                                     */
/************************************************************************/

static CPSharedBase *gXML=NULL;
static CPSharedBase *gCOMMON=NULL;
static CPSharedBase *gCOLLISION=NULL;
static CPSharedBase *gOPTIMIZATION=NULL;


static CPSharedBase* gSharedBase=NULL;

static CPSharedBase *gDialogs[DLGCOUNT] = 
{
	gXML,
	gCOMMON,
};

CPBParentDialog::CPBParentDialog(CKParameter* Parameter,CWnd*parent,CK_CLASSID Cid)	: 
	CParameterDialog(Parameter,Cid)  , CPSharedBase(Parameter,this,Cid) , m_TabControl(parent)
{	


	if (_cleanUpIfNew(Parameter))
	{
		_emptyTabs();
	}

	if (gSharedBase)
		gSharedBase=NULL;

	gSharedBase =this;
	setRootDialog(this);
	setRootParameter(Parameter);
	getInstance();
	//loadTabPane();
}
CPBParentDialog::CPBParentDialog(CKParameter* Parameter,CK_CLASSID Cid)	: 
	CParameterDialog(Parameter,Cid)  , CPSharedBase(Parameter,this,Cid) , m_TabControl(NULL)
{	

		if (_cleanUpIfNew(Parameter))
		{
			_emptyTabs();
		}

		gSharedBase = this;		
		setRootDialog(this);
		setRootParameter(Parameter);
		//loadTabPane();


}


void CPBParentDialog::_destroy()
{
	if (getTabs().GetTabCount())
	{
		_emptyTabs();
	}

	gSharedBase=NULL;
}

/*

CPBParentDialog::CPBParentDialog(CWnd* pParent = NULL)	: 
CParameterDialog(Parameter,Cid)  , CPSharedBase(Parameter,this,Cid)
{	

	if (_cleanUpIfNew(Parameter))
	{
		_emptyTabs();
	}

	gSharedBase = this;		
	setRootDialog(this);
	setRootParameter(Parameter);

}*/



/************************************************************************/
/*                                                                      */
/************************************************************************/







/************************************************************************/
/*                                                                      */
/************************************************************************/

BOOL CPSharedBase::initChildWin(CDialog* pDlg, UINT dialogID,UINT dialogPlaceHolderID)
{

	ASSERT( pDlg );

	if (!pDlg)
	{
		return false;
	}


	// wind handle of the target bed :
	HWND dstWindow = NULL;
	CPBParentDialog* parent  = (CPBParentDialog*)getInstance()->getRootDlg();
	if (parent)
	{
		dstWindow = parent->getDlgWindowHandle(dialogPlaceHolderID);
	
	}else
		return false;



	CWnd *cDstWindow = CWnd::FromHandle(dstWindow);
	if (cDstWindow)
	{
		int p = getNbDialogs();
	}else
	{
		return false;
	}



	//CWnd* pWnd =   pDlg->GetDlgItem( dialogPlaceHolderID );
	CRect rcValue;

	if (pDlg) 
	{
		cDstWindow->GetWindowRect( &rcValue ); // Use picture box position.
		parent->ScreenToClient( &rcValue );

		if (pDlg->Create( dialogID, cDstWindow ))
		{
			pDlg->SetWindowPos( cDstWindow, rcValue.left, rcValue.top,rcValue.Width(), rcValue.Height(), SWP_HIDEWINDOW);
			return true;
		}
		//pWnd->ShowWindow(SWP_SHOWWINDOW);
	}

	return false;
}



void CPSharedBase::_dtrBodyCommonDlg(){}



CPSharedBase* CPSharedBase::getDialog(int identifier)
{
	ASSERT( identifier >= 0 && identifier < DLGCOUNT  );
	if (gDialogs[identifier])
		return gDialogs[identifier];

	return NULL;
}

void CPSharedBase::setDialog(int identifier,CPSharedBase*src)
{
	ASSERT( identifier >= 0 && identifier < DLGCOUNT  );
	//if (gDialogs[identifier] != src )
	gDialogs[identifier] = src;
}
void CPSharedBase::destroyDialog(int identifier,CPSharedBase*src)
{
	ASSERT( identifier >= 0 && identifier < DLGCOUNT  );
	if (src)
	{
		
	}
	src = gDialogs[identifier] = NULL;

}
CParameterDialog	*CPSharedBase::getRootDlg()
{	
	return getInstance() ?  getInstance()->dlgRoot : NULL;	
}
CKParameter			*CPSharedBase::getRootParameter()
{
	return getInstance() ?  getInstance()->parameter : NULL;	
}
void	CPSharedBase::setRootParameter(CKParameter * val) 
{
	if (getInstance())
		getInstance()->parameter = val;	
}
void	CPSharedBase::setRootDialog(CParameterDialog * val)
{		
	if (getInstance())		
		getInstance()->dlgRoot = val;	
}
ParDialogArrayType&	CPSharedBase::getDialogs()
{
	return getInstance() ? getInstance()->mDialogs : ParDialogArrayType() ; 
}


CPSharedBase::~CPSharedBase()
{
	_reset();
}
void CPSharedBase::_destroy()
{	

}
void CPSharedBase::_reset()
{
	parameter = NULL;
	dlgRoot = NULL;
	mDialogs.Clear();
	mTab = NULL;
	
}

void CPSharedBase::_construct()
{

}

CPSharedBase::CPSharedBase(
						   CKParameter* Parameter,
						   CParameterDialog *rootDialog,
						   CK_CLASSID Cid)	:
								dlgRoot(rootDialog) , parameter(Parameter)
{		
	mDialogs.Clear();
	mTab = NULL;
}

CPSharedBase::CPSharedBase(CKParameter* Parameter,CK_CLASSID Cid)
{	
	dlgRoot=NULL;
	mDialogs.Clear();
	mTab = NULL;
	

}
CPSharedBase*CPSharedBase::getInstance()
{	
	return gSharedBase;
}
CPSharedBase::CPSharedBase(CParameterDialog *consumer,CKParameter* parameter)
{		
	addDialog(consumer,parameter);	
}
void CPSharedBase::addDialog(CParameterDialog *dlg,CKParameter*parameter)
{		
	/*if(getInstance() &&  (*getDialogs().FindPtr(parameter)))		
		getDialogs().InsertUnique(parameter,dlg);	*/
}
CPSharedBase**	CPSharedBase::getStaticDialogs()
{
	return gDialogs;	
}
int CPSharedBase::getNbDialogs()
{	
	int result = 0;
	for ( int i  = 0 ;  i < DLGCOUNT ; i++ )
	{
		if (getStaticDialogs()[i] != NULL )
		{
			result++;
		}
	}
	return result;

}

/*
LRESULT CPBParentDialog::OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam)
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
*/
/*
void CPBParentDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	//int cStates = GET_KEYSTATE_WPARAM(wParam);
	int wheelDirection = zDelta;

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
}
*/


void CPBParentDialog::InitAllControls()
{
	CRect rect;
	EnableWindow(true);


	PBodyTabContrl*  viCtrl = getTabControl();
	if (!viCtrl)return;



	GetClientRect(&rect);
	HRESULT res = ((VITabCtrl*)getTabControl())->Create("NoName",VITabCtrl::VITAB_UP|VITabCtrl::VITAB_DOWN,rect,((CWnd*)(getTabControl())),IDC_PBODY_TAB_PANE);
	getTabControl()->GetClientRect(&rect);

	getTabControl()->_construct();

	getTabControl()->GetClientRect(&rect);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	// wind handle of the target bed :
	HWND dstWindow = NULL;
	CWnd *cDstWindow = NULL;
	CPBParentDialog* parent  = (CPBParentDialog*)getInstance()->getRootDlg();




	if (parent)
	{
		dstWindow = parent->getDlgWindowHandle(IDC_SPACER);
		if (dstWindow)
		{
			cDstWindow= CWnd::FromHandle(dstWindow);
		
			CRect rcValue;
			cDstWindow->GetWindowRect( &rcValue ); // Use picture box position.
			parent->ScreenToClient( &rcValue );

			CWnd * target = parent->GetDlgItem(IDC_PBODY_TAB_PANE);
			if (target)
			{

				//target->SetWindowPos( cDstWindow, rcValue.left, rcValue.top,rcValue.Width(), rcValue.Height(), SWP_SHOWWINDOW);
				//pWnd->ShowWindow(SWP_SHOWWINDOW);
			}
		}
	}


	




	/************************************************************************/
	/*                                                                      */
	/************************************************************************/




	//CPBParentDialog* parent  = (CPBParentDialog*)getInstance()->getRootDlg();
			

			
			int err = getTabControl()->InsertTab(-1,"XML",parent,true);
			err = getTabControl()->InsertTab(-1,"XMLasas",parent,true );
			
		
			VITabCtrl::VITab *bTab= new VITabCtrl::VITab();
			bTab->m_Flags=VITabCtrl::VITAB_BORDER;
			bTab->m_Name = "XML";
			bTab->m_Width = 30;
			bTab->m_Wnd =  NULL;


			err = getTabControl()->GetTabCount();
			err = getTabControl()->InsertTab(-1,bTab,true);

			

		
			
	//		parent->ScreenToClient( &rect );
	//		getTabControl()->ScreenToClient(&rect);
			//*/


}

int CPBParentDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 1;
}
