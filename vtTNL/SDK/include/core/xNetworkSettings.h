#ifndef __XNETWORKSETTINGS_H
#define __XNETWORKSETTINGS_H

#include "precomp.h"

struct xNetworkSettings
{

	XString name;
	int port;
	bool isServer;
	TNL::Address bindAddress;
	TNL::Address pingAddr;

};

#endif
