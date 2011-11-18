#include <StdAfx.h>

#include "pCommon.h"

#include "IParameter.h"
#include "pVehicleAll.h"
#include <xDebugTools.h>

CKObjectDeclaration	*FillBehaviorPVSetMotorValuesDecl();
CKERROR CreatePVSetMotorValuesProto(CKBehaviorPrototype **pproto);
int PVSetMotorValues(const CKBehaviorContext& behcontext);
CKERROR PVSetMotorValuesCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;
using namespace vtTools::ParameterTools;



enum bInputs
{

	I_XML,
	I_Flags,
	I_Clutch,

	I_maxRPM,
	I_minRPM,
	I_maxTorque,

	I_intertia,
	I_engineFriction,
	I_breakCoeff,
	I_GroundForceFeedBackScale,

	I_tList,
};



#define BB_SSTART 0

BBParameter pInMap5[]  = 
{

	
	BB_SPIN(I_XML,VTE_XML_VMOTOR_SETTINGS,"XML Link","None"),
	BB_SPIN(I_Flags,VTF_VEHICLE_ENGINE_FLAGS,"Flags","None"),
	BB_SPIN(I_Clutch,CKPGUID_FLOAT,"Clutch","None"),

	BB_SPIN(I_maxRPM,CKPGUID_FLOAT,"Maximum RPM","None"),
	BB_SPIN(I_minRPM,CKPGUID_FLOAT,"Minimum RPM","None"),
	BB_SPIN(I_maxTorque,CKPGUID_FLOAT,"Maximum Torque","None"),

	BB_SPIN(I_intertia,CKPGUID_FLOAT,"Inertia","None"),
	BB_SPIN(I_engineFriction,CKPGUID_FLOAT,"Friction","1.0f"),
	BB_SPIN(I_breakCoeff,CKPGUID_FLOAT,"Break Factor","0.0f"),
	BB_SPIN(I_GroundForceFeedBackScale,CKPGUID_FLOAT,"Ground Force Scale","0.0f"),

	BB_SPIN(I_tList,VTS_VMOTOR_TVALUES,"Torque per RPM","None"),

};

#define gPIMAP pInMap5

CKERROR PVSetMotorValuesCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	BB_DECLARE_PMAP;
	switch(behcontext.CallbackMessage) 
	{


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

//************************************
// Method:    FillBehaviorPVSetMotorValuesDecl
// FullName:  FillBehaviorPVSetMotorValuesDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration*FillBehaviorPVSetMotorValuesDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVMotor");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Modifies motor parameters of a vehicle controller");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7b044c81,0xde9489a));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVSetMotorValuesProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVSetMotorValuesProto
// FullName:  CreatePVSetMotorValuesProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVSetMotorValuesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVMotor");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	

	//BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(gPIMAP);


	/*!  PVMotor  

	PVSetMotorValues is categorized in \ref Vehicle

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PVMotor.png

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

	proto->SetBehaviorCallbackFct( PVSetMotorValuesCB );
	
	

	//proto->DeclareSetting("Output Curve",CKPGUID_BOOL,"TRUE");
	//proto->DeclareSetting("Automatic Gears",CKPGUID_BOOL,"TRUE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PVSetMotorValues);
	*pproto = proto;
	return CK_OK;
}


int PVSetMotorValues(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	CKContext* ctx = behcontext.Context;

	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);



		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target )	bbSErrorME(E_PE_REF);

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)		bbSErrorME(E_PE_NoBody);

		pVehicle *v = body->getVehicle();
		if (!v)			bbSErrorME(E_PE_NoVeh);

		if (!v->isValidEngine())
			bbErrorME("Vehicle is not complete");

		pEngine *engine = v->getEngine();
		if (!engine)			bbErrorME("No valid engine");
		
		
		CK2dCurve* pOCurve = NULL;	//optional 

		BB_DECLARE_PIMAP;//retrieves the parameter input configuration array
		BBSParameter(I_XML);//our bb settings concated as s##I_XML
		BBSParameter(I_Flags);
		BBSParameter(I_Clutch);
		BBSParameter(I_tList);
		BBSParameter(I_maxRPM);
		BBSParameter(I_minRPM);
		BBSParameter(I_maxTorque);
		BBSParameter(I_intertia);
		BBSParameter(I_engineFriction);
		BBSParameter(I_breakCoeff);
		BBSParameter(I_GroundForceFeedBackScale);

		if (sI_XML){
			int xmlLink = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_XML));
		}
		
		if (sI_GroundForceFeedBackScale)
			engine->setForceFeedbackScale(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_GroundForceFeedBackScale)));

		if (sI_Flags)
			engine->setFlags(GetInputParameterValue<int>(beh,BB_IP_INDEX(I_Flags)));
		if (sI_Clutch)
			engine->setClutch(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_Clutch)));

		if (sI_maxTorque)
			engine->setMaxTorque(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_maxTorque)));

		if (sI_maxTorque)
			engine->setMaxTorque(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_maxTorque)));
		if (sI_maxRPM)
			engine->setMaxRPM(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_maxRPM)));
		if (sI_minRPM)
			engine->setIdleRPM(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_minRPM)));
		if (sI_engineFriction)
			engine->setFriction(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_engineFriction)));
		if (sI_intertia)
			engine->SetInertia(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_intertia)));

		if (sI_breakCoeff)
			engine->setBrakingCoeff(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_breakCoeff)));


		if (sI_tList){

			CKParameterIn *inP = beh->GetInputParameter(BB_IP_INDEX(I_tList));
			CKParameter *pout= inP->GetDirectSource();
			if (pout)
			{
				
				if (engine->getTorqueCurve())
				{
					pLinearInterpolation &curve = *engine->getTorqueCurve();

					curve.clear();
					IParameter::Instance()->copyTo(curve,pout);
					engine->preStep();

				}
				
			}
			//engine->setTorqueCurve();
		}

	

		//////////////////////////////////////////////////////////////////////////
		//a optinal curve to display ratio between horse power and rpm

/*		DWORD outputCurve;	beh->GetLocalParameterValue(BB_PMAP_SIZE(gPIMAP),&outputCurve);//special settings !
		if (outputCurve)
			beh->GetOutputParameterValue(0,&pOCurve);

*/
		


		//////////////////////////////////////////////////////////////////////////
		//
		//			Checking we have to replace of the entire motor object ! 
		//
		if (sI_tList)
		{
			float maxInputRPM =0.0f;			
			float maxInputNM =0.0f;

			//////////////////////////////////////////////////////////////////////////
			//
			// this is only a test run ! 
			pLinearInterpolation nTable;
			//getNewTorqueTable(beh,nTable,maxInputRPM,maxInputNM,NULL);

			if (nTable.getSize() )
			{

				/*
				if (pOCurve)
				{
					while (pOCurve->GetControlPointCount())
					{
						pOCurve->DeleteControlPoint(pOCurve->GetControlPoint(0));
						pOCurve->Update();
					}
				}

				*/
				//////////////////////////////////////////////////////////////////////////
				//	we just copy into the motor we can get !
				
			
				//////////////////////////////////////////////////////////////////////////
				// we make a new torque ratios 

				//getNewTorqueTable(beh,mDesc->torqueCurve,maxInputRPM,maxInputNM,pOCurve);
				
				/*
				if (!mDesc->isValid())					bbErrorME("motor description was invalid,  aborting update ");

				if (!motor)
				{
					motor = pFactory::Instance()->createVehicleMotor(*mDesc);
					v->setMotor(motor);
				}else
				{
					if (mDesc->torqueCurve.getSize())
					{
						motor->loadNewTorqueCurve(mDesc->torqueCurve);
					}
				}*/
			}
		}


		

		//////////////////////////////////////////////////////////////////////////
		//
		//	Flexibility : 
		//
		
		/*
		if (sI_minRDown &&		motor )motor->setMinRpmToGearDown(mDDown);
		if (sI_maxRUp &&			motor )motor->setMaxRpmToGearUp(mDUp);
		if (sI_maxR	&&			motor )motor->setMaxRpm(maxR);
		if (sI_minR	&&			motor )motor->setMinRpm(minR);

		*/
		if ( pOCurve	)
		{
			/*
			#ifdef _DEBUG
						XString error;
						error << "t curve created with " << pOCurve->GetControlPointCount() << " with l : " << pOCurve->GetLength();
						bbErrorME(error.CStr());
			#endif
			*/


			/*
			for (int i  = 0 ; i < pOCurve->GetControlPointCount() ; i++)
			{
				CK2dCurvePoint*  point = pOCurve->GetControlPoint(i);
				point->SetLinear(true);
			}
			pOCurve->Update();
			beh->SetOutputParameterValue(0,&pOCurve);
			*/

		}
		//delete mDesc;
		//mDesc  = NULL;
	}

	beh->ActivateOutput(0);
	return 0;
}

