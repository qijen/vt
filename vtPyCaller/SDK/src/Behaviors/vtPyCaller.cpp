#include "StdAfx.h"
#include "vtcxglobal.h"
#include "InitMan.h"
#include "vt_python_funcs.h"


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

CKPluginInfo g_PluginInfo;
vt_python_man *pym = NULL;

PLUGIN_EXPORT int CKGetPluginInfoCount(){return 2;}


CKERROR InitInstanc1e(CKContext* context);


CKERROR InitInstance(CKContext* context)
{

	CKParameterManager* pm = context->GetParameterManager();
	vt_python_man* initman =new vt_python_man(context);
	pym = initman;
	pym->py = NULL;
	initman->pLoaded = false;
	initman->RegisterVSL();
	return CK_OK;

}


CKERROR ExitInstance(CKContext* context)
{
	vt_python_man* initman =(vt_python_man*)context->GetManagerByGuid(INIT_MAN_GUID);

	DestroyPython();
	delete initman;
	return CK_OK;
}
#define INIT_BEH_GUID		CKGUID(0x587c6467,0x4b970a1)

PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	switch (Index) 
	{
	case 0:
		g_PluginInfo.m_Author			= VTCX_AUTHOR;
		g_PluginInfo.m_Description		= "Python Building Blocks";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_BEHAVIOR_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= NULL;
		g_PluginInfo.m_ExitInstanceFct	= NULL;
		g_PluginInfo.m_GUID				= INIT_BEH_GUID;
		g_PluginInfo.m_Summary			= "Python Building Blocks";
		break;

	case 1:
		g_PluginInfo.m_Author			= VTCX_AUTHOR;
		g_PluginInfo.m_Description		= "Python Manager";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_MANAGER_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= InitInstance;
		g_PluginInfo.m_ExitInstanceFct	= ExitInstance;
		g_PluginInfo.m_GUID				= INIT_MAN_GUID;
		g_PluginInfo.m_Summary			= "Python Manager";
		break;


	}
	return &g_PluginInfo;
}


PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);


void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{
	RegisterBehavior(reg,FillBehaviorCallPythonFuncDecl);
	RegisterBehavior(reg,FillBehaviorCallPythonFuncDecl2);
	RegisterBehavior(reg,FillBehaviorLoadPythonDecl);
	RegisterBehavior(reg,FillBehaviorDestroyPythonDecl);
	RegisterBehavior(reg,FillBehaviorGetNextBBIdDecl);


}
