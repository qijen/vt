#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorJD6SetSoftLimitDecl();
CKERROR CreateJD6SetSoftLimitProto(CKBehaviorPrototype **pproto);
int JD6SetSoftLimit(const CKBehaviorContext& behcontext);
CKERROR JD6SetSoftLimitCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyA=0,
	bbI_BodyB,
	bbI_Anchor,
	bbI_AnchorRef,
	bbI_Axis,
	bbI_AxisRef
};

//************************************
// Method:    FillBehaviorJD6SetSoftLimitDecl
// FullName:  FillBehaviorJD6SetSoftLimitDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJD6SetSoftLimitDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJD6SetSoftLimit");	
	od->SetCategory("Physic/D6");
	od->SetDescription("Sets the soft limits in a D6 joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x536d5df9,0x4e5b275c));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJD6SetSoftLimitProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJD6SetSoftLimitProto
// FullName:  CreateJD6SetSoftLimitProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJD6SetSoftLimitProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJD6SetSoftLimit");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	/*! \page PJD6SetSoftLimit


	<br>
	PJD6SetSoftLimit is categorized in \ref D6
	<br>
	<br>See <A HREF="PJD6.cmo">PJD6.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies D6 joint limits.
	<br>
	<br>
	<h3>Technical Information</h3>

	\image html PJD6SetSoftLimit.png


	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body to identfy the joint.
	<BR>
	<BR>

	<SPAN CLASS="pin">Limit Axis: </SPAN>The target limit axis. See #D6LimitAxis.
	<BR>
	<SPAN CLASS="pin">Soft Limit: </SPAN>The new limit. See #pJD6SoftLimit. 
	<BR>
	<BR>

	<hr>

	See \ref D6AngularLimitGuide "Angular Limits"<br>

	See \ref D6LinearLimitGuide "Linear Limits"<br>
	
		*/

	
	
	proto->SetBehaviorCallbackFct( JD6SetSoftLimitCB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Limit Axis",VTE_JOINT_LIMIT_AXIS,0);

	proto->DeclareInParameter("Soft Limit",VTS_JOINT_SLIMIT);

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JD6SetSoftLimit);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JD6SetSoftLimit
// FullName:  JD6SetSoftLimit
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JD6SetSoftLimit(const CKBehaviorContext& behcontext)
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
			D6LimitAxis limitAxis = GetInputParameterValue<D6LimitAxis>(beh,1); 
			pJD6SoftLimit softLimit;

			CKParameter* pout = beh->GetInputParameter(2)->GetRealSource();
			CK_ID* ids = (CK_ID*)pout->GetReadDataPtr();
			float damping,spring,value,restitution;
			pout = (CKParameterOut*)ctx->GetObject(ids[0]);
			pout->GetValue(&damping);

			pout = (CKParameterOut*)ctx->GetObject(ids[1]);
			pout->GetValue(&spring);
			pout = (CKParameterOut*)ctx->GetObject(ids[2]);
			pout->GetValue(&value);
			pout = (CKParameterOut*)ctx->GetObject(ids[3]);
			pout->GetValue(&restitution);

			softLimit.damping = damping;
			softLimit.spring = spring;
			softLimit.value = value;
			softLimit.restitution = restitution;


			
			if (joint)
			{
				switch(limitAxis)
				{
					case D6LA_Linear:
						joint->setLinearLimit(softLimit);
					break;
					case D6LA_Swing1:
						joint->setSwing1Limit(softLimit);
					break;
					case D6LA_Swing2:
						joint->setSwing2Limit(softLimit);
					break;
					case D6LA_TwistHigh:
						joint->setTwistHighLimit(softLimit);
					break;
					case D6LA_TwistLow:
						joint->setTwistLowLimit(softLimit);
					break;
					
				}
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JD6SetSoftLimitCB
// FullName:  JD6SetSoftLimitCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JD6SetSoftLimitCB(const CKBehaviorContext& behcontext)
{
	return CKBR_OK;
}

