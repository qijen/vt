#include <StdAfx.h>
#include "vtPhysXAll.h"

static pFactory* pFact = NULL;

int _registerJointAttributeType(CKGUID guid,char *name)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(GetPMan()->GetContext()->GetParameterManager());
	CKAttributeManager *am = (CKAttributeManager*)GetPMan()->GetContext()->GetAttributeManager();

//	CKSTRING test = vtTools::ParameterTools::GetParameterAsString(p);
//	if (!strlen(test))return;

	CKAttributeType aIType = am->GetAttributeTypeByName(name);
	if (aIType==-1)
	{
		aIType = am->RegisterNewAttributeType(name,guid,CKCID_OBJECT);
		am->SetAttributeCategory(aIType,"Physic Constraints");
	}
	return aIType;
}



int pFactory::cloneLimitPlanes(pJoint *src,pJoint *dst,CK3dEntity *srcReference,CK3dEntity *dstReference)
{
	//----------------------------------------------------------------
	//
	// sanity checks
	//
	#ifdef _DEBUG
		assert(src);
		assert(dst);
		assert(dstReference);
		assert(srcReference);
	#endif // _DEBUG

	NxJoint *j = src->getJoint();
	if (!j)
		return 0;
	
	
	int numLimitPlanes = src->getNbLimitPlanes();
	if (!numLimitPlanes)
		return -1;

	NxVec3 *planeNormal = new NxVec3[numLimitPlanes];
	NxReal *planeD = new NxReal[numLimitPlanes];
	NxReal *restitution = new NxReal[numLimitPlanes];

	NxVec3 planeLimitPt;
	bool onActor2 = j->getLimitPoint( planeLimitPt );
	NxVec3 *worldLimitPt = new NxVec3[numLimitPlanes];
	
	//----------------------------------------------------------------
	//
	// copy to buffer
	//
	j->resetLimitPlaneIterator();

	for(int iter=0 ; iter < numLimitPlanes ; iter++ )
	{
		j->getNextLimitPlane( planeNormal[iter], planeD[iter], &restitution[iter] );
	}

	

	//----------------------------------------------------------------
	//
	// create limitPoints
	//

	dst->setLimitPoint(getFrom(planeLimitPt),onActor2);


	for(int iter=0 ; iter < numLimitPlanes ; iter++ )
	{
	
		if ( ( fabs(planeNormal[iter].z) > fabs(planeNormal[iter].x) ) &&
			( fabs(planeNormal[iter].z) > fabs(planeNormal[iter].y) ) )
		{
			worldLimitPt[iter].x = planeLimitPt.x;
			worldLimitPt[iter].y = planeLimitPt.y;
			worldLimitPt[iter].z = -planeD[iter] / planeNormal[iter].z;
		}
		// k, that didn't work - try x,z = 0
		else if ( ( fabs(planeNormal[iter].y) > fabs(planeNormal[iter].x) ) &&
			( fabs(planeNormal[iter].y) > fabs(planeNormal[iter].z) ) )
		{
			worldLimitPt[iter].x = planeLimitPt.x;
			worldLimitPt[iter].z = planeLimitPt.z;
			worldLimitPt[iter].y = -planeD[iter] / planeNormal[iter].y;
		}
		else
		{
			worldLimitPt[iter].y = planeLimitPt.y;
			worldLimitPt[iter].z = planeLimitPt.z;
			worldLimitPt[iter].x = -planeD[iter] / planeNormal[iter].x;
		}

		dst->addLimitPlane(getFrom(planeNormal[iter]),getFrom(worldLimitPt[iter]),restitution[iter]);

	}

	delete []worldLimitPt;
	delete []planeNormal;
	delete []restitution;
	delete []planeD;

	worldLimitPt = NULL;
	planeD = NULL;
	restitution = NULL;
	planeD = NULL;


	return numLimitPlanes;
	
}

