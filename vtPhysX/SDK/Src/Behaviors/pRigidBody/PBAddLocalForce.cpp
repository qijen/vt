#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBAddLocalForceDecl();
CKERROR CreatePBAddLocalForceProto(CKBehaviorPrototype **pproto);
int PBAddLocalForce(const CKBehaviorContext& behcontext);
CKERROR PBAddLocalForceCB(const CKBehaviorContext& behcontext);



enum bbInputs
{

	bbI_Force,
	bbI_Mode
};

//************************************
// Method:    FillBehaviorPBAddLocalForceDecl
// FullName:  FillBehaviorPBAddLocalForceDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBAddLocalForceDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBAddLocalForce");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Applies a force (or impulse) defined in the actor local coordinate frame to the body.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x23cb2655,0x50da723c));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBAddLocalForceProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBAddLocalForceProto
// FullName:  CreatePBAddLocalForceProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBAddLocalForceProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBAddLocalForce");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PBAddLocalForce  

	PBAddLocalForce is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies a force (or impulse) defined in the actor local coordinate frame, acting at a 
	particular point in global coordinates, to the actor. <br>

	<h3>Technical Information</h3>

	\image html PBAddLocalForce.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pin">Local Force:</SPAN>Force/Impulse to apply defined in the local frame.
	<BR>
	<SPAN CLASS="pin">Force Mode: </SPAN>The way how the force is applied.See #ForceMode
	<BR>
	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<h3>Note</h3><br>
	
	Note that if the force does not act along the center of mass of the actor, this
	will also add the corresponding torque. Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.
	<br>
	<br>
	Is utilizing #pRigidBody #pWorld #PhysicManager #pRigidBody::addLocalForce().<br>
	*/
	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");
	
	proto->DeclareInParameter("Local Force",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE,0);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBAddLocalForce);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBAddLocalForce
// FullName:  PBAddLocalForce
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBAddLocalForce(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{

		//////////////////////////////////////////////////////////////////////////
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world)
		{
			beh->ActivateOutput(0);
			return 0;

		}
		
		//the vector : 
		VxVector force  = GetInputParameterValue<VxVector>(beh,bbI_Force);
		int fMode = GetInputParameterValue<int>(beh,bbI_Mode);

		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBody(target);
		if(result)
		{
			result->addLocalForce(force,(ForceMode)fMode);
		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBAddLocalForceCB
// FullName:  PBAddLocalForceCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBAddLocalForceCB(const CKBehaviorContext& behcontext)
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

