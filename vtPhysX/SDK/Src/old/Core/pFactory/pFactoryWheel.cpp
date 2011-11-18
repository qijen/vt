#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "Stream.h"
#include "cooking.h"
#include "tinyxml.h"

#include <xDebugTools.h>


pWheel* pFactory::createWheel(CK3dEntity *bodyReference,CK3dEntity*srcReference,pWheelDescr wheelDescr,pConvexCylinderSettings convexCylinder,VxVector localPositionOffset)
{
	//----------------------------------------------------------------
	//
	// main objects
	//
	pRigidBody *body=  GetPMan()->getBody(bodyReference);
	pWheel *wheel = NULL;
	bool assertCondition = true;
	
	iAssertWR(bodyReference,"",assertCondition);
	iAssertWR(body,"",assertCondition);
	iAssertWR(srcReference,"",assertCondition);

	if (!assertCondition)
		return NULL;


	iAssertWR(wheelDescr.isValid(),wheelDescr.setToDefault(),assertCondition);

	//----------------------------------------------------------------
	//
	// adjust wheel description data
	//
	iAssertW(wheelDescr.radius.isValid(),wheelDescr.radius.evaluate(srcReference),"");
	if(!wheelDescr.radius.isValid())
		wheelDescr.radius.value = srcReference->GetRadius();

	

	pObjectDescr objectDescription;
	objectDescription.hullType = HT_Wheel;
	objectDescription.density = 1.0f;
	
	objectDescription.flags = (BodyFlags)(BF_SubShape|BF_Collision);
	
	objectDescription.setWheel(wheelDescr);
	objectDescription.mask|=OD_Wheel;
	

	if(body->addCollider(objectDescription,srcReference))
	{

		NxShape*wheelShape = body->getSubShape(srcReference);
		//----------------------------------------------------------------
		//
		// handle wheel types 
		//
		//if( wheelShape && wheelShape->isWheel() && wheelDescr.wheelFlags & WF_UseWheelShape)
		//WF_UseWheelShape
		if( wheelShape && wheelShape->isWheel() )
		{
			wheel = (pWheel*)createWheel(body,wheelDescr);
			((pWheel2*)wheel)->setWheelShape((NxWheelShape*)wheelShape);
			wheel->setEntID(srcReference->GetID());
			((pSubMeshInfo*)(wheelShape->userData))->wheel  = wheel;
			wheel->setFlags(wheelDescr.wheelFlags);
		}
	}

	return wheel;

}

NxShape*pFactory::createWheelShape2(CK3dEntity *bodyReference,CK3dEntity*wheelReference,pWheelDescr wheelDescr)
{
	

	NxWheelShape *result = NULL;
	bool assertResult = true;
	pRigidBody *body=GetPMan()->getBody(bodyReference);
	
	iAssertWR(bodyReference && wheelReference && wheelDescr.isValid() && body ,"",assertResult);
	if (!assertResult)
		return NULL;

	CKMesh *mesh = wheelReference->GetCurrentMesh();
	VxVector box_s = mesh->GetLocalBox().GetSize();
	float radius = wheelDescr.radius.value > 0.0f ? wheelDescr.radius.value : box_s.v[wheelDescr.radius.referenceAxis] * 0.5f;

	VxQuaternion quatOffset;
	VxVector posOffset;
	vtAgeia::calculateOffsets(bodyReference,wheelReference,quatOffset,posOffset);

	CK_ID srcID = mesh->GetID();


	NxWheelShapeDesc shape;
	shape.setToDefault();

	shape.radius = radius;
	shape.localPose.M = pMath::getFrom(quatOffset);
	shape.localPose.t = pMath::getFrom(posOffset);

	float heightModifier = (wheelDescr.wheelSuspension + radius ) / wheelDescr.wheelSuspension;
	shape.suspension.spring = wheelDescr.springRestitution*heightModifier;
	shape.suspension.damper = wheelDescr.springDamping * heightModifier;
	shape.suspension.targetValue = wheelDescr.springBias * heightModifier;
	shape.suspensionTravel = wheelDescr.wheelSuspension;
	shape.inverseWheelMass = wheelDescr.inverseWheelMass;


	const pTireFunction& latFunc  = wheelDescr.latFunc;
	const pTireFunction& longFunc  = wheelDescr.longFunc;


	NxTireFunctionDesc lngTFD;
	lngTFD.extremumSlip = longFunc.extremumSlip ;
	lngTFD.extremumValue = longFunc.extremumValue;
	lngTFD.asymptoteSlip = longFunc.asymptoteSlip;
	lngTFD.asymptoteValue = longFunc.asymptoteValue;
	lngTFD.stiffnessFactor = longFunc.stiffnessFactor;

	NxTireFunctionDesc latTFD;
	latTFD.extremumSlip = latFunc.extremumSlip ;
	latTFD.extremumValue = latFunc.extremumValue;
	latTFD.asymptoteSlip = latFunc.asymptoteSlip;
	latTFD.asymptoteValue = latFunc.asymptoteValue;
	latTFD.stiffnessFactor = latFunc.stiffnessFactor;


	shape.lateralTireForceFunction =latTFD;
	shape.longitudalTireForceFunction = lngTFD;

	shape.wheelFlags  =wheelDescr.wheelShapeFlags;
	
	result = (NxWheelShape*)body->getActor()->createShape(shape);


	return (NxShape*)result;

}

