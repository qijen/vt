#include <StdAfx.h>
#include "pCommon.h"
#include "IParameter.h"
#include <xDebugTools.h>


CKObjectDeclaration	*FillBehaviorPVSetBreakSettingsDecl();
CKERROR CreatePVSetBreakSettingsProto(CKBehaviorPrototype **pproto);
int PVSetBreakSettings(const CKBehaviorContext& behcontext);
CKERROR PVSetBreakSettingsCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_XML_Link,
	bbI_BreakFlags,
	bbI_BSmallPressure,
	bbI_BMediumPressure,
	bbI_BHighPressure,
	bbI_BMediumTime,
	bbI_BHighTime,
	bbI_BTableSmall,
	bbI_BTableMedium,
	bbI_BTableHigh,

};

#define BB_SSTART bbI_BTableSmall

BBParameter pInMap233[]  =
{

	BB_PIN(bbI_XML_Link,VTE_BRAKE_XML_LINK,"XML Link","Stub"),
	BB_PIN(bbI_BreakFlags,VTF_BRAKE_FLAGS,"Use Break Table","Use Table"),
	BB_PIN(bbI_BSmallPressure,CKPGUID_FLOAT,"Break Pressure : Small ","0.1f"),
	BB_PIN(bbI_BMediumPressure,CKPGUID_FLOAT,"Break Pressure : Medium ","0.3f"),
	BB_PIN(bbI_BHighPressure,CKPGUID_FLOAT,"Break Pressure : High","1.0f"),
	BB_PIN(bbI_BMediumTime,CKPGUID_TIME,"Medium Apply Time","1.5"),
	BB_PIN(bbI_BHighTime,CKPGUID_TIME,"High Apply Time","3.5"),
	BB_SPIN(bbI_BTableSmall,VTS_BRAKE_TABLE,"Break Table : Small",""),
	BB_SPIN(bbI_BTableMedium,VTS_BRAKE_TABLE,"Break Table : Medium",""),
	BB_SPIN(bbI_BTableHigh,VTS_BRAKE_TABLE,"Break Table : High",""),
};
#define gPIMAP pInMap233

CKObjectDeclaration	*FillBehaviorPVSetBreakSettingsDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVSetBreakSettings");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Sets custom break behavior.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x719d41c2,0x46b76965));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVSetBreakSettingsProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVSetBreakSettingsProto
// FullName:  CreatePVSetBreakSettingsProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVSetBreakSettingsProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVSetBreakSettings");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/* PVSetBreakSettings  

	PVSetBreakSettings is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Registers an object in the physic engine.<br>
	See <A HREF="PVSetBreakSettingsSamples.cmo">PVSetBreakSettings.cmo</A> for example.
	
	<h3>Technical Information</h3>

	\image html PVSetBreakSettings.png

	<SPAN CLASS="in">In: </SPAN>triggers the process <BR>
	
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed <BR>
	
	<BR>
	
	<SPAN CLASS="pin">Target:</SPAN>The 3D Entity associated to the rigid body <BR>
	
*/


	BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(gPIMAP)



	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct( PVSetBreakSettingsCB );
	proto->SetFunction(PVSetBreakSettings);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PVSetBreakSettings
// FullName:  PVSetBreakSettings
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PVSetBreakSettings(const CKBehaviorContext& behcontext)
{

	using namespace vtTools::BehaviorTools;
	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;


	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//	objects		
	//
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	pFactory *pf = pFactory::Instance();

	//the object :
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) bbSErrorME(E_PE_REF);

	//the world reference, optional used
	CK3dEntity*worldRef = NULL;

	//the world object, only used when reference has been specified
	pWorld *world = NULL;

	//final object description 
	pObjectDescr *oDesc = new pObjectDescr();

	pRigidBody *body = NULL;
	pVehicle *v = NULL;

	XString errMesg;

	//----------------------------------------------------------------
	//
	// sanity checks 
	//
	body = GetPMan()->getBody(target);
	if (!body)
		bbSErrorME(E_PE_NoBody);

	v = body->getVehicle();
	if (!v)	
		bbSErrorME(E_PE_NoVeh);

    
	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//		Collecting data. Stores all settings in a pObjectDescr.
	//

	//get the parameter array
	BB_DECLARE_PIMAP;

	v->setBreakFlags(GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_BreakFlags)));
	
	//----------------------------------------------------------------
	//
	// brake pressures
	//
	float pressureSmall = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BSmallPressure));
	iAssertW( pressureSmall >=0.0f && pressureSmall <=1.0f  , _FLT_ASSIGMENT(pressureSmall,0.1f) ,"");

	float pressureMedium = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BMediumPressure));
	iAssertW( pressureMedium>=0.0f && pressureMedium<=1.0f  , _FLT_ASSIGMENT(pressureMedium,0.3f) ,"");

	float pressureHigh = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BHighPressure));
	iAssertW( pressureHigh >=0.0f && pressureHigh<=1.0f  , _FLT_ASSIGMENT(pressureHigh,1.0f) ,"");

	v->setBreakPressure(BL_Small,pressureSmall);
	v->setBreakPressure(BL_Medium,pressureMedium);
	v->setBreakPressure(BL_High,pressureHigh);

	//----------------------------------------------------------------
	//
	// brake table 
	//
	BBSParameterM(bbI_BTableSmall,BB_SSTART);
	if(sbbI_BTableSmall)
		IParameter::Instance()->copyTo(v->mSmallBrakeTable,beh->GetInputParameter(BB_IP_INDEX(bbI_BTableSmall))->GetRealSource());

	BBSParameterM(bbI_BTableMedium,BB_SSTART);
	if(sbbI_BTableMedium)
		IParameter::Instance()->copyTo(v->mMediumBrakeTable,beh->GetInputParameter(BB_IP_INDEX(bbI_BTableMedium))->GetRealSource());

	BBSParameterM(bbI_BTableHigh,BB_SSTART);
	if(sbbI_BTableHigh)
		IParameter::Instance()->copyTo(v->mHighBrakeTable,beh->GetInputParameter(BB_IP_INDEX(bbI_BTableHigh))->GetRealSource());

	//----------------------------------------------------------------
	//
	// brake wait time 
	//
	v->mBrakeMediumThresold = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BMediumTime));
	v->mBrakeHighThresold = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BHighTime));

		//----------------------------------------------------------------
	//
	// error out
	//
	errorFound:
	{
		beh->ActivateOutput(0);
		return CKBR_GENERICERROR;
	}

	//----------------------------------------------------------------
	//
	// All ok
	//
	allOk:
	{
		beh->ActivateOutput(0);
		return CKBR_OK;
	}

	return 0;
}

//************************************
// Method:    PVSetBreakSettingsCB
// FullName:  PVSetBreakSettingsCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PVSetBreakSettingsCB(const CKBehaviorContext& behcontext)
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