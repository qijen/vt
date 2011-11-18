 #include "CKAll.h"
#include "InitMan.h"
#include "VSLManagerSDK.h"


InitMan::~InitMan(){}



CKERROR 
InitMan::PreProcess(){	
	PerformMessages();	return CK_OK; 
}


CKERROR
InitMan::PostClearAll(){
	return CK_OK;
}
//////////////////////////////////////////////////////////////////////////

CKERROR InitMan::OnCKEnd(){

	return CK_OK;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR InitMan::OnCKReset(){
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CKERROR InitMan::OnCKPlay()
{
	
	return CK_OK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKERROR InitMan::OnCKInit(){
	
	m_Context->ActivateManager((CKBaseManager*) this,true);	
	return CK_OK;
}

#include "InitMan.h"
#include "ResourceTools.h"



bool
InitMan::AddFileEntry(XString ArchiveName,XString FileEntry){

	if (strlen(FileEntry.Str()) == 0)return FALSE;
	ZiJoIt it = zili.find(ArchiveName);
	if (  it == zili.end())
		it = zili.insert(zili.end(),std::make_pair(ArchiveName,new XFileList()));
	it->second->push_back(FileEntry);
	return TRUE;
}


int 
InitMan::GetUnzipDllVersion(){
	return m_GetUnzipDllVersion();
}

int 
InitMan::GetZipDllVersion(){
	return m_GetZipDllVersion();
}



BOOL __stdcall DefaultZipCallback(CZipCallbackData *pData){
	return FALSE;

}

void 
InitMan::SetDefaultZipValues(CZipParams * pParams){
	
	pParams->m_hwndHandle			= NULL;
	pParams->m_pCaller				= NULL;
	pParams->m_liVersion			= GetZipDllVersion();
	pParams->m_pfCallbackFunction	= DefaultZipCallback;
	pParams->m_bTraceEnabled		= FALSE;

	pParams->m_pszZipPassword		= NULL;
	pParams->m_bSuffix				= FALSE;
	pParams->m_bEncrypt				= FALSE;
	pParams->m_bSystem				= FALSE;
	pParams->m_bVolume				= FALSE;
	pParams->m_bExtra				= FALSE;
	pParams->m_bNoDirEntries		= FALSE;
	pParams->m_bDate				= FALSE;
	pParams->m_bVerboseEnabled		= FALSE;
	pParams->m_bQuiet				= FALSE;
	pParams->m_bLevel				= 9;
	pParams->m_bComprSpecial		= FALSE;
	pParams->m_bCRLF_LF				= FALSE;
	pParams->m_bJunkDir				= FALSE;
	pParams->m_bRecurse				= FALSE;
	pParams->m_bGrow				= TRUE;
	pParams->m_bForce				= FALSE;
	pParams->m_bMove				= FALSE;
	pParams->m_bDeleteEntries		= FALSE;
	pParams->m_bUpdate				= FALSE;
	pParams->m_bFreshen				= FALSE;
	pParams->m_bJunkSFX				= FALSE;
	pParams->m_bLatestTime			= FALSE;

	for (int j=0; j<8; j++)
		pParams->m_cDate[j] = 0;
	pParams->m_liFileCount			= 0;
	pParams->m_pszArchiveFileName	= NULL;
	pParams->m_liSeven				= 7;

	
}
void 
InitMan::SetDefaultUnZipValues(CUnzipParams * pParams)
{
	
	pParams->m_wndHandle          = NULL;
	pParams->m_pCaller            = NULL;
	pParams->m_liVersion          = GetUnzipDllVersion();
	pParams->m_pfCallbackFunction = DefaultZipCallback;
	pParams->m_bTraceEnabled = FALSE;

	pParams->m_bPromptToOverwrite = FALSE;
	pParams->m_pszZipPassword     = NULL;
	pParams->m_bTest              = FALSE;
	pParams->m_bComments          = FALSE;
	pParams->m_bConvert           = FALSE;

	pParams->m_bQuiet             = FALSE;
	pParams->m_bVerboseEnabled    = FALSE;
	pParams->m_bUpdate            = FALSE;
	pParams->m_bFreshen           = FALSE;
	pParams->m_bDirectories       = TRUE;
	pParams->m_bOverwrite         = TRUE;

	pParams->m_liFileCount        = 0;
	pParams->m_pszArchiveFileName = NULL;
	pParams->m_liSeven            = 7;

}


BOOL
InitMan::UnLoadZipDll(){
	
	FreeLibrary(m_ZipDllHandle);
	m_ZipDllHandle       = 0;
	m_ZipDllExec         = 0;
	m_GetZipDllVersion   = 0;
	if(DeleteFile(ZipDllTempFile))return TRUE;
	return FALSE;
}
BOOL
InitMan::UnLoadUnZipDll(){
	FreeLibrary(m_UnzipDllHandle);
	m_ZipDllHandle       = 0;
	m_ZipDllExec         = 0;
	m_GetZipDllVersion   = 0;
	if(DeleteFile(UnZipDllTempFile))return TRUE;
	return FALSE;
}

BOOL 
InitMan::LoadZipDll(){
	
	
	 
/*	
	HMODULE mod =GetParentModule(CKPLUGIN_MANAGER_DLL,INIT_MAN_GUID);
	if (mod)
		m_ZipDllHandle = GetModulefromResource(mod,ZIP_DLL,ZipDllTempFile);
		
*/
	m_ZipDllHandle = LoadLibrary("ZIP.DLL");

	if (!m_ZipDllHandle)return FALSE;
	
	m_GetZipDllVersion  = (CGetZipDllVersion)GetProcAddress(m_ZipDllHandle, "GetZipDllVersion");
	if (!m_GetZipDllVersion)return FALSE;
	
	m_ZipDllExec = (CZipDllExec)GetProcAddress(m_ZipDllHandle, "ZipDllExec");
	if (!m_ZipDllExec)return FALSE;
	return TRUE;
}

BOOL 
InitMan::LoadUnzipDll(){

	/*
	HMODULE mod =GetParentModule(CKPLUGIN_MANAGER_DLL,INIT_MAN_GUID);
	if (mod)m_UnzipDllHandle = GetModulefromResource(mod,UNZIP_DLL,UnZipDllTempFile);*/
	

	m_UnzipDllHandle = LoadLibrary("UNZIP.DLL");
	
	if (!m_UnzipDllHandle)return FALSE;
	

	m_GetUnzipDllVersion  = (CGetUnzipDllVersion)GetProcAddress(m_UnzipDllHandle, "GetUnzDllVersion");
	if (!m_GetUnzipDllVersion)return FALSE;

	m_UnzipDllExec = (CUnzipDllExec)GetProcAddress(m_UnzipDllHandle, "UnzDllExec");
	if (!m_UnzipDllExec)return FALSE;
	return TRUE;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