NxShape *pFactory::_createWheelShape1(NxActor *actor,pWheel1 *dstWheel,pObjectDescr *descr,pWheelDescr *wheelDescr,CK3dEntity*srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation)
{
	
	#ifdef _DEBUG
		assert(dstWheel);
		assert(descr);
		assert(wheelDescr);
		assert(srcReference || mesh );
	#endif // _DEBUG

	NxShape *result = NULL;


	//################################################################
	//
	// some setup data
	//
	NxQuat rot = pMath::getFrom(localRotation);
	NxVec3 pos = getFrom(localPos);
	CK_ID srcID = mesh->GetID();

	NxConvexShapeDesc shape;
	if (!_createConvexCylinder(&shape,srcReference))
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create convex cylinder mesh");

	shape.density  = descr->density;
	shape.localPose.t = pMath::getFrom(localPos);
	shape.localPose.M = rot;
	if (descr->skinWidth!=-1.0f)
		shape.skinWidth =  descr->skinWidth;
	

	//################################################################
	//
	// Create the convex shape : 
	//
	dstWheel->setWheelConvex(actor->createShape(shape)->isConvexMesh());
	
	//if (!_createConvexCylinder(shape,srcReference))
	//	xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create convex cylinder mesh");


	//################################################################
	//
	// Find the wheels cylinder description

	pConvexCylinderSettings cylDescr;
	findSettings(cylDescr,srcReference);
	cylDescr.radius.value *=0.5f; 

	//################################################################
	//
	// Create a joint spring for the suspension 
	//

	NxReal heightModifier = (wheelDescr->wheelSuspension + cylDescr.radius.value) / wheelDescr->wheelSuspension;
	if (wheelDescr->wheelSuspension < 1)
		heightModifier = 1.f / wheelDescr->wheelSuspension;

	NxSpringDesc wheelSpring;
	wheelSpring.spring					= wheelDescr->springRestitution*heightModifier;
	wheelSpring.damper					= wheelDescr->springDamping*heightModifier;
	wheelSpring.targetValue				= wheelDescr->springBias*heightModifier;


	//################################################################
	//
	// The original code creates a material here ! We skip this !
	//

			//#########################
	

	//################################################################
	//
	// The wheel capsule is perpendicular to the floor
	//

	NxVec3 forwardAxis = getFrom(cylDescr.forwardAxis);
	NxVec3 downAxis = getFrom(cylDescr.downAxis);
	NxVec3 wheelAxis = getFrom(cylDescr.rightAxis);


	NxMaterialDesc materialDesc;
	materialDesc.restitution			= 0.0f;
	materialDesc.dynamicFriction		= wheelDescr->frictionToSide;
	materialDesc.staticFriction			= 2.0f;
	materialDesc.staticFrictionV		= wheelDescr->frictionToFront*4;
	materialDesc.dynamicFrictionV		= wheelDescr->frictionToFront;
	materialDesc.dirOfAnisotropy		= forwardAxis;
	materialDesc.frictionCombineMode	= NX_CM_MULTIPLY;
	materialDesc.flags					|=  NX_MF_ANISOTROPIC;

	

	dstWheel->material = actor->getScene().createMaterial(materialDesc);

	NxCapsuleShapeDesc capsuleShape;
	capsuleShape.radius = cylDescr.radius.value * 0.1f;
	capsuleShape.height = wheelDescr->wheelSuspension + cylDescr.radius.value;
	capsuleShape.flags = NX_SWEPT_SHAPE;

	//capsuleShape.localPose.M.setColumn(0, NxVec3( 1, 0, 0));
	//capsuleShape.localPose.M.setColumn(1, NxVec3( 0,-1, 0));
	//capsuleShape.localPose.M.setColumn(2, NxVec3( 0, 0,-1));	//rotate 180 degrees around x axis to cast downward!

	capsuleShape.materialIndex = dstWheel->material->getMaterialIndex();


	capsuleShape.localPose.M.setColumn(0, forwardAxis);
	capsuleShape.localPose.M.setColumn(1, downAxis);
	capsuleShape.localPose.M.setColumn(2, wheelAxis);
	if(wheelDescr->wheelSuspension >= 1) 
	{
		capsuleShape.localPose.t = pos + downAxis*(cylDescr.radius.value);
	}
	else 
	{
		capsuleShape.localPose.t = pos + (-1.0f *downAxis)*((cylDescr.radius.value + wheelDescr->wheelSuspension)*0.5f);
	}

	//################################################################
	//
	// Finalize 
	//
	result = dstWheel->getWheelConvex();
	if (!capsuleShape.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Capsule shape description during wheel1 construction was invalid");
	}
	
	dstWheel->setWheelCapsule(actor->createShape(capsuleShape)->isCapsule());
	


	dstWheel->_radius = cylDescr.radius.value;
	dstWheel->_turnAngle = 0;
	dstWheel->_turnVelocity = 0;
	dstWheel->_perimeter = dstWheel->_radius * NxTwoPi;
	dstWheel->_maxSuspension = wheelDescr->wheelSuspension;
	dstWheel->_wheelWidth = cylDescr.height.value;
	dstWheel->_maxPosition = localPos;
	
	dstWheel->_frictionToFront = wheelDescr->frictionToFront;
	dstWheel->_frictionToSide = wheelDescr->frictionToSide;


	NxU32 contactReportFlags = actor->getContactReportFlags();
	contactReportFlags |=NX_NOTIFY_ON_TOUCH;
	actor->setContactReportFlags(contactReportFlags);
	return dstWheel->getWheelCapsule();

}


