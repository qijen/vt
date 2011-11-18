#ifndef __PLOGGER_H__
#define __PLOGGER_H__
#include "pTypes.h"

	class pLogger
	{

	public:
		pLogger();
		~pLogger();
		
		pErrorStream * getErrorStream() const { return mErrorStream; }
		void setErrorStream(pErrorStream * val) { mErrorStream = val; }

	
		protected:
			pErrorStream *mErrorStream;
	};


#endif