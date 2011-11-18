/********************************************************************
	created:	2009/04/14
	created:	14:4:2009   11:29
	filename: 	x:\ProjectRoot\vtmodsvn\tools\VTCPPProjectPremakerSimple\Sdk\Src\Core\DataManagerRemote.cpp
	file path:	x:\ProjectRoot\vtmodsvn\tools\VTCPPProjectPremakerSimple\Sdk\Src\Core 
	file base:	DataManagerRemote
	file ext:	cpp
	author:		Günter Baumgart
	
	purpose:	handles remote messages
*********************************************************************/
#include "StdAfx.h"
#include "DataManager.h"

#ifdef G_EXTERNAL_ACCESS

#include "MemoryFileMappingTypes.h"

#include <process.h>
#include "AutoLock.h"
#include "vtGUID.h"

using namespace AutoLock;

vtExternalEvent *Msg;

HANDLE hmem = NULL;

int post = 0;

HANDLE m_hLogItemSendEvent = ::CreateEvent(NULL,TRUE,FALSE,"LogItemSendEventName");
HANDLE m_hShutdownEvent = ::CreateEvent(NULL,FALSE,FALSE,"SendRcvShutdownEvent");
HANDLE m_hLogItemReceivedEvent = ::CreateEvent(NULL,FALSE,FALSE,"LogItemReceivedEventName");
HANDLE aHandles[] = { m_hShutdownEvent , m_hLogItemSendEvent };

BOOL recieved = false;
BOOL changed = true;




int DataManager::_SharedMemoryTickPost(int flagsOfWhatever)
{

	if (!changed){

		SetEvent( m_hShutdownEvent );
	}
	return CK_OK;

	return 0;
}

int DataManager::_SharedMemoryTick(int flagsOfWhatever)
{

	HRESULT hr = S_OK;

	//USES_CONVERSION;

	switch( ::WaitForMultipleObjects(sizeof(aHandles) /sizeof(HANDLE),&(aHandles[0]),FALSE,1))
	{

	case WAIT_OBJECT_0:	
		{	
			SetEvent( m_hShutdownEvent );
			break;
		}
	case WAIT_OBJECT_0 + 1:
		{
			try
			{

				CLockableMutex  m_mtxMMFile("sharedMem");
				CAutoLockT< CLockableMutex > lock(&m_mtxMMFile, 5000 );
				vtExternalEvent *pLI = reinterpret_cast<vtExternalEvent*>(m_pData);
				
	
				//XString command(pLI->command);
				//XString commandArg(pLI->commandArg);


				XString msgStr;
				msgStr.Format("Remote Message:%s",pLI->command);
				m_Context->OutputToConsole(msgStr.Str(),FALSE);

				changed = true;
				SetEvent( m_hLogItemReceivedEvent );
				::ResetEvent(m_hShutdownEvent);
				return CK_OK;
			}
			catch( CAutoLockTimeoutExc )
			{
				hr = HRESULT_FROM_WIN32( WAIT_TIMEOUT );
			}
			catch( CAutoLockWaitExc& e )
			{
				hr = HRESULT_FROM_WIN32( e.GetLastError() );
			}
			break;

		}
		//	default:
		//		ATLASSERT(0);
	}
	return CK_OK;
}



int DataManager::_initSharedMemory(int flagsOfWhatever)
{

	if( NULL != ( m_hMMFile = CreateFileMapping( INVALID_HANDLE_VALUE,
		NULL,PAGE_READWRITE,0,
		sizeof( vtExternalEvent ),"sharedMemFile" ) ))//_T("LogSndRcvMMF") )))
	{
		if( NULL != (m_pData = (vtExternalEvent*)::MapViewOfFile(m_hMMFile,
			FILE_MAP_READ | FILE_MAP_WRITE, 
			0,
			0,
			sizeof( vtExternalEvent* )) ) )
		{
			return S_OK;
		}
	}

	return HRESULT_FROM_WIN32( ::GetLastError( ) );
}
#endif
