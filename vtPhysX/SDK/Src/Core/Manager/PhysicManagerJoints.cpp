#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"

pJoint*PhysicManager::getJoint(CK3dEntity*referenceA,CK3dEntity*referenceB/* =NULL */,JType type/* =JT_Any */)
{

	pJoint *result = NULL;

	pRigidBody *a	= GetPMan()->getBody(referenceA);
	pRigidBody *b	= GetPMan()->getBody(referenceB);

	pWorld *wA		= a ? a->getWorld() : NULL;
	pWorld *wB		= b ? b->getWorld()	: NULL;
	pWorld *worldFinal = NULL;


	bool oneBodyJoint = false;// body with world frame ? 
	CK3dEntity* oneBodyJointReference =NULL;

	
	//----------------------------------------------------------------
	//
	// Sanity Checks
	//
	XString errorString;
	
	//	Reference A physicalized at all ? 
	if ( !referenceA && !referenceB )
	{
		errorString.Format("No reference specified");
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}

	//	Reference A IS NOT Reference B ?
	if ( (referenceA!=NULL && referenceB!=NULL)  && (a==b) )
	{
		errorString.Format("Reference A (%s) is the same as Reference B (%s)",referenceA->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}

	//	Reference A physicalized at all ? 
	if (referenceA && !a){
		
		errorString.Format("Reference A (%s) valid but not physicalized",referenceA->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}

	//	Reference B physicalized at all ? 
	if (referenceB && !b){

		errorString.Format("Reference B (%s) valid but not physicalized",referenceB->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}


	//	world of object a valid ? 
	if (a && !wA){
		errorString.Format("Reference A (%s) is physicalized but has no valid world object",referenceA->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}

	//	world of object b valid ? 
	if (b && !wB){
		errorString.Format("Reference B (%s) is physicalized but has no valid world object",referenceB->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}

	// rigid bodies are in the same world
	if (		(wA!=NULL && wB!=NULL)		&&		(wA!=wB)	)
	{
		errorString.Format("Reference A (%s) and B(%s) is physicalized but are not in the same world",referenceA->GetName(),referenceB->GetName());
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errorString.Str());
		return result;
	}
	
	//----------------------------------------------------------------
	//
	// Preparing lookup 
	//
	if (	 wA )
		worldFinal = wA;
	if (	 wB )
		worldFinal = wB;


	//	constraint is attached to world frame ? 
	if ( (a && !b)	|| (b && !a) )
		oneBodyJoint = true;

	//	world frame constraint, track the reference 
	if (oneBodyJoint)
	{
		if ( a )
			oneBodyJointReference = referenceA;
		if ( b )
			oneBodyJointReference = referenceB;
	}


	//----------------------------------------------------------------
	//
	// Parse the scene joints
	//
	NxU32	jointCount = worldFinal->getScene()->getNbJoints();
	if (jointCount)
	{
		NxArray< NxJoint * > joints;

		worldFinal->getScene()->resetJointIterator();
		for	(NxU32 i = 0;	i <	jointCount;	++i)
		{
			NxJoint	*j = worldFinal->getScene()->getNextJoint();

			pJoint *_cJoint = static_cast<pJoint*>( j->userData );
			if (!_cJoint)
				continue;

			//----------------------------------------------------------------
			//
			// Special case : has a joint at all, only for world frame constraints
			//
			if (type == JT_Any)
			{
					if (		oneBodyJoint &&	
							_cJoint->GetVTEntA() == oneBodyJointReference ||
							_cJoint->GetVTEntB() == oneBodyJointReference 
					   )
					return _cJoint;
			}

			//----------------------------------------------------------------
			//
			// Specific joint type
			//
			if ( j->getType()  == type)
			{

				//----------------------------------------------------------------
				//
				// world constraint joint
				//
				if (oneBodyJoint &&
					_cJoint->GetVTEntA() == oneBodyJointReference ||
					_cJoint->GetVTEntB() == oneBodyJointReference
				)
				return _cJoint;
				
				//----------------------------------------------------------------
				//
				// Two references given
				//
				if (	_cJoint->GetVTEntA() == referenceA && _cJoint->GetVTEntB() == referenceB )
					return _cJoint;

				if (	_cJoint->GetVTEntA() == referenceB && _cJoint->GetVTEntB() == referenceA )
					return _cJoint;

			}
		}
	}
	return NULL;
}