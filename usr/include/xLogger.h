///
////////////////////////////////////////////////////////
//  xLogger.h
//  Implementation of the Class xLogger
//  Created on:      10-Feb-2007 12:40:39
///////////////////////////////////////////////////////////

#if !defined __X_LOGGER_H__
#define __X_LOGGER_H__

#include <pch.h>
//#include <LogTools.h>
#include <xBitSet.h>
//#include <base.h>
#include <TypeInfo.h>

#include <BaseMacros.h>

#include <stdlib.h>
#include <map>
#include <vector>




#ifndef DEBUG_CALLER
	#define DEBUG_CALLER
#endif

#ifndef TNL_COMPILER_VISUALC
	#define TNL_COMPILER_VISUALC
#endif

#ifndef DEBUG_CALLERS_FUNC
	#define DEBUG_CALLERS_FUNC
#endif



#ifndef this
	#define XL_PREFIX __FUNCTION__
#else
	#define  XL_PREFIX typeid(this).name()
#endif

#ifndef this
	#define  XL_OBJECT_PREFIX(T) "FN:"##T
#else
	#define  XL_OBJECT_PREFIX(T) "Call in Class:"##T
#endif


#define XL_START XL_OBJECT_PREFIX(XL_PREFIX)


#define XLOG_PREFIX0  


#ifdef VIRTOOLS_USER_SDK
	class CKContext;
#endif


namespace xUtils
{
  

	typedef enum xLoggerFlags { 
		ELOGGER_NONE=0,
		ELOGGER_CONSOLE=2,
		ELOGGER_MCORE=4,
		ELOGGER_CEGUI=8,
		ELOGGER_OUT_OBJECTS=16
	}xLoggerFlags;

	enum ELOGTYPE
	{
		ELOGDEBUG,
		ELOGTRACE,
		ELOGERROR,
		ELOGWARNING,
		ELOGINFO,
		ELOGALL,
	};

	typedef enum E_PRINT_STYLE_FLAGS
	{

		E_PSF_PRINT_LOG_TYPE=1,
		E_PSF_PRINT_COMPONENT,
		E_PSF_PRINT_NEWLINE,
		E_PSF_PRINT_TAB,

	};

	static char* sLogTypes[]=
	{
		"DEBUG",
		"TRACE",
		"ERROR",
			"WARNING",
			"INFO",
	};

	/// xLogConsumer is the base class for the message logging system in TNL.
	///
	/// TNL by default doesn't log messages anywhere, but users of the library
	/// can instantiate subclasses that override the logString method.
	/// Any instantiated subclass of xLogConsumer will receive all general
	/// logprintf's, as well as any log messages that are enabled via
	/// the TNLLogEnable macro.
	class xLogConsumer
	{
		xLogConsumer *mNextConsumer; ///< Next xLogConsumer in the global linked list of log consumers.
		xLogConsumer *mPrevConsumer; ///< Previous xLogConsumer in the global linked list of log consumers.

		static xLogConsumer *mLinkedList;       ///< Head of the global linked list of log consumers.

	public:
		/// Constructor adds this xLogConsumer to the global linked list.
		xLogConsumer();

		/// Destructor removes this xLogConsumer from the global linked list, and updates the log flags.
		virtual ~xLogConsumer();

		/// Returns the head of the linked list of all log consumers.
		static xLogConsumer *getLinkedList() { return mLinkedList; }

		/// Returns the next xLogConsumer in the linked list.
		xLogConsumer *getNext() { return mNextConsumer; }

		/// Writes a string to this instance of xLogConsumer.
		///
		/// By default the string is sent to the Platform::outputDebugString function. Subclasses
		/// might log to a file, a remote service, or even a message box.
		virtual void logString(const char *string);
	};

	class MODULE_API xLogger
	{
	
		public:

			
			

			xLogger();
			virtual ~xLogger();
			void Init();
			
			
			
			static xBitSet& getVerbosityFlags();
			void setVerbosityFlags(xBitSet val) { m_VerbosityFlags = val; }

			static xBitSet& getLogFlags();
			void setLogFlags(xBitSet val) { mLogFlags = val; }
			static bool isLogging(int logItem);
			static void enableLogItem(int logItem,bool enbabled=true);

			static void xLog(int verbosity,const char *header,const char*msg,...);
			static void xLog(int type,int component,const char *header, ...);
			static void xLog(xBitSet styleFlags,int type,int component,const char *header, ...);
			static void xLog(char *cppText,int type,int component,const char *header, ...);
			static void xLogExtro(int style,const char *header, ...);
			

			static xLogger*GetInstance();

			void DoProcessing(float eTime/* =0::0f */);

			
			typedef std::map<int,xBitSet>xLogItems;
			typedef std::vector<const char*>itemDescriptionArray;
			
			itemDescriptionArray mItemsDescriptions;
			itemDescriptionArray& getItemDescriptions(){return xLogger::GetInstance()->mItemsDescriptions;}
			
			inline void addItemDescription(const char*descr){ mItemsDescriptions.push_back(descr);  }

			const char*getItemDescription();


			xLogItems mLogItems;
			xUtils::xLogger::xLogItems& getLogItems()  { return mLogItems; }
			void setLogItems(xUtils::xLogger::xLogItems val) { mLogItems = val; }

			void addLogItem(int item);
			void setLoggingLevel(int item,xBitSet flags);
            xBitSet getLogLevel(int item);
			void enableLoggingLevel(int item,int level,int enable);

			void finalPrint(const char*string);

			#ifdef VIRTOOLS_USER_SDK
				CKContext * getVirtoolsContext() const { return mContext; }
				void setVirtoolsContext(CKContext * val) { mContext = val; }
			#endif

			void enableConsoleOutput(bool enable);

			protected :

			#ifdef VIRTOOLS_USER_SDK
				CKContext *mContext;
			#endif

				xBitSet mLogFlags;
				xBitSet m_VerbosityFlags;
				int m_Verbosity;
				int m_LogOutChannels;


	public :
				int lastTyp;
				int lastComponent;
				

		};
}


using namespace xUtils;


#endif // !defined(EA_3DFA15A6_382B_4624_9E00_4531365CB9AF__INCLUDED_)

