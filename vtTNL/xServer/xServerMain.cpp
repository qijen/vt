
#include "xNetInterface.h"
#include "vtConnection.h"



#include <tnlAsymmetricKey.h>
//#include "windows.h"

TNL::RefPtr<xNetInterface>m_NetInterfaceServer = NULL;
TNL::RefPtr<xNetInterface>GetServerInterface(){ return m_NetInterfaceServer;}
const char *localBroadcastAddress = "IP:broadcast:28999";
const char *localHostAddress = "IP:127.0.0.1:28999";
const char *localServerAddress ="IP:Any:28999";

#include "xLogger.h"
#include "IMessages.h"

class DedicatedServerLogConsumer : public TNL::LogConsumer
{
public:
	void logString(const char *string)
	{
		//ctx()->OutputToConsoleEx("%s\n",string);
		printf("%s\n\n", string);
	}
} gDedicatedServerLogConsumer;

using namespace TNL;


float mLastTime=0.0;
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <process.h>
#include <tchar.h>
#include "xNetConstants.h"
HANDLE m_hPromptThread;
bool m_bQuitThread = false;

UINT WINAPI PromptThread( LPVOID pParam );
//-----------------------------------------------------------------------------
// Name: ParseInput
// Desc: Handle user input
//-----------------------------------------------------------------------------
void ParseInput( TCHAR* buffer )
{
	
	
	TCHAR* token = _tcstok( buffer, TEXT(" \t") );
	if( token == NULL )
		return;

	_tcsupr( token );

	if( !_tcscmp( token, TEXT("LOGLEVEL") ) ||
		!_tcscmp( token, TEXT("SETLOG") ))
	{
		token = _tcstok( NULL, TEXT(" \t") );
		int component = 0; 
		int level  = 0;
		int value = 0;
		if( token )
		{
			component = _ttol( token );

		}
		token = _tcstok( NULL, TEXT(" \t") );
		if( token )
		{
			level = _ttol( token );
		}
		
		token = _tcstok( NULL, TEXT(" \t") );
		if( token )
		{
			value = _ttol( token );
		}
		
		token = NULL;

		if (GetServerInterface())
		{
			GetServerInterface()->enableLogLevel(component,level,value);
		}
		
		return;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	if( !_tcscmp( token, TEXT("STATS") ) || 		!_tcscmp( token, TEXT("PRINT") ) )
	{
			
		token = _tcstok( NULL, TEXT(" \t") );
		int component = 0; 
		if( token )
		{
			component = _ttol( token );
			if (GetServerInterface())
			{
				GetServerInterface()->printObjects(component,component);
			}
		}else
		{
			if (GetServerInterface())
			{
				GetServerInterface()->printObjects(0,0);
			}
		}

		return;
	}
	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	if( !_tcscmp( token, TEXT("SETMSGMINTIME") ) || 		!_tcscmp( token, TEXT("MMTIME") ) )
	{

		token = _tcstok( NULL, TEXT(" \t") );
		float component = 0; 
		if( token )
		{
			component = (float)_ttol( token );
			if (GetServerInterface())
			{
				GetServerInterface()->getMessagesInterface()->setThresoldTicker(component);
			}
		}
		return;
	}
	if( !_tcscmp( token, TEXT("SETMSGTIMEOUT") ) || 		!_tcscmp( token, TEXT("MTOUT") ) )
	{

		token = _tcstok( NULL, TEXT(" \t") );
		float component = 0; 
		if( token )
		{
			component = (float)_ttol( token );
			if (GetServerInterface())
			{
				GetServerInterface()->getMessagesInterface()->setMessageTimeout(component);
			}
		}
		return;
	}
	
	if( !_tcscmp( token, TEXT("HELP") ) || 		!_tcscmp( token, TEXT("?") ) )
	{

		xLogger::xLog(ELOGINFO,E_LI_CPP,"Print component index for command setLog");
		for (int i  = 0 ; i < 16 ; i++ )
		{
			xLogger::xLogExtro(0,"Index : %d : \t %s",i,sLogItems[i]);
            
		}
		
		xLogger::xLog(ELOGINFO,E_LI_CPP,"Print verbosity levels");
		xLogger::xLogExtro(0,"0 : DEBUG");
		xLogger::xLogExtro(0,"1 : TRACE");
		xLogger::xLogExtro(0,"2 : ERROR");
		xLogger::xLogExtro(0,"3 : WARNING");
		xLogger::xLogExtro(0,"4 : INFO");

		xLogger::xLogExtro(0,"\t write 'setLog 1 2 1' to enable all session related errors" );
		xLogger::xLogExtro(0,"\t write 'setLog 1 2 0' to disable all session related errors \n" );

		xLogger::xLog(ELOGINFO,E_LI_CPP,"commands :");
		xLogger::xLogExtro(0,"\t print [Index]:0-4 : prints object informations\n" );
		xLogger::xLogExtro(0,"\t setMsgMinTime [value]:5-x ms : sets minimum time between message sending\n" );
		xLogger::xLogExtro(0,"\t setMsgTimeout [value]:5-x ms : sets message lifetime,\n" );




		return;
	}

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	if( !_tcscmp( token, TEXT("STOP") ) || 
		!_tcscmp( token, TEXT("QUIT") ) || 
		!_tcscmp( token, TEXT("EXIT") ) )
	{
		m_bQuitThread = TRUE;

	}
	
	
}
void DoPrompt( TCHAR* strPromptString, TCHAR* strBuffer )
{
	//m_pMazeApp->ConsolePrintf( LINE_PROMPT, strPromptString );

	DWORD dwRead;
	BOOL bSuccess;
	bSuccess = ReadConsole( GetStdHandle(STD_INPUT_HANDLE), strBuffer, 128, &dwRead, NULL );

	if( !bSuccess || dwRead < 2 )
	{
		_tcscpy( strBuffer, TEXT("") );
		return;
	}

	strBuffer[dwRead-2]=0;
	//m_pMazeApp->ConsolePrintf( LINE_INPUT, strBuffer );
}
UINT WINAPI StaticPromptThread( LPVOID pParam )
{
	return PromptThread( pParam );
}


//-----------------------------------------------------------------------------
// Name: PromptThread
// Desc: Thread body for infite command prompts
//-----------------------------------------------------------------------------
UINT WINAPI PromptThread( LPVOID pParam )
{
	// Loop around getting and dealing with keyboard input
	TCHAR buffer[512];
	while( !m_bQuitThread )
	{

		DoPrompt( TEXT("Command> "), buffer );
		ParseInput( buffer );
	};

	//_tprintf( TEXT("Stopping...") );

	return 0;
}



int main(int argc, const char **argv)
{


	xLogger::GetInstance()->addLogItem(E_LI_SESSION);
	//xLogger::GetInstance()->enableLoggingLevel(E_LI_SESSION,ELOGINFO,1);
	
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
	xLogger::GetInstance()->enableLoggingLevel(E_LI_CPP,ELOGINFO,1);
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


	TNLLogEnable(LogNetInterface,true);




	//////////////////////////////////////////////////////////////////////////
	//create a server : 
	
	TNL::Address *add = new TNL::Address("IP:Any:28999");
	TNL::Address *addBC=new TNL::Address("IP:broadcast:28999");
	m_NetInterfaceServer =  new xNetInterface(true,*add,*addBC);
	TNL::AsymmetricKey *theKey = new TNL::AsymmetricKey(32);
	m_NetInterfaceServer->setPrivateKey(theKey);
	m_NetInterfaceServer->setRequiresKeyExchange(false);

	if (m_NetInterfaceServer)
	{
		m_NetInterfaceServer->setAllowsConnections(true);
		
	}

	//Sleep(2000);
	if (GetServerInterface() && GetServerInterface()->getSocket().isValid() )
	{
		
		//logprintf("\t Server Created");
		xLogger::xLog(ELOGINFO,E_LI_CPP,"Server created");
		xLogger::xLog(ELOGINFO,E_LI_CPP,"enter '?' for available commands !" );


	}else{
        xLogger::xLog(ELOGERROR,E_LI_CPP,"Couldn't create socket");
	}

	
	UINT dwPromptThreadID;
	m_hPromptThread = (HANDLE)_beginthreadex( NULL, 0, StaticPromptThread, NULL, 0, &dwPromptThreadID );
	for (;;)
	{

		TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();

		float timeDelta = (currentTime - mLastTime);
		mLastTime = (float)currentTime;
		GetServerInterface()->getMessagesInterface()->advanceTime(timeDelta);
		GetServerInterface()->tick();
		GetServerInterface()->getMessagesInterface()->checkMessages();
		GetServerInterface()->getMessagesInterface()->deleteAllOldMessages();
		if (m_bQuitThread)
		{
			GetServerInterface()->destroy();
			break;
		}
		/*char message[400];
		gets(message);
		if (!strcmp(message,"exit"))
		{
			break;
		}
		

		while (kbhit()){
			getch();
		}*/
		TNL::Platform::sleep(1);
	}


	return 0;
}
