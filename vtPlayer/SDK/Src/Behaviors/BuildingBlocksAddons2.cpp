#include "Manager.h"

#ifdef CK_LIB
	#define RegisterBehaviorDeclarations	Register_CSHARP_BehaviorDeclarations
	#define InitInstance					_CSHARP_InitInstance
	#define ExitInstance					_CSHARP_ExitInstance
	#define CKGetPluginInfoCount			CKGet_CSHARP_PluginInfoCount
	#define CKGetPluginInfo					CKGet_CSHARP_PluginInfo
	#define g_PluginInfo					g_CSHARP_PluginInfo
#else
	#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
	#define InitInstance					InitInstance
	#define ExitInstance					ExitInstance
	#define CKGetPluginInfoCount			CKGetPluginInfoCount
	#define CKGetPluginInfo					CKGetPluginInfo
	#define g_PluginInfo					g_PluginInfo
#endif


PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);

#define ADDONS2_BEHAVIOR	CKGUID(0x119d47be,0x74c0241b)

CKPluginInfo g_PluginInfo;

PLUGIN_EXPORT int CKGetPluginInfoCount() { return	 2; }

CKERROR InitInstance(CKContext* context)
{
	CSManager* csman =new CSManager(context);
	return CK_OK;
}	

CKERROR ExitInstance(CKContext* context)
{
	CSManager* cman =(CSManager*)context->GetManagerByGuid(INIT_MAN_GUID);
	delete cman;
	return CK_OK;
}	

PLUGIN_EXPORT  CKPluginInfo* CKGetPluginInfo(int Index)
{
	switch (Index) 
	{
	case 0:
		g_PluginInfo.m_Author			= "Günter Baumgart";
		g_PluginInfo.m_Description		= "tool building blocks";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_BEHAVIOR_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= NULL;
		g_PluginInfo.m_ExitInstanceFct	= NULL;
		g_PluginInfo.m_GUID				= ADDONS2_BEHAVIOR;
		g_PluginInfo.m_Summary			= "CSharp BB";
		break;

	case 1:
		g_PluginInfo.m_Author			= "Günter Baumgart";
		g_PluginInfo.m_Description		= "tool Manager ";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_MANAGER_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= InitInstance;
		g_PluginInfo.m_ExitInstanceFct	= ExitInstance;
		g_PluginInfo.m_GUID				= INIT_MAN_GUID;
		g_PluginInfo.m_Summary			= "CSharpManager";
		break;
	}
	return &g_PluginInfo;

}

/**********************************************************************************/
/**********************************************************************************/


void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{
	
	RegisterBehavior(reg, FillBehaviorSendCSMessageDecl);
	/*
	// Get Nearest Vertex
	RegisterBehavior(reg, FillBehaviorGetNearestVertexDecl);
	// Scene Antialias
	RegisterBehavior(reg, FillBehaviorSceneAntialiasDecl);
	// Merge Mesh
	RegisterBehavior(reg, FillBehaviorMergeMeshDecl);
	// Get Host Platform
	RegisterBehavior(reg, FillBehaviorGetHostPlatformDecl);
	//Open File
	RegisterBehavior(reg, FillBehaviorOpenFileDecl);*/
	
}
