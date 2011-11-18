#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "tinyxml.h"

#include <xDebugTools.h>


NxMaterialDesc* pFactory::createMaterialFromEntity(CKBeObject*object)
{

	//////////////////////////////////////////////////////////////////////////
	//sanity checks : 
	if (!object )
	{
		return NULL;
	}

	if (!object->HasAttribute(GetPMan()->att_surface_props))
	{
		return NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	CKParameterManager *pm = object->GetCKContext()->GetParameterManager();
	int parameterType  =  pm->ParameterGuidToType(VTE_XML_MATERIAL_TYPE);

	NxMaterialDesc *result = new NxMaterialDesc();
	using namespace vtTools::AttributeTools;
	XString nodeName;


	int enumID = GetValueFromAttribute<int>(object,GetPMan()->att_surface_props,0);
	if (enumID==0)
	{
		goto takeFromAttribute;
	}


	CKEnumStruct *enumStruct = pm->GetEnumDescByType(parameterType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{
			if(i == enumID)
			{
				nodeName  = enumStruct->GetEnumDescription(i);
				if (nodeName.Length())
				{
					result  = createMaterialFromXML(nodeName.CStr(),getDefaultDocument());
					if (result)
					{
						return result;
					}
				}
			}
		}
	}


takeFromAttribute :
	{
		float dynamicFriction = GetValueFromAttribute<float>(object,GetPMan()->att_surface_props,1);
		if (dynamicFriction >=0.0f)
		{
			result->dynamicFriction  =dynamicFriction;
		}

		float statFriction = GetValueFromAttribute<float>(object,GetPMan()->att_surface_props,2);
		if (statFriction>=0.0f)
		{
			result->staticFriction=statFriction;
		}

		float rest = GetValueFromAttribute<float>(object,GetPMan()->att_surface_props,3);
		if (rest>=0.0f)
		{
			result->restitution=rest;
		}

		float dynamicFrictionV = GetValueFromAttribute<float>(object,GetPMan()->att_surface_props,4);
		if (dynamicFrictionV >=0.0f)
		{
			result->dynamicFrictionV  =dynamicFrictionV;
		}

		float staticFrictionV = GetValueFromAttribute<float>(object,GetPMan()->att_surface_props,5);
		if (staticFrictionV>=0.0f)
		{
			result->staticFriction=staticFrictionV;
		}

		VxVector anis = GetValueFromAttribute<VxVector>(object,GetPMan()->att_surface_props,6);
		if (anis.Magnitude()>=0.01f)
		{
			result->dirOfAnisotropy=pMath::getFrom(anis);
		}

		int fMode = GetValueFromAttribute<int>(object,GetPMan()->att_surface_props,7);
		if (fMode !=-1)
		{
			result->frictionCombineMode=(NxCombineMode)fMode;
		}
		int rMode = GetValueFromAttribute<int>(object,GetPMan()->att_surface_props,8);
		if (rMode !=-1)
		{
			result->restitutionCombineMode=(NxCombineMode)rMode;
		}

		return result;


	}


	return result;


}

bool pFactory::copyTo(pMaterial&dst,NxMaterial*src)
{

	if (!src)
	{
		return false;
	}

	dst.xmlLinkID = (int)src->userData;
	dst.dirOfAnisotropy = getFrom(src->getDirOfAnisotropy());
	dst.dynamicFriction = src->getDynamicFriction();
	dst.dynamicFrictionV = src->getDynamicFrictionV();
	dst.staticFriction = src->getStaticFriction();
	dst.staticFrictionV = src->getStaticFrictionV();
	dst.frictionCombineMode  = (CombineMode)src->getFrictionCombineMode();
	dst.restitutionCombineMode  = (CombineMode)src->getRestitutionCombineMode();
	dst.restitution  = src->getRestitution();
	dst.flags = src->getFlags();
	dst.setNxMaterialID(src->getMaterialIndex());

	return false;
}
bool pFactory::copyTo(NxMaterialDesc &dst, const pMaterial &src)
{

	dst.dirOfAnisotropy = getFrom(src.dirOfAnisotropy);
	dst.dynamicFriction = src.dynamicFriction;
	dst.dynamicFrictionV = src.dynamicFrictionV;
	dst.staticFriction = src.staticFriction;
	dst.staticFrictionV = src.staticFrictionV;
	dst.frictionCombineMode  = (NxCombineMode)src.frictionCombineMode;
	dst.restitutionCombineMode  = (NxCombineMode)src.restitutionCombineMode;
	dst.restitution  = src.restitution;
	dst.flags = src.flags;


	return true;

}

bool pFactory::copyTo(pMaterial& dst,CKParameter*src)
{
	if (!src)return false;

	using namespace vtTools::ParameterTools;
	
	dst.xmlLinkID = GetValueFromParameterStruct<int>(src,E_MS_XML_TYPE);
	dst.dirOfAnisotropy = GetValueFromParameterStruct<VxVector>(src,E_MS_ANIS);
	dst.dynamicFriction = GetValueFromParameterStruct<float>(src,E_MS_DFRICTION);
	dst.dynamicFrictionV = GetValueFromParameterStruct<float>(src,E_MS_DFRICTIONV);
	dst.staticFriction = GetValueFromParameterStruct<float>(src,E_MS_SFRICTION);
	dst.staticFrictionV = GetValueFromParameterStruct<float>(src,E_MS_SFRICTIONV);
	dst.restitution= GetValueFromParameterStruct<float>(src,E_MS_RESTITUTION);
	dst.restitutionCombineMode= (CombineMode)GetValueFromParameterStruct<int>(src,E_MS_RCMODE);
	dst.frictionCombineMode= (CombineMode)GetValueFromParameterStruct<int>(src,E_MS_FCMODE);
	dst.flags= GetValueFromParameterStruct<int>(src,E_MS_FLAGS);
	return true;



}

bool pFactory::findSettings(pMaterial& dst,CKBeObject*src)
{

	if (!src)return false;

	CKParameterOut *parMaterial = src->GetAttributeParameter(GetPMan()->att_surface_props);
	
	if (parMaterial){
		pFactory::Instance()->copyTo(dst,(CKParameter*)parMaterial);
		goto EVALUATE;
	}


	
	CK3dEntity *ent3D  = static_cast<CK3dEntity*>(src);
	if (ent3D)
	{
		CKMesh *mesh = ent3D->GetCurrentMesh();
		if (mesh)
		{
			parMaterial = mesh->GetAttributeParameter(GetPMan()->att_surface_props);
			if (!parMaterial)
			{
				for (int i  = 0 ; i  < mesh->GetMaterialCount() ; i++)
				{
					CKMaterial *entMaterial  = mesh->GetMaterial(i);
					parMaterial = entMaterial->GetAttributeParameter(GetPMan()->att_surface_props);
					if (parMaterial)
					{
						break;
					}
				}
			}
		}
	}

	if (!parMaterial)		return false;


	//////////////////////////////////////////////////////////////////////////
	//copy parameter content to c++ 
	pFactory::Instance()->copyTo(dst,(CKParameter*)parMaterial);

	///////////////////////////////////////////////////////////////////////////
	//
	//	Evaluate from XML
	//

	 
EVALUATE:

	if ( dst.xmlLinkID !=0 )
	{
		XString nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_MATERIAL_TYPE,dst.xmlLinkID);
		bool err = loadFrom(dst,nodeName.Str(),getDefaultDocument());
		if (err)
		{
			copyTo(parMaterial,dst);
			return true;
		}


	}
	return false;
}

