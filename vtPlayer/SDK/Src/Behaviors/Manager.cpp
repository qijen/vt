#include "CKAll.h"
#include "Manager.h"
#include "vtTools.h"
static CSManager* _im2=NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSManager::CSManager(CKContext* context):CKBaseManager(context,INIT_MAN_GUID,"csharp manager")//Name as used in profiler
{
	m_Context->RegisterNewManager(this);
	_im2 = this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSManager::~CSManager(){}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR CSManager::PreProcess()
{	
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR CSManager::OnCKReset(){	

	GetCSMessages().clear();

	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR CSManager::OnCKInit(){		m_Context->ActivateManager((CKBaseManager*) this,true);		return CK_OK; }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSManager::AddMessage(csMessage*msg)
{
	GetCSMessages().push_back(msg);
	SetHasMessages(1);
}

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////