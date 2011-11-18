#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJGroupBreakIteratorDecl();
CKERROR CreatePJGroupBreakIteratorProto(CKBehaviorPrototype **pproto);
int PJGroupBreakIterator(const CKBehaviorContext& behcontext);
CKERROR PJGroupBreakIteratorCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPJGroupBreakIteratorDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJGroupBreakIterator");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Enables trigger output for multiple objects when a joint got broken");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7a64881,0x3aca7f0c));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJGroupBreakIteratorProto);
	od->SetCompatibleClassId(CKCID_GROUP);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePJGroupBreakIteratorProto
// FullName:  CreatePJGroupBreakIteratorProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJGroupBreakIteratorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJGroupBreakIterator");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJGroupBreakIterator  

	PJGroupBreakIterator is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Triggers outputs if the body enters,stays or leaves another body .<br>
	See <A HREF="pBTriggerEvent.cmo">pBTriggerEvent.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PJGroupBreakIterator.png

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
	
	proto->DeclareOutput("None");
	proto->DeclareOutput("Break");
	
	proto->DeclareOutParameter("Body A",CKPGUID_3DENTITY,0);
	proto->DeclareOutParameter("Body B",CKPGUID_3DENTITY,0);
	proto->DeclareOutParameter("Impulse",CKPGUID_FLOAT,0);

	proto->DeclareLocalParameter("currentIndex", CKPGUID_INT);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PJGroupBreakIterator);
	*pproto = proto;
	return CK_OK;
}
enum bOutputs
{
	bbO_None,
	bbO_Break,
};

enum bInputs
{
	bbI_Init,
	bbI_Next,
};

bool isInGroup2(CKGroup *src, CK3dEntity* testObject)
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

//************************************
// Method:    PJGroupBreakIterator
// FullName:  PJGroupBreakIterator
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJGroupBreakIterator(const CKBehaviorContext& behcontext)
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

	int nbOfEvents = GetPMan()->getJointFeedbackList().Size();
	/************************************************************************/
	/* handle init                                                                     */
	/************************************************************************/
	if( beh->IsInputActive(bbI_Init) )
	{
		beh->ActivateInput(bbI_Init,FALSE);
		if (nbOfEvents)
		{
			beh->ActivateInput(bbI_Next,TRUE);
		}
	}

	/************************************************************************/
	/* handle trigger 'next'                                                                     */
	/************************************************************************/
	if( beh->IsInputActive(bbI_Next) && nbOfEvents )
	{
		beh->ActivateInput(bbI_Next,FALSE);
		beh->ActivateOutput(bbO_Break,FALSE);
		CK3dEntity *bodyAEnt =NULL; //static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mAEnt));
		CK3dEntity *bodyBEnt = NULL;//static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mBEnt));

		pBrokenJointEntry *entry = *GetPMan()->getJointFeedbackList().At(0);

		bodyAEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mAEnt));
		bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mBEnt));

		if (isInGroup2(target,bodyAEnt) || isInGroup2(target,bodyBEnt))
		{

			beh->SetOutputParameterObject(0,bodyAEnt);
			beh->SetOutputParameterObject(1,bodyBEnt);
			SetOutputParameterValue<float>(beh,2,entry->impulse);
			GetPMan()->getJointFeedbackList().EraseAt(0);
			beh->ActivateOutput(bbO_Break);
			return 0;
		}
	}

	/************************************************************************/
	/* terminate                                                                     */
	/************************************************************************/
	beh->ActivateOutput(bbO_None);
	return 0;
}

//************************************
// Method:    PJGroupBreakIteratorCB
// FullName:  PJGroupBreakIteratorCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJGroupBreakIteratorCB(const CKBehaviorContext& behcontext)
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

