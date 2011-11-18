#include <StdAfx.h>

#include "vtPhysXAll.h"

#include "vtStructHelper.h"

//################################################################
//
// Prototype
//
XString getDefaultValue(CKParameter *inputParameter)
{	
	CustomParametersArrayType& inputArray =  GetPMan()->_getCustomStructures();
	XString result;

	int s = inputArray.Size();



	CustomParametersArrayIteratorType it = inputArray.Find(inputParameter->GetGUID());
	if (it == inputArray.End())
		return result;

	using namespace vtTools::ParameterTools;

	int x = 0;
	CustomStructure *cStruct = *it;
	if (cStruct)
	{
		cStruct->getArray().size();
	}

	CKParameterTypeDesc *tdescr = GetPMan()->GetContext()->GetParameterManager()->GetParameterTypeDescription( inputParameter->GetType() );
	if( (tdescr->dwFlags & CKPARAMETERTYPE_STRUCT) == 0x00000010 )
	{
		int y = cStruct->getArray().size();
		int y2 = cStruct->getArray().size();
	}else{
	}

	return result;
}

//################################################################
//
// Not being used
//
void rigidBodyAttributeCallback(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg)
{

	//	recheckWorldsFunc(AttribType,Set,obj,arg);

}

//################################################################
//
// Body functions
//
pRigidBody*PhysicManager::getBody(const char*name,int flags/* =0 */)
{

	for (pWorldMapIt it  = getWorlds()->Begin(); it!=getWorlds()->End(); it++)
	{
		pWorld *w = *it;
		if(w)
		{
			int nbActors = w->getScene()->getNbActors();
			NxActor** actors = w->getScene()->getActors();
			while(nbActors--)
			{
				NxActor* actor = *actors++;
				if(actor->userData != NULL)
				{
					if (!strcmp(actor->getName(),name) )
					{

						pRigidBody* body =static_cast<pRigidBody*>(actor->userData);
						if (body)
						{
							return body;
						}
					}
				}
			}
		}
	}
	return 0;
}
pRigidBody*PhysicManager::getBody(CK3dEntity *ent)
{

	pWorld* w  = getWorldByBody(ent);
	if (w)
	{

		pRigidBody *body  = w->getBody(ent);
		if (body)
		{
			return body;
		}
	}

	return NULL;
}


