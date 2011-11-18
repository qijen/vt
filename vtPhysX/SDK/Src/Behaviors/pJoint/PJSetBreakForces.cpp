#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJSetBreakForcesDecl();
CKERROR CreatePJSetBreakForcesProto(CKBehaviorPrototype **pproto);
int PJSetBreakForces(const CKBehaviorContext& behcontext);
CKERROR PJSetBreakForcesCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyB,
	bbI_Type,
	bbI_MaxForce,
	bbI_MaxTorque
};

//************************************
// Method:    FillBehaviorPJSetBreakForcesDecl
// FullName:  FillBehaviorPJSetBreakForcesDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJSetBreakForcesDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJSetBreakForces");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets the maximum forces on a joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3ace1062,0x265d33c5));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJSetBreakForcesProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePJSetBreakForcesProto
// FullName:  CreatePJSetBreakForcesProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJSetBreakForcesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJSetBreakForces");
	if(!proto) return CKERR_OUTOFMEMORY;


	/*! \page PJSetBreakForces  

	<br>
	PJSetBreakForces is categorized in \ref Joints
	<br>
	

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Sets a joint as breakable or unbreakable.
	
	<br>
	
	<h3>Technical Information</h3>


	Joints can be set to be breakable. This means that if a joint is tugged on with a large enough force, it will snap apart. To set a breakable joint, set the maxForce and maxTorque members of NxJointDesc to the desired values - the smaller the value, the more easily the joint will break. The maxForce defines the limit for linear forces that can be applied to a joint before it breaks, while the maxTorque defines the limit for angular forces. The exact behavior depends on the type of joint. It is possible to change these parameters for an existing joint using #pJoint::setBreakForces().

	When a joint breaks, the \ref PJIsBroken will be called.

	<b> Maximum Force and maximum Torque</b>
	
	The distinction between maxForce and maxTorque is dependent on how the joint is implemented internally, which may not be obvious. For example, what appears to be an angular degree of freedom may be constrained indirectly by a linear constraint.

	Therefore, in most practical applications the user should set both maxTorque and maxForce to similar values.

	In the current implementation, the following joints use angular constraints:

	-	\ref PJD6  - Only in the case where angular DOFs are locked. 
	-	\ref PJFixed 
	-	\ref PJPrismatic
	-	\ref PJRevolute 


	\image html PJSetBreakForces.png


	<SPAN CLASS="in">In: </SPAN> triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN> is activated when the process is completed.
	<BR>
	
	<SPAN CLASS="pin">Body B: </SPAN> The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	
	<SPAN CLASS="pin">Joint Type:</SPAN> The joint type. This helps the building block to identify a joint constraint. As usual there can be only one joint of the type x between two bodies.
	<BR>
	
	<SPAN CLASS="pin">Maximum Force:</SPAN> Maximum force the joint can withstand without breaking. 
		-	<b>Range:</b> (0,inf]
	<BR>

	<SPAN CLASS="pin">Maximum Torque:</SPAN> Maximum torque the joint can withstand without breaking.
	-	<b>Range:</b> (0,inf]
	<BR>


	\Note If Maximum Force = 0.0 AND Maximum Torque = 0.0 the joint becomes unbreakable.

	<h3>VSL : Set break forces </h3><br>
	<SPAN CLASS="NiceCode">
	\include pJSetBreakForces.vsl
	</SPAN>
	<br>
	
	Is utilizing #pRigidBody #pWorld #PhysicManager #pJoint::setBreakForces().<br>
	*/

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	
	proto->SetBehaviorCallbackFct( PJSetBreakForcesCB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Joint Type",VTE_JOINT_TYPE,"0");
	proto->DeclareInParameter("Maximum Force",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Maximum Torque",CKPGUID_FLOAT,"0");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PJSetBreakForces);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJSetBreakForces
// FullName:  PJSetBreakForces
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJSetBreakForces(const CKBehaviorContext& behcontext)
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
		//the object A + B: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(bbI_BodyB);
		
		int jointType = GetInputParameterValue<int>(beh,bbI_Type); 
		float maxForce = GetInputParameterValue<float>(beh,bbI_MaxForce); 
		float maxTorque = GetInputParameterValue<float>(beh,bbI_MaxTorque); 

		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,jointType))
			return CK_OK;
		
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
		
		
		// the physic object A && B: 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);

		//at least one needs to be there
		if(bodyA || bodyB)
		{
			pJoint *joint = (worldA->getJoint(target,targetB,(JType)jointType));
			if (joint)
				joint->setBreakForces(maxForce,maxTorque);
		}
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJSetBreakForcesCB
// FullName:  PJSetBreakForcesCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJSetBreakForcesCB(const CKBehaviorContext& behcontext)
{
	return CKBR_OK;
}