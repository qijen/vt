// OggReaderDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#define READER_COUNT  1
CKPluginInfo g_piInfo[READER_COUNT];

#define READER_VERSION 0x00000001
#define OGGREADER_GUID CKGUID(0x28391930,0x225f26df)


CKPluginInfo* OGGReader::GetReaderInfo()
{
	return &g_piInfo[0];
}

#ifdef CK_LIB
	CKDataReader *CKGet_OggReader_Reader(int pos)
#else
	CKDataReader* CKGetReader(int pos)
#endif
{
	return new OGGReader;
}
 


#ifdef CK_LIB
	CKPluginInfo* CKGet_OggReader_PluginInfo(int index)
#else
	CKPluginInfo* CKGetPluginInfo(int index)
#endif
{
		g_piInfo[0].m_GUID		    = OGGREADER_GUID;
		g_piInfo[0].m_Version		= READER_VERSION;
		g_piInfo[0].m_Description	= "Ogg Sound Files";
		g_piInfo[0].m_Summary		= "Ogg reader";
		g_piInfo[0].m_Extension		= "ogg";
		g_piInfo[0].m_Author		= "Flashbang Studios, LLC";
		//g_piInfo[0].m_InitInstanceFct = InitInstance;
		g_piInfo[0].m_Type			= CKPLUGIN_SOUND_READER;	// Plugin Type

		return &g_piInfo[0];
}

