#ifndef __P_ERRROR_STREAM_H__
#define __P_ERRROR_STREAM_H__

#include "pTypes.h"
#include "NxUserOutputStream.h"

namespace vtAgeia
{

class pErrorStream : public NxUserOutputStream
{

public:
	pErrorStream(){}
	void reportError(NxErrorCode e, const char* message, const char* file, int line);
	NxAssertResponse reportAssertViolation(const char* message, const char* file, int line);
	void print(const char* message);
};

}

#endif