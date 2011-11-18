#include "InitMan.h"
#ifndef _WIN32_WINNT	
#define _WIN32_WINNT 0x0501
#endif	

#include <iostream>
#include <iomanip>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include "AutoLock.h"
using namespace AutoLock;
vtExternalEvent *Msg;
HANDLE hmem = NULL;

int post = 0;
extern InitMan *_im;

#pragma comment (lib,"ATLSD.LIB")

HANDLE m_hLogItemSendEvent = ::CreateEvent(NULL,TRUE,FALSE,"LogItemSendEventName");
HANDLE m_hShutdownEvent = ::CreateEvent(NULL,FALSE,FALSE,"SendRcvShutdownEvent");
HANDLE m_hLogItemReceivedEvent = ::CreateEvent(NULL,FALSE,FALSE,"LogItemReceivedEventName");
HANDLE aHandles[] = { m_hShutdownEvent , m_hLogItemSendEvent };

BOOL vt_UnloadPlugin(CKGUID input);
BOOL vt_ReloadPlugin(CKGUID input);
BOOL recieved = false;
BOOL changed = true;

void
InitMan::InitMessages(int flags,XString name)
{

	m_Context->ActivateManager((CKBaseManager*) this,true);	

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
			//					return S_OK;
		}
	}
	//return HRESULT_FROM_WIN32( ::GetLastError( ) );
}



CKERROR InitMan::PostProcess()
{	
	if (!changed){

		SetEvent( m_hShutdownEvent );
	}
	return CK_OK;
}

void
InitMan::PerformMessages()
{

//	m_Context->OutputToConsole("performe");
	HRESULT hr = S_OK;

	USES_CONVERSION;

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

				if (pLI)
				{
					m_Context->OutputToConsole("vtPluginProxy : msg recieved",FALSE);
					
					XString command(pLI->command);
					m_Context->OutputToConsoleEx(command.Str());
					XString commandArg(pLI->commandArg);
					if (!command.Compare("unload"))
					{
						/*vtGUID inputGuid;
						inputGuid.FromString(commandArg);
						m_Context->OutputToConsole("vtPluginProxy : Unload  Plugin",FALSE);
						vt_UnloadPlugin(inputGuid.GetVirtoolsGUID());*/

					}

					if (!command.Compare("reload"))
					{
						/*vtGUID inputGuid;
						inputGuid.FromString( commandArg );
						m_Context->OutputToConsole("vtPluginProxy : Reload  Plugin",FALSE);
						vt_ReloadPlugin(inputGuid.GetVirtoolsGUID());*/
					}

					changed = true;
					SetEvent( m_hLogItemReceivedEvent );
					::ResetEvent(m_hShutdownEvent);
				}

				return ;
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
	default:
		int op=0;
//		ATLASSERT(0);
	}
}

