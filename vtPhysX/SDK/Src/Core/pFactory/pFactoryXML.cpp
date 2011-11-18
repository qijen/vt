#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pSleepingSettings.h"
#include "pWorldSettings.h"

#include "tinyxml.h"

TiXmlDocument*pFactory::loadConfig(const char* filename)
{

	// load and check file
	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,filename);	
	XString name(Ini);
	name << '\0';

	m_DefaultDocument = new TiXmlDocument(filename);
	m_DefaultDocument ->LoadFile(Ini);
	m_DefaultDocument ->Parse(Ini);

	if (m_DefaultDocument->Error())
	{

		delete m_DefaultDocument;
		m_DefaultDocument = NULL;

		return NULL;
	}

	// get the ogreode element.
	TiXmlNode* node = m_DefaultDocument->FirstChild( "vtPhysics" );
	if (!node)
	{
		return NULL;
	}
	return m_DefaultDocument;

}

int pFactory::reloadConfig(const char *fName)
{


	if (! fName || !strlen(fName))return -1;
	////////////////////////////////////////Load our physic default xml document : 
	if (getDefaultDocument())
	{
		delete m_DefaultDocument;
		m_DefaultDocument = NULL;
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Deleted old default config");
	} 

	TiXmlDocument * defaultDoc  = loadConfig(fName);
	if (!defaultDoc)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Loading default config  : PhysicDefaults.xml: failed");
		setDefaultDocument(NULL);
	}else
	{
		setDefaultDocument(defaultDoc);
	}
	return 1;
}


