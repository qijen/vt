#include "stdafx2.h"
#include "vtAgeiaInterfaceEditor.h"
#include "vtAgeiaInterfaceEditorDlg.h"
#include "vtAgeiaInterfaceKeyboardShortcuts.h"

void	GlobalKSCallback(int commandID)
{
  if (g_Editor)
    g_Editor->OnGlobalKeyboardShortcut(commandID);
}

int		RegisterKeyboardShortcutCategory()
{
	KeyboardShortcutManager* ksm = s_Plugininterface->GetKeyboardShortcutManager();
	if (ksm)
	{
		//remove the comments delimiters to register global keyboard shortcut category
		//that will be detected by the application whatever the focused window is.
		int index = ksm->RegisterCategory(STR_CATEGORY,NULL,ACTIVE/*+GLOBAL*/,NULL,NULL/*GlobalKSCallback*/,GetCommandName);
		if (index>=0)
			return 1;
	}
	return 0;
}

int		UnregisterKeyboardShortcutCategory()
{
	KeyboardShortcutManager* ksm = s_Plugininterface->GetKeyboardShortcutManager();
	if (ksm)
	{
		return ksm->UnregisterCategory(STR_CATEGORY);
	}
	return 0;
}

int		RegisterKeyboardShortcuts()
{
	KeyboardShortcutManager* ksm = s_Plugininterface->GetKeyboardShortcutManager();
	KeyboardShortcutManager::KS ks;

	int index		= ksm->GetCategoryIndex(STR_CATEGORY);

//sample code to register Ctrl+shift+alt+A with commandid_a and B with commandid_b
	ks.key			= 'A';
	ks.flags		= ks.KS_CONTROL|ks.KS_SHIFT|ks.KS_ALT;
	ks.commandID	= CID_A;
	ksm->RegisterKS(index,ks);

	ks.key			= 'B';
	ks.flags		= 0;
	ks.commandID	= CID_B;
	ksm->RegisterKS(index,ks);
//end sample code

	return 1;	
}

const char*	GetCommandName(int commandID)
{
//sample code
	switch(commandID)
	{
	case CID_A:
		return STR_A;
	case CID_B:
		return STR_B;
	}
//sample code
	return NULL;
}

const char*	GetCommandMenuName(int commandID,XString &name)
{
	name="";
	const char* cstr	= GetCommandName(commandID);
	if (!cstr)
		return NULL;

	name				= cstr;

	KeyboardShortcutManager* ksm = s_Plugininterface->GetKeyboardShortcutManager();
	if (ksm)
	{
		ksm->GetKSName(	STR_CATEGORY,
						commandID,
						name,
						FALSE/*do not clear string*/,
						TRUE/*add tab to string if string not empty*/);
	}

	return name.CStr();
}