pWheel *pFactory::createWheelSubShape(pRigidBody *body,CK3dEntity* subEntity,CKMesh *mesh,pObjectDescr *descr,VxVector localPos, VxQuaternion localRotation,NxShape*dstShape)
{
	//################################################################
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(body && subEntity && descr );	// Should never occur ! 
	#endif // _DEBUG


	XString errorStr;

	//################################################################
	//
	// Retrieve the wheel setting from attribute 
	//
	int attTypeWheelSettings = GetPMan()->att_wheelDescr;
	int attTypeConvexCylinderSettings  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR);
	
	if (!subEntity->HasAttribute(attTypeWheelSettings))
	{
		errorStr.Format("Object %s has been set as wheel but there is no wheel attribute attached",subEntity->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorStr.CStr());
		return NULL;
	}


	pWheelDescr *wDescr  = new pWheelDescr();
	CKParameterOut *par = subEntity->GetAttributeParameter(attTypeWheelSettings);
	if (par)
	{
		
		int err = copyTo(wDescr,par);
		if (!wDescr->isValid() )
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Wheel Description is invalid");
			SAFE_DELETE(wDescr);
			return NULL;
		}
	}

	//################################################################
	//
	// Construct the final wheel object basing on the type of the wheel
	//

	pWheel *result  = NULL;


	//if(wDescr->wheelFlags & WF_UseWheelShape)	{
	
	result = new pWheel2(body,wDescr);
	
	/*else
	{
		//################################################################
		// Wheel type 1 specified, check there is also a override for the convex cylinder 
		if (!subEntity->HasAttribute(attTypeConvexCylinderSettings))
		{
			errorStr.Format("Object %s has been created with default settings for convex cylinder shape",subEntity->GetName());
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,errorStr.CStr());
		}
		result = new pWheel1(body,wDescr);
	}*/

	//################################################################
	//
	// Create the wheel shape 
	//
	


	//if(wDescr->wheelFlags & WF_UseWheelShape){
	
	dstShape=_createWheelShape2(body->getActor(),descr,wDescr,subEntity,mesh,localPos,localRotation);
	((pWheel2*)result)->setWheelShape((NxWheelShape*)dstShape);
