#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBSet2Decl();
CKERROR CreatePBSet2Proto(CKBehaviorPrototype **pproto);
int PBSet2(const CKBehaviorContext& behcontext);
CKERROR PBSet2CB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_CollisionGroup,
	bbI_Kinematic,
	bbI_Gravity,
	bbI_Collision,
	bbI_MassOffset,
	bbI_ShapeOffset,
	bbI_CollNotify,
	bbI_TFlags,
	bbI_LDamp,
	bbI_ADamp,
	bbI_GroupsMask,
};

//************************************
// Method:    FillBehaviorPBSet2Decl
// FullName:  FillBehaviorPBSet2Decl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBSet2Decl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSetEx");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Sets physical quantities.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3da14376,0x3b4748a2));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSet2Proto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBSet2Proto
// FullName:  CreatePBSet2Proto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBSet2Proto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSetEx-Obsolete");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/* PBSetEx  

	PBSet2 is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PBSet2.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pin">Collisions Group: </SPAN>Which collision group this body is part of.See pRigidBody::setCollisionsGroup().
	<BR>
	<SPAN CLASS="pin">Kinematic Object: </SPAN>The kinematic state. See pRigidBody::setKinematic().
	<BR>
	<SPAN CLASS="pin">Gravity: </SPAN>The gravity state.See pRigidBody::enableGravity().
	<BR>
	<SPAN CLASS="pin">Collision: </SPAN>Determines whether the body reacts to collisions.See pRigidBody::enableCollision(). 
	<BR>
	<SPAN CLASS="pin">Mass Offset: </SPAN>The new mass center in the bodies local space.
	<BR>
	<SPAN CLASS="pin">Pivot Offset: </SPAN>The initial shape position in the bodies local space.
	<BR>
	<SPAN CLASS="pin">Notify Collision: </SPAN>Enables collision notification.This is necessary to use collisions related building blocks. 
	<BR>
	<SPAN CLASS="pin">Transformation Locks: </SPAN>Specifies in which dimension a a transformation lock should occour.
	<BR>
	<SPAN CLASS="pin">Linear Damping: </SPAN>The new linear damping scale.
	<BR>
	<SPAN CLASS="pin">Angular Damping: </SPAN>The new linear damping scale.
	<BR>

	<SPAN CLASS="pin">Filter Groups: </SPAN>Sets the filter mask of the initial or sub shape.
	<BR>


	<SPAN CLASS="setting">Collisions Group: </SPAN>Enables input for collisions group.
	<BR>
	<SPAN CLASS="setting">Kinematic Object: </SPAN>Enables input for kinematic object.
	<BR>
	<SPAN CLASS="setting">Gravity: </SPAN>Enables input for gravity.
	<BR>
	<SPAN CLASS="setting">Collision: </SPAN>Enables input for collision. 
	<BR>
	<SPAN CLASS="setting">Mas Offset: </SPAN>Enables input for mass offset. 
	<BR>
	<SPAN CLASS="setting">Pivot Offset: </SPAN>Enables input for pivot offset.
	<BR>
	<SPAN CLASS="setting">Notify Collision: </SPAN>Enables input for collision. 
	<BR>
	<SPAN CLASS="setting">Linear Damping: </SPAN>Enables input for linear damping.
	<BR>
	<SPAN CLASS="setting">Angular Damping: </SPAN>Enables input for angular damping.
	<BR>
	<SPAN CLASS="setting">Filter Groups: </SPAN>Enables input for filter groups.
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

	proto->DeclareSetting("Collisions Group",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Kinematic",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Mass Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Pivot Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Notify Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Transformation Locks",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Linear Damping",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Angular Damping",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Filter Groups",CKPGUID_BOOL,"FALSE");

	proto->SetBehaviorCallbackFct( PBSet2CB );
	
	proto->DeclareInParameter("Collisions Group",CKPGUID_INT,"0");
	proto->DeclareInParameter("Kinematic Object",CKPGUID_BOOL,"FALSE");
	proto->DeclareInParameter("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareInParameter("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareInParameter("Mass Offset",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Pivot Offset",CKPGUID_VECTOR,"0.0f");
	proto->DeclareInParameter("Notify Collisions",CKPGUID_BOOL,"FALSE");
	proto->DeclareInParameter("Transformation Locks",VTF_BODY_TRANS_FLAGS,"FALSE");
	proto->DeclareInParameter("Linear Damping",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Angular Damping",CKPGUID_FLOAT,"0");
	proto->DeclareInParameter("Filter Groups",VTS_FILTER_GROUPS,"0");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBSet2);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBSet2
// FullName:  PBSet2
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBSet2(const CKBehaviorContext& behcontext)
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

		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBodyFromSubEntity(target);
		if(result)
		{
			
			//////////////////////////////////////////////////////////////////////////
			//linear damp : 
			DWORD cGroup;
			beh->GetLocalParameterValue(bbI_CollisionGroup,&cGroup);
			if (cGroup)
			{
				int val  = GetInputParameterValue<int>(beh,bbI_CollisionGroup);
				result->setCollisionsGroup(val);
			}

			DWORD kine;
			beh->GetLocalParameterValue(bbI_Kinematic,&kine);
			if (kine && (result->getFlags() & BF_Moving) )
			{
				int val  = GetInputParameterValue<int>(beh,bbI_Kinematic);
				result->setKinematic(val);

			}

			
			DWORD gravity;
			beh->GetLocalParameterValue(bbI_Gravity,&gravity);
			if (gravity)
			{
				int val  = GetInputParameterValue<int>(beh,bbI_Gravity);
				if (result->getActor() && (result->getFlags() & BF_Moving))
				{
					if (!val)
					{
						result->getActor()->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
					}else{
						result->getActor()->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
					}
				}
			}
				
			DWORD col=0;
			beh->GetLocalParameterValue(bbI_Collision,&col);
			if (col)
			{
				int val  = GetInputParameterValue<int>(beh,bbI_Collision);
				result->enableCollision(!val);
			}


			DWORD mass=0;
			beh->GetLocalParameterValue(bbI_MassOffset,&mass);
			if (mass)
			{
				VxVector val  = GetInputParameterValue<VxVector>(beh,bbI_MassOffset);
				result->setMassOffset(val);
			}

			
			DWORD pivot=0;
			beh->GetLocalParameterValue(bbI_ShapeOffset,&pivot);
			if (mass)
			{
				VxVector val  = GetInputParameterValue<VxVector>(beh,bbI_ShapeOffset);
				result->setPivotOffset(val);

			}

			DWORD collNotify=0;
			beh->GetLocalParameterValue(bbI_CollNotify,&collNotify);
			if (collNotify)
			{
				int val  = GetInputParameterValue<int>(beh,bbI_CollNotify);
				if (val)
				{
					result->getActor()->setContactReportFlags(NX_NOTIFY_ON_TOUCH);
				}else
					result->getActor()->setContactReportFlags(NX_IGNORE_PAIR);
			}

			DWORD tFlags=0;
			beh->GetLocalParameterValue(bbI_TFlags,&tFlags);
			if (tFlags && (result->getFlags() & BF_Moving))
			{
				int val  = GetInputParameterValue<int>(beh,bbI_TFlags);
				result->lockTransformation(val);
			}

			DWORD LDamp;
			beh->GetLocalParameterValue(bbI_LDamp,&LDamp);
			if (LDamp)
			{
				float val  = GetInputParameterValue<float>(beh,bbI_LDamp);
				result->setLinearDamping(val);
			}

			DWORD ADamp;
			beh->GetLocalParameterValue(bbI_ADamp,&ADamp);
			if (ADamp)
			{
				float val  = GetInputParameterValue<float>(beh,bbI_ADamp);
				result->setAngularDamping(val);
			}

			DWORD fMask;
			beh->GetLocalParameterValue(bbI_GroupsMask,&fMask);
			if (fMask)
			{

				NxShape *dstShape = NULL;
				
				NxShape *mainshape = result->getMainShape();
				if (mainshape==NULL)
				{
					int op = 2;
					op++;

				}
				NxShape *inputShape = result->_getSubShapeByEntityID(target->GetID());
				if (mainshape==inputShape)
				{
					dstShape = mainshape;
				}else{
					dstShape = inputShape;
				}

				CKParameter *fConst1 = beh->GetInputParameter(bbI_GroupsMask)->GetRealSource();

				using namespace vtTools::BehaviorTools;
				using namespace vtTools::ParameterTools;

				NxGroupsMask mask1;
				mask1.bits0  = GetValueFromParameterStruct<int>(fConst1,0);
				mask1.bits1  = GetValueFromParameterStruct<int>(fConst1,1);
				mask1.bits2  = GetValueFromParameterStruct<int>(fConst1,2);
				mask1.bits3  = GetValueFromParameterStruct<int>(fConst1,3);

				if (dstShape)
				{
					dstShape->setGroupsMask(mask1);
				}
			}
		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBSet2CB
// FullName:  PBSet2CB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBSet2CB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD collGroup;
			beh->GetLocalParameterValue(bbI_CollisionGroup,&collGroup);
			beh->EnableInputParameter(bbI_CollisionGroup,collGroup);

			DWORD kinematic;
			beh->GetLocalParameterValue(bbI_Kinematic,&kinematic);
			beh->EnableInputParameter(bbI_Kinematic,kinematic);
			
			DWORD grav;
			beh->GetLocalParameterValue(bbI_Gravity,&grav);
			beh->EnableInputParameter(bbI_Gravity,grav);

			DWORD coll;
			beh->GetLocalParameterValue(bbI_Collision,&coll);
			beh->EnableInputParameter(bbI_Collision,coll);


			DWORD mOff;
			beh->GetLocalParameterValue(bbI_MassOffset,&mOff);
			beh->EnableInputParameter(bbI_MassOffset,mOff);

			DWORD pOff;
			beh->GetLocalParameterValue(bbI_ShapeOffset,&pOff);
			beh->EnableInputParameter(bbI_ShapeOffset,pOff);

			DWORD collNotify;
			beh->GetLocalParameterValue(bbI_CollNotify,&collNotify);
			beh->EnableInputParameter(bbI_CollNotify,collNotify);
			
			DWORD tFlags;
			beh->GetLocalParameterValue(bbI_TFlags,&tFlags);
			beh->EnableInputParameter(bbI_TFlags,tFlags);

			DWORD lDamp;
			beh->GetLocalParameterValue(bbI_LDamp,&lDamp);
			beh->EnableInputParameter(bbI_LDamp,lDamp);

			DWORD ADamp;
			beh->GetLocalParameterValue(bbI_ADamp,&ADamp);
			beh->EnableInputParameter(bbI_ADamp,ADamp);

			DWORD fMask;
			beh->GetLocalParameterValue(bbI_GroupsMask,&fMask);
			beh->EnableInputParameter(bbI_GroupsMask,fMask);


		}
		break;
	}
	return CKBR_OK;

}

