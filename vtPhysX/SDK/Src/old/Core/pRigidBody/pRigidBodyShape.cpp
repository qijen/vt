#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "IParameter.h"

#include <xDebugTools.h>
int pRigidBody::addCollider(pObjectDescr objectDescr,CK3dEntity*srcRefEntity)
{

	int result = 0 ; 

	using namespace vtTools::AttributeTools;

	CK3dEntity* child = NULL;
	CKMesh *srcMesh = NULL;

	if (!srcRefEntity ||!srcRefEntity->GetCurrentMesh())
		return 0;
	bool isChild = vtAgeia::isChildOf(GetVT3DObject(), srcRefEntity);

	objectDescr.subEntID = srcRefEntity->GetID();
	//----------------------------------------------------------------
	//
	// essential values
	//
	VxVector box_s = BoxGetZero(srcRefEntity);
	
	float radius = 1.0f;
	radius  = srcRefEntity->GetCurrentMesh()->GetRadius();

	VxQuaternion refQuad;
	srcRefEntity->GetQuaternion(&refQuad,GetVT3DObject());
	VxVector relPos;
	srcRefEntity->GetPosition(&relPos,GetVT3DObject());
	
	NxQuat rot = pMath::getFrom(refQuad);

	srcMesh = srcRefEntity->GetCurrentMesh();

	pWheel *wheel =NULL;
	NxShape *shape = NULL;


	if (objectDescr.hullType != HT_Wheel)
	{
		shape = pFactory::Instance()->createShape(GetVT3DObject(),objectDescr,srcRefEntity,srcMesh,relPos,refQuad);
	}
	else
	{
		iAssertW( objectDescr.wheel.isValid(),objectDescr.wheel.setToDefault());
		iAssertW( objectDescr.wheel.radius.isValid(),objectDescr.wheel.radius.evaluate(srcRefEntity));
		shape = pFactory::Instance()->createWheelShape2(GetVT3DObject(),srcRefEntity, objectDescr.wheel );
	}

	if (!shape)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create sub shape!");
		return 0;
	}

	pSubMeshInfo *sInfo  = new pSubMeshInfo();

	sInfo->meshID = srcMesh->GetID();
	sInfo->mesh =(CKBeObject*)srcMesh;

	sInfo->entID = srcRefEntity->GetID();
	sInfo->refObject  = (CKBeObject*)srcRefEntity;
	sInfo->wheel = NULL;
	
	shape->setName(srcRefEntity->GetName());
	shape->userData = (void*)sInfo;
	sInfo->initDescription = objectDescr;

	getActor()->wakeUp();


	if ( (!objectDescr.flags &BF_Hierarchy) )
	{
		return result;
	}
	/*

	//################################################################
	//
	// If more entities in hierarchy, invoke this function recursively 
	//
	CK3dEntity* subEntity = NULL;
	while (subEntity= srcRefEntity->HierarchyParser(subEntity) )
	{

		pObjectDescr *subDescr  = NULL;

		//-try old version : 
		if (subEntity->HasAttribute(GetPMan()->GetPAttribute()))
		{
			subDescr = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
		}

		//-try new version
		int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
		if (subEntity->HasAttribute(attTypePBSetup))
		{
			subDescr  = new pObjectDescr();
			CKParameterOut *par =  subEntity->GetAttributeParameter(attTypePBSetup);
			IParameter::Instance()->copyTo(subDescr,par);
			subDescr->version  = pObjectDescr::E_OD_VERSION::OD_DECR_V1;

		}

		if (!subDescr)
			continue;

		if (subDescr->flags & BF_SubShape)
		{

			//////////////////////////////////////////////////////////////////////////
			if (subDescr->hullType != HT_Cloth)
			{
				addSubShape(NULL,*subDescr,subEntity);
			}

			if (subDescr->hullType == HT_Cloth)
			{

			}
		}
	}
	*/
	return 1;
}