bool pFactory::copyTo(CKParameterOut*dst,const pMaterial&src)
{
	if (!dst)return false;

	using namespace vtTools::ParameterTools;

	//////////////////////////////////////////////////////////////////////////
	//@todo : find the xml name !

	SetParameterStructureValue<int>(dst,E_MS_XML_TYPE,src.xmlLinkID,false);
	SetParameterStructureValue<float>(dst,E_MS_DFRICTION,src.dynamicFriction,false);
	SetParameterStructureValue<float>(dst,E_MS_DFRICTIONV,src.dynamicFrictionV,false);
	SetParameterStructureValue<float>(dst,E_MS_SFRICTION,src.staticFriction,false);
	SetParameterStructureValue<float>(dst,E_MS_SFRICTIONV,src.staticFrictionV,false);
	SetParameterStructureValue<int>(dst,E_MS_FCMODE,src.frictionCombineMode,false);
	SetParameterStructureValue<int>(dst,E_MS_RCMODE,src.restitutionCombineMode,false);
	SetParameterStructureValue<float>(dst,E_MS_RESTITUTION,src.restitution,false);
	SetParameterStructureValue<int>(dst,E_MS_XML_TYPE,src.xmlLinkID,false);
	SetParameterStructureValue<int>(dst,E_MS_FLAGS,src.flags,false);
	SetParameterStructureValue<VxVector>(dst,E_MS_ANIS,src.dirOfAnisotropy,false);

}

