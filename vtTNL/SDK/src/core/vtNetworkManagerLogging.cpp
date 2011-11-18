#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"

#include "xNetInterface.h"

#include "tnlRandom.h"
#include "tnlSymmetricCipher.h"
#include "tnlAsymmetricKey.h"
#include <pch.h>
#include <vtNetAll.h>
#include "xLogger.h"


class vtLogConsumer :  public xLogConsumer
{
public:
	
	void logString(const char *string)
	{
		
		XString newLog(string);
		if (GetNM()->GetLastLogEntry().Compare(newLog))
		{
		if ( string && strlen(string) < 255)
		{
			GetNM()->SetLastLogEntry(string);
			ctx()->OutputToConsoleEx(newLog.Str());

		}

		//printf("%s\n", string);
		}
	}

}gVTLogger;

class DedicatedServerLogConsumer : public TNL::LogConsumer
{
public:
	void logString(const char *string)
	{
		gVTLogger.logString(string);
		/*
		XString newLog(string);
		if (GetNM()->GetLastLogEntry().Compare(newLog))
		{
			if ( string && strlen(string) < 255)
			{
				GetNM()->SetLastLogEntry(string);
				ctx()->OutputToConsoleEx(newLog.Str());

			}
			
			//printf("%s\n", string);
		}

		*/
	}
} gDedicatedServerLogConsumer;



void vtNetworkManager::initLogger()
{

	xLogger::GetInstance()->addLogItem(E_LI_SESSION);
	//xLogger::GetInstance()->enableLoggingLevel(E_LI_SESSION,ELOGINFO,1);
	//xLogger::GetInstance()->enableLoggingLevel(E_LI_SESSION,ELOGTRACE,1);
	xLogger::GetInstance()->addLogItem(E_LI_CLIENT);
	xLogger::GetInstance()->addLogItem(E_LI_3DOBJECT);
	xLogger::GetInstance()->addLogItem(E_LI_2DOBJECT);
	xLogger::GetInstance()->addLogItem(E_LI_DISTRIBUTED_BASE_OBJECT);
	xLogger::GetInstance()->addLogItem(E_LI_DISTRIBUTED_CLASS_DESCRIPTORS);
	xLogger::GetInstance()->addLogItem(E_LI_MESSAGES);
	xLogger::GetInstance()->addLogItem(E_LI_ARRAY_MESSAGES);
	xLogger::GetInstance()->addLogItem(E_LI_CONNECTION);
	xLogger::GetInstance()->addLogItem(E_LI_NET_INTERFACE);
	xLogger::GetInstance()->addLogItem(E_LI_GHOSTING);
	xLogger::GetInstance()->addLogItem(E_LI_STATISTICS);
	xLogger::GetInstance()->addLogItem(E_LI_BUILDINGBLOCKS);
	xLogger::GetInstance()->addLogItem(E_LI_VSL);
	xLogger::GetInstance()->addLogItem(E_LI_CPP);
	xLogger::GetInstance()->addLogItem(E_LI_ASSERTS);
	xLogger::GetInstance()->addLogItem(E_LI_PREDICTION);
	xLogger::GetInstance()->addLogItem(E_LI_SERVER_MESSAGES);

	xLogger::GetInstance()->addItemDescription("Session");
	xLogger::GetInstance()->addItemDescription("Client");
	xLogger::GetInstance()->addItemDescription("3dObject");
	xLogger::GetInstance()->addItemDescription("2dObject");
	xLogger::GetInstance()->addItemDescription("DistBase Object");
	xLogger::GetInstance()->addItemDescription("DistClassDescr");
	xLogger::GetInstance()->addItemDescription("Messages");
	xLogger::GetInstance()->addItemDescription("ArrayMessages");
	xLogger::GetInstance()->addItemDescription("Connection");
	xLogger::GetInstance()->addItemDescription("NetInterface");
	xLogger::GetInstance()->addItemDescription("Ghosting");
	xLogger::GetInstance()->addItemDescription("Stats");
	xLogger::GetInstance()->addItemDescription("BB");
	xLogger::GetInstance()->addItemDescription("VSL");
	xLogger::GetInstance()->addItemDescription("CPP");
	xLogger::GetInstance()->addItemDescription("Asserts");
	xLogger::GetInstance()->addItemDescription("Prediction");
	xLogger::GetInstance()->addItemDescription("ServerMsg");

}