int pRigidBody::addSubShape( CKMesh *mesh,pObjectDescr& objectDescr,CK3dEntity*srcRefEntity,VxVector localPosition,VxQuaternion localRotation)
{
	

	int result = 0 ; 

	int att = GetPMan()->GetPAttribute();
	int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
	bool isNewType  = ( objectDescr.version == pObjectDescr::E_OD_VERSION::OD_DECR_V1)  ? true : false ;

	
	using namespace vtTools::AttributeTools;



	CK3dEntity* child = NULL;
	bool isChild = false;
	while (child = GetVT3DObject()->HierarchyParser(child) )
	{
		if (child  == srcRefEntity )
		{
			isChild = true;
		}
	}

	/************************************************************************/
	/* parameters for final composition :                                                                     */
	/************************************************************************/
	CKMesh *srcMesh = NULL;
	float density = 0.0f;
	VxVector mOffset;
	VxVector sOffset;
	float skinWidth;
	int hType = 0;


	if (!mesh && !srcRefEntity )
	{
		return result;
	}
	if (!mesh && srcRefEntity && !srcRefEntity->GetCurrentMesh() )
	{
		return result;
	}

	CKBeObject *attObject = NULL;
	if (mesh && (mesh->HasAttribute(att) || isNewType) )
	{
		attObject = (CKBeObject*)mesh;
	}
	if (srcRefEntity && (srcRefEntity->HasAttribute(att) || isNewType ) )
	{
		attObject = (CKBeObject*)srcRefEntity;
	}


	//################################################################
	//
	// Fill sub shapes object description. 
	//
	
	if (attObject && !isNewType ) //we have attribute values:
	{
		objectDescr.density  = GetValueFromAttribute<float>(attObject ,att, E_PPS_DENSITY);
		objectDescr.massOffset= GetValueFromAttribute<VxVector>(attObject ,att, E_PPS_MASS_OFFSET);
		objectDescr.shapeOffset =  GetValueFromAttribute<VxVector>(attObject ,att, E_PPS_SHAPE_OFFSET);
		objectDescr.skinWidth  = GetValueFromAttribute<float>(attObject,att, E_PPS_SKIN_WIDTH);
		objectDescr.hullType = GetValueFromAttribute<HullType>(attObject,att,E_PPS_HULLTYPE);
		objectDescr.hirarchy = GetValueFromAttribute<int>(attObject,att,E_PPS_HIRARCHY);
		objectDescr.collisionGroup = GetValueFromAttribute<int>(attObject,att,E_PPS_COLL_GROUP);
		objectDescr.newDensity  = GetValueFromAttribute<int>(attObject,att,E_PPS_NEW_DENSITY);
		objectDescr.totalMass = GetValueFromAttribute<int>(attObject,att,E_PPS_TOTAL_MASS);
	}

	if (srcRefEntity)
	{
		objectDescr.subEntID = srcRefEntity->GetID();
	}

	//################################################################
	//
	//	Transformation values 
	//
	VxVector box_s;

	if (!mesh && srcRefEntity)
	{
		box_s = BoxGetZero(srcRefEntity);
	}
	if (!srcRefEntity && mesh)
	{
		box_s = mesh->GetLocalBox().GetSize();
	}
	if (srcRefEntity && mesh )
	{
		box_s = mesh->GetLocalBox().GetSize();
	}

	//################################################################
	//
	// Determine radius
	//
	float radius = 1.0f;
	if ( mesh && !srcRefEntity )
	{
		radius = mesh->GetRadius();
	}
	if (!mesh && srcRefEntity && srcRefEntity->GetCurrentMesh() )
	{
		radius  = srcRefEntity->GetCurrentMesh()->GetRadius();
	}
	if (mesh && srcRefEntity)
	{
		radius  = mesh->GetRadius();
	}

	//################################################################
	//
	// Calculate destination matrix
	//
	VxMatrix v_matrix ;
	VxVector pos,scale;		
	VxQuaternion quat;	


	if (srcRefEntity)
	{
		if (isChild)
		{
			v_matrix  = srcRefEntity->GetLocalMatrix();
		}else
		{
			v_matrix = srcRefEntity->GetWorldMatrix();
		}
	}
	Vx3DDecomposeMatrix(v_matrix,quat,pos,scale);
	

	if (mesh && !srcRefEntity)
	{
		pos  = localPosition;
		quat = localRotation;
	}

	if (mesh && srcRefEntity )
	{

		VxQuaternion refQuad2;
		srcRefEntity->GetQuaternion(&refQuad2,GetVT3DObject());
		VxVector relPos;
		srcRefEntity->GetPosition(&relPos,GetVT3DObject());
		if (!isChild)
		{
			pos = relPos;
			quat = refQuad2;
		}
	}

	if (!mesh && srcRefEntity )
	{
		VxVector relPos;
		srcRefEntity->GetPosition(&relPos,GetVT3DObject());
		VxQuaternion refQuad2;
		srcRefEntity->GetQuaternion(&refQuad2,GetVT3DObject());
		pos = relPos;
		quat = refQuad2;
	}
	NxQuat rot = pMath::getFrom(quat);

	//################################################################
	//
	// Determine the mesh 
	//
	if (mesh && srcRefEntity==NULL )
	{
		srcMesh = mesh;
	}
	if (!mesh && srcRefEntity && srcRefEntity->GetCurrentMesh() )
	{
		srcMesh = srcRefEntity->GetCurrentMesh();
	}

	if (mesh && srcRefEntity && srcRefEntity->GetCurrentMesh())
	{
		srcMesh = mesh;
	}

	CK_ID srcID = 0 ; 
	if (srcMesh)
	{
		srcID  = srcMesh->GetID();
	}


	//################################################################
	//
	// Create the final sub shape
	//

	pSubMeshInfo *sInfo  = new pSubMeshInfo();
	bool isWheel1 = false;
	pWheel *wheel =NULL;
	NxShape *shape = NULL;


	if (objectDescr.hullType != HT_Wheel)
	{
		shape = pFactory::Instance()->createShape(GetVT3DObject(),objectDescr,srcRefEntity,srcMesh,pos,quat);
	}else
	{
		wheel = pFactory::Instance()->createWheelSubShape(this,srcRefEntity,srcMesh,&objectDescr,pos,quat,shape);
		if (wheel)
		{
			sInfo->wheel = wheel;
			pWheel1* w1 = dynamic_cast<pWheel1*>(wheel);
			if (w1){

				isWheel1 = true;
				shape = (NxShape*)w1->getWheelConvex();
			}

			pWheel2* w2 = dynamic_cast<pWheel2*>(wheel);
			if (w2)
			{
				shape =(NxShape*)w2->getWheelShape();
			}
            			
		}else
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Creating wheel sub shape failed");
			return NULL;
		}
	}

	if (!shape)
	{

		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create sub shape!");
	}

	if (shape)
	{

		//################################################################
		//
		// Setup the material
		//
		int materialIndex = 0;

		pMaterial bMaterial;
		bool hasMaterial = pFactory::Instance()->findSettings(bMaterial,srcRefEntity);
		if (!hasMaterial)
		{
			hasMaterial = pFactory::Instance()->findSettings(bMaterial,mesh);
		}

		if (hasMaterial)
		{
			NxMaterialDesc nxMatDescr;
			pFactory::Instance()->copyTo(nxMatDescr,bMaterial);
			NxMaterial *nxMaterial  = getWorld()->getScene()->createMaterial(nxMatDescr);
			if (nxMaterial)
			{
				materialIndex = nxMaterial->getMaterialIndex();
				nxMaterial->userData = (void*)bMaterial.xmlLinkID;
			}
		}else
		{
			materialIndex = getWorld()->getDefaultMaterial()->getMaterialIndex();
		}
		shape->setMaterial(materialIndex);

		//################################################################
		//
		// Store meta info in shape user data. 
		//
		
		shape->setGroup(objectDescr.collisionGroup);
		


		if (srcMesh)
		{
			sInfo->meshID = srcMesh->GetID();
			sInfo->mesh =(CKBeObject*)srcMesh;
			shape->setName(srcMesh->GetName());
		}

		if(srcRefEntity)
		{
			sInfo->entID = srcRefEntity->GetID();
			sInfo->refObject  = (CKBeObject*)srcRefEntity;
			shape->setName(srcRefEntity->GetName());
		}

		shape->userData = (void*)sInfo;
	}


	//################################################################
	//
	// Wheel Type one has an additional capsule swept shape
	// We store the shape meta data there as well
	if (
		isWheel1 && 
		wheel && 
		dynamic_cast<pWheel1*>(wheel) &&
		(pWheel1*)(dynamic_cast<pWheel1*>(wheel))->getWheelCapsule()
		)
	{
		((pWheel1*)(dynamic_cast<pWheel1*>(wheel)))->getWheelCapsule()->userData = sInfo;
	}

	//################################################################
	//
	// Modify mass 
	//
	if (objectDescr.mass.newDensity!=0.0f || objectDescr.mass.totalMass!=0.0f )
	{
		getActor()->updateMassFromShapes(objectDescr.mass.newDensity,objectDescr.mass.totalMass);
	}
	
	//################################################################
	//
	// Post routine
	//
	getActor()->wakeUp();
	if(mesh)
		return result;

	if (!objectDescr.hirarchy)
	{
		return result;
	}

	if (!srcRefEntity)
	{
		return result;
	}


	//################################################################
	//
	// If more entities in hierarchy, invoke this function recursively 
	//
	CK3dEntity* subEntity = NULL;
	while (subEntity= srcRefEntity->HierarchyParser(subEntity) )
	{
	
		pObjectDescr *subDescr  = NULL;

		//-try old version : 
		if (subEntity->HasAttribute(GetPMan()->GetPAttribute()))
		{
			subDescr = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
		}

		//-try new version
		int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
		if (subEntity->HasAttribute(attTypePBSetup))
		{
			subDescr  = new pObjectDescr();
			CKParameterOut *par =  subEntity->GetAttributeParameter(attTypePBSetup);
			IParameter::Instance()->copyTo(subDescr,par);
			subDescr->version  = pObjectDescr::E_OD_VERSION::OD_DECR_V1;

		}

		if (!subDescr)
			continue;

		if (subDescr->flags & BF_SubShape)
		{

			//////////////////////////////////////////////////////////////////////////
			if (subDescr->hullType != HT_Cloth)
			{
				addSubShape(NULL,*subDescr,subEntity);
			}

			if (subDescr->hullType == HT_Cloth)
			{

			}
		}
		
	}

	return result;
}



