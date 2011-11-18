#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBAddShapeDecl();
CKERROR CreatePBAddShapeProto(CKBehaviorPrototype **pproto);
int PBAddShape(const CKBehaviorContext& behcontext);
CKERROR PBAddShapeCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPBAddShapeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBAddShape");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Adds a sub shape given by a mesh or a reference object.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x55c61f90,0x2e512638));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBAddShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBAddShapeProto
// FullName:  CreatePBAddShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************

enum bInputs
{
	bbI_Mesh=0,
	bbI_PObject,
	bbI_Pos,
	bbI_Rot,
	bbI_Ref,
	bbI_Density,
	bbI_TotalMass,
};

CKERROR CreatePBAddShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBAddShape");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*
	PBAddShape  
	PBAddShape is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Adds a sub shape. See also pRigidBody::addSubShape()
	.<br>
	See <A HREF="PBAddShape.cmo">PBAddShape.cmo</A> for example.
	<h3>Technical Information</h3>

	\image html PBAddShape.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>


	<SPAN CLASS="pin">Target:</SPAN>The 3D Entity associated to the rigid body.<BR>

	<SPAN CLASS="pin">Mesh:</SPAN>The mesh reference. If null then it’s using the current mesh of the reference 3D-entity object.<BR>
	
	<SPAN CLASS="pin">Physic Properties: </SPAN>The hull type, new density and/or total mass are used only. You can overwrite these values if the source mesh or the reference object has a physic attribute.<br>

	<SPAN CLASS="pin">Local Position:</SPAN> Local position in the bodies’ space. This parameter is used when 

	mesh !=null AND reference 3D-entity = null <br>


	<SPAN CLASS="pin">Local Orientation: </SPAN>Local rotation in the bodies’ space. This parameter is used when :

	Mesh !=null AND reference 3D-entity = null <br>

	<SPAN CLASS="pin">Reference: </SPAN>If mesh != null and reference !=null ,then its adding the shape and using the reference only as transformation helper.

	If mesh = null and reference !=null, then its adding reference’s current mesh. In that, the function will try to get the hull type from objects attribute. If there is no attribute, then it’s using the building blocks physic properties parameter.


	<BR>
	<h3>Note</h3>
	
	<b>Physic Material Lookup: (it’s looking for the physic material attribute on the objects listed below)</b>
	
	- Materials lookup order for the case <b>reference is null </b>AND <b>mesh is not null</b> : 
		- mesh 
		- mesh’s material (index = 0) 

	- Materials lookup order for the case <b>mesh is null</b> AND <b>reference is not null </b> : 
		-	reference object 
		-	reference’s  current mesh’s material (index = 0 ) 

	- Materials lookup order for the case <b>mesh is not null </b>AND <b>reference is not null</b>: 
		-	mesh 
		-	mesh’s material (index = 0) 
		-	reference’s  current mesh’s material (index = 0 ) 


	When it couldn’t find any material attribute until now, it’s using the target’s body material. This material is chosen or even created during the registration (\ref PBPhysicalize).
	Here the lookup order during the registration of an entity in the physic engine: 
	-	entity
	-	entity current mesh
	-	entity current mesh’s material ( index = 0 )
	
	If no material specified, it fails back the the holding world’s material ( retrieved from PhysicDefaults.xml/Default ) 


	<h3>Note</h3><br>
	<br>

	You can execute this function multiple times. 
	
	Is utilizing #pRigidBody #pWorld #PhysicManager <br>

	

	*/
	
	proto->DeclareInParameter("Mesh",CKPGUID_MESH);
	proto->DeclareInParameter("Physic Properties",VTS_PHYSIC_PARAMETER);
	proto->DeclareInParameter("Local Position",CKPGUID_VECTOR);
	proto->DeclareInParameter("Local Rotation",CKPGUID_QUATERNION);
	proto->DeclareInParameter("Reference",CKPGUID_3DENTITY);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBAddShape);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBAddShape
// FullName:  PBAddShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBAddShape(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{

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
			return 0;
		}
		
		CKMesh *mesh  = (CKMesh*)GetInputParameterValue<CKObject*>(beh,bbI_Mesh);
		VxVector pos  = GetInputParameterValue<VxVector>(beh,bbI_Pos);
		VxQuaternion rot = GetInputParameterValue<VxQuaternion>(beh,bbI_Rot);
		pObjectDescr *descr = pFactory::Instance()->createPObjectDescrFromParameter(beh->GetInputParameter(bbI_PObject)->GetRealSource());
		CK3dEntity *ref  = (CK3dEntity*)GetInputParameterValue<CKObject*>(beh,bbI_Ref);
		//float density = GetInputParameterValue<float>(beh,bbI_Density);
		//float totalMass = GetInputParameterValue<float>(beh,bbI_TotalMass);

		result->addSubShape(mesh,*descr,ref,pos,rot);

		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBAddShapeCB
// FullName:  PBAddShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBAddShapeCB(const CKBehaviorContext& behcontext)
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