pJoint *pFactory::cloneJoint(pJoint *src,CK3dEntity *srcReference,CK3dEntity *dstReference,int copyFlags)
{

	//----------------------------------------------------------------
	//
	// sanity checks
	//
	#ifdef _DEBUG
		assert(src);
		assert(dstReference);
	#endif // _DEBUG


	//----------------------------------------------------------------
	//
	// Prepare pointers
	//
	NxActor *actorA  = NULL;
	pRigidBody *actorABody = GetPMan()->getBody(dstReference);
	if (actorABody){
		actorA = actorABody->getActor();
	}
	else{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"You need at least one rigid body");
		return NULL;
	}



	//actorB can be NULL and can not be the same as actorA : 
	NxActor *actorB  = NULL;
	pRigidBody *actorBBody = NULL;
	CK3dEntity *referenceB = NULL;
	

	//----------------------------------------------------------------
	//
	// determine the first, this will be the clone !
	//
	if ( src->GetVTEntA() && src->GetVTEntA() !=srcReference   )
	{
		actorBBody = GetPMan()->getBody(src->GetVTEntA());
		if (actorBBody)
		{
			actorB = actorBBody->getActor();
			referenceB = actorBBody->GetVT3DObject();
		}
	}

	//----------------------------------------------------------------
	//
	// determine the second
	//
	if ( src->GetVTEntB() && src->GetVTEntB() !=srcReference )
	{
		actorBBody = GetPMan()->getBody(src->GetVTEntB());
		if (actorBBody)
		{
			actorB = actorBBody->getActor();
			referenceB = actorBBody->GetVT3DObject();
		}
	}

	if ( (copyFlags & PB_CF_SWAP_JOINTS_REFERENCES) )
	{
		XSwap(actorA,actorB);
		XSwap(actorABody,actorBBody);

	}

	CK3dEntity *testA  = actorABody->GetVT3DObject();
	CK3dEntity *testB  = NULL;
	if (actorBBody)
		testB   = actorBBody->GetVT3DObject();



	switch (src->getType())
	{

		//----------------------------------------------------------------
		//
		// Distance Joint : 
		//
		case JT_Distance :
		{

			NxDistanceJoint *nxSrc = src->getJoint()->isDistanceJoint();
			if (nxSrc)
			{
				NxDistanceJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;
				
				NxDistanceJoint *nxJoint  = (NxDistanceJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointDistance *joint = new pJointDistance(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());

				return joint;
			}
			break;
		}

		//----------------------------------------------------------------
		//
		// Fixed Joint : 
		//
		case JT_Fixed :
		{

			NxFixedJoint *nxSrc = src->getJoint()->isFixedJoint();
			if (nxSrc)
			{
				NxFixedJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxFixedJoint *nxJoint  = (NxFixedJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointFixed *joint = new pJointFixed(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// Ball Joint : 
		//
		case JT_Spherical:
		{

			NxSphericalJoint *nxSrc = src->getJoint()->isSphericalJoint();
			if (nxSrc)
			{
				NxSphericalJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxSphericalJoint *nxJoint  = (NxSphericalJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointBall *joint = new pJointBall(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// Revolute Joint : 
		//
		case JT_Revolute:
		{

			NxRevoluteJoint *nxSrc = src->getJoint()->isRevoluteJoint();
			if (nxSrc)
			{
				NxRevoluteJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxRevoluteJoint *nxJoint  = (NxRevoluteJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointRevolute *joint = new pJointRevolute(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// Cylindrical Joint : 
		//
		case JT_Cylindrical:
		{

			NxCylindricalJoint *nxSrc = src->getJoint()->isCylindricalJoint();
			if (nxSrc)
			{
				NxCylindricalJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxCylindricalJoint *nxJoint  = (NxCylindricalJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointCylindrical*joint = new pJointCylindrical(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// Prismatic Joint : 
		//
		case JT_Prismatic:
		{

			NxPrismaticJoint *nxSrc = src->getJoint()->isPrismaticJoint();
			if (nxSrc)
			{
				NxPrismaticJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxPrismaticJoint *nxJoint  = (NxPrismaticJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointPrismatic*joint = new pJointPrismatic(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// PointInPlane Joint : 
		//
		case JT_PointInPlane:
		{

			NxPointInPlaneJoint*nxSrc = src->getJoint()->isPointInPlaneJoint();
			if (nxSrc)
			{
				NxPointInPlaneJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxPointInPlaneJoint *nxJoint  = (NxPointInPlaneJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}

				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointPointInPlane*joint = new pJointPointInPlane(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;

			}
			break;
		}

		//----------------------------------------------------------------
		//
		// PointOnLine Joint : 
		//
		case JT_PointOnLine:
		{

			NxPointOnLineJoint*nxSrc = src->getJoint()->isPointOnLineJoint();
			if (nxSrc)
			{
				NxPointOnLineJointDesc descrNew;
				nxSrc->saveToDesc(descrNew);

				descrNew.actor[0] = actorA;	descrNew.actor[1] = actorB;

				NxPointOnLineJoint *nxJoint  = (NxPointOnLineJoint*)actorA->getScene().createJoint(descrNew);
				if (!nxJoint)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
					return NULL;
				}
				//////////////////////////////////////////////////////////////////////////,
				//		vt object :
				pJointPointOnLine*joint = new pJointPointOnLine(actorABody,actorBBody);
				nxJoint->userData = joint;
				joint->setJoint(nxJoint);
				joint->setWorld(actorABody->getWorld());
				return joint;
			}
			break;
		}
	}
	return NULL;
}

void pFactory::cloneJoints(CK3dEntity *src,CK3dEntity *dst,int copyFlags)
{

	//----------------------------------------------------------------
	//
	// sanity checks
	//
	#ifdef _DEBUG
		assert(src);
		assert(dst);
	#endif // _DEBUG


	pRigidBody *srcBody = GetPMan()->getBody(src);
	pRigidBody *dstBody = GetPMan()->getBody(dst);
	
	XString errMsg;

	if ( !srcBody || !dstBody )
	{
		errMsg.Format("You need two rigid objects to clone joints from a rigid body");
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errMsg.Str());
		return;
	}

	pWorld* srcBodyWorld = srcBody->getWorld();
	pWorld* dstBodyWorld = dstBody->getWorld();

	if(	!srcBodyWorld || !dstBodyWorld )
	{
		errMsg.Format("You need two valid world objects to clone joints from a rigid body");
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errMsg.Str());
		return;
	}

	if(	srcBodyWorld!=dstBodyWorld )
	{
		errMsg.Format("Worlds objects must be same");
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errMsg.Str());
		return;
	}

	//----------------------------------------------------------------
	//
	// copy process
	//
	
	pWorld *testWorld = dstBodyWorld;


	NxU32	jointCount = testWorld->getScene()->getNbJoints();
	if (jointCount)
	{
		NxArray< NxJoint * > joints;

		testWorld->getScene()->resetJointIterator();

		NxJoint *last = NULL;
		pJoint *lastJ = NULL;
		for	(NxU32 i = 0;	i	<	jointCount;	++i)
		{
			NxJoint	*j = testWorld->getScene()->getNextJoint();
			pJoint *mJoint = static_cast<pJoint*>( j->userData );
			if ( mJoint )
			{
				if (mJoint==lastJ)// avoid double clone
				{
					continue;
				}
				
				if (mJoint->GetVTEntA() == srcBody->GetVT3DObject() || mJoint->GetVTEntB() == srcBody->GetVT3DObject() )
				{
					pJoint *clone  = pFactory::cloneJoint(mJoint,src,dst,copyFlags);
					if (clone  )
					{
						if ((copyFlags & PB_CF_LIMIT_PLANES))
						{
							int limitPlanes = pFactory::cloneLimitPlanes(mJoint,clone,src,dst);
						}
					}
				}
			}
			lastJ  = mJoint;
		}
	}
}


pJointPointOnLine*pFactory::createPointOnLineJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision,float maxForce,float maxTorque,const char* attributeName)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;


			if (referenceObject)
			{
				
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_POINT_ON_LINE,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JPOL_BODY_B,other->GetID());
					SetParameterStructureValue<VxVector>(parAttribute,PS_JPOL_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JPOL_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JPOL_COLLISION,collision);

					SetParameterStructureValue<float>(parAttribute,PS_JPOL_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JPOL_MAX_FORCE,maxTorque);
				}
			}
			return NULL;
		}
	}

	if (jointCheckPreRequisites(a,b,JT_PointOnLine)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}

	NxPointOnLineJointDesc desc;
	desc.actor[0] = a0;
	desc.actor[1] = a1;
	desc.setGlobalAxis(getFrom(axis));
	desc.setGlobalAnchor(getFrom(anchor));

	if (!desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}

	NxPointOnLineJoint*	nxJoint  = (NxPointOnLineJoint*)world->getScene()->createJoint(desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointPointOnLine*joint = new pJointPointOnLine(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	joint->setWorld(world);
	return joint;
}

pJointPointInPlane*pFactory::createPointInPlaneJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision,float maxForce,float maxTorque,const char* attributeName)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;
			if (referenceObject)
			{
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_POINT_IN_PLANE,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JPIP_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JPIP_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JPIP_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JPIP_COLLISION,collision);
					SetParameterStructureValue<float>(parAttribute,PS_JPIP_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JPIP_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}


	if (jointCheckPreRequisites(a,b,JT_PointInPlane)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}

	NxPointInPlaneJointDesc desc;
	desc.actor[0] = a0;
	desc.actor[1] = a1;
	desc.setGlobalAxis(getFrom(axis));
	desc.setGlobalAnchor(getFrom(anchor));

	if (!desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}

	NxPointInPlaneJoint*	nxJoint  = (NxPointInPlaneJoint*)world->getScene()->createJoint(desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointPointInPlane*joint = new pJointPointInPlane(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	joint->setWorld(world);
	return joint;
}

pJointCylindrical*pFactory::createCylindricalJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision,float maxForce,float maxTorque,const char* attributeName)
{
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;


			if (referenceObject)
			{
				
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_CYLINDRICAL,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JCYLINDRICAL_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JCYLINDRICAL_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JCYLINDRICAL_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JCYLINDRICAL_COLLISION,collision);

					SetParameterStructureValue<float>(parAttribute,PS_JCYLINDRICAL_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JCYLINDRICAL_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}


	if (jointCheckPreRequisites(a,b,JT_Cylindrical)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}

	NxCylindricalJointDesc desc;
	desc.actor[0] = a0;
	desc.actor[1] = a1;
	desc.setGlobalAxis(getFrom(axis));
	desc.setGlobalAnchor(getFrom(anchor));

	if (!desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}
		
	NxCylindricalJoint*	nxJoint  = (NxCylindricalJoint*)world->getScene()->createJoint(desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointCylindrical*joint = new pJointCylindrical(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	joint->setWorld(world);
	return joint;

}
pJointPrismatic *pFactory::createPrismaticJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision,float maxForce,float maxTorque,const char* attributeName)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;


			if (referenceObject)
			{
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_PRISMATIC,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JPRISMATIC_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JPRISMATIC_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JPRISMATIC_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JPRISMATIC_COLLISION,collision);

					SetParameterStructureValue<float>(parAttribute,PS_JPRISMATIC_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JPRISMATIC_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}



	if (jointCheckPreRequisites(a,b,JT_Prismatic)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}

	NxPrismaticJointDesc desc;
	desc.actor[0] = a0;
	desc.actor[1] = a1;
	desc.setGlobalAxis(getFrom(axis));
	desc.setGlobalAnchor(getFrom(anchor));
	
	if (!desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}

	NxPrismaticJoint*	nxJoint  = (NxPrismaticJoint*)world->getScene()->createJoint(desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointPrismatic*joint = new pJointPrismatic(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	joint->setWorld(world);
	return joint;
	
}
pJointRevolute*pFactory::createRevoluteJoint(CK3dEntity*a,CK3dEntity*b,
											 VxVector anchor,VxVector axis,
											 bool collision,float maxForce,float maxTorque,const char* attributeName)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;
			if (referenceObject)
			{
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_REVOLUTE,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JREVOLUTE_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JREVOLUTE_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JREVOLUTE_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JREVOLUTE_COLLISION,collision);
					SetParameterStructureValue<float>(parAttribute,PS_JREVOLUTE_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JREVOLUTE_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}



	if (jointCheckPreRequisites(a,b,JT_Revolute)  == -1 )
	{
		return NULL;
	}

	
	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);

	if (!ba && !bb)
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"No body specified");
		return NULL;
	}
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world && bb)
	{
		world = bb->getWorld();
	}


	NxRevoluteJointDesc descr;
	
	/*descr.projectionDistance = (NxReal)0.05;
	descr.projectionAngle= (NxReal)0.05;
	descr.projectionMode = NX_JPM_POINT_MINDIST;*/

	

	if (ba)
	{
		descr.actor[0]=ba->getActor();
	}
	
	if (bb)
	{
		descr.actor[1]=bb->getActor();
	}
	
	NxRevoluteJoint *nxJoint=(NxRevoluteJoint*)world->getScene()->createJoint(descr);
	
	if (!descr.isValid())
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"Revolute joint invalid!");
		return NULL;
	}

	nxJoint->setGlobalAnchor(getFrom(anchor));
	nxJoint->setGlobalAxis(getFrom(axis));


	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointRevolute*joint = new pJointRevolute(ba,bb);
	nxJoint->userData = joint;
	joint->setWorld(world);
	joint->setJoint(nxJoint);



	return joint;



}


pJointPulley* 
pFactory::createPulleyJoint( CK3dEntity*a,CK3dEntity*b,
							VxVector pulley0,VxVector pulley1,VxVector anchor0, 
							VxVector anchor1,
							bool collision,float maxForce,float maxTorque)
{

	/*
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;
			if (referenceObject)
			{
				int attributeType  = GetPMan()->getAttributeTypeByGuid(VTS_JOINT_REVOLUTE);
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JREVOLUTE_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JREVOLUTE_ANCHOR,anchor);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JREVOLUTE_AXIS,axis);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JREVOLUTE_COLLISION,collision);
					SetParameterStructureValue<float>(parAttribute,PS_JREVOLUTE_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JREVOLUTE_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}

	*/

	if (jointCheckPreRequisites(a,b,JT_Pulley)  == -1 )
	{
		return NULL;
	}


	VxVector globalAxis(0,1,0);
	float distance = 10.0f;
	float ratio  = 1.0f;
	float stiffness = 1.0f;
	int flags = 0 ; 
	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);

	if (!ba && !bb)
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"You need two bodies for a pulley joint!");
		return NULL;
	}
	pWorld*world  = GetPMan()->getWorldByBody(a);


	//////////////////////////////////////////////////////////////////////////
	//	nx object : 
	NxPulleyJointDesc descr;
	descr.setToDefault();

	descr.actor[0]=ba->getActor();
	descr.actor[1]=bb->getActor();
	descr.localAnchor[0] = pMath::getFrom(anchor0);
	descr.localAnchor[1] = pMath::getFrom(anchor1);
	descr.setGlobalAxis(pMath::getFrom(globalAxis));



	descr.pulley[0] = pMath::getFrom(pulley0); 	// suspension points of two bodies in world space.
	descr.pulley[1] = pMath::getFrom(pulley1); 	// suspension points of two bodies in world space.
	descr.distance = distance;		// the rest length of the rope connecting the two objects.  The distance is computed as ||(pulley0 - anchor0)|| +  ||(pulley1 - anchor1)|| * ratio.
	descr.stiffness = stiffness;		// how stiff the constraint is, between 0 and 1 (stiffest)
	descr.ratio = ratio;			// transmission ratio
	descr.flags = flags;	// This is a combination of the bits defined by ::NxPulleyJointFlag. 

	if (!descr.isValid())
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"Pully joint invalid!");
		return NULL;
	}
	NxPulleyJoint *nxJoint  = (NxPulleyJoint*)world->getScene()->createJoint(descr);
	if (!nxJoint)
	{
		xLogger::xLog(ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}


	
	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointPulley*joint = new pJointPulley(ba,bb);
	nxJoint->userData = joint;
	joint->setWorld(world);
	joint->setJoint(nxJoint);
	return joint;


	//descr.motor = gMotorDesc;
	//	pulleyDesc.projectionMode = NX_JPM_NONE;
	//	pulleyDesc.projectionMode = NX_JPM_POINT_MINDIST;
	//	pulleyDesc.jointFlags |= NX_JF_COLLISION_ENABLED;
	return NULL;
}

pJointFixed*pFactory::createFixedJoint(CK3dEntity *a, CK3dEntity *b,float maxForce, float maxTorque,const char* attributeName)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{

			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;

			if (referenceObject)
			{

				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_FIXED,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
					{
						SetParameterStructureValue<CK_ID>(parAttribute,0,other->GetID());
					}
					SetParameterStructureValue<float>(parAttribute,1,maxForce);
					SetParameterStructureValue<float>(parAttribute,2,maxTorque);
				}
			}
			int op = 2;
			return 0;
		}
	}

		
	if (jointCheckPreRequisites(a,b,JT_Fixed)  == -1 )
	{
		return NULL;
	}
	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}


	NxFixedJointDesc descr;
	descr.actor[0] = a0;
	descr.actor[1] = a1;

	if (!descr.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}


	NxJoint *nxJoint  = (NxDistanceJoint*)world->getScene()->createJoint(descr);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

		//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointFixed*joint = new pJointFixed(ba,bb);
	nxJoint->userData = joint;
	joint->setWorld(world);
	joint->setJoint(nxJoint);
	return joint;

}

