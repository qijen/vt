#include "XLoader.h"
#include "InitGuid.h"
#include "CKAll.h"

#include "pCommon.h"


#define X_PLUGIN_VERSION		0x0000001
#define X_READER_GUID			CKGUID(0x499d11a7,0x24aa03b6)



#ifdef CK_LIB
#define RegisterBehSDatorDeclarations	Register_SDatReader_BehaviorDeclarations
#define InitInstance					_SDatReader_InitInstance
#define ExitInstance					_SDatReader_ExitInstance
#define CKGetPluginInfoCount			CKGet_SDatReader_PluginInfoCount
#define CKGetPluginInfo					CKGet_SDatReader_PluginInfo
#define g_PluginInfo					g_SDatReader_PluginInfo
#define	CKGetReader						CKGet_SDatReader_Reader
#else
#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
#define InitInstance					InitInstance
#define ExitInstance					ExitInstance
#define CKGetPluginInfoCount			CKGetPluginInfoCount
#define CKGetPluginInfo					CKGetPluginInfo
#define g_PluginInfo						g_PluginInfo
#define	CKGetReader						CKGetReader
#endif


CKPluginInfo g_PluginInfo;

/**********************************************
Called by the engine when a file with the AVI 
extension is being loaded, a reader has to be 
created.
***********************************************/
CKDataReader *CKGetReader(int pos)
{
	return new CKXReader();
}


CKPluginInfo* CKGetPluginInfo(int index)
{
	// Not Valid under Win NT 4
	if (VxGetOs() == VXOS_WINNT4) return 0;
	g_PluginInfo.m_GUID=X_READER_GUID;
	g_PluginInfo.m_Extension="dae";
	g_PluginInfo.m_Description="NxStream Parser";
	g_PluginInfo.m_Author="Guenter Baumgart";
	g_PluginInfo.m_Summary="Loads xml files, created by Maya/Max Exporters";
	g_PluginInfo.m_Version=X_PLUGIN_VERSION;
	g_PluginInfo.m_InitInstanceFct=NULL;	// 
	g_PluginInfo.m_Type=CKPLUGIN_MODEL_READER;	// Plugin Type
	return &g_PluginInfo; 
}




CKPluginInfo g_PluginInfos;
int CKGetPluginInfoCount()
{
	return 1;
}


CKPluginInfo* CKXReader::GetReaderInfo() {
	return &g_PluginInfo;
}
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//------- Returns the name of a LPDIRECTXFILEOBJECT 
XString GetFileObjectName(LPDIRECTXFILEOBJECT obj)
{
	if (!obj) return XString("");

	
	DWORD NameSize = 0;

	if (FAILED(obj->GetName(NULL,&NameSize)))
		return XString("");
	if (!NameSize) 
		return XString("");
	NameSize++;
	XString Temp(NameSize);
	if (FAILED(obj->GetName(Temp.Str(),&NameSize)))
		return XString("");
	return Temp;
}
//////////////////////////////////////////////////////////////////////////
CKERROR CKXReader::Load(CKContext* context,CKSTRING FileName,CKObjectArray *array,CKDWORD LoadFlags,CKCharacter *carac)
{
	if(!array) return CKERR_INVALIDPARAMETER;
	if(!FileName) return CKERR_INVALIDPARAMETER;




	HRESULT hr = S_OK;

	m_Context = context;
	m_LoadFlags = LoadFlags;
	m_FileName = FileName;

	
	XString filename(FileName);
	filename.Trim();
	int ok  = context->GetPathManager()->ResolveFileName(filename,DATA_PATH_IDX,-1);

	pSerializer *ser  = pSerializer::Instance();
	if (ser && ok==0 )
	{
		ser->parseFile(filename.CStr(),0);
		//ser->loadCollection(filename.CStr())
	}

	

	
	int t = 0;



	//LoadFromFileC(context,FileName,false,LoadFlags,array,"null");

	return CK_OK;
}

//////////////////////////////////////////////////////////////////////////
BOOL CKXReader::LoadFromFileC(CKContext *ctx, XString filename, CKBOOL hidden, CKDWORD loadflags, CKObjectArray* targetArray, XString password)
{
	CKBOOL result = false;

	return result;
}
