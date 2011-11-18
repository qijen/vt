#include <StdAfx.h>
#include "pCommon.h"



CKObjectDeclaration	*FillBehaviorPBSetHardDecl();
CKERROR CreatePBSetHardProto(CKBehaviorPrototype **pproto);
int PBSetHard(const CKBehaviorContext& behcontext);
CKERROR PBSetHardCB(const CKBehaviorContext& behcontext);


enum bInputs
{

	bbI_Vel,
	bbI_AVel,
	bbI_Momentum,
	bbI_Torque,
	bbI_Pos,
	bbI_Rotation,
	
};

#define BB_SSTART 0

BBParameter pInMap23[]  =
{


	BB_SPIN(bbI_Vel,CKPGUID_VECTOR,"Linear Velocity",""),
	BB_SPIN(bbI_AVel,CKPGUID_VECTOR,"Angular Velocity",""),
	BB_SPIN(bbI_Momentum,CKPGUID_VECTOR,"Linear Momentum",""),
	BB_SPIN(bbI_Torque,CKPGUID_VECTOR,"Angualar Momentum",""),
	BB_SPIN(bbI_Pos,CKPGUID_VECTOR,"Position",""),
	BB_SPIN(bbI_Rotation,CKPGUID_QUATERNION,"Rotation",""),

};

#define gPIMAP pInMap23

//************************************
// Method:    FillBehaviorPBSetHardDecl
// FullName:  FillBehaviorPBSetHardDecl
// Access:    public
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBSetHardDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSetHard");
	od->SetCategory("Physic/Body");
	od->SetDescription("Sets momentum,velocities and transformations.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x528d58e0,0x2fca203a));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSetHardProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePBSetHardProto
// FullName:  CreatePBSetHardProto
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBSetHardProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSetHard");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	/*! \page PBSetHard  

	PBSet is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PBSetHard.png

	<SPAN CLASS="in">In:</SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out:</SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated with the rigid body or its sub shape. 
	<BR>
	<BR>
	<SPAN CLASS="pin">Linear Velocity: </SPAN>The linear velocity.See pRigidBody::setLinearVelocity().
	<BR>
	<SPAN CLASS="pin">Angular Velocity: </SPAN>The angular velocity.See pRigidBody::setAngularVelocity().
	<BR>
	<SPAN CLASS="pin">Agular Momentum: </SPAN>The angular momentum.See pRigidBody::getAngularMomentum().
	<BR>
	<SPAN CLASS="pin">Linear Momentum: </SPAN>The linear momentum.See pRigidBody::getLinearMomentum(). 
	<BR>
	<SPAN CLASS="pin">Position: </SPAN>The position.See pRigidBody::setPosition(). 
	<BR>
	<SPAN CLASS="pin">Orientation: </SPAN>The new rotation.See pRigidBody::setRotation(). 
	<BR>


	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>

	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager .
	<br>
	<br>
	<SPAN CLASS="framedWhite">
	The known building block <A HREF="Documentation.chm::/behaviors/3D%20Transformations/Set%20Position.html">"Set Position"</A> has new settings avaiable.
	Enable there "Update Physics" in order to transfer the new position on the body automatically!
	</SPAN>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include PBSet.cpp
	</SPAN>

	*/

	proto->SetBehaviorCallbackFct( PBSetHardCB );

	BB_EVALUATE_SETTINGS(gPIMAP)

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PBSetHard);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBSetHard
// FullName:  PBSetHard
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBSetHard(const CKBehaviorContext& behcontext)
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
		if( !target ) 	bbErrorME("No Reference Object specified");

		pRigidBody *body = NULL;

		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world)
		{
			if (!body)	bbErrorME("No valid world object found");
		}


		body = GetPMan()->getBody(target);
		if (!body)	bbErrorME("No Reference Object specified");

		BB_DECLARE_PIMAP;

		/************************************************************************/
		/* retrieve settings state  */
		/************************************************************************/
		BBSParameter(bbI_Vel);
		BBSParameter(bbI_AVel);
		BBSParameter(bbI_Torque);
		BBSParameter(bbI_Momentum);
		BBSParameter(bbI_Pos);
		BBSParameter(bbI_Rotation);
		
	
	
		if (sbbI_Vel)
		{
			VxVector velocity= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_Vel));
			body->setLinearVelocity(velocity);
		}

		if (sbbI_AVel)
		{
			VxVector aVelocity= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_AVel));
			body->setAngularVelocity(aVelocity);
		}

		if (sbbI_Torque)
		{
			VxVector torque= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_Torque));
			body->setAngularMomentum(torque);
		}
		if (sbbI_Momentum)
		{
			VxVector momentum= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_Momentum));
			body->setLinearMomentum(momentum);
		}

		
		if (sbbI_Pos)
		{
			VxVector pos= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_Pos));
			body->setPosition(pos,target);
		}
		if (sbbI_Rotation)
		{
			VxQuaternion rot= GetInputParameterValue<VxQuaternion>(beh,BB_IP_INDEX(bbI_Rotation));
			body->setRotation(rot,target);
		}

	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PBSetHardCB
// FullName:  PBSetHardCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBSetHardCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
	
	CKContext* ctx = behcontext.Context;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;

}

