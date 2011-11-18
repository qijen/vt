#include <StdAfx.h>

#include "vtPhysXAll.h"
#include "pVehicleAll.h"
#include "pWorldSettings.h"


#include <stdlib.h>
#include <exception>


using namespace vtTools::AttributeTools;



int pWorld::hadBrokenJoint()
{

	int nbActors = getScene()->getNbActors();
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor && actor->userData != NULL)
		{
			pRigidBody* body = (pRigidBody*)actor->userData;
			if (body && body->hasBrokenJoint)
			{
				return 1;
			}
		}
	}

	return 0;
}

void pWorld::cleanBrokenJoints()
{

	int nbActors = getScene()->getNbActors();
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor && actor->userData != NULL)
		{
			pRigidBody* body = (pRigidBody*)actor->userData;
			if (body )
			{
				body->hasBrokenJoint = false;
			}
		}
	}
}

void pWorld::step(float stepsize)
{
	if (getScene())
	{

		CKVariableManager* vm = GetPMan()->GetContext()->GetVariableManager();
		if( !vm )
			return;

		int disable = 0 ;
		vm->GetValue("Physic/Disable Physics",&disable);
		if (disable)
		{
			return;
		}


		//int hasBroken = hadBrokenJoint();



		NxU32 nbTransforms = 0;    
		NxActiveTransform *activeTransforms = getScene()->getActiveTransforms(nbTransforms);    

		updateVehicles(stepsize);

		updateClothes();

#ifdef HAS_FLUIDS
		updateFluids();
#endif


		vtAgeia::pWorldSettings *wSettings = GetPMan()->getDefaultWorldSettings();

		if (wSettings->isFixedTime())
			getScene()->setTiming(wSettings->getFixedTimeStep() * 10.0f, wSettings->getNumIterations() , NX_TIMESTEP_FIXED);
		/*else			getScene()->setTiming( (stepsize * 100 ) / wSettings->getNumIterations() , wSettings->getNumIterations() , NX_TIMESTEP_VARIABLE);		*/
		/*
		NxU32 nbIter = 0 ;
		int op =2;
		NxTimeStepMethod mode = NX_TIMESTEP_FIXED;*/
		/*float stepSz;getScene()->getTiming(stepSz,nbIter,op);*/
	
		int nbDeleted = GetPMan()->_checkRemovalList();
		if (nbDeleted)
		{
			//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"objects to delete");
			//return;
		}


		int nbReset = GetPMan()->_getRestoreMap()->Size();
		if(nbReset)
		{
			GetPMan()->_checkResetList();
			//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"objects to reset");
			GetPMan()->_getResetList().Clear();
			return;
		}

		int nbCheck = GetPMan()->getCheckList().Size();
		if(nbCheck)
		{
			//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"objects to check");
			GetPMan()->_checkObjectsByAttribute(GetPMan()->GetContext()->GetCurrentLevel()->GetCurrentScene());
			GetPMan()->_checkListCheck();
			if(nbCheck)
			{
				if(nbCheck && GetPMan()->sceneWasChanged )
				{

					//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"objects to check & and scene change, aborting");
				}
				//GetPMan()->checkWorlds();drdrdr
				//return;
			}
		}
		



			if(!getScene()->checkResults(NX_RIGID_BODY_FINISHED,false))
			{
				getScene()->simulate(stepsize * GetPMan()->GetContext()->GetTimeManager()->GetTimeScaleFactor() );
			}
			

			if(getScene()->checkResults(NX_RIGID_BODY_FINISHED,false) /*&& hasBroken==0*/)
			{
				//if(nbDeleted)					return;
				int nbNewPostObjects = GetPMan()->getAttributePostObjects().Size();
				/*
				if(m_bCompletedLastFrame)
				{
				//getScene()->simulate(stepsize * GetPMan()->GetContext()->GetTimeManager()->GetTimeScaleFactor() );
				getScene()->simulate( m_fTimeSinceLastCallToSimulate );
				m_bCompletedLastFrame = false;
				m_fTimeSinceLastCallToSimulate = 0.0;
				}

				m_fTimeSinceLastCallToSimulate +=(stepsize * GetPMan()->GetContext()->GetTimeManager()->GetTimeScaleFactor());

				if(getScene()->checkResults(NX_ALL_FINISHED,false))
				{*/
				m_bCompletedLastFrame = true;
				if(nbTransforms && activeTransforms && !nbDeleted && !nbReset && !nbCheck && !nbNewPostObjects /*&& hasBroken== 0*/) 
				{        
					for(NxU32 i = 0; i < nbTransforms; ++i)        
					{            
						// the user data of the actor holds the game object pointer            
						NxActor *actor = activeTransforms[i].actor;
						//XString name = actor->getName();
						if (actor !=NULL)
						{
							pRigidBody *body = static_cast<pRigidBody*>(actor->userData);

							// update the game object's transform to match the NxActor
							if(body !=NULL)
							{
								assert(body && body->getEntID());
								NxMat34 transformation  = activeTransforms[i].actor2World;
								VxQuaternion rot  = pMath::getFrom(transformation.M);
								VxVector pos = pMath::getFrom(transformation.t);
								VxVector pos2 = pMath::getFrom(transformation.t);
								
								

								CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(body->getEntID()));
								if (ent !=NULL && ent->GetClassID() != CKCID_3DOBJECT)
								{
									continue;
								}
								//assert(ent && ent->GetID());

								/*
								XString errMessage= "update body";
								errMessage+=ent->GetName();
								xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errMessage.Str());
								*/

								//NxShape *mainShape=body->getMainShape();
								if (ent !=NULL)
								{
									int hier = (body->getFlags()  & BF_Hierarchy );
									ent->SetPosition(&pos,NULL);
									ent->SetQuaternion(&rot,NULL);	
									body->updateSubShapes();
									body->onMove(true,true,pos,rot);
									
									if (body->hasWheels())
									{
										body->wakeUp();
									}
								}else{
									//xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Invalid entity due simulation.");
								}
							
							}else
							{
								//xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Invalid Body due simulation.");
							}
						}
					}    
				}

				NxU32 error=0;
				getScene()->flushStream();
				getScene()->fetchResults(NX_ALL_FINISHED,true,&error);
				if(error !=0)
				{
					XString err="error fetching results : ";
					err << error;
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,err.Str());
				}
								
				
			}else
			{
				//cleanBrokenJoints();
				//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"world has broken joints !");
				return;
			}
		
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//
	//	update Virtools from hardware objects



	int nbActors = getScene()->getNbActors();
	NxActor** actors = getScene()->getActors();
	if(getCompartment())
	{
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if(actor && actor->userData != NULL)
			{
				pRigidBody* body = (pRigidBody*)actor->userData;
				if (body && body->getActor()->getCompartment())
				{
					NxMat34 transformation  = actor->getGlobalPose();
					VxQuaternion rot  = pMath::getFrom(transformation.M);
					VxVector pos = pMath::getFrom(transformation.t);
					VxVector pos2 = pMath::getFrom(transformation.t);
					CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(body->getEntID()));
					//NxShape *mainShape=body->getMainShape();
					if (ent)
					{

						//int hier = (body->getFlags()  & BF_Hierarchy );
						ent->SetPosition(&pos,NULL);
						ent->SetQuaternion(&rot,NULL);	
						body->updateSubShapes();
						body->onMove(true,true,pos,rot);
						if (body->hasWheels())
						{
							//		body->wakeUp();
						}
					}else{
						//xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Invalid Body due simulation.");
					}
				}
			}
		}
	}

	/*
	if(getScene()->checkResults(NX_RIGID_BODY_FINISHED))
	{
	NxU32 error;
	getScene()->flushStream();
	getScene()->fetchResults(NX_RIGID_BODY_FINISHED,true,&error);
	}
	*/

	/*

	int err = error;

	NxReal maxTimestep;
	NxTimeStepMethod method;
	NxU32 maxIter;
	NxU32 numSubSteps;
	//getScene()->getTiming(maxTimestep, maxIter, method, &numSubSteps);


	int op2 = 3;
	op2++;
	*/

}


