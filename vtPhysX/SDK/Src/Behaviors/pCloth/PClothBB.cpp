#include <StdAfx.h>
#include "pCommon.h"



CKObjectDeclaration	*FillBehaviorPClothDecl();
CKERROR CreatePClothProto(CKBehaviorPrototype **pproto);
int PCloth(const CKBehaviorContext& behcontext);
CKERROR PClothCB(const CKBehaviorContext& behcontext);

enum bInputs
{


	bbI_AttachmentFlags=E_CS_FLAGS+2,
	bbI_TargetWorld = E_CS_FLAGS+3,


};


enum bSettings
{
	bbS_USE_DEFAULT_WORLD,
	bbS_ADD_ATTRIBUTES
};

//************************************
// Method:    FillBehaviorPClothDecl
// FullName:  FillBehaviorPClothDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothDecl()
{


	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCloth");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Creates or modifies a cloth.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7901564e,0x7fdb5d76));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothProto
// FullName:  CreatePClothProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCloth");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PCloth  

	PCloth is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Creates or modifies a cloth.<br>

	<h3>Technical Information</h3>

	\image html PCloth.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target reference. The entity can not be a rigid body or a shub shape!
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Thickness: </SPAN>Thickness of the cloth.

		The thickness is usually a fraction of the overall extent of the cloth and
		should not be set to a value greater than that.	A good value is the maximal
		distance between two adjacent cloth particles in their rest pose. Visual
		artifacts or collision problems may appear if the thickness is too small.

		<b>Default:</b> 0.01 <br>
		<b>Range:</b> [0,inf)

		@see pCloth.setThickness()	<BR>
	
	<SPAN CLASS="pin">Density : </SPAN> Density of the cloth (mass per area).
	
		<b>Default:</b> 1.0 <br>
		<b>Range:</b> (0,inf)	<BR>
	
	<BR>



	<SPAN CLASS="pin">Bending Stiffness: </SPAN> Bending stiffness of the cloth in the range 0 to 1.
	
	Only has an effect if the flag PCF_Bending is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,1]

	@see PCF_Bending pCloth.setBendingStiffness()





	<BR>
	<SPAN CLASS="pin">Stretching Stiffness: </SPAN>Stretching stiffness of the cloth in the range 0 to 1.
	
	\note: stretching stiffness must be larger than 0.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> (0,1]

	@see pCloth.setStretchingStiffness()

	<BR>
	<BR>


	<BR>
	<SPAN CLASS="pin">Damping: </SPAN>Spring damping of the cloth in the range 0 to 1.

	Only has an effect if the flag PCF_Damping is set.

	<b>Default:</b> 0.5 <br>
	<b>Range:</b> [0,1]

	@see PCF_Damping pCloth.setDampingCoefficient()

	<BR>
	<BR>


	<BR>
	<SPAN CLASS="pin">Friction</SPAN>Friction coefficient in the range 0 to 1. 

	Defines the damping of the velocities of cloth particles that are in contact.

	<b>Default:</b> 0.5 <br>
	<b>Range:</b> [0,1]

	@see pCloth.setFriction()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Pressure :</SPAN>

	If the flag PCF_Pressure is set, this variable defines the volume
	of air inside the mesh as volume = pressure * restVolume. 

	For pressure < 1 the mesh contracts w.r.t. the rest shape
	For pressure > 1 the mesh expands w.r.t. the rest shape

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	@see PCF_pressure pCloth.setPressure()

	<BR>
	<BR>



	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Tear Factor: </SPAN>
	
		If the flag PCF_Tearable is set, this variable defines the 
		elongation factor that causes the cloth to tear. 

		Must be larger than 1.
		Make sure meshData.maxVertices and the corresponding buffers
		in meshData are substantially larger (e.g. 2x) than the number 
		of original vertices since tearing will generate new vertices.

		When the buffer cannot hold the new vertices anymore, tearing stops.
		
		<b>Default:</b> 1.5 <br>
		<b>Range:</b> (1,inf)

		@see pCloth.setTearFactor()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Collision Response Coefficient: </SPAN>

	Defines a factor for the impulse transfer from cloth to colliding rigid bodies.

	Only has an effect if PCF_CollisionTwoWay is set.

	<b>Default:</b> 0.2 <br>
	<b>Range:</b> [0,inf)

	@see PCF_CollisionTwoWay pCloth.setCollisionResponseCoefficient()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin"></SPAN>
	<BR>
	<BR>



	<BR>
	<SPAN CLASS="pin">Attachment Response Coefficient: </SPAN>Defines a factor for the impulse transfer from cloth to attached rigid bodies.

	Only has an effect if the mode of the attachment is set to nx_cloth_attachment_twoway.

	<b>Default:</b> 0.2 <br>
	<b>Range:</b> [0,1]

	@see pCloth.attachToShape pCloth.attachToCollidingShapes pCloth.attachVertexToShape pCloth.setAttachmentResponseCoefficient()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Attachment Tear Factor: </SPAN>

	If the flag PCF_Tearable is set in the attachment method of pCloth, 
	this variable defines the elongation factor that causes the attachment to tear. 

	Must be larger than 1.

	<b>Default:</b> 1.5 <br>
	<b>Range:</b> (1,inf)

	@see pCloth.setAttachmentTearFactor() pCloth.attachToShape pCloth.attachToCollidingShapes pCloth.attachVertexToShape
	
	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">To Fluid Response Coefficient: </SPAN>
	
	Defines a factor for the impulse transfer from this cloth to colliding fluids.

	Only has an effect if the PCF_FluidCollision flag is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	Note: Large values can cause instabilities

	@see pClothDesc.flags pClothDesc.fromFluidResponseCoefficient
	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">From Fluid Response Coefficient: </SPAN>

	Defines a factor for the impulse transfer from colliding fluids to this cloth.

	Only has an effect if the PCF_FluidCollision flag is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	Note: Large values can cause instabilities

	@see pClothDesc.flags pClothDesc.ToFluidResponseCoefficient

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Min Adhere Velocity: </SPAN>
	
	If the PCF_Adhere flag is set the cloth moves partially in the frame 
	of the attached actor. 

	This feature is useful when the cloth is attached to a fast moving character.
	In that case the cloth adheres to the shape it is attached to while only 
	velocities below the parameter minAdhereVelocity are used for secondary effects.

	<b>Default:</b> 1.0
	<b>Range:</b> [0,inf)

	@see PCF_AdHere

	

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Solver Iterations: </SPAN>
	
	Number of solver iterations. 

	Note: Small numbers make the simulation faster while the cloth gets less stiff.

	<b>Default:</b> 5
	<b>Range:</b> [1,inf)

	@see pCloth.setSolverIterations()
	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">External acceleration : </SPAN>
	
		External acceleration which affects all non attached particles of the cloth. 

		<b>Default:</b> (0,0,0)

		@see pCloth.setExternalAcceleration()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Wind Acceleration: </SPAN>

		Acceleration which acts normal to the cloth surface at each vertex.

		<b>Default:</b> (0,0,0)

		@see pCloth.setWindAcceleration()


	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Wake Up Counter: </SPAN>

		The cloth wake up counter.

		<b>Range:</b> [0,inf)<br>
		<b>Default:</b> 20.0f*0.02f

		@see pCloth.wakeUp() pCloth.putToSleep()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Sleep Linear Velocity:</SPAN>

	Maximum linear velocity at which cloth can go to sleep.

	If negative, the global default will be used.

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> -1.0

	@see pCloth.setSleepLinearVelocity() pCloth.getSleepLinearVelocity()

	<BR>
	<BR>


	<BR>
	<SPAN CLASS="pin">Collision Group: </SPAN>

	Sets which collision group this cloth is part of.

	<b>Range:</b> [0, 31]
	<b>Default:</b> 0

	pCloth.setCollisionGroup()

	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Valid Bounds: </SPAN>

	If the flag PCF_ValidBounds is set, this variable defines the volume
	outside of which cloth particle are automatically removed from the simulation. 

	@see PCF_ValidBounds pCloth.setValidBounds()


	<BR>
	<BR>

	<BR>
	<SPAN CLASS="pin">Relative Grid Spacing: </SPAN>

	This parameter defines the size of grid cells for collision detection.

	The cloth is represented by a set of world aligned cubical cells in broad phase.
	The size of these cells is determined by multiplying the length of the diagonal
	of the AABB of the initial cloth size with this constant.

	<b>Range:</b> [0.01,inf)<br>
	<b>Default:</b> 0.25

	<BR>
	<BR>


	<BR>
	<SPAN CLASS="pin">Flags: </SPAN>

	Cloth flags.

	<b>Default:</b> Gravity

	@see pClothFlag

	<BR>
	<BR>


	<BR>
	<SPAN CLASS="pin">Attachment Flags: </SPAN>

	Attachment flags.

	<b>Default:</b>  PCAF_ClothAttachmentTwoway

	@see pClothAttachmentFlag

	<BR>
	<BR>

	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager #pCloth <br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include PClothCreate.vsl
	</SPAN>
	*/



	proto->SetBehaviorCallbackFct( PClothCB );

	proto->DeclareInParameter("Thickness",CKPGUID_FLOAT,"0.01f");
	proto->DeclareInParameter("Density",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("Bending Stiffness",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("Stretching Stiffness",CKPGUID_FLOAT,"1.0f");
	
	proto->DeclareInParameter("Damping Coefficient",CKPGUID_FLOAT,"0.50f");
	
	proto->DeclareInParameter("Friction",CKPGUID_FLOAT,"0.5f");
	proto->DeclareInParameter("Pressure",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("Tear Factor",CKPGUID_FLOAT,"1.5f");
	proto->DeclareInParameter("Collision Response Coefficient",CKPGUID_FLOAT,"0.2f");

	proto->DeclareInParameter("Attachment Response Coefficient",CKPGUID_FLOAT,"0.2f");
	proto->DeclareInParameter("AttachmentTearFactor",CKPGUID_FLOAT,"1.5f");
	proto->DeclareInParameter("ToFluidResponseCoefficient",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("FromFluidResponseCoefficient",CKPGUID_FLOAT,"1.0f");

	proto->DeclareInParameter("MinAdhereVelocity",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("SolverIterations",CKPGUID_INT,"5");
	proto->DeclareInParameter("ExternalAcceleration",CKPGUID_VECTOR);
	proto->DeclareInParameter("WindAcceleration",CKPGUID_VECTOR);

	proto->DeclareInParameter("WakeUpCounter",CKPGUID_FLOAT,"0.4f");
	proto->DeclareInParameter("SleepLinearVelocity",CKPGUID_FLOAT,"-1.0f");
	proto->DeclareInParameter("CollisionGroup",CKPGUID_INT);
	proto->DeclareInParameter("ValidBounds",CKPGUID_BOX);

	proto->DeclareInParameter("RelativeGridSpacing",CKPGUID_FLOAT,"0.25f");
	proto->DeclareInParameter("Flags",VTE_CLOTH_FLAGS,"Gravity");
	proto->DeclareInParameter("Tear Vertex Color",CKPGUID_COLOR,"1.0f");
	proto->DeclareInParameter("Target World Reference",CKPGUID_3DENTITY,"pDefaultWorld");
	proto->DeclareInParameter("Attachment Flags",VTE_CLOTH_ATTACH_FLAGS);

	
	proto->DeclareInParameter("Core Body Reference",CKPGUID_3DENTITY,"");

	proto->DeclareInParameter("Metal Impulse Threshold",CKPGUID_FLOAT,"50");
	proto->DeclareInParameter("Metal Penetration Depth",CKPGUID_FLOAT,"0.5");
	proto->DeclareInParameter("Metal Max Deformation Distance",CKPGUID_FLOAT,"0.5");

	//proto->DeclareSetting("Use Default World",CKPGUID_BOOL,"TRUE");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PCloth);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PCloth
// FullName:  PCloth
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PCloth(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;

#ifdef DONLGE
	if(!PhysicManager::DongleHasAdvancedVersion)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : no license !");
		beh->ActivateOutput(0);
		return 0;
	}

#endif

	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);


		//////////////////////////////////////////////////////////////////////////
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;

		
		//////////////////////////////////////////////////////////////////////////

		CK3dEntity*worldRef  = (CK3dEntity *) beh->GetInputParameterObject(E_CS_WORLD_REFERENCE);
		if (!worldRef)
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : invalid world reference !");
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		
		CK3dEntity*metalCoreRef  = (CK3dEntity *) beh->GetInputParameterObject(E_CS_ATTACHMENT_FLAGS +1);
		if (!metalCoreRef)
		{
			//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : invalid world reference !");
			//beh->ActivateOutput(0);
			//return CKBR_PARAMETERERROR;
		}
		
		
		pWorld *world  = GetPMan()->getWorld(worldRef->GetID());
		if (!world)
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : couldn't find world !");
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		pRigidBody*body = world->getBody(target);
		if(body)
		{
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Cannot create cloth : target can not be a rigid body !");
			beh->ActivateOutput(0);
			return CKBR_OWNERERROR;
		}

		NxShape *shape  = world->getShapeByEntityID(target->GetID());
		if(shape)
		{
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Cannot create cloth : target can not be a shape !");
			beh->ActivateOutput(0);
			return CKBR_OWNERERROR;
		}
		

		pClothDesc *descr  = new pClothDesc();

		using namespace vtTools::ParameterTools;
		using namespace vtTools::BehaviorTools;
		descr->thickness = GetInputParameterValue<float>(beh,E_CS_THICKNESS);
		descr->density = GetInputParameterValue<float>(beh,E_CS_DENSITY);
		descr->bendingStiffness = GetInputParameterValue<float>(beh,E_CS_BENDING_STIFFNESS);
		descr->stretchingStiffness = GetInputParameterValue<float>(beh,E_CS_STRETCHING_STIFFNESS);
		descr->dampingCoefficient = GetInputParameterValue<float>(beh,E_CS_DAMPING_COEFFICIENT);
		descr->friction = GetInputParameterValue<float>(beh,E_CS_FRICTION);
		descr->pressure = GetInputParameterValue<float>(beh,E_CS_PRESSURE);
		descr->tearFactor = GetInputParameterValue<float>(beh,E_CS_TEAR_FACTOR);
		descr->collisionResponseCoefficient = GetInputParameterValue<float>(beh,E_CS_COLLISIONRESPONSE_COEFFICIENT);
		descr->attachmentResponseCoefficient = GetInputParameterValue<float>(beh,E_CS_ATTACHMENTRESPONSE_COEFFICIENT);
		descr->attachmentTearFactor = GetInputParameterValue<float>(beh,E_CS_ATTACHMENT_TEAR_FACTOR);
		descr->toFluidResponseCoefficient = GetInputParameterValue<float>(beh,E_CS_TO_FLUID_RESPONSE_COEFFICIENT);

		descr->fromFluidResponseCoefficient = GetInputParameterValue<float>(beh,E_CS_FROM_FLUIDRESPONSE_COEFFICIENT);
		descr->minAdhereVelocity = GetInputParameterValue<float>(beh,E_CS_MIN_ADHERE_VELOCITY);
		descr->solverIterations = GetInputParameterValue<unsigned int>(beh,E_CS_SOLVER_ITERATIONS);


		descr->externalAcceleration = GetInputParameterValue<VxVector>(beh,E_CS_EXTERN_ALACCELERATION);
		descr->windAcceleration = GetInputParameterValue<VxVector>(beh,E_CS_WIND_ACCELERATION);
		descr->wakeUpCounter = GetInputParameterValue<float>(beh,E_CS_WAKE_UP_COUNTER);

		descr->sleepLinearVelocity = GetInputParameterValue<float>(beh,E_CS_SLEEP_LINEAR_VELOCITY);
		descr->collisionGroup = GetInputParameterValue<int>(beh,E_CS_COLLISIONG_ROUP);

		descr->validBounds = GetInputParameterValue<VxBbox>(beh,E_CS_VALID_BOUNDS);
		descr->relativeGridSpacing = GetInputParameterValue<float>(beh,E_CS_RELATIVE_GRID_SPACING);

		descr->flags = GetInputParameterValue<int>(beh,E_CS_FLAGS);
		descr->tearVertexColor  = GetInputParameterValue<VxColor>(beh,E_CS_TEAR_VERTEX_COLOR);
		descr->attachmentFlags  = (pClothAttachmentFlag)GetInputParameterValue<int>(beh,E_CS_ATTACHMENT_FLAGS);


		float metalImpulsThresold = GetInputParameterValue<float>(beh,E_CS_ATTACHMENT_FLAGS  + 2);
		float metalPenetrationDepth = GetInputParameterValue<float>(beh,E_CS_ATTACHMENT_FLAGS  + 3);
		float metalMaxDeform= GetInputParameterValue<float>(beh,E_CS_ATTACHMENT_FLAGS  + 4);


		descr->worldReference  = worldRef->GetID();
		
		//descr->setToDefault();
//		descr->flags = descr->flags | PCF_Bending;

		pCloth *cloth  = world->getCloth(target);
		if (!cloth)
		{
			cloth = pFactory::Instance()->createCloth(target,*descr);
			if (!cloth)
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : factory object failed !");
			}
		}

//		descr->flags = descr->flags | PCF_AttachToCore;


		if (cloth)
		{
			cloth->setThickness(descr->thickness);
			cloth->setBendingStiffness(descr->bendingStiffness);
			cloth->setStretchingStiffness(descr->stretchingStiffness);
			cloth->setDampingCoefficient(descr->dampingCoefficient);
			cloth->setFriction(descr->friction);
			cloth->setPressure(descr->pressure);
			cloth->setTearFactor(descr->tearFactor);
			cloth->setCollisionResponseCoefficient(descr->collisionResponseCoefficient);
			cloth->setAttachmentResponseCoefficient(descr->attachmentResponseCoefficient);
			cloth->setAttachmentTearFactor(descr->attachmentTearFactor);
			cloth->setToFluidResponseCoefficient(descr->toFluidResponseCoefficient);
			cloth->setFromFluidResponseCoefficient(descr->fromFluidResponseCoefficient);
			cloth->setMinAdhereVelocity(descr->minAdhereVelocity);
			cloth->setSolverIterations(descr->solverIterations);
			cloth->setExternalAcceleration(descr->externalAcceleration);
			cloth->setWindAcceleration(descr->windAcceleration);
			cloth->setSleepLinearVelocity(descr->sleepLinearVelocity);
			cloth->setGroup(descr->collisionGroup);
			cloth->setValidBounds(descr->validBounds);
			cloth->setFlags(descr->flags);

		}

		int flags = GetInputParameterValue<int>(beh,bbI_AttachmentFlags);

		if (descr->flags & PCF_AttachToParentMainShape )
		{
			if (target->GetParent())
			{
				CK3dEntity *bodyReference = pf->getMostTopParent(target);
				if (bodyReference)
				{
					pRigidBody *body  = GetPMan()->getBody(bodyReference);
					if (body)
					{
						cloth->attachToShape((CKBeObject*)bodyReference,flags);
					}
				}
			}
		}



		if (descr->flags & PCF_AttachToCollidingShapes)
		{
			cloth->attachToCollidingShapes(flags);
		}


	
		if (descr->flags & PCF_AttachToCore)
		{
			if (metalCoreRef)
			{
				pRigidBody*metalBody = world->getBody(metalCoreRef);
				if (!metalBody)
				{


					pObjectDescr objDesc;
					objDesc.flags =(BodyFlags)(BF_Collision|BF_Gravity|BF_Moving);
					objDesc.hullType = HT_Capsule;
					objDesc.density = 0.1f;
					metalBody = pFactory::Instance()->createCapsule(metalCoreRef,worldRef,&objDesc,0);
					if (metalBody)
					{
						cloth->attachToCore(metalCoreRef,metalImpulsThresold,metalPenetrationDepth,metalMaxDeform);
					}
					
				}
			}
		}
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothCB
// FullName:  PClothCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
/*
			DWORD useDWorld;
			beh->GetLocalParameterValue(bbS_USE_DEFAULT_WORLD,&useDWorld);
			beh->EnableInputParameter(bbI_TargetWorld,!useDWorld);
*/
			

			/*DWORD ADamp;
			beh->GetLocalParameterValue(bbI_ADamp,&ADamp);
			beh->EnableInputParameter(bbI_ADamp,ADamp);*/


		}
		break;
	}
	return CKBR_OK;

}