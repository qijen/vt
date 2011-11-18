#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorJSetBallDecl();
CKERROR CreateJSetBallProto(CKBehaviorPrototype **pproto);
int JSetBall(const CKBehaviorContext& behcontext);
CKERROR JSetBallCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Anchor,
	bI_AnchorRef,
	bbI_SwingAxis,
	bbI_PMode,
	bbI_PDistance,
	bbI_Collision,
	bbI_SwingLimit,
	bbI_TwistHighLimit,
	bbI_TwistLowLimit,
	bbI_SwingSpring,
	bbI_TwistSpring,
	bbI_JointSpring
};

//************************************
// Method:    FillBehaviorJSetBallDecl
// FullName:  FillBehaviorJSetBallDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJSetBallDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJBall");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets/modifies a ball  joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2df921e4,0x20295f52));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJSetBallProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreateJSetBallProto
// FullName:  CreateJSetBallProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJSetBallProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJBall");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJBall  

	<br>
	PJBall is categorized in \ref Joints
	<br>
	<br>See <A HREF="pJBall.cmo">pJBall.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a sphere joint.
	<br>
	<br>
	DOFs removed: 3<br>
	DOFs remaining: 3<br>
	<br>
	\image html sphericalJoint.png
	
	A spherical joint is the simplest kind of joint. It constrains two points on two different bodies from coinciding. 
	This point, located in world space, is the only parameter that has to be specified (the other parameters are optional). 
	Specifying the anchor point (point that is forced to coincide) in world space guarantees that the point in the local space of each 
	body will coincide when the point is transformed back from local into world space. 

	By attaching a series of spherical joints and bodies, chains/ropes can be created. 
	Another example for a common spherical joint is a person's shoulder, which is quite limited in its range of motion.


	<h3>Technical Information</h3>

	\image html PJBall.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>
	<SPAN CLASS="pin">Anchor:</SPAN>A point in world space coordinates. See pJointBall::setAnchor().
	<BR>
	<SPAN CLASS="pin">Anchor Reference: </SPAN>A helper entity to transform a local anchor into the world space.
	<BR>
	<SPAN CLASS="pin">Swing Limt Axis: </SPAN>Limit axis defined in the joint space of body a. See pJointBall::setSwingLimitAxis().
	<BR>
	<SPAN CLASS="pin">Projection Mode: </SPAN>Joint projection mode. See pJointBall::setProjectionMode() and #ProjectionMode.
	<BR>
	<BR>
	<SPAN CLASS="pin">Projection Distance: </SPAN>If any joint projection is used, it is also necessary to set 
				projectionDistance to a small value greater than zero. When the joint error is larger than projectionDistance the SDK will change it so that the joint error is equal to projectionDistance. Setting  projectionDistance too 
				small will introduce unwanted oscillations into the simulation.
	<br>
	<SPAN CLASS="pin">Collision :</SPAN>Enable Collision. See pJointRevolute::enableCollision().
	<BR>
	<br>
	<SPAN CLASS="pin">Swing Limit: </SPAN>Swing limit of the twist axis.
	<BR>
	<br>
	<SPAN CLASS="pin">Twist High Limit: </SPAN>Higher rotation limit around twist axis. See pJointBall::setTwistHighLimit().
	<BR>
	<br>
	<SPAN CLASS="pin">Twist Low Limit: </SPAN>Lower rotation limit around twist axis. See pJointBall::setTwistLowLimit().
	<BR>
	<br>

	<SPAN CLASS="pin">Swing Spring: </SPAN>.The spring that works against swinging. See pJointBall::setSwingSpring().
	<BR>

	<SPAN CLASS="pin">Twist Spring: </SPAN>.The spring that works against twisting. See pJointBall::setTwistSpring().
	<BR>

	<SPAN CLASS="pin">Joint Spring: </SPAN>.The spring that lets the joint get pulled apart. See pJointBall::setJointSpring().
	<BR>
	<BR>
	<BR>


	<SPAN CLASS="setting">Swing Limit: </SPAN>Enables parameter input for swing limit.
	<BR>
	<SPAN CLASS="setting">Twist High Limit: </SPAN>Enables parameter input for high twist limit.
	<BR>
	<SPAN CLASS="setting">Twist Low Limit: </SPAN>Enables parameter input for low twist limit.
	<BR>
	<SPAN CLASS="setting">Swing Spring: </SPAN>.Enables parameter input for swing spring.
	<BR>
	<SPAN CLASS="setting">Twist Spring: </SPAN>.Enables parameter input for twist spring.
	<BR>
	<SPAN CLASS="setting">Joint Spring: </SPAN>.Enables parameter input for joint spring.
	<BR>

	<h4>Spherical Joint Limits</h4>
	
		Spherical joints allow limits that can approximate the physical limits of motion on a human arm. 
		It is possible to specify a cone which limits how far the arm can swing from a given axis. 
		In addition, a twist limit can be specified which controls how much the arm is allowed to twist around its own axis.

	\image html sphericalLimits.png

	NOTE: There are similar restrictions on the twist limits for spherical joints as there are for revolute joints.

	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>

	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
		\include PJSetBall.vsl
	</SPAN>
	<br>

	<h3>VSL : Limit Modification  </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJSetBallLimits.vsl
	</SPAN>
	<br>
	<br>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createJointBall().<br>

	*/
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( JSetBallCB );
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY,"0.0f");
	proto->DeclareInParameter("Limit Swing Axis",CKPGUID_VECTOR,"0.0f");

	proto->DeclareInParameter("Projection Mode",VTE_JOINT_PROJECTION_MODE,"0");
	proto->DeclareInParameter("Projection Distance",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Collision",CKPGUID_BOOL,"TRUE");



	proto->DeclareInParameter("Swing Limit",VTS_JLIMIT,"");
	proto->DeclareInParameter("Twist High Limit",VTS_JLIMIT);
	proto->DeclareInParameter("Twist Low Limit",VTS_JLIMIT);
	
	proto->DeclareInParameter("Swing Spring",VTS_JOINT_SPRING);
	proto->DeclareInParameter("Twist Spring",VTS_JOINT_SPRING);
	proto->DeclareInParameter("Joint Spring",VTS_JOINT_SPRING);

	

	proto->DeclareSetting("Swing Limit",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Twist Limit",CKPGUID_BOOL,"FALSE");


	proto->DeclareSetting("Swing Spring",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Twist Spring",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Joint Spring",CKPGUID_BOOL,"FALSE");



		
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(JSetBall);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JSetBall
// FullName:  JSetBall
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JSetBall(const CKBehaviorContext& behcontext)
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
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Spherical))
		{

			return CK_OK;
		}
		
		
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA && ! worldB )
		{
			return 0;
		}
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



		//anchor : 
		VxVector anchor  = GetInputParameterValue<VxVector>(beh,bI_Anchor);
		VxVector anchorOut  = anchor;
		CK3dEntity*anchorReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AnchorRef);

		if (anchorReference)
		{
			anchorReference->Transform(&anchorOut,&anchor);
		}

		//swing axis
		VxVector swingAxis  = GetInputParameterValue<VxVector>(beh,bbI_SwingAxis);

		ProjectionMode mode =GetInputParameterValue<ProjectionMode>(beh,bbI_PMode); 
		float distance = GetInputParameterValue<float>(beh,bbI_PDistance); 
		int coll  = GetInputParameterValue<int>(beh,bbI_Collision);
		


		//////////////////////////////////////////////////////////////////////////
		//swing limit : 
		pJointLimit sLimit;
		DWORD swingLimit;
		beh->GetLocalParameterValue(0,&swingLimit);
		if (swingLimit)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_SwingLimit);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					sLimit  = pFactory::Instance()->createLimitFromParameter(rPar);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//twist limit high : 
		pJointLimit tLimitH;
		DWORD twistLimit;
		beh->GetLocalParameterValue(1,&twistLimit);
		if (twistLimit)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_TwistHighLimit);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					tLimitH  = pFactory::Instance()->createLimitFromParameter(rPar);
					
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//twist limit low : 
		pJointLimit tLimitL;
		if (twistLimit)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_TwistLowLimit);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					tLimitL  = pFactory::Instance()->createLimitFromParameter(rPar);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		DWORD swingSpring;
		pSpring sSpring;  
		beh->GetLocalParameterValue(2,&swingSpring);
		if (swingSpring)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_SwingSpring);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					sSpring  = pFactory::Instance()->createSpringFromParameter(rPar);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		DWORD twistSpring;
		pSpring tSpring; 
		beh->GetLocalParameterValue(3,&twistSpring);
		if (twistSpring)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_TwistSpring);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					tSpring= pFactory::Instance()->createSpringFromParameter(rPar);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//
		DWORD jointSpring;
		pSpring jSpring;
		beh->GetLocalParameterValue(4,&jointSpring);
		if (jointSpring)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_JointSpring);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					  jSpring = pFactory::Instance()->createSpringFromParameter(rPar);
				}
			}
		}
		pJointBall *joint = static_cast<pJointBall*>(worldA->getJoint(target,targetB,JT_Spherical));
		if(bodyA || bodyB)
		{
			//////////////////////////////////////////////////////////////////////////
			//joint create ? 
			if (!joint)
			{
				joint   = static_cast<pJointBall*>(pFactory::Instance()->createBallJoint(target,targetB,anchorOut,swingAxis));
			}
			////////////////////////////////////////////////////////////////////////// Modification : 
			if (joint)
			{

				joint->setAnchor(anchorOut);
				joint->setSwingLimitAxis(swingAxis);
				joint->setProjectionMode(mode);
				joint->setProjectionDistance(distance);
				joint->enableCollision(coll);
		
				
				//////////////////////////////////////////////////////////////////////////
				if(swingLimit)
				{
					joint->setSwingLimit(sLimit);					
				}
				joint->enableFlag(NX_SJF_SWING_LIMIT_ENABLED,swingLimit);
				
				//////////////////////////////////////////////////////////////////////////
				if (twistLimit)
				{
					joint->setTwistHighLimit(tLimitH);
					joint->setTwistLowLimit(tLimitL);
				}
				joint->enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,twistLimit);
				
				//////////////////////////////////////////////////////////////////////////
				if (swingSpring)
				{
					joint->setSwingSpring(sSpring);
				}
				joint->enableFlag(NX_SJF_SWING_SPRING_ENABLED,swingSpring);
				//////////////////////////////////////////////////////////////////////////
				if (twistSpring)
				{
					joint->setTwistSpring(tSpring);
				}
				joint->enableFlag(NX_SJF_TWIST_SPRING_ENABLED,twistSpring);
				//////////////////////////////////////////////////////////////////////////
				if (jointSpring)
				{
					joint->setJointSpring(jSpring);
				}
				joint->enableFlag(NX_SJF_JOINT_SPRING_ENABLED,jointSpring);
				
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JSetBallCB
// FullName:  JSetBallCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JSetBallCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
			DWORD swingLimit;
			beh->GetLocalParameterValue(0,&swingLimit);
			beh->EnableInputParameter(bbI_SwingLimit,swingLimit);
	
			DWORD twistLimit;
			beh->GetLocalParameterValue(1,&twistLimit);
			beh->EnableInputParameter(bbI_TwistHighLimit,twistLimit);
			beh->EnableInputParameter(bbI_TwistLowLimit,twistLimit);

			DWORD springSwing;
			beh->GetLocalParameterValue(2,&springSwing);
			beh->EnableInputParameter(bbI_SwingSpring,springSwing);

			DWORD twistSwing;
			beh->GetLocalParameterValue(3,&twistSwing);
			beh->EnableInputParameter(bbI_TwistSpring,twistSwing);

			DWORD jointSwing;
			beh->GetLocalParameterValue(4,&jointSwing);
			beh->EnableInputParameter(bbI_JointSpring,jointSwing);

		break;
	}
	}
	return CKBR_OK;
}