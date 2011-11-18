
#include "CKAll.h"
#include "vtNetworkManager.h"

#include "xNetTypes.h"
#include "tnl.h"
#include "tnlPlatform.h"

#include "xNetInterface.h"
#include "tnlGhostConnection.h"
#include "vtConnection.h"

#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"


#include "IDistributedObjectsInterface.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"

#include "xPredictionSetting.h"



CKGUID GetNetManagerGUID() { 	return NET_MAN_GUID;}
extern xNetInterface* GetNetInterfaceServer();
extern xNetInterface *GetNetInterfaceClient();

xNetInterface*
GetNetInterface()
{

	if (GetNetInterfaceClient())
	{
		return GetNetInterfaceClient();
	}
	if (GetNetInterfaceServer())
	{
		return GetNetInterfaceServer();
	}

	return NULL;
}


#include "xLogger.h"

#include "VSLManagerSDK.h"

//#define VIRTOOLS_DEV35


void xSConsoleMessage(const char *msg)
{
	//TNL::logprintf(msg);
	xLogger::xLog(ELOGWARNING,E_LI_VSL,"vsl message  : %s",msg);
}
void xSConsoleError(const char *msg)
{
	//TNL::logprintf(msg);
	xLogger::xLog(ELOGERROR,E_LI_VSL,"vsl message  : %s",msg);
}

const char*
xSGetCommandLine()
{
	return GetCommandLine();

}
void vtNetworkManager::RegisterVSL()
{

		//TNLLogEnable(LogGhostConnection, true);
		//TNLLogEnable(LogNetInterface,true);
		//TNLLogEnable(LogConnectionProtocol,true);
		//TNLLogEnable(LogNetConnection,true);

		STARTVSLBIND(m_Context)


		DECLAREFUN_C_0(CKGUID, GetNetManagerGUID)
		DECLAREFUN_C_1(void,xSConsoleMessage,const char*)
		DECLAREFUN_C_1(void,xSConsoleError,const char*)
		DECLAREFUN_C_0(const char*,xSGetCommandLine)

		/************************************************************************/
		/* Dist Object enumerations                                                                      */
		/************************************************************************/

		DECLAREENUM("E_DC_BASE_TYPE")
		DECLAREENUMVALUE("E_DC_BASE_TYPE","E_DC_BTYPE_3D_ENTITY",0)
		DECLAREENUMVALUE("E_DC_BASE_TYPE","E_DC_BTYPE_2D_ENTITY",1)
		DECLAREENUMVALUE("E_DC_BASE_TYPE","E_DC_BTYPE_CLIENT",2)
		DECLAREENUMVALUE("E_DC_BASE_TYPE","E_DC_BTYPE_SESSION",3)

		DECLAREENUM("E_OBJECT_TYPE")
		DECLAREENUMVALUE("E_OBJECT_TYPE","E_OT_DIST_OBJECT",0)
		DECLAREENUMVALUE("E_OBJECT_TYPE","E_OT_DIST_PROPERTY",1)
		DECLAREENUMVALUE("E_OBJECT_TYPE","E_OT_CLASS",2)




		DECLAREENUM("E_DC_PROPERTY_TYPE")
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_3DVECTOR",0)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_QUATERNION",1)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_2DVECTOR",2)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_FLOAT",3)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_INT",4)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_BOOL",5)
		DECLAREENUMVALUE("E_DC_PROPERTY_TYPE","E_DC_PTYPE_STRING",6)

		DECLAREENUM("E_DC_3D_NATIVE_PROPERTIES")
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_LOCAL_POSITION",1)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_LOCAL_ROTATION",2)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_LOCAL_SCALE",3)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_WORLD_POSITION",4)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_WORLD_ROTATION",5)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_WORLD_SCALE",6)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_VISIBILITY",7)
		DECLAREENUMVALUE("E_DC_3D_NATIVE_PROPERTIES","E_DC_3D_NP_USER_0",8)


		DECLAREENUM("E_PREDICTION_TYPE")
		DECLAREENUMVALUE("E_PREDICTION_TYPE","E_PTYPE_PREDICTED",0)
		DECLAREENUMVALUE("E_PREDICTION_TYPE","E_PTYPE_RELIABLE",1)
		DECLAREENUMVALUE("E_PREDICTION_TYPE","E_PTYPE_NON_RELIABLE",2)
		

		DECLAREPOINTERTYPE(xDistributedClass)
		DECLAREPOINTERTYPE(xDistributed3DObjectClass)
		DECLAREPOINTERTYPE(IDistributedClasses)
		DECLAREPOINTERTYPE(IDistributedObjects)
		
		DECLAREPOINTERTYPE(xDistributedObject)
		DECLAREPOINTERTYPE(xDistributed3DObject)

		DECLAREPOINTERTYPE(xDistributedPropertyInfo)
		DECLAREPOINTERTYPE(xDistributedProperty)
		DECLAREPOINTERTYPE(xPredictionSetting)
        

		DECLAREPOINTERTYPE(vtConnection)
		DECLAREPOINTERTYPE(xNetInterface)
		
		/************************************************************************/
		/* Distributed Network Classes :						                                                                       */
		/************************************************************************/
		
		DECLAREMETHOD_2(IDistributedClasses,xDistributedClass*,createClass,const char*,int)
		DECLAREMETHOD_1(IDistributedClasses,xDistributedClass*,get,const char*)
		DECLAREMETHOD_1(IDistributedClasses,xDistributedClass*,getByIndex,int)
		DECLAREMETHOD_1(IDistributedClasses,xDistributedClass*,getByIndex,int)
		DECLAREMETHOD_0(IDistributedClasses,int,getNumClasses)