//somebody wants to enter new torque values : 
//if (outputCurve && sI_tList )
//	beh->GetOutputParameterValue(0,&pOCurve);

/*
if (sI_tList)
{

float maxInputRPM =0.0f;			float maxInputNM =0.0f;

pLinearInterpolation torqueCurve;
CKParameterIn *inP = beh->GetInputParameter(BB_IP_INDEX(I_tList));			CKParameter *pout= inP->GetDirectSource();

if (pout)
{
float rpm = 0.0f;				float nm = 0.0f;

bool maximaFound = false;				bool dataCollected = false;


evaluate:
for (int i = 0 ; i < 6 ; i++)
{
CK_ID* paramids = static_cast<CK_ID*>(pout->GetReadDataPtr());
CKParameter * sub = static_cast<CKParameterOut*>(GetPMan()->m_Context->GetObject(paramids[i]));
if (sub)
{

rpm  = GetValueFromParameterStruct<float>(sub,0,false);
nm = GetValueFromParameterStruct<float>(sub,1,false);


if ( rpm >0.0f &&  nm > 0.0f  )
{

if (rpm > maxInputRPM) maxInputRPM = rpm;
if (nm > maxInputNM) maxInputNM = nm;


if (maximaFound || !outputCurve	)
{
torqueCurve.insert(rpm,nm);
dataCollected =true;
}


if (maximaFound && outputCurve )
{
pOCurve->AddControlPoint(Vx2DVector(rpm/maxInputRPM , nm/maxInputNM)  );
dataCollected =true;
}
}
}
}




if (dataCollected)				{					goto end;				}

if ( maxInputRPM >0.0f &&  maxInputNM> 0.0f  )
{
maximaFound  = true;
goto evaluate;
}

end :

if (dataCollected )
{

if (outputCurve)
beh->SetOutputParameterValue(0,&pOCurve);



//////////////////////////////////////////////////////////////////////////
if (!motor)
{

mDesc = new pVehicleMotorDesc();
mDesc->setToDefault();

mDesc->maxRpmToGearUp = sI_maxRUp  ? mDUp : mDesc->maxRpmToGearUp;
mDesc->minRpmToGearDown = sI_minRDown  = mDDown : mDesc->minRpmToGearDown ;
mDesc->torqueCurve  = torqueCurve;
motor = pFactory::Instance()->createVehicleMotor(*mDesc);
v->setMotor(motor);
}

}
}
}

*/