#include "stdafx2.h"
#include "vtAgeiaInterfaceeditor.h"
#include "vtAgeiaInterfaceMenu.h"

//declaration of menu callback (for InitMenu function)
void PluginMenuCallback(int commandID);

//static main menu
CMenu*							s_MainMenu			= NULL;

#define PLUGINMENU_MAXENTRIES	20	//put there the max entries your menu may use





//adds the menu to Virtools Dev main menu
void InitMenu()
{
	if (!s_Plugininterface)
		return;

	s_MainMenu		= s_Plugininterface->AddPluginMenu(STR_MAINMENUNAME,PLUGINMENU_MAXENTRIES,NULL,(VoidFunc1Param)PluginMenuCallback);
}

//removes the menu from Virtools Dev main menu
void RemoveMenu()
{
	if (!s_Plugininterface || !s_MainMenu)
		return;

	s_Plugininterface->RemovePluginMenu(s_MainMenu);
}

//up to user.
//(May be called on new composition notification for instance)
//Note that first commandID can be 0
//but last command ID must be lesser thanPLUGINMENU_MAXENTRIES
void UpdateMenu()
{
	s_Plugininterface->ClearPluginMenu(s_MainMenu);		//clear menu

	s_Plugininterface->AddPluginMenuItem(s_MainMenu,0,"item0");		//add simple item sample
	s_Plugininterface->AddPluginMenuItem(s_MainMenu,1,"item1");		//add simple item sample
	s_Plugininterface->AddPluginMenuItem(s_MainMenu,-1,NULL,TRUE);	//add separator sample
	CMenu* sub0 = s_Plugininterface->AddPluginMenuItem(s_MainMenu,2,"SubMenu0",FALSE,TRUE);	//sub menu sample
	CMenu* sub1 = s_Plugininterface->AddPluginMenuItem(s_MainMenu,3,"SubMenu1",FALSE,TRUE);	//sub menu sample
	s_Plugininterface->AddPluginMenuItem(sub0,4,"item2");		//add simple item to sub menu sample
	s_Plugininterface->AddPluginMenuItem(sub0,5,"item3");		//add simple item to sub menu sample
	s_Plugininterface->AddPluginMenuItem(sub1,6,"item4");		//add simple item to sub menu sample
	s_Plugininterface->AddPluginMenuItem(sub1,7,"item5");		//add simple item to sub menu sample

	s_Plugininterface->UpdatePluginMenu(s_MainMenu);	//update menu,always needed when you finished to update the menu
														//unless you want the menu not to have Virtools Dev main menu color scheme.
}

//fill with your command IDs and your actions
void PluginMenuCallback(int commandID)
{
	/*switch(commandID)
	{
	}*/
}
