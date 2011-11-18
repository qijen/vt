#include <StdAfx.h>
#include "vtPhysXAll.h"
//#include "tinyxml.h"
#include <PhysicManager.h>


static pFactory* pFact = NULL;

void pFactory::findAttributeIdentifiersByGuid(CKGUID guid,std::vector<int>&targetList)
{

	CKContext *ctx = GetPMan()->GetContext();

	CKAttributeManager *attMan = ctx->GetAttributeManager();
	CKParameterManager *parMan = ctx->GetParameterManager();

	int cCount = attMan->GetAttributeCount();
	for(int i  = 0 ; i < cCount ; i++)
	{
		CKSTRING name = attMan->GetAttributeNameByType(i);
		if ( parMan->ParameterTypeToGuid(attMan->GetAttributeParameterType(i)) == guid )
		{
			targetList.push_back(i);
		}
		
	}
}

//************************************
// Method:    Instance
// FullName:  vtODE::pFactory::Instance
// Access:    public 
// Returns:   pFactory*
// Qualifier:
//************************************
pFactory* pFactory::Instance()
{

	if (!pFact)
	{
		pFact  = new pFactory(GetPMan(),GetPMan()->getDefaultConfig());
	}
	return pFact;
}





//************************************
// Method:    ~pFactory
// FullName:  vtODE::pFactory::~pFactory
// Access:    public 
// Returns:   
// Qualifier:
//************************************
pFactory::~pFactory()
{

	//Clean();
	delete pFact;
	pFact = NULL;
}

pFactory::pFactory()
{

}
//************************************
// Method:    pFactory
// FullName:  vtODE::pFactory::pFactory
// Access:    public 
// Returns:   // Qualifier: : m_PManager(prm1), m_DefaultDocument(prm2)
// Parameter: PhysicManager* prm1
// Parameter: TiXmlDocument*prm2
//************************************
pFactory::pFactory(PhysicManager* prm1,TiXmlDocument*prm2) : 
	mManager(prm1), m_DefaultDocument(prm2)
{
	pFact = this;
	mPhysicSDK = NULL;
}
//************************************
// Method:    ResolveFileName
// FullName:  vtODE::pFactory::ResolveFileName
// Access:    public 
// Returns:   XString
// Qualifier:
// Parameter: const char *input
//************************************
XString pFactory::ResolveFileName(const char *input)
{
	CKPathManager *pm = GetPMan()->m_Context->GetPathManager();
	
	FILE *file  =  fopen(input,"r");
	XString result;
	if (file)
	{
		char Ini[MAX_PATH];
		char drive[MAX_PATH];
		char dir[MAX_PATH];
		char szPath[MAX_PATH];

		GetModuleFileName(NULL,szPath,_MAX_PATH);
		_splitpath(szPath, drive, dir, NULL, NULL );
		sprintf(Ini,"%s%s%s",drive,dir,input);	
		
		fclose(file);

		return XString(Ini);
	}

	if (!file)
	{
		CKSTRING lastCmo = GetPMan()->m_Context->GetLastCmoLoaded();
		CKPathSplitter splitter(const_cast<char*>(lastCmo));
		CKPathSplitter splitter2(const_cast<char*>(input));
		CKPathMaker maker(splitter.GetDrive(),splitter.GetDir(),const_cast<char*>(input),"");
		char* NewFilename = maker.GetFileName();
		file = fopen(NewFilename,"r");
		if (!file)
		{
			char Ini[MAX_PATH];
			char drive[MAX_PATH];
			char dir[MAX_PATH];
			char szPath[MAX_PATH];

			GetModuleFileName(NULL,szPath,_MAX_PATH);
			_splitpath(szPath, drive, dir, NULL, NULL );
			sprintf(Ini,"%s%s%s",drive,dir,input);	
			file = fopen(Ini,"r");
			if(file)
			{
				fclose(file);
				return XString(Ini);
			}

			if(pm)
			{
				XString fname(const_cast<char*>(input));
				CKERROR error  = GetPMan()->m_Context->GetPathManager()->ResolveFileName( fname , DATA_PATH_IDX);
				if (error ==CK_OK)
				{
					file  = fopen(fname.CStr(),"r");
					if (file)
					{
						fclose(file);
						result = fname;
					}
				}

			}
		}
	}
	return result;
}

//************************************
// Method:    CreateFrame
// FullName:  vtODE::pFactory::CreateFrame
// Access:    public 
// Returns:   CK3dEntity*
// Qualifier:
// Parameter: XString name
//************************************
CK3dEntity*
pFactory::createFrame(const char* name)
{
	if (!strlen(name))
	{
		return NULL;
	}

	int count = ctx()->GetObjectsCountByClassID(CKCID_3DENTITY);
	CK_ID* ol = ctx()->GetObjectsListByClassID(CKCID_3DENTITY);

	for(int j=0;j<count;j++ )
	{
		CKBeObject* o  = static_cast<CKBeObject*>(ctx()->GetObject(ol[j]));
		if (!strcmp(o->GetName(),name ) )
		{
			return static_cast<CK3dEntity*>(o);
		}
	}

	CK_OBJECTCREATION_OPTIONS creaoptions = CK_OBJECTCREATION_NONAMECHECK;
	//if (dynamic) creaoptions = CK_OBJECTCREATION_DYNAMIC;

	// The Creation
	
	CKObject* object = ctx()->CreateObject(CKCID_3DENTITY,const_cast<char*>(name),creaoptions);
	CK3dEntity* ent=(CK3dEntity*)object;
	ent->SetFlags(ent->GetFlags()|CK_3DENTITY_FRAME);

	// we add it to the level

	CKLevel *level  = ctx()->GetCurrentLevel();
	if (level)
	{
		level->AddObject(object);
	}

	return ent;

}



//************************************
// Method:    _str2Vec
// FullName:  vtODE::pFactory::_str2Vec
// Access:    public 
// Returns:   VxVector
// Qualifier:
// Parameter: XString _in
//************************************
VxVector 
pFactory::_str2Vec(XString _in)
{
	short nb = 0 ; 
	VxVector out;
	XStringTokenizer	tokizer(_in.CStr(), ",");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) && nb < 3)
	{
		XString tokx(tok);
		out.v[nb] = tokx.ToFloat();
		nb++;
	}
	return out;
}

int pFactory::_str2MaterialFlag(XString _in)
{
	
	short nb = 0 ; 
	int result = 0;
	XStringTokenizer	tokizer(_in.CStr(), "|");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) && nb < 3)
	{
		XString tokx(tok);
		
		if ( _stricmp(tokx.CStr(),"Anisotropic") == 0 ) 
		{	
			result |= MF_Anisotropic;
		}
		if ( _stricmp(tokx.CStr(),"DisableFriction") == 0 ) 
		{	
			result |= MF_DisableFriction;
		}
		if ( _stricmp(tokx.CStr(),"DisableStrongFriction") == 0 ) 
		{	
			result |= MF_DisableStrongFriction;
		}
 		nb++;
	}
	return result;
}


int pFactory::_getEnumIndex(XString enumerationFull,XString enumValue)
{

	int result = 0;
	XStringTokenizer	tokizer(enumerationFull.CStr(), ",");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)))
	{
		XString tokx(tok);
		if (	!strcmp(tokx.CStr(),enumValue.CStr())	)
		{
			return result;
		}
	//	out.v[nb] = tokx.ToFloat();
		result++;
	}
	return 0;
}