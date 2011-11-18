#include "pch.h"
#include "GProfile.h"
#include "GException.h"
#include "DirectoryListing.h"
#include <string.h> //for: memcpy()

#ifndef _WIN32
	#include <errno.h>
#endif


#ifdef _WIN32
	#include <Windows.h>
#endif

static GProfile *g_AlternateProfile = 0;
GProfile *SetProfile(GProfile *pApplicationProfile)
{
	GProfile *pReturn = g_AlternateProfile;
	g_AlternateProfile = pApplicationProfile;
	return pReturn;
}

GProfile &GetProfile()
{
	static GProfile *s_pDefaultProfile = 0;
	if (g_AlternateProfile)
		return *g_AlternateProfile;
	if (!s_pDefaultProfile)
		s_pDefaultProfile = new GProfile ("txml.txt","XML_CONFIG_FILE");
	return *s_pDefaultProfile;
}


GProfile::GProfile(const char *pzFileName, const char *pzEnvOverride )
	: m_bCached(0)
{
	m_strEnvOverride = pzEnvOverride;
	m_strBaseFileName = pzFileName; // no path

	GString strConfigFileDefault;
#ifdef _WIN32					// like "txml.txt"
	strConfigFileDefault.Format("c:\\%s",pzFileName);
#else
	strConfigFileDefault.Format("/%s",pzFileName);
#endif
	// env var like "XML_CONFIG_FILE"
	if (getenv(pzEnvOverride))
		// Use the system environment setting
		m_strFile = getenv(pzEnvOverride);
	else
		// Use the default
		m_strFile = strConfigFileDefault;
}


//
// load the profile configuration file yourself, 
// and create this object "with no disk config file"
GProfile::GProfile(const char *szBuffer, long dwSize)
{
	ProfileParse(szBuffer, dwSize);
}


// create a profile from the supplied file name
GProfile::GProfile(const char *pzFilePathAndName )
	: m_bCached(0)
{
	m_strEnvOverride = "NONE";
	m_strBaseFileName = CDirectoryListing::LastLeaf(pzFilePathAndName); //"txml.txt";

	if (pzFilePathAndName && pzFilePathAndName[0])
		// use the supplied pzFilePathAndName
		m_strFile = pzFilePathAndName;

}


GProfile::~GProfile()
{
	Destroy();
}

void GProfile::Destroy()
{
	GListIterator itSections(&m_lstSections);
	while (itSections())
	{
		Section *pSection = (Section *)itSections++;

		GListIterator itNVP(&pSection->m_lstNVP);
		while (itNVP())
		{
			NameValuePair *pNVP = (NameValuePair *)itNVP++;
			delete pNVP;
		}

		delete pSection;
	}

	m_lstSections.RemoveAll();
}

#ifdef _WIN32
void GProfile::ThrowLastError(const char *pzFile)
{
	LPVOID lpMsgBuf;

	DWORD dwExp = GetLastError();
	FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					dwExp,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
				);

	GString strTemp = (char *)lpMsgBuf;
	strTemp.TrimRightWS();

	GString strFile;
	strFile.Format("%s - %s", (const char *)strTemp,pzFile);
	GenericException exp(dwExp,(const char *)strFile);
	LocalFree( lpMsgBuf );
	throw exp;
}
#endif

void GProfile::GetLine(GString &strLine, 
					   const char *szBuffer, 
					   int *nIdx, 
					   int dwSize)
{
	strLine.Empty();

	while (*nIdx < dwSize)
	{
		if (szBuffer[*nIdx] == '\n')
			break;

		strLine += szBuffer[*nIdx];
		(*nIdx)++;
	}

	(*nIdx)++;
}

