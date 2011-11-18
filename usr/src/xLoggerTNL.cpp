///////////////////////////////////////////////////////////
//  xLogger.cpp
//  Implementation of the Class xLogger
//  Created on:      10-Feb-2007 12:40:39
///////////////////////////////////////////////////////////
//#include <virtools/vtCXGlobal.h>
//#include <pch.h>

#include <Windows.h>


#include <stdio.h> 
#include <stdarg.h>

#include "xLogger.h"
#include "uxString.h"
#include "xBitSet.h"

#include <tnl.h>
#include "tnlLog.h"









using xUtils::xLogger;
using xUtils::xLoggerFlags;
//using xUtils::xLogConsumer;
using namespace xUtils;

/*
using namespace TNL;
using namespace TNL::Platform;
*/




xLogConsumer *xLogConsumer::mLinkedList = NULL;
void xLogConsumer::logString(const char *string)
{
	// by default the LogConsumer will output to the platform debug 
	// string printer, but only if we're in debug mode
//#ifdef TNL_DEBUG

	TNL::Platform::outputDebugString(string);
	TNL::Platform::outputDebugString("\n");

	//printf(string);
/*	OutputDebugString(string);
	OutputDebugString("\n");
*/

}

xLogConsumer::xLogConsumer()
{
	mNextConsumer = mLinkedList;
	if(mNextConsumer)
		mNextConsumer->mPrevConsumer = this;
	mPrevConsumer = NULL;
	mLinkedList = this;
}

xLogConsumer::~xLogConsumer()
{
	if(mNextConsumer)
		mNextConsumer->mPrevConsumer = mPrevConsumer;
	if(mPrevConsumer)
		mPrevConsumer->mNextConsumer = mNextConsumer;
	else
		mLinkedList = mNextConsumer;
}



static xLogger *xlog=NULL;

const char*formatLine(const char *msg,...);
void printMessage(const char*buffer);

#include <cstdio>
#define ANSI            /* Comment out for UNIX version     */
#ifdef ANSI             /* ANSI compatible version          */
	#include <stdarg.h>
	int average( int first, ... );
#else                   /* UNIX compatible version          */
	#include <varargs.h>
	int average( va_list );
#endif


#ifdef VIRTOOLS_USER_SDK
	#include "CKAll.h"
#endif

void xLogger::finalPrint(const char*string)
{

#ifdef VIRTOOLS_USER_SDK

	if (getVirtoolsContext())
	{
		getVirtoolsContext()->OutputToConsole(const_cast<char*>(string),FALSE);
	}
#endif

	if (strlen(string))
	{
		printf(string);
		TNL::Platform::outputDebugString(string);

	}
	
	for(xLogConsumer *walk = xLogConsumer::getLinkedList(); walk; walk = walk->getNext())
		walk->logString(string);

}

void xLogger::enableLoggingLevel(int item,int level,int enable)
{

	std::map<int,xBitSet>::iterator it = getLogItems().find(item);

	if ( it !=getLogItems().end() )
	{

		xBitSet& flags  = it->second;
		if (level !=8)
		{
			flags.set( 1 << level,enable);
		}else
			flags.set();
		
	}
}


//************************************
// Method:    getLogLevel
// FullName:  xLogger::getLogLevel
// Access:    public 
// Returns:   xBitSet
// Qualifier:
// Parameter: int item
//************************************
xBitSet xLogger::getLogLevel(int item)
{
	std::map<int,xBitSet>::iterator it = getLogItems().find(item);

	if ( it !=getLogItems().end() )
	{
		return it->second;
	}

	xBitSet result;
	result.set();
	return result;


}






//************************************
// Method:    addLogItem
// FullName:  xLogger::addLogItem
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int item
//************************************
void xLogger::addLogItem(int item)
{


	xBitSet flags;
	flags.set(1 << ELOGWARNING,true);
	flags.set(1 << ELOGERROR,true);
	flags.set(1 << ELOGINFO,true);

	getLogItems().insert(std::make_pair(item,flags));


}









//************************************
// Method:    setLoggingLevel
// FullName:  xLogger::setLoggingLevel
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int item
// Parameter: xBitSet flags
//************************************
void xLogger::setLoggingLevel(int item,xBitSet flags)
{

	std::map<int,xBitSet>::iterator it = getLogItems().find(item);

	if ( it !=getLogItems().end() )
	{
        xBitSet& lflags = it->second;
		lflags = flags;
        
	}

}








//************************************
// Method:    dVsprintf
// FullName:  dVsprintf
// Access:    public 
// Returns:   signed int
// Qualifier:
// Parameter: char *buffer
// Parameter: unsigned int bufferSize
// Parameter: const char *format
// Parameter: void *arglist
//************************************
signed int dVsprintf(char *buffer, unsigned int bufferSize, const char *format, void *arglist)
{
#ifdef TNL_COMPILER_VISUALC
	signed int len = _vsnprintf(buffer, bufferSize, format, (va_list) arglist);
#else
	signed int len = vsnprintf(buffer, bufferSize, format, (char *) arglist);
#endif
	return len;
}