void pRigidBody::setBoxDimensions( const VxVector&dimension,CKBeObject* subShapeReference/*=NULL*/ )
{
	if (!isValid() || !getMainShape() )
	{
		return;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxBoxShape *box  = static_cast<NxBoxShape*>(getMainShape()->isBox());
		if (!box)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a box!");
			return;
		}
		box->setDimensions(getFrom(dimension));
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxBoxShape *box = static_cast<NxBoxShape*>(s->isBox());
		if (!box)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a box!");
			return;
		}
		box->setDimensions(getFrom(dimension));
	}
}
VxVector pRigidBody::getBoxDimensions(CKBeObject* subShapeReference)
{
	VxVector result(-1.f,-1.f,-1.f);
	if (!isValid() || !getMainShape() )
	{
		return result;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxBoxShape *box  = static_cast<NxBoxShape*>(getMainShape()->isBox());
		if (!box)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a box!");
			return result;
		}
		return getFrom(box->getDimensions());
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxBoxShape *box = static_cast<NxBoxShape*>(s->isBox());
		if (!box)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a box!");
			return result;
		}
		getFrom(box->getDimensions());
	}
	return result;
}





void pRigidBody::setSphereRadius(float radius,CKBeObject* subShapeReference/* =NULL */)
{

	if (!isValid() || !getMainShape() )
	{
		return;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxSphereShape *sphere  = static_cast<NxSphereShape*>(getMainShape()->isSphere());
		if (!sphere)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a sphere!");
			return;
		}
		sphere->setRadius(radius);
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxSphereShape*sphere = static_cast<NxSphereShape*>(s->isSphere());
		if (!sphere)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a sphere!");
			return;
		}
		sphere->setRadius(radius);
	}
}



