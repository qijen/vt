#include <StdAfx.h>
#include "pCommon.h"



CKObjectDeclaration	*FillBehaviorPBSetParDecl();
CKERROR CreatePBSetParProto(CKBehaviorPrototype **pproto);
int PBSetPar(const CKBehaviorContext& behcontext);
CKERROR PBSetParCB(const CKBehaviorContext& behcontext);


enum bInputs
{

	bbI_CollisionGroup,
	bbI_GroupsMask,
	bbI_Flags,
	bbI_TFlags,
	bbI_LinDamp,
	bbI_AngDamp,
	bbI_MassOffset,
	bbI_ShapeOffset,
	
};

#define BB_SSTART 0

BBParameter pInMap22[]  =
{

	BB_SPIN(bbI_CollisionGroup,CKPGUID_INT,"Collision Group",""),
	BB_SPIN(bbI_GroupsMask,VTS_FILTER_GROUPS,"Group Mask","0.0"),
	BB_SPIN(bbI_Flags,VTF_BODY_FLAGS,"Body Flags",""),
	BB_SPIN(bbI_TFlags,VTF_BODY_TRANS_FLAGS,"Transformation Lock Flags",""),
	BB_SPIN(bbI_LinDamp,CKPGUID_FLOAT,"Linear Damping","0.0"),
	BB_SPIN(bbI_AngDamp,CKPGUID_FLOAT,"Angular Damping",""),
	BB_SPIN(bbI_MassOffset,CKPGUID_VECTOR,"Mass Offset",""),
	BB_SPIN(bbI_ShapeOffset,CKPGUID_VECTOR,"Pivot Offset",""),

};

#define gPIMAP pInMap22

//************************************
// Method:    FillBehaviorPBSetParDecl
// FullName:  FillBehaviorPBSetParDecl
// Access:    public
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBSetParDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSetPar");
	od->SetCategory("Physic/Body");
	od->SetDescription("Sets physic parameters.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x137e7ec4,0x4ca85a4b));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSetParProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePBSetParProto
// FullName:  CreatePBSetParProto
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBSetParProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSetPar");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PBSetPar

	PBSetPar is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PBSetPar.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body or its sub shape.
	<BR>
	<SPAN CLASS="pin">Collisions Group: </SPAN>Which collision group this body or the sub shape is part of.See pRigidBody::setCollisionsGroup().
	<BR>
	<SPAN CLASS="pin">Groupsmask: </SPAN>Sets 128-bit mask used for collision filtering. It can be sub shape specific.See comments for ::pGroupsMask and pRigidBody::setGroupsMask()
	<BR>

	<SPAN CLASS="pin">Flags: </SPAN>Changes essential physic behavior. See pRigidBody::setFlags() <br>
		-	This flags can be changed after the bodies registration : 
			-	Gravity	-	See pRigidBody::ennableGravity() 
			-	Kinematic	-	See pRigidBody::setKinematic() 
			-	Trigger Shape	- See pRigidBody::enableTriggerShape() 
			-	Collisions Notify - See pRigidBody::enableCollisionsNotify()
			-	Sleep	- pRigidBody::setSleeping() or pRigidBody::isSleeping()
	
	<SPAN CLASS="pin">Transformation Locks: </SPAN>Locks a translation or orientation in a specific dimension.
	<BR>
	
	<SPAN CLASS="pin">Linear Damping: </SPAN>The new linear damping scale.
	<BR>
	<SPAN CLASS="pin">Angular Damping: </SPAN>The new linear damping scale.
	<BR>

	<SPAN CLASS="pin">Mass Offset: </SPAN>The new mass center in the bodies local space.
	<BR>
	<SPAN CLASS="pin">Pivot Offset: </SPAN>Specifies the shapes position in the bodies local space. See pRigidBody::setLocalShapePosition().
	<BR>
	
	
	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include PBSetEx.cpp
	</SPAN>
	*/

	proto->SetBehaviorCallbackFct( PBSetParCB );

	BB_EVALUATE_SETTINGS(pInMap22)

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PBSetPar);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBSetPar
// FullName:  PBSetPar
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBSetPar(const CKBehaviorContext& behcontext)
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
		if( !target ) 	bbErrorME("No Reference Object specified");

		pRigidBody *body = NULL;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world)
			bbErrorME("No valid world object found");
		

		body = GetPMan()->getBody(target);
		if (!body)	bbErrorME("Object not physicalized");


		BB_DECLARE_PIMAP;

		/************************************************************************/
		/* retrieve settings state  */
		/************************************************************************/
		BBSParameter(bbI_CollisionGroup);
		BBSParameter(bbI_Flags);
		BBSParameter(bbI_TFlags);
		BBSParameter(bbI_GroupsMask);
		BBSParameter(bbI_LinDamp);
		BBSParameter(bbI_AngDamp);
		BBSParameter(bbI_MassOffset);
		BBSParameter(bbI_ShapeOffset);

	
		if (sbbI_GroupsMask)
		{
			NxGroupsMask mask1;
			CKParameter *fConst1 = beh->GetInputParameter(BB_IP_INDEX(bbI_GroupsMask))->GetRealSource();

			using namespace vtTools::BehaviorTools;
			using namespace vtTools::ParameterTools;

			NxShape *dstShape = NULL;

			NxShape *mainshape = body->getMainShape();
			if (mainshape==NULL)
			{
				int op = 2;
				op++;

			}
			NxShape *inputShape = body->_getSubShapeByEntityID(target->GetID());
			if (mainshape==inputShape)
			{
				dstShape = mainshape;
			}else{
				dstShape = inputShape;
			}


			mask1.bits0  = GetValueFromParameterStruct<int>(fConst1,0);
			mask1.bits1  = GetValueFromParameterStruct<int>(fConst1,1);
			mask1.bits2  = GetValueFromParameterStruct<int>(fConst1,2);
			mask1.bits3  = GetValueFromParameterStruct<int>(fConst1,3);
			
			if (dstShape)
			{
				dstShape->setGroupsMask(mask1);
			}
		}

		if (sbbI_CollisionGroup)
		{
			int collGroup = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_CollisionGroup));
			body->setCollisionsGroup(collGroup,target);
		}

		if (sbbI_Flags)
		{
			int flags = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_Flags));
			body->updateFlags(flags,target);
		}

		if (sbbI_TFlags)
		{
			int tflags = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_TFlags));
			body->lockTransformation(tflags);
		}

		if (sbbI_LinDamp)
		{
			float lDamp = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_LinDamp));
			body->setLinearDamping(lDamp);
		}
		if (sbbI_AngDamp)
		{
			float aDamp= GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_AngDamp));
			body->setAngularDamping(aDamp);
		}

		if (sbbI_MassOffset)
		{
			VxVector mOffset= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_MassOffset));
			body->setMassOffset(mOffset);
		}

		if (sbbI_ShapeOffset)
		{
			VxVector sOffset= GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(bbI_ShapeOffset));
			body->setLocalShapePosition(sOffset,target);
		}

	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PBSetParCB
// FullName:  PBSetParCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBSetParCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
	
	CKContext* ctx = behcontext.Context;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;

}

