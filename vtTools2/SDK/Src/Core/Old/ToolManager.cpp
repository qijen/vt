#include <StdAfx.h>

#include "ToolManager.h"

#include "vtCModuleDefines.h"
#include "vtBaseMacros.h"


ToolManager *manager = NULL;


//////////////////////////////////////////////////////////////////////////
ToolManager::ToolManager(CKContext* ctx)	:	vtBaseManager(ctx,VTM_TOOL_MANAGER_GUID,"ToolManager")
{
	m_Context->RegisterNewManager(this);
	manager = this;
}
//////////////////////////////////////////////////////////////////////////
CKERROR ToolManager::OnCKPause()
{
	return CK_OK;
}
CKERROR ToolManager::OnCKPlay()
{
	return CK_OK;
}
CKERROR ToolManager::PostClearAll()
{
	return CK_OK;
}
CKERROR ToolManager::OnCKInit()
{
	return CK_OK;
}
CKERROR ToolManager::PreSave()
{
	return CK_OK;
}
CKContext* ToolManager::GetContext()
{
	return GetInstance()->m_Context;
}
ToolManager* ToolManager::GetInstance()
{
	if (manager)
	{
		return manager;
	}
	
	return NULL;
}
ToolManager::~ToolManager(){}

CKERROR ToolManager::SequenceDeleted(CK_ID *objids,int count)
{
	return CK_OK;
}

CKERROR ToolManager::SequenceAddedToScene(CKScene *scn,CK_ID *objids,int count)
{
	return CK_OK;
}
CKERROR ToolManager::SequenceToBeDeleted(CK_ID *objids,int count)
{
	return CK_OK;
}

CKERROR ToolManager::SequenceRemovedFromScene(CKScene *scn,CK_ID *objids,int count)
{
	return CK_OK;
}


CKERROR ToolManager::OnCKReset()
{
	return CK_OK;
}
CKERROR ToolManager::OnCKEnd()
{
	return 0;
}

CKStateChunk*
ToolManager::SaveData(CKFile* SavedFile)
{
	return CK_OK;
}
CKERROR ToolManager::PostProcess()
{
	return CK_OK;
}
CKERROR ToolManager::PreProcess()
{
	return CK_OK;
}
CKERROR ToolManager::LoadData(CKStateChunk *chunk,CKFile* LoadedFile)
{
	return CK_OK;
}
CKERROR ToolManager::PostSave()
{
	return CK_OK;
}