void GProfile::ProfileParse(const char *szBuffer, long dwSize)
{
	Section *pSection = 0;

	// parse the file
	int nIdx = 0;
	while (nIdx < dwSize)
	{
		GString strLine;
		GetLine(strLine, szBuffer, &nIdx, dwSize);


		strLine.TrimRightWS();
		strLine.TrimLeftWS();
		if ((strLine.IsEmpty()) || (strLine[0] == ';'))
			continue;


		strLine.Replace("\\n", '\n');

		if (strLine[0] == '[')
		{
			int nIdx = strLine.Find(']');
			if (nIdx == -1)
				nIdx = strLine.Length();

			// new section
			pSection = new Section;
			pSection->m_strName = strLine.Mid(1, nIdx - 1);
			pSection->m_strName.TrimLeftWS();
			pSection->m_strName.TrimRightWS();

			m_lstSections.AddLast(pSection);
		}
		else if (pSection)
		{
			int nIdx = strLine.Find('=');
			if (nIdx == -1)
				continue;
			NameValuePair *pNVP = new NameValuePair;
			pSection->m_lstNVP.AddLast(pNVP);
			pNVP->m_strName = strLine.Left(nIdx);
			pNVP->m_strName.TrimLeftWS();
			pNVP->m_strName.TrimRightWS();
			
			pNVP->m_strValue = strLine.Mid(nIdx + 1);
			pNVP->m_strValue.TrimLeftWS();
			pNVP->m_strValue.TrimRightWS();
		}
	}
	m_bCached = true;
}

void GProfile::Load()
{
	if (m_bCached)
		return;

	// destroy the cached objects
	Destroy();

	char *szBuffer = 0;
	long dwSize = 0;



#ifdef _WIN32
	
		// open the file
		HANDLE hFile = CreateFile((const char *)m_strFile, 
								  GENERIC_READ, 0, 0, OPEN_EXISTING,
								  FILE_ATTRIBUTE_NORMAL, 0);

		dwSize = GetFileSize(hFile, NULL); 
 
		szBuffer = new char[dwSize + 1];

		// read the file
		long dwRead;
		if (!ReadFile(hFile, szBuffer, dwSize, (DWORD *)&dwRead, 0))
		{
			delete [] szBuffer;
			CloseHandle(hFile);
		}

		// close the file
		CloseHandle(hFile);


#else
	// open the file
	GString strTemp;
	int nResult = strTemp.FromFile((const char *)m_strFile, 0);

	szBuffer = new char[strTemp.Length() + 1];
	memcpy(szBuffer,(const char *)strTemp, strTemp.Length());
	dwSize = strTemp.Length();

#endif


	// terminate the buffer
	//szBuffer[dwSize] = 0;

	ProfileParse(szBuffer, dwSize);

	delete [] szBuffer;

}

GProfile::Section *GProfile::FindSection(const char *szSection)
{
	Load();

	Section *pRet = 0;

	GListIterator itSections(&m_lstSections);
	while ( itSections() && (!pRet))
	{
		Section *pSection = (Section *)itSections++;

		if (pSection->m_strName.CompareNoCase(szSection) == 0)
			pRet = pSection;
	}

	return pRet;
}

GProfile::NameValuePair *GProfile::FindKey(const char *szKey, GProfile::Section *pSection)
{
	NameValuePair *pRet = 0;
	
	if (pSection)
	{
		GListIterator itNVP(&pSection->m_lstNVP);
		while ((itNVP()) && (!pRet))
		{
			NameValuePair *pNVP = (NameValuePair *)itNVP++;
			if (pNVP->m_strName.CompareNoCase(szKey) == 0)
				pRet = pNVP;
		}
	}

	return pRet;
}

// function retrieves the names of all sections
void GProfile::GetSectionNames(GStringList *lpList)
{
	Load();

	if (lpList)
	{
		GListIterator itSections(&m_lstSections);
		while (itSections())
		{
			Section *pSection = (Section *)itSections++;
			lpList->AddLast(pSection->m_strName);
		}
	}
}

const GList *GProfile::GetSection(const char *szSectionName)
{
	Section *pSection = FindSection(szSectionName);

	if (pSection)
		return &pSection->m_lstNVP;
	
	return 0;
}


