#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "IParameter.h"


#define  BB_OBJECT_COPY CKGUID(0x3f6b0ac7,0x47d20f78)
#define  BB_OBJECT_DELETE CKGUID(0x74120ded,0x76524673)

enum BB_COPY_SETTINGS
{

	bbS_Dynamic,
	bbS_Awak,
	bbs_Flags,
	bbs_NewBodyFlags,

};

CKBEHAVIORFCT BBCopy;
int BB_CopyNew(const CKBehaviorContext& context)
{


	CKBehavior	*beh = context.Behavior;
	CKContext	*ctx = context.Context;

	BBCopy(context);

	// Copy Objects
	int count = beh->GetInputParameterCount();

	// Dynamic ?
	BOOL dynamic = TRUE;
	beh->GetLocalParameterValue(0,&dynamic);

	int flags = 0;
	beh->GetLocalParameterValue(bbs_Flags,&flags);

	int newBodyFlags = 0;
	beh->GetLocalParameterValue(bbs_NewBodyFlags,&newBodyFlags);

	CKDependencies* dep = *(CKDependencies**)beh->GetInputParameterReadDataPtr(0);
	
	
	XArray<CK3dEntity*>SrcObjects;
	
	int i;
	for(i=1;i<count;++i) 
	{
		// Input Reading
		CKObject* srcObject = beh->GetInputParameterObject(i);
		if (!srcObject) continue;

		// we do not want to to duplicate level
		if (CKIsChildClassOf(srcObject,CKCID_LEVEL)) continue;
		
		if (CKIsChildClassOf(srcObject,CKCID_3DENTITY))
		{
			

			CK3dEntity *src=(CK3dEntity*)srcObject;
			CK3dEntity *dst=(CK3dEntity*)(beh->GetOutputParameterObject(i-1));

			pObjectDescr oDescr;
			IParameter::Instance()->copyTo(oDescr,src,flags);


			//----------------------------------------------------------------
			//
			// IC Data, we just save it out
			//
			if ( flags & PB_CF_COPY_IC )
			{
				CKScene *scene = ctx()->GetCurrentScene();
				if(scene && src->IsInScene(scene)) 
				{
					CKStateChunk *chunk = scene->GetObjectInitialValue(src);
					if(chunk) 
					{
						
						CKStateChunk *chunkClone=CreateCKStateChunk(CKCID_OBJECT);
						chunkClone->Clone(chunk);
						
						//----------------------------------------------------------------
						//
						// copy and restore IC of parent object
						//
						if(chunkClone)
						{
							scene->SetObjectInitialValue(dst,chunkClone);
						}

						CKERROR error = 0;
						if ( flags & PB_CF_RESTORE_IC )
						{
							error = CKReadObjectState(dst,chunkClone);
						}

						//----------------------------------------------------------------
						//
						// copy and restore IC for children :
						//
						int a = ((*dep->At(CKCID_3DENTITY)) & CK_DEPENDENCIES_COPY_3DENTITY_CHILDREN);
						int a1 = (( flags & PB_CF_OVRRIDE_BODY_FLAGS ) && (newBodyFlags & BF_Hierarchy));
						int a2 = ( oDescr.flags & BF_Hierarchy);

						if ( ((*dep->At(CKCID_3DENTITY)) & CK_DEPENDENCIES_COPY_3DENTITY_CHILDREN)  && 
							( (( flags & PB_CF_OVRRIDE_BODY_FLAGS ) && (newBodyFlags & BF_Hierarchy)) ||
							( oDescr.flags & BF_Hierarchy) )
							)
						{
							CK3dEntity* subEntity = NULL;
							SrcObjects.Clear();

							while (subEntity= dst->HierarchyParser(subEntity) )
							{
								SrcObjects.PushBack(subEntity);
							}
							int dCount = dst->GetChildrenCount();
							int dCount2 = SrcObjects.Size();

							for(i = 0;i<SrcObjects.Size(); ++i) 
							{
								subEntity = *SrcObjects.At(i);
								CK3dEntity *orginalObject = findSimilarInSourceObject(src,dst,subEntity);
								if (orginalObject)
								{
									CKStateChunk *chunkSub = scene->GetObjectInitialValue(orginalObject);
									if (chunkSub)
									{
										CKStateChunk *chunkCloneSub=CreateCKStateChunk(CKCID_OBJECT);
										chunkCloneSub->Clone(chunkSub);
										scene->SetObjectInitialValue(subEntity,chunkCloneSub);
										if ( flags & PB_CF_RESTORE_IC )
										{
											CKReadObjectState(subEntity,chunkCloneSub);
											subEntity->SetParent(dst);
										}
									}
								}
								
							}
						}
					}
				}
			}
			pRigidBody *srcBody  = GetPMan()->getBody((CK3dEntity*)src);
			NxShape *shape = NULL;
			if (srcBody)
			{
				shape  = srcBody->getSubShape((CK3dEntity*)src);
			}
            //----------------------------------------------------------------
            //
            // copy sub shape ? 
            //
			if (shape && shape!=srcBody->getMainShape())
			{
				pFactory::Instance()->cloneShape((CK3dEntity*)src,(CK3dEntity*)(beh->GetOutputParameterObject(i-1)),srcBody->GetVT3DObject(),flags,newBodyFlags);
				continue;
			}
	
			//----------------------------------------------------------------
			//
			// copy body ?
			//
			if (srcBody)
				pFactory::Instance()->cloneRigidBody(src,dst,dep,flags,newBodyFlags);
		}
	}
	return CK_OK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CKERROR
PhysicManager::_HookGenericBBs()
{
	CKBehaviorManager *bm = m_Context->GetBehaviorManager();
	
	CKBehaviorPrototype *bproto = CKGetPrototypeFromGuid(BB_OBJECT_COPY);
	if(bproto)
	{
		bproto->DeclareSetting("Copy Flags",VTF_PHYSIC_ACTOR_COPY_FLAGS,"");
		bproto->DeclareSetting("New Body Flags",VTF_BODY_FLAGS,"");

		BBCopy = bproto->GetFunction();
		bproto->SetFunction(BB_CopyNew);
	}

	return CK_OK;
}
