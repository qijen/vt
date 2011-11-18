#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPVWGetDecl();
CKERROR CreatePVWGetProto(CKBehaviorPrototype **pproto);
int PVWGet(const CKBehaviorContext& behcontext);
CKERROR PVWGetCB(const CKBehaviorContext& behcontext);

using namespace vtTools;
using namespace BehaviorTools;



enum bbI_Inputs
{
	bbI_BodyReference,

};

#define BB_SSTART 0

enum bbOutputs
{
	O_AxleSpeed,
	O_WheelRollAngle,
	O_RPM,
	O_SteerAngle,
	O_MotorTorque,
	O_BreakTorque,
	O_Suspension,
	O_SuspensionTravel,
	O_Radius,
	O_WFlags,
	O_WSFlags,
	O_LatFunc,
	O_LongFunc,
	O_Contact,

};

BBParameter pOutMap[]  = 
{
	BB_SPOUT(O_AxleSpeed,CKPGUID_FLOAT,"Axle Speed","0.0"),
	BB_SPOUT(O_WheelRollAngle,CKPGUID_FLOAT,"Roll Angle","0.0"),
	BB_SPOUT(O_RPM,CKPGUID_FLOAT,"RPM","0.0"),
	BB_SPOUT(O_SteerAngle,CKPGUID_ANGLE,"Steer Angle","0.0"),
	BB_SPOUT(O_MotorTorque,CKPGUID_FLOAT,"Motor Torque","0.0"),
	BB_SPOUT(O_BreakTorque,CKPGUID_FLOAT,"Break Torque","0.0"),
	BB_SPOUT(O_Suspension,VTS_JOINT_SPRING,"Suspension Spring","0.0"),
	BB_SPOUT(O_SuspensionTravel,CKPGUID_FLOAT,"Suspension Spring","0.0"),
	BB_SPOUT(O_Radius,CKPGUID_FLOAT,"Radius","0.0"),
	BB_SPOUT(O_WFlags,VTS_PHYSIC_WHEEL_FLAGS,"Wheel Flags","0.0"),
	BB_SPOUT(O_WSFlags,VTF_VWSHAPE_FLAGS,"Wheel Shape Flags","0.0"),
	BB_SPOUT(O_LatFunc,VTF_VWTIRE_SETTINGS,"Lateral Force Settings","0.0"),
	BB_SPOUT(O_LongFunc,VTF_VWTIRE_SETTINGS,"Longitudinal Force Settings","0.0"),
	BB_SPOUT(O_Contact,VTS_WHEEL_CONTACT,"Contact Data","0.0"),
	//BB_SPOUT(O_LongFunc,VTF_VWTIRE_SETTINGS,"Longitudinal Force Settings","0.0"),
};

#define gOPMap pOutMap


CKERROR PVWGetCB(const CKBehaviorContext& behcontext)
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

CKObjectDeclaration	*FillBehaviorPVWGetDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVWGet");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Retrieves wheel related parameters.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x24b704e4,0x65ed5fad));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVWGetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVWGetProto
// FullName:  CreatePVWGetProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVWGetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVWGet");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*! \page PVWGet  

	PVWGet is categorized in \ref Vehicle

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Retrieves various physical informations.<br>

	<h3>Technical Information</h3>

	\image html PVWGet.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pout">Collisions Group: </SPAN>Which collision group this body is part of.See pRigidBody::getCollisionsGroup().
	<BR>
	<SPAN CLASS="pout">Kinematic Object: </SPAN>The kinematic state. See pRigidBody::isKinematic().
	<BR>
	<SPAN CLASS="pout">Gravity: </SPAN>The gravity state.See pRigidBody::isAffectedByGravity().
	<BR>
	<SPAN CLASS="pout">Collision: </SPAN>Determines whether the body reacts to collisions.See pRigidBody::isCollisionEnabled(). 
	<BR>
	<BR>
	<SPAN CLASS="setting">Collisions Group: </SPAN>Enables output for collisions group.
	<BR>
	<SPAN CLASS="setting">Kinematic Object: </SPAN>Enables output for kinematic object.
	<BR>
	<SPAN CLASS="setting">Gravity: </SPAN>Enables output for gravity.
	<BR>
	<SPAN CLASS="setting">Collision: </SPAN>Enables output for collision. 
	<BR>
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
		\include PBGetEx.cpp
	</SPAN>
	*/
	

	//proto->DeclareInParameter("World Reference",CKPGUID_3DENTITY,"pDefaultWorld");
	//proto->DeclareInParameter("Body Reference",CKPGUID_3DENTITY,"Body");

	
	/*proto->DeclareSetting("Collisions Group",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Kinematic",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Mass Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Pivot Offset",CKPGUID_BOOL,"FALSE");

	
	
	proto->DeclareOutParameter("Collisions Group",CKPGUID_INT,"0");
	proto->DeclareOutParameter("Kinematic Object",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Mass Offset",CKPGUID_VECTOR,"0.0f");
	proto->DeclareOutParameter("Pivot Offset",CKPGUID_VECTOR,"0.0f");

	*/

	proto->SetBehaviorCallbackFct( PVWGetCB );


	BB_EVALUATE_SETTINGS(gOPMap);

	//----------------------------------------------------------------
	//
	// We just want create the building block pictures
	//
	#ifdef _DOC_ONLY_
		BB_EVALUATE_OUTPUTS(gOPMap);
	#endif // _DOC_ONLY_



	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVWGet);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PVWGet