int pWorld::onPreProcess()
{


	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return 0;
	}
	
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		pRigidBody *body = static_cast<pRigidBody*>(actor->userData);
		if (body)
		{
			pVehicle *v = body->getVehicle();
			if (!v && body->hasWheels())
			{
				int nbShapes = actor->getNbShapes();
				NxShape ** slist = (NxShape **)actor->getShapes();
				for (NxU32 j=0; j<nbShapes; j++)
				{
					NxShape *s = slist[j];
					if (s)
					{
						pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
						if (sinfo && sinfo->wheel !=NULL)
						{
							pWheel *wheel = sinfo->wheel;
							pWheel2* wheel2 = dynamic_cast<pWheel2*>(wheel);

							if ( wheel2 && (wheel2->getCallMask().test(CB_OnPreProcess)) )
							{
								wheel2->onPreProcess();
							}

						}
					}
				}
			}

			//if (v && (v->getCallMask().test(CB_OnPreProcess)))
			//	v->onPreProcess();
		}
	}

	return 1;
}
int pWorld::onPostProcess()
{

	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return 0;
	}

	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		pRigidBody *body = static_cast<pRigidBody*>(actor->userData);
		if (body)
		{
			pVehicle *v = body->getVehicle();
			if (!v && body->hasWheels())
			{
				int nbShapes = actor->getNbShapes();
				NxShape ** slist = (NxShape **)actor->getShapes();
				for (NxU32 j=0; j<nbShapes; j++)
				{
					NxShape *s = slist[j];
					if (s)
					{
						pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
						if (sinfo && sinfo->wheel !=NULL)
						{
							pWheel *wheel = sinfo->wheel;
							pWheel2* wheel2 = dynamic_cast<pWheel2*>(wheel);

							if ( wheel2 && (wheel2->getCallMask().test(CB_OnPostProcess)))
							{
								wheel2->onPostProcess();
							}

						}
					}
				}
			}
			//if (v && (v->getCallMask().test(CB_OnPostProcess)))
			//v->onPostProcess();
		}
	}

	return 1;
}
