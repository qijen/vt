#include <StdAfx.h>
#include "vtPhysXAll.h"


using namespace xUtils;
using namespace vtAgeia;


#include "vtAttributeHelper.h"


//  [3/31/2009 master] Temp ! 
#include "IParameter.h"



//################################################################
//
// Declaration of rigid body related attribute callback function
//
int registerRigidBody(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{

	using namespace vtTools::ParameterTools;
	int error = 0 ;


	//----------------------------------------------------------------
	//
	// Sanity checks :
	//
	assert(target);
	
	CKAttributeManager* attman = GetPMan()->GetContext()->GetAttributeManager();
	
	CKParameterOut *bodyParameter = target->GetAttributeParameter(attributeType);
	
	assert(bodyParameter);
	CKStructHelper sHelper(bodyParameter);
	if (sHelper.GetMemberCount() ==0 )
	{	//happens when dev is being opened and loads a cmo with physic objects.
		return -1;
	}

	

	if (set)
	{

		pRigidBody* body = GetPMan()->getBody(target);
		if (body)
		{
			return true;
		}
		//----------------------------------------------------------------
		//
		// attribute has been added at run-time, post pone the registration to the next frame.
		//
		if ( isPostJob  && GetPMan()->GetContext()->IsPlaying() )
		{
			pAttributePostObject postAttObject(target->GetID(),registerRigidBody,attributeType);
			GetPMan()->getAttributePostObjects().PushBack(postAttObject);
			return true;
		}
		if(!GetPMan()->GetContext()->IsPlaying())
			return true;


		pObjectDescr *objDecr = new pObjectDescr();
		IParameter::Instance()->copyTo(objDecr,bodyParameter);
		
		//----------------------------------------------------------------
		//
		// Pivot override ? 
		//
		int attPivot = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_PIVOT_OFFSET);
		if (target->HasAttribute(attPivot)){
			IParameter::Instance()->copyTo(objDecr->pivot,target->GetAttributeParameter(attPivot));
			objDecr->mask << OD_Pivot;
		}

		//----------------------------------------------------------------
		//
		// Pivot override ? 
		//
		int attMass = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_MASS_SETUP);
		if (target->HasAttribute(attMass)){
			IParameter::Instance()->copyTo(objDecr->mass,target->GetAttributeParameter(attMass));
			objDecr->mask << OD_Mass;
		}

		//----------------------------------------------------------------
		//
		// register the body
		//
		if(!body && !(objDecr->flags & BF_SubShape) )
		{
			body  = pFactory::Instance()->createRigidBody(target,*objDecr);
			if (body)
			{
				body->setInitialDescription(objDecr);
			}
		}
		
		//SAFE_DELETE(objDecr);


	}else
	{

		pRigidBody* body = GetPMan()->getBody(target);
		if (body)
		{
			body->getWorld()->deleteBody(body);
		}
	}
	error++;
	return error;
}


/*
//################################################################
//
// Not being used
//
int registerRigidBody(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{

if (!GetPMan()->GetContext()->IsPlaying() && set)
{

using namespace vtTools::ParameterTools;
CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);

XString errString;
errString.Format("attr added");
xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errString.Str());

XString value  = getDefaultValue(distanceParameter);
}

return 0;
}

*/