pJointD6*pFactory::createD6Joint(CK3dEntity *a, CK3dEntity *b, VxVector globalAnchor,VxVector globalAxis,bool collision,float maxForce, float maxTorque)
{

	if (jointCheckPreRequisites(a,b,JT_D6)  == -1 )
	{
		return NULL;
	}
	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}


	NxD6JointDesc d6Desc;
	d6Desc.actor[0] = a0;
	d6Desc.actor[1] = a1;
	d6Desc.setGlobalAnchor(pMath::getFrom(globalAnchor));
	d6Desc.setGlobalAxis(pMath::getFrom(globalAxis));
	if (collision)
	{
		//d6Desc.flags 
		d6Desc.jointFlags|=NX_JF_COLLISION_ENABLED;
	}
	
	
		
	if (!d6Desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}


	NxJoint *nxJoint  = (NxDistanceJoint*)world->getScene()->createJoint(d6Desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

		//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointD6 *joint = new pJointD6(ba,bb);
	nxJoint->userData = joint;

	joint->setWorld(world);
	joint->setJoint(nxJoint);
	return joint;

}
pJointBall *pFactory::createBallJoint(CK3dEntity*a,CK3dEntity*b,
									  VxVector anchor,VxVector swingAxis,VxVector globalAxis,
									  bool collision,float maxForce,float maxTorque,const char* attributeName)
{
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{
			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;
			if (referenceObject)
			{
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_BALL,name.Str());
				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JBALL_BODY_B,other->GetID());
					
					SetParameterStructureValue<VxVector>(parAttribute,PS_JBALL_ANCHOR,anchor);
					SetParameterStructureValue<BOOL>(parAttribute,PS_JBALL_COLLISION,collision);
					SetParameterStructureValue<float>(parAttribute,PS_JBALL_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JBALL_MAX_TORQUE,maxTorque);
				}
			}
			return NULL;
		}
	}


	pJointBall *result;

	if (jointCheckPreRequisites(a,b,JT_Spherical)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);
	if (!world)
	{
		world = GetPMan()->getWorldByBody(b);
	}

	NxActor *a0 = NULL;
	if (ba)
	{
		a0 = ba->getActor();
	}
	NxActor *a1 = NULL;
	if (bb)
	{
		a1 = bb->getActor();
	}


	NxSphericalJointDesc desc;
	desc.actor[0] = a0;
	desc.actor[1] = a1;
	desc.swingAxis = pMath::getFrom(swingAxis);

	if (globalAxis.SquareMagnitude() > 0.01f )
		desc.setGlobalAxis(getFrom(globalAxis));


	/*desc.projectionDistance = (NxReal)0.15;
	desc.projectionMode = NX_JPM_POINT_MINDIST;*/

	
	if (!desc.isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Description failed");
		return NULL;
	}


	NxSphericalJoint*	nxJoint  = (NxSphericalJoint*)world->getScene()->createJoint(desc);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		return NULL;
	}

		//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointBall*joint = new pJointBall(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	nxJoint->setGlobalAnchor(pMath::getFrom(anchor));
	//nxJoint->setGlobalAxis(NxVec3(0,1,0));



	joint->setWorld(world);
	
	return joint;
}