pRemoteDebuggerSettings pFactory::createDebuggerSettings(const TiXmlDocument * doc)
{

	pRemoteDebuggerSettings result;
	result.enabled = -1;

	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if (doc)
	{
		
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "Debugger" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{
						if(!strcmp( element->Attribute("name"),"Default" ) )
						{
							for (const TiXmlNode *node = element->FirstChild(); node; node = node->NextSibling() )
							{
								if ((node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(node->Value(),"settings")))
								{
									const TiXmlElement *sube = (const TiXmlElement*)node;
									double v;
									int i=0;


									//////////////////////////////////////////////////////////////////////////
									int res = sube->QueryIntAttribute("enabled",&i);
									if (res == TIXML_SUCCESS)
									{
										result.enabled = i ;
									}
									
									res = sube->QueryIntAttribute("port",&i);
									if (res == TIXML_SUCCESS)
									{
										result.port = i ;
									}

									//////////////////////////////////////////////////////////////////////////
									const char* host  = sube->Attribute("host");
									if (strlen(host))
									{
										//strcpy(result.mHost,host);
										result.mHost = host;
									}
									return result;
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}
//************************************
// Method:    GetFirstDocElement
// FullName:  vtODE::pFactory::GetFirstDocElement
// Access:    public 
// Returns:   const TiXmlElement*
// Qualifier:
// Parameter: const TiXmlElement *root
//************************************
const TiXmlElement*pFactory::getFirstDocElement(const TiXmlElement *root)
{

	if (!strcmp(root->Value(), "vtPhysics"))
	{
		return root;
	}
	for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling())
	{
		if (child->Type() == TiXmlNode::ELEMENT)
		{
			const TiXmlElement *res = getFirstDocElement(child->ToElement ());
			if (res)
				return res;
		}
	}
	return 0;
}
//************************************
// Method:    GetDocument
// FullName:  vtPhysics::pFactory::GetDocument
// Access:    public 
// Returns:   TiXmlDocument*
// Qualifier:
// Parameter: XString filename
//************************************
TiXmlDocument* pFactory::getDocument(XString filename)
{
	

	XString fname(filename.Str());
	if ( fname.Length() )
	{
		XString fnameTest = ResolveFileName(fname.CStr());
		if ( fnameTest.Length())
		{
			TiXmlDocument* result  = new TiXmlDocument(fnameTest.Str());
			result->LoadFile(fnameTest.Str());
			result->Parse(fnameTest.Str());

			TiXmlNode* node = result->FirstChild( "vtPhysics" );
			if (!node)
			{
				GetPMan()->m_Context->OutputToConsoleEx("PFactory : Couldn't load Document : %s",filename.Str());
				return NULL;
			}else
			{
				return result;
			}
		}
	}
	return NULL;
}

//************************************
// Method:    CreateWorldSettings
// FullName:  vtPhysics::pFactory::CreateWorldSettings
// Access:    public 
// Returns:   pWorldSettings*
// Qualifier:
// Parameter: const char* nodeName
// Parameter: const char* filename
//************************************
/*!
 * \brief
 * Write brief comment for createWorldSettings here.
 * 
 * \param nodeName
 * Description of parameter nodeName.
 * 
 * \param filename
 * Description of parameter filename.
 * 
 * \returns
 * Write description of return value here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for createWorldSettings here.
 * 
 * \remarks
 * Write remarks for createWorldSettings here.
 * 
 * \see
 * Separate items with the '|' character.
 */
pWorldSettings*pFactory::createWorldSettings(const char* nodeName, const char* filename)
{

	XString fname(filename);
	XString nName(nodeName);
	if ( nName.Length() && fname.Length() )
	{
		pWorldSettings *result = NULL;
		TiXmlDocument * document  = getDocument(fname);
		if (document)
		{
			result =createWorldSettings(nodeName,document);
			if ( result)
			{
				delete document;
				return result;
			}
		}

		if (document)
		{
			result  = new pWorldSettings();
			result->setGravity(VxVector(0,-9.81,0));
			result->setSkinWith(0.02f);
			return result;
		}
	}
	return NULL;
}
//************************************
// Method:    CreateWorldSettings
// FullName:  vtPhysics::pFactory::CreateWorldSettings
// Access:    public 
// Returns:   pWorldSettings*
// Qualifier:
// Parameter: XString nodeName
// Parameter: TiXmlDocument * doc 
//************************************

pWorldSettings*
pFactory::createWorldSettings(const XString nodeName/* = */, const TiXmlDocument * doc /* = NULL */)
{

	pWorldSettings *result  = new pWorldSettings();
	
	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if (doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "world" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{
						if(!strcmp( element->Attribute("name"),nodeName.CStr() ) )
						{
							for (const TiXmlNode *node = element->FirstChild(); node; node = node->NextSibling() )
							{
								if ((node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(node->Value(),"settings")))
								{
									const TiXmlElement *sube = (const TiXmlElement*)node;
									double v;
									int iVal=0;

									//////////////////////////////////////////////////////////////////////////
									int res = sube->QueryDoubleAttribute("SkinWith",&v);
									if (res == TIXML_SUCCESS)
									{
										result->setSkinWith((float)v);
									}

									//////////////////////////////////////////////////////////////////////////
									const char* grav  = sube->Attribute("Gravity");
									VxVector vec  = _str2Vec(grav);
									result->setGravity(vec);

									res = sube->QueryIntAttribute("IsFixedTime",&iVal);
									if (res == TIXML_SUCCESS)
									{
										result->setFixedTime(iVal==1? true : false );

									}

									res = sube->QueryIntAttribute("NumIterations",&iVal);
									if (res == TIXML_SUCCESS)
									{
										result->setNumIterations(iVal);
									}


									res = sube->QueryDoubleAttribute("FixedTimeStep",&v);
									if (res == TIXML_SUCCESS)
									{
										result->setFixedTimeStep((float)v);
									}
									const char* flags  = NULL;
									flags  = sube->Attribute("SceneFlags");
									if (flags && strlen(flags))
									{
										result->setSceneFlags(_str2SceneFlags(flags));
									}
									const char* pflags  = NULL;
									pflags  = sube->Attribute("PhysicFlags");
									if (pflags && strlen(pflags))
									{
										result->setPhysicFlags(_str2PhysicFlags(pflags));
									}
								}
							}
						}
					}
				}
			}
		}
	}


	//result->setGravity(VxVector(0,-9.81,0));
	//result->setSkinWith(0.02f);

	return result;
}


//************************************
// Method:    CreateSleepingSettings
// FullName:  vtPhysics::pFactory::CreateSleepingSettings
// Access:    public 
// Returns:   pSleepingSettings*
// Qualifier:
// Parameter: const char* nodeName
// Parameter: const char *filename
//************************************
pSleepingSettings*pFactory::CreateSleepingSettings(const char* nodeName,const char *filename)
{
	
	XString fname(filename);
	XString nName(nodeName);
	if ( nName.Length() && fname.Length() )
	{
		TiXmlDocument * document  = getDocument(fname);
		if (document)
		{
			pSleepingSettings *result = CreateSleepingSettings(nodeName,document);
			if ( result)
			{
				delete document;
				return result;
			}
		}
	}
	return NULL;

}

//************************************
// Method:    CreateSleepingSettings
// FullName:  vtPhysics::pFactory::CreateSleepingSettings
// Access:    public 
// Returns:   pSleepingSettings*
// Qualifier:
// Parameter: XString nodeName
// Parameter: TiXmlDocument * doc 
//************************************
pSleepingSettings*pFactory::CreateSleepingSettings(XString nodeName/* = */, TiXmlDocument * doc /* = NULL */)
{
	pSleepingSettings *result = new pSleepingSettings();
	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if (nodeName.Length() && doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "sleepsettings" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{
						if(!strcmp( element->Attribute("name"),nodeName.CStr() ) )
						{

							//////////////////////////////////////////////////////////////////////////
							int v;
							int res = element->QueryIntAttribute("SleepSteps",&v);
							if (res == TIXML_SUCCESS)
							{
								result->SleepSteps(v);
							}
							//////////////////////////////////////////////////////////////////////////
							res = element->QueryIntAttribute("AutoSleepFlag",&v);
							if (res == TIXML_SUCCESS)
							{
								result->AutoSleepFlag(v);
							}
							//////////////////////////////////////////////////////////////////////////
							float vF;
							res = element->QueryFloatAttribute("AngularThresold",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->AngularThresold(vF);
							}
							//////////////////////////////////////////////////////////////////////////
							res = element->QueryFloatAttribute("LinearThresold",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->LinearThresold(vF);
							}

							return result;
						}
					}
				}
			}
		}
	}
	return result;
}



