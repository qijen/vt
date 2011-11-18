#include "CKAll.h"
#include "VSLManagerSDK.h"


enum vtEventState
{
	EEVT_STARTED,
	EEVT_FINISHED
};

struct vtExternalEvent
{
	unsigned long timeOfCreation;
	
	char command[MAX_PATH];
	char commandArg[MAX_PATH];
	vtEventState state;
};

struct TSharedMemory {

	vtExternalEvent event;
};

struct haptekMsg
{
	XString command;
	int k;

};