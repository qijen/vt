// PBDodyTab.cpp : implementation file
//

#include "stdafx.h"
#include "PBDodyTab.h"


// PBDodyTab

IMPLEMENT_DYNAMIC(PBDodyTab, CTabCtrl)

PBDodyTab::PBDodyTab()
{

	EnableAutomation();
}

PBDodyTab::~PBDodyTab()
{
}

void PBDodyTab::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CTabCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(PBDodyTab, CTabCtrl)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(PBDodyTab, CTabCtrl)
END_DISPATCH_MAP()

// Note: we add support for IID_IPBDodyTab to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {35C830F1-59A2-40B2-971F-2BEEC5930188}
static const IID IID_IPBDodyTab =
{ 0x35C830F1, 0x59A2, 0x40B2, { 0x97, 0x1F, 0x2B, 0xEE, 0xC5, 0x93, 0x1, 0x88 } };

BEGIN_INTERFACE_MAP(PBDodyTab, CTabCtrl)
	INTERFACE_PART(PBDodyTab, IID_IPBDodyTab, Dispatch)
END_INTERFACE_MAP()


// PBDodyTab message handlers


