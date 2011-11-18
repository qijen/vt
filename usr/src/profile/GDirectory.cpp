// --------------------------------------------------------------------------
//					www.UnitedBusinessTechnologies.com
//			  Copyright (c) 1998 - 2002  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// 
// UBT welcomes any suggestions, improvements or new platform ports.
// email to: XMLFoundation@UnitedBusinessTechnologies.com
// --------------------------------------------------------------------------
#include "pch.h"
#include "DirectoryListing.h"
#include "GString.h"
#include "GException.h"
//#include <string.h>			

// #include <windows.h>

//	#include "Winbase.h"
#include <sys/stat.h>
#include  <io.h>
#include  <direct.h> // for: mkdir





const char *CDirectoryListing::LastLeaf(const char *pzFullPath, char chSlash/*= 0*/)
{
	static GString strReturnValue;
	strReturnValue = "";
	if (pzFullPath && pzFullPath[0])
	{
		strReturnValue = pzFullPath;
		int nLen = strlen(pzFullPath);
		if (chSlash)
		{
			if (pzFullPath[nLen - 1] == chSlash)
				nLen--;

		}
		else if ( pzFullPath[nLen - 1] == '\\' ||  pzFullPath[nLen - 1] == '/')
		{
			nLen--; // if the input value is "/etc/bin/" start searching behind the last '/'
					// so that the return leaf value is "bin"
		}

		for(int i = nLen-1; i > -1; i-- )
		{
			if (chSlash)
			{
				if (pzFullPath[i] == chSlash)
				{
					strReturnValue = &pzFullPath[i+1];
					break;
				}
			}
			else if (pzFullPath[i] == '\\' || pzFullPath[i] == '/')	
			{
				strReturnValue = &pzFullPath[i+1];
				break;
			}
		}
	}
	return strReturnValue;
}




void CDirectoryListing::CreatePath(const char *pzPathOnlyOrPathAndFileName, int bPathHasFileName)
{
	// cast off the const, we'll modify then restore the string
	char *pzFileAndPath = (char *)pzPathOnlyOrPathAndFileName;

	if (!pzFileAndPath)
	{
		return;
	}
	int nLen = strlen(pzFileAndPath);
	for(int i=0;i<nLen+1;i++)
	{
		if (pzFileAndPath[i] == '\\' || pzFileAndPath[i] == '/' || pzFileAndPath[i] == 0)
		{
			if ( bPathHasFileName && pzFileAndPath[i] == 0)
			{
				// if the path includes a filename, we're done.
				break; 
			}

			char ch = pzFileAndPath[i];
			pzFileAndPath[i] = 0;

			int nAttempts = 0;
RETRY_MKDIR:
			nAttempts++;


			int nRslt = mkdir(pzFileAndPath);

			if (nRslt != 0 && nAttempts < 5 && errno == 2)
			{
				goto RETRY_MKDIR;
			}

			pzFileAndPath[i] = ch;
		}
	}
}

// returns 1 if the argument is a directory, 0 if it's a file or a bad path.
int CDirectoryListing::IsDirectory(const char *szDirPath)
{
	struct stat sstruct;
	int result = stat(szDirPath, &sstruct);
	if (result == 0)
	{

		if ( (sstruct.st_mode & _S_IFDIR)  )
		{
			return 1;
		}

	}
	return 0;
}


