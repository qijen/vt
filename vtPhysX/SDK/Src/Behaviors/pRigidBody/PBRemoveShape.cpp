#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPBRemoveShapeDecl();
CKERROR CreatePBRemoveShapeProto(CKBehaviorPrototype **pproto);
int PBRemoveShape(const CKBehaviorContext& behcontext);
CKERROR PBRemoveShapeCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPBRemoveShapeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBRemoveShape");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Removes a sub shape given by a mesh.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x25637ead,0x3a881190));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBRemoveShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBRemoveShapeProto
// FullName:  CreatePBRemoveShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************

enum bInputs
{
	bbI_Mesh=0,
	bbI_Density,
	bbI_TotalMass,
};

CKERROR CreatePBRemoveShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBRemoveShape");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PBRemoveShape  

	PBRemoveShape is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Removes a sub shape. See also pRigidBody::addSubShape()	.<br>
	
	<h3>Technical Information</h3>

	\image html PBRemoveShape.png


	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>


	<SPAN CLASS="pin">Target:</SPAN>The 3D Entity associated to the rigid body.<BR>

	<SPAN CLASS="pin">Mesh Reference:</SPAN>The mesh reference. Must be a Entity3D or a Mesh.<BR>

	<br>
	<SPAN CLASS="pin">New Density: </SPAN>Density scale factor of the shapes belonging to the body.If you supply a non-zero total mass, 
	the bodies mass and inertia will first be computed as	above and then scaled to fit this total mass. See #pRigidBody::updateMassFromShapes().
	<br>
	<br>
	<SPAN CLASS="pin">Total Mass: </SPAN>Total mass if it has sub shapes.If you supply a non-zero density, 
	the bodies mass and inertia will first be computed as above and then scaled by this factor.See #pRigidBody::updateMassFromShapes().
	<br>

	<BR>
	<h3>Note</h3>

	The mesh reference can NOT be the initial shape of the target body.

	*/
	
	proto->DeclareInParameter("Reference",CKPGUID_BEOBJECT);
	proto->DeclareInParameter("Density",CKPGUID_FLOAT);
	proto->DeclareInParameter("Total Mass",CKPGUID_FLOAT);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBRemoveShape);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBRemoveShape
// FullName:  PBRemoveShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBRemoveShape(const CKBehaviorContext& behcontext)
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
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world)
		{
			beh->ActivateOutput(0);
			return 0;

		}
		pRigidBody*result = world->getBody(target);
		if(!result)
		{
			beh->ActivateOutput(0);
			return 0;
		}
		
		CKMesh *mesh  = (CKMesh*)GetInputParameterValue<CKObject*>(beh,bbI_Mesh);
		float density = GetInputParameterValue<float>(beh,bbI_Density);
		float totalMass = GetInputParameterValue<float>(beh,bbI_TotalMass);

		result->removeSubShape(mesh,density,totalMass);
		
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBRemoveShapeCB
// FullName:  PBRemoveShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBRemoveShapeCB(const CKBehaviorContext& behcontext)
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

