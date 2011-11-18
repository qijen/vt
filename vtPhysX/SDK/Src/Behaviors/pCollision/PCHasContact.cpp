#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorCollisionsCheckADecl();
CKERROR CreateCollisionsCheckAProto(CKBehaviorPrototype **pproto);
int CollisionsCheckA(const CKBehaviorContext& behcontext);
CKERROR CollisionsCheckACB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorCollisionsCheckADecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCHasContact");	
	od->SetDescription("Checks for a collision on a given body");
	od->SetCategory("Physic/Collision");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5189255c,0x18134074));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateCollisionsCheckAProto);
	//od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	return od;
}

CKERROR CreateCollisionsCheckAProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCHasContact");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->SetBehaviorCallbackFct( CollisionsCheckACB );
	proto->DeclareInput("Check");
	
	proto->DeclareOutput("Collision");
	proto->DeclareOutput("No Collision");

	proto->DeclareOutParameter("Collider", CKPGUID_3DENTITY);
	proto->DeclareOutParameter("Position", CKPGUID_VECTOR);
	proto->DeclareOutParameter("Normal", CKPGUID_VECTOR);
	proto->DeclareOutParameter("Face Index", CKPGUID_INT);
	proto->DeclareOutParameter("Normal Force", CKPGUID_VECTOR);
	proto->DeclareOutParameter("Friction Force", CKPGUID_VECTOR);

	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(CollisionsCheckA);

	*pproto = proto;
	return CK_OK;
}


int CollisionsCheckA(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	PhysicManager *pm = GetPMan();

	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;


	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0, FALSE);

		//////////////////////////////////////////////////////////////////////////
		//we haven't't seen yet, create a local array  : 
			
		//the object A: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_PARAMETERERROR;

		pRigidBody *body = GetPMan()->getBody(target);
		if (body)
		{
			VxVector pos;
			VxVector normal;
			float depth;
			int size = body->getCollisions().Size();

			CK3dEntity *colliderEntity = NULL;

			if (!size)
			{
				beh->ActivateOutput(1);
				beh->SetOutputParameterObject(0,NULL);
				return 0;
				
			}
			if (size)
			{

				

				for (int i  = 0; i < body->getCollisions().Size() ; i ++)
				{
					pCollisionsEntry *entry  = body->getCollisions().At(i);
					if (entry)
					{
						CK3dEntity *shapeA = (CK3dEntity*)GetPMan()->GetContext()->GetObject(entry->shapeEntityA);
						CK3dEntity *shapeB = (CK3dEntity*)GetPMan()->GetContext()->GetObject(entry->shapeEntityB);
						pRigidBody *collider = NULL;
						body->getCollisions().RemoveAt(i);
						if( target == body->GetVT3DObject())
						{
							if (entry->bodyA && entry->bodyA == body)
							{
								collider = entry->bodyB;
								colliderEntity  = shapeB;
							}
							if (entry->bodyB && entry->bodyB == body)
							{
								collider = entry->bodyA;
								colliderEntity = shapeA;
							}
						}else // its a sub shape collision
						{
							if (shapeA && shapeA == target )
							{
								colliderEntity = shapeB;
							}
							if (shapeB && shapeB == target)
							{
								colliderEntity = shapeA;

							}
						}
						if (colliderEntity)
						{
							beh->SetOutputParameterObject(0,colliderEntity);
							beh->ActivateOutput(0);
							SetOutputParameterValue<VxVector>(beh,1,entry->point);
							SetOutputParameterValue<VxVector>(beh,2,entry->faceNormal);
							SetOutputParameterValue<int>(beh,3,entry->faceIndex);
							SetOutputParameterValue<VxVector>(beh,4,entry->sumNormalForce);
							SetOutputParameterValue<VxVector>(beh,5,entry->sumFrictionForce);
							body->getCollisions().Clear();
							return 0;
						}
					}
				}
			}
/*			int size = world->getCollisions().Size();
			for (int i  = 0; i < world->getCollisions().Size() ; i ++)
			{
				pCollisionsEntry *entry  = world->getCollisions().At(i);
				if (entry)
				{
					printf("asdasd");
					int o = 2;
					o++;
				}

			}
*/
			//CK3dEntity *collider  = world->CIsInCollision(target,pos,normal,depth);
			/*
			if (collider)
			{
				beh->SetOutputParameterObject(0,collider);
				beh->SetOutputParameterValue(1,&pos);
				beh->SetOutputParameterValue(2,&normal);
				beh->SetOutputParameterValue(3,&depth);
				beh->ActivateOutput(0);
				return 0;
			}else
			{

				beh->ActivateOutput(1);
			}*/
		}
	}
	beh->ActivateOutput(1);
	return 0;
}

CKERROR CollisionsCheckACB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {

	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			
		}
		break;
	}
	return CKBR_OK;
}
