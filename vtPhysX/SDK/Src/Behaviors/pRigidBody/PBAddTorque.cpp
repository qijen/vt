#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBAddTorqueDecl();
CKERROR CreatePBAddTorqueProto(CKBehaviorPrototype **pproto);
int PBAddTorque(const CKBehaviorContext& behcontext);
CKERROR PBAddTorqueCB(const CKBehaviorContext& behcontext);


//************************************
// Method:    FillBehaviorPBAddTorqueDecl
// FullName:  FillBehaviorPBAddTorqueDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBAddTorqueDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBAddTorque");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Applies an impulsive torque defined in the global coordinate frame to the body.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x54cf4fe3,0x1b912870));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBAddTorqueProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBAddTorqueProto
// FullName:  CreatePBAddTorqueProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBAddTorqueProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBAddTorque");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PBAddTorque  

	PBAddTorque is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies an impulsive torque defined in the global coordinate frame to the actor.<br>
	<h3>Technical Information</h3>

	\image html PBAddTorque.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<SPAN CLASS="pin">Global Torque: </SPAN>Torque to apply defined in the global frame. 
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
	Is utilizing #pRigidBody #pWorld #PhysicManager #pRigidBody::addTorque().<br>
	*/
	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");
	
	proto->DeclareInParameter("Global Torque",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE,0);

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBAddTorque);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBAddTorque
// FullName:  PBAddTorque
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBAddTorque(const CKBehaviorContext& behcontext)
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
		VxVector vec  = GetInputParameterValue<VxVector>(beh,0);
		int fMode = GetInputParameterValue<int>(beh,1);
				
		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBody(target);
		if(result)
		{
			result->addTorque(vec,(ForceMode)fMode);
		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBAddTorqueCB
// FullName:  PBAddTorqueCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBAddTorqueCB(const CKBehaviorContext& behcontext)
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

