#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBGetVelocitiesAndForcesDecl();
CKERROR CreatePBGetVelocitiesAndForcesProto(CKBehaviorPrototype **pproto);
int PBGetVelocitiesAndForces(const CKBehaviorContext& behcontext);
CKERROR PBGetVelocitiesAndForcesCB(const CKBehaviorContext& behcontext);

using namespace vtTools;
using namespace BehaviorTools;



enum bbI_Inputs
{
	bbI_BodyReference,

};

#define BB_SSTART 0


enum bInputs
{

	bbO_Vel,
	bbO_AVel,
	bbO_Momentum,
	bbO_Torque,
};
BBParameter pOutMapPBGetPar2[]  = 
{
	BB_SPOUT(bbO_Vel,CKPGUID_VECTOR,"Linear Velocity",""),
	BB_SPOUT(bbO_AVel,CKPGUID_VECTOR,"Angular Velocity",""),
	BB_SPOUT(bbO_Torque,CKPGUID_VECTOR,"Linear Momentum",""),
	BB_SPOUT(bbO_Momentum,CKPGUID_VECTOR,"Angular Momentum",""),

};

#define gOPMap pOutMapPBGetPar2


CKERROR PBGetVelocitiesAndForcesCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	int cb = behcontext.CallbackMessage;
	BB_DECLARE_PMAP;
	switch(behcontext.CallbackMessage) {



		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
			{
				BB_LOAD_POMAP(gOPMap,BB_SSTART);
				break;
			}
		case CKM_BEHAVIORDETACH:
			{
				BB_DESTROY_PMAP;
				break;
			}

		case CKM_BEHAVIORATTACH:
			{
				BB_INIT_PMAP(gOPMap,BB_SSTART);
				break;

			}

		case CKM_BEHAVIORSETTINGSEDITED:
			{
				BB_REMAP_PMAP(gOPMap,BB_SSTART);
				break;
			}
	}
	return CKBR_OK;
}

CKObjectDeclaration	*FillBehaviorPBGetVelocitiesAndForcesDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBGetParEx");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Retrieves forces and velocities");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x1076b62,0x1dea09ed));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBGetVelocitiesAndForcesProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBGetVelocitiesAndForcesProto
// FullName:  CreatePBGetVelocitiesAndForcesProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBGetVelocitiesAndForcesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBGetParEx");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*! \page PBGetParEx  

	PBGetParEx is categorized in \ref Vehicle

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Retrieves velocities and forces.<br>

	<h3>Technical Information</h3>

	\image html PBGetParEx.png
	<SPAN CLASS="in">In:</SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out:</SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pout">Linear Velocity: </SPAN>The linear velocity.See pRigidBody::getLinearVelocity().
	<BR>
	<SPAN CLASS="pout">Angular Velocity: </SPAN>The angular velocity.See pRigidBody::getAngularVelocity().
	<BR>
	<SPAN CLASS="pout">Agular Momentum: </SPAN>The angular momentum.See pRigidBody::getAngularMomentum().
	<BR>
	<SPAN CLASS="pout">Linear Momentum: </SPAN>The linear momentum.See pRigidBody::getLinearMomentum(). 
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
		\include PBGetEx.cpp
	</SPAN>
	*/
	proto->SetBehaviorCallbackFct( PBGetVelocitiesAndForcesCB );


	BB_EVALUATE_SETTINGS(gOPMap);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBGetVelocitiesAndForces);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBGetVelocitiesAndForces
// FullName:  PBGetVelocitiesAndForces
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBGetVelocitiesAndForces(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) 	bbErrorME("No Reference Object specified");

		pRigidBody *body = NULL;


		body = GetPMan()->getBody(target);
		if (!body)	bbErrorME("No Reference Object specified");

		BB_DECLARE_PMAP;

		BBSParameter(bbO_Vel);
		BBSParameter(bbO_AVel);
		
		BBSParameter(bbO_Torque);
		BBSParameter(bbO_Momentum);

        

		BB_O_SET_VALUE_IF(VxVector,bbO_Vel,body->getLinearVelocity());
		BB_O_SET_VALUE_IF(VxVector,bbO_AVel,body->getAngularVelocity());
		BB_O_SET_VALUE_IF(VxVector,bbO_Momentum,body->getLinearMomentum());
		BB_O_SET_VALUE_IF(VxVector,bbO_Vel,body->getLinearVelocity());
		
	}
	
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PBGetVelocitiesAndForcesCB
// FullName:  PBGetVelocitiesAndForcesCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


