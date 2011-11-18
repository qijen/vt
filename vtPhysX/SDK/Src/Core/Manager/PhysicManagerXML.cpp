#include <StdAfx.h>
#include "vtPhysXAll.h"




void PhysicManager::reloadXMLDefaultFile(const char*fName)
{

	
	if (! fName || !strlen(fName))return;



	////////////////////////////////////////Load our physic default xml document : 
	if (getDefaultConfig())
	{
		delete m_DefaultDocument;
		m_DefaultDocument = NULL;
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Deleted old default config");

	} 

	TiXmlDocument * defaultDoc  = loadDefaults(XString(fName));
	if (!defaultDoc)
	{
		enableFlag(_getManagerFlags(),E_MF_LOADING_DEFAULT_CONFIG_FAILED);
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Loading default config  : PhysicDefaults.xml: failed");
		setDefaultConfig(NULL);
	}else
	{
		setDefaultConfig(defaultDoc);
	}

}