#ifndef __MEMORY_FILE_MAPPING_TYPES_H__
#define __MEMORY_FILE_MAPPING_TYPES_H__

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

#endif