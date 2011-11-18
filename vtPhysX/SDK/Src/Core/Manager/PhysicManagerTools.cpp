#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"

#include "pCommon.h"
#include "IParameter.h"
#include "vtBBHelper.h"
#include "xDebugTools.h"
#include "pCallbackSignature.h"


int PhysicManager::_checkListCheck()
{
	int result = 0 ;
	while(bodyListCheck.Size())
	{	
		result ++;
		bodyListCheck.PopBack();
	}
	return result;
}

int PhysicManager::_checkRemovalList()
{

	int result = 0 ; 

	if (bodyListRemove.Size() < 1)
		return 0;

	pWorldMapIt wit  = getWorlds()->Begin();

	bool wasDeleting = false;
	while(wit != getWorlds()->End())
	{
		pWorld *w = *wit;
		
			int cCount = bodyListRemove.Size();

			//for (CK3dEntity** it = bodyListRemove.Begin(); it != bodyListRemove.End(); ++it)
			//int i = 0 ; i  < bodyListRemove.Size() ; i ++ )
			while(bodyListRemove.Size())
			{	
				CK_ID id = *bodyListRemove.At(0);
				CK3dEntity * ent  = (CK3dEntity*)GetPMan()->GetContext()->GetObject(id);
				if(ent)
				{
					pRigidBody* body = GetPMan()->getBody(ent);
					if (body)
					{
						xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"deleting body ");
						body->destroy();
						SAFE_DELETE(body)
						cCount = bodyListRemove.Size();
						result ++;
					}
				}
				bodyListRemove.PopFront();
			}
		wit++;
	}
	//bodyListRemove.Clear();
	//_cleanOrphanedJoints();
	if(wasDeleting)
	{
	}
	//getScene()->checkResults(NX_ALL_FINISHED,true)
/*
	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		w->getScene()->fetchResults(NX_ALL_FINISHED,false);
		it++;
	}
	*/
	return result;
}


XString printSignature(BBParameter pAarray[],int size)
{

	CKParameterManager *pm = GetPMan()->GetContext()->GetParameterManager();
	XString signature;
	
	for(int i  = 0 ; i < size ; i ++ )
	{
		BBParameter *par = &pAarray[i];
		signature << "\t\t Parameter :" << par->name << ": must be type of " << pm->ParameterGuidToName(par->guid) << " \n ";
	}
	return signature;

}

bool isCompatible(CKParameterManager *pm,CKGUID a,CKGUID b)
{

	if (a == b)
	{
		return true;
	}
	
	//----------------------------------------------------------------
	//
	// special case custom flags or enumeration, integer is possible
	//
	if	(	(	pm->GetEnumDescByType(pm->ParameterGuidToType(a)) ||
				pm->GetFlagsDescByType(pm->ParameterGuidToType(a)) 
			) &&
			pm->IsDerivedFrom(b,CKPGUID_INT)  
		)
	{
		return true;
	}
	return false;
}

int checkInputSignature(CKBehavior *beh,BBParameter *pAarray,int size)
{

	int result = true;
	int parameterCount = beh->GetInputParameterCount();
	if ( parameterCount < size)
	{
		result = 0;
	}

	CKParameterManager *pm = GetPMan()->GetContext()->GetParameterManager();
	for (int i = 0 ; i < size ; i ++)
	{
		CKParameterIn *inPar = beh->GetInputParameter(i);
		BBParameter *par = &pAarray[i];

		if (!isCompatible(pm,par->guid,inPar->GetGUID()))
		{
			result =  0;
		}
	}
	return result;
}

int checkOutputSignature(CKBehavior *beh,BBParameter *pAarray,int size)
{

	int result = 1;
	int parameterCount = beh->GetOutputParameterCount();
	if ( parameterCount < size)
	{
		result= 0;
	}

	CKParameterManager *pm = GetPMan()->GetContext()->GetParameterManager();
	for (int i = 0 ; i < size ; i ++)
	{
		CKParameterOut *inPar = beh->GetOutputParameter(i);
		BBParameter *par = &pAarray[i];

		if (!isCompatible(pm,par->guid,inPar->GetGUID()))
		{
			result= 0;
		}
	}

	
	return result;
}

