#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJPointPlaneDecl();
CKERROR CreatePJPointPlaneProto(CKBehaviorPrototype **pproto);
int PJPointPlane(const CKBehaviorContext& behcontext);
CKERROR PJPointPlaneCB(const CKBehaviorContext& behcontext);

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
// Method:    FillBehaviorPJPointPlaneDecl
// FullName:  FillBehaviorPJPointPlaneDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJPointPlaneDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJPointInPlane");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets/modifies a point in plane joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6b7f6a11,0x4cb565b8));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJPointPlaneProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJPointPlaneProto
// FullName:  CreatePJPointPlaneProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJPointPlaneProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJPointInPlane");
	if(!proto) return CKERR_OUTOFMEMORY;


	/*! \page PJPointInPlane  

	<br>
	PJPointPlane is categorized in \ref Joints
	<br>

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a cylindrical joint between a two bodies or the world.
	<br>
	<br>See <A HREF="PJPointInPlane.cmo">PJPointInPlane.cmo</A>.


	\image html pointInPlaneJoint.png



	<h3>Technical Information</h3>

	\image html PJPointPlane.png

	A point in plane joint constrains a point on one actor to only move inside a plane attached to another actor. The point attached to the plane is defined by the anchor point. The joint's axis specifies the plane normal.

	An example for a point in plane joint is a magnet on a refrigerator.

	DOFs removed: 1
	DOFs remaining: 5



	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>
	<SPAN CLASS="pin">Anchor:</SPAN>A point in world space coordinates. See pJointPointInPlane::setGlobalAnchor().
	<BR>
	<SPAN CLASS="pin">Anchor Reference: </SPAN>A helper entity to transform a local anchor into the world space.
	<BR>
	<SPAN CLASS="pin">Axis: </SPAN>An in world space. See pJointPointInPlane::setGlobalAxis().
	<BR>
	<SPAN CLASS="pin">Axis Up Reference: </SPAN>A helper entity to transform a local axis into the world space.
	<BR>
	<SPAN CLASS="pin">Collision: </SPAN>Enables Collision. See pJointPointInPlane::enableCollision().
	<BR>



	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
	\include PJPointInPlane.vsl
	</SPAN>
	<br>

	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createPointInPlaneJoint().<br>

	*/


	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( PJPointPlaneCB );
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR);
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY);
	
	proto->DeclareInParameter("Axis",CKPGUID_VECTOR);
	proto->DeclareInParameter("Axis Up Reference",CKPGUID_3DENTITY);
	
	proto->DeclareInParameter("Collision",CKPGUID_BOOL);
	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJPointPlane);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJPointPlane
// FullName:  PJPointPlane
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJPointPlane(const CKBehaviorContext& behcontext)
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
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_PointInPlane))
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
		pJointPointInPlane *joint = static_cast<pJointPointInPlane*>(GetPMan()->getJoint(target,targetB,JT_PointInPlane));
		if(bodyA || bodyB)
		{
			//////////////////////////////////////////////////////////////////////////
			//joint create ? 
			if (!joint)
			{
				joint   = static_cast<pJointPointInPlane*>(pFactory::Instance()->createPointInPlaneJoint(target,targetB,anchorOut,axisOut));
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
// Method:    PJPointPlaneCB
// FullName:  PJPointPlaneCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJPointPlaneCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			break;
		}
	}
	return CKBR_OK;
}