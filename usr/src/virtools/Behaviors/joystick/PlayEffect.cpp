#define STRICT
#define DIRECTINPUT_VERSION 0x0800

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#include <tchar.h>
#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <commdlg.h>
#include <dinput.h>

#include "CKAll.h"


static CKContext *ctx = NULL;

//----------------------------------------------------------------
//
// 
//
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

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
struct EFFECTS_NODE
{
    LPDIRECTINPUTEFFECT pDIEffect;
    DWORD               dwPlayRepeatCount;
    EFFECTS_NODE*       pNext;
};

LPDIRECTINPUT8        g_pDI       = NULL;         
LPDIRECTINPUTDEVICE8  g_pFFDevice = NULL;
EFFECTS_NODE          g_EffectsList;




//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDialogProc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK EnumFFDevicesCallback( LPCDIDEVICEINSTANCE pDDI, VOID* pvRef );
BOOL CALLBACK EnumAndCreateEffectsCallback( LPCDIFILEEFFECT pDIFileEffect, VOID* pvRef );

HRESULT InitDirectInput( HWND hDlg );
HRESULT FreeDirectInput2();
VOID    EmptyEffectList();
HRESULT OnReadFile( HWND hDlg,const char*file);
HRESULT OnPlayEffects2( HWND hDlg );

