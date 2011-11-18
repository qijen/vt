#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pWorldSettings.h"
#include "pLogger.h"
#include "pConfig.h"

#include "UserAllocator.h"
#include "NxControllerManager.h"
#include "xTime.h"
#include "pVehicleAll.h"
/*
class myLogConsumer : public xLogConsumer 
{

public:
	myLogConsumer() : xLogConsumer()
	{

	}

	void logString(const char *string);
};

void myLogConsumer::logString(const char *string)
{
	xLogConsumer::logString(string);

	int test  = 2;
	test++;


}

myLogConsumer pLogConsumer;
*/

#include <xDebugTools.h>

int PhysicManager::initManager(int flags/* =0 */)
{
	xBitSet inFlags = flags;
	xBitSet resultFlags = 0;



	setProcessOptions(pVPO_CheckLowSpeed|pVPO_Lat_Damping|pVPO_Long_Damping|pVPO_SA_Damping|pVPO_Wheel_UsePHYSX_CONTACT_DATA|pVPO_Wheel_UsePHYSX_Load|pVPO_SV_Tansa);



	xAssertionEx::Instance();
	customizeAsserts();
	
	////////////////////////////////////////Load our physic default xml document : 
	if (isFlagOn(inFlags,E_MFI_LOAD_CONFIG) &&  getDefaultConfig())
	{
		delete m_DefaultDocument;
		m_DefaultDocument = NULL;
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Deleted old default config");
	} 

	if (isFlagOn(inFlags,E_MFI_LOAD_CONFIG))
	{
		TiXmlDocument * defaultDoc  = loadDefaults("PhysicDefaults.xml");
		if (!defaultDoc)
		{
			//enableFlag(resultFlags,E_MF_LOADING_DEFAULT_CONFIG_FAILED);
			enableFlag(_getManagerFlags(),E_MF_LOADING_DEFAULT_CONFIG_FAILED);
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Loading default config  :PhysicDefaults.xml: failed");
			setDefaultConfig(NULL);
		}else
		{
			setDefaultConfig(defaultDoc);
		}

	}
	
	////////////////////////////////////////////////////////////////////////////factory : 
	if (isFlagOn(inFlags,E_MFI_CREATE_FACTORY))
	{
		if (getCurrentFactory())
		{
			
			getCurrentFactory()->setPhysicSDK(getPhysicsSDK());
			enableFlag(_getManagerFlags(),E_MF_FACTORY_CREATED);
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Old factory updated");
		}
		if (!getCurrentFactory())
		{

			pFactory *factory = new pFactory(this,getDefaultConfig());
			factory->setPhysicSDK(getPhysicsSDK());
			enableFlag(_getManagerFlags(),E_MF_FACTORY_CREATED);
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"New factory created");
			setCurrentFactory(factory);
		}
	}

	if (getDefaultWorldSettings())
	{
		SAFE_DELETE(mDefaultWorldSettings);
	}

	//////////////////////////////////////////////////////////////////////////
	//world settings :
	pWorldSettings *wSettings  = pFactory::Instance()->createWorldSettings(XString("Default"),getDefaultConfig());
	if (!wSettings)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Couldn't load default world settings!");
		wSettings = new pWorldSettings();
	}

	setDefaultWorldSettings(wSettings);
	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Default world settings created");


	//////////////////////////////////////////default world : 
	if (isFlagOn(_getManagerFlags(),E_MF_FACTORY_CREATED) &&
		getDefaultWorldSettings() 	)
	{
		if (pFactory::Instance()->createDefaultWorld("pDefaultWorld"))
		{
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Default world created");
		}
		//else 			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Creating default world failed");
	}
	_RegisterDynamicParameters();

	int mask  = resultFlags.getMask();

	//----------------------------------------------------------------
	//
	// character controllers
	//
	mUserAllocator = new UserAllocator();
	_createControllerManager();

	return mask;


}
void PhysicManager::_createControllerManager()
{

	mControllerManager = NxCreateControllerManager(mUserAllocator);

}

int PhysicManager::initPhysicEngine(int flags /* = 0 */)
{

	if (getPhysicsSDK())
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Physic SDK already initiated! Releasing old SDK ");
		NxReleasePhysicsSDK(getPhysicsSDK());
		setPhysicsSDK(NULL);
	}

	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
//NxSDKCreationFlag
	//NxSDKCreationFlag
