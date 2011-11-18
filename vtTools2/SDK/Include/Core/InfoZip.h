#include "ZipDll.h"
#include "UnzipDll.h"
#include "CKAll.h"

//#include "..\Manager\ZipManager.h"

#define ZERROR_NONE                    0
#define ZERROR_DLL_NOT_FOUND           1
#define ZERROR_DLL_FOUNCTION_NOT_FOUND 2
#define ZERROR_NOT_INITIALIZED         3

class CInfoZip {
public:

	
	CInfoZip();
	virtual ~CInfoZip();

	BOOL ExtractFiles(const char* pszArchive, const char* pszTargetFolder);
	void SetDefaultValues(CUnzipParams *pParams);
	void ReleaseParams(CUnzipParams *pParams);
	BOOL Execute(CUnzipParams *pParams);
	BOOL FinalizeUnzip();
	BOOL FinalizeZip(char *tempfile);
	BOOL InitializeUnzip();
	BOOL InitializeZip(char *tempfile);
	BOOL GetInitializedUnzip();
	BOOL GetInitializedZip();
	void ReleaseParams(CZipParams *pParams);
	BOOL AddFiles(const char *pszArchive, char **paFiles, int iFileCount);
	BOOL Execute(CZipParams *pParams);
	void SetDefaultValues(CZipParams *pZipParms);
	BOOL GetInitialized();
	UINT GetLastError();
	int GetZipDllVersion();
	int GetUnzipDllVersion();

	CZipDllExec m_ZipDllExec;
	
	

	void SetLastError(UINT uiError);
	UINT m_uiLastError;
	HINSTANCE m_ZipDllHandle;
	HINSTANCE m_UnzipDllHandle;
	
	CUnzipDllExec m_UnzipDllExec;
	CGetZipDllVersion   m_GetZipDllVersion;
	
	

private:
	
};

BOOL __stdcall DefaultZipCallback(CZipCallbackData *pData);