//	}
	/*else
	{
		dstShape=_createWheelShape1(body->getActor(),(pWheel1*)result,descr,wDescr,subEntity,mesh,localPos,localRotation);
	}*/

	if (!dstShape)
	{
		errorStr.Format("Couldn't create wheel shape for object %s",subEntity->GetName());
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,errorStr.CStr());
		
		SAFE_DELETE(wDescr);
		SAFE_DELETE(result);
		return NULL;
	}


	//################################################################
	//
	// Finalize wheel setup
	//

	result->setEntID(subEntity->GetID());


	return result;
}


pTireFunction pFactory::createTireFuncFromParameter(CKParameter *par)
{
	pTireFunction result;
	result.setToDefault();

	if (!par)
	{
		return result;
	}
	result.extremumSlip = vtTools::ParameterTools::GetValueFromParameterStruct<float>(par,1);
	result.extremumValue = vtTools::ParameterTools::GetValueFromParameterStruct<float>(par,2);
	result.asymptoteSlip = vtTools::ParameterTools::GetValueFromParameterStruct<float>(par,3);
	result.asymptoteValue= vtTools::ParameterTools::GetValueFromParameterStruct<float>(par,4);
	result.stiffnessFactor = vtTools::ParameterTools::GetValueFromParameterStruct<float>(par,5);

	/************************************************************************/
	/* Lat Tire Func from XML ?                                                                      */
	/************************************************************************/
	int xmlLinkId = vtTools::ParameterTools::GetValueFromParameterStruct<int>(par,0);
	if (xmlLinkId!=0)
	{
		XString nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_TIRE_SETTINGS,xmlLinkId );
		loadFrom(result,nodeName.CStr(),getDefaultDocument());
		if (!result.isValid())
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Latitude Tire Function was incorrect, setting to default");
			result.setToDefault();
		}else{
			copyTo((CKParameterOut*)par,result);
		}
	}


	if (!result.isValid())
	{
		result.setToDefault();
	}
	return result;
}

