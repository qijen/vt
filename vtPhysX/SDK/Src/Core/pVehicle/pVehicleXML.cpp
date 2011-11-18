#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "tinyxml.h"

int pFactory::loadFrom(pTireFunction& dst,const char* nodeName,const TiXmlDocument * doc )
{
	int result = 0 ;
	if (!strlen(nodeName))	{	return -1;	}
	if (!doc)	{ return -1;	}
	const TiXmlElement *root = pFactory::Instance()->getFirstDocElement(doc->RootElement());
	for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	{
		if ( (child->Type() == TiXmlNode::ELEMENT))
		{
			XString name = child->Value();
			if (!strcmp(child->Value(), "tireFunction" ) )
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

								int res = sube->QueryDoubleAttribute("ExtremumSlip",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.extremumSlip  = static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("ExtremumValue",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.extremumValue  = static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("AsymptoteSlip",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.asymptoteSlip  = static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("AsymptoteValue",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.asymptoteValue  = static_cast<float>(v);
									}
								}
								res = sube->QueryDoubleAttribute("StiffnessFactor",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.stiffnessFactor  = static_cast<float>(v);
									}
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

int pFactory::loadWheelDescrFromXML(pWheelDescr& dst,const char* nodeName,const TiXmlDocument * doc )
{
	int result = 0 ;
	if (!strlen(nodeName))	{	return -1;	}
	if (!doc)	{ return -1;	}
	const TiXmlElement *root = pFactory::Instance()->getFirstDocElement(doc->RootElement());
	for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	{
		if ( (child->Type() == TiXmlNode::ELEMENT))
		{
			XString name = child->Value();
			if (!strcmp(child->Value(), "wheel" ) )
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

								int res = sube->QueryDoubleAttribute("Suspension",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.wheelSuspension  = static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("Restitution",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.springRestitution  = static_cast<float>(v);
									}
								}
								
								res = sube->QueryDoubleAttribute("Damping",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.springDamping  = static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("Bias",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.springBias= static_cast<float>(v);
									}
								}

								res = sube->QueryDoubleAttribute("MaxBreakForce",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.maxBrakeForce  = static_cast<float>(v);
										continue;
									}
								}
								
							
								res = sube->QueryDoubleAttribute("FrictionToSide",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.frictionToSide  = static_cast<float>(v);
										continue;
									}
								}

								res = sube->QueryDoubleAttribute("FrictionToFront",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.frictionToFront  = static_cast<float>(v);
										continue;
									}
								}

								res = sube->QueryDoubleAttribute("InverseWheelMass",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.inverseWheelMass  = static_cast<float>(v);
										continue;
									}
								}

								const char* flags  = NULL;
								flags  = sube->Attribute("Flags");
								if (flags && strlen(flags))
								{
									dst.wheelFlags  = (WheelFlags)_str2WheelFlag(flags);
								}
								
								const char* sflags  = NULL;
								sflags  = sube->Attribute("ShapeFlags");
								if (sflags && strlen(sflags))
								{
									dst.wheelShapeFlags  = (WheelShapeFlags)_str2WheelShapeFlag(sflags);
								}


								const char* latFunc  = NULL;
								latFunc  = sube->Attribute("LateralFunction");
								if (latFunc && strlen(latFunc))
								{
									int index  = getEnumIndex(getManager()->GetContext()->GetParameterManager(),VTE_XML_TIRE_SETTINGS,latFunc);
									if (index!=0)
									{
										loadFrom(dst.latFunc,latFunc,doc);
										dst.latFunc.xmlLink = index;
										if (!dst.latFunc.isValid())
										{
											xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Lateral Tire Function from XML was incorrect, setting to default");
											dst.latFunc.setToDefault();
										}
									}
								}

								const char* longFunc  = NULL;
								longFunc  = sube->Attribute("LongitudeFunction");
								if (longFunc && strlen(longFunc))
								{
									int index  = getEnumIndex(getManager()->GetContext()->GetParameterManager(),VTE_XML_TIRE_SETTINGS,longFunc);
									if (index!=0)
									{
										loadFrom(dst.longFunc,longFunc,doc);
										dst.longFunc.xmlLink = index;
										if (!dst.longFunc.isValid())
										{
											xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Longitude Tire Function from XML was incorrect, setting to default");
											dst.longFunc.setToDefault();
										}
									}
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
int pFactory::loadVehicleDescrFromXML(pVehicleDesc& dst,const char* nodeName,const TiXmlDocument * doc )
{
	
	
	int result = 0 ;
	if (!strlen(nodeName))	{	return -1;	}
	if (!doc)	{ return -1;	}
	const TiXmlElement *root = pFactory::Instance()->getFirstDocElement(doc->RootElement());
	for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	{
		if ( (child->Type() == TiXmlNode::ELEMENT))
		{
			XString name = child->Value();
			if (!strcmp(child->Value(), "vehicle" ) )
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

								int res = sube->QueryDoubleAttribute("Mass",&v);
								if (res == TIXML_SUCCESS)
								{
									if(v >=0.0f)
									{
										dst.mass  = static_cast<float>(v);
										continue;
									}
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

int pFactory::_str2PhysicFlags(XString _in)
{


	short nb = 0 ; 
	int result = 0;
	XStringTokenizer	tokizer(_in.CStr(), "|");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) )
	{
		XString tokx(tok);

		if ( _stricmp(tokx.CStr(),"DisableHardware") == 0 ) 
		{	
			result |= PMF_DONT_USE_HARDWARE;
		}

		if ( _stricmp(tokx.CStr(),"DoNotDeleteScenes") == 0 ) 
		{	
			result |= PMF_DONT_DELETE_SCENES;
		}
		nb++;
	}
	return result;
}

int pFactory::_str2SceneFlags(XString _in)
{


	short nb = 0 ; 
	int result = 0;
	XStringTokenizer	tokizer(_in.CStr(), "|");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) )
	{
		XString tokx(tok);

		if ( _stricmp(tokx.CStr(),"DisableSSE") == 0 ) 
		{	
			result |= NX_SF_DISABLE_SSE;
		}

		if ( _stricmp(tokx.CStr(),"DisableCollision") == 0 ) 
		{	
			result |= NX_SF_DISABLE_COLLISIONS;
		}
		if ( _stricmp(tokx.CStr(),"SimulateSeperateThread") == 0 ) 
		{	
			result |= NX_SF_SIMULATE_SEPARATE_THREAD;
		}
		if ( _stricmp(tokx.CStr(),"EnableMultiThread") == 0 ) 
		{	
			result |= NX_SF_ENABLE_MULTITHREAD;
		}

		if ( _stricmp(tokx.CStr(),"EnableActiveTransforms") == 0 ) 
		{	
			result |= NX_SF_ENABLE_ACTIVETRANSFORMS;
		}

		if ( _stricmp(tokx.CStr(),"RestrictedScene") == 0 ) 
		{	
			result |= NX_SF_RESTRICTED_SCENE;
		}
		if ( _stricmp(tokx.CStr(),"DisableSceneMutex") == 0 ) 
		{	
			result |= NX_SF_DISABLE_SCENE_MUTEX;
		}

		if ( _stricmp(tokx.CStr(),"ForceConeFriction") == 0 ) 
		{	
			result |= NX_SF_FORCE_CONE_FRICTION;
		}

		if ( _stricmp(tokx.CStr(),"DisableSceneMutex") == 0 ) 
		{	
			result |= NX_SF_SEQUENTIAL_PRIMARY;
		}
		
		/*
		if ( _stricmp(tokx.CStr(),"DisableHardware") == 0 ) 
		{	
			result |= NX_SF_SEQUENTIAL_PRIMARY;
		}*/
		
		nb++;
	}
	return result;
}

int pFactory::_str2WheelFlag(XString _in)
{

	
	short nb = 0 ; 
	int result = 0;
	XStringTokenizer	tokizer(_in.CStr(), "|");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) )
	{
		XString tokx(tok);

		if ( _stricmp(tokx.CStr(),"Steerable") == 0 ) 
		{	
			result |= WF_SteerableInput;
		}
		
		if ( _stricmp(tokx.CStr(),"VehicleControlled") == 0 ) 
		{	
			result |= WF_VehicleControlled;
		}
		if ( _stricmp(tokx.CStr(),"SteerableAuto") == 0 ) 
		{	
			result |= WF_SteerableAuto;
		}
		if ( _stricmp(tokx.CStr(),"Handbrake") == 0 ) 
		{	
			result |= WF_AffectedByHandbrake;
		}
		
		if ( _stricmp(tokx.CStr(),"Accelerated") == 0 ) 
		{	
			result |= WF_Accelerated;
		}

		if ( _stricmp(tokx.CStr(),"Differential") == 0 ) 
		{	
			result |= WF_AffectedByDifferential;
		}
		if ( _stricmp(tokx.CStr(),"IgnoreTireFunction") == 0 ) 
		{	
			result |= WF_IgnoreTireFunction;
		}


		
		/*if ( _stricmp(tokx.CStr(),"Wheelshape") == 0 ) 
		{	
			result |= WF_UseWheelShape;
		}*/
		
		nb++;
	}
	return result;
}
int pFactory::_str2WheelShapeFlag(XString _in)
{


	short nb = 0 ; 
	int result = 0;
	XStringTokenizer	tokizer(_in.CStr(), "|");
	const char*tok = NULL;
	while ((tok=tokizer.NextToken(tok)) )
	{
		XString tokx(tok);

		if ( _stricmp(tokx.CStr(),"AxisContactNormal") == 0 ) 
		{	
			result |= WSF_AxleSpeedOverride;
		}

		if ( _stricmp(tokx.CStr(),"InputLateralSlip") == 0 ) 
		{	
			result |= WSF_InputLatSlipVelocity;
		}
		if ( _stricmp(tokx.CStr(),"InputLongitudinal") == 0 ) 
		{	
			result |= WSF_InputLongSlipVelocity;
		}
		if ( _stricmp(tokx.CStr(),"UnscaledSpringBehavior") == 0 ) 
		{	
			result |= WSF_UnscaledSpringBehavior;
		}

		if ( _stricmp(tokx.CStr(),"WheelAxisContactNormal") == 0 ) 
		{	
			result |= WSF_WheelAxisContactNormal;
		}

		if ( _stricmp(tokx.CStr(),"AxleSpeedOverride") == 0 ) 
		{	
			result |= WSF_AxleSpeedOverride;
		}

		if ( _stricmp(tokx.CStr(),"EmulateLegacyWheel") == 0 ) 
		{	
			result |= WSF_EmulateLegacyWheel;
		}
		if ( _stricmp(tokx.CStr(),"ClampedFriction") == 0 ) 
		{	
			result |= WSF_ClampedFriction;
		}
		nb++;
	}
	return result;
}

/*

int pVehicle::loadFromXML(const char* nodeName,const TiXmlDocument * doc )
{
	NxMaterialDesc *result  = new NxMaterialDesc();
	result->setToDefault();

	int res = 0;
	
	if (!strlen(nodeName))	{	return -1;	}
	if (!doc)	{ return -1;	}


	if ( strlen(nodeName) && doc)
	{
		const TiXmlElement *root = pFactory::Instance()->getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();
				if (!strcmp(child->Value(), "vehicle" ) )
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
									int res = sube->QueryDoubleAttribute("Mass",&v);
									if (res == TIXML_SUCCESS)
									{
										if(v >=0.0f)
										{
											//setMass() = 
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
										VxVector vec  = pFactory::Instance()->_str2Vec(dirOfAnisotropy);
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
//										int fMode  = _str2CombineMode(FrictionCombineMode);
//										result->frictionCombineMode = (NxCombineMode)fMode;
										continue;
									}

									//////////////////////////////////////////////////////////////////////////
									const char* RestitutionCombineMode  = NULL;
									RestitutionCombineMode = sube->Attribute("RestitutionCombineMode");
									if (RestitutionCombineMode && strlen(RestitutionCombineMode))
									{
//										int fMode  = _str2CombineMode(RestitutionCombineMode);
//										result->restitutionCombineMode= (NxCombineMode)fMode;
										continue;
									}
								}
							}	
//							return result;
						}
					}
				}
			}
		}
	}



	return 0;
}
*/