void CDirectoryListing::RecurseFolder(const char *pzDir, GStringList *strDirs, GStringList *strFiles)
{
	char chSlash = '\\';

	static GString strDot("[dir] .");
	static GString strDotDot("[dir] ..");
	try
	{
		// Sample listing 2 files + 1 directory = "file1.txt*[dir] Temp*file2.txt"
		GString strResults;
		CDirectoryListing dir(pzDir, 2); // directories only
		GStringIterator it(&dir);
		while (it())
		{
			// pzResult will look something like "[dir] SubDir"
			const char *pzResult = it++; 
			if (strDot.Compare(pzResult) != 0 && strDotDot.Compare(pzResult) != 0)
			{
				// pzDir may be "/myPath" to begin with
				GString strFullPath(pzDir);
				if ( strFullPath.GetAt(strFullPath.GetLength() - 1) != '\\' && 
					 strFullPath.GetAt(strFullPath.GetLength() - 1) != '/')
				{
					// pzDir will now end with a slash if it did not already.
					// like "/myPath/" or "c:\myPath\"
					strFullPath += chSlash;
				}

				// add the file name to the complete path we're building
				strFullPath += &pzResult[6]; // skip the "[dir] ", add a string like "SubDir"

				if(strDirs)
				{
					strDirs->AddLast(strFullPath);
				}

				// now add the final slash for a string like this "/myPath/SubDir/"
				strFullPath += chSlash;

				// go down into that directory now.
				RecurseFolder(strFullPath, strDirs, strFiles);
			}
		}

		if(strFiles)
		{
			CDirectoryListing files(pzDir, 1); // files only
			GStringIterator it2(&files);
			while (it2())
			{
				// pzDir may be "/myPath" to begin with
				GString strFullPath(pzDir);
				if ( strFullPath.GetAt(strFullPath.GetLength() - 1) != '\\' && 
					 strFullPath.GetAt(strFullPath.GetLength() - 1) != '/')
				{
					// strFullPath will now end with a slash like "/myPath/" 
					strFullPath += chSlash;
				}


				const char *pzFile = it2++;
				strFullPath += pzFile;
				strFiles->AddLast((const char *)strFullPath);
			}
		}
	}
	catch( GenericException & )
	{
		// ignore the directory we can't access 
		// rErr.GetDescription();
	}
}


void CDirectoryListing::Init(const char *szDirPath, int nMode)
{
	static GString dotdot("..");
	static GString dot(".");
	
	bool bIncludeSubDirs = (nMode == 2 || nMode == 3) ? 1 : 0;
	bool bIncludeFiles = (nMode == 1 || nMode == 3) ? 1 : 0;
	
	
	GString strPathWithTrailingSlash(szDirPath);
	GString strPathWithNoTrailingSlash(szDirPath);

	// if szDirPath ends with a slash
	if ( strPathWithNoTrailingSlash.Right(1) == "/" ||
		 strPathWithNoTrailingSlash.Right(1) == "\\" )
	{
		// if the path is "/" leave it alone
		if (strPathWithNoTrailingSlash.Length() > 1)
			strPathWithNoTrailingSlash = strPathWithNoTrailingSlash.Left(strPathWithNoTrailingSlash.Length() - 1);
	}
	else
	{
		strPathWithTrailingSlash += "\\";

	}

	if( _access( (const char *)strPathWithNoTrailingSlash, 0 ) )
	{
		throw GenericException("GDirectoryListing",0,(const char *)strPathWithNoTrailingSlash, errno);
	}

	// FindFirstFile & FindNextFile
	HANDLE hFindFile;
	WIN32_FIND_DATA find;
	BOOL fRet = TRUE;
	GString strSearch( strPathWithTrailingSlash );
	strSearch += "*.*";
	hFindFile = FindFirstFile((const char *)strSearch, &find);
	while (hFindFile != (HANDLE)-1 && fRet == TRUE) 
	{
		GString strTemp( strPathWithTrailingSlash );
		strTemp += find.cFileName;
		struct stat sstruct;

		int result = stat(strTemp, &sstruct);
		if (result == 0)
		{
			if ( !(sstruct.st_mode & _S_IFDIR)  )
			{
				// Add the file
				if (bIncludeFiles)
				{
					AddLast((const char *)find.cFileName);
				}
			}
			else if (bIncludeSubDirs)
			{
				GString strFileName( LastLeaf( (char *)(const char *)strTemp,'\\') );
				if ( ( dotdot.Compare(strFileName) != 0 ) && ( dot.Compare(strFileName) != 0 ))
				{
					GString strFormattedDir;
					strFormattedDir.Format("[dir] %s", LastLeaf( (char *)(const char *)strFileName,'\\') );
					AddLast((const char *)strFormattedDir);
				}
			}

		}

		fRet = FindNextFile(hFindFile, &find);
	}
	FindClose(hFindFile);
}


CDirectoryListing::CDirectoryListing(const char *szPath)
{
	Init(szPath, 1);
}

// nMode = 1 files, 2 dirs, 3 both
CDirectoryListing::CDirectoryListing(const char *szPath, int nMode)
{
	Init(szPath, nMode);
}

CDirectoryListing::~CDirectoryListing()
{
}

