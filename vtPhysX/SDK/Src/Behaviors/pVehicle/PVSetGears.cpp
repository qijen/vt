#include <StdAfx.h>
#include "pCommon.h"


#include "IParameter.h"
#include "pVehicleAll.h"
#include <xDebugTools.h>


CKObjectDeclaration	*FillBehaviorPVSetGearsDecl();
CKERROR CreatePVSetGearsProto(CKBehaviorPrototype **pproto);
int PVSetGears(const CKBehaviorContext& behcontext);
CKERROR PVSetGearsCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;
using namespace vtTools::ParameterTools;




enum bInputs
{

	I_XML,
	I_nbFGears,
	I_Flags,
	I_Ratios,
	I_ShiftUpRPM,
	I_ShiftDownRPM,
	I_Clutch,
	I_ClutchRelease,
	
};

#define BB_SSTART 4


BBParameter pInMap6[]  = 
{

	BB_PIN(I_XML,VTE_XML_VGEAR_SETTINGS,"XML Link","None"),
	BB_PIN(I_nbFGears,CKPGUID_INT,"Number of Forward Gears","None"),
	BB_PIN(I_Flags,VTS_VGEARBOX_FLAGS,"Flags","None"),
	BB_PIN(I_Ratios,VTS_VGEAR_RATIOS,"Forward Ratios/Inertias","None"),
	BB_SPIN(I_ShiftUpRPM,CKPGUID_FLOAT,"Shift Up RPM",""),
	BB_SPIN(I_ShiftDownRPM,CKPGUID_FLOAT,"Shift Down RPM",""),
	BB_SPIN(I_Clutch,CKPGUID_TIME,"Clutch Time",""),
	BB_SPIN(I_ClutchRelease,CKPGUID_TIME,"Clutch Release Time",""),





	/*BB_SPIN(I_RatioC,VTS_VGEAR_SETTINGS,"Gear Graphic Setup","None"),*/
};

#define gPIMAP pInMap6

CKERROR PVSetGearsCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
	CKContext *ctx = beh->GetCKContext();

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORCREATE:
		case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDELETE:
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
// Method:    FillBehaviorPVSetGearsDecl
// FullName:  FillBehaviorPVSetGearsDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration*FillBehaviorPVSetGearsDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PVGears");	
	od->SetCategory("Physic/Vehicle");
	od->SetDescription("Attachs and/or modifies a gearbox of a vehicle controller");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5d701f33,0x17d519eb));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePVSetGearsProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePVSetGearsProto
// FullName:  CreatePVSetGearsProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePVSetGearsProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PVGears");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	//proto->DeclareOutParameter("Result Ratio Curve",CKPGUID_2DCURVE,"FALSE");

//	proto->DeclareOutParameter("Result Torque Curve",CKPGUID_2DCURVE,"FALSE");
//
	/*
	
	PVSetGears  

	PVSetGears is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PVSetGears.png

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
	proto->SetBehaviorCallbackFct( PVSetGearsCB );
	
	BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(gPIMAP);

	//proto->DeclareSetting("Output Ratio Curve",CKPGUID_BOOL,"TRUE");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	

	proto->SetFunction(PVSetGears);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PVSetGears
// FullName:  PVSetGears
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************



int getNewRatioTable(	CKBehavior*beh,float dst[],CK2dCurve *resultCurve,int size = 6)
{

	int result =  0 ;


	BB_DECLARE_PIMAP;//retrieves the parameter input configuration array
	//////////////////////////////////////////////////////////////////////////
	//
	//		We store safely some temporary data to determine the given 
	//		user values are valid and can be send to the motor 
	//
	CKParameterIn *inP = beh->GetInputParameter(BB_IP_INDEX(I_Ratios));
	CKParameter *pout= inP->GetDirectSource();
	if (pout)
	{
		
		int gear = 0;
		float ratio = 0.0f;
		for (int i = 0 ; i < 6 ; i++)
		{
			CK_ID* paramids = static_cast<CK_ID*>(pout->GetReadDataPtr());
			CKParameter * sub = static_cast<CKParameterOut*>(GetPMan()->m_Context->GetObject(paramids[i]));
			if (sub)
			{

				gear = GetValueFromParameterStruct<int>(sub,0,false);
				ratio = GetValueFromParameterStruct<float>(sub,1,false);

				if ( ratio !=0.0 )
				{

					dst[i] = ratio;

					result++;
					//dst.insert(gear,ratio);

					if (resultCurve )
					{

						float dx = 1/ (size) ;
						float dy = ratio / 10;
						resultCurve->AddControlPoint(Vx2DVector(dx,dy)  );
						beh->SetOutputParameterValue(0,&resultCurve);

					}
				}
			}
		}
	}

	return result;
}

int PVSetGears(const CKBehaviorContext& behcontext)
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
		if (!body)			bbSErrorME(E_PE_NoBody);
		
		pVehicle *v = body->getVehicle();
		if (!v)				bbSErrorME(E_PE_NoVeh);

		if (!v->isValidEngine())
			bbErrorME("Vehicle is not complete");

		pGearBox *gBox = v->getGearBox();
		if (!gBox)			bbErrorME("No valid gearbox");



		CK2dCurve* pOCurve = NULL;	//optional 
		CK2dCurve* pICurve = NULL;	//optional 



		BB_DECLARE_PIMAP;//retrieves the parameter input configuration array


		BBSParameterM(I_XML,BB_SSTART);//our bb settings concated as s##I_XML
		BBSParameterM(I_nbFGears,BB_SSTART);
		BBSParameterM(I_Flags,BB_SSTART);
		BBSParameterM(I_Ratios,BB_SSTART);
		BBSParameterM(I_ShiftUpRPM,BB_SSTART);
		BBSParameterM(I_ShiftDownRPM,BB_SSTART);
		BBSParameterM(I_Clutch,BB_SSTART);
		BBSParameterM(I_ClutchRelease,BB_SSTART);

		int xmlLink = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_XML));
		int nbGears = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_nbFGears));
		int flags = GetInputParameterValue<int>(beh,BB_IP_INDEX(I_Flags));

		//----------------------------------------------------------------
		//
		// setup ratios / inertias
		//
		CKParameterIn *inP = beh->GetInputParameter(BB_IP_INDEX(I_Ratios));

		CKParameter *pout= inP->GetDirectSource();
		if (pout)
		{
			IParameter::Instance()->copyTo(gBox,pout);
			
			v->getDriveLine()->CalcPreClutchInertia();
			v->getDriveLine()->CalcCumulativeRatios();
			v->getDriveLine()->CalcEffectiveInertiae();
			v->getDriveLine()->CalcPostClutchInertia();

		}
		
		//----------------------------------------------------------------
		//
		//	Setup nb of gears
		//
		iAssertW1(X_IS_BETWEEN(nbGears,3,10),nbGears=3);
		gBox->setGears(nbGears);

		//----------------------------------------------------------------
		//
		// RPMs
		//
		if (sI_ShiftUpRPM)
			gBox->setShiftUpRPM(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_ShiftUpRPM)));

		if (sI_ShiftDownRPM)
			gBox->setShiftDownRPM(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_ShiftDownRPM)));

		if (sI_Clutch)
			gBox->setTimeToClutch(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_Clutch)));

		if (sI_ClutchRelease)
			gBox->setTimeToDeclutch(GetInputParameterValue<float>(beh,BB_IP_INDEX(I_ClutchRelease)));


		//float bRatio = GetInputParameterValue<float>(beh,BB_IP_INDEX(I_BRatio));//silent update 

		//////////////////////////////////////////////////////////////////////////
		//a optinal curve to display ratio between horse power and rpm
		/*DWORD outputCurve;	beh->GetLocalParameterValue(BB_PMAP_SIZE(gPIMAP),&outputCurve);//special settings !
		if (outputCurve)
			beh->GetOutputParameterValue(0,&pOCurve);
		*/

		/*if (!gears && !sI_Ratios )
		{
			sI_Ratios = true;
		}*/
		//////////////////////////////////////////////////////////////////////////
		//
		//			Checking we have to replace of the entire motor object ! 
		//
		if (sI_Ratios)
		{
			//////////////////////////////////////////////////////////////////////////
			//
			pLinearInterpolation nTable;

			/*nbGears = gDesc->nbForwardGears;
			int ratioSize = getNewRatioTable(beh,gDesc->forwardGearRatios,NULL,nbGears);
			*/

			/*
			if (ratioSize )
			{
				//////////////////////////////////////////////////////////////////////////
				
				
				if (pOCurve)
				{
					while (pOCurve->GetControlPointCount())
					{
						pOCurve->DeleteControlPoint(pOCurve->GetControlPoint(0));
						pOCurve->Update();
					}
				}
			

//				gDesc->backwardGearRatio	=	sI_BRatio	?	bRatio : gears ?  gears->_minRpm: mDesc->minRpm;

				//getNewRatioTable(beh,gDesc->forwardGearRatios,pOCurve,ratioSize);

				//if (!gDesc->isValid())					bbErrorME("gear description was invalid,  aborting update ");
				//gDesc->nbForwardGears =ratioSize;
				
				//v->setGears(pFactory::Instance()->createVehicleGears(*gDesc));

				pVehicleGears *loaded = v->getGears();
				if (!loaded)
					bbErrorME("creating of new gears failed unexpected");

			}
			*/
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//	Flexibility : 
		//
		
		/*
		if (sI_nbFGears && gears )gears->_nbForwardGears = nbGears;
		if (sI_BRatio && gears )gears->_backwardGearRatio;
		if (outputCurve && pOCurve	)
		{
			int s  = pOCurve->GetControlPointCount();
			for (int i  = 0 ; i < pOCurve->GetControlPointCount() ; i++)
			{
				CK2dCurvePoint*  point = pOCurve->GetControlPoint(i);
				point->SetLinear(true);
			}
			pOCurve->Update();
			beh->SetOutputParameterValue(0,&pOCurve);
		}
		delete gDesc;
		gDesc;
		*/
	}
	beh->ActivateOutput(0);
	return 0;
}