int pFactory::copyTo(pWheelDescr *dst,CKParameter *src)
{

	int result  = 1;
	if (!src || !dst)
	{
		return NULL;
	}

	using namespace vtTools::ParameterTools;
	dst->setToDefault();


	dst->wheelSuspension = GetValueFromParameterStruct<float>(src,E_WD_SUSPENSION);
	dst->springRestitution= GetValueFromParameterStruct<float>(src,E_WD_SPRING_RES);
	dst->springBias = GetValueFromParameterStruct<float>(src,E_WD_SPRING_BIAS);
	dst->springDamping= GetValueFromParameterStruct<float>(src,E_WD_DAMP);

	dst->maxBrakeForce= GetValueFromParameterStruct<float>(src,E_WD_MAX_BFORCE);
	dst->frictionToSide= GetValueFromParameterStruct<float>(src,E_WD_FSIDE);
	dst->frictionToFront= GetValueFromParameterStruct<float>(src,E_WD_FFRONT);
	
	CKParameterOut *pOld  = GetParameterFromStruct(src,E_WD_INVERSE_WHEEL_MASS);
	if (pOld)
	{
		if (pOld->GetGUID()  == CKPGUID_FLOAT)
		{
			dst->inverseWheelMass= GetValueFromParameterStruct<float>(src,E_WD_INVERSE_WHEEL_MASS);
		}

		if (pOld->GetGUID()  == CKPGUID_INT)
		{
			dst->wheelApproximation= GetValueFromParameterStruct<int>(src,E_WD_INVERSE_WHEEL_MASS);
		}
	}

	//dst->wheelApproximation= GetValueFromParameterStruct<int>(float,E_WD_INVERSE_WHEEL_MASS);

	dst->wheelFlags= (WheelFlags)GetValueFromParameterStruct<int>(src,E_WD_FLAGS);
	dst->wheelShapeFlags=(WheelShapeFlags) GetValueFromParameterStruct<int>(src,E_WD_SFLAGS);


	CKParameterOut *parLatFunc = GetParameterFromStruct(src,E_WD_LAT_FUNC);
	CKParameterOut *parLongFunc = GetParameterFromStruct(src,E_WD_LONG_FUNC);



	/************************************************************************/
	/* XML Setup ?                                                                     */
	/************************************************************************/
	int xmlLinkId= GetValueFromParameterStruct<int>(src,E_WD_XML);
	bool wIsXML=false;
	bool latIsXML= false;
	bool longIsXML=false;

	XString nodeName;
	if ( xmlLinkId !=0 )
	{
		nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_WHEEL_SETTINGS,xmlLinkId);
		loadWheelDescrFromXML(*dst,nodeName.CStr(),getDefaultDocument());
		wIsXML =true;
		if (!dst->isValid())
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Wheel Description was invalid");
		}


		if (dst->latFunc.xmlLink!=0)
		{
			latIsXML=true;
		}

		if (dst->longFunc.xmlLink!=0)
		{
			longIsXML=true;
		}
	}



	if (!latIsXML)
	{
		dst->latFunc = createTireFuncFromParameter(parLatFunc);
	}


	if (!longIsXML)
	{
		dst->longFunc= createTireFuncFromParameter(parLongFunc);
	}

	if (wIsXML)
	{
		copyTo((CKParameterOut*)src,dst);
	}

	
	if (longIsXML)
	{
		copyTo(GetParameterFromStruct(src,E_WD_LONG_FUNC),dst->longFunc);
	}

	if (latIsXML)
	{
		copyTo(GetParameterFromStruct(src,E_WD_LAT_FUNC),dst->latFunc);
	}
	
	return result;
}