bool PhysicManager::checkCallbackSignature(CKBehavior *beh,int type,XString& errMessage)
{

	if (!beh)
	{
		return false;
	}
	CKParameterManager *pm = GetPMan()->GetContext()->GetParameterManager();

	int parameterCount = beh->GetInputParameterCount();
	switch(type)
	{
		//----------------------------------------------------------------
		//
		// Collisions notify
		//
		case CB_OnContactNotify:
		{
			errMessage.Format("\n\t Input parameters for collisions notify must have this signature : \n"); 
			if ( parameterCount < BB_PMAP_SIZE(pInMapContactCallback))
			{
				errMessage << printSignature(pInMapContactCallback,BB_PMAP_SIZE(pInMapContactCallback) );
				return false;
			}
			for (int i = 0 ; i < BB_PMAP_SIZE(pInMapContactCallback) ; i ++)
			{
				CKParameterIn *inPar = beh->GetInputParameter(i);
				BBParameter *par = &pInMapContactCallback[i];
				
				if (!isCompatible(pm,par->guid,inPar->GetGUID()))
				{
					errMessage << printSignature(pInMapContactCallback,BB_PMAP_SIZE(pInMapContactCallback) );
					return false;
				}
			}
			return true;
		}

		//----------------------------------------------------------------
		//
		// wheel contact modify
		//
		case CB_OnWheelContactModify:
			{
			
				//----------------------------------------------------------------
				//
				// check input parameters
				//
				bool result = true;
				errMessage.Format("\n\t Input parameters for wheel contact modification must have this signature : \n"); 
				int ok = checkInputSignature(beh,pInMapWheelContactModifyCallback,BB_PMAP_SIZE(pInMapWheelContactModifyCallback));
				if ( !ok )
				{
					errMessage << printSignature(pInMapWheelContactModifyCallback,BB_PMAP_SIZE(pInMapWheelContactModifyCallback) );
					result =false;
				}
				//----------------------------------------------------------------
				//
				// check output parameters
				//
				errMessage.Format("\n\t Input parameters for wheel contact modification must have this signature : \n"); 
				ok  = checkOutputSignature(beh,pOutMapWheelContactModifyCallback,BB_PMAP_SIZE(pOutMapWheelContactModifyCallback));
				if (!ok)
				{
					errMessage << printSignature(pOutMapWheelContactModifyCallback,BB_PMAP_SIZE(pOutMapWheelContactModifyCallback) );
					result = false;
				}
				return result;
			}
		//----------------------------------------------------------------
		//
		// Collisions notify
		//
		case CB_OnRayCastHit:
		{
			//----------------------------------------------------------------
			//
			// check input parameters
			//
			errMessage.Format("\n\t Input parameters for raycast hit report must have this signature : \n"); 
			int ok = checkInputSignature(beh,pInMapRaycastHitCallback,BB_PMAP_SIZE(pInMapRaycastHitCallback));
			if ( !ok )
			{
				errMessage << printSignature(pInMapRaycastHitCallback,BB_PMAP_SIZE(pInMapRaycastHitCallback));
				return false;
			}
			return true;
		}

		//----------------------------------------------------------------
		//
		// Collisions notify
		//
		case CB_OnContactModify:
		{
			//----------------------------------------------------------------
			//
			// check input parameters
			//
			bool result = true;
			errMessage.Format("\n\t Input parameters for contact modification must have this signature : \n"); 
			int ok = checkInputSignature(beh,pInMapContactModifyCallback,BB_PMAP_SIZE(pInMapContactModifyCallback));
			if ( !ok )
			{
				errMessage << printSignature(pInMapContactModifyCallback,BB_PMAP_SIZE(pInMapContactModifyCallback) );
				result =false;
			}
			//----------------------------------------------------------------
			//
			// check output parameters
			//
			errMessage.Format("\n\t Output parameters for contact modification must have this signature : \n"); 
			ok  = checkOutputSignature(beh,pOutMapContactModifyCallback,BB_PMAP_SIZE(pOutMapContactModifyCallback));
			if (!ok)
			{
				errMessage << printSignature(pOutMapContactModifyCallback,BB_PMAP_SIZE(pOutMapContactModifyCallback) );
				result = false;
			}
			return result;
		}
		//----------------------------------------------------------------
		//
		// trigger
		//
		//----------------------------------------------------------------
		//
		// Collisions notify
		//
		case CB_OnTrigger:
		{
			errMessage.Format("\n\t Input parameters for trigger notify must have this signature : \n"); 
			if ( parameterCount < BB_PMAP_SIZE(pInMapTriggerCallback))
			{
				errMessage << printSignature(pInMapTriggerCallback,BB_PMAP_SIZE(pInMapTriggerCallback) );
				return false;
			}
			for (int i = 0 ; i < BB_PMAP_SIZE(pInMapTriggerCallback) ; i ++)
			{
				CKParameterIn *inPar = beh->GetInputParameter(i);
				BBParameter *par = &pInMapTriggerCallback[i];

				if (!isCompatible(pm,par->guid,inPar->GetGUID()))
				{
					errMessage << printSignature(pInMapTriggerCallback,BB_PMAP_SIZE(pInMapTriggerCallback) );
					return false;
				}
			}
			return true;
		}


		//----------------------------------------------------------------
		//
		// Joint Break Script
		//
		case CB_OnJointBreak:
		{
			errMessage.Format("\n\t Input parameters for joint breaks must have this signature : \n"); 
			if ( parameterCount < BB_PMAP_SIZE(pInMapJointBreakCallback))
			{
				errMessage << printSignature(pInMapJointBreakCallback,BB_PMAP_SIZE(pInMapJointBreakCallback) );
				return false;
			}
			for (int i = 0 ; i < BB_PMAP_SIZE(pInMapJointBreakCallback) ; i ++)
			{
				CKParameterIn *inPar = beh->GetInputParameter(i);
				BBParameter *par = &pInMapJointBreakCallback[i];

				if (!isCompatible(pm,par->guid,inPar->GetGUID()))
				{
					errMessage << printSignature(pInMapJointBreakCallback,BB_PMAP_SIZE(pInMapJointBreakCallback) );
					return false;
				}
			}
			return true;
		}

	}
	return false;
}