//	NX_SIMULATION_SW
	pErrorStream* eStream = getLogger()->getErrorStream(); 
	mPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL,(NxUserOutputStream*)eStream, desc, &errorCode);
	if(mPhysicsSDK == NULL) 
	{

		xLogger::xLog(XL_START,ELOGTRACE,E_LI_AGEIA,"Physic SDK already initiated! Releasing old SDK ");
		enableFlag(_getManagerFlags(),E_MF_PSDK_FAILED);
		return E_PE_AGEIA_ERROR;
	}
	
	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Physic SDK initiated!");
	
	if (getDefaultConfig())
	{
		pRemoteDebuggerSettings rSettings  = pFactory::Instance()->createDebuggerSettings(getDefaultConfig());

		if (rSettings.enabled)
		{
			mPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(rSettings.mHost.CStr(),rSettings.port);
			
			if (!mPhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected())
			{
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"couldn't connect to remote debugger !");
			}else 
			{

				mRemoteDebugger = mPhysicsSDK->getFoundationSDK().getRemoteDebugger();

			}
		}
	}
	

	/*mPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	mPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	mPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1);
	mPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, 1);
*/
	
	setPSDKParameters(getSDKParameters());

	pRemoteDebuggerSettings &dbgSetup = getRemoteDebuggerSettings();
	if (dbgSetup.enabled)
	{

		mPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(dbgSetup.mHost.CStr(),dbgSetup.port);
		if (!mPhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected())
		{
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"couldn't connect to remote debugger !");
		}else 
		{

			mRemoteDebugger = mPhysicsSDK->getFoundationSDK().getRemoteDebugger();
		}
	}

	enableFlag(_getManagerFlags(),E_MF_PSDK_LOADED);
	return E_PE_OK;
}
int PhysicManager::performInitialization()
{

	XString buffer;
#ifdef DEMO_ONLY

	SYSTEMTIME sys;

	GetSystemTime(&sys);

	int dayd  = sys.wDay;
	int monthd = sys.wMonth;
	int yeard = sys.wYear;

	if (!GetContext()->IsInInterfaceMode()){
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"sorry guy, just a demo !");
		MessageBox(NULL,"vtPhysX Demonstration","VTMOD Productions",0);
		//return E_PE_INVALID_OPERATION;
	}

	if ( yeard == 2010 )
	{
		if (monthd <=END_MONTH && monthd >=START_MONTH )
		{
			goto init;
		}else goto expired;
	}else goto expired;


expired : 


	buffer << "today is the " << dayd << " of " << monthd << "\n";
	buffer << "but this release only works until : " << END_MONTH;
	xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,buffer.CStr());
	xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Demo expired!");
	return E_PE_INVALID_OPERATION;

#endif


init:




	xBitSet initFlags = 0 ;
	enableFlag(initFlags,E_MFI_LOAD_CONFIG);
	enableFlag(initFlags,E_MFI_CREATE_FACTORY);
	int error = initManager( initFlags.getMask());
	if (error == E_PE_INVALID_OPERATION )
	{
		return 0;
	}


	if (isFlagOff(_getManagerFlags(),E_MF_PSDK_LOADED))
	{
		if (initPhysicEngine()!= E_PE_OK )
		{
			return E_MF_PSDK_FAILED;
		}

		enableFlag(initFlags,E_MFI_USE_XML_WORLD_SETTINGS);
		enableFlag(initFlags,E_MFI_CREATE_DEFAULT_WORLD);


		int error = initManager( initFlags.getMask());
		if (error != E_PE_OK )
		{
            xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't init manager");
			return E_PE_INVALID_OPERATION;
		}
	}
	return E_PE_OK;
}

bool PhysicManager::isValid()
{
	return isFlagOn(_getManagerFlags(),E_MF_PSDK_LOADED) &&
		isFlagOn(_getManagerFlags(),E_MF_FACTORY_CREATED) &&
		getDefaultWorldSettings() && getDefaultWorld();
}

void PhysicManager::_construct(xBitSet flags /* = 0  */)
{
	mManagerFlags = 0 ;
	mPhysicsSDK = NULL;
	mLogger = NULL;
	mDefaultWorldSettings = NULL;
	m_DefaultDocument  = NULL;
	m_currentFactory = NULL;
	m_DefaultDocument = NULL;
	m_currentFactory = NULL;
	m_DefaultWorld = NULL;
	mRemoteDebugger = NULL;
	mIParameter = NULL;
	mUserAllocator = NULL;
	mControllerManager = NULL;
	restoreMap = NULL;


	
	time = new xTime();
	xLogger::GetInstance()->setVirtoolsContext(m_Context);

	xLogger::GetInstance()->addLogItem(E_LI_AGEIA);
	xLogger::GetInstance()->addLogItem(E_LI_MANAGER);
	xLogger::GetInstance()->addLogItem(E_VSL);
	xLogger::GetInstance()->addLogItem(E_BB);

	xLogger::GetInstance()->addItemDescription("AGEIA");
	xLogger::GetInstance()->addItemDescription("MANAGER");
	xLogger::GetInstance()->addItemDescription("VSL");
	xLogger::GetInstance()->addItemDescription("BB");

	
	xLogger::GetInstance()->enableLoggingLevel(E_BB,ELOGERROR,1);
	xLogger::GetInstance()->enableLoggingLevel(E_BB,ELOGTRACE,0);
	xLogger::GetInstance()->enableLoggingLevel(E_BB,ELOGWARNING,0);
	xLogger::GetInstance()->enableLoggingLevel(E_BB,ELOGINFO,0);

	xLogger::GetInstance()->enableLoggingLevel(E_LI_AGEIA,ELOGERROR,1);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_AGEIA,ELOGTRACE,0);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_AGEIA,ELOGWARNING,0);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_AGEIA,ELOGINFO,0);

	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGERROR,1);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGTRACE,0);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGWARNING,0);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGINFO,0);

	SetLastLogEntry("");

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Initiated");

	if (!getLogger())
	{
		setLogger(new pLogger());
	}
}