NxShape *pFactory::_createWheelShape2(NxActor *actor, pObjectDescr *descr, pWheelDescr *wheelDescr, CK3dEntity*srcReference,CKMesh *mesh, VxVector localPos, VxQuaternion localRotation)
{

	NxWheelShape *result = NULL;

	if (!actor || !descr || !mesh )
	{
		return result;
	}

	int hType = descr->hullType;
	VxVector box_s = mesh->GetLocalBox().GetSize();
	float density  = descr->density;
	float skinWidth   = descr->skinWidth;
	float radius = box_s.x*0.5f;
	NxQuat rot = pMath::getFrom(localRotation);

	CK_ID srcID = mesh->GetID();


	NxWheelShapeDesc shape;
	shape.setToDefault();

	shape.radius = box_s.z*0.5f;
	shape.density  = density;
	shape.localPose.M = rot;
	shape.localPose.t = pMath::getFrom(localPos);
	if (skinWidth!=-1.0f)
		shape.skinWidth = skinWidth;

	float heightModifier = (wheelDescr->wheelSuspension + radius ) / wheelDescr->wheelSuspension;
	shape.suspension.spring = wheelDescr->springRestitution*heightModifier;
	shape.suspension.damper = wheelDescr->springDamping * heightModifier;
	shape.suspension.targetValue = wheelDescr->springBias * heightModifier;
	shape.suspensionTravel = wheelDescr->wheelSuspension;

	//shape.lateralTireForceFunction.stiffnessFactor *= wheelDescr->frictionToSide;
	//shape.longitudalTireForceFunction.stiffnessFactor*=wheelDescr->frictionToFront;
	shape.inverseWheelMass = wheelDescr->inverseWheelMass;

	
	const pTireFunction& latFunc  = wheelDescr->latFunc;
	const pTireFunction& longFunc  = wheelDescr->longFunc;


	NxTireFunctionDesc lngTFD;
	lngTFD.extremumSlip = longFunc.extremumSlip ;
	lngTFD.extremumValue = longFunc.extremumValue;
	lngTFD.asymptoteSlip = longFunc.asymptoteSlip;
	lngTFD.asymptoteValue = longFunc.asymptoteValue;
	lngTFD.stiffnessFactor = longFunc.stiffnessFactor;

	NxTireFunctionDesc latTFD;
	latTFD.extremumSlip = latFunc.extremumSlip ;
	latTFD.extremumValue = latFunc.extremumValue;
	latTFD.asymptoteSlip = latFunc.asymptoteSlip;
	latTFD.asymptoteValue = latFunc.asymptoteValue;
	latTFD.stiffnessFactor = latFunc.stiffnessFactor;


	shape.lateralTireForceFunction =latTFD;
	shape.longitudalTireForceFunction = lngTFD;
    
	//wshape->setWheelFlags;
	//shape.wheelFlags = wheelDescr->wheelFlags;
	//shape.wheelFlags |= (NxWheelShapeFlags(NX_WF_WHEEL_AXIS_CONTACT_NORMAL));
	//shape.shapeFlags  = wheelDescr->wheelShapeFlags;
	shape.wheelFlags  =wheelDescr->wheelShapeFlags;

	/*
	if (wheelDescr->wheelFlags &  (WF_Accelerated|WF_UseWheelShape) )
	{
		int isValid = shape.isValid();

	}
	*/
	


	int isValid = shape.isValid();
	result = (NxWheelShape*)actor->createShape(shape);

	if (result)
	{
		

		//result->setWheelFlags(wheelDescr->wheelFlags);
		//pWheel::getWheelFlag()


		/*		if ( &  E_WF_VEHICLE_CONTROLLED )
		{
		int isValid = shape.isValid();

		}
		*/
	}



	return (NxShape*)result;


}

NxShape *pFactory::createWheelShape(NxActor *actor, pObjectDescr *descr, pWheelDescr *wheelDescr, CK3dEntity*srcReference,CKMesh *mesh, VxVector localPos, VxQuaternion localRotation)
{

//	if(wheelDescr->wheelFlags & WF_UseWheelShape){
		
	return _createWheelShape2(actor,descr,wheelDescr,srcReference,mesh,localPos,localRotation);
	
	/*}else
		return NULL;//_createWheelShape1(actor,descr,wheelDescr,srcReference,mesh,localPos,localRotation);
*/
	return NULL;



}


pWheel*pFactory::createWheel(pRigidBody *body, pWheelDescr descr)
{

	pWheel *result  = NULL;
	if (!body || !body->isValid() )
	{
		return result;
	}

	//if(descr.wheelFlags & WF_UseWheelShape)	{
	
	result = new pWheel2(body,&descr);
	
/*	}else
	{
		result = new pWheel1(body,&descr);
	}
*/

	if (result)
	{
		result->setFlags(descr.wheelFlags);
	}//
	return result;
}

XString pFactory::_getVehicleTireFunctionAsEnumeration(const TiXmlDocument * doc)
{

	if (!doc)
	{
		return XString("");
	}

	XString result("None=0");
	int counter = 1;


	/************************************************************************/
	/* */
	/************************************************************************/
	if ( doc)
	{
		const TiXmlElement *root = getFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();
				if (!strcmp(child->Value(), "tireFunction" ) )
				{

					const TiXmlElement *sube = (const TiXmlElement*)child;

					const char* vSName  = NULL;
					vSName = sube->Attribute("name");
					if (vSName && strlen(vSName))
					{
						if (result.Length())
						{
							result << ",";
						}
						result << vSName;
						result << "=" << counter;
						counter ++;
					}
				}
			}
		}
	}

	return result;
}



