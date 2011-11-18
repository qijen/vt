#include "ResourceTools.h"

HINSTANCE GetModulefromResource(HMODULE hModule,int name,char *tempfile){

	BYTE *data;
	HANDLE hfile;
	DWORD len,c;
	char temppath[MAX_PATH];
	HRSRC hres;

	if (hres=FindResource((HMODULE)hModule,MAKEINTRESOURCE(name),RT_RCDATA)){
			len=SizeofResource(hModule,hres);
			hres=(HRSRC)LoadResource(hModule,hres);
			data=(BYTE*)LockResource((HRSRC)hres);


	}
	GetTempPath(MAX_PATH,temppath);
	GetTempFileName(temppath,"tostrong",0,tempfile);
	hfile=CreateFile(tempfile,GENERIC_WRITE|FILE_SHARE_DELETE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_TEMPORARY,NULL);
	WriteFile(hfile,data,len,&c,NULL);
	CloseHandle(hfile);
	delete data;
	return LoadLibrary(tempfile);
}

HMODULE	GetParentModule(CK_PLUGIN_TYPE type,CKGUID guid){
	
	CKPluginManager* ThePluginManager=CKGetPluginManager();
	for (int i=0;i<ThePluginManager->GetPluginDllCount();i++){
		CKPluginEntry* desc=ThePluginManager->GetPluginInfo(type,i); 
		CKPluginDll*   dll =ThePluginManager->GetPluginDllInfo(desc->m_PluginDllIndex); 
		if (desc->m_PluginInfo.m_GUID == guid)return ((HMODULE)dll->m_DllInstance);
	}
	return NULL;
}