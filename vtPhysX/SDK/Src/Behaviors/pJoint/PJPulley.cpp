#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJPulleyDecl();
CKERROR CreatePJPulleyProto(CKBehaviorPrototype **pproto);
int PJPulley(const CKBehaviorContext& behcontext);
CKERROR PJPulleyCB(const CKBehaviorContext& behcontext);

enum bbInputs
{
	bI_ObjectB,
	bI_Anchor0,
	bI_Anchor1,
	bI_Pulley0,
	bI_Pulley0Ref,
	bI_Pulley1,
	bI_Pulley1Ref,
	bI_Stifness,
	bI_Distance,
	bI_Ratio,
	bI_Rigid,
	bi_Collision,
	bI_Motor,

};
//************************************
// Method:    FillBehaviorPJPulleyDecl
// FullName:  FillBehaviorPJPulleyDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJPulleyDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJPulley");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets a pulley joint between two bodies.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x43d5361f,0x683f2741));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJPulleyProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	return od;
}
//************************************
// Method:    CreatePJPulleyProto
// FullName:  CreatePJPulleyProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJPulleyProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJPulley");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJPulley

	<br>
	PJPulley is categorized in \ref Joints
	<br>
	<br>See <A HREF="pJPulley.cmo">pJPulley.cmo</A>.

	

	

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a pulley (rope) joint.
	<br>

	\image html pulleyJoint.png


	The pulley joint simulates a rope that can be thrown across a pair of pulleys. In this way, it is similar to the distance joint (the length of the rope is the distance) but the rope doesn't connect the two bodies along the shortest path, rather it leads from the connection point on one actor to the pulley point (fixed in world space), then to the second pulley point, and finally to the other actor.

	The pulley joint can also be used to simulate a rope around a single point by making the pulley points coincide.

	Note that a setup where either object attachment point coincides with its corresponding pulley suspension point in world space is invalid. In this case, the simulation would be unable to determine the appropriate direction in which to pull the object and a random direction would result. The simulation will be unstable. Note that it is also invalid to allow the simulation to end up in such a state. 


	<h3>Technical Information</h3>

	\image html PJPulley.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>

	
	<SPAN CLASS="pin">Local Anchor A:</SPAN>Sets the attachment point of joint in bodie[0]'s space.See pJointPulley::setLocalAnchorA().
	<BR>

	<SPAN CLASS="pin">Local Anchor B:</SPAN>Sets the attachment point of joint in bodie[1]'s space.See pJointPulley::setLocalAnchorB().
	<BR>



	<SPAN CLASS="pin">Pulley A: </SPAN>Sets the suspension point of joint in world space. See pJointPulley::setPulleyA().
	<BR>

	<SPAN CLASS="pin">Pulley A Reference: </SPAN>Sets the suspension point of joint in local space by using an reference entity. The above vector gets transformed.
	<BR>

	<SPAN CLASS="pin">Pulley B: </SPAN>Sets the suspension point of joint in world space.See pJointPulley::setPulleyA().
	<BR>

	<SPAN CLASS="pin">Pulley B Reference: </SPAN>Sets the suspension point of joint in local space by using an reference entity. The above vector gets transformed.
	<BR>

	<SPAN CLASS="pin">Stiffness: </SPAN>Sets how stiff the constraint is, between 0 and 1 (stiffest). See pJointPulley::setStiffness().
	<BR>

	<SPAN CLASS="pin">Distance: </SPAN> Sets the rest length of the rope connecting the two objects.See pJointPulley::setDistance().
	<BR>

	\Note The distance is computed as ||(pulley0 - anchor0)|| + ||(pulley1 - anchor1)|| * ratio.

	<SPAN CLASS="pin">Ratio: </SPAN>.Sets transmission ratio. See pJointPulley::setRatio().
	<BR>

	<SPAN CLASS="pin">Is Rigid: </SPAN>.Set true if the joint also has to maintain a minimum distance, not just a maximum. See pJointPulley::setRigid().
	<BR>


	<SPAN CLASS="pin">Collision: </SPAN>Enables Collision. See pJointPulley::enableCollision().
	<BR>

	<SPAN CLASS="pin">Motor: </SPAN>Motor parameters. See pJointPulley::setMotor().
	<BR>
	<BR>
	<hr>


	<BR>
	<h3>Warning</h3>
	At least one body must be dynamic.
	<br>
	

	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include pJPulley.vsl
	</SPAN>
	<br>

	<h3>VSL : Motor Modification  </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJRevoluteSetMotor.vsl
	</SPAN>
	<br>
	<br>
	

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createJointPulley(). #pJointPulley<br>

	*/


	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( PJPulleyCB );
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Local Anchor A",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Local Anchor B",CKPGUID_VECTOR,"0.0f");
	
	proto->DeclareInParameter("Pulley A",CKPGUID_VECTOR);
	proto->DeclareInParameter("Pulley A Reference",CKPGUID_3DENTITY);

	proto->DeclareInParameter("Pulley B",CKPGUID_VECTOR);
	proto->DeclareInParameter("Pulley B Reference",CKPGUID_3DENTITY);

	proto->DeclareInParameter("Stiffness",CKPGUID_FLOAT);
	proto->DeclareInParameter("Distance",CKPGUID_FLOAT);
	proto->DeclareInParameter("Ratio",CKPGUID_FLOAT);

	proto->DeclareInParameter("Is Rigid",CKPGUID_BOOL);
	
	proto->DeclareInParameter("Collision",CKPGUID_BOOL);

	proto->DeclareInParameter("Motor",VTS_JOINT_MOTOR);

	
	proto->DeclareSetting("Motor",CKPGUID_BOOL,"FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJPulley);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJPulley
// FullName:  PJPulley
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJPulley(const CKBehaviorContext& behcontext)
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
		//the object A: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Pulley))
		{
			return CK_OK;
		}
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 

		if (!worldA && !worldB)
			bbErrorME("Couldnt find any world object");
		
		// the physic object A : 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);

		if (!bodyA && !bodyB)
			bbErrorME("Couldnt find any physic object");



		if(bodyA && bodyB)
		{
			
			VxVector anchor0  = GetInputParameterValue<VxVector>(beh,bI_Anchor0);
			VxVector anchor1  = GetInputParameterValue<VxVector>(beh,bI_Anchor1);

			VxVector pulley0  = GetInputParameterValue<VxVector>(beh,bI_Pulley0);
			CK3dEntity*pulley0Ref=  (CK3dEntity *) beh->GetInputParameterObject(bI_Pulley0Ref);
			VxVector pulley1  = GetInputParameterValue<VxVector>(beh,bI_Pulley1);
			CK3dEntity*pulley1Ref=  (CK3dEntity *) beh->GetInputParameterObject(bI_Pulley1Ref);


			int coll  = GetInputParameterValue<int>(beh,bi_Collision);



			VxVector pulley0Out  = pulley0;
			if (pulley0Ref)
				pulley0Ref->Transform(&pulley0Out,&pulley0);

			VxVector pulley1Out  = pulley1;
			if (pulley1Ref)
				pulley1Ref->Transform(&pulley1Out,&pulley1);


			float ratio  =  GetInputParameterValue<float>(beh,bI_Ratio);
			float stiff  =  GetInputParameterValue<float>(beh,bI_Stifness);
			float distance =  GetInputParameterValue<float>(beh,bI_Distance);
			int rigid =  GetInputParameterValue<float>(beh,bI_Rigid);
			
			pMotor motor;
			
			DWORD hasMotor;beh->GetLocalParameterValue(0,&hasMotor);
			if (hasMotor)
			{
				CKParameterIn *par = beh->GetInputParameter(bI_Motor);
				if (par)
				{
					CKParameter *rPar = par->GetRealSource();
					if (rPar)
					{
						motor  = pFactory::Instance()->createMotorFromParameter(rPar);
					}
				}
			}

			
			//////////////////////////////////////////////////////////////////////////
			//
			pJointPulley*joint = static_cast<pJointPulley*>(worldA->getJoint(target,targetB,JT_Pulley));

			if (!joint)
			{
				joint   = static_cast<pJointPulley*>(pFactory::Instance()->createPulleyJoint(target,targetB,pulley0Out,pulley1Out,anchor0,anchor1));
				if (!joint)
				{
					bbErrorMesg("Couldn't create Pulley joint!");
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//joint exists : update : 
			if (joint)
			{
				joint->setStiffness(stiff);
				joint->setRatio(ratio);
				joint->setRigid(rigid);
				joint->setDistance(distance);
				joint->setPulleyA(pulley0Out);
				joint->setPulleyB(pulley1Out);
				joint->setLocalAnchorA(anchor0);
				joint->setLocalAnchorB(anchor1);
				joint->enableCollision(coll);

				if (hasMotor)
				{
					joint->setMotor(motor);
				}
			}
		}
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJPulleyCB
// FullName:  PJPulleyCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJPulleyCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
			DWORD hasMotor;
			beh->GetLocalParameterValue(0,&hasMotor);
			beh->EnableInputParameter(bI_Motor,hasMotor);


		break;
	}
	}
	return CKBR_OK;
}