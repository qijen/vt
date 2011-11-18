#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"
#include "xLogger.h"
int liftimeMax = 2000;

static vtNetworkManager *_im;
extern xNetInterface* GetNetInterfaceServer();
extern xNetInterface *GetNetInterfaceClient();
extern void SetNetInterfaceClient(xNetInterface *cInterface);
extern void SetNetInterfaceServer(xNetInterface *cInterface);


vtNetworkManager::vtNetworkManager(CKContext* context):CKBaseManager(context,NET_MAN_GUID,"vtNetworkManager")//Name as used in profiler
{
	m_Context->RegisterNewManager(this);

	localConnection = NULL;
	_im = this;
	mCurrentThresholdTicker = 0.0f;
	m_MinTickTime = 10.0f;

	
	initLogger();


	TNL::logprintf("asdasdd");
	xLogger::xLog(ELOGINFO,XL_START,"binding object");



    
	/*
	xBitSet test = xLogger::GetInstance()->getLogLevel(E_LI_SESSION);
	bool f = test.test(1 << ELOGWARNING);
	bool f2 = test.test(1 << ELOGTRACE);
	int h =2;*/

	//xLogger::GetInstance()->SetLogVerbosity(ELOGINFO);
	//xLogger::GetInstance()->SetLogVerbosity(ELOGERROR);


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
vtNetworkManager*vtNetworkManager::Instance(){	return _im;}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
vtNetworkManager::~vtNetworkManager()
{

	if (GetServerNetInterface())
	{
		SetServerNetInterface(NULL);
	}

	if (GetClientNetInterface())
	{
		SetClientNetInterface(NULL);
	}
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKERROR vtNetworkManager::PostProcess()
{

	if(GetClientNetInterface() && isFlagOn(GetNetInterfaceClient()->getInterfaceFlags(),E_NI_DESTROYED_BY_SERVER) )
	{
		//TNL::logprintf("server left");
		SetClientNetInterface(NULL);
		return 0;
	}
	 mCurrentThresholdTicker =getCurrentThresholdTicker() + m_Context->GetTimeManager()->GetLastDeltaTime();

	 if (getCurrentThresholdTicker() > getMinTickTime() )
	{
		mCurrentThresholdTicker = 0.0f;
	}
	/************************************************************************/
	/* advance subsystems time :                                                                      */
	/************************************************************************/
	if (GetServerNetInterface())
	{
		//GetServerNetInterface()->advanceTime(TNL::Platform::getRealMilliseconds(),m_Context->GetTimeManager()->GetLastDeltaTime());
		GetServerNetInterface()->getMessagesInterface()->advanceTime(m_Context->GetTimeManager()->GetLastDeltaTime());
	}

	if(GetClientNetInterface())
	{
		GetClientNetInterface()->advanceTime(TNL::Platform::getRealMilliseconds(),m_Context->GetTimeManager()->GetLastDeltaTime());
	}

	/************************************************************************/
	/* passing app values to distributed values, position,etc....                                                                      */
	/************************************************************************/

	if(GetClientNetInterface())
	{
	
		if(GetClientNetInterface()->isValid())
		{
			if (getCurrentThresholdTicker() < getMinTickTime() )
			{
				UpdateDistributedObjects(0);

				
			}
		}

		GetClientNetInterface()->getMessagesInterface()->checkMessages();
		GetClientNetInterface()->getMessagesInterface()->deleteAllOldMessages();
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	
	if (GetServerNetInterface())
	{
		GetServerNetInterface()->tick();
		GetServerNetInterface()->getMessagesInterface()->checkMessages();
		GetServerNetInterface()->getMessagesInterface()->deleteAllOldMessages();
	}

	if(GetClientNetInterface())
	{
		GetClientNetInterface()->tick();
	}



	if(GetClientNetInterface() && GetClientNetInterface()->isValid())
	{
		if (getCurrentThresholdTicker() < getMinTickTime() )
		{
			performGhostUpdates();
		}
	}

	return CK_OK;
}
	

//************************************
// Method:    PreProcess
// FullName:  vt_python_man::PreProcess
// Access:    public 
// Returns:   CKERROR
// Qualifier:
//************************************
CKERROR
vtNetworkManager::PreProcess()
{
	
	return CK_OK;
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKERROR vtNetworkManager::PostClearAll(){ 	return CK_OK; }
CKERROR vtNetworkManager::OnCKEnd(){	return CK_OK;}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKERROR vtNetworkManager::OnCKReset()
{

	if (GetNetInterfaceClient())
	{
		SetClientNetInterface(NULL);
	}

	if (GetNetInterfaceServer())
	{
		SetServerNetInterface(NULL);
	}
	return CK_OK;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CKERROR vtNetworkManager::OnCKPlay()
{
	
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR vtNetworkManager::OnCKInit(){
	
	m_Context->ActivateManager((CKBaseManager*) this,true);	
	

	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void
vtNetworkManager::Test(const char*inpo)
{

	xNetInterface *si= GetServerNetInterface();

	if (si)
	{
		using namespace TNL;
		Socket *socket  = &si->getSocket();
		if (socket)
		{
			bool o  = socket->isValid();

			int o1  =0;

		}

		int csize = si->getConnectionList().size();


		int op = 9 ; 

	}
}