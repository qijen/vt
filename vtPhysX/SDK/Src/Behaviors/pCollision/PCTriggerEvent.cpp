#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPCTriggerEventDecl();
CKERROR CreatePCTriggerEventProto(CKBehaviorPrototype **pproto);
int PCTriggerEvent(const CKBehaviorContext& behcontext);
CKERROR PCTriggerEventCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPCTriggerEventDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCTriggerEvent");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Triggers output if body stays,enter or leaves a body .");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7e866b0f,0x5935367c));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePCTriggerEventProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePCTriggerEventProto
// FullName:  CreatePCTriggerEventProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePCTriggerEventProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCTriggerEvent");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PCTriggerEvent  

	PCTriggerEvent is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Triggers outputs if the body enters,stays or leaves another body .<br>
	See <A HREF="pBTriggerEvent.cmo">pBTriggerEvent.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PCTriggerEvent.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	
	<SPAN CLASS="out">No Event: </SPAN>Nothing touched.
	<BR>
	<SPAN CLASS="out">Entering: </SPAN>Body entered.
	<BR>
	<SPAN CLASS="out">Leaving: </SPAN>Body leaved.
	<BR>
	<SPAN CLASS="out">Stay: </SPAN>Inside body .
	<BR>

	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<SPAN CLASS="pout">Touched Object: </SPAN>The touched body.
	<BR>
	
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>
	<h3>Note</h3><br>
	
	
	<br>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pRigidBody::addForce().<br>
	
	*/
	proto->DeclareInput("In");
	proto->DeclareInput("Next");
	proto->DeclareOutput("No Event");
	proto->DeclareOutput("Entering");
	proto->DeclareOutput("Stay");
	proto->DeclareOutput("Leaving");
	
	proto->DeclareOutParameter("Touched Object",CKPGUID_3DENTITY,0);

	//proto->DeclareSetting("Trigger on Enter",CKPGUID_BOOL,"FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PCTriggerEvent);
	*pproto = proto;
	return CK_OK;
}
enum bOutputs
{
	bbO_None,
	bbO_Enter,
	bbO_Stay,
	bbO_Leave,
};

//************************************
// Method:    PCTriggerEvent
// FullName:  PCTriggerEvent
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PCTriggerEvent(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	using namespace vtTools::BehaviorTools;
	
	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) return CKBR_OWNERERROR;

	//world->getScene()->setFilterOps()
	// body exists already  ?  clean and delete it : 
	pRigidBody*result = GetPMan()->getBody(target);
	if (!result)
	{
		beh->ActivateOutput(bbO_None);
		return 0;
	}

	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);


		if (GetPMan()->getTriggers().Size()==0)
		{
			beh->ActivateOutput(bbO_None);
			return 0;
		}

		if (GetPMan()->getTriggers().Size())
		{


			int nbEntries = GetPMan()->getTriggers().Size() ; 
			for (int i  = 0 ; i < GetPMan()->getTriggers().Size(); i++ )
			{
				pTriggerEntry &entry  = *GetPMan()->getTriggers().At(i);

				if (!entry.triggered)
				{
					
					if(entry.triggerEvent == NX_TRIGGER_ON_ENTER)
					{
						beh->ActivateOutput(bbO_Enter);
					}
					if(entry.triggerEvent == NX_TRIGGER_ON_STAY)
					{
						beh->ActivateOutput(bbO_Stay);
					}
					if(entry.triggerEvent == NX_TRIGGER_ON_LEAVE)
					{
						beh->ActivateOutput(bbO_Leave);
					}

					nbEntries--;
					entry.triggered = true;


					NxActor *triggerActor = &entry.shapeA->getActor();
					NxActor *otherActor = &entry.shapeB->getActor();

					pRigidBody *triggerBody  = NULL;
					pRigidBody *otherBody  = NULL;

					if (triggerActor)
					{
						triggerBody  = static_cast<pRigidBody*>(triggerActor->userData);
					}

					if (otherActor)
					{
						otherBody  = static_cast<pRigidBody*>(otherActor->userData);
					}

					if (triggerBody &&  result == triggerBody)
					{
						CK_ID id = otherBody->getEntID();
						CKObject *entOut= (CK3dEntity*)ctx->GetObject(id);
						beh->SetOutputParameterObject(0,entOut);
					}

					if (otherBody &&  result == otherBody )
					{
						CK_ID id = triggerBody->getEntID();
						CKObject *entOut= (CK3dEntity*)ctx->GetObject(id);
						beh->SetOutputParameterObject(0,entOut);
					}

				}
			}
		}else{
			beh->SetOutputParameterObject(0,NULL);
		}
	}
	return 0;
}

//************************************
// Method:    PCTriggerEventCB
// FullName:  PCTriggerEventCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PCTriggerEventCB(const CKBehaviorContext& behcontext)
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

