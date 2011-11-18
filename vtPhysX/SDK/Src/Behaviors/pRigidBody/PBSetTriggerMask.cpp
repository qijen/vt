#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBSetTriggerMaskDecl();
CKERROR CreatePBSetTriggerMaskProto(CKBehaviorPrototype **pproto);
int PBSetTriggerMask(const CKBehaviorContext& behcontext);
CKERROR PBSetTriggerMaskCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_OnEnter,
	bbI_OnStay,
	bbI_OnLeave,
};

//************************************
// Method:    FillBehaviorPBSetTriggerMaskDecl
// FullName:  FillBehaviorPBSetTriggerMaskDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBSetTriggerMaskDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCSetTriggerMask");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Modifies trigger mask");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x27ae51d4,0x45bf2c6c));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSetTriggerMaskProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBSetTriggerMaskProto
// FullName:  CreatePBSetTriggerMaskProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBSetTriggerMaskProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCSetTriggerMask");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PCSetTriggerMask  

	PCSetTriggerMask is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies the trigger mask of a bodies or a sub shape of it.<br>
	See <A HREF="pBTriggerEvent.cmo">pBTriggerEvent.cmo</A> for example.


	<h3>Technical Information</h3>

	\image html PBSetTriggerMask.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The shape reference. Can be a sub shape or the initial mesh.
	<BR>
	<BR>
	<SPAN CLASS="pin">On Enter: </SPAN>Enables triggering if the body starts entering another shape.
	<BR>
	<SPAN CLASS="pin">On Stay: </SPAN>Enables triggering if the body stays in another shape.
	<BR>
	<SPAN CLASS="pin">On Leave: </SPAN>Enables triggering if the body leaves another shape.
	<BR>

	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include PCSetTriggerMask.cpp
	</SPAN>
	*/

	proto->SetBehaviorCallbackFct( PBSetTriggerMaskCB );
	proto->DeclareInParameter("On Enter",CKPGUID_BOOL,"true");
	proto->DeclareInParameter("On Stay",CKPGUID_BOOL,"true");
	proto->DeclareInParameter("On Leave",CKPGUID_BOOL,"true");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBSetTriggerMask);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBSetTriggerMask
// FullName:  PBSetTriggerMask
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBSetTriggerMask(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		
		pWorld *world  = GetPMan()->getWorldByShapeReference(target);
		if (!world)
		{
			beh->ActivateOutput(0);
			return 0;
		}

		if (world)
		{
			NxShape *shape  = world->getShapeByEntityID(target->GetID());
			if (shape)
			{

				int onEnter = GetInputParameterValue<int>(beh,bbI_OnEnter);
				int onStay = GetInputParameterValue<int>(beh,bbI_OnStay);
				int onLeave = GetInputParameterValue<int>(beh,bbI_OnLeave);

				shape->setFlag(NX_TRIGGER_ON_ENTER,onEnter);
				shape->setFlag(NX_TRIGGER_ON_STAY,onStay);
				shape->setFlag(NX_TRIGGER_ON_LEAVE,onLeave);

			}
		}
			
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBSetTriggerMaskCB
// FullName:  PBSetTriggerMaskCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBSetTriggerMaskCB(const CKBehaviorContext& behcontext)
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

