#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPWSetFilteringDecl();
CKERROR CreatePWSetFilteringProto(CKBehaviorPrototype **pproto);
int PWSetFiltering(const CKBehaviorContext& behcontext);
CKERROR PWSetFilteringCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPWSetFilteringDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PWSetFiltering");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Sets the worlds filter settings for contact generation.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7ae099f,0x1f8e6949));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePWSetFilteringProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePWSetFilteringProto
// FullName:  CreatePWSetFilteringProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
enum bInput
{

	bbI_FBool,
	bbI_FOp0,
	bbI_FOp1,
	bbI_FOp2,
	bbI_FConst0,
	bbI_FConst1,


};
CKERROR CreatePWSetFilteringProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PWSetFiltering");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PWSetFiltering  

	PWSetFiltering is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Sets the worlds filter settings for contact generation.<br>
	See <A HREF="pWFiltering.cmo">pWFiltering.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PWSetFiltering.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>

	<SPAN CLASS="pin">Target: </SPAN>The world reference. Choose pDefaultWord if there are no multiple worlds in use.
	<BR>
	
	<SPAN CLASS="pin">Filter Bool: </SPAN>Setups filtering's boolean value.
	<BR>

	<SPAN CLASS="pin">Filter Op 0: </SPAN>Setups filtering operation.
	<BR>
	<SPAN CLASS="pin">Filter Op 1: </SPAN>Setups filtering operation.
	<BR>
	<SPAN CLASS="pin">Filter Op 2: </SPAN>Setups filtering operation.
	<BR>
	<SPAN CLASS="pin">Filter Constant 0: </SPAN>Setups filtering's K0 value. 
	<BR>
	<SPAN CLASS="pin">Filter Constant 1: </SPAN>Setups filtering's K1 value. 
	<BR>

	<h3>Note</h3><br>

	See \ref CollisionFiltering for more details. 
	
	<br>
	<br>

	Is utilizing #pWorld .<br>
	
	*/
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Filter Bool",CKPGUID_BOOL,"true");
	proto->DeclareInParameter("Filter Op 0",VTE_FILTER_OPS,"Or");
	proto->DeclareInParameter("Filter Op 1",VTE_FILTER_OPS,"Or");
	proto->DeclareInParameter("Filter Op 2",VTE_FILTER_OPS,"And");
	proto->DeclareInParameter("Filter Constant 0",VTS_FILTER_GROUPS,"0");
	proto->DeclareInParameter("Filter Constant 1",VTS_FILTER_GROUPS,"0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PWSetFiltering);
	*pproto = proto;
	return CK_OK;
}
enum bOutputs
{
	bbO_None,
	bbO_Enter,
	bbO_Stay,
	bbO_Leave,
};

//************************************
// Method:    PWSetFiltering
// FullName:  PWSetFiltering
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PWSetFiltering(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	using namespace vtTools::ParameterTools;

	

	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) return CKBR_OWNERERROR;


	//////////////////////////////////////////////////////////////////////////
	// the world :  
	pWorld *world=GetPMan()->getWorld(target->GetID());
	if (!world)
	{
		beh->ActivateOutput(0);
		return 0;
	}

	NxScene *scene =  world->getScene();
	if (!scene)
	{
		return 0;
	}

	int filterBool = GetInputParameterValue<int>(beh,bbI_FBool);
	int fOP0  =  GetInputParameterValue<int>(beh,bbI_FOp0);
	int fOP1  =  GetInputParameterValue<int>(beh,bbI_FOp1);
	int fOP2  =  GetInputParameterValue<int>(beh,bbI_FOp2);

	CKParameter *fConst0 = beh->GetInputParameter(bbI_FConst0)->GetRealSource();

	NxGroupsMask mask0;
	mask0.bits0  = GetValueFromParameterStruct<int>(fConst0,0);
	mask0.bits1  = GetValueFromParameterStruct<int>(fConst0,1);
	mask0.bits2  = GetValueFromParameterStruct<int>(fConst0,2);
	mask0.bits3  = GetValueFromParameterStruct<int>(fConst0,3);

	CKParameter *fConst1 = beh->GetInputParameter(bbI_FConst1)->GetRealSource();

	NxGroupsMask mask1;
	mask1.bits0  = GetValueFromParameterStruct<int>(fConst1,0);
	mask1.bits1  = GetValueFromParameterStruct<int>(fConst1,1);
	mask1.bits2  = GetValueFromParameterStruct<int>(fConst1,2);
	mask1.bits3  = GetValueFromParameterStruct<int>(fConst1,3);


	scene->setFilterBool(filterBool);
	scene->setFilterOps((NxFilterOp)fOP0,(NxFilterOp)fOP1,(NxFilterOp)fOP2);
	
	scene->setFilterConstant0(mask0);
	scene->setFilterConstant1(mask1);

	//scene->raycastAnyShape()

	beh->ActivateOutput(0);



	return 0;
}

//************************************
// Method:    PWSetFilteringCB
// FullName:  PWSetFilteringCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PWSetFilteringCB(const CKBehaviorContext& behcontext)
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

