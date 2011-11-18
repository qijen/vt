#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPBSetDecl();
CKERROR CreateBodySetProto(CKBehaviorPrototype **pproto);
int BodySet(const CKBehaviorContext& behcontext);
CKERROR BodySetCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_Vel=0,
	bbI_AVel,
	bbI_Torque,
	bbI_Force,
	bbI_Pos,
	bbI_Rot

};

enum bSettings
{
	bbS_Vel=0,
	bbS_AVel,
	bbS_Torque,
	bbS_Force,
	bbS_Pos,
	bbS_Rot
};

//************************************
// Method:    FillBehaviorPBSetDecl
// FullName:  FillBehaviorPBSetDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBSetDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSet");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Sets physical quantities.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6915390d,0x1d775a96));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateBodySetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}
CKERROR CreateBodySetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSet-Obsolete");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->SetBehaviorCallbackFct( BodySetCB );
	
	

	/*
	PBSet  

	PBSet is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PBSet.png

	<SPAN CLASS="in">In:</SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out:</SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
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
	<BR>
	
	<SPAN CLASS="setting">Linear Velocity: </SPAN>Enables output for linear velocity.
	<BR>
	<SPAN CLASS="setting">Angular Velocity: </SPAN>Enables output for angular velocity.
	<BR>
	<SPAN CLASS="setting">Agular Momentum: </SPAN>Enables output for angular momentum.
	<BR>
	<SPAN CLASS="setting">Linear Momentum: </SPAN>Enables output for linear momentum. 
	<BR>
	<SPAN CLASS="setting">Position: </SPAN>Enables output for position. 
	<BR>
	<SPAN CLASS="setting">Orientation: </SPAN>Enables output for orientation. 
	<BR>
	<BR>
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

	
	proto->DeclareInParameter("Linear Velocity",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Angular Velocity",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Angular Momentum",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Linear Momentum",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Position",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Orientation",CKPGUID_QUATERNION);

	
	proto->DeclareSetting("Velocity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Angular Velocity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Torque",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Force",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Position",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Rotation",CKPGUID_BOOL,"FALSE");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(BodySet);
	*pproto = proto;
	return CK_OK;
}

int BodySet(const CKBehaviorContext& behcontext)
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
		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBody(target);
		if(result)
		{
			
			//////////////////////////////////////////////////////////////////////////
			//velocity : 
			DWORD vel=0;
			beh->GetLocalParameterValue(bbS_Vel,&vel);
			if (vel)
			{
				VxVector vec  = GetInputParameterValue<VxVector>(beh,bbI_Vel);
				result->setLinearVelocity(vec);
			}

			//////////////////////////////////////////////////////////////////////////
			//angular velocity 
			DWORD avel=0;
			beh->GetLocalParameterValue(bbS_AVel,&avel);
			if (avel)
			{
				VxVector vec  = GetInputParameterValue<VxVector>(beh,bbI_AVel);
				result->setAngularVelocity(vec);
			}

			//////////////////////////////////////////////////////////////////////////
			//torque
			DWORD torque=0;
			beh->GetLocalParameterValue(bbS_Torque,&avel);
			if (torque)
			{
				VxVector vec  = GetInputParameterValue<VxVector>(beh,bbI_Torque);
				result->setAngularMomentum(vec);
			}

			//////////////////////////////////////////////////////////////////////////
			//force
			DWORD force=0;
			beh->GetLocalParameterValue(bbS_Force,&force);
			if (force)
			{
				VxVector vec  = GetInputParameterValue<VxVector>(beh,bbI_Force);
				result->setLinearMomentum(vec);
			}

			//////////////////////////////////////////////////////////////////////////
			//force
			DWORD pos=0;
			beh->GetLocalParameterValue(bbS_Pos,&pos);
			if (pos)
			{

				VxVector vec  = GetInputParameterValue<VxVector>(beh,bbI_Pos);
				result->setPosition(vec);
			}
			//////////////////////////////////////////////////////////////////////////
			DWORD rot=0;
			beh->GetLocalParameterValue(bbS_Rot,&rot);
			if (rot)
			{

				VxQuaternion vec  = GetInputParameterValue<VxQuaternion>(beh,bbI_Rot);
				result->setRotation(vec);
			}

		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    BodySetCB
// FullName:  BodySetCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR BodySetCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD vel;
			beh->GetLocalParameterValue(bbS_Vel,&vel);
			beh->EnableInputParameter(bbI_Vel,vel);
			
			DWORD avel;
			beh->GetLocalParameterValue(bbS_AVel,&avel);
			beh->EnableInputParameter(bbI_AVel,avel);

			DWORD torque;
			beh->GetLocalParameterValue(bbS_Torque,&torque);
			beh->EnableInputParameter(bbI_Torque,torque);

			DWORD force;
			beh->GetLocalParameterValue(bbS_Force,&force);
			beh->EnableInputParameter(bbI_Force,force);
			
			DWORD pos;
			beh->GetLocalParameterValue(bbS_Pos,&pos);
			beh->EnableInputParameter(bbI_Pos,pos);
			DWORD rot;
			beh->GetLocalParameterValue(bbS_Rot,&rot);
			beh->EnableInputParameter(bbI_Rot,rot);
		
		}
		break;
	}
	return CKBR_OK;

}