float pRigidBody::getSphereRadius(CKBeObject* subShapeReference/* =NULL */)
{

	if (!isValid() || !getMainShape() )
	{
		return -1.0f;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxSphereShape *sphere  = static_cast<NxSphereShape*>(getMainShape()->isSphere());
		if (!sphere)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a sphere!");
			return -1.0f;
		}
		return sphere->getRadius();
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxSphereShape*sphere = static_cast<NxSphereShape*>(s->isSphere());
		if (!sphere)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a sphere!");
			return -1.0f;
		}
		return sphere->getRadius();
	}
	return -1.0f;
}




void pRigidBody::setCapsuleDimensions(float radius,float length,CKBeObject* subShapeReference)
{

	if (!isValid() || !getMainShape() )
	{
		return;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxCapsuleShape *capsule  = static_cast<NxCapsuleShape*>(getMainShape()->isCapsule());
		if (!capsule)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a capsule!");
			return;
		}
		capsule->setHeight(length);
		capsule->setRadius(radius);

	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxCapsuleShape*capsule = static_cast<NxCapsuleShape*>(s->isCapsule());
		if (!capsule)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a capsule!");
			return;
		}
		capsule->setHeight(length);
		capsule->setRadius(radius);
	}
}

void pRigidBody::getCapsuleDimensions(float& radius,float& length,CKBeObject* subShapeReference)
{
	if (!isValid() || !getMainShape() )
	{
		return;
	}

	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		NxCapsuleShape *capsule  = static_cast<NxCapsuleShape*>(getMainShape()->isCapsule());
		if (!capsule)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a capsule!");
			radius = -1.0f;
			length = -1.0f;
			return;
		}

		radius  = capsule->getRadius();
		length =  capsule->getHeight();

	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try a mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		NxCapsuleShape*capsule = static_cast<NxCapsuleShape*>(s->isCapsule());
		if (!capsule)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"target shape is not a capsule!");
			radius = -1.0f;
			length = -1.0f;
			return;
		}
		radius  = capsule->getRadius();
		length =  capsule->getHeight();
	}

}
HullType pRigidBody::getShapeType(CKBeObject* subShapeReference)
{
	if (!isValid() || !getMainShape() )
	{
		return HT_Unknown;
	}
	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL)
	{
		return (HullType)vtAgeia::getHullTypeFromShape(getMainShape());
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try a mesh :
			s = _getSubShape(subShapeReference->GetID());
		}
		if (!s)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"couldn't find sub shape!");
			return HT_Unknown;
		}
		return (HullType)vtAgeia::getHullTypeFromShape(s);
	}
	return HT_Unknown;
}

