#include <StdAfx.h>
#include "pCommon.h"
#include "pVehicleAll.h"

CKObjectDeclaration	*FillBehaviorPVSetExDecl();
CKERROR CreatePVSetExProto(CKBehaviorPrototype **pproto);
int PVSetEx(const CKBehaviorContext& behcontext);
CKERROR PVSetExCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;



enum bInputs
{
	I_XML,
	I_Flags,
	I_PFlags,	
	I_SteerMax,
};



#define BB_SSTART 0

BBParameter pInMapx[]  = 
{
	
	BB_SPIN(I_XML,VTE_XML_VEHICLE_SETTINGS,"Vehicle Settings","None"),
	BB_SPIN(I_Flags,VTF_VFLAGS,"Vehicle Flags",""),
	BB_SPIN(I_PFlags,VTF_VEHICLE_PROCESS_OPTIONS,"Vehicle Process Flags",""),
	BB_SPIN(I_SteerMax,CKPGUID_ANGLE,"Maximum Steer","15"),

};

#define gVSMap pInMapx
//#define BB_LOAD_PIMAP(SOURCE_MAP,SETTING_START_INDEX)	BBHelper::loadPIMap(beh,pIMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX)


CKERROR PVSetExCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	BB_DECLARE_PMAP;
	switch(behcontext.CallbackMessage) 
	{

		
		case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gVSMap,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gVSMap,BB_SSTART);
			break;

		}

		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gVSMap,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;

}
CKObjectDeclaration	*FillBehaviorPVSetExDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVSetEx");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Modifies vehicle parameter.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5b527a19,0x1e1600a9));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVSetExProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}
CKERROR CreatePVSetExProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVSetEx");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetBehaviorCallbackFct( PVSetExCB );

	
	BB_EVALUATE_SETTINGS(gVSMap);

	//----------------------------------------------------------------
	//
	// We just want create the building block pictures
	//
	#ifdef _DOC_ONLY_
		BB_EVALUATE_INPUTS(pInMap);
	#endif // _DOC_ONLY_

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVSetEx);
	*pproto = proto;
	return CK_OK;
}

int PVSetEx(const CKBehaviorContext& behcontext)
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
		if( !target ) bbSErrorME(E_PE_REF);

		
		pRigidBody *body = NULL;
		
		
		BB_DECLARE_PIMAP;

		body = GetPMan()->getBody(target);
		if (!body)
			bbSErrorME(E_PE_NoBody);

	
		float steerMax = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_SteerMax));
		
		
		int flags = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_Flags));
		int pflags = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_PFlags));


		

		BBSParameter(I_XML);
		BBSParameter(I_Flags);
		BBSParameter(I_PFlags);
		BBSParameter(I_SteerMax);



		pVehicle *v = body->getVehicle();
		if (!v)
		{
			pVehicleDesc vdesc;
			vdesc.setToDefault();

			if (sI_SteerMax)
			{
				vdesc.steeringMaxAngle = steerMax  * RR_RAD_DEG_FACTOR ;
			}
			if (sI_PFlags)
			{
				vdesc.processFlags = pflags;
			}

			v = pFactory::Instance()->createVehicle(target,vdesc);

		}

		if (!v)
			bbErrorME("Couldn't create vehicle");


		if (sI_SteerMax)
			v->getSteer()->setLock(steerMax * RR_RAD_DEG_FACTOR);
		

		if (sI_PFlags)v->setProcessOptions(pflags);



		/*
		if (sI_Mass) v->setMass(mass);
		if (sI_Flags) v->flags  = flags;
		if (sI_MotorForce)	v->setMotorForce(mForce);
		if (sI_DiffRatio)v->setDifferentialRatio(dRatio);
		if (sI_SteerMax)v->setMaxSteering(steerMax) ;
		if (sI_DSDelta)v->setDigitalSteeringDelta(dsDelta);
		if (sI_MaxVelocity)v->setMaxVelocity(maxVel);
		if (sI_CMass)v->setCenterOfMass(cMass);
		if (sI_TEff)v->setTransmissionEfficiency(tEff);
		if(sI_SteeringSteerPoint)v->setSteeringSteerPoint(steeringSteerPoint);
		if(sI_SteerTurnPoint)v->setSteeringTurnPoint(steeringTurnPoint );
		*/



		
	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PVSetExCB
// FullName:  PVSetExCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
/*

dx	-0.99999821	float
angle	-0.21089047	float
zPos	0.99999857	float
dz	4.6712832	float
xPos	-0.99999821	float
atan3	-0.21089047	float


dx	1.0000019	float
angle	0.21089132	float
zPos	1.0000004	float
dz	4.6712813	float
xPos	1.0000019	float
atan3	0.21089132	float




*/

/*
//				int indexA = BBHelper::getIndex(beh,pMap,bbI_AxleSpeed);
//				int indexB = BBHelper::getIndex(beh,pMap,bbI_Steer);

/*
EnablePInputBySettings(beh,bbI_AxleSpeed);
EnablePInputBySettings(beh,bbI_Steer);
EnablePInputBySettings(beh,bbI_MotorTorque);
EnablePInputBySettings(beh,bbI_BrakeTorque);
EnablePInputBySettings(beh,bbI_SuspensionSpring);
EnablePInputBySettings(beh,bbI_SuspensionTravel);
EnablePInputBySettings(beh,bbI_Radius);
*/




/*
		case CKM_BEHAVIORDETACH:
			{

				/*				BB_DECLARE_PIMAP;
				while (	pIMap->getArray().size() )
				{

				BBParameter *p=pIMap->getArray().at(0);
				pIMap->getArray().erase(pIMap->getArray().begin());
				delete p;
				p = NULL;
				}
				break;
				*/
			//				BB_DECLARE_PIMAP;
			//				BBHelper::destroyPIMap(beh,pIMap,BB_PMAP_SIZE);



