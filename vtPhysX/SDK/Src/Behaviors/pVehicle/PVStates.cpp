#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPVGetDecl();
CKERROR CreatePVGetProto(CKBehaviorPrototype **pproto);
int PVGet(const CKBehaviorContext& behcontext);
CKERROR PVGetCB(const CKBehaviorContext& behcontext);

using namespace vtTools;
using namespace BehaviorTools;



enum bbI_Inputs
{
	bbI_BodyReference,

};

#define BB_SSTART 0

enum bbOutputs
{
	O_StateFlags,
	O_Acceleration,
	O_Steering,
	O_MTorque,
	O_MPH,
	O_RPM,
	O_MRPM,
	O_WRPM,
	O_Gear,
};

BBParameter pOutMapv[]  = 
{
	BB_SPOUT(O_StateFlags,VTF_VSTATE_FLAGS,"State Flags","0.0"),
	BB_SPOUT(O_Acceleration,CKPGUID_FLOAT,"Acceleration","0.0"),
	BB_SPOUT(O_Steering,CKPGUID_ANGLE,"Steering","0.0"),
	BB_SPOUT(O_MTorque,CKPGUID_FLOAT,"Motor Torque","0.0"),
	BB_SPOUT(O_MPH,CKPGUID_FLOAT,"MPH","0.0"),
	BB_SPOUT(O_RPM,CKPGUID_FLOAT,"RPM","0.0"),
	BB_SPOUT(O_MRPM,CKPGUID_FLOAT,"M - RPM","0.0"),
	BB_SPOUT(O_WRPM,CKPGUID_FLOAT,"Wheel RPM","0.0"),
	BB_SPOUT(O_Gear,CKPGUID_INT,"Gear","0.0"),
};

#define gOPMap pOutMapv


CKERROR PVGetCB(const CKBehaviorContext& behcontext)
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

CKObjectDeclaration	*FillBehaviorPVGetDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVGet");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Retrieves vehicle related parameters.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x70b293c,0x1ef4fa7));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVGetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVGetProto
// FullName:  CreatePVGetProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVGetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVGet");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->SetBehaviorCallbackFct( PVGetCB );


	BB_EVALUATE_SETTINGS(gOPMap);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVGet);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PVGet
// FullName:  PVGet
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PVGet(const CKBehaviorContext& behcontext)
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
		if (!body)
			bbSErrorME(E_PE_NoBody);


		pVehicle *v = body->getVehicle();
		if (!v)
		{
			bbSErrorME(E_PE_NoVeh);
		}

		BB_DECLARE_PMAP;


		/************************************************************************/
		/* retrieve settings state  */
		/*****
		*******************************************************************/

		BBSParameter(O_StateFlags);
		BBSParameter(O_Acceleration);
		BBSParameter(O_Steering);
		BBSParameter(O_MTorque);
		BBSParameter(O_MPH);
		BBSParameter(O_RPM);
		BBSParameter(O_MRPM);
		BBSParameter(O_WRPM);
		BBSParameter(O_Gear);
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/

		BB_O_SET_VALUE_IF(int,O_StateFlags,v->getStateFlags());
		BB_O_SET_VALUE_IF(float,O_Acceleration,v->_cAcceleration);
		BB_O_SET_VALUE_IF(float,O_Steering,v->_cSteering);
		BB_O_SET_VALUE_IF(float,O_MPH,v->getMPH());

		float wRPM = v->_computeRpmFromWheels();
		BB_O_SET_VALUE_IF(float,O_WRPM,wRPM);

		//----------------------------------------------------------------
		//
		// output new engine values
		//
		if (v->isValidEngine())
		{
			BB_O_SET_VALUE_IF(float,O_MTorque,v->getEngine()->GetEngineTorque());
			BB_O_SET_VALUE_IF(float,O_RPM,v->getRPM());
			BB_O_SET_VALUE_IF(int,O_Gear,v->getGear());
		}



		/*
		if (v->getMotor())
		{
			BB_O_SET_VALUE_IF(float,O_MTorque,v->getMotor()->getTorque());
			float mRPM = v->_computeMotorRpm(wRPM);
			BB_O_SET_VALUE_IF(float,O_MRPM,mRPM);
			BB_O_SET_VALUE_IF(float,O_RPM,v->getMotor()->getRpm());
		}

		if (v->getGears())
		{

			BB_O_SET_VALUE_IF(int,O_Gear,v->getGears()->getGear());
		}
		*/
	}
	
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PVGetCB
// FullName:  PVGetCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


