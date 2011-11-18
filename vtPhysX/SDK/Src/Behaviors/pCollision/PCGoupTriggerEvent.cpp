#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPCGroupTriggerEventDecl();
CKERROR CreatePCGroupTriggerEventProto(CKBehaviorPrototype **pproto);
int PCGroupTriggerEvent(const CKBehaviorContext& behcontext);
CKERROR PCGroupTriggerEventCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPCGroupTriggerEventDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCGroupTriggerEvent");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Triggers output if elements of a group stays within, enters or leaves a body .");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xd8c142a,0x4ce04f7b));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePCGroupTriggerEventProto);
	od->SetCompatibleClassId(CKCID_GROUP);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePCGroupTriggerEventProto
// FullName:  CreatePCGroupTriggerEventProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePCGroupTriggerEventProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCGroupTriggerEvent");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PCGroupTriggerEvent  

	PCGroupTriggerEvent is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Triggers outputs if the body enters,stays or leaves another body .<br>
	See <A HREF="pBTriggerEvent.cmo">pBTriggerEvent.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PCGroupTriggerEvent.png

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

	<SPAN CLASS="pin">Target Group: </SPAN>The group which the bb outputs triggers for.
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

	//proto->DeclareInParameter("Extra Filter",VTF_TRIGGER,"0");
	//proto->DeclareInParameter("Remove Event",CKPGUID_BOOL,"0");

	proto->DeclareInput("In");
	proto->DeclareInput("Next");
	
	proto->DeclareOutput("No Event");
	proto->DeclareOutput("Entering");
	proto->DeclareOutput("Stay");
	proto->DeclareOutput("Leaving");
	proto->DeclareOutput("Has More");

	
	proto->DeclareOutParameter("Trigger Object",CKPGUID_3DENTITY,0);
	proto->DeclareOutParameter("Touched Object",CKPGUID_3DENTITY,0);
	proto->DeclareOutParameter("Trigger Event",VTF_TRIGGER,0);

	proto->DeclareLocalParameter("currentIndex", CKPGUID_INT);


	//proto->DeclareSetting("Trigger on Enter",CKPGUID_BOOL,"FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PCGroupTriggerEvent);
	*pproto = proto;
	return CK_OK;
}
enum bOutputs
{
	bbO_None,
	bbO_Enter,
	bbO_Stay,
	bbO_Leave,
	bbO_HasMore,
};

enum bInputs
{
	bbI_Init,
	bbI_Next,
};

bool isInGroup(CKGroup *src, CK3dEntity* testObject)
{

	if (src && testObject)
	{
		for (int i = 0  ; i < src->GetObjectCount() ; i++ )
		{
			CK3dEntity *ent = (CK3dEntity*)src->GetObject(i);
			if(ent)
			{
				if (ent==testObject)
				{
					return true;
				}
			}
		}
	}

	return false;
}

int getEventCount()
{

	int result= 0;
	int nbEntries = GetPMan()->getTriggers().Size() ; 
	
	for (int i  = 0 ; i < GetPMan()->getTriggers().Size(); i++ )
	{
		pTriggerEntry &entry  = *GetPMan()->getTriggers().At(i);
		if(!entry.triggered)
			result++;
	}

	return result;

}


//************************************
// Method:    PCGroupTriggerEvent
// FullName:  PCGroupTriggerEvent
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PCGroupTriggerEvent(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	

	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CKGroup *target = (CKGroup *) beh->GetTarget();
	if( !target ) return CKBR_OWNERERROR;

	//int extraFilter = GetInputParameterValue<int>(beh,0);
	//int remove = GetInputParameterValue<int>(beh,1);

	int nbOfEvents = getEventCount();
	if (!nbOfEvents)
	{
		beh->ActivateOutput(bbO_None);
		return 0;
	}

	/************************************************************************/
	/* handle init                                                                     */
	/************************************************************************/
	if( beh->IsInputActive(bbI_Init) )
	{
		beh->ActivateInput(bbI_Init,FALSE);

		int index = 0;beh->SetLocalParameterValue(0,&index);
		
		//we have some, forward to in 1:next
		if (nbOfEvents)
		{
			beh->ActivateInput(bbI_Next,TRUE);
			beh->ActivateOutput(bbO_HasMore);
		}
	}

	/************************************************************************/
	/* handle trigger 'next'                                                                     */
	/************************************************************************/
	if( beh->IsInputActive(bbI_Next) )
	{
		beh->ActivateInput(bbI_Next,FALSE);

		int index = 0;beh->GetLocalParameterValue(0,&index);
		for (int i  = index ; i < GetPMan()->getTriggers().Size(); i++ )
		{
			pTriggerEntry &entry  = *GetPMan()->getTriggers().At(i);
			if (!entry.triggered)
			{
				if (isInGroup(target,entry.triggerShapeEnt))
				{
					beh->SetOutputParameterObject(0,entry.triggerShapeEnt);
					beh->SetOutputParameterObject(1,entry.otherObject);

					SetOutputParameterValue<int>(beh,2,entry.triggerEvent);
					entry.triggered = true;

					//trigger out 
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

					//store index
					beh->SetLocalParameterValue(1,&index);
					int nbOfLeftEvents = getEventCount();
					if (nbOfLeftEvents)
					{
						beh->ActivateOutput(bbO_HasMore);
						return 0;
					}else
					{
						beh->ActivateOutput(bbO_None);
						return 0;
					}
				}
			}
		}
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	return 0;
}

//************************************
// Method:    PCGroupTriggerEventCB
// FullName:  PCGroupTriggerEventCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PCGroupTriggerEventCB(const CKBehaviorContext& behcontext)
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

