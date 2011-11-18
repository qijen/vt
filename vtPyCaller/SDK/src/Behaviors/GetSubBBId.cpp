/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            AddNodalLink
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "StdAfx.h"
#include "virtools/vtcxglobal.h"
#include "windows.h"
#include "Python.h"
#include "vt_python_funcs.h"
#include "pyembed.h"
#include "InitMan.h"




CKObjectDeclaration	*FillBehaviorGetNextBBIdDecl();
CKERROR CreateGetNextBBIdProto(CKBehaviorPrototype **);
int GetNextBBId(const CKBehaviorContext& behcontext);
CKERROR GetNextBBIdCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorGetNextBBIdDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("GetNextBBId");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x572066cc,0x58402b59));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetNextBBIdProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Python");
	return od;
}


CKERROR CreateGetNextBBIdProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("GetNextBBId");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	//proto->DeclareOutput("Error");

	proto->DeclareOutParameter("ck_id",CKPGUID_INT);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( GetNextBBId );
	
	*pproto = proto;
	return CK_OK;
	
}


int GetNextBBId(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	
	beh->ActivateInput(0,FALSE);
	
	int count = beh->GetParent()->GetSubBehaviorLinkCount();
	int result = -1;
    
	for (int i=0; i<count; i++)
	{
		CKBehaviorLink *link =  beh->GetParent()->GetSubBehaviorLink(i);
		if (link->GetInBehaviorIO() == beh->GetOutput(0))
		{
			result   = link->GetOutBehaviorIO()->GetOwner()->GetID();
			beh->SetOutputParameterValue(0,&result);
			//threadInfo->targetBeh =  link->GetOutBehaviorIO()->GetOwner();

			/*int targetInputs = threadInfo->targetBeh->GetInputCount();
			if (targetInputs == 1)
			{
				//threadInfo->targetInputToActivate = 0;
			}
			else
			{
				for (int j=0; j<targetInputs; j++)
				{
					if (link->GetOutBehaviorIO() == threadInfo->targetBeh->GetInput(j))
					{
						//threadInfo->targetInputToActivate = j;
						break;
					}
				}
			}*/
			break;
		}
	}
	
	CKBehavior *script   = static_cast<CKBehavior*>(ctx->GetObject(result));
	if (script)
	{
		int bc = script->GetOutputCount();

		int bc2 = script->GetInputCount();

	}


	beh->ActivateOutput(0);
	beh->SetOutputParameterValue(0,&result);
	return CKBR_OK;
}
