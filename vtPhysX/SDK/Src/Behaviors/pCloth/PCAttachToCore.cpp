/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 2005 by <your name/ organization here>
 */
#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothAttachToCoreDecl();
CKERROR CreatePClothAttachToCoreProto(CKBehaviorPrototype **pproto);
int PClothAttachToCore(const CKBehaviorContext& behcontext);
CKERROR PClothAttachToCoreCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_BodyReference, 
	bbI_ImpulseThreshold,
	bbI_PenetrationDepth,
	bbI_MaxDeform,
};





//************************************
// Method:    FillBehaviorPClothAttachToCoreDecl
// FullName:  FillBehaviorPClothAttachToCoreDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
/*!
 * \brief
 * Write brief comment for FillBehaviorPClothAttachToCoreDecl here.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for FillBehaviorPClothAttachToCoreDecl here.
 * 
 * \remarks
 * Write remarks for FillBehaviorPClothAttachToCoreDecl here.
 * 
 * \see
 * Separate items with the '|' character.
 */
CKObjectDeclaration	*FillBehaviorPClothAttachToCoreDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAttachToCore");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Surrounds a rigid body with a cloth.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x37d75f67,0x7041320f));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAttachToCoreProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAttachToCoreProto
// FullName:  CreatePClothAttachToCoreProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAttachToCoreProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAttachToCore");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAttachToCore


	PClothAttachToCore is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Attaches a cloth to a body.<br>

	Call this function only once right after the cloth is created.
	Turning cloth into metal and vice versa during the simulation is not recommended.

	This feature is well suited for volumetric objects like barrels. 
	It cannot handle two dimensional flat pieces well.

	After this call, the cloth is infinitely stiff between collisions and simply 
	moves with the body. At impacts with an impact impulse greater than impulseThreshold, 
	the cloth is plastically deformed. Thus, a cloth with a core behaves like a piece of metal.

	The core body's geometry is adjusted automatically. Its size also depends on the 
	cloth thickness. Thus, it is recommended to choose small values for the thickness.
	At impacts, colliding objects are moved closer to the cloth by the value provided in 
	penetrationDepth which causes a more dramatic collision result.

	The core body must have at least one shape, and currently supported shapes are 
	spheres, capsules, boxes and compounds of spheres.
	It is recommended to specify the density rather than the mass of the core body. 
	This way the mass and inertia tensor are updated when the core deforms.

	The maximal deviation of cloth particles from their initial positions
	(modulo the global rigid body transforms translation and rotation) can be
	specified via the parameter maxDeformationDistance. Setting this parameter to
	zero means that the deformation is not limited.  

	<h3>Technical Information</h3>

	\image html PClothAttachToCore.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	<SPAN CLASS="pin">Body Reference: </SPAN>The core body to attach the cloth to.

	@see pCloth::attachToCore()
	<BR>
	<SPAN CLASS="pin">Impulse Threshold: </SPAN>Threshold for when deformation is allowed.
	<BR>

	<SPAN CLASS="pin">Penetration Depth: </SPAN>Amount by which colliding objects are brought closer to the cloth.
	<BR>


	<SPAN CLASS="pin">Max Deformation Distance: </SPAN>Maximum deviation of cloth particles from initial position.
	<BR>


	
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include pCloth.cpp
	</SPAN>
	*/



	proto->SetBehaviorCallbackFct( PClothAttachToCoreCB );

	proto->DeclareInParameter("Body Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Impulse Threshold",CKPGUID_FLOAT);
	proto->DeclareInParameter("Penetration Depth",CKPGUID_FLOAT);
	proto->DeclareInParameter("Max Deformation Distance",CKPGUID_FLOAT);


	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAttachToCore);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAttachToCore
// FullName:  PClothAttachToCore
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAttachToCore(const CKBehaviorContext& behcontext)
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

		CK3dEntity*bodyReference  = (CK3dEntity *) beh->GetInputParameterObject(bbI_BodyReference);
		if (!bodyReference)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		pRigidBody *body = GetPMan()->getBody(bodyReference);
		if (!body)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		pWorld *world  = body->getWorld();
		if (!world)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}
		
		pCloth *cloth  = world->getCloth(target);
		if (!cloth)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}
		
		

		float impulseThreshold = GetInputParameterValue<float>(beh,bbI_ImpulseThreshold);
		float penetrationDepth = GetInputParameterValue<float>(beh,bbI_PenetrationDepth);
		float maxDeform = GetInputParameterValue<int>(beh,bbI_MaxDeform);

		cloth->attachToCore(bodyReference,impulseThreshold,penetrationDepth,maxDeform);

		beh->ActivateOutput(0);

	}
	return 0;
}

//************************************
// Method:    PClothAttachToCoreCB
// FullName:  PClothAttachToCoreCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAttachToCoreCB(const CKBehaviorContext& behcontext)
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