//************************************
// Method:    xLog
// FullName:  xLogger::xLog
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char *cppText
// Parameter: int type
// Parameter: int component
// Parameter: const char *header
// Parameter: ...
//************************************
void xLogger::xLog(char *cppText,int type,int component,const char *header, ...)
{


	
	if (!xLogger::GetInstance()->getLogLevel(component).test(1<< type) )
	{
		return;
	}

	char buffer[4096];
	unsigned int bufferStart = 0;

	va_list s;
	va_start( s, header );
	dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, header, s);

	va_end(s);

	char headerout[2048];
	if (strlen(cppText))
	{		
		sprintf(headerout,"%s :\t->%s",cppText,header);
		xLog(type,component,headerout,buffer);
	}else
	{
		xLog(type,component,header,buffer);
	}

}





//************************************
// Method:    xLog
// FullName:  xLogger::xLog
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: xBitSet styleFlags
// Parameter: int type
// Parameter: int component
// Parameter: const char *header
// Parameter: ...
//************************************
void xLogger::xLog(xBitSet styleFlags,int type,int component,const char *header, ...)
{

	/*
	if (!xLogger::GetInstance()->getLogLevel(component).test(1<< type) )
	{
		return;
	}

	char buffer[4096];
	unsigned int bufferStart = 0;

	va_list s;
	va_start( s, header );
	dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, header, s);
	va_end(s);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	char headerBuffer[4096];
	char verbosityStr[512];
	char componentString[256];
	sprintf(componentString,"%s",sLogItems[component]);


	uxString leadIn("");

	if (type > 4)	{		type =4;	} 
	if (type < 0)	{		type =0;	} 

	if ( styleFlags.test(E_PSF_PRINT_LOG_TYPE) )
	{
		leadIn << "\n<-----------------> " << sLogTypes[type] << " :  ";
	}
	
	if ( styleFlags.test(E_PSF_PRINT_COMPONENT) )
	{
		leadIn << "|---" << componentString << "--|";
	}

	leadIn << "\n";
	


	switch(type)
	{
		case ELOGINFO:
			SetConsoleTextAttribute(hConsole, 10);
			break;
		case ELOGTRACE:
			SetConsoleTextAttribute(hConsole, 11);
			break;
		case ELOGERROR:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
			break;
		case ELOGWARNING:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN);
			break;
	}

	if ( styleFlags.test(E_PSF_PRINT_LOG_TYPE) ||  styleFlags.test(E_PSF_PRINT_COMPONENT) )
		TNL::logprintf(leadIn.CStr());

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED |FOREGROUND_GREEN | FOREGROUND_BLUE);

	char buffer2[4096];
	sprintf(buffer2," : %s ",buffer);
	TNL::logprintf(buffer2);

	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);
*/
	return;

}

//************************************
// Method:    xLog
// FullName:  xLogger::xLog
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int type
// Parameter: int component
// Parameter: const char *header
// Parameter: ...
//************************************
void xLogger::xLog(int type,int component,const char *header, ...)
{


	
	if (!xLogger::GetInstance()->getLogLevel(component).test(1<< type) )
	{
		return;
	}

	char buffer[4096];
	unsigned int bufferStart = 0;

	va_list s;
	va_start( s, header );
	dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, header, s);
	va_end(s);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	char headerBuffer[4096];
	char verbosityStr[512];
	char componentString[256];

	int descriptionS = xLogger::GetInstance()->getItemDescriptions().size();
	const char* cString = GetInstance()->getItemDescriptions().at(component);
	if(component <= xLogger::GetInstance()->getItemDescriptions().size() )
		sprintf(componentString,"%s",GetInstance()->getItemDescriptions().at(component));
	else
		sprintf(componentString,"UNKNOWN COMPONENT");



	
	switch(type)
	{
	case ELOGINFO:
		{
			sprintf(verbosityStr,"\n<----------------->	INFO :  |---%s---|\n",componentString);
			SetConsoleTextAttribute(hConsole, 10);
		}
		break;
	case ELOGTRACE:
		{
			sprintf(verbosityStr,"\n<----------------->	TRACE :  |---%s---|\n",componentString);
			SetConsoleTextAttribute(hConsole, 11);
		}
		break;
	case ELOGERROR:
		{
			sprintf(verbosityStr,"\n<----------------->	ERROR  :   |---%s---|\n",componentString);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
		break;
	case ELOGWARNING:
		{
			sprintf(verbosityStr,"\n<----------------->	WARNING : |---%s---|\n",componentString);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);
		}
		break;
	}
	sprintf(headerBuffer,"%s",verbosityStr);
	GetInstance()->finalPrint(headerBuffer);


	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);

	char buffer2[4096];
	sprintf(buffer2," : %s\n",buffer);
	GetInstance()->finalPrint(buffer2);


	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);

	return;

}


