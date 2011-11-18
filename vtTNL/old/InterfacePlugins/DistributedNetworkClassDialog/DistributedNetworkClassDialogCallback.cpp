#include "stdafx.h"
#include "DistributedNetworkClassDialogCallback.h"


#include "DistributedNetworkClassDialogMenu.h"

//static plugin interface that allow direct communication with Virtools Dev
PluginInterface*		s_Plugininterface	= NULL;


//main plugin callback for Virtools Dev
void PluginCallback(PluginInfo::CALLBACK_REASON reason,PluginInterface* plugininterface)
{
	switch(reason)
	{
	case PluginInfo::CR_LOAD:
		{
			s_Plugininterface	= plugininterface;
			InitMenu();
			UpdateMenu();
		}break;
	case PluginInfo::CR_UNLOAD:
		{
			RemoveMenu();
			s_Plugininterface	= NULL;
		}break;
	case PluginInfo::CR_NEWCOMPOSITIONNAME:
		{
		}break;
	case PluginInfo::CR_NOTIFICATION:
		{
		}break;
	}
}
