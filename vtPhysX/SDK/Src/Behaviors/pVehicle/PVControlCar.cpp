
#include <StdAfx.h>
#include "pCommon.h"





CKObjectDeclaration	*FillBehaviorPVControl2Decl();
CKERROR CreatePVControl2Proto(CKBehaviorPrototype **pproto);
int PVControl2(const CKBehaviorContext& behcontext);
CKERROR PVControl2CB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;



enum bInTrigger
{
	TI_DO,
};

enum bOutputs
{

	O_Gear,
	O_MPH,
	O_GearRatio,
	O_MotorRPM,
	/*O_RPM,
	O_RPM_WHEELS,
	O_RATIO,*/
};


enum bTInputs
{

	IT_On,
	IT_Off,
	IT_Forward,
	IT_Backwards,
	IT_Left,
	IT_Right,
	IT_HandBrake,
	IT_GUP,
	IT_GDOWN
	
};

enum bTOutputs
{
	OT_On,
	OT_Off,
	OT_GearUp,
	OT_GearDown,
};


enum bSettings
{
	bs_Manual,
};



#define BB_SSTART 0




//************************************
// Method:    FillBehaviorPVControl2Decl
// FullName:  FillBehaviorPVControl2Decl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPVControl2Decl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVCarControl");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Physics Car");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x26371b1c,0x4e3924));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVControl2Proto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVControl2Proto
// FullName:  CreatePVControl2Proto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVControl2Proto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVCarControl");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("On");
	proto->DeclareInput("Off");
	proto->DeclareInput("Forward");
	proto->DeclareInput("Backward");
	proto->DeclareInput("Turn Left");
	proto->DeclareInput("Turn Right");
	proto->DeclareInput("Handbrake");

	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("GearUp");
	proto->DeclareOutput("GearDown");
	
	

	proto->DeclareOutParameter("Current Gear",CKPGUID_INT,"-1");
	proto->DeclareOutParameter("MPH",CKPGUID_FLOAT,"-1");
	proto->DeclareOutParameter("Gear Ratio",CKPGUID_FLOAT,"-1");
	proto->DeclareOutParameter("Motor RPM",CKPGUID_FLOAT,"-1");


	proto->DeclareSetting("Automatic",CKPGUID_BOOL,"TRUE");
	proto->DeclareSetting("Semi Analog",CKPGUID_BOOL,"TRUE");


	/* PVControl2  

	PVControl2 is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PVControl2.png

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

	
*/
	proto->SetBehaviorCallbackFct( PVControl2CB );

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVControl2);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PVControl2
// FullName:  PVControl2
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************


