#include <StdAfx.h>	
#include "pCommon.h"




CKObjectDeclaration	*FillBehaviorPVWSetDecl();
CKERROR CreatePVWSetProto(CKBehaviorPrototype **pproto);
int PVWSet(const CKBehaviorContext& behcontext);
CKERROR PVWSetCB(const CKBehaviorContext& behcontext);


enum bInputs
{
	bbI_XML,
	bbI_AxleSpeed,
	bbI_Steer,
	bbI_MotorTorque,
	bbI_BrakeTorque,
	bbI_SuspensionSpring,
	bbI_SuspensionTravel,
	bbI_Radius,
	bbI_WFlags,
	bbI_WSFlags,
	bbI_LatFunc,
	bbI_LongFunc,
};


#define BB_SSTART 0

BBParameter pInMap2[]  =
{

	BB_SPIN(bbI_XML,VTE_XML_VEHICLE_SETTINGS,"XML Link","None"),
	BB_SPIN(bbI_AxleSpeed,CKPGUID_FLOAT,"Axle Speed","2000.0"),
	BB_SPIN(bbI_Steer,CKPGUID_ANGLE,"Steer Angle","1.0"),
	BB_SPIN(bbI_MotorTorque,CKPGUID_FLOAT,"Motor Torque","0.0"),
	BB_SPIN(bbI_BrakeTorque,CKPGUID_FLOAT,"Break Torque","0.0"),
	BB_SPIN(bbI_SuspensionSpring,VTS_JOINT_SPRING,"Suspension Spring",""),
	BB_SPIN(bbI_SuspensionTravel,CKPGUID_FLOAT,"Suspension Travel","0.0"),
	BB_SPIN(bbI_Radius,CKPGUID_FLOAT,"Radius","1.0"),
	BB_SPIN(bbI_WFlags,VTS_PHYSIC_WHEEL_FLAGS,"Wheel Flags",""),
	BB_SPIN(bbI_WSFlags,VTF_VWSHAPE_FLAGS,"Wheel ShapeFlags",""),
	BB_SPIN(bbI_LatFunc,VTF_VWTIRE_SETTINGS,"Latitude Force Settings",""),
	BB_SPIN(bbI_LongFunc,VTF_VWTIRE_SETTINGS,"Longitudinal Force Settings","0.0"),

};

#define gPIMAP pInMap2

