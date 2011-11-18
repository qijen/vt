/********************************************************************
	created:	2009/06/01
	created:	1:6:2009   14:12
	filename: 	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick\hasFFe.cpp
	file path:	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick 
	file base:	hasFFe
	file ext:	cpp
	author:		Günter Baumgart
	
	purpose:	
*********************************************************************/

#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <commdlg.h>
#include "dinput.h"
#include "CKAll.h"

//----------------------------------------------------------------
//
// globals
//
LPDIRECTINPUT8        g_pDI2       = NULL;         
LPDIRECTINPUTDEVICE8  g_pFFDevice2 = NULL;


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



CKObjectDeclaration	*FillBehaviorHasFFEffectsDecl();
CKERROR CreateHasFFEffectsProto(CKBehaviorPrototype **);
int HasFFEffects(const CKBehaviorContext& behcontext);
CKERROR PlayFFECallBackObject2(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorHasFFEffectsDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("JHasForceFeedback");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x12641a78,0x7ca70c45));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateHasFFEffectsProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Joystick");
	return od;
}


CKERROR CreateHasFFEffectsProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("JHasForceFeedback");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("yes");
	proto->DeclareOutput("no");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( HasFFEffects );
	*pproto = proto;
	return CK_OK;
}

BOOL CALLBACK EnumFFDevicesCallback0( LPCDIDEVICEINSTANCE pDDI, VOID* pvRef )
{
    if( FAILED( g_pDI2->CreateDevice( pDDI->guidInstance, &g_pFFDevice2, NULL ) ) )
        return DIENUM_CONTINUE; // If failed, try again
    return DIENUM_STOP;
}

HRESULT FreeDirectInput0()
{
    // Release any DirectInputEffect objects.
    if( g_pFFDevice2 ) 
    {

        g_pFFDevice2->Unacquire();
        SAFE_RELEASE( g_pFFDevice2 );
    }

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pDI2 );

    return S_OK;
}

int HasFFEffects(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;

	HWND mWin = (HWND )ctx->GetMainWindow();

	DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDI2, NULL )  ;

    // Get the first enumerated force feedback device
    g_pDI2->EnumDevices( 0, EnumFFDevicesCallback0, 0, DIEDFL_ATTACHEDONLY | DIEDFL_FORCEFEEDBACK );

	if( g_pFFDevice2 == NULL )
          beh->ActivateOutput(1);
	else
		beh->ActivateOutput(0);
	
	FreeDirectInput0();
	return CKBR_OK;
}

