// DataRelay.cpp : Defines the initialization routines for the plugin DLL.
//
#include "CKAll.h"
#include "DataManager.h"

#include "bginstancer.h"
#include "gblasyncblock.h"



#ifdef CK_LIB
#define RegisterBehaviorDeclarations Register_TOOLS_BehaviorDeclarations		
#define InitInstance					_TOOLS_InitInstance
#define ExitInstance					_TOOLS_ExitInstance
#define CKGetPluginInfoCount			CKGet_TOOLS_PluginInfoCount
#define CKGetPluginInfo					CKGet_TOOLS_PluginInfo
#define g_PluginInfo					g_TOOLS_PluginInfo
#else
#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
#define InitInstance					InitInstance
#define ExitInstance					ExitInstance
#define CKGetPluginInfoCount			CKGetPluginInfoCount
#define CKGetPluginInfo					CKGetPluginInfo
#define g_PluginInfo					g_PluginInfo
#endif

CKERROR InitInstance(CKContext* context);
CKERROR ExitInstance(CKContext* context);

#define PLUGIN_COUNT 2

CKPluginInfo g_PluginInfo[PLUGIN_COUNT];

PLUGIN_EXPORT int CKGetPluginInfoCount(){return 2;}


PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	int Plugin = 0;
	g_PluginInfo[Plugin].m_Author			= "Virtools";
	g_PluginInfo[Plugin].m_Description		= "Generic tools";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00010000;
	g_PluginInfo[Plugin].m_InitInstanceFct	= NULL;
	g_PluginInfo[Plugin].m_GUID				= CKGUID(0xbd1e1d69, 0x0ee9fdca);
	g_PluginInfo[Plugin].m_Summary			= "Collection of generic tools";
	Plugin++;
	g_PluginInfo[Plugin].m_Author			= "Virtools";
	g_PluginInfo[Plugin].m_Description		= "Tools manager";
	g_PluginInfo[Plugin].m_Extension		= "";
	g_PluginInfo[Plugin].m_Type				= CKPLUGIN_MANAGER_DLL;
	g_PluginInfo[Plugin].m_Version			= 0x00010000;
	g_PluginInfo[Plugin].m_InitInstanceFct	= InitInstance;
	g_PluginInfo[Plugin].m_ExitInstanceFct	= ExitInstance;
	g_PluginInfo[Plugin].m_GUID				= DataManagerGUID;
	g_PluginInfo[Plugin].m_Summary			= "Tool manager";
	return &g_PluginInfo[Index];
}

//  If no manager is used in the plugin 
//	these functions are optional and can be exported.
//  Virtools will call 'InitInstance' when loading the behavior library
//  and 'ExitInstance' when unloading it.
//	It is a good place to perform Attributes Types declaration, 
//  registering new enums or new parameter types.

CKERROR InitInstance(CKContext* context)
{
  new DataManager(context);
	return CK_OK;
}

CKERROR ExitInstance(CKContext* context)
{
	// This function will only be called if the dll is unloaded explicitely 
	// by a user in Virtools Dev interface
	// Otherwise the manager destructor will be called by Virtools runtime directly
  delete context->GetManagerByGuid(DataManagerGUID);
	return CK_OK;
}

void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{

	::CKStoreDeclaration(reg, BGWrapper::FillBehaviour());
	::CKStoreDeclaration(reg, ExeInThread::FillBehaviour());

	RegisterBehavior(reg,FillBehaviorGetAdaptersDecl);
	RegisterBehavior(reg,FillBehaviorGetAdaptersModieDecl);
	RegisterBehavior(reg,FillBehaviorGoFullScreenDecl);



	RegisterBehavior(reg, FillBehaviorHasFFEffectsDecl);
	RegisterBehavior(reg, FillBehaviorJSetXYForceDecl);


	RegisterBehavior(reg,FillBehaviorGetCurrentPathDecl);
	
	RegisterBehavior(reg,FillBehaviorDirToArrayDecl);
	
	

}






