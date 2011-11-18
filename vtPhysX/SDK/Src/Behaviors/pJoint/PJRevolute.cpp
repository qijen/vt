#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJRevoluteDecl();
CKERROR CreatePJRevoluteProto(CKBehaviorPrototype **pproto);
int PJRevolute(const CKBehaviorContext& behcontext);
CKERROR PJRevoluteCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Anchor,
	bI_AnchorRef,
	bI_Axis,
	bI_AxisRef,
	bbI_Collision,
	bbI_PMode,
	bbI_PDistance,
	bbI_PAngle,
	bbI_Spring,
	bbI_HighLimit,
	bbI_LowLimit,
	bbI_Motor
};

//************************************
// Method:    FillBehaviorPJRevoluteDecl
// FullName:  FillBehaviorPJRevoluteDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJRevoluteDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJRevolute");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets/modifies a revolute joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x77cb361c,0x670112a9));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJRevoluteProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJRevoluteProto
// FullName:  CreatePJRevoluteProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJRevoluteProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJRevolute");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJRevolute  

	<br>
	PJRevolute is categorized in \ref Joints
	<br>
	<br>See <A HREF="PJRevolute.cmo">PJRevolute.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a revolute joint.
	<br>
	<br>
	DOFs removed: 5<br>
	DOFs remaining: 1<br>
	<br>
	\image html revoluteJoint.png

	A revolute joint removes all but a single rotational degree of freedom from two objects. 
	The axis along which the two bodies may rotate is specified with a point and a direction 
	vector. In theory, the point along the direction vector does not matter, but in practice, 
	it should be near the area where the bodies are closest to improve simulation stability.

	An example for a revolute joint is a door hinge. 
	Another example would be using a revolute joint to attach rotating fan blades to a 
	ceiling. 	The revolute joint could be motorized, causing the fan to rotate.

	<h3>Technical Information</h3>

	\image html PJRevolute.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Anchor:</SPAN>A point in world space coordinates. See pJointRevolute::setGlobalAnchor().
	<BR>
	<SPAN CLASS="pin">Anchor Reference: </SPAN>A helper entity to transform a local anchor into the world space.
	<BR>
	
	<SPAN CLASS="pin">Axis: </SPAN>An in world space. See pJointRevolute::setGlobalAxis().
	<BR>
	<SPAN CLASS="pin">Axis Up Reference: </SPAN>A helper entity to transform a local axis into the world space.
	<BR>
	<BR>
	<hr>

	<SPAN CLASS="pin">Collision: </SPAN>Enable Collision. See pJointRevolute::enableCollision().
	<BR>

	<hr>
	<SPAN CLASS="pin">Projection Mode: </SPAN>Joint projection mode. See pJointRevolute::setProjectionMode() and #ProjectionMode.
	<BR>

	<SPAN CLASS="pin">Projection Distance: </SPAN>If any joint projection is used, it is also necessary to set 
	projectionDistance to a small value greater than zero. When the joint error is larger than projectionDistance the SDK will change it so that the joint error is equal to projectionDistance. Setting  projectionDistance too 
	small will introduce unwanted oscillations into the simulation.See pJointRevolute::setProjectionDistance().
	<br>

	<SPAN CLASS="pin">Projection Angle: </SPAN>Angle must be greater than 0.02f .If its smaller then current algo gets too close to a singularity. See pJointRevolute::setProjectionAngle().
	<BR>


	<hr>
	<SPAN CLASS="pin">Spring: </SPAN>Make it springy.See pJointRevolute::setSpring().
	<BR>

	<hr>
	<SPAN CLASS="pin">High Limit: </SPAN>Higher rotation limit around the rotation axis. See pJointRevolute::setHighLimit().
	<BR>
	
	<SPAN CLASS="pin">Low Limit: </SPAN>Lower rotation limit around rotation axis. See pJointRevolute::setLowLimit().
	<BR>
	
	<hr>
	<SPAN CLASS="pin">Motor: </SPAN>Motor parameters. See pJointRevolute::setMotor().
	<BR>
	<BR>

	<hr>

	<SPAN CLASS="setting">Spring: </SPAN>Enables parameter input for spring settings.
	<BR>

	<SPAN CLASS="setting">High Limit: </SPAN>Enables parameter inputs for angular limits.
	<BR>
	
	
	<SPAN CLASS="setting">Motor: </SPAN>Enables parameter input for motor settings.
	<BR>
	


	<h4>Revolute Joint Limits</h4>

	A revolute joint allows limits to be placed on how far it rotates around the joint axis. For example, a hinge on a door cannot rotate through 360 degrees; rather, it can rotate between 20 degrees and 180 degrees.
	The angle of rotation is measured using the joints normal (axis orthogonal to the joints axis). This is the angle reported by NxRevoluteJoint::getAngle(). The limits are specified as a high and low limit, which must satisfy the condition -Pi < low < high <Pi degrees.
	Below are valid revolute joint limits in which the joint is able to move between low and high:

	\image html revoluteJointLimits.png
	<br>

	Note : The white region represents the allowable rotation for the joint.

	<h4>Limitations of Revolute Joint Limits</h4>
	As shown below, it is not possible to specify certain limit configurations without rotating the joint axes, due to the restrictions on the values of low and high: 
	\image html revoluteLimitLimitation.png

	To achieve this configuration, it is necessary to rotate the joint counter-clockwise so that low is below the 180 degree line. 

	NOTE: If the angular region that is prohibited by the twist limit (as in the above figures) is very small, only a few degrees or so, then the joint may "push through" the limit and out on the other side if the relative angular velocity is large enough in relation to the time step. Care must be taken to make sure the limit is "thick" enough for the typical angular velocities it will be subjected to.


	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>

	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJRevolute.vsl
	</SPAN>
	<br>

	<h3>VSL : Limit Modification  </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJRevoluteSetLimits.vsl
	</SPAN>
	<br>

	<h3>VSL : Motor Modification  </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJRevoluteSetMotor.vsl
	</SPAN>
	<br>
	<br>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createJointRevolute().<br>

	*/
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( PJRevoluteCB );
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY,"0.0f");
	
	proto->DeclareInParameter("Axis",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Axis Up Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Collision",CKPGUID_BOOL);
	proto->DeclareInParameter("Projection Mode",VTE_JOINT_PROJECTION_MODE,"0");
	proto->DeclareInParameter("Projection Distance",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Projection Angle",CKPGUID_FLOAT,"0.025");
	
	proto->DeclareInParameter("Spring",VTS_JOINT_SPRING);
	proto->DeclareInParameter("High Limit",VTS_JLIMIT);
	proto->DeclareInParameter("Low Limit",VTS_JLIMIT);
	proto->DeclareInParameter("Motor",VTS_JOINT_MOTOR);


	proto->DeclareSetting("Spring",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Limit",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Motor",CKPGUID_BOOL,"FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJRevolute);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJRevolute
// FullName:  PJRevolute
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJRevolute(const CKBehaviorContext& behcontext)
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
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(bI_ObjectB);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Revolute))
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
		VxVector Axis  = GetInputParameterValue<VxVector>(beh,bI_Axis);
		VxVector axisOut  = Axis;
		CK3dEntity*axisReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AxisRef);

		if (axisReference)
		{
			VxVector dir,up,right;
			axisReference->GetOrientation(&dir,&up,&right);
			axisReference->TransformVector(&axisOut,&up);
		}

		//////////////////////////////////////////////////////////////////////////
		//limit high : 
		pJointLimit limitH;
		pJointLimit limitL;
		DWORD limit;
		beh->GetLocalParameterValue(1,&limit);
		if (limit)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_HighLimit);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					limitH  = pFactory::Instance()->createLimitFromParameter(rPar);
				}
			}
		}
		
		if (limit)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_LowLimit);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					limitL  = pFactory::Instance()->createLimitFromParameter(rPar);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		DWORD spring;
		pSpring sSpring;  
		beh->GetLocalParameterValue(0,&spring);
		if (spring)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_Spring);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					sSpring  = pFactory::Instance()->createSpringFromParameter(rPar);
				}
			}
		}

		pMotor motor;

		DWORD hasMotor;beh->GetLocalParameterValue(2,&hasMotor);
		if (hasMotor)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_Motor);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					motor  = pFactory::Instance()->createMotorFromParameter(rPar);
				}
			}
		}

		
		int col  = GetInputParameterValue<int>(beh,bbI_Collision);
		ProjectionMode mode =GetInputParameterValue<ProjectionMode>(beh,bbI_PMode); 
		float distance = GetInputParameterValue<float>(beh,bbI_PDistance); 
		float angle= GetInputParameterValue<float>(beh,bbI_PAngle); 

		//////////////////////////////////////////////////////////////////////////
		//
		pJointRevolute *joint = static_cast<pJointRevolute*>(worldA->getJoint(target,targetB,JT_Revolute));
		if(bodyA || bodyB)
		{
			//////////////////////////////////////////////////////////////////////////
			//joint create ? 
			if (!joint)
			{
				joint   = static_cast<pJointRevolute*>(pFactory::Instance()->createRevoluteJoint(target,targetB,anchorOut,axisOut));
			}
			////////////////////////////////////////////////////////////////////////// Modification : 
			if (joint)
			{

				joint->setGlobalAxis(axisOut);
				joint->setGlobalAnchor(anchorOut);
				
				if (mode!=0)
				{
					joint->setProjectionMode(mode);
					joint->setProjectionDistance(distance);
					joint->setProjectionAngle(angle);
				}
				
				//////////////////////////////////////////////////////////////////////////
				if(limit)
				{
					joint->setHighLimit(limitH);					
					joint->setLowLimit(limitL);					
				}
				
				//////////////////////////////////////////////////////////////////////////
				if (spring)
				{
					joint->setSpring(sSpring);
				}

				if (hasMotor)
				{
					joint->setMotor(motor);
				}
				
				joint->enableCollision(col);
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJRevoluteCB
// FullName:  PJRevoluteCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJRevoluteCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
		
			DWORD twistLimit;
			beh->GetLocalParameterValue(1,&twistLimit);
			beh->EnableInputParameter(bbI_HighLimit,twistLimit);
			beh->EnableInputParameter(bbI_LowLimit,twistLimit);

			DWORD springSwing;
			beh->GetLocalParameterValue(0,&springSwing);
			beh->EnableInputParameter(bbI_Spring,springSwing);

			DWORD motor;
			beh->GetLocalParameterValue(2,&motor);
			beh->EnableInputParameter(bbI_Motor,motor);

		break;
	}
	}
	return CKBR_OK;
}