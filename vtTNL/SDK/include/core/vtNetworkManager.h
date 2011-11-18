#ifndef InitMAN_H
#define InitMAN_H



#include "CKBaseManager.h"
#include "xNetConstants.h"
#include <virtools/vtcxglobal.h>
#include "vtGuids.h"


#define NET_MAN_GUID		CKGUID(0x21aa716b,0x57e37304)


class xNetInterface;
class vtConnection;
class xDistributedObject;
class xDistributedProperty;




class vtNetworkManager : public CKBaseManager
{

	public:
		//Ctor
		vtNetworkManager(CKContext* ctx);
		//Dtor
		~vtNetworkManager();
		
		int Init();

		void initLogger();

		
		
		xNetInterface* GetServerNetInterface();
		void SetServerNetInterface(xNetInterface *cinterface);
		
		xNetInterface *GetClientNetInterface();
		void SetClientNetInterface(xNetInterface *cinterface);
        

		XString m_LastLogEntry;
		XString GetLastLogEntry() const { return m_LastLogEntry; }
		void SetLastLogEntry(XString val) { m_LastLogEntry = val; }
		
		int CreateServer(bool deleteExisting,int port,const char *address);
		void DeleteServer(int flags);
		int CreateClient(bool deleteExisting,int port,const char *address);


		vtConnection *localConnection;
		int CreateLocalConnection();

		int ConnectToServer(bool deleteExisting,const char *address);
		void UpdateDistributedObjects(DWORD flags);
		static vtNetworkManager* Instance();

		static vtNetworkManager * Cast(CKBaseManager* iM) { return Instance();}

		// Initialization	
		virtual CKERROR OnCKInit();
		virtual CKERROR OnCKEnd();
		virtual CKERROR OnCKReset();
		virtual CKERROR PreProcess();
		virtual CKERROR PostProcess();
		virtual CKERROR PostClearAll();
		virtual CKERROR OnCKPlay();

	virtual CKDWORD	GetValidFunctionsMask()	
	{ return CKMANAGER_FUNC_OnCKInit|
		CKMANAGER_FUNC_PostProcess|
		CKMANAGER_FUNC_OnCKEnd|
		CKMANAGER_FUNC_OnCKReset|
		CKMANAGER_FUNC_PreProcess|
		CKMANAGER_FUNC_PostClearAll|
		CKMANAGER_FUNC_OnCKPlay; 
	}

	
	/************************************************************************/
	/* Parameter Functions																	                                        */
	/************************************************************************/

	void Test(const char*);
	void RegisterVSL();
	bool pLoaded;

	float m_CurrentDeltaTime;
	float getCurrentDeltaTime() const { return m_CurrentDeltaTime; }
	void setCurrentDeltaTime(float val) { m_CurrentDeltaTime = val; }

	float mCurrentThresholdTicker;
	
	float& getCurrentThresholdTicker(){ return mCurrentThresholdTicker; }
	void setCurrentThresholdTicker(float val) { mCurrentThresholdTicker = val; }

	void performGhostUpdates();

	void updateLocalPosition(xDistributedObject* distObject,CK3dEntity *target,xDistributedProperty*prop);
	void updateLocalRotation(xDistributedObject* distObject,CK3dEntity *target,xDistributedProperty*prop);

	float m_MinTickTime;
	float getMinTickTime() const { return m_MinTickTime; }
	void setMinTickTime(float val) { m_MinTickTime = val; }


	void RegisterParameters();



	protected:

};


#define GetNM() vtNetworkManager::Instance()
#define ctx() vtNetworkManager::Instance()->m_Context

#endif
