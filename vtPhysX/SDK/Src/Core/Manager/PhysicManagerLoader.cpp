#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "tinyxml.h"



static const TiXmlElement *getFirstOdeElement(const TiXmlElement *root);
static const TiXmlElement *getFirstOdeElement(const TiXmlElement *root)                       
{
	// should have some recursive algo 
	if (!strcmp(root->Value(), "vtPhysics"))
	{
		return root;
	}
	for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling())
	{
		if (child->Type() == TiXmlNode::ELEMENT)
		{
			const TiXmlElement *res = getFirstOdeElement(child->ToElement ());
			if (res)
				return res;
		}
	}

	return 0;
}
TiXmlDocument*
PhysicManager::loadDefaults(XString filename)
{

	
	// load and check file
	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,filename.CStr());	
	XString name(Ini);
	name << '\0';

	m_DefaultDocument = new TiXmlDocument(filename.Str());
	m_DefaultDocument ->LoadFile(Ini);
	m_DefaultDocument ->Parse(Ini);
	
	if (m_DefaultDocument->Error())
	{
		//xLogger::xLog("couldn't load default document");

		delete m_DefaultDocument;
		m_DefaultDocument = NULL;

		return NULL;
	}

	// get the ogreode element.
	TiXmlNode* node = m_DefaultDocument->FirstChild( "vtPhysics" );
	if (!node)
	{
		//Ogre::LogManager::getSingleton().logMessage("  cannot find ogreode root in XML file!");
		return NULL;
	}
	return m_DefaultDocument;

}