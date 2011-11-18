#pragma once

//by default these functions are called in DistributedNetworkClassDialogcallback.cpp
//if you do not use callback (do not have DistributedNetworkClassDialogcallback.cpp), you should call these functions manually

//to add a menu in Virtools Dev main menu.
void InitMenu();
//to remove the menu from Virtools Dev main menu
void RemoveMenu();
//to fill menu with your own commands
void UpdateMenu();

#define STR_MAINMENUNAME	"DistributedNetworkClassDialog Menu"
