#include <StdAfx.h>
#include <stdio.h> 
#include <cstdio>
#include <stdarg.h>

#include "xLogger.h"
#include "uxString.h"
#include "xBitSet.h"
#include <Base.h>

#ifdef VIRTOOLS_USER_SDK
	#include "CKAll.h"
#endif

#include <ConStream.h>

using xUtils::xLogger;
using xUtils::xLoggerFlags;
using namespace xUtils;

ConStream *consoleStream = NULL;

//	consoleStream = new ConStream();

void
xLogger::enableConsoleOutput(bool enable)
{
	if (enable)
	{
		if (!consoleStream)
		{
			m_LogOutChannels |= ELOGGER_CONSOLE;
			consoleStream = new ConStream();
			consoleStream->Open();
		}else{
            consoleStream->Open();
		}

	}
	if (!enable && consoleStream)
	{
		consoleStream->Close();
	}
}

xLogConsumer *xLogConsumer::mLinkedList = NULL;
void xLogConsumer::logString(const char *string)
{
	OutputDebugString(string);
	OutputDebugString("\n");
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


#define ANSI            /* Comment out for UNIX version     */
#ifdef ANSI             /* ANSI compatible version          */
	#include <stdarg.h>
	int average( int first, ... );
#else                   /* UNIX compatible version          */
	#include <varargs.h>
	int average( va_list );
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
		OutputDebugString(string);
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

void xLogger::addLogItem(int item)
{


	xBitSet flags;
	flags.set(1 << ELOGWARNING,true);
	flags.set(1 << ELOGERROR,true);
	flags.set(1 << ELOGINFO,true);

	getLogItems().insert(std::make_pair(item,flags));


}

void xLogger::setLoggingLevel(int item,xBitSet flags)
{

	std::map<int,xBitSet>::iterator it = getLogItems().find(item);

	if ( it !=getLogItems().end() )
	{
        xBitSet& lflags = it->second;
		lflags = flags;
        
	}

}

signed int dVsprintf(char *buffer, unsigned int bufferSize, const char *format, void *arglist)
{
#ifdef TNL_COMPILER_VISUALC
	signed int len = _vsnprintf(buffer, bufferSize, format, (va_list) arglist);
#else
	signed int len = vsnprintf(buffer, bufferSize, format, (char *) arglist);
#endif
	return len;
}

void xLogger::xLog(char *cppText,int type,int component,const char *header, ...)
{


	GetInstance()->lastTyp = type;
	GetInstance()->lastComponent = component;

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

	char headerout[4096];
	if (strlen(cppText))
	{		
		sprintf(headerout,"%s::%s\n",cppText,header);
		xLog(type,component,headerout,buffer);
	}else
	{
		xLog(type,component,header,buffer);
	}

}


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

void xLogger::xLog(int type,int component,const char *header, ...)
{

	GetInstance()->lastTyp = type;
	GetInstance()->lastComponent = component;

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
			sprintf(verbosityStr,"->	INFO : %s ",componentString);
			SetConsoleTextAttribute(hConsole, 10);
		}
		break;
	case ELOGTRACE:
		{
			sprintf(verbosityStr,"->TRACE: %s",componentString);
			SetConsoleTextAttribute(hConsole, 11);
		}
		break;
	case ELOGERROR:
		{
			sprintf(verbosityStr,"->ERROR:%s",componentString);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
		break;
	case ELOGWARNING:
		{
			sprintf(verbosityStr,"->WARNING : %s",componentString);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);
		}
		break;
	}
	sprintf(headerBuffer,"%s",verbosityStr);
	//GetInstance()->finalPrint(headerBuffer);


	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);

	char buffer2[4096];
	sprintf(buffer2,":%s :%s",headerBuffer,buffer);
	GetInstance()->finalPrint(buffer2);


	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);

	return;

}

void xLogger::xLogExtro(int style,const char *header, ...)
{


	char buffer[4096];
	unsigned int bufferStart = 0;

	va_list s;
	va_start( s, header );
		dVsprintf(buffer + bufferStart, sizeof(buffer) - bufferStart, header, s);
	va_end(s);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	char headerBuffer[4096];

	sprintf(headerBuffer,"%s\n",header);
	GetInstance()->finalPrint(buffer);
	return;
}

void xLogger::xLog(int verbosity,const char *header,const char*msg,...)
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
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
						FOREGROUND_INTENSITY | FOREGROUND_RED);
				}
				break;
		}

		sprintf(headerBuffer,"%s %s-\n--",verbosityStr,header);
		printf(headerBuffer);
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN);

		printf("--->%s\n ",buffer);
	
		
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);



		return;

		va_list va;
		va_start (va,msg);
		char bufferN[1024] = { 0 } ; 
		vsprintf(bufferN,msg,va );
		va_end (va);
		printMessage(bufferN);
	

}

xBitSet& xLogger::getLogFlags()
{
	return GetInstance()->mLogFlags ;
}


xBitSet& xLogger::getVerbosityFlags()
{
	return GetInstance()->m_VerbosityFlags;
}

bool xLogger::isLogging(int logItem)
{ 
	return GetInstance()->getVerbosityFlags().test(1<<logItem); 
}

void xLogger::enableLogItem(int logItem,bool enbabled)
{ 
	return GetInstance()->getVerbosityFlags().set(1<< logItem,enbabled); 
}

void xLogger::Init()
{
	#ifdef CONSOLE_OUTPUT
		//consoleStream->Open();
		
	#endif 
}

xLogger::xLogger()
{
	xlog = this;
	m_LogOutChannels = ELOGGER_NONE;

	lastTyp = 0;
	lastComponent = 0;

#ifdef CONSOLE_OUTPUT
	//m_LogOutChannels |= ELOGGER_CONSOLE;
	//consoleStream = new ConStream();
#endif 
	
	#ifdef VIRTOOLS_USER_SDK
		mContext = NULL;
	#endif
}
xLogger*xLogger::GetInstance()
{
	if (xlog ==NULL)	{
		xlog = new xLogger();	}
	return xlog;
}

void printMessage(const char*buffer)
{
	using namespace xUtils;

#ifdef CONSOLE_OUTPUT

	if (  consoleStream )
	{
		Consoleln(buffer);
		fflush (stderr);
		fflush (stdout);
		strcpy(consoleStream->buf,buffer);
	}
#endif

}


xLogger::~xLogger()
{

}
