#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"
#include "xDOStructs.h"

#include "IDistributedObjectsInterface.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"

#include "vtNetConfig.h"

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

PLUGIN_EXPORT int CKGetPluginInfoCount(){return 2;}


CKERROR InitInstanc1e(CKContext* context);

vtNetworkManager *nm = NULL;


CKERROR InitInstance(CKContext* context)
{

	CKParameterManager* pm = context->GetParameterManager();
	vtNetworkManager* initman =new vtNetworkManager(context);
	nm = initman;
	
	initman->RegisterVSL();
	initman->RegisterParameters();

	return CK_OK;

}


CKERROR ExitInstance(CKContext* context)
{
	vtNetworkManager* initman =(vtNetworkManager*)context->GetManagerByGuid(NET_MAN_GUID);
	delete initman;
	return CK_OK;
}
#define INIT_BEH_GUID		CKGUID(0x8ac02ae,0x224341f4)

PLUGIN_EXPORT CKPluginInfo* CKGetPluginInfo(int Index)
{
	switch (Index) 
	{
	case 0:
		g_PluginInfo.m_Author			= "Günter Baumgart";
		g_PluginInfo.m_Description		= "Network Building Blocks";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_BEHAVIOR_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= NULL;
		g_PluginInfo.m_ExitInstanceFct	= NULL;
		g_PluginInfo.m_GUID				= INIT_BEH_GUID;
		g_PluginInfo.m_Summary			= "Network Building Blocks";
		break;

	case 1:
		g_PluginInfo.m_Author			= "Günter Baumgart";
		g_PluginInfo.m_Description		= "Network Manager";
		g_PluginInfo.m_Extension		= "";
		g_PluginInfo.m_Type				= CKPLUGIN_MANAGER_DLL;
		g_PluginInfo.m_Version			= 0x000001;
		g_PluginInfo.m_InitInstanceFct	= InitInstance;
		g_PluginInfo.m_ExitInstanceFct	= ExitInstance;
		g_PluginInfo.m_GUID				= NET_MAN_GUID;
		g_PluginInfo.m_Summary			= "Network Manager";
		break;


	}
	return &g_PluginInfo;
}

#undef HAS_DISTOBJECTS


PLUGIN_EXPORT void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);


void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{

	RegisterBehavior(reg,FillBehaviorConnectToServerDecl);
	
	

	RegisterBehavior(reg,FillBehaviorLogEntryDecl);

	RegisterBehavior(reg,FillBehaviorGetIncomingUserDecl);
	RegisterBehavior(reg,FillBehaviorGetOutgoingUserUserDecl);
	RegisterBehavior(reg,FillBehaviorSetUserNameDecl);
	RegisterBehavior(reg,FillBehaviorUserNameModifiedDecl);
	RegisterBehavior(reg,FillBehaviorNDisconnectDecl);
	
	RegisterBehavior(reg,FillBehaviorDOCreateDistributedObjectDecl);
	RegisterBehavior(reg,FillBehaviorDODistributedObjectCreatedDecl);
	RegisterBehavior(reg,FillBehaviorDOUserValueModifiedDecl);
	RegisterBehavior(reg,FillBehaviorDOBindDecl);
	RegisterBehavior(reg,FillBehaviorDODestroyDecl);
	RegisterBehavior(reg,FillBehaviorDODestroyedDecl);
	RegisterBehavior(reg,FillBehaviorDOSetUserValueDecl);
	RegisterBehavior(reg,FillBehaviorDOControlDecl);
	RegisterBehavior(reg,FillBehaviorDOOwnerChangedDecl);
	RegisterBehavior(reg,FillBehaviorDOGetCurrentOwnerDecl);
	RegisterBehavior(reg,FillBehaviorDOReleaseDecl);


	//////////////////////////////////////////////////////////////////////////
	

	RegisterBehavior(reg,FillBehaviorNSCreateObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSGetListObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSJoinObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSLeaveObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSLockObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSUnlockObjectDecl);
	RegisterBehavior(reg,FillBehaviorNSRemoveUserDecl);

	RegisterBehavior(reg,FillBehaviorNMSendDecl);
	RegisterBehavior(reg,FillBehaviorNMWaiterDecl);

	RegisterBehavior(reg,FillBehaviorNCheckForLanServerDecl);


	RegisterBehavior(reg,FillBehaviorDebugTextDecl);

	RegisterBehavior(reg,FillBehaviorNServerStartDecl);
	RegisterBehavior(reg,FillBehaviorNServerStopDecl);
	RegisterBehavior(reg,FillBehaviorNServerIsRunningDecl);
	RegisterBehavior(reg,FillBehaviorNGetInterfacesDecl);
	RegisterBehavior(reg,FillBehaviorNSetParametersDecl);



}