int pFactory::jointCheckPreRequisites(CK3dEntity*_a,CK3dEntity*_b,int type)
{

		pRigidBody *a  = GetPMan()->getBody(_a);
		pRigidBody *b  = GetPMan()->getBody(_b);

		//bodies have already a joint together ? 
		if ( !a && !b)
		{
			return -1;
		}

		if (a && !a->isValid() )
		{
			return -1;
		}

		if (a && !GetPMan()->getWorldByBody(_a))
		{
			return -1;
		}

		if (b && !GetPMan()->getWorldByBody(_b))
		{
			return -1;
		}

		if (b && !b->isValid())
		{
			return -1;
		}


		if ( a && b )
		{
			pWorld*worldA  = GetPMan()->getWorldByBody(_a);
			pWorld*worldB  = GetPMan()->getWorldByBody(_b);
			if (!worldA || !worldB || (worldA!=worldB) || !worldA->isValid() )
			{
				return -1;
			}
		}


		return 1;
}

pJointDistance*pFactory::createDistanceJoint(CK3dEntity*a,CK3dEntity*b,
											 VxVector anchor0,VxVector anchor1,
											 float minDistance,float maxDistance,
											 pSpring sSettings,
											 bool collision,float maxForce,float maxTorque,const char* attributeName)
{
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;
	//////////////////////////////////////////////////////////////////////////
	//
	//	store as attributes only !
	//	
	if ( !GetPMan()->isValid()  )
	{
		if (!GetPMan()->GetContext()->IsPlaying())
		{
			CK3dEntity *referenceObject = a ? a : b ? b : NULL;
			CK3dEntity *other = b ? b :NULL;
			if (referenceObject)
			{
				XString name(attributeName);
				int attributeType  = _registerJointAttributeType(VTS_JOINT_DISTANCE,name.Str());

				//////////////////////////////////////////////////////////////////////////
				// we remove the old physic attribute : 
				if (referenceObject->HasAttribute(attributeType))
				{
					referenceObject->RemoveAttribute(attributeType);
				}
				referenceObject->SetAttribute(attributeType);
				//////////////////////////////////////////////////////////////////////////
				// store settings into attributes
				CKParameterOut *parAttribute =  referenceObject->GetAttributeParameter(attributeType);
				if (parAttribute)
				{
					if (other)
						SetParameterStructureValue<CK_ID>(parAttribute,PS_JDISTANCE_BODY_B,other->GetID());

					SetParameterStructureValue<VxVector>(parAttribute,PS_JDISTANCE_LOCAL_ANCHOR_A_POS,anchor0);
					SetParameterStructureValue<VxVector>(parAttribute,PS_JDISTANCE_LOCAL_ANCHOR_B_POS,anchor1);
					
					

					SetParameterStructureValue<BOOL>(parAttribute,PS_JDISTANCE_COLL,collision);
					SetParameterStructureValue<float>(parAttribute,PS_JDISTANCE_MIN_DISTANCE,minDistance);
					SetParameterStructureValue<float>(parAttribute,PS_JDISTANCE_MAX_DISTANCE,maxDistance);

					SetParameterStructureValue<float>(parAttribute,PS_JDISTANCE_MAX_FORCE,maxForce);
					SetParameterStructureValue<float>(parAttribute,PS_JDISTANCE_MAX_TORQUE,maxTorque);

					SetParameterStructureValue<float>(GetParameterFromStruct(parAttribute,PS_JDISTANCE_SPRING),0,sSettings.damper);
					SetParameterStructureValue<float>(GetParameterFromStruct(parAttribute,PS_JDISTANCE_SPRING),1,sSettings.spring);
					SetParameterStructureValue<float>(GetParameterFromStruct(parAttribute,PS_JDISTANCE_SPRING),2,sSettings.targetValue);

					SetParameterStructureValue<CK_ID>(parAttribute,PS_JDISTANCE_LOCAL_ANCHOR_A_REF,0);
					SetParameterStructureValue<CK_ID>(parAttribute,PS_JDISTANCE_LOCAL_ANCHOR_B_REF,0);
					
				}
			}
			return NULL;
		}
	}


	if (jointCheckPreRequisites(a,b,JT_Distance)  == -1 )
	{
		return NULL;
	}

	pRigidBody *ba  = GetPMan()->getBody(a);
	pRigidBody *bb  = GetPMan()->getBody(b);
	pWorld*world  = GetPMan()->getWorldByBody(a);

	//////////////////////////////////////////////////////////////////////////
	//	nx object : 
	NxDistanceJointDesc descr;
	descr.actor[0]=ba->getActor();
	descr.actor[1]= b ?  bb->getActor() : NULL ;




	descr.localAnchor[0] = pMath::getFrom(anchor0);
	descr.localAnchor[1] = pMath::getFrom(anchor1);

	descr.minDistance = minDistance;
	descr.maxDistance = maxDistance;
	
	if (minDistance!=0.0f)
		 descr.flags|=NX_DJF_MIN_DISTANCE_ENABLED;
	if (maxDistance!=0.0f)
		 descr.flags|=NX_DJF_MAX_DISTANCE_ENABLED;
	
	if(sSettings.damper!=0.0f || sSettings.damper!=0.0f )
	{	
		descr.flags|=NX_DJF_SPRING_ENABLED;
		NxSpringDesc sDescr;
		sDescr.damper = sSettings.damper;
		sDescr.spring = sSettings.spring;
		sDescr.targetValue = sSettings.targetValue;
		descr.spring = sDescr;

	}
	
	NxDistanceJoint *nxJoint  = (NxDistanceJoint*)world->getScene()->createJoint(descr);
	if (!nxJoint)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed by PhysX-SDK");
		int v = descr.isValid();
		return NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////,
	//		vt object :
	pJointDistance *joint = new pJointDistance(ba,bb);
	nxJoint->userData = joint;
	joint->setJoint(nxJoint);
	joint->setWorld(world);

	return joint;

}
pJoint*pFactory::createJoint(CK3dEntity*_a,CK3dEntity*_b,int type)	
{
	return NULL ;
}


