#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBAddLocalForceAtLocalPosDecl();
CKERROR CreatePBAddLocalForceAtLocalPosProto(CKBehaviorPrototype **pproto);
int PBAddLocalForceAtLocalPos(const CKBehaviorContext& behcontext);
CKERROR PBAddLocalForceAtLocalPosCB(const CKBehaviorContext& behcontext);



enum bbInputs
{

	bbI_Force,
	bbI_Pos,
	bbI_Mode
};

//************************************
// Method:    FillBehaviorPBAddLocalForceAtLocalPosDecl
// FullName:  FillBehaviorPBAddLocalForceAtLocalPosDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBAddLocalForceAtLocalPosDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBAddLocalForceAtLocalPos");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Applies a force (or impulse) defined in the bodies local coordinate frame, acting at a particular point in local coordinates, to the actor.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x27b72bda,0x4db179ce));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBAddLocalForceAtLocalPosProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBAddLocalForceAtLocalPosProto
// FullName:  CreatePBAddLocalForceAtLocalPosProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBAddLocalForceAtLocalPosProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBAddLocalForceAtLocalPos");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PBAddLocalForceAtLocalPos  

	PBAddLocalForceAtLocalPos is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies a force (or impulse) defined in the actor local coordinate frame, acting at a 
	particular point in local coordinates, to the actor.<br>

	<h3>Technical Information</h3>

	\image html PBAddLocalForceAtLocalPos.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pin">Local Force:</SPAN>Force/impulse to add, defined in the local frame.
	<BR>
	<SPAN CLASS="pin">Local Position:</SPAN>Position in the local frame to add the force at.
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
	Is utilizing #pRigidBody #pWorld #PhysicManager #pRigidBody::addLocalForceAtLocalPos().<br>
	*/

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	
	proto->DeclareInParameter("Local Force",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Local Position",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE,0);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBAddLocalForceAtLocalPos);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBAddLocalForceAtLocalPos
// FullName:  PBAddLocalForceAtLocalPos
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBAddLocalForceAtLocalPos(const CKBehaviorContext& behcontext)
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
		VxVector pos = GetInputParameterValue<VxVector>(beh,bbI_Pos);
		int fMode = GetInputParameterValue<int>(beh,bbI_Mode);

		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBody(target);
		if(result)
		{
			result->addLocalForceAtLocalPos(force,pos,(ForceMode)fMode);
		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBAddLocalForceAtLocalPosCB
// FullName:  PBAddLocalForceAtLocalPosCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBAddLocalForceAtLocalPosCB(const CKBehaviorContext& behcontext)
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