#ifndef VIRTOOLS_DEV35

		DECLAREMETHOD_1(IDistributedClasses,int,destroyClass,xDistributedClass*)
		DECLAREMETHOD_1(IDistributedClasses,void,deployClass,xDistributedClass*)

		DECLAREMETHOD_1(xDistributedClass,xDistributedClass*,cast,xDistributed3DObjectClass*) 
		DECLAREMETHOD_1(xDistributedClass,xDistributed3DObjectClass*,cast,int)

#endif


		//////////////////////////////////////////////////////////////////////////
		//
		//
		//
		//
		//////////////////////////////////////////////////////////////////////////
		DECLAREMETHOD_2(xDistributedClass,void,addProperty,int,int)
		DECLAREMETHOD_3(xDistributedClass,void,addProperty,const char*,int,int)


		DECLAREOBJECTTYPE(vtNetworkManager)

		/************************************************************************/
		/* Network Manager :						                                                                       */
		/************************************************************************/

		DECLARESTATIC_1(vtNetworkManager,vtNetworkManager*,Cast,CKBaseManager* iM)
		DECLAREMETHOD_3(vtNetworkManager,int,CreateClient,bool,int,const char*)
		DECLAREMETHOD_3(vtNetworkManager,int,CreateServer,bool,int,const char*)
		DECLAREMETHOD_2(vtNetworkManager,int,ConnectToServer,bool,const char*)
		DECLAREMETHOD_0(vtNetworkManager,int,CreateLocalConnection)
		
#ifndef VIRTOOLS_DEV35
		DECLAREMETHOD_1(vtNetworkManager,int,DeleteServer,int)
		DECLAREMETHOD_0(vtNetworkManager,float,getMinTickTime)
		DECLAREMETHOD_1(vtNetworkManager,void,setMinTickTime,float)
#endif
		



		DECLAREMETHOD_0(vtNetworkManager,xNetInterface*,GetServerNetInterface)
		DECLAREMETHOD_0(vtNetworkManager,xNetInterface*,GetClientNetInterface)

		/************************************************************************/
		/*  net interface methods :                               */
		/************************************************************************/
		DECLAREFUN_C_0(xNetInterface*, GetNetInterface)
		DECLAREMETHOD_0(xNetInterface,IDistributedObjects*,getDistObjectInterface)
		DECLAREMETHOD_0(xNetInterface,void,createScopeObject)
		DECLAREMETHOD_0(xNetInterface,IDistributedClasses*,getDistributedClassInterface)
		DECLAREMETHOD_1(xNetInterface,int,getNumDistributedObjects,int)
		DECLAREMETHOD_2(xNetInterface,void,printObjects,int,int)
		DECLAREMETHOD_3(xNetInterface,void,enableLogLevel,int,int,int)

		DECLAREMETHOD_0(xNetInterface,float,getLastOneWayTime)
		DECLAREMETHOD_0(xNetInterface,float,getLastRoundTripTime)



		/************************************************************************/
		/*IDistObjects :                                                                       */
		/************************************************************************/
		DECLAREMETHOD_2(IDistributedObjects,void,create,const char*,const char*)
		DECLAREMETHOD_3(IDistributedObjects,xDistributedObject*,create,int,const char*,const char*)
		DECLAREMETHOD_1(IDistributedObjects,xDistributedObject*,get,int)

		
		/************************************************************************/
		/* xDistributed Objects :                                                                      */
		/************************************************************************/
		DECLAREMETHOD_1(xDistributedObject,xDistributed3DObject*,cast,xDistributedObject*)
		DECLAREMETHOD_0(xDistributed3DObject,void,updateAll)
		DECLAREMETHOD_0(xDistributed3DObject,void,doInitUpdate)


#ifndef VIRTOOLS_DEV35
		DECLAREMETHOD_0(xDistributedObject,void,getGhostDebugID)
		DECLAREMETHOD_1(xDistributedObject,void,setGhostDebugID,int)
		DECLAREMETHOD_1(xDistributedObject,void,setSessionID,int)
		DECLAREMETHOD_0(xDistributedObject,int,getSessionID)


		DECLAREMETHOD_1(xDistributedObject,xDistributedProperty*,getProperty,int)
		DECLAREMETHOD_1(xDistributedObject,xDistributedProperty*,getUserProperty,const char*)
		DECLAREMETHOD_0(xDistributedProperty,xPredictionSetting*,getPredictionSettings)
		DECLAREMETHOD_1(xDistributedObject,xDistributedProperty*,getProperty,int)

		DECLAREMETHOD_1(xPredictionSetting,void,setMinSendTime,float)
		DECLAREMETHOD_0(xPredictionSetting,float,getMinSendTime)

		DECLAREMETHOD_1(xPredictionSetting,void,setMinDifference,float)
		DECLAREMETHOD_0(xPredictionSetting,float,getMinDifference)

		DECLAREMETHOD_1(xPredictionSetting,void,setPredictionFactor,float)
		DECLAREMETHOD_0(xPredictionSetting,float,getPredictionFactor)

#endif
		
		STOPVSLBIND


		
}