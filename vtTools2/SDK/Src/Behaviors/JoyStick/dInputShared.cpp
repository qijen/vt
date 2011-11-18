/********************************************************************
	created:	2009/06/01
	created:	1:6:2009   14:19
	filename: 	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick\dInputShared.cpp
	file path:	x:\ProjectRoot\vtmodsvn\tools\vtTools\Sdk\Src\Behaviors\JoyStick 
	file base:	dInputShared
	file ext:	cpp
	author:		Günter Baumgart
	
	purpose:	shared joystick functions
*********************************************************************/

#include "dInputShared.h"
#include "CKAll.h"

extern CKContext *ctx=NULL;

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


LPDIRECTINPUTEFFECT     g_pEffect = NULL;
BOOL                    g_bActive = TRUE;

DWORD                   g_dwNumForceFeedbackAxis = 0;
INT                     g_nXForce;
INT                     g_nYForce;
DWORD                   g_dwLastEffectSet; // Time of the previous force feedback effect set

//-----------------------------------------------------------------------------
// Name: EnumAxesCallback()
// Desc: Callback function for enumerating the axes on a joystick and counting
//       each force feedback enabled axis
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
							   VOID* pContext )
{
	DWORD* pdwNumForceFeedbackAxis = ( DWORD* )pContext;

	if( ( pdidoi->dwFlags & DIDOI_FFACTUATOR ) != 0 )
		( *pdwNumForceFeedbackAxis )++;

	return DIENUM_CONTINUE;
}

HRESULT InitDirectInput2( HWND hDlg )
{
    HRESULT hr;
	   
	DIPROPDWORD dipdw;

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
    if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumFFDevicesCallback2, 0, 
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
    //hr = g_pFFDevice->SetCooperativeLevel( hDlg , DISCL_EXCLUSIVE | DISCL_FOREGROUND) ;
	hr = g_pFFDevice->SetCooperativeLevel( hDlg , DISCL_EXCLUSIVE | DISCL_BACKGROUND) ;

	

	//DISCL_NONEXCLUSIVE 

	// Since we will be playing force feedback effects, we should disable the
	// auto-centering spring.
	dipdw.diph.dwSize = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = FALSE;

	if( FAILED( hr = g_pFFDevice->SetProperty( DIPROP_AUTOCENTER, &dipdw.diph ) ) )
		return hr;

	// Enumerate and count the axes of the joystick 
	if( FAILED( hr = g_pFFDevice->EnumObjects( EnumAxesCallback,
		( VOID* )&g_dwNumForceFeedbackAxis, DIDFT_AXIS ) ) )
		return hr;


	// This simple sample only supports one or two axis joysticks
	if( g_dwNumForceFeedbackAxis > 2 )
		g_dwNumForceFeedbackAxis = 2;

	// This application needs only one effect: Applying raw forces.
	DWORD rgdwAxes[2] = { DIJOFS_X, DIJOFS_Y };
	LONG rglDirection[2] = { 0,0 };
	DICONSTANTFORCE cf = { 0 };
	cf.lMagnitude = 0;


	DIEFFECT eff;
	ZeroMemory( &eff, sizeof( eff ) );
	eff.dwSize = sizeof( DIEFFECT );
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.dwDuration = INFINITE;
	eff.dwSamplePeriod = 0;
	eff.dwGain = DI_FFNOMINALMAX;
	eff.dwTriggerButton = DIEB_NOTRIGGER;
	eff.dwTriggerRepeatInterval = 0;
	eff.cAxes = g_dwNumForceFeedbackAxis;
	eff.rgdwAxes = rgdwAxes;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;
	
	// Create the prepared effect
	if( FAILED( hr = g_pFFDevice->CreateEffect( GUID_ConstantForce,
		&eff, &g_pEffect, NULL ) ) )
	{
		return hr;
	}

	if( NULL == g_pEffect )
		return E_FAIL;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EnumFFDevicesCallback2()
// Desc: Get the first enumerated force feedback device
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumFFDevicesCallback2( LPCDIDEVICEINSTANCE pDDI, VOID* pvRef )
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
        EmptyEffectList2();
        g_pFFDevice->Unacquire();
        SAFE_RELEASE( g_pFFDevice );
    }

    // Release any DirectInput objects.
    SAFE_RELEASE( g_pDI );

    return S_OK;
}
//-----------------------------------------------------------------------------
// Name: EmptyEffectList2()
// Desc: Goes through the circular linked list and releases the effects, 
//       and deletes the nodes
//-----------------------------------------------------------------------------
VOID EmptyEffectList2()
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
// Name: OnReadFile2()
// Desc: Reads a file contain a collection of DirectInput force feedback 
//       effects.  It creates each of effect read in and stores it 
//       in the linked list, g_EffectsList.
//-----------------------------------------------------------------------------
HRESULT OnReadFile2( HWND hDlg,const char*file)
{
    HRESULT hr;

     EmptyEffectList2();

    // Enumerate the effects in the file selected, and create them in the callback
    if( FAILED( hr = g_pFFDevice->EnumEffectsInFile( file,EnumAndCreateEffectsCallback2, 
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

BOOL CALLBACK EnumAndCreateEffectsCallback2(LPCDIFILEEFFECT pDIFileEffect, VOID* pvRef )
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

HRESULT SetDeviceForcesXY(float x,float y)
{
	// Modifying an effect is basically the same as creating a new one, except
	// you need only specify the parameters you are modifying
	LONG rglDirection[2] = { 0, 0 };

	DICONSTANTFORCE cf;

	if( g_dwNumForceFeedbackAxis == 1 )
	{
		// If only one force feedback axis, then apply only one direction and 
		// keep the direction at zero
		cf.lMagnitude = x;
		rglDirection[0] = 0;
	}
	else
	{
		// If two force feedback axis, then apply magnitude from both directions 
		rglDirection[0] = x;
		rglDirection[1] = y;
		cf.lMagnitude = ( DWORD )sqrt( x * x + y * y );

	}

	DIEFFECT eff;
	ZeroMemory( &eff, sizeof( eff ) );
	eff.dwSize = sizeof( DIEFFECT );
	eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	eff.cAxes = g_dwNumForceFeedbackAxis;
	eff.rglDirection = rglDirection;
	eff.lpEnvelope = 0;
	eff.cbTypeSpecificParams = sizeof( DICONSTANTFORCE );
	eff.lpvTypeSpecificParams = &cf;
	eff.dwStartDelay = 0;

	// Now set the new parameters and start the effect immediately.
	HRESULT hr = S_OK;
	
	hr = g_pEffect->SetParameters( &eff, DIEP_DIRECTION |DIEP_TYPESPECIFICPARAMS |DIEP_START );
	HRESULT a1 = DIERR_INVALIDPARAM;
	


	return hr;
}