float pRigidBody::getSkinWidth(CKBeObject* subShapeReference)
{

	if (!isValid() || !getMainShape() )
	{
		return -1.0f;
	}
	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL && !getMainShape() )
	{
		return mSkinWidth;

	}
	if(subShapeReference == NULL && getMainShape() )
	{
		return getMainShape()->getSkinWidth();
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try a mesh :
			s = _getSubShape(subShapeReference->GetID());
		}
		if (!s)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"couldn't find sub shape!");
			return -1.0f;
		}
		return s->getSkinWidth();
	}
	return -1.0f;
}

void pRigidBody::setSkinWidth(const float skinWidth,CKBeObject* subShapeReference)
{
	if (!isValid() || !getMainShape() )
	{
		mSkinWidth = mSkinWidth;
		return;
	}
	/// shape is specified -> modify bodies first shape :
	if (subShapeReference == NULL && !getMainShape() )
	{
		mSkinWidth = skinWidth;
	}
	if(subShapeReference == NULL && getMainShape() )
	{
		getMainShape()->setSkinWidth(skinWidth);
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try a mesh :
			s = _getSubShape(subShapeReference->GetID());
		}
		if (!s)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"couldn't find sub shape!");
			return;
		}
		s->setSkinWidth(skinWidth);
	}
}


int pRigidBody::removeSubShape( CKBeObject *reference,float newensity/*=0.0f*/,float totalMass/*=0.0f*/ )
{


	int result  = -1;
	if (!reference || !getActor())
	{
		return result;
	}

	NxShape  *subShape = NULL;
	bool found = false;
	while(subShape = _getSubShape(reference->GetID()))
	{
		getActor()->releaseShape(*subShape);
		found =true;
	}

	//////////////////////////////////////////////////////////////////////////

	if (!found)
	{
		while(subShape = _getSubShape(reference->GetID()))
		{
			getActor()->releaseShape(*subShape);
			found =true;
		}
	}

	if (found && newensity !=0.0f || totalMass!=0.0f )
	{
		getActor()->updateMassFromShapes(newensity,totalMass);
	}
	getActor()->wakeUp();

	return 1;
}

NxShape * pRigidBody::_getSubShapeByEntityID( CK_ID id )
{
	if (!getActor())
	{
		return NULL;
	}
	int nbShapes = getActor()->getNbShapes();
	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->entID == id)
			{
				return s;
			}
		}
	}
	return NULL;
}
bool pRigidBody::isSubShape(CKBeObject *object)
{

	bool result  = false;
	if (!object)
	{
		return result;
	}

	if ( _getSubShape(object->GetID()) || _getSubShapeByEntityID(object->GetID() ))
	{
		return true ;
	}
	return result;
}
NxShape *pRigidBody::_getSubShape(CK_ID meshID)
{

	if (!getActor())
	{
		return NULL;
	}

	int nbShapes = getActor()->getNbShapes();

	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->meshID == meshID)
			{
				return s;
			}
		}
	}
	return NULL;
}


int pRigidBody::updateMassFromShapes( float density, float totalMass )
{

	if (getActor())
	{
		return getActor()->updateMassFromShapes(density,totalMass);
	}
	return -1;
}


NxShape *pRigidBody::getShapeByIndex(int index/* =0 */)
{

	NxU32 nbShapes = getActor()->getNbShapes();
	if ( nbShapes )
	{
		NxShape ** slist = (NxShape **)getActor()->getShapes();
		for (NxU32 j=0; j<nbShapes; j++)
		{
			NxShape *s = slist[j];
			if (s && j == index)
			{
				return s;
			}
		}
	}
	return NULL;
}

