#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJCylindricalDecl();
CKERROR CreatePJCylindricalProto(CKBehaviorPrototype **pproto);
int PJCylindrical(const CKBehaviorContext& behcontext);
CKERROR PJCylindricalCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Anchor,
	bI_AnchorRef,
	bI_Axis,
	bI_AxisRef,
	bI_Coll
};

//************************************
// Method:    FillBehaviorPJCylindricalDecl
// FullName:  FillBehaviorPJCylindricalDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJCylindricalDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJCylindrical");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets/modifies a cylindrical joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x54821791,0x38290804));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJCylindricalProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJCylindricalProto
// FullName:  CreatePJCylindricalProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJCylindricalProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJCylindrical");
	if(!proto) return CKERR_OUTOFMEMORY;


	/*! \page PJCylindrical  

	<br>
	PJCylindrical is categorized in \ref Joints
	<br>

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a cylindrical joint between a two bodies or the world.
	<br>
	<br>See <A HREF="PJCylindrical.cmo">PJCylindrical.cmo</A>.


	\image html cylinderJoint.png


	<h3>Technical Information</h3>

	\image html PJCylindrical.png

	A cylindrical joint permits both relative translational and rotational movement between two bodies along a single axis (i.e., the bodies are allowed to both slide and twist along the axis of the joint). It is usually necessary to add joint limits to prevent the bodies from getting too far from each other along the joint axis. If the distance becomes too great, then the SDK can have difficulty maintaining the joint constraints (see Joint Limits).

	An example for a cylindrical joint is a telescopic radio antenna. Another example is a piston on a machine such as a steam engine.

	DOFs removed: 4
	DOFs remaining: 2


	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<SPAN CLASS="pin">Anchor:</SPAN>A point in world space coordinates. See pJointCylindrical::setGlobalAnchor().
	<BR>
	<SPAN CLASS="pin">Anchor Reference: </SPAN>A helper entity to transform a local anchor into the world space.
	<BR>
	<SPAN CLASS="pin">Axis: </SPAN>An in world space. See pJointCylindrical::setGlobalAxis().
	<BR>
	<SPAN CLASS="pin">Axis Up Reference: </SPAN>A helper entity to transform a local axis into the world space.
	<BR>
	<SPAN CLASS="pin">Collision: </SPAN>Enables Collision. See pJointCylindrical::enableCollision().
	<BR>



	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include pJCylindrical.vsl
	</SPAN>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createJointCylindrical().<br>

	*/


	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( PJCylindricalCB );
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR);
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY);
	
	proto->DeclareInParameter("Axis",CKPGUID_VECTOR);
	proto->DeclareInParameter("Axis Up Reference",CKPGUID_3DENTITY);
	
	proto->DeclareInParameter("Collision",CKPGUID_BOOL);
	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJCylindrical);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJCylindrical
// FullName:  PJCylindrical
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJCylindrical(const CKBehaviorContext& behcontext)
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
			bbErrorME("Couldnt find any world object");

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
		int col  = GetInputParameterValue<int>(beh,bI_Coll);
		//////////////////////////////////////////////////////////////////////////
		//
		pJointCylindrical *joint = static_cast<pJointCylindrical*>(worldA->getJoint(target,targetB,JT_Cylindrical));
		if(bodyA || bodyB)
		{
			//////////////////////////////////////////////////////////////////////////
			//joint create ? 
			if (!joint)
			{
				joint   = static_cast<pJointCylindrical*>(pFactory::Instance()->createCylindricalJoint(target,targetB,anchorOut,axisOut));
			}
			////////////////////////////////////////////////////////////////////////// Modification : 
			if (joint)
			{

				joint->setGlobalAxis(axisOut);
				joint->setGlobalAnchor(anchorOut);
				joint->enableCollision(col);
				
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJCylindricalCB
// FullName:  PJCylindricalCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJCylindricalCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
/*		
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
*/
		break;
	}
	}
	return CKBR_OK;
}