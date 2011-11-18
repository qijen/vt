#include <StdAfx.h>
#include "pCommon.h"
#include "pConfig.h"
#include "gConfig.h"

#pragma message(__FILE__ NXPHYSXLOADERDLL_API )


void FindResourceX();

#ifdef WebPack
	extern CKERROR InitInstanceCamera(CKContext* context);
	extern CKERROR ExitInstanceCamera(CKContext* context);
	extern int RegisterCameraBeh(XObjectDeclarationArray *reg);
#endif


//----------------------------------------------------------------
//
//! \brief Switches between static library or dll 
//
#ifdef CK_LIB
	#define RegisterBehaviorDeclarations	Register_VTPHYSX_BehaviorDeclarations
	#define InitInstance					_VTPHYSX_InitInstance
	#define ExitInstance					_VTPHYSX_ExitInstance
	#define CKGetPluginInfoCount			CKGet_VTPHYSX_PluginInfoCount
	#define CKGetPluginInfo					CKGet_VTPHYSX_PluginInfo
	#define g_PluginInfo					g_PluginInfo
#else
	#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
	#define InitInstance					InitInstance
	#define ExitInstance					ExitInstance
	#define CKGetPluginInfoCount			CKGetPluginInfoCount
	#define CKGetPluginInfo					CKGetPluginInfo
	#define g_PluginInfo					g_PluginInfo
#endif

/*
*******************************************************************
* Function: CKERROR InitInstance( CKContext* theContext )
*
* Description : If no manager is used in the plugin these functions 
*               are optional and can be exported. Virtools will call 
*               'InitInstance' when loading the behavior library and 
*               'ExitInstance' when unloading it. It is a good place 
*               to perform Attributes Types declaration, registering new 
*               enums or new parameter types.
*		
* Parameters : CKContext* theContext
*    
*
* Returns : CKERROR
*
*******************************************************************
*/
CKERROR InitInstanceBB(CKContext* context)
{


	return CK_OK;
}

CKERROR ExitInstanceBB(CKContext* context)
{


	return CK_OK;
}

/*
*******************************************************************
* Function: CKERROR InitInstance( CKContext* theContext )
*
* Description : If no manager is used in the plugin these functions 
*               are optional and can be exported. Virtools will call 
*               'InitInstance' when loading the behavior library and 
*               'ExitInstance' when unloading it. It is a good place 
*               to perform Attributes Types declaration, registering new 
*               enums or new parameter types.
*		
* Parameters : CKContext* theContext
*    
*
* Returns : CKERROR
*
*******************************************************************
*/
CKERROR InitInstanceIM(CKContext* context){


#ifdef Webpack
	InitInstanceCamera(context);
#endif

	PhysicManager* PM =new PhysicManager(context);
	context->ActivateManager(PM,TRUE);

#ifdef DONGLE_VERSION
	PM->_initResources(0);
#endif

#ifndef DONGLE_VERSION
	PM->DongleHasBasicVersion =1;
#endif // _DEBUG


	if(PM->DongleHasBasicVersion)
	{

		PM->_RegisterDynamicParameters();
		PM->_RegisterParameters();
		PM->_RegisterVSL();


#ifdef HAS_FLUIDS
		PM->_RegisterFluid_VSL();
#endif // HAS_FLUIDS
		PM->_RegisterParameterOperations();
		PM->_registerWatchers(context);
	}
	return CK_OK;
}

/*
*******************************************************************
* Function: CKERROR ExitInstance( CKContext* theContext )
*
* Description : This function will only be called if the dll is 
*               unloaded explicitely by a user in Virtools Dev interface
*               Otherwise the manager destructor will be called by Virtools 
*               runtime directly.
*		
* Parameters : CKContext* theContext
*    
* Returns : CKERROR
*
*******************************************************************
*/
CKERROR ExitInstanceIM(CKContext* context){
	PhysicManager* PM =(PhysicManager*)context->GetManagerByGuid(GUID_MODULE_MANAGER);
	delete PM;

	#ifdef Webpack
		ExitInstanceCamera(context);
	#endif



	return CK_OK;
}



CKPluginInfo g_PluginInfo[2];
#include "base_macros.h"




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

/*
*******************************************************************
* Function: int CKGetPluginInfoCount() 
*
* Description : Returns the number of plugins in this DLL
*		
* Parameters :
*    None.
*
* Returns : Number of plugins defined in this DLL
*
*******************************************************************
*/
int  CKGetPluginInfoCount(){return 2;}
PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);



PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	g_PluginInfo[0].m_Author			= VTCX_AUTHOR; 
	g_PluginInfo[0].m_Description		= VTCX_API_ENTRY("PhysX Building Blocks");
	g_PluginInfo[0].m_Extension		= "";
	g_PluginInfo[0].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[0].m_Version			= 0x000001;
	g_PluginInfo[0].m_InitInstanceFct	= InitInstanceBB;
	g_PluginInfo[0].m_ExitInstanceFct	= ExitInstanceBB;
	g_PluginInfo[0].m_GUID				= GUID_MODULE_BUILDING_BLOCKS;

	g_PluginInfo[1].m_Author			=VTCX_AUTHOR;
	g_PluginInfo[1].m_Description		=VTCX_API_ENTRY("PhysX Manager");
	g_PluginInfo[1].m_Extension			= "";
	g_PluginInfo[1].m_Type				= CKPLUGIN_MANAGER_DLL;
	g_PluginInfo[1].m_Version			= 0x000001;
	g_PluginInfo[1].m_InitInstanceFct	= InitInstanceIM;
	g_PluginInfo[1].m_ExitInstanceFct	= ExitInstanceIM;
	g_PluginInfo[1].m_GUID				= GUID_MODULE_MANAGER;
	g_PluginInfo[1].m_Summary =		VTCX_API_ENTRY("PhysX Physic Manager");

		
	return &g_PluginInfo[Index];
}

/*
******************************************************************* 
* Function: void RegisterBehaviorDeclarations( XObjectDeclarationArray *reg )
* 
* Description : 
* 
* Parameters : 
* param rw/r/w param description 
* 
* Returns : 
* 
******************************************************************* 
*/ 

