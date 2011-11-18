#ifndef _XNET_CONSTANTS_H_
#define _XNET_CONSTANTS_H_

//#include "xNetTypes.h"

static float DO_TIME_THRESHOLD  = 150.0f;

static char* sLogItems[]=
{
		"CLIENT",
		"SESSION",
		"3DOBJECT",
		"2DOBJECT",
		"Distributed Base Object",
		"Distributed Classes",
		"MESSAGES",
		"ARRAY_MESSAGES",
		"CONNECTION",
		"NET_INTERFACE",
		"GHOSTING",
		"STATISTICS",
		"BUILDINGBLOCKS",
		"VSL",
		"CPP",
		"ASSERTS",
		"PREDICTION",
		"SERVER_MESSAGES"
};

static char* sErrorStrings[]=
{
	"OK",
	"\t Intern :",
	"\t No connection :",
	"\t Not joined to a session:",
	"\t Session needs right password:",
	"\t Session is locked:",
	"\t Session already exists:",
	"\t Session is full:",
	"\t You must be session master:",
	"\t Invalid parameter :",
	"\t There is not such user :",
	"\t Distributed class already exists :",
	"\t Couldn't connect to any server :",
	"\t You already joined to a session :",
	"\t No such session :"
};

#endif
