#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPBGetParameterDecl();
CKERROR CreatePBGetParameterProto(CKBehaviorPrototype **pproto);
int PBGetParameter(const CKBehaviorContext& behcontext);
CKERROR PBGetParameterCB(const CKBehaviorContext& behcontext);

using namespace vtTools;
using namespace BehaviorTools;



enum bbI_Inputs
{
	bbI_BodyReference,

};

#define BB_SSTART 0


enum bInputs
{

	bbO_CollisionGroup,
	bbO_GroupsMask,
	bbO_Flags,
	bbO_TFlags,
	bbO_LinDamp,
	bbO_AngDamp,
	bbO_MassOffset,
	bbO_ShapeOffset,

};
BBParameter pOutMap12[]  = 
{
	BB_SPOUT(bbO_CollisionGroup,CKPGUID_INT,"Collision Group","0.0"),
	BB_SPOUT(bbO_GroupsMask,VTS_FILTER_GROUPS,"Groups Mask",""),
	BB_SPOUT(bbO_Flags,VTF_BODY_FLAGS,"Body Flags",""),
	BB_SPOUT(bbO_TFlags,VTF_BODY_TRANS_FLAGS,"Transformation Lock Flags",""),
	BB_SPOUT(bbO_LinDamp,CKPGUID_FLOAT,"Linear Damping","0.0"),
	BB_SPOUT(bbO_AngDamp,CKPGUID_FLOAT,"Angular Damping","0.0"),
	BB_SPOUT(bbO_MassOffset,CKPGUID_VECTOR,"Mass Offset","0.0"),
	BB_SPOUT(bbO_ShapeOffset,CKPGUID_VECTOR,"Pivot Offset","0.0"),

};

#define gOPMap pOutMap12


CKERROR PBGetParameterCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	int cb = behcontext.CallbackMessage;
	BB_DECLARE_PMAP;
	switch(behcontext.CallbackMessage) {



		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
			{
				BB_LOAD_POMAP(gOPMap,BB_SSTART);
				break;
			}
		case CKM_BEHAVIORDETACH:
			{
				BB_DESTROY_PMAP;
				break;
			}

		case CKM_BEHAVIORATTACH:
			{
				BB_INIT_PMAP(gOPMap,BB_SSTART);
				break;

			}

		case CKM_BEHAVIORSETTINGSEDITED:
			{
				BB_REMAP_PMAP(gOPMap,BB_SSTART);
				break;
			}
	}
	return CKBR_OK;
}

CKObjectDeclaration	*FillBehaviorPBGetParameterDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBGetPar");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Retrieves rigid bodies parameters");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x59a670a,0x59a557ec));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBGetParameterProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBGetParameterProto
// FullName:  CreatePBGetParameterProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBGetParameterProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBGetPar");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*
	PBGetPar
	PBGetPar is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Retrieves various physical informations.<br>

	<h3>Technical Information</h3>

	\image html PBGetPar.png

	
	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pout">Collisions Group: </SPAN>Which collision group this body or the sub shape is part of.See pRigidBody::getCollisionsGroup().
	<BR>
	<SPAN CLASS="pout">Groupsmask: </SPAN>Sets 128-bit mask used for collision filtering. It can be sub shape specific.See comments for ::pGroupsMask and pRigidBody::setGroupsMask()
	<BR>
	<SPAN CLASS="pout">Flags: </SPAN>The current flags. See pRigidBody::getFlags() 
	<br>
	<SPAN CLASS="pout">Transformation Locks: </SPAN>Current transformations locks.
	<BR>
	<SPAN CLASS="pout">Linear Damping: </SPAN>The linear damping scale.
	<BR>
	<SPAN CLASS="pout">Angular Damping: </SPAN>The linear damping scale.
	<BR>
	<SPAN CLASS="pout">Mass Offset: </SPAN>The mass center in the bodies local space.
	<BR>
	<SPAN CLASS="pout">Pivot Offset: </SPAN>The shapes position in the bodies local space. See pRigidBody::getLocalShapePosition().
	<BR>

	<h3>Warning</h3>

	*/

	proto->SetBehaviorCallbackFct( PBGetParameterCB );


	BB_EVALUATE_SETTINGS(gOPMap);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBGetParameter);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBGetParameter
// FullName:  PBGetParameter
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBGetParameter(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) 	bbErrorME("No Reference Object specified");

		pRigidBody *body = NULL;


		body = GetPMan()->getBody(target);
		if (!body)	bbErrorME("No Reference Object specified");

		BB_DECLARE_PMAP;

		BBSParameter(bbO_CollisionGroup);
		BBSParameter(bbO_GroupsMask);
		
		BBSParameter(bbO_Flags);
		BBSParameter(bbO_TFlags);
		
		BBSParameter(bbO_LinDamp);
		BBSParameter(bbO_AngDamp);
		
		BBSParameter(bbO_MassOffset);
		BBSParameter(bbO_ShapeOffset);

		body->recalculateFlags(0);
		
		BB_O_SET_VALUE_IF(int,bbO_CollisionGroup,body->getCollisionsGroup(target));
		BB_O_SET_VALUE_IF(int,bbO_Flags,body->getFlags());
		BB_O_SET_VALUE_IF(int,bbO_TFlags,body->getTransformationsLockFlags());
		BB_O_SET_VALUE_IF(float,bbO_LinDamp,body->getLinearDamping());
		BB_O_SET_VALUE_IF(float,bbO_AngDamp,body->getAngularDamping());
		BB_O_SET_VALUE_IF(VxVector,bbO_MassOffset,body->getMassOffset());
		BB_O_SET_VALUE_IF(VxVector,bbO_ShapeOffset,body->getPivotOffset(target));

		if (sbbO_GroupsMask)
		{
			pGroupsMask gMask  =  body->getGroupsMask(target);
			CKParameterOut *poutMask = beh->GetOutputParameter(BB_OP_INDEX(bbO_GroupsMask));

			if (poutMask)
			{
				pFactory::Instance()->copyTo(poutMask,gMask);
			}
		}
	}
	
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PBGetParameterCB
// FullName:  PBGetParameterCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