bool pRigidBody::isCollisionEnabled( CK3dEntity* subShapeReference/*=NULL*/ )
{
	NxShape *subShape = getSubShape(subShapeReference);
	if (subShape )
	{
		if (subShape ==getMainShape())
		{
			if (getActor())
			{
				return !getActor()->readActorFlag(NX_AF_DISABLE_COLLISION);
			}
		}else{
			return !subShape->getFlag(NX_SF_DISABLE_COLLISION);
		}
	}
	return false;
}
void pRigidBody::enableCollision( bool enable,CK3dEntity* subShapeReference/*=NULL*/ )
{
	NxShape *subShape = getSubShape(subShapeReference);

	if (!subShape)
	{
		subShape==getMainShape();
	}
	if (subShape )
	{
		if (subShape ==getMainShape())
		{
			if (!enable)
			{
				getActor()->raiseActorFlag(NX_AF_DISABLE_COLLISION);
			}else
			{
				getActor()->clearActorFlag(NX_AF_DISABLE_COLLISION);
			}
		}else{

			subShape->setFlag(NX_SF_DISABLE_RESPONSE, !enable );
		}
	}
}
void pRigidBody::enableTriggerShape( bool enable,CK3dEntity* subShapeReference/*=NULL*/ )
{

	NxShape *subShape = getSubShape(subShapeReference);
	if (subShape)
	{
		subShape->setFlag(NX_TRIGGER_ENABLE,enable);
	}
}

bool pRigidBody::isTriggerShape( CK3dEntity* subShapeReference/*=NULL*/ )
{
	NxShape *subShape = getSubShape(subShapeReference);
	if (subShape)
	{
		return subShape->getFlag(NX_TRIGGER_ENABLE);
	}
	return false;
}

bool pRigidBody::isCollisionsNotifyEnabled()
{

	return ( getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH );
}

void pRigidBody::enableCollisionsNotify( bool enable )
{
	if (enable)
	{
		getActor()->setContactReportFlags(NX_NOTIFY_ON_TOUCH);
	}else
		getActor()->setContactReportFlags(NX_IGNORE_PAIR);
}

VxVector pRigidBody::getPivotOffset(CK3dEntity*shapeReference)
{

	NxShape *subshape = getSubShape(shapeReference);
	if (subshape)	return getFrom(subshape->getLocalPosition());

	return VxVector();
}

NxShape*pRigidBody::getSubShape(CK3dEntity*shapeReference/* =NULL */)
{


	if (shapeReference)
	{
	
		NxShape *inputShape = _getSubShapeByEntityID(shapeReference->GetID());
		if (getMainShape() ==inputShape)
		{
			return getMainShape();
		}else{
			return inputShape;
		}
	}

	return getMainShape();

}
void pRigidBody::setShapeMaterial(pMaterial&material,CK3dEntity*shapeReference/* =NULL */)
{
	
	NxShape *dstShape  = NULL;


	if (shapeReference)
	{
		dstShape  = _getSubShapeByEntityID(shapeReference->GetID());
	}else{
		dstShape = getMainShape();
	}


	#ifdef _DEBUG
		assert(dstShape);
	#endif

		
	int materialIndex = dstShape->getMaterial();

	NxMaterial *currentMaterial = getActor()->getScene().getMaterialFromIndex(materialIndex);

	if (!material.isValid())
		return;

	NxMaterialDesc nxMatDescr;

	//////////////////////////////////////////////////////////////////////////
	//
	//		We dont alter the default material ! We create a new one !!
	//

	int defaultID = getWorld()->getDefaultMaterial()->getMaterialIndex();
	if ( !currentMaterial  || materialIndex ==0 || materialIndex == getWorld()->getDefaultMaterial()->getMaterialIndex() )
	{
		
		pFactory::Instance()->copyTo(nxMatDescr,material);

		NxMaterial *newMaterial  = getActor()->getScene().createMaterial(nxMatDescr);
		if (newMaterial){
			dstShape->setMaterial(newMaterial->getMaterialIndex());
			newMaterial->userData = (void*)material.xmlLinkID;
		}
	}
	else
	{
		pFactory::Instance()->copyTo(nxMatDescr,material);
		currentMaterial->loadFromDesc(nxMatDescr);
	}


}
pMaterial&pRigidBody::getShapeMaterial(CK3dEntity *shapeReference/* =NULL */)
{

	pMaterial result;

	if (shapeReference && !isSubShape(shapeReference))
	{
		return result;
	}

	NxShape *srcShape  = NULL;
	CK3dEntity *ent = GetVT3DObject();
	
	if (shapeReference)
	{
		srcShape  = getSubShape(shapeReference);
	}else{
		srcShape = getMainShape();
	}

	#ifdef _DEBUG
		assert(srcShape);
	#endif

	int index = srcShape->getMaterial();
	NxMaterial *mat = getActor()->getScene().getMaterialFromIndex(srcShape->getMaterial());

	pFactory::Instance()->copyTo(result,getActor()->getScene().getMaterialFromIndex(srcShape->getMaterial()));


	return result;

}
void pRigidBody::_checkForRemovedSubShapes()
{


}

