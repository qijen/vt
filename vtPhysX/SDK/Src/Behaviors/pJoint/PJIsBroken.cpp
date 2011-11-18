#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJIsBrokenDecl();
CKERROR CreatePJIsBrokenProto(CKBehaviorPrototype **pproto);
int PJIsBroken(const CKBehaviorContext& behcontext);
CKERROR PJIsBrokenCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyB,
	bbI_Type,
};

//************************************
// Method:    FillBehaviorPJIsBrokenDecl
// FullName:  FillBehaviorPJIsBrokenDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJIsBrokenDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJIsBroken");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Triggers when joint has been broken.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x17082b70,0x6eae5b38));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJIsBrokenProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePJIsBrokenProto
// FullName:  CreatePJIsBrokenProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJIsBrokenProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJIsBroken");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJIsBroken  

	<br>
	PJIsBroken is categorized in \ref Joints
	<br>
	

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Activates output trigger when a joint has been broken.

	<br>

	<h3>Technical Information</h3>

	\image html PJIsBroken.png


	<SPAN CLASS="in">In: </SPAN> triggers the process.
	<BR>
	<BR>
	
	<SPAN CLASS="out">Exit On: </SPAN> is activated when the building block has been started.
	<BR>

	<SPAN CLASS="out">Exit Off: </SPAN> is activated when the process is completed.
	<BR>

	<SPAN CLASS="out">Broken: </SPAN> is activated when the joint has been broken.
	<BR>
	<BR>

	<SPAN CLASS="pin">Body B: </SPAN> The second body. Leave blank to create a joint constraint with the world. 
	<BR>

	<SPAN CLASS="pin">Joint Type:</SPAN> The joint type. This helps the building block to identify a joint constraint. As usual there can be only one joint of the type x between two bodies.
	<BR>
	<BR>

	<SPAN CLASS="pout">Breaking Impulse:</SPAN> The impulse which broke the joint. This is clamped to the values of maxForce (or maxTorque, depending on what made the joint break) that was specified for the joint.
	<BR>

	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>
	*/

	proto->DeclareInput("On");
	proto->DeclareInput("Off");
	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("Broken");
	proto->DeclareOutParameter("Break Impulse",CKPGUID_FLOAT);


	proto->SetBehaviorCallbackFct( PJIsBrokenCB );

	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Joint Type",VTE_JOINT_TYPE,"0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PJIsBroken);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJIsBroken
// FullName:  PJIsBroken
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJIsBroken(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;

	if( beh->IsInputActive(1) )
	{
		beh->ActivateInput(1,FALSE);
		return CK_OK;
	}

	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);
	}


	//////////////////////////////////////////////////////////////////////////
	//the object A: 
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(bbI_BodyB);
	
	int jointType = GetInputParameterValue<int>(beh,bbI_Type); 

	if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,jointType))
	{

		return CKBR_ACTIVATENEXTFRAME;
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
	int size = GetPMan()->getJointFeedbackList().Size();

	if(bodyA || bodyB)
	{
		for (int i = 0 ; i < GetPMan()->getJointFeedbackList().Size(); i++ )
		{
			pBrokenJointEntry *entry = *GetPMan()->getJointFeedbackList().At(i);
			if (entry)
			{
				
				if ( entry->jType ==jointType )
				{
					CK3dEntity *bodyAEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mAEnt));
					CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mBEnt));

					if ( bodyAEnt == target && bodyBEnt ==targetB)
					{
						beh->ActivateOutput(2);
						SetOutputParameterValue<float>(beh,0,entry->impulse);
						GetPMan()->getJointFeedbackList().EraseAt(i);
						return CKBR_ACTIVATENEXTFRAME;
					}

					if (bodyBEnt == targetB && bodyAEnt==target)
					{
						beh->ActivateOutput(2);
						SetOutputParameterValue<float>(beh,0,entry->impulse);
						GetPMan()->getJointFeedbackList().EraseAt(i);
						return CKBR_ACTIVATENEXTFRAME;
					}
				}
			}
		}
	}
	return CKBR_ACTIVATENEXTFRAME;
}
//************************************
// Method:    PJIsBrokenCB
// FullName:  PJIsBrokenCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJIsBrokenCB(const CKBehaviorContext& behcontext)
{
	return CKBR_OK;
}

