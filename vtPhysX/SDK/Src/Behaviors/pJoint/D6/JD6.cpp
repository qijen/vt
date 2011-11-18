#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorJSetD6Decl();
CKERROR CreateJSetD6Proto(CKBehaviorPrototype **pproto);
int JSetD6(const CKBehaviorContext& behcontext);
CKERROR JSetD6CB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyA=0,
	bbI_BodyB,
	bbI_Anchor,
	bbI_AnchorRef,
	bbI_Axis,
	bbI_AxisRef,
	bbI_Coll,
	bbI_PMode,
	bbI_PDistance,
	bbI_PAngle
};

//************************************
// Method:    FillBehaviorJSetD6Decl
// FullName:  FillBehaviorJSetD6Decl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJSetD6Decl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJD6");	
	od->SetCategory("Physic/D6");
	od->SetDescription("Sets or modifies a D6 joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7456331,0xc6c58d6));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJSetD6Proto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJSetD6Proto
// FullName:  CreateJSetD6Proto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJSetD6Proto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJD6");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	/*! \page PJD6

	<br>
	PJD6 is categorized in \ref Joints
	<br>
	<br>See <A HREF="PJD6.cmo">PJD6.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a D6 joint.
	<br>
	<br>
	DOFs removed: 0-6<br>
	DOFs remaining: 0-6<br>
	<br>
	<br>

	This joint type can be configured to model nearly any joint imaginable. Each degree of freedom - both linear and angular - can be selectively locked or freed, and separate limits can be applied to each. The 6 DOF joint provides motor drive on all axes independently, and also allows soft limits.

	The joint axis (localAxis[]) defines the joint's local x-axis. The joint normal (localNormal[]) defines the joint's local y-axis. The local z-axis is computed as a cross product of the first two. 

	When constraining the angular motion of the joint, rotation around the x-axis is referred to as twist, rotation around the y-axis as swing1, and rotation around the z-axis as swing2.


	\image html 6dofaxis.png

	
	<h3>Technical Information</h3>

	\image html PJD6.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>

	<hr>

	<SPAN CLASS="pin">Anchor:</SPAN>A point in world space coordinates. See pJointD6::setGlobalAnchor().
	<BR>
	<SPAN CLASS="pin">Anchor Reference: </SPAN>A helper entity to transform a local anchor into the world space.
	<BR>

	<hr>

	<SPAN CLASS="pin">Axis: </SPAN>An in world space. See pJointD6::setGlobalAxis().
	<BR>
	<SPAN CLASS="pin">Axis Up Reference: </SPAN>A helper entity to transform a local axis into the world space.
	<BR>
	<BR>
	<hr>

	<SPAN CLASS="pin">Collision: </SPAN>Enable Collision. See pJointD6::enableCollision().
	<BR>

	<hr>
	<SPAN CLASS="pin">Projection Mode: </SPAN>Joint projection mode. See pJointD6::setProjectionMode() and #ProjectionMode.
	<BR>

	<SPAN CLASS="pin">Projection Distance: </SPAN>If any joint projection is used, it is also necessary to set 
	projectionDistance to a small value greater than zero. When the joint error is larger than projectionDistance the SDK will change it so that the joint error is equal to projectionDistance. Setting  projectionDistance too 
	small will introduce unwanted oscillations into the simulation.See pJointD6::setProjectionDistance().
	<br>

	<SPAN CLASS="pin">Projection Angle: </SPAN>Angle must be greater than 0.02f .If its smaller then current algo gets too close to a singularity. See pJointD6::setProjectionAngle().
	<BR>




	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>

	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include D6Creation.vsl
	</SPAN>
	<br>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createD6Joint().<br>

	*/
	
	proto->SetBehaviorCallbackFct( JSetD6CB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY,"0.0f");
	
	proto->DeclareInParameter("Axis",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Axis Up Reference",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Collision",CKPGUID_BOOL,"TRUE");

	proto->DeclareInParameter("Projection Mode",VTE_JOINT_PROJECTION_MODE,"0");
	proto->DeclareInParameter("Projection Distance",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Projection Angle",CKPGUID_FLOAT,"0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JSetD6);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JSetD6
// FullName:  JSetD6
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JSetD6(const CKBehaviorContext& behcontext)
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
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_D6))
		{

			return CK_OK;
		}
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA)
		{
			worldA = worldB;
		}
		if (!worldA)
		{
			beh->ActivateOutput(0);
			return 0;

		}
		
		
		// the physic object A : 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);

		if(bodyA || bodyB)
		{

			pJointD6 *joint =static_cast<pJointD6*>(worldA->getJoint(target,targetB,JT_D6));
			//anchor : 
			VxVector anchor  = GetInputParameterValue<VxVector>(beh,1);
			VxVector anchorOut  = anchor;
			CK3dEntity*anchorReference  =  (CK3dEntity *) beh->GetInputParameterObject(2);

			if (anchorReference)
			{
				anchorReference->Transform(&anchorOut,&anchor);
			}

			//axis 
			VxVector axis  = GetInputParameterValue<VxVector>(beh,3);
			VxVector axisOut  = axis;
			CK3dEntity*axisReference  =  (CK3dEntity *) beh->GetInputParameterObject(4);

			if (axisReference)
			{
				VxVector dir,up,right;
				axisReference->GetOrientation(&dir,&up,&right);
				axisReference->TransformVector(&axisOut,&up);
			}

			int coll  = GetInputParameterValue<int>(beh,bbI_Coll);

			ProjectionMode mode =GetInputParameterValue<ProjectionMode>(beh,bbI_PMode); 
			float distance = GetInputParameterValue<float>(beh,bbI_PDistance); 
			float angle= GetInputParameterValue<float>(beh,bbI_PAngle); 

			
			if (!joint)
			{
				joint   = static_cast<pJointD6*>(pFactory::Instance()->createD6Joint(target,targetB,anchorOut,axisOut,coll));
			}

			//////////////////////////////////////////////////////////////////////////
			//joints parameters : 
			//set it up : 
			if (joint)
			{
				joint->setGlobalAnchor(anchorOut);
				joint->setGlobalAxis(axisOut);
				joint->enableCollision(coll);
				if (mode != 0)
				{
					joint->setProjectionMode(mode);
					joint->setProjectionDistance(distance);
					joint->setProjectionAngle(angle);
				}
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JSetD6CB
// FullName:  JSetD6CB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JSetD6CB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	

	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD fmax;
			beh->GetLocalParameterValue(0,&fmax);
			if (fmax  )
			{
				beh->EnableInputParameter(6,fmax);
				beh->EnableInputParameter(7,fmax);
			
			}else
			{
				beh->EnableInputParameter(6,fmax);
				beh->EnableInputParameter(7,fmax);
				//////////////////////////////////////////////////////////////////////////
				//the object A: 
				CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
				CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);

				if (target && targetB)
				{
					// the world :  
					pWorld *world=GetPMan()->getWorldByBody(target); 
					if (!world)
						return CKBR_OK;

					// the physic object A and B : 
					pRigidBody*bodyA= world->getBody(target);
					pRigidBody*bodyB= world->getBody(targetB);

					if(bodyA && bodyB)
					{
//						pJointHinge2 *joint = static_cast<pJointHinge2*>(bodyA->isConnected(targetB));
						/*
						if (joint && joint->GetFeedBack())
						{
							joint->SetFeedBack(NULL,0,0);
						}*/
					}
				}
			}
		}
		break;
	}
	return CKBR_OK;
}