HRESULT InitDirectInput( HWND hDlg )
{
    HRESULT hr;

    // Setup the g_EffectsList circular linked list
    ZeroMemory( &g_EffectsList, sizeof( EFFECTS_NODE ) );
    g_EffectsList.pNext = &g_EffectsList;

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
	{
		ctx->OutputToConsole("PlayFFE :: DirectInput8Create");
	    return hr;
	}

    // Get the first enumerated force feedback device
    if( FAILED( hr = g_pDI->EnumDevices( 0, EnumFFDevicesCallback, 0, 
                                         DIEDFL_ATTACHEDONLY | 
                                         DIEDFL_FORCEFEEDBACK ) ) )
	{
		ctx->OutputToConsole("PlayFFE :: EnumDevices failed");		
		return hr;
	}

    
    if( g_pFFDevice == NULL )
    {

	   ctx->OutputToConsole("PlayFFE :: No force feedback device found.");
       return -1;
    }


    // Set the data format
    if( FAILED( hr = g_pFFDevice->SetDataFormat( &c_dfDIJoystick ) ) )
        return hr;


    // Set the coop level
    hr = g_pFFDevice->SetCooperativeLevel( hDlg , DISCL_EXCLUSIVE | DISCL_FOREGROUND) ;



    // Disable auto-centering spring
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = FALSE;



    if( FAILED( hr = g_pFFDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
        return hr;

    // Acquire the device
    if( FAILED( hr = g_pFFDevice->Acquire() ) )
        return hr;


	int op = 2;
	
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EnumFFDevicesCallback()
// Desc: Get the first enumerated force feedback device
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumFFDevicesCallback( LPCDIDEVICEINSTANCE pDDI, VOID* pvRef )
{
    if( FAILED( g_pDI->CreateDevice( pDDI->guidInstance, &g_pFFDevice, NULL ) ) )
        return DIENUM_CONTINUE; // If failed, try again

    // Stop when a device was successfully found
    return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: FreeDirectInput2()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT FreeDirectInput2()
{
    // Release any DirectInputEffect objects.
    if( g_pFFDevice ) 
    {
        EmptyEffectList();
        g_pFFDevice->Unacquire();
        SAFE_RELEASE( g_pFFDevice );
    }

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pDI );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EmptyEffectList()
// Desc: Goes through the circular linked list and releases the effects, 
//       and deletes the nodes
//-----------------------------------------------------------------------------
VOID EmptyEffectList()
{
    EFFECTS_NODE* pEffectNode = g_EffectsList.pNext;
    EFFECTS_NODE* pEffectDelete;

    while ( pEffectNode != &g_EffectsList )
    {
        pEffectDelete = pEffectNode;       
        pEffectNode = pEffectNode->pNext;

        SAFE_RELEASE( pEffectDelete->pDIEffect );
        SAFE_DELETE( pEffectDelete );
    }

    g_EffectsList.pNext = &g_EffectsList;
}




//-----------------------------------------------------------------------------
// Name: OnReadFile()
// Desc: Reads a file contain a collection of DirectInput force feedback 
//       effects.  It creates each of effect read in and stores it 
//       in the linked list, g_EffectsList.
//-----------------------------------------------------------------------------
HRESULT OnReadFile( HWND hDlg,const char*file)
{
    HRESULT hr;

     EmptyEffectList();

    // Enumerate the effects in the file selected, and create them in the callback
    if( FAILED( hr = g_pFFDevice->EnumEffectsInFile( file,EnumAndCreateEffectsCallback, 
                                                     NULL, DIFEF_MODIFYIFNEEDED ) ) )
        return hr;

    // If list of effects is empty, then we haven't been able to create any effects
    if( g_EffectsList.pNext == &g_EffectsList )
    {
		ctx->OutputToConsole("Unable to create any effects.");

    }
    else
    {
        // We have effects so enable the 'play effects' button

    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EnumAndCreateEffectsCallback()
// Desc: Create the effects as they are enumerated and add them to the 
//       linked list, g_EffectsList
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAndCreateEffectsCallback(LPCDIFILEEFFECT pDIFileEffect, VOID* pvRef )
{   
    HRESULT hr;
    LPDIRECTINPUTEFFECT pDIEffect = NULL;

    // Create the file effect
    if( FAILED( hr = g_pFFDevice->CreateEffect( pDIFileEffect->GuidEffect, 
                                                pDIFileEffect->lpDiEffect, 
                                                &pDIEffect, NULL ) ) )
    {
        ctx->OutputToConsole("Could not create force feedback effect on this device");
        return DIENUM_CONTINUE;
    }

    // Create a new effect node
    EFFECTS_NODE* pEffectNode = new EFFECTS_NODE;
    if( NULL == pEffectNode )
        return DIENUM_STOP;

    // Fill the pEffectNode up
    ZeroMemory( pEffectNode, sizeof( EFFECTS_NODE ) );
    pEffectNode->pDIEffect         = pDIEffect;
    pEffectNode->dwPlayRepeatCount = 1;

    // Add pEffectNode to the circular linked list, g_EffectsList
    pEffectNode->pNext  = g_EffectsList.pNext;
    g_EffectsList.pNext = pEffectNode;

    return DIENUM_CONTINUE;
}




//-----------------------------------------------------------------------------
// Name: OnPlayEffects2()
// Desc: Plays all of the effects enumerated in the file 
//-----------------------------------------------------------------------------
HRESULT OnPlayEffects2( HWND hDlg )
{
    EFFECTS_NODE*       pEffectNode = g_EffectsList.pNext;
    LPDIRECTINPUTEFFECT pDIEffect   = NULL;
    HRESULT             hr;

    // Stop all previous forces
    if( FAILED( hr = g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL ) ) )
        return hr;

	
    while ( pEffectNode != &g_EffectsList )
    {
        // Play all of the effects enumerated in the file 
        pDIEffect = pEffectNode->pDIEffect;

        if( NULL != pDIEffect )
        {
            if( FAILED( hr = pDIEffect->Start( pEffectNode->dwPlayRepeatCount, 0 ) ) )
                return hr;
        }

        pEffectNode = pEffectNode->pNext;
    }

    return S_OK;
}




CKObjectDeclaration	*FillBehaviorPlayFFEffectDecl();
CKERROR CreatePlayFFEffectProto(CKBehaviorPrototype **);
int PlayFFEffect(const CKBehaviorContext& behcontext);
CKERROR PlayFFECallBackObject(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorPlayFFEffectDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PlayFFEffect");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x54397475,0x4ca43e26));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePlayFFEffectProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Joystick");
	return od;
}


CKERROR CreatePlayFFEffectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("PlayFFEffect");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("Init");
	proto->DeclareInput("play");
	proto->DeclareInput("stop");
	proto->DeclareInput("release device");
	

	proto->DeclareOutput("initiated");
	proto->DeclareOutput("play exit");
	proto->DeclareOutput("stopped");
	proto->DeclareOutput("released");

	proto->DeclareOutput("error");


	proto->DeclareInParameter("effect file",CKPGUID_STRING);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( PlayFFEffect );

	proto->SetBehaviorCallbackFct(PlayFFECallBackObject);
	
	*pproto = proto;
	return CK_OK;

}


int PlayFFEffect(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx2 = behcontext.Context;
	ctx  = ctx2;



	HWND mWin = (HWND )ctx->GetMainWindow();

	//init and load effect
	if( beh->IsInputActive(0) ){
		beh->ActivateInput(0,FALSE);


		HRESULT  result = InitDirectInput(mWin);
		HRESULT  sa = S_OK;
		if (InitDirectInput(mWin) == S_OK)
		{

			XString filename((CKSTRING) beh->GetInputParameterReadDataPtr(0));
			OnReadFile(mWin,filename.Str());
			beh->ActivateOutput(0);
			return CKBR_OK;
		}else{
			beh->ActivateOutput(4);
			return CKBR_OK;
		}
		
	}


	//play

	if( beh->IsInputActive(1) ){
		beh->ActivateInput(1,FALSE);
			
		
		if (OnPlayEffects2(NULL) != S_OK ){
			beh->ActivateOutput(4);
			return CKBR_OK;
		}
		beh->ActivateOutput(1);
		return CKBR_OK;
	}


	//stop the effect
	if( beh->IsInputActive(2) ){
		beh->ActivateInput(2,FALSE);

	   // Stop all previous forces
		g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL );
		beh->ActivateOutput(2);
		return CKBR_OK;
	}


	//  [11/7/2004]
	//save device release

	if( beh->IsInputActive(3) ){
		beh->ActivateInput(3,FALSE);



		if ( g_pFFDevice)
			g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL );
			
		FreeDirectInput2();
		beh->ActivateOutput(3);
		return CKBR_OK;


	}

	return CKBR_OK;
}


CKERROR PlayFFECallBackObject(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	case CKM_BEHAVIORCREATE:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORDETACH:
	case CKM_BEHAVIORRESET:
	{
		
		
			if ( g_pFFDevice)
				g_pFFDevice->SendForceFeedbackCommand( DISFFC_STOPALL );
			FreeDirectInput2();
			//Sleep(2000);
			

	}
		break;
	}

	return CKBR_OK; 
}
