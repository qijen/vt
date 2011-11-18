#include "CKAll.h"
#include "Manager/InitMan.h"



#ifdef CK_LIB
#define RegisterBehaviorDeclarations	Register_TOOLS_BehaviorDeclarations
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

CKPluginInfo g_PluginInfo;

PLUGIN_EXPORT int CKGetPluginInfoCount(){return 2;}

#ifdef Dev25
#include "vslmanagersdk.h"
#endif

#ifdef Dev3
#include "VSLManager.h"
#endif



CKERROR InitInstance(CKContext* context)
{

	CKParameterManager* pm = context->GetParameterManager();
	InitMan* initman =new InitMan(context);
	initman->RegisterVSL();

	
	return CK_OK;

}

CKERROR ExitInstance(CKContext* context)
{
	InitMan* initman =(InitMan*)context->GetManagerByGuid(INIT_MAN_GUID);

	delete initman;
	return CK_OK;
}
#define INIT_BEH_GUID		CKGUID(0x64cb5f9a,0x1aac3b37)

PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	switch (Index) 
	{
	case 0:
		g_PluginInfo.m_Author			= "mw";
		g_PluginInfo.m_Description		= "tool building blocks";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_BEHAVIOR_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= NULL;
		g_PluginInfo.m_ExitInstanceFct	= NULL;
		g_PluginInfo.m_GUID				= INIT_BEH_GUID;
		g_PluginInfo.m_Summary			= "tool BB's";
		break;

	case 1:
		g_PluginInfo.m_Author			= "mw";
		g_PluginInfo.m_Description		= "tool Manager ";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_MANAGER_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= InitInstance;
		g_PluginInfo.m_ExitInstanceFct	= ExitInstance;
		g_PluginInfo.m_GUID				= INIT_MAN_GUID;
		g_PluginInfo.m_Summary			= "tool Manager";
		break;


	}
	return &g_PluginInfo;
}


PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);

void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{



	

	RegisterBehavior(reg,FillBehaviorAddNodalLinkDecl);

	RegisterBehavior(reg,FillBehaviorDirToArrayDecl);
	RegisterBehavior(reg,FillBehaviorSetNodalDifficultDecl);
	RegisterBehavior(reg,FillBehaviorCopyFileBBDecl);
	RegisterBehavior(reg,FillBehaviorExecBBDecl);
	

	//  [11/6/2004] // FTP

	/*
	RegisterBehavior(reg,FillBehaviorFTPLoginDecl);
	RegisterBehavior(reg,FillBehaviorGetFileDecl);
	RegisterBehavior(reg,FillBehaviorSendFileDecl);*


	//  [11/6/2004] //Zip
	RegisterBehavior(reg,FillBehaviorLoadUnZipLibraryDecl);
	RegisterBehavior(reg,FillBehaviorLoadZipLibraryDecl);
	RegisterBehavior(reg,FillBehaviorUnzipFilesDecl);
	RegisterBehavior(reg,FillBehaviorZipFilesDecl);	**/
	RegisterBehavior(reg,FillBehaviorSaveObjectsDecl);
	

	
	RegisterBehavior(reg,FillBehaviorTextureSinusDecl);
	RegisterBehavior(reg,FillBehaviorNoiseDecl);

	
	RegisterBehavior(reg, FillBehaviorHasFFEffectsDecl);
	RegisterBehavior(reg, FillBehaviorJSetXYForceDecl);


	RegisterBehavior(reg,FillBehaviorGetCurrentPathDecl);



	RegisterBehavior(reg,FillBehaviorResolveFileNameDecl);
	RegisterBehavior(reg,FillBehaviorMimicDecl);
	/*
	
	RegisterBehavior(reg,*/
//	RegisterBehavior(reg,FillBehaviorCharacterControllerDecl);
}