pMaterial pFactory::loadMaterial(const char* nodeName,int& error)
{
	pMaterial result;
	if(loadFrom(result,nodeName,getDefaultDocument()))
	{
		error = 0;
	}else
		error =1;

	return result;



}
bool pFactory::loadMaterial(pMaterial&dst,const char* nodeName/* = */)
{
	return loadFrom(dst,nodeName,getDefaultDocument());
}
bool pFactory::loadFrom(pMaterial &dst,const char *nodeName,const TiXmlDocument *doc)
{
	

	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if ( strlen(nodeName) && doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "material" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{	
						if(!strcmp( element->Attribute("name"),nodeName ) )
						{
							for (const TiXmlNode *node = element->FirstChild(); node; node = node->NextSibling() )
							{
								if ((node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(node->Value(),"settings")))
								{
									const TiXmlElement *sube = (const TiXmlElement*)node;
									double v;

									//////////////////////////////////////////////////////////////////////////
									int res = sube->QueryDoubleAttribute("DynamicFriction",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f)
										{
											dst.dynamicFriction = (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("StaticFriction",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											dst.staticFriction= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("Restitution",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											dst.restitution= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("DynamicFrictionV",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											dst.dynamicFrictionV= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("StaticFrictionV",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											dst.staticFrictionV= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									const char* dirOfAnisotropy  = NULL;
									dirOfAnisotropy  = sube->Attribute("DirOfAnisotropy");
									if (dirOfAnisotropy && strlen(dirOfAnisotropy))
									{
										VxVector vec  = _str2Vec(dirOfAnisotropy);
										dst.dirOfAnisotropy = vec;
										continue;
									}

									const char* flags  = NULL;
									flags  = sube->Attribute("Flags");
									if (flags && strlen(flags))
									{
										dst.flags = _str2MaterialFlag(flags);
										continue;
									}

									//////////////////////////////////////////////////////////////////////////
									const char* FrictionCombineMode  = NULL;
									FrictionCombineMode = sube->Attribute("FrictionCombineMode");
									if (FrictionCombineMode && strlen(FrictionCombineMode))
									{
										int fMode  = _str2CombineMode(FrictionCombineMode);
										dst.frictionCombineMode = (CombineMode)fMode;
										continue;
									}

									//////////////////////////////////////////////////////////////////////////
									const char* RestitutionCombineMode  = NULL;
									RestitutionCombineMode = sube->Attribute("RestitutionCombineMode");
									if (RestitutionCombineMode && strlen(RestitutionCombineMode))
									{
										int fMode  = _str2CombineMode(RestitutionCombineMode);
										dst.restitutionCombineMode= (CombineMode)fMode;
										continue;
									}
								}
							}	
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

NxMaterialDesc*pFactory::createMaterialFromXML(const char* nodeName/* = */,const TiXmlDocument * doc /* = NULL */)
{

	
	
	NxMaterialDesc *result  = new NxMaterialDesc();
	result->setToDefault();

	if (doc ==NULL)
		return result;

	if (!getFirstDocElement(doc->RootElement()))
		return result;


	/************************************************************************/
	/* try to load settings from xml :                                                                      */
	/************************************************************************/
	if ( strlen(nodeName) && doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "material" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{	
						if(!strcmp( element->Attribute("name"),nodeName ) )
						{
							for (const TiXmlNode *node = element->FirstChild(); node; node = node->NextSibling() )
							{
								if ((node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(node->Value(),"settings")))
								{
									const TiXmlElement *sube = (const TiXmlElement*)node;
									double v;

									//////////////////////////////////////////////////////////////////////////
									int res = sube->QueryDoubleAttribute("DynamicFriction",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f)
										{
											result->dynamicFriction = (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("StaticFriction",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											result->staticFriction= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("Restitution",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											result->restitution= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("DynamicFrictionV",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											result->dynamicFrictionV= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									res = sube->QueryDoubleAttribute("StaticFrictionV",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f){
											result->staticFrictionV= (float)v;
											continue;
										}
									}
									//////////////////////////////////////////////////////////////////////////
									const char* dirOfAnisotropy  = NULL;
									dirOfAnisotropy  = sube->Attribute("DirOfAnisotropy");
									if (dirOfAnisotropy && strlen(dirOfAnisotropy))
									{
										VxVector vec  = _str2Vec(dirOfAnisotropy);
										if (vec.Magnitude() >0.1f)
										{
											result->flags = NX_MF_ANISOTROPIC;
											result->dirOfAnisotropy = pMath::getFrom(vec);
											continue;
										}else
										{
											result->dirOfAnisotropy = pMath::getFrom(VxVector(0,0,0));
										}

										//result->setGravity(vec);

									}

									//////////////////////////////////////////////////////////////////////////
									const char* FrictionCombineMode  = NULL;
									FrictionCombineMode = sube->Attribute("FrictionCombineMode");
									if (FrictionCombineMode && strlen(FrictionCombineMode))
									{
										int fMode  = _str2CombineMode(FrictionCombineMode);
										result->frictionCombineMode = (NxCombineMode)fMode;
										continue;
									}

									//////////////////////////////////////////////////////////////////////////
									const char* RestitutionCombineMode  = NULL;
									RestitutionCombineMode = sube->Attribute("RestitutionCombineMode");
									if (RestitutionCombineMode && strlen(RestitutionCombineMode))
									{
										int fMode  = _str2CombineMode(RestitutionCombineMode);
										result->restitutionCombineMode= (NxCombineMode)fMode;
										continue;
									}
								}
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