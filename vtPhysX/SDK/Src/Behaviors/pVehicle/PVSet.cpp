#include <StdAfx.h>
#include "pCommon.h"




CKObjectDeclaration	*FillBehaviorPVSetDecl();
CKERROR CreatePVSetProto(CKBehaviorPrototype **pproto);
int PVSet(const CKBehaviorContext& behcontext);
CKERROR PVSetCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;



enum bInputs
{
	I_XML,
	I_Mass,
	I_CMass,
	
	I_MotorForce,
	I_DiffRatio,
	I_TEff,
	I_MaxVelocity,
	
	I_SteerMax,
	I_SteeringSteerPoint,
	I_SteerTurnPoint,
	I_DSDelta,
	
};



#define BB_SSTART 0

BBParameter pInMap[]  = 
{
	
	BB_SPIN(I_XML,VTE_XML_VEHICLE_SETTINGS,"Vehicle Settings","None"),
	BB_SPIN(I_Mass,CKPGUID_FLOAT,"Mass","1000"),
	BB_SPIN(I_CMass,CKPGUID_VECTOR,"Center of Mass","0,0,0"),
	
	BB_SPIN(I_MotorForce,CKPGUID_FLOAT,"Motor Force","2000.0"),
	BB_SPIN(I_DiffRatio,CKPGUID_FLOAT,"Differential Ratio","1.0"),
	BB_SPIN(I_TEff,CKPGUID_FLOAT,"Transmission Efficiency","0.5"),
	BB_SPIN(I_MaxVelocity,CKPGUID_FLOAT,"Max Velocity","1000"),
	
	BB_SPIN(I_SteerMax,CKPGUID_ANGLE,"Maximum Steer","15"),
	BB_SPIN(I_CMass,CKPGUID_VECTOR,"Steering Steer Point","0,0,0"),
	BB_SPIN(I_CMass,CKPGUID_VECTOR,"Steering Turn Point","0,0,0"),
	BB_SPIN(I_DSDelta,CKPGUID_FLOAT,"Digital Steering Delta","0.5"),
	
	

};

#define gVSMap pInMap
//#define BB_LOAD_PIMAP(SOURCE_MAP,SETTING_START_INDEX)	BBHelper::loadPIMap(beh,pIMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX)


CKERROR PVSetCB(const CKBehaviorContext& behcontext)
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

//************************************
// Method:    FillBehaviorPVSetDecl
// FullName:  FillBehaviorPVSetDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPVSetDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVSet");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Modifies vehicle parameter.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xb7e2395,0x7cfe4597));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVSetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVSetProto
// FullName:  CreatePVSetProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVSetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVSet");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! PVSet  

	PVSet is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PVSet.png

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

	//proto->DeclareSetting("Collisions Group",CKPGUID_BOOL,"FALSE");
	/*proto->DeclareSetting("Kinematic",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Mass Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Pivot Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Notify Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Transformation Locks",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Linear Damping",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Angular Damping",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Filter Groups",CKPGUID_BOOL,"FALSE");
*/
	proto->SetBehaviorCallbackFct( PVSetCB );

	
	BB_EVALUATE_SETTINGS(pInMap);

	//----------------------------------------------------------------
	//
	// We just want create the building block pictures
	//
	#ifdef _DOC_ONLY_
		BB_EVALUATE_INPUTS(pInMap);
	#endif // _DOC_ONLY_

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVSet);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PVSet
// FullName:  PVSet
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


#define BBSParameter(A) DWORD s##A;\
	beh->GetLocalParameterValue(A,&s##A)

int PVSet(const CKBehaviorContext& behcontext)
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


		int xmlValue = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_XML));
		float mass = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_Mass));
		float mForce = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_MotorForce));
		float dRatio = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_DiffRatio));
		float steerMax = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_SteerMax));
		float dsDelta = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_DSDelta));
		float tEff = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_TEff));
		float maxVel = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_MaxVelocity));
		VxVector cMass = GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(I_CMass));

		VxVector steeringSteerPoint = GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(I_SteeringSteerPoint));
		VxVector steeringTurnPoint = GetInputParameterValue<VxVector>(beh,BB_IP_INDEX(I_SteerTurnPoint));


		BBSParameter(I_XML);
		BBSParameter(I_Mass);
		BBSParameter(I_MotorForce);
		BBSParameter(I_DiffRatio);
		BBSParameter(I_SteerMax);
		BBSParameter(I_DSDelta);
		BBSParameter(I_MaxVelocity);
		BBSParameter(I_CMass);
		BBSParameter(I_TEff);
		
		BBSParameter(I_SteeringSteerPoint);
		BBSParameter(I_SteerTurnPoint);


		pVehicle *v = body->getVehicle();
		if (!v)
		{
			pVehicleDesc vdesc;
			vdesc.setToDefault();

			//pVehicleGearDesc *gearDesc  = vdesc.getGearDescription();
			//gearDesc->setToCorvette();

			//pVehicleMotorDesc *motorDesc  = vdesc.getMotorDescr();
			//motorDesc->setToCorvette();
			v = pFactory::Instance()->createVehicle(target,vdesc);

		}

		if (!v)
			bbErrorME("Couldn't create vehicle");

		if (sI_Mass) v->setMass(mass);
		if (sI_MotorForce)	v->setMotorForce(mForce);
		if (sI_DiffRatio)v->setDifferentialRatio(dRatio);
		if (sI_SteerMax)v->setMaxSteering(steerMax) ;
		if (sI_DSDelta)v->setDigitalSteeringDelta(dsDelta);
		if (sI_MaxVelocity)v->setMaxVelocity(maxVel);
		if (sI_CMass)v->setCenterOfMass(cMass);
		if (sI_TEff)v->setTransmissionEfficiency(tEff);
		if(sI_SteeringSteerPoint)v->setSteeringSteerPoint(steeringSteerPoint);
		if(sI_SteerTurnPoint)v->setSteeringTurnPoint(steeringTurnPoint );

		
	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PVSetCB
// FullName:  PVSetCB
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