XString pFactory::_getEnumDescription(const TiXmlDocument *doc, XString identifier)
{

	if (!doc)
	{
		return XString("");
	}

	XString result("None=0");
	int counter = 1;


	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if ( doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), identifier.Str() ) )
				{

					const TiXmlElement *sube = (const TiXmlElement*)child;

					const char* matName  = NULL;
					matName = sube->Attribute("name");
					if (matName && strlen(matName))
					{
						if (result.Length())
						{
							result << ",";
						}
						result << matName;
						result << "=" << counter;
						counter ++;
					}
				}
			}
		}
	}

	return result;

}

XString pFactory::_getBodyXMLInternalEnumeration(const TiXmlDocument * doc)
{

	return _getEnumDescription(doc,"Body");

}
XString pFactory::_getBodyXMLExternalEnumeration(const TiXmlDocument * doc)
{

	return _getEnumDescription(doc,"Body");

}

XString pFactory::_getMaterialsAsEnumeration(const TiXmlDocument * doc)
{

	if (!doc)
	{
		return XString("");
	}

	XString result("None=0");
	int counter = 1;


	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if ( doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "material" ) )
				{
								
					const TiXmlElement *sube = (const TiXmlElement*)child;
					
					const char* matName  = NULL;
					matName = sube->Attribute("name");
					if (matName && strlen(matName))
					{
						if (result.Length())
						{
							result << ",";
						}
						result << matName;
						result << "=" << counter;
						counter ++;
					}
				}
			}
		}
	}

	return result;

}


//************************************
// Method:    _str2CombineMode
// FullName:  vtAgeia::pFactory::_str2CombineMode
// Access:    protected 
// Returns:   int
// Qualifier:
// Parameter: const char*input
//************************************
int pFactory::_str2CombineMode(const char*input)
{

	int result  = -1;
	if (!strlen(input))
	{
		return result;
	}

	if (!strcmp(input,"AVERAGE"))
	{
		return 0;
	}
	
	if (!strcmp(input,"MIN"))
	{
		return 1;
	}
	if (!strcmp(input,"MAX"))
	{
		return 3;
	}

	if (!strcmp(input,"MULTIPLY"))
	{
		return 2;
	}
	return result;

}