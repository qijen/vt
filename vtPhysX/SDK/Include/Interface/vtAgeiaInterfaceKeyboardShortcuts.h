#pragma once

//-----------------------------------------------------------------------------
//CATEGORY FLAGS---------------------------------------------------------------
#define FIXED	KeyboardShortcutManager::KSCategory::FIXED
#define GLOBAL	KeyboardShortcutManager::KSCategory::GLOBAL
#define ACTIVE	KeyboardShortcutManager::KSCategory::ACTIVE
#define HIDDEN	KeyboardShortcutManager::KSCategory::HIDDEN

//Notes :
//
//for Global shortcuts, you can check for them in WindowProc
//  on WM_KEYDOWN
//  VirtoolsExternalPlugin::KeyboardShortcutManager* ksm = GetInterface()->GetKeyboardShortcutManager();
//  int commandID	= ksm->TestKS(STR_CATEGORY,pMsg->wParam);
//
//for Local shortcuts you can check for them in PreTranslateMessage
//  on WM_SYSKEYDOWN (for alt+key or F10) or WM_KEYDOWN
//  VirtoolsExternalPlugin::KeyboardShortcutManager* ksm = GetInterface()->GetKeyboardShortcutManager();
//  int commandID	= ksm->TestKS(STR_CATEGORY,pMsg->wParam);

//category name under which you want to have your keyboard shortcuts
#define STR_CATEGORY	"vtAgeiaInterface Keyboard Shortcuts Category"

//command ids sample - command ID min must be 1
#define CID_A		1
#define CID_B		2

//names of these commands, sample
#define STR_A		"Command A"
#define STR_B		"Command B"
//command ids sample end


//IMPORTANT!
//by default these functions are called in vtAgeiaInterfacecallback.cpp
//if you do not use callback (do not have vtAgeiaInterfacecallback.cpp), you should call these functions manually

//register keyboard shortcut category (shortcuts of this category will be saved when Virtools Dev is closed)
int RegisterKeyboardShortcutCategory();
//unregister keyboard shortcut category (shortcuts of this category will be saved no more when Virtools Dev is closed)
int	UnregisterKeyboardShortcutCategory();
//register keyboard shortcuts for your editor. Fill this function
int	RegisterKeyboardShortcuts();

//function that returns command name from its command id
//used in RegisterActionEditorKSCategory, as a callback for the keyboard shortcut category
//to know the command name
//Note : in ksm->RegisterCategory,
//  you can set the callback to NULL (change "GetCommandName" into "NULL"
//  but you should then put directly names into keyboard shortcuts you register in that category
//  see in RegisterActionEditorKS();
//    ks.key			= 'A';
//    ks.flags		= ks.KS_CONTROL|ks.KS_SHIFT|ks.KS_ALT;
//    ks.commandID	= CID_A;
//    ks.name		= "Command A";		//LINE TO ADD TO SET NAME DIRECTLY
//    ksm->RegisterKS(index,ks);
//  but in that case, it will be less easy to integrate into popup menus
const char*	GetCommandName(int commandID);

//function that returns command name <tab> <current keyboard shortcut>
//usefull for popup menus
const char*	GetCommandMenuName(int commandID,XString &name);

/*
//here the sample code to fill a menu
#define MENUSTR(commandID) GetActionEditorMenuName(commandID,name)

	POINT p;
	GetCursorPos(&p);
	XString name;
	CKVIPopupMenu menu;
	menu.AddItem(MENUSTR(CID_A),CID_A);
	menu.AddItem(MENUSTR(CID_B),CID_B);
	m_ActionMenu.AddSeparator();
	menu.Show(p.x,p.y);
*/