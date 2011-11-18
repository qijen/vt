#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPVControlDecl();
CKERROR CreatePVControlProto(CKBehaviorPrototype **pproto);
int PVControl(const CKBehaviorContext& behcontext);
CKERROR PVControlCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;



enum bInTrigger
{
	TI_DO,
};

enum bOutputs
{

	O_Flags,
	O_Gear,
	O_RPM,
	O_RPM_WHEELS,
	O_RATIO,


};


enum bInputs
{
	I_Acc,
	I_Steer,
	I_AnalogAcc,
	I_AnalogSteering,
	I_Handbrake,
		
};



#define BB_SSTART 0

BBParameter pInMap3[]  = 
{
	
	BB_SPIN(I_Acc,CKPGUID_FLOAT,"Acceleration","0"),
	BB_SPIN(I_Steer,CKPGUID_FLOAT,"Steering","0"),
	BB_SPIN(I_AnalogAcc,CKPGUID_BOOL,"Analog Acceleration","0"),
	BB_SPIN(I_AnalogSteering,CKPGUID_BOOL,"Analog Steering","0"),
	BB_SPIN(I_Handbrake,CKPGUID_BOOL,"Handbrake","0"),


};

#define BBIMAP pInMap3

CKERROR PVControlCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(BBIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(BBIMAP,BB_SSTART);
			break;

		}

		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(BBIMAP,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;

}

//************************************
// Method:    FillBehaviorPVControlDecl
// FullName:  FillBehaviorPVControlDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPVControlDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVControl");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Controls a vehicle by real values instead of input trigger");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x550d3ca3,0x27f10ac3));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVControlProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVControlProto
// FullName:  CreatePVControlProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVControlProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVControl");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/* PVControl  

	PVControl is categorized in \ref Vehicles

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Controls a vehicle by real values instead of input trigger.<br>

	<h3>Technical Information</h3>

	\image html PVControl.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the vehicle. The entity needs to be physicalized before. It needs to have at least one wheel in its hierarchy. 
	<BR>
	<BR>
	<SPAN CLASS="pin">Acceleration: </SPAN>The acceleration for this frame. <br>
	<b>Range:</b> [0,1]
	<br>

	<SPAN CLASS="pin">Steering: </SPAN>The steering for this frame. <br>
	<b>Range:</b> [-1,1]
	<BR>

	<SPAN CLASS="pin">Analog Acceleration: </SPAN>Switches between analog and digital acceleration. <br>
	<b>Range:</b> TRUE,FALSE]
	<BR>

	<SPAN CLASS="pin">Analog Steering: </SPAN>Switches between analog and digital steering. <br>
	<b>Range:</b> TRUE,FALSE]
	<BR>

	<SPAN CLASS="pin">Handbrake: </SPAN>Sets the handbrake status to enabled for this frame. <br>
	<b>Range:</b> TRUE,FALSE]
	<BR>

	<SPAN CLASS="setting">Acceleration: </SPAN>Enables input for acceleration.
	<BR>

	<SPAN CLASS="setting">Steering: </SPAN>Enables input for steering.
	<BR>

	<SPAN CLASS="setting">Analog Acceleration: </SPAN>Enables input for analog acceleration.
	<BR>

	<SPAN CLASS="setting">Analog Steering: </SPAN>Enables input for analog steering.
	<BR>

	<SPAN CLASS="setting">Handbrake: </SPAN>Enables input for handbrake.
	<BR>
	
	<BR>
	<h3>Warning</h3>
		The body must be dynamic. <br>
		The body must have a vehicle object. See #ref PVSet
	<br>

	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	
	
	*/



	proto->SetBehaviorCallbackFct( PVControlCB );
	
	proto->DeclareOutParameter("Flags",VTF_VSTATE_FLAGS,"-1");
	proto->DeclareOutParameter("Gear",CKPGUID_INT,"-1");
	proto->DeclareOutParameter("RPM",CKPGUID_FLOAT,"-1");
	proto->DeclareOutParameter("RPM Wheels",CKPGUID_FLOAT,"-1");
	proto->DeclareOutParameter("Motor Ratio",CKPGUID_FLOAT,"-1");

	
	BB_EVALUATE_SETTINGS(BBIMAP);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVControl);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PVControl
// FullName:  PVControl
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


#define BBSParameter(A) DWORD s##A;\
	beh->GetLocalParameterValue(A,&s##A)

int PVControl(const CKBehaviorContext& behcontext)
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



		float acc = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_Acc));
		int accAnalog = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_AnalogAcc));
		float steer = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_Steer));
		int steerAnalog = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_AnalogSteering));
		int handBrake = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_Handbrake));
		

		BBSParameter(I_Acc);
		BBSParameter(I_AnalogAcc);
		BBSParameter(I_Steer);
		BBSParameter(I_AnalogSteering);
		BBSParameter(I_Handbrake);

		pVehicle *v = body->getVehicle();
		if (!v)
		{
			bbSErrorME(E_PE_NoVeh);
		}

		if (sI_Acc && acc!=v->_cAcceleration)	
			v->setControlState(E_VCS_ACCELERATION,acc);
		
		if (sI_AnalogAcc && accAnalog!=v->_cAnalogAcceleration)	
			v->setControlMode(E_VCS_ACCELERATION,accAnalog ? E_VCSM_ANALOG : E_VCSM_DIGITAL);

		if (sI_Steer && steer!=v->_cSteering)	
			v->setControlState(E_VCS_STEERING,steer);

		if (sI_AnalogSteering && steerAnalog!=v->_cAnalogSteering )	
			v->setControlMode(E_VCS_STEERING,steerAnalog ? E_VCSM_ANALOG : E_VCSM_DIGITAL);

		
		if (sI_Handbrake && handBrake )	
			v->setControlState(E_VCS_HANDBRAKE,handBrake == 1  ? true : false);


		
		DWORD flags = v->getStateFlags();
		beh->SetOutputParameterValue(O_Flags,&flags);


		float ratio = v->_getGearRatio();
		beh->SetOutputParameterValue(O_RATIO,&ratio);

		if (v->getGears())
		{
			int g = v->getGears()->getGear();
			beh->SetOutputParameterValue(O_Gear,&g);
		}

		if (v->getMotor())
		{
			float rpm = v->getMotor()->getRpm();
			beh->SetOutputParameterValue(O_RPM,&rpm);
		}

		beh->ActivateOutput(0);
	}
	return 0;
}