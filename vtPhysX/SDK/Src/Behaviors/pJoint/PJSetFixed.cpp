#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorJSetFixedDecl();
CKERROR CreateJSetFixedProto(CKBehaviorPrototype **pproto);
int JSetFixed(const CKBehaviorContext& behcontext);
CKERROR JSetFixedCB(const CKBehaviorContext& behcontext);



//************************************
// Method:    FillBehaviorJSetFixedDecl
// FullName:  FillBehaviorJSetFixedDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJSetFixedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJFixed");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Creates a Fixed Joint between world('None') or an entity.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2d80b0a,0x3d5a0caf));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJSetFixedProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJSetFixedProto
// FullName:  CreateJSetFixedProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJSetFixedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJFixed");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PJFixed  

	<br>
	PJFixed is categorized in \ref Joints
	<br>

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates a fixed joint between a two bodies or the world.
	<br>
	<br>

	
	\image html fixedJoint.png

	

	<h3>Technical Information</h3>

	\image html PJFixed.png
	
	The fixed joint effectively glues two bodies together with no remaining degrees of freedom for relative motion. It is useful to set it to be breakable (see Breakable Joint) to simulate simple fracture effects.

	An example for a fixed joint is a factory chimney divided into sections, each section held together with fixed joints. When the chimney is hit, it will break apart and topple over by sections rather than unrealistically falling over in one piece.

	DOFs removed: 6
	DOFs remaining: 0

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<BR>


	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include pJFixed.vsl
	</SPAN>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createJointFixed().<br>

	*/


	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JSetFixed);
	proto->SetBehaviorCallbackFct( JSetFixedCB);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JSetFixed
// FullName:  JSetFixed
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JSetFixed(const CKBehaviorContext& behcontext)
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
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Fixed))
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

			pJointFixed*joint =static_cast<pJointFixed*>(worldA->getJoint(target,targetB,JT_Fixed));
			
			if (!joint)
			{
				joint   = static_cast<pJointFixed*>(pFactory::Instance()->createFixedJoint(target,targetB));
			}
			beh->ActivateOutput(0);
		}
	}
	return 0;
}
//************************************
// Method:    JSetFixedCB
// FullName:  JSetFixedCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JSetFixedCB(const CKBehaviorContext& behcontext)
{

	return CKBR_OK;

}