void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{


	//----------------------------------------------------------------
	//
	// generic building blocks 
	//

#ifdef BBC_TOOLS
	RegisterBehavior(reg,FillBehaviorGetNextBBIdDecl);
#endif
/*
#ifdef BBC_JOYSTICK
	RegisterBehavior(reg,FillBehaviorHasFFEffectsDecl);
	RegisterBehavior(reg,FillBehaviorJSetXYForceDecl);
#endif
	*/

	/************************************************************************/
	/* Bodies:                                                                     */
	/************************************************************************/
	
	RegisterBehavior(reg,FillBehaviorPBPhysicalizeDecl);

	RegisterBehavior(reg,FillBehaviorPBAddTorqueDecl);
	RegisterBehavior(reg,FillBehaviorPBAddForceDecl);
	
	RegisterBehavior(reg,FillBehaviorPBAddForceAtLocalPosDecl);
	RegisterBehavior(reg,FillBehaviorPBAddLocalForceDecl);
	RegisterBehavior(reg,FillBehaviorPBAddLocalForceAtPosDecl);
	RegisterBehavior(reg,FillBehaviorPBAddLocalForceAtLocalPosDecl);
	RegisterBehavior(reg,FillBehaviorPBAddLocalTorqueDecl);
	
	RegisterBehavior(reg,FillBehaviorAddForceAtPosDecl);
	
	
	RegisterBehavior(reg,FillBehaviorPBSetDecl);
	RegisterBehavior(reg,FillBehaviorPBSet2Decl);

	RegisterBehavior(reg,FillBehaviorPBSetParDecl);
	RegisterBehavior(reg,FillBehaviorPBPhysicalizeExDecl);
	RegisterBehavior(reg,FillBehaviorPBAddShapeExDecl);

	RegisterBehavior(reg,FillBehaviorPBSetHardDecl);



	RegisterBehavior(reg,FillBehaviorPBGetDecl);
	RegisterBehavior(reg,FillBehaviorPBGet2Decl);


	RegisterBehavior(reg,FillBehaviorPBGetParameterDecl);
	RegisterBehavior(reg,FillBehaviorPBGetVelocitiesAndForcesDecl);

	RegisterBehavior(reg,FillBehaviorPBDestroyDecl);

#ifdef BBC_JOINTS
	/************************************************************************/
	/* D6                                                                     */
	/************************************************************************/

	RegisterBehavior(reg,FillBehaviorPJGroupBreakIteratorDecl);

	RegisterBehavior(reg,FillBehaviorJSetD6Decl);
	RegisterBehavior(reg,FillBehaviorJD6SetMotionModeDecl);
	RegisterBehavior(reg,FillBehaviorJD6SetSoftLimitDecl);
	RegisterBehavior(reg,FillBehaviorJD6SetDriveDecl);
	RegisterBehavior(reg,FillBehaviorJD6SetParametersDecl);

	/************************************************************************/
	/* Joints                                                                     */
	/************************************************************************/
	RegisterBehavior(reg,FillBehaviorJSetBallDecl);
	RegisterBehavior(reg,FillBehaviorPJSetBreakForcesDecl);
	RegisterBehavior(reg,FillBehaviorJSetFixedDecl);
	RegisterBehavior(reg,FillBehaviorPJIsBrokenDecl);
	RegisterBehavior(reg,FillBehaviorJDestroyDecl);
	RegisterBehavior(reg,FillBehaviorPJCreateDistanceJointDecl);
	RegisterBehavior(reg,FillBehaviorPJRevoluteDecl);
	RegisterBehavior(reg,FillBehaviorPJIteratorDecl);

	RegisterBehavior(reg,FillBehaviorPJPulleyDecl);
	RegisterBehavior(reg,FillBehaviorPJPrismaticDecl);
	RegisterBehavior(reg,FillBehaviorPJCylindricalDecl);
	RegisterBehavior(reg,FillBehaviorPJPointPlaneDecl);
	RegisterBehavior(reg,FillBehaviorPJPointOnLineDecl);
	RegisterBehavior(reg,FillBehaviorPJAddLimitPlaneDecl);
#endif // BBC_JOINTS




	/************************************************************************/
	/* collision                                                                     */
	/************************************************************************/
	
	RegisterBehavior(reg,FillBehaviorPWSetCollisionGroupFlagDecl);
	RegisterBehavior(reg,FillBehaviorPCIgnorePairDecl);
	RegisterBehavior(reg,FillBehaviorCollisionsCheckADecl);
	RegisterBehavior(reg,FillBehaviorPBSetTriggerMaskDecl);
	RegisterBehavior(reg,FillBehaviorPCTriggerEventDecl);
	RegisterBehavior(reg,FillBehaviorPCGroupTriggerEventDecl);

	
	/************************************************************************/
	/* Shapes :                                                                      */
	/************************************************************************/

	RegisterBehavior(reg,FillBehaviorPBAddShapeDecl);
	RegisterBehavior(reg,FillBehaviorPBRemoveShapeDecl);

	RegisterBehavior(reg,FillBehaviorPBSetCallbackDecl);


	/************************************************************************/
	/* Clothes :                                                                      */
	/************************************************************************/


#ifdef DONGLE_VERSION
	if(!PhysicManager::DongleHasAdvancedVersion)
		return;
#endif


#ifdef BBC_CLOTHES


	RegisterBehavior(reg,FillBehaviorPClothDecl);
	RegisterBehavior(reg,FillBehaviorPClothAttachToShapeDecl);
	//RegisterBehavior(reg,FillBehaviorPClothAttachToCoreDecl);

	RegisterBehavior(reg,FillBehaviorPClothAttachVertexToShapeDecl);
	RegisterBehavior(reg,FillBehaviorPClothAttachVertexToPositionDecl);
	RegisterBehavior(reg,FillBehaviorPClothFreeVertexDecl);
	RegisterBehavior(reg,FillBehaviorPClothDominateVertexDecl);
	RegisterBehavior(reg,FillBehaviorPClothDetachFromShapeDecl);
	RegisterBehavior(reg,FillBehaviorPClothDestroyDecl);
	RegisterBehavior(reg,FillBehaviorPClothAddForceAtPosDecl);
	RegisterBehavior(reg,FillBehaviorPClothAddForceAtVertexDecl);
	RegisterBehavior(reg,FillBehaviorPClothAddDirectedForceAtPosDecl);

#endif

	RegisterBehavior(reg,FillBehaviorPWSetFilteringDecl);
	RegisterBehavior(reg,FillBehaviorPWRayCastAllShapeDecl);
	RegisterBehavior(reg,FillBehaviorPWRayCastAnyBoundsDecl);
	RegisterBehavior(reg,FillBehaviorPWOverlapSphereDecl);

	RegisterBehavior(reg,FillBehaviorPWOBBOverlapDecl);
	//////////////////////////////////////////////////////////////////////////
	//
	//	Vehicles and Wheels  : 
	
#ifdef BBC_VEHICLES
	RegisterBehavior(reg,FillBehaviorPVWGetDecl);
	RegisterBehavior(reg,FillBehaviorPVWSetDecl);
	RegisterBehavior(reg,FillBehaviorPVSetDecl);
	RegisterBehavior(reg,FillBehaviorPVSetExDecl);
	RegisterBehavior(reg,FillBehaviorPVControlDecl);
	
	RegisterBehavior(reg,FillBehaviorPVSetMotorValuesDecl);
	RegisterBehavior(reg,FillBehaviorPVSetGearsDecl);
	RegisterBehavior(reg,FillBehaviorPVControl2Decl);
	RegisterBehavior(reg,FillBehaviorPVSetBreakSettingsDecl);
	RegisterBehavior(reg,FillBehaviorPVGetDecl);
#endif

	///////////////////////////////////////////////////////////////////////////
	//
	//	Material 
	RegisterBehavior(reg,FillBehaviorPBSetMaterialDecl);

	///////////////////////////////////////////////////////////////////////////
	//
	//	Material 

	RegisterBehavior(reg,FillBehaviorPManagerDecl);
	RegisterBehavior(reg,FillBehaviorRegisterAttributeTypeDecl);

	RegisterBehavior(reg,FillBehaviorLogEntryDecl);
	RegisterBehavior(reg,FillBehaviorPMaterialIteratorDecl);



	#ifdef WebPack
		RegisterCameraBeh(reg);
	#endif

}