// FullName:  PVWGet
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PVWGet(const CKBehaviorContext& behcontext)
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


		
		pWheel *wheel = body->getWheel(target);
		if (!wheel)bbErrorME("pWheel object doesnt exist!");

		pWheel2 *wheel2  = wheel->castWheel2();

		//if (!wheel2)bbErrorME("Couldnt cast a pWheel2 object");

		BB_DECLARE_PMAP;


		/************************************************************************/
		/* retrieve settings state  */
		/*****
		*******************************************************************/

		BBSParameter(O_AxleSpeed);
		BBSParameter(O_WheelRollAngle);
		BBSParameter(O_RPM);
		BBSParameter(O_SteerAngle);
		BBSParameter(O_MotorTorque);
		BBSParameter(O_BreakTorque);
		BBSParameter(O_Suspension);
		BBSParameter(O_SuspensionTravel);
		BBSParameter(O_Radius);
		BBSParameter(O_WFlags);
		BBSParameter(O_WSFlags);
		BBSParameter(O_LatFunc);
		BBSParameter(O_LongFunc);
		BBSParameter(O_Contact);

		BB_O_SET_VALUE_IF(float,O_AxleSpeed,wheel2->getAxleSpeed());
		BB_O_SET_VALUE_IF(float,O_WheelRollAngle,wheel->getWheelRollAngle());
		BB_O_SET_VALUE_IF(float,O_RPM,wheel2->getRpm());
		
		if (wheel2)
		{
			BB_O_SET_VALUE_IF(float,O_SteerAngle,wheel2->GetHeading());
		}

		/*
		BB_O_SET_VALUE_IF(float,O_SteerAngle,wheel2->rotationV.x);*/
		BB_O_SET_VALUE_IF(float,O_MotorTorque,wheel2->getWheelShape()->getMotorTorque());
		BB_O_SET_VALUE_IF(float,O_BreakTorque,wheel2->getWheelShape()->getBrakeTorque());
		BB_O_SET_VALUE_IF(float,O_SuspensionTravel,wheel2->getWheelShape()->getSuspensionTravel());
		BB_O_SET_VALUE_IF(float,O_Radius,wheel2->getWheelShape()->getRadius());

		BB_O_SET_VALUE_IF(int,O_WFlags,wheel->mWheelFlags);
		BB_O_SET_VALUE_IF(int,O_WSFlags,wheel2->getWheelShape()->getWheelFlags());



		if (wheel2){

			if (sO_LatFunc)
			{
				NxTireFunctionDesc tf = wheel2->getWheelShape()->getLateralTireForceFunction();
				CKParameterOut *pout = beh->GetOutputParameter(BB_OP_INDEX(O_LatFunc));
				if (pout)
				{
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,1,tf.extremumSlip);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,2,tf.extremumValue);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,3,tf.asymptoteSlip);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,4,tf.asymptoteValue);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,5,tf.stiffnessFactor);
				}
			}
			if (sO_LongFunc)
			{
				NxTireFunctionDesc tf = wheel2->getWheelShape()->getLongitudalTireForceFunction();
				CKParameterOut *pout = beh->GetOutputParameter(BB_OP_INDEX(O_LongFunc));
				if (pout)
				{
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,1,tf.extremumSlip);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,2,tf.extremumValue);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,3,tf.asymptoteSlip);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,4,tf.asymptoteValue);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,5,tf.stiffnessFactor);
				}
			}

		}

		if (wheel2)
		{
			if (sO_Suspension)
			{
				NxSpringDesc s = wheel2->getWheelShape()->getSuspension();
				CKParameterOut *pout = beh->GetOutputParameter(BB_OP_INDEX(O_Suspension));
				if (pout)
				{
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,0,s.damper);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,1,s.spring);
					vtTools::ParameterTools::SetParameterStructureValue<float>(pout,2,s.targetValue);
				}
			}

			if (sO_Contact)
			{

				pWheelContactData cData = *wheel2->getContact();
				CKParameterOut *pout = beh->GetOutputParameter(BB_OP_INDEX(O_Contact));
				if (pout)
				{
					pFactory::Instance()->copyTo(pout,cData);
				}
				
			}
		}
	}
	
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PVWGetCB
// FullName:  PVWGetCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