//************************************
// Method:    FillBehaviorPVWSetDecl
// FullName:  FillBehaviorPVWSetDecl
// Access:    public
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPVWSetDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVWSet");
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Sets physical quantities.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7805147,0x322e17e7));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVWSetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePVWSetProto
// FullName:  CreatePVWSetProto
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVWSetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVWSet");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*! \page PVWSet

	PVWSet is categorized in \ref Vehicle

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PVWSet.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
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

	
	proto->SetBehaviorCallbackFct( PVWSetCB );

	BB_EVALUATE_SETTINGS(pInMap2);

	//----------------------------------------------------------------
	//
	// We just want create the building block pictures
	//
	#ifdef _DOC_ONLY_
		BB_EVALUATE_INPUTS(pInMap2);
	#endif // _DOC_ONLY_
	


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PVWSet);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PVWSet
// FullName:  PVWSet
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PVWSet(const CKBehaviorContext& behcontext)
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


		body = GetPMan()->getBody(target);
		if (!body)	bbErrorME("No Reference Object specified");


		pWheel *wheel = body->getWheel(target);
		if (!wheel)bbErrorME("pWheel object doesnt exist!");

		pWheel2 *wheel2  = wheel->castWheel2();
		if (!wheel2)bbErrorME("Couldnt cast a pWheel2 object");



		BB_DECLARE_PIMAP;


		/************************************************************************/
		/* engel 
		kuehne un partner 
		*/
		/************************************************************************/

		/************************************************************************/
		/* retrieve settings state  */
		/************************************************************************/
		BBSParameter(bbI_XML);
		BBSParameter(bbI_AxleSpeed);
		BBSParameter(bbI_Steer);
		BBSParameter(bbI_MotorTorque);
		BBSParameter(bbI_BrakeTorque);
		BBSParameter(bbI_SuspensionSpring);
		BBSParameter(bbI_SuspensionTravel);
		BBSParameter(bbI_Radius);

		BBSParameter(bbI_WFlags);
		BBSParameter(bbI_WSFlags);

		BBSParameter(bbI_LatFunc);
		BBSParameter(bbI_LongFunc);


		/************************************************************************/
		/* retrieve values                                                                     */
		/************************************************************************/

		int xmlValue = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_XML));
		float axleVel = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_AxleSpeed));
		float steer = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_Steer));
		float mTorque = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_MotorTorque));
		float bTorque = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_BrakeTorque));
		float suspensionTravel = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_SuspensionTravel));

		int wFlags = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_WFlags));
		int wSFlags = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_WSFlags));

		pSpring sSpring;
		if (sbbI_SuspensionSpring)
		{
			CKParameterIn *par = beh->GetInputParameter(bbI_SuspensionSpring);
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					sSpring  = pFactory::Instance()->createSpringFromParameter(rPar);
					NxSpringDesc xsp;
					xsp.damper = sSpring.damper;
					xsp.spring = sSpring.spring;
					xsp.targetValue = sSpring.targetValue;
					if (xsp.isValid())
					{
						wheel2->setSuspensionSpring(sSpring);
					}else
						bbErrorME("Invalid Spring Setings!");

				}
			}
		}

		/************************************************************************/
		/* Update Object !                                                                     */
		/************************************************************************/

		//////////////////////////////////////////////////////////////////////////
		//	load some settings from XML
		if(sbbI_XML)
		{
		}

		if (sbbI_Steer)wheel2->setAngle(steer);
		if (sbbI_AxleSpeed)wheel2->setAxleSpeed(axleVel);
		if (sbbI_MotorTorque)wheel2->setMotorTorque(mTorque);
		if (sbbI_BrakeTorque)wheel2->setBreakTorque(bTorque);
		if (sbbI_SuspensionSpring)wheel2->setSuspensionSpring(sSpring);
		if (sbbI_SuspensionTravel)wheel2->setSuspensionTravel(suspensionTravel);
		if (sbbI_WFlags)
		{

			//////////////////////////////////////////////////////////////////////////
			//
			//
			wheel2->setFlags(wFlags);

		}


		if (sbbI_WSFlags)
		{
			wheel2->getWheelShape()->setWheelFlags(wSFlags);
		}


		if (sbbI_LatFunc)
		{

			CKParameterIn *par = beh->GetInputParameter(BB_IP_INDEX(bbI_LatFunc));
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					pTireFunction func  = pFactory::Instance()->createTireFuncFromParameter(rPar);
					if (func.isValid())
					{
						NxTireFunctionDesc xFn;
						xFn.asymptoteSlip = func.asymptoteSlip;
						xFn.asymptoteValue = func.asymptoteValue;
						xFn.extremumSlip= func.extremumSlip;
						xFn.extremumValue= func.extremumValue;
						xFn.stiffnessFactor= func.stiffnessFactor;
						wheel2->getWheelShape()->setLongitudalTireForceFunction(xFn);
					}else
						bbErrorME("Invalid Tire Function Settings!");
				}
			}
		}

		if (sbbI_LongFunc)
		{

			CKParameterIn *par = beh->GetInputParameter(BB_IP_INDEX(bbI_LongFunc));
			if (par)
			{
				CKParameter *rPar = par->GetRealSource();
				if (rPar)
				{
					pTireFunction func  = pFactory::Instance()->createTireFuncFromParameter(rPar);
					if (func.isValid())
					{
						NxTireFunctionDesc xFn;
						xFn.asymptoteSlip = func.asymptoteSlip;
						xFn.asymptoteValue = func.asymptoteValue;
						xFn.extremumSlip= func.extremumSlip;
						xFn.extremumValue= func.extremumValue;
						xFn.stiffnessFactor= func.stiffnessFactor;
						wheel2->getWheelShape()->setLongitudalTireForceFunction(xFn);
					}else
						bbErrorME("Invalid Tire Function Settings!");
				}
			}
		}
	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PVWSetCB
// FullName:  PVWSetCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PVWSetCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) {

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