// written by sean, not sure what this is about...
static bool IsTrue(char chValue)
{
	bool bRet;  
	
	if (chValue > '1')
		bRet = ((chValue & 0x10) != 0);
	else
		bRet = ((chValue & 0x01) != 0);
		
	return bRet;
}

// function retrieves a boolean from the specified section
short GProfile::GetBool(const char *szSectionName, const char *szKey, short bThrowNotFound /* = true */)
{
	Section *pSection = FindSection(szSectionName);
	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (pNVP)
		{
			char chTest = '0';
			if (!pNVP->m_strValue.IsEmpty())
				chTest = pNVP->m_strValue[0];
			return IsTrue(chTest);
		}
		else if (bThrowNotFound)
		{
			// throw key not found

		}
	}
	else if (bThrowNotFound)
	{

	}

	return 0;
}

// function retrieves a boolean from the specified section
short GProfile::GetBoolean(const char *szSectionName, const char *szKey, short bThrowNotFound /* = true */)
{
	Section *pSection = FindSection(szSectionName);
	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (pNVP)
		{
			if (pNVP->m_strValue.CompareNoCase("true") == 0 ||
				pNVP->m_strValue.CompareNoCase("on") == 0  ||
				pNVP->m_strValue.CompareNoCase("yes") == 0 ||
				pNVP->m_strValue.CompareNoCase("1") == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (bThrowNotFound)
		{

		}
	}
	else if (bThrowNotFound)
	{
		// throw key not found

	}

	return 0;
}


// function retrieves a long from the specified section
long GProfile::GetLong(const char *szSectionName, const char *szKey, short bThrowNotFound /* = true */)
{
	Section *pSection = FindSection(szSectionName);
	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (pNVP)
		{
			if (!pNVP->m_strValue.IsEmpty())
				return atol((const char *)pNVP->m_strValue);
		}
		else if (bThrowNotFound)
		{
			// throw key not found

		}
	}
	else if (bThrowNotFound)
	{
		// throw key not found

	}

	return 0;
}

const char *GProfile::GetPath(const char *szSectionName, const char *szKey, short bThrowNotFound)
{
	Section *pSection = FindSection(szSectionName);
	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (pNVP)
		{
			if ( !( pNVP->m_strValue.Right(1) == "/" || pNVP->m_strValue.Right(1) == "\\") )
			{
		#ifdef _WIN32
				pNVP->m_strValue += "\\";
		#else
				pNVP->m_strValue += "/";
		#endif
			}
			return pNVP->m_strValue;
		}
		else if (bThrowNotFound)
		{
			// throw key not found

		}
	}
	else if (bThrowNotFound)
	{
		// throw key not found

	}

	return 0;
}

// function retrieves a string from the specified section
const char *GProfile::GetString(const char *szSectionName, const char *szKey, short bThrowNotFound /* = true */)
{
	Section *pSection = FindSection(szSectionName);
	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (pNVP)
		{
			return (const char *)pNVP->m_strValue;
		}
		else if (bThrowNotFound)
		{
			// throw key not found

		}
	}
	else if (bThrowNotFound)
	{
		// throw key not found

	}

	return 0;
}

void GProfile::SetString(const char *szSectionName, const char *szKey, const char *szValue)
{
	m_bCached = true;

	Section *pSection = FindSection(szSectionName);
	if (!pSection)
	{
		pSection = new Section;
		pSection->m_strName = szSectionName;
		pSection->m_strName.TrimLeftWS();
		pSection->m_strName.TrimRightWS();

		m_lstSections.AddLast(pSection);
	}

	if (pSection)
	{
		NameValuePair *pNVP = FindKey(szKey, pSection);
		if (!pNVP)
		{
			pNVP = new NameValuePair;
			pSection->m_lstNVP.AddLast(pNVP);
			pNVP->m_strName = szKey;
			pNVP->m_strName.TrimLeftWS();
			pNVP->m_strName.TrimRightWS();
		}
			
		if (pNVP)
		{
			pNVP->m_strValue = szValue;
			pNVP->m_strValue.TrimLeftWS();
			pNVP->m_strValue.TrimRightWS();
		}
	}
}