void pRigidBody::_checkForNewSubShapes()
{

	pObjectDescr *oDescr = pFactory::Instance()->createPObjectDescrFromParameter(GetVT3DObject()->GetAttributeParameter(GetPMan()->GetPAttribute()));
	if (!oDescr)
	{
		return;
	}

	if (! ( getFlags() & BF_Hierarchy ) ) 
	{
		return ;
	}

	CK3dEntity* subEntity = NULL;
	while (subEntity= GetVT3DObject()->HierarchyParser(subEntity) )
	{
		if ( !_getSubShapeByEntityID(subEntity->GetID()) )
		{
			CKSTRING name = subEntity->GetName();
			int s = isSubShape(subEntity);

			if ( subEntity->HasAttribute(GetPMan()->GetPAttribute()	)  )
			{
				pObjectDescr *subDescr = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
				if (subDescr->flags & BF_SubShape)
				{
					if (subDescr->hullType != HT_Cloth)
					{
						addSubShape(NULL,*oDescr,subEntity);
					}

					if (subDescr->hullType == HT_Cloth)
					{
						//pClothDesc *cloth = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
					}
				}
			}
		}
	}


	if (( getFlags() & BF_Hierarchy ))
	{
		if (oDescr->newDensity!=0.0f || oDescr->totalMass!=0.0f )
		{
			updateMassFromShapes(oDescr->newDensity,oDescr->totalMass);
		}
	}
}
int pRigidBody::updateSubShapes(bool fromPhysicToVirtools/* = true */,bool position/* =true */,bool rotation/* =true */)
{
	if(!getActor())
		return -1;
	NxU32 nbShapes = getActor()->getNbShapes();
	if ( nbShapes )
	{
		NxShape ** slist = (NxShape **)getActor()->getShapes();
		for (NxU32 j=0; j<nbShapes; j++)
		{
			NxShape *s = slist[j];
			if (s)
			{
				pSubMeshInfo *info =  static_cast<pSubMeshInfo*>(s->userData);
				if (info)
				{
					CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(info->entID));
					if (ent)
					{

						if (s == getMainShape())
							continue;


						if (fromPhysicToVirtools)
						{
							//pVehicle *v  =getVehicle(); 
							pWheel* wheel  = (pWheel*)getWheel(ent);
							if (wheel)
							{
								wheel->_updateVirtoolsEntity(position,rotation);
							}else
							{

								if (position)
								{
									VxVector gPos  = getFrom(s->getLocalPose().t);
									ent->SetPosition(&gPos,GetVT3DObject());
								}
								if (rotation)
								{
									VxQuaternion rot  = pMath::getFrom( s->getLocalPose().M );
									if(s->isWheel())
									{
									}else
									{
										ent->SetQuaternion(&rot,GetVT3DObject());
									}
								}
							}
						}else//Virtools to Ageia !
						{
							if (position)
							{
								VxVector relPos;
								ent->GetPosition(&relPos,GetVT3DObject());
								s->setLocalPosition(getFrom(relPos));
							}

							if (rotation)
							{
								VxQuaternion refQuad2;
								ent->GetQuaternion(&refQuad2,GetVT3DObject());
								s->setLocalOrientation(getFrom(refQuad2));
							}

						}
						/*if (s->getType()  ==NX_SHAPE_WHEEL )
						{
						NxWheelShape *wShape = static_cast<NxWheelShape*>(s);
						if (wShape)
						{

						float cS = wShape->getSuspension().spring;
						float cST = wShape->getSuspensionTravel();

						VxVector gPos  = getFrom(s->getGlobalPose().t);
						VxVector lPos  = getFrom(s->getLocalPose().t);
						VxVector diff ;
						ent->GetPosition(&diff,GetVT3DObject());
						ent->SetPosition(&gPos);


						}
						}
						*/
					}
				}
			}
		}
	}


	return 0;
}
int pRigidBody::getCollisionsGroup( CK3dEntity* subShapeReference/*=NULL*/ )
{

	NxShape *subShape = getSubShape(subShapeReference);
	if (subShape)
	{
		return subShape->getGroup();
	}
	return -1;
}
void pRigidBody::setCollisionsGroup( int index,CK3dEntity* subShapeReference/*=NULL*/ )
{

	if(!getActor())
		return;

	if (subShapeReference == NULL)
	{
		if(index>=0 && index <=32)
			getActor()->setGroup(index);

		NxU32 nbShapes = getActor()->getNbShapes();
		if ( nbShapes )
		{
			NxShape ** slist = (NxShape **)getActor()->getShapes();
			for (NxU32 j=0; j<nbShapes; j++)
			{
				NxShape *s = slist[j];
				if (s)
				{
					if(index>=0 && index <=32)
						s->setGroup(index);
				}
			}
		}
	}

	// shape reference is specified - > modify sub shape : 
	if (subShapeReference !=NULL && isSubShape(subShapeReference) )
	{
		// try an entity : 
		NxShape *s = _getSubShapeByEntityID(subShapeReference->GetID());
		if (!s)
		{
			//try an mesh :
			s = _getSubShape(subShapeReference->GetID());
		}

		if (!s)
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"there is no such sub shape!");
			return;
		}
		s->setGroup(index);
	}
}