int pFactory::copyTo(pWheelDescr &dst,CKParameterOut *src)
{
	if (!src)return false;
	using namespace vtTools::ParameterTools;

	int result = 0;

	//SetParameterStructureValue<float>(dst,E_WD_SPRING_BIAS,src.,false);

	return result;


}

int pFactory::copyTo(CKParameterOut *dst,const pTireFunction& src)
{

	if (!dst)return false;
	using namespace vtTools::ParameterTools;

	int result = 0;

	SetParameterStructureValue<int>(dst,0,src.xmlLink,false);
	SetParameterStructureValue<float>(dst,1,src.extremumSlip,false);
	SetParameterStructureValue<float>(dst,2,src.extremumValue,false);
	SetParameterStructureValue<float>(dst,3,src.asymptoteSlip,false);
	SetParameterStructureValue<float>(dst,4,src.asymptoteValue,false);
	SetParameterStructureValue<float>(dst,5,src.stiffnessFactor,false);


	return result;

}

int pFactory::copyTo(CKParameterOut *dst,pWheelDescr *src)
{

	if (!src)return false;
	using namespace vtTools::ParameterTools;

	int result = 0;

	SetParameterStructureValue<float>(dst,E_WD_SPRING_BIAS,src->springBias,false);
	SetParameterStructureValue<float>(dst,E_WD_SPRING_RES,src->springRestitution,false);
	SetParameterStructureValue<float>(dst,E_WD_DAMP,src->springDamping,false);
	SetParameterStructureValue<float>(dst,E_WD_MAX_BFORCE,src->maxBrakeForce,false);
	SetParameterStructureValue<float>(dst,E_WD_FFRONT,src->frictionToFront,false);
	SetParameterStructureValue<float>(dst,E_WD_FSIDE,src->frictionToSide,false);
	SetParameterStructureValue<int>(dst,E_WD_FLAGS,src->wheelFlags,false);
	SetParameterStructureValue<int>(dst,E_WD_SFLAGS,src->wheelShapeFlags,false);
	SetParameterStructureValue<float>(dst,E_WD_INVERSE_WHEEL_MASS,src->inverseWheelMass,false);


	return result;

}

int pFactory::copyTo(CKParameterOut *dst,const pWheelContactData& src)
{

	if (!dst)return false;
	using namespace vtTools::ParameterTools;

	int result = 0;

	SetParameterStructureValue<VxVector>(dst,E_WCD_CPOINT,src.contactPoint,false);
	SetParameterStructureValue<VxVector>(dst,E_WCD_CNORMAL,src.contactNormal,false);
	SetParameterStructureValue<VxVector>(dst,E_WCD_LONG_DIR,src.longitudalDirection,false);
	SetParameterStructureValue<VxVector>(dst,E_WCD_LAT_DIR,src.lateralDirection,false);

	SetParameterStructureValue<float>(dst,E_WCD_CONTACT_FORCE,src.contactForce,false);

	SetParameterStructureValue<float>(dst,E_WCD_LONG_SLIP,src.longitudalSlip,false);
	SetParameterStructureValue<float>(dst,E_WCD_LAT_SLIP,src.longitudalSlip,false);

	SetParameterStructureValue<float>(dst,E_WCD_LONG_IMPULSE,src.longitudalImpulse,false);
	SetParameterStructureValue<float>(dst,E_WCD_LAT_IMPULSE,src.longitudalImpulse,false);

	SetParameterStructureValue<float>(dst,E_WCD_C_POS,src.contactPosition,false);

	if (src.contactEntity)
	{
		SetParameterStructureValue<CK_ID>(dst,E_WCD_CONTACT_ENTITY,src.contactEntity->GetID(),false);
	}

	CKParameter *materialParameter =  vtTools::ParameterTools::GetParameterFromStruct(dst,E_WCD_OTHER_MATERIAL_INDEX,false);
	pFactory::Instance()->copyTo((CKParameterOut*)materialParameter,src.otherMaterial);


	return result;

}


bool pFactory::loadFrom(pWheelDescr& dst,const char* nodeName)
{
	return loadWheelDescrFromXML(dst,nodeName,getDefaultDocument());

}