//************************************
// Method:    xLogExtro
// FullName:  xLogger::xLogExtro
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int style
// Parameter: const char *header
// Parameter: ...
//************************************
void xLogger::xLogExtro(int style,const char *header, ...)
{


	char buffer[4096];
	unsigned int bufferStart = 0;

	va_list s;
	va_start( s, header );
	dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, header, s);
//	TNL::Platform::outputDebugString(buffer);
//	TNL::Platform::outputDebugString("\n");
	va_end(s);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	char headerBuffer[4096];

	sprintf(headerBuffer,"%s\n",header);
	GetInstance()->finalPrint(buffer);


	//printf(headerBuffer);

	/*
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN);
	*/

//	printf("\n%s",buffer);
	
//TNL::logprintf(headerBuffer);
//	TNL::logprintf(buffer);
	/*
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);*/


	return;



}

void 
xLogger::xLog(int verbosity,const char *header,const char*msg,...)
{

	
		char buffer[4096];
		unsigned int bufferStart = 0;

		va_list s;
		va_start( s, msg );
		dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, msg, s);
		va_end(s);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	
		char headerBuffer[4096];
		char verbosityStr[512];
	
		switch(verbosity)
		{
			case ELOGINFO:
				{
					sprintf(verbosityStr,"--------------------------------------INFO	\n-\n");
					SetConsoleTextAttribute(hConsole, 10);
				}
			break;
			case ELOGERROR:
				{
					sprintf(verbosityStr,"ooo--------------------------------ERROR	\n-\n");
					/*SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_INTENSITY | FOREGROUND_RED |
						FOREGROUND_GREEN | FOREGROUND_BLUE);*/
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_INTENSITY | FOREGROUND_RED);
				}
				break;
		}
		//sprintf(headerBuffer,"%s %s\n\t%s\n----",verbosityStr,header,buffer);
		sprintf(headerBuffer,"%s %s-\n--",verbosityStr,header);
		printf(headerBuffer);
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);

		printf("--->%s\n",buffer);
	
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);



		//TNL::logprintf(headerBuffer);
		return;

		va_list va;
		va_start (va,msg);
		char bufferN[1024] = { 0 } ; 
		vsprintf(bufferN,msg,va );
		va_end (va);
		printMessage(bufferN);
	

}

//************************************
// Method:    getLogFlags
// FullName:  xLogger::getLogFlags
// Access:    public 
// Returns:   xBitSet&
// Qualifier:
//************************************
xBitSet& xLogger::getLogFlags()
{
	return GetInstance()->mLogFlags ;
}


//************************************
// Method:    getVerbosityFlags
// FullName:  xLogger::getVerbosityFlags
// Access:    public 
// Returns:   xBitSet&
// Qualifier:
//************************************
xBitSet& xLogger::getVerbosityFlags()
{
	return GetInstance()->m_VerbosityFlags;
}

//************************************
// Method:    isLogging
// FullName:  xLogger::isLogging
// Access:    public 
// Returns:   bool
// Qualifier:
// Parameter: int logItem
//************************************
bool xLogger::isLogging(int logItem)
{ 
	return GetInstance()->getVerbosityFlags().test(1<<logItem); 
}

//************************************
// Method:    enableLogItem
// FullName:  xLogger::enableLogItem
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int logItem
// Parameter: bool enbabled
//************************************
void xLogger::enableLogItem(int logItem,bool enbabled)
{ 
	return GetInstance()->getVerbosityFlags().set(1<< logItem,enbabled); 
}

/*
void 
xLogger::xLog(DWORD verbosity,const char*msg,...)
{

		char buffer[4096];
		unsigned int bufferStart = 0;

		va_list s;
		va_start( s, msg );
		dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, msg, s);
		va_end(s);

		//printf(buffer);
		TNL::logprintf(buffer);
		return;

		va_list va;
		va_start (va,msg);
		char bufferN[1024] = { 0 } ; 
		vsprintf(bufferN,msg,va );
		va_end (va);
		printMessage(bufferN);

}
*/


void 
xLogger::Init()
{

}

xLogger::xLogger()
{

	xlog = this;
	m_LogOutChannels = ELOGGER_NONE;

#ifdef VIRTOOLS_USER_SDK
	mContext = NULL;
#endif



}

//************************************
// Method:    GetInstance
// FullName:  xLogger::GetInstance
// Access:    public 
// Returns:   xLogger*
// Qualifier:
//************************************
xLogger*xLogger::GetInstance()
{
	if (xlog ==NULL)	{
		xlog = new xLogger();	}
	return xlog;
}

void printMessage(const char*buffer)
{

	using namespace xUtils;

	/*if ( (xlog->m_LogOutChannels & ELOGGER_CONSOLE) && console )
	{
		//Consoleln(buffer);
		fflush (stderr);
		fflush (stdout);
		strcpy(xlog->m_ConsStream->buf,buffer);

	}

	if ( (xlog->m_LogOutChannels & ELOGGER_CEGUI) && ceGUI && xGuiSystem::GetInstance()->m_RenderingGUI )
	{
		getApp->SetDebugPanelText(0,buffer);
	}*/
}


xLogger::~xLogger()
{

}
