////////////////////////////////////////////////////////////////////////////////
//
// ARToolKitPlusLogger
// --------------------------------------
//
// Description:
// Little helper class which encapsulate the error logging
// (look into ARToolKitPlus for description)
//
// Version 1.0	: First Release
//
// Known Bugs	: Not really integrated into Virtools, just a dummy call
//
// Copyright Jörg Stöcklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////

#ifndef ARToolKitPlusLogger_H
#define ARToolKitPlusLogger_H "$Id:$"

#include <ARToolKitPlus/TrackerSingleMarkerImpl.h> 

class ARToolKitLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
	{
		printf(nStr);
	}
};

#endif