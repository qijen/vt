/********************************************************************
	created:	2009/06/01
	created:	1:6:2009   14:15
	filename: 	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick\JSetXYForce.cpp
	file path:	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick 
	file base:	JSetXYForce
	file ext:	cpp
	author:		Günter Baumgart
	
	purpose:	
*********************************************************************/



#include <dinput.h>
#include "CKAll.h"
#include "dInputShared.h"


static CKContext *ctx = NULL;
static bool gInitiated = false;
extern LPDIRECTINPUTEFFECT     g_pEffect;
extern LPDIRECTINPUTDEVICE8  g_pFFDevice;


#define HAS_CONFIG

#ifdef HAS_CONFIG
	#include "gConfig.h"
#endif // BB_TOOLS

#ifdef BB_TOOLS
	
	#include <vtInterfaceEnumeration.h>
	#include "vtLogTools.h"
	#include "vtCBBErrorHelper.h"
	#include <virtools/vtBBHelper.h>
	#include <virtools/vtBBMacros.h>

	using namespace vtTools::BehaviorTools;

#endif


CKObjectDeclaration	*FillBehaviorJSetXYForceDecl();
CKERROR CreateJSetXYForceProto(CKBehaviorPrototype **);
int JSetXYForce(const CKBehaviorContext& behcontext);
CKERROR PlayFFECallBackObject(const CKBehaviorContext& behcontext);
CKObjectDeclaration	*FillBehaviorJSetXYForceDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("JSetXYForce");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6890534f,0x31c12074));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJSetXYForceProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Joystick");
	return od;
}

enum bbIO_Inputs
{
	BB_I_DO,
	BB_I_RELEASE,
};

enum bbIO_Outputs
{
	BB_O_DONE,
	BB_O_RELEASED,
	BB_O_ERROR,
};

CKERROR CreateJSetXYForceProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("JSetXYForce");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareInput("release device");
	
	proto->DeclareOutput("Done");
	proto->DeclareOutput("Released");
	proto->DeclareOutput("Error");


	proto->DeclareInParameter("Force Vector",CKPGUID_2DVECTOR);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( JSetXYForce );

	proto->SetBehaviorCallbackFct(PlayFFECallBackObject);
	
	*pproto = proto;
	return CK_OK;
}





int JSetXYForce(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx2 = behcontext.Context;
	if (!ctx)
	{
		ctx  = ctx2;
	}
	
	HWND mWin = (HWND )ctx->GetMainWindow();
	HRESULT hr = S_OK;


	//init and load effect
	if( beh->IsInputActive(BB_I_DO) )
	{
		beh->ActivateInput(BB_I_DO,FALSE);
		if (!gInitiated)
		{
			if (!InitDirectInput2(mWin) == S_OK)
			{
				beh->ActivateOutput(BB_O_ERROR);
				return CKBR_OK;
			}else{

				hr = g_pFFDevice->Acquire();
				hr =g_pEffect->Start( 1, 0 ); // Start the effect
				gInitiated = true;
			}
		}
		
		Vx2DVector vectorForce;
		beh->GetInputParameterValue(0,&vectorForce);
		
		

		SetDeviceForcesXY(vectorForce.x,vectorForce.y);
		beh->ActivateOutput(BB_O_DONE);
	}


	//play

	if( beh->IsInputActive(BB_I_RELEASE))
	{
		beh->ActivateInput(BB_I_RELEASE,FALSE);
		{
			beh->ActivateOutput(BB_I_RELEASE);
			FreeDirectInput2();
			return CKBR_OK;
		}
	}


	/*
	//stop the effect
	if( beh->IsInputActive(2) ){
		beh->ActivateInput(2,FALSE);

		//g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL );
		beh->ActivateOutput(2);
		return CKBR_OK;
	}*/


	return CKBR_OK;
}


CKERROR PlayFFECallBackObject(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) 
	{
		case CKM_BEHAVIORDETACH:
		case CKM_BEHAVIORRESET:
		{
			gInitiated = false;
			if ( g_pFFDevice)
				g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL );
			FreeDirectInput2();
		}
			break;
	}

	return CKBR_OK; 
}