void pRigidBody::setGroupsMask(CK3dEntity *shapeReference,const pGroupsMask& mask)
{


	NxShape *mainshape = getMainShape();
	NxShape *dstShape = getMainShape();

	if (shapeReference)
	{
		NxShape *inputShape = _getSubShapeByEntityID(shapeReference->GetID());
		if (mainshape==inputShape)
		{
			dstShape = mainshape;
		}else{
			dstShape = inputShape;
		}
	}


	NxGroupsMask mask1;
	mask1.bits0  = mask.bits0;
	mask1.bits1  = mask.bits1;
	mask1.bits2  = mask.bits2;
	mask1.bits3  = mask.bits3;

	if (dstShape)
	{
		dstShape->setGroupsMask(mask1);
	}

}


pGroupsMask pRigidBody::getGroupsMask(CK3dEntity *shapeReference)
{

	NxShape *mainshape = getMainShape();
	NxShape *dstShape = getMainShape();

	if (shapeReference)
	{
		NxShape *inputShape = _getSubShapeByEntityID(shapeReference->GetID());
		if (mainshape==inputShape)
		{
			dstShape = mainshape;
		}else{
			dstShape = inputShape;
		}
	}

	if (dstShape)
	{

		NxGroupsMask gMask  = dstShape->getGroupsMask();

		pGroupsMask vtGMask;

		vtGMask.bits0 = gMask.bits0;
		vtGMask.bits1 = gMask.bits1;
		vtGMask.bits2 = gMask.bits2;
		vtGMask.bits3 = gMask.bits3;
		return  vtGMask;

	}


	return pGroupsMask();

}

/*
if (srcRefEntity && srcRefEntity->HasAttribute(GetPMan()->att_wheelDescr ))
{
	CKParameterOut *par = srcRefEntity->GetAttributeParameter(GetPMan()->att_wheelDescr );
	if (par)
	{
		pWheelDescr *wDescr  = new pWheelDescr();
		int err = pFactory::Instance()->copyTo(wDescr,par);

		if (wDescr && !wDescr->isValid() )
		{
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Wheel Description was invalid");
			delete wDescr;
		}

		if (wDescr)
		{
			pWheel *wheel  = pFactory::Instance()->createWheel(this,*wDescr);

			shape = pFactory::Instance()->createWheelShape(getActor(),&objectDescr,wDescr,srcRefEntity,srcMesh,pos,quat);
			if (shape)
			{

				if(wDescr->wheelFlags & E_WF_USE_WHEELSHAPE)
				{
					pWheel2 * wheel2 = (pWheel2*)wheel;
					wheel2->setWheelShape((NxWheelShape*)shape);

				}else{	

				}

				if (wheel->getWheelFlag(E_WF_VEHICLE_CONTROLLED) && mVehicle)
					mVehicle->getWheels().push_back(wheel);

				wheel->mWheelFlags = wDescr->wheelFlags;
				sInfo->wheel = wheel;
				wheel->setEntID(srcRefEntity->GetID());

			}
		}
	}
}
*/