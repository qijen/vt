/******************************************************************************
File : CustomPlayerDefines.h

Description: This file contains some defines/enums/types used by the project.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#if !defined(CUSTOMPLAYERDEFINES_H)
#define CUSTOMPLAYERDEFINES_H


/************************************************************************/
/* Error Codes :                                                                      */
/************************************************************************/

#define INIT_ERROR						"Initialisation Error"
#define CANNOT_READ_CONFIG				"Cannot read configuration/command line.\nPlayer will quit!"
#define FAILED_TO_CREATE_WINDOWS		"Unable to create windows.\nPlayer will quit!"
#define UNABLE_TO_INIT_PLUGINS			"Unable to initialize plugins.\nPlayer will quit!"
#define UNABLE_TO_LOAD_RENDERENGINE		"Unable to load a RenderEngine.\nPlayer will quit!"
#define UNABLE_TO_INIT_CK				"Unable to initialize CK Engine.\nPlayer will quit!"
#define UNABLE_TO_INIT_MANAGERS			"Unable to initialize Managers.\nPlayer will quit!"
#define UNABLE_TO_INIT_DRIVER			"Unable to initialize display driver.\nPlayer will quit!"
#define UNABLE_TO_CREATE_RENDERCONTEXT	"Cannot initialize RenderContext.\nPlayer will quit!"
#define CANNOT_FIND_LEVEL				"Cannot find Level.\nPlayer will quit!"

#define MAINWINDOW_TITLE				"Virtools Custom Player"
#define MAINWINDOW_CLASSNAME			"CustomPlayer"
#define RENDERWINDOW_CLASSNAME			"CustomPlayerRender"
#define MISSINGUIDS_LOG					"CustomPlayerMissingGuids.log"



//  [11/28/2007 mc007] //////////////////////////////////////////////////////////////////////////
//
//	PROFILE LOAD ERRORS
//
#define CPE_OK 0
#define CPE_PROFILE_STARTING_ERROR      0
#define CPE_PROFILE_ERROR_FILE_ERROR													(CPE_PROFILE_STARTING_ERROR + 10)

#define CPE_PROFILE_ERROR_FILE_INCORRECT											(CPE_PROFILE_ERROR_FILE_ERROR + 1)
#define CPE_PROFILE_ERROR_FILE_INCORRECT_DESC									"Couldnt Load File"

#define CPE_PROFILE_ERROR_ENTRY_INCORRECT											(CPE_PROFILE_ERROR_FILE_ERROR + 2)
#define CPE_PROFILE_ERROR_ENTRY_INCORRECT_DESC									"Couldnt find entry"

#define CPE_PROFILE_ERROR_PROFILEID_INCORRECT_PARAM                       (CPE_PROFILE_ERROR_FILE_ERROR + 2)
#define CPE_PROFILE_ERROR_PROFILEID_INCORRECT_PARAM_DESC                  "Missing or incorrect parameter provided"


#define CUSTOM_PLAYER_CONFIG_FILE	"player.ini"

//  [12/15/2007 mc007]	//////////////////////////////////////////////////////////////////////////
//
//	Application Requirements :

#define CPR_CHECK_DIRECTX	1		//	this leads to a directx check, is using xUtils.dll
#define CPR_MINIMUM_DIRECTX_VERSION 9		// this is our minimum version
#define CPR_MINIMUM_DIRECTX_VERSION_FAIL_URL	"http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=2da43d38-db71-4c1b-bc6a-9b6652cd92a3"
#define CPR_OFFER_INTERNAL_DIRECTX_INSTALLER 0		// not implemented 

#define CPR_CHECK_RAM	1	//performes a memory check, is using xUtils.dll and it needs a directx !!!
#define CPR_MINIMUM_RAM	256	//our minimum pRam
#define CPR_MINIMUM_RAM_FAIL_ABORT 0	// this leads to a application exit !


#define CP_SUPPORT_EMAIL "info@atlantisvr.com" // 

//  [12/16/2007 mc007]	//////////////////////////////////////////////////////////////////////////
//
//	Application Features : 


#define CPF_SPLASH_FILE	"splash.bmp" //not used !
#define CPF_SPLASH_TEXT_TYPE	"MicrogrammaDBolExt" 
#define CPF_SPLASH_TEXT_FORMAT (DT_SINGLELINE | DT_RIGHT | DT_BOTTOM)



//  [12/17/2007 mc007]
#define CPA_SHOW_ABOUT_TAB 1	//adds an about tab, is using about.txt from \project source folder \res
#define CPA_SHOW_LICENSE_TAB 0	//adds an about tab, is using license.txt from \project source folder \res
#define CPA_SHOW_ERROR_TAB 1
#define CPA_ABORT_ON_ERROR 1	// aborts when the requirements are failing

//////////////////////////////////////////////////////////////////////////	


#define PUBLISHING_RIGHT_TITLE		"AtlantisVR.com"
#define PUBLISHING_RIGHT_TEXT	\
	"Click Okay for hooray\n"	\
	"code prior to compilation).\n\n"	\
	"This dialog box serves to remind you that publishing rights, a.k.a. runtime\n"	\
	"fees, are due when building any custom executable (like the player you just\n"	\
	"compiled). Contact info@virtools.com for more information.\n"


enum EConfigPlayer
{
	eAutoFullscreen	= 1,	// -auto_fullscreen (or -f) on command line
	eDisableKeys	= 2		// -disable_keys (or -d) on command line
};

#endif // CUSTOMPLAYERDEFINES_H