pJointSettings*pFactory::CreateJointSettings(const XString nodeName/* = */,const TiXmlDocument * doc /* = NULL */)
{

	/*
	pJointSettings *result  = new pJointSettings();
	if (nodeName.Length() && doc)
	{
		const TiXmlElement *root = GetFirstDocElement(doc->RootElement());
		for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
		{
			if ( (child->Type() == TiXmlNode::ELEMENT))
			{
				XString name = child->Value();

				if (!strcmp(child->Value(), "JointSettings" ) )
				{
					const TiXmlElement *element = (const TiXmlElement*)child;
					if (element->Type()  == TiXmlNode::ELEMENT )
					{
						if(!strcmp( element->Attribute("name"),nodeName.CStr() ) )
						{

							float vF=0.0f;
							int res;
							
							//////////////////////////////////////////////////////////////////////////
							res = element->QueryFloatAttribute("LowStop",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->LowStop(vF);
							}

							//////////////////////////////////////////////////////////////////////////
							res = element->QueryFloatAttribute("HiStop",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->HighStop(vF);
							}

							//////////////////////////////////////////////////////////////////////////
							res = element->QueryFloatAttribute("LowStop2",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->LowStop2(vF);
							}

							//////////////////////////////////////////////////////////////////////////
							res = element->QueryFloatAttribute("HiStop2",&vF);
							if (res == TIXML_SUCCESS)
							{
								result->HighStop2(vF);
							}

					
							return result;
						}
					}
				}
			}
		}
	}
	*/
	return NULL;
}

pJointSettings*pFactory::CreateJointSettings(const char* nodeName,const char *filename)
{

	/*
	XString fname(filename);
	XString nName(nodeName);
	if ( nName.Length() && fname.Length() )
	{
		TiXmlDocument * document  = getDocument(fname);
		if (document)
		{
			pJointSettings *result = CreateJointSettings(nodeName,document);
			if ( result)
			{
				delete document;
				return result;
			}
		}
	}
	*/
	return NULL;

}

pJointLimit pFactory::createLimitFromParameter(CKParameter *par)
{


	pJointLimit result;
	
	CKContext *ctx = GetPMan()->GetContext();


	if (par)
	{
			CKParameter* pout = NULL;
			CK_ID* ids = (CK_ID*)par->GetReadDataPtr();
			
			float value,restitution,hardness;
			
			pout = (CKParameterOut*)ctx->GetObject(ids[0]);
			pout->GetValue(&value);

			pout = (CKParameterOut*)ctx->GetObject(ids[1]);
			pout->GetValue(&restitution);
			pout = (CKParameterOut*)ctx->GetObject(ids[2]);
			pout->GetValue(&hardness);

			
			result.value = value;
			result.restitution = restitution;
			result.hardness =hardness;
			return result;
	}
	return result;
}

pSpring pFactory::createSpringFromParameter(CKParameter *par)
{
	
	pSpring result;

	CKContext *ctx = GetPMan()->GetContext();



	if (par)
	{
			CKParameter* pout = NULL;
			CK_ID* ids = (CK_ID*)par->GetReadDataPtr();
			
			float damping,spring,targetValue;
			pout = (CKParameterOut*)ctx->GetObject(ids[0]);
			pout->GetValue(&damping);

			pout = (CKParameterOut*)ctx->GetObject(ids[1]);
			pout->GetValue(&spring);
			pout = (CKParameterOut*)ctx->GetObject(ids[2]);
			pout->GetValue(&targetValue);

			result.spring = spring;
			result.damper = damping;
			result.targetValue = targetValue;
			return result;
	}
	return result;
}



pMotor pFactory::createMotorFromParameter(CKParameter *par)
{
	pMotor result;
	CKContext *ctx = GetPMan()->GetContext();



	if (par)
	{
		CKParameter* pout = NULL;
		CK_ID* ids = (CK_ID*)par->GetReadDataPtr();

		float targetVel,maxF;
		int freeSpin;
		pout = (CKParameterOut*)ctx->GetObject(ids[0]);
		pout->GetValue(&targetVel);

		pout = (CKParameterOut*)ctx->GetObject(ids[1]);
		pout->GetValue(&maxF);
		pout = (CKParameterOut*)ctx->GetObject(ids[2]);
		pout->GetValue(&freeSpin);
		result.targetVelocity = targetVel;
		result.maximumForce = maxF;
		result.freeSpin = freeSpin;
		return result;
	}
	return result;
}

pJoint*pFactory::GetJoint(CK3dEntity*_a,CK3dEntity*_b)
{
	
	/*
	if (!_a ||!_b)
	{
		return 0;
	}

	pRigidBody *ba  = getBody(_a);
	pRigidBody *bb  = getBody(_b);

	if ( !ba || !bb || ba == bb )
		return NULL;

	//we check whether both associated bodies are in the same world : 
	if ( ba->World()  !=  bb->World()  )
	{
		return 0;
	}

	OdeBodyType oba  = ba->GetOdeBody();
	OdeBodyType obb  = bb->GetOdeBody();
	
	if ( !oba || !obb  )
		return NULL;


	for (int i   = 0 ; i < dBodyGetNumJoints(oba)  ;  i++)
	{
		dJointID jID = dBodyGetJoint(oba,i);
		pJoint *joint = static_cast<pJoint*>(dJointGetData(jID));
		if (joint && joint->GetOdeBodyB() == obb)
		{
			return joint;
		}
	}

	for (int i   = 0 ; i < dBodyGetNumJoints(obb)  ;  i++)
	{
		dJointID jID = dBodyGetJoint(obb,i);
		pJoint *joint = static_cast<pJoint*>(dJointGetData(jID));
		if (joint && joint->GetOdeBodyB() == oba)
		{
			return joint;
		}
	}
	*/
	return NULL;
}

