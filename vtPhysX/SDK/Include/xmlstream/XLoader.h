/*************************************************************************/
/*	File : XLoader.h				 				 					 */
/*																		 */	
/*	DirectX .X files loader												 */	
/*																		 */	
/*	Virtools SDK 														 */	 
/*	Copyright (c) Virtools 2000, All Rights Reserved.					 */	
/*************************************************************************/

#ifndef _XLOADER_H
#define _XLOADER_H

//#include "Windows.h"
#include "stdio.h"
#include "DxFile.h"
#include "rmxfguid.h"
#include "rmxftmpl.h"
#include "Ge2Virtools.h"
#include "ptypes.h"


XString GetFileObjectName(LPDIRECTXFILEOBJECT obj);


#define SAFERELEASE(x) { if (x) x->Release(); x = NULL; }

/**************************************************
+ Overload of a model reade
+ 
***************************************************/	
class CKXReader: public CKModelReader {
public:	
	void Release() {delete this; };
	
// Reader Info
	virtual CKPluginInfo* GetReaderInfo();

// No specific Options
	virtual int GetOptionsCount() { return 0; }
	virtual CKSTRING GetOptionDescription(int i) { return NULL; }
	
// This reader can only load .X files
	virtual CK_DATAREADER_FLAGS GetFlags() {return (CK_DATAREADER_FLAGS)CK_DATAREADER_FILELOAD;}

// Load Method
	virtual CKERROR Load(CKContext* context,CKSTRING FileName,CKObjectArray *liste,CKDWORD LoadFlags,CKCharacter *carac=NULL);
	BOOL LoadFromFileC(CKContext *ctx, XString filename, CKBOOL hidden, CKDWORD loadflags, CKObjectArray* targetArray, XString password);


	CKXReader() {

		m_Context = NULL;
		m_VirtoolsExport = NULL;
		
		m_Unnamed		 = 0;
	}
	~CKXReader() {
		CleanUp();
	}

protected:
	void CleanUp() {
	
		delete m_VirtoolsExport;
		m_VirtoolsExport = NULL;
	}
//-- High level 
	

	//-- For unnamed objects return a generic string "Unnamed_XX"  
	XString GetUnnamed() {  XString Temp = "Unnamed_"; Temp << m_Unnamed++; return Temp; }

public: 
	CKContext*					m_Context;
	CK_OBJECTCREATION_OPTIONS	m_CreationOptions;
	CKCharacter*				m_Character;
	DWORD						m_LoadFlags;
	CK_CLASSID					m_3dObjectsClass;
	XString						m_FileName;

	Export2Virtools*			m_VirtoolsExport;	
	
	int							m_Unnamed;
	float						m_AnimationLength;


};


#endif