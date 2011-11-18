////////////////////////////////////////////////////////////////////////////////
//
// ARPlusBehavior 
// --------------
//
// Description:
// Defines the initialization routines for the plugin DLL. Done by the Virtools
// wizzard
//
// Version 1.0	: First Release
//
// Known Bugs	: None
//
// Copyright Jörg Stöcklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////


#include "CKAll.h"
#include "ARToolKitPlusManager.h"


//----------------------------------------------------------------
//
//! \brief Switches between static library or dll 
//
#ifdef CK_LIB
#define RegisterBehaviorDeclarations	Register_VTAR_BehaviorDeclarations
	#define InitInstance					_VTAR_InitInstance
	#define ExitInstance					_VTAR_ExitInstance
	#define CKGetPluginInfoCount			CKGet_VTAR_PluginInfoCount
	#define CKGetPluginInfo					CKGet_VTAR_PluginInfo
#define g_PluginInfo					g_PluginInfo
#else
#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
	#define InitInstance					InitInstance
	#define ExitInstance					ExitInstance
	#define CKGetPluginInfoCount			CKGetPluginInfoCount
	#define CKGetPluginInfo					CKGetPluginInfo
	#define g_PluginInfo					g_PluginInfo
#endif



CKERROR InitInstance(CKContext* context)
{
#ifdef Webpack
	InitInstanceCamera(context);
#endif

	new ARToolKitPlusManager(context);
	return CK_OK;
}

CKERROR ExitInstance(CKContext* context)
{
	// This function will only be called if the dll is unloaded explicitely 
	// by a user in Virtools Dev interface
	// Otherwise the manager destructor will be called by Virtools runtime directly

	delete context->GetManagerByGuid(ARToolKitPlusManagerGUID);

#ifdef Webpack
	ExitInstanceCamera(context);
#endif

	return CK_OK;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifdef WebPack
	extern CKERROR InitInstanceCamera(CKContext* context);
	extern CKERROR ExitInstanceCamera(CKContext* context);
	extern int RegisterCameraBeh(XObjectDeclarationArray *reg);
#endif

#ifdef WebPack
	#define GUID_MODULE_BUILDING_BLOCKS	CKGUID(0x12d94eba,0x47057415)
#else
	#define GUID_MODULE_BUILDING_BLOCKS	CKGUID(0x6d8452a7, 0x94937df2)
#endif

/************************************************************************/
/*                                                                      */
/************************************************************************/

#define PLUGIN_COUNT 2

CKPluginInfo g_PluginInfo[PLUGIN_COUNT];

int CKGetPluginInfoCount() {	return PLUGIN_COUNT; }

/*
*******************************************************************
* Function: CKPluginInfo* CKGetPluginInfo( int index )
*
* Description : This function takes an index and returns a corresponding plugin desciptor
*		
* Parameters :
*    index, index of the plugin information to get
*
* Returns : pointer to the requested plugin information
*
*******************************************************************
*/
PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index);

PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	int Plugin = 0;
	// Register ARTPlus
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "The ARTPlus building block initialize the ARToolKitPlus for Single marker detection";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL;
	g_PluginInfo[Plugin].m_GUID				= GUID_MODULE_BUILDING_BLOCKS;
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Tracker for Single Marker";
	Plugin++;
	
	/*
	// Register ARTPlusPatternTransformation
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "Buildingblock that transforms a given object by the detected transformation matrix of the corrosponding pattern (single marker)";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL; 
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0x6b733302,0x12d23c1f);
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Pattern Transformation";
	Plugin++;
	// Register ARToolKitPlusDetect
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "Buildingblock that detects pattern (single marker) in the given video image";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL; 
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0x1ff46552,0x6c31e58);
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Single Marker Detection";
	Plugin++;
	//Register ARTPlusDetectionAndTransformation
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "Buildingblock that detects a pattern (single marker) and transforms the related Object";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL; 
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0x2f9e2472,0x639e208c);
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Single Marker Detection and Transformation";
	Plugin++;
	// Register ARTPlusMulti
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "The ARTPlus building block initialize the ARToolKitPlus for multi marker detection";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL; 
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0xa59f1,0x3d486f81);
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Tracker for Multi Marker";
	Plugin++;
	//Register ARTPlusMultiDetectionAndTransformation
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "Buildingblock that detects a pattern (multi marker) and transforms the related Object";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL; 
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0x61232b96,0x598d625a);
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Multi Marker Detection and Transformation";
	Plugin++;
	*/

	// Register ARToolKitPlusManager
	g_PluginInfo[Plugin].m_Author			= "Jörg Stöcklein";
	g_PluginInfo[Plugin].m_Description		= "This manager is used for setting the right camera projection matrix if an ARToolKitPlus building block is used and the flag ARTPlus(Multi)CorrectCamera is set";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_MANAGER_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00000001;
	g_PluginInfo[Plugin].m_InitInstanceFct	= InitInstance;
	g_PluginInfo[Plugin].m_ExitInstanceFct	= ExitInstance;
	g_PluginInfo[Plugin].m_GUID				= ARToolKitPlusManagerGUID;
	g_PluginInfo[Plugin].m_Summary			= "ARToolKitPlus Manager";
	return &g_PluginInfo[Index];
}

void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{

	RegisterBehavior(reg, FillBehaviorARTPlusDecl);
	RegisterBehavior(reg, FillBehaviorARTPlusMultiDecl);
	RegisterBehavior(reg, FillBehaviorARToolKitPlusDetectDecl);
	RegisterBehavior(reg, FillBehaviorARTPlusPatternTransformationDecl);
	RegisterBehavior(reg, FillBehaviorARTPlusDetectionAndTransformationDecl);
	RegisterBehavior(reg, FillBehaviorARTPlusMultiDetectionAndTransformationDecl);

	#ifdef WebPack
		RegisterCameraBeh(reg);
	#endif

}






