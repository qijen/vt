//////////////////////////////////////////////////////////////////////////////////////////////////////////
//		            DataManager
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CKAll.h"
#include "DataManager.h"

DataManager::DataManager(CKContext *Context) : 
CKBaseManager(Context, DataManagerGUID, "DataManager")
{
	Context->RegisterNewManager(this);
	_vPos.Set(0,0,0);
}

DataManager::~DataManager()
{

}


CKERROR DataManager::OnCKInit()
{

#ifdef G_EXTERNAL_ACCESS
	_initSharedMemory(0);
#endif
	return CK_OK;
}

CKERROR DataManager::PostProcess()
{

	#ifdef G_EXTERNAL_ACCESS
		_SharedMemoryTickPost(0);
	#endif
	return CK_OK;
}

CKERROR DataManager::PreProcess()
{

	#ifdef G_EXTERNAL_ACCESS
		_SharedMemoryTick(0);
	#endif
	return CK_OK;
}