#define BBSParameter(A) DWORD s##A;\
	beh->GetLocalParameterValue(A,&s##A)

int PVControl2(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;

	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) bbSErrorME(E_PE_REF);


	pRigidBody *body = NULL;

	body = GetPMan()->getBody(target);
	if (!body)
		bbSErrorME(E_PE_NoBody);


	pVehicle *v = body->getVehicle();
	if (!v)
	{
		bbSErrorME(E_PE_NoVeh);
	}



	if( beh->IsInputActive(IT_On) )
	{
		beh->ActivateInput(IT_On,FALSE);
		beh->ActivateOutput(OT_On);


	}

	if( beh->IsInputActive(IT_Off) )
	{
		beh->ActivateInput(IT_Off,FALSE);
		beh->ActivateOutput(OT_Off);
		return 0;

	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//		Acceleration 
	//


	float acceleration = 0.0f;
	float steering = 0.0;

	int gearUP= 0;
	int gearDown= 0;
	int handbrake = 0;


	/*
	v->setControlState(E_VCS_ACCELERATION,acceleration);
	v->setControlState(E_VCS_HANDBRAKE,handbrake);
	v->setControlState(E_VCS_STEERING,steering);
	v->setControlMode(E_VCS_ACCELERATION,E_VCSM_DIGITAL);
	v->setControlMode(E_VCS_STEERING,E_VCSM_DIGITAL);
	*/

	//////////////////////////////////////////////////////////////////////////
	//
	//		Acceleration + Handbrake
	//

	if( beh->IsInputActive(IT_Forward) )
	{
		beh->ActivateInput(IT_Forward,FALSE);
		acceleration = 1.0f;
		v->setControlState(E_VCS_ACCELERATION,acceleration);
	}

	if( beh->IsInputActive(IT_Backwards) )
	{
		beh->ActivateInput(IT_Backwards,FALSE);
		acceleration = -1.0f;
		v->setControlState(E_VCS_ACCELERATION,acceleration);
	}

	
	if( beh->IsInputActive(IT_HandBrake) )
	{
		beh->ActivateInput(IT_HandBrake,FALSE);
		handbrake = 1;
		v->setControlState(E_VCS_HANDBRAKE,handbrake);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Steering	
	if( beh->IsInputActive(IT_Left) )
	{
		beh->ActivateInput(IT_Left,FALSE);
		steering = 1.0f;
		v->setControlState(E_VCS_STEERING,steering);
	}

	if( beh->IsInputActive(IT_Right) )
	{
		beh->ActivateInput(IT_Right,FALSE);
		steering = -1.0f;
		v->setControlState(E_VCS_STEERING,steering);
	}

	//if(acceleration !=0.0f || steering !=0.0f || handbrake)
	//v->control(steering,false,acceleration,false,handbrake);


	int semiAnalog = 0;
	beh->GetLocalParameterValue(1,&semiAnalog);
	
	

	//////////////////////////////////////////////////////////////////////////
	//
	//		Gears	
	//
	int automatic=0;
	beh->GetLocalParameterValue(bs_Manual,&automatic);
	v->setAutomaticMode(automatic);


	int lastGear=0;
	beh->GetOutputParameterValue(O_Gear,&lastGear);

	int currentGear = 0;

	if (v->getGears())
	{
		currentGear = v->getGears()->getGear();
	}

	if (!automatic)
	{

		if( beh->IsInputActive(IT_GUP) )
		{
			beh->ActivateInput(IT_GUP,FALSE);
			gearUP =1;
		}
		if( beh->IsInputActive(IT_GDOWN) )
		{
			beh->ActivateInput(IT_GDOWN,FALSE);
			gearDown=1;
		}

	}

	if ( gearUP && !gearDown)
	{
		v->setControlState(E_VCS_GUP,1);
		v->setControlState(E_VCS_GDOWN,0);
	}else{
		v->setControlState(E_VCS_GUP,0);
	}

	if ( !gearUP && gearDown)
	{
		v->setControlState(E_VCS_GUP,0);
		v->setControlState(E_VCS_GDOWN,1);
	}
	if ( !gearUP)
	{
		v->setControlState(E_VCS_GUP,0);
	}
	if ( !gearDown)
	{
		v->setControlState(E_VCS_GDOWN,0);
	}

/*

	if (!automatic)
	{
		
		if( beh->IsInputActive(IT_GUP) )
		{
			beh->ActivateInput(IT_GUP,FALSE);
			gearUP =1;
		}

		if( beh->IsInputActive(IT_GDOWN) )
		{
			beh->ActivateInput(IT_GDOWN,FALSE);
			gearDown=1;
		}

		if (gearUP && !gearDown)
		{
			v->gearUp();
		}
		if (!gearUP && gearDown)
		{
			v->gearDown();
		}
	}else
	{
		if (v->getGears())
		{
			if (currentGear!=lastGear)
			{
				if (currentGear > lastGear)
				{
					beh->ActivateOutput(OT_GearUp);
				}else
					beh->ActivateOutput(OT_GearDown);
			}
		}
	}

*/



	///////////////////////////////////////////////////////////////////////////
	//
	//		Vehicle Data : 
	//


	float mph = v->getMPH();
	beh->SetOutputParameterValue(O_MPH,&mph);

	float gRatio = v->_getGearRatio();
	beh->SetOutputParameterValue(O_GearRatio,&gRatio);
	beh->SetOutputParameterValue(O_Gear,&currentGear);
	if (v->getMotor())
	{
		float rpm = v->getMotor()->getRpm();
		beh->SetOutputParameterValue(O_MotorRPM,&rpm);
	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR PVControl2CB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) 
	{

	case CKM_BEHAVIORLOAD:
		{
			break;
		}
	case CKM_BEHAVIORDETACH:
		{
				break;
		}

	case CKM_BEHAVIORATTACH:
		{
		break;

		}

	case CKM_BEHAVIORSETTINGSEDITED:
		{
			int automatic=0;
			beh->GetLocalParameterValue(bs_Manual,&automatic);

			int nbI = beh->GetInputCount();
			
			if (automatic)
			{
				if (nbI  > IT_GUP  )
				{
					beh->DeleteInput( IT_GUP);
					beh->DeleteInput( IT_GUP);
				}
			}

			if (!automatic)
			{
				if (nbI  < IT_GDOWN  )
				{
					beh->AddInput("Gear Up");
					beh->AddInput("Gear Down");
				}
			}

			break;
		}
	}
	return CKBR_OK;

}
