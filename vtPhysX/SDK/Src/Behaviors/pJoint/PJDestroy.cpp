#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorJDestroyDecl();
CKERROR CreateJDestroyProto(CKBehaviorPrototype **pproto);
int JDestroy(const CKBehaviorContext& behcontext);
CKERROR JDestroyCB(const CKBehaviorContext& behcontext);



//************************************
// Method:    FillBehaviorJDestroyDecl
// FullName:  FillBehaviorJDestroyDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJDestroyDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJDestroy");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Destroys a joint given by two bodies and the joint type.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x42cd6243,0x5f7b18af));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJDestroyProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreateJDestroyProto
// FullName:  CreateJDestroyProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJDestroyProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJDestroy");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJDestroy  

	<br>
	PJDestroy is categorized in \ref Joints
	<br>
	<br>See <A HREF="PJDestroy.cmo">PJDestroy.cmo</A>.

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Deletes a joint, given by references and type.

	<br>

	<h3>Technical Information</h3>

	\image html PJDestroy.png


	<SPAN CLASS="in">In: </SPAN> triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN> is activated when the process is completed.
	<BR>

	<SPAN CLASS="pin">Body B: </SPAN> The second body. Leave blank to create a joint constraint with the world. 
	<BR>

	<SPAN CLASS="pin">Joint Type:</SPAN> The joint type. This helps the building block to identify a joint constraint. As usual there can be only one joint of the type x between two bodies.
	<BR>

	<b>Sleeping:</b> This call wakes the actor(s) if they are sleeping.

	Is utilizing #pRigidBody #pWorld #PhysicManager #pWorld::deleteJoint().<br>
	*/

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->SetBehaviorCallbackFct( JDestroyCB );
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Type",VTE_JOINT_TYPE,"");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(JDestroy);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JDestroy
// FullName:  JDestroy
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JDestroy(const CKBehaviorContext& behcontext)
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
	
		int jointType = GetInputParameterValue<int>(beh,1); 

		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,jointType))
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

			pJoint *joint = worldA->getJoint(target,targetB,(JType)jointType);
			if (joint)
			{
				worldA->deleteJoint(joint);
			}
		}

		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JDestroyCB
// FullName:  JDestroyCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JDestroyCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {

	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			
		}
		break;
	}
	return CKBR_OK;
}
