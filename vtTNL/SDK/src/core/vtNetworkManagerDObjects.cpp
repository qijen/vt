#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"
#include "vtNetworkManager.h"
#include <xDOStructs.h>
#include "xMathTools.h"

//#undef  TNL::Point3F
int counter = 0 ;


#include "xDistributedPoint3F.h"
#include "xDistributedPoint4F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"

#include "xDistributedSession.h"


void
vtNetworkManager::performGhostUpdates()
{

	//////////////////////////////////////////////////////////////////////////
	//this is to store states in our dist objects

	xNetInterface *nInterface = GetClientNetInterface();
	if (!nInterface) return;

	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = nInterface->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	if (nInterface->IsServer())return;
	//////////////////////////////////////////////////////////////////////////
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			xDistributedClass *_class = dobj->getDistributedClass();
			if (_class != NULL )
			{
				
				
				/************************************************************************/
				/*                                                                      */
				/************************************************************************/
				if (_class->getEnitityType() == E_DC_BTYPE_SESSION )
				{

					xDistributedSession*session = static_cast<xDistributedSession*>(dobj);
					if (session)
					{
						TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();
						xDistributedPropertyArrayType &props = *session->getDistributedPorperties();

						int gFlags = session->getGhostUpdateBits().getMask();

						for (unsigned int i = 0  ;  i < props.size() ; i++ )
						{
							xDistributedProperty *prop  = props[i];
							xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
							int blockIndex =  prop->getBlockIndex();
							dobj->getGhostUpdateBits().set(1 << prop->getBlockIndex(),false);
						}
					}
				}


				/************************************************************************/
				/*                                                                      */
				/************************************************************************/
				if (_class->getEnitityType() == E_DC_BTYPE_3D_ENTITY )
				{
					xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(dobj);
					if (dobj3D)
					{
						TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();
						xDistributedPropertyArrayType &props = *dobj3D->getDistributedPorperties();
						
						int gFlags = dobj->getGhostUpdateBits().getMask();
						
						for (unsigned int i = 0  ;  i < props.size() ; i++ )
						{
							xDistributedProperty *prop  = props[i];
							xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
							int blockIndex =  prop->getBlockIndex();
							
							if ( blockIndex < _class->getFirstUserField() && dobj->getGhostUpdateBits().testStrict(1<<blockIndex))
							{
								CK3dEntity *ghostDebug  = (CK3dEntity*)m_Context->GetObject(dobj3D->getGhostDebugID());
								CK3dEntity *bindObject  = (CK3dEntity*)m_Context->GetObject(dobj3D->getEntityID());

								switch(propInfo->mNativeType)
								{

									//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
									case E_DC_3D_NP_WORLD_POSITION:
									{

										xDistributedPoint3F *propValue = static_cast<xDistributedPoint3F*>(prop);

										Point3F currentPos = propValue->mLastServerValue;
										Point3F velocity = propValue->mLastServerDifference;
										
										
										/************************************************************************/
										/* we are updated by remote :						                                                                       */
										/************************************************************************/
										if (bindObject && dobj3D->getInterfaceFlags() & E_DO_BINDED && dobj3D->getUserID() != nInterface->getConnection()->getUserID()  )
										{
                                            
											updateLocalPosition(dobj,bindObject,prop);
										}

										if (ghostDebug)
										{
											if (propValue)
											{
												
												
												VxVector realPos;
												ghostDebug->GetPosition(&realPos);
												Point3F realPos2(realPos.x,realPos.y,realPos.z);
												float oneWayTime =  (dobj3D->getNetInterface()->GetConnectionTime() / 1000.0f);
												oneWayTime +=oneWayTime * prop->getPredictionSettings()->getPredictionFactor();
												Point3F predictedPos  = currentPos + velocity * oneWayTime;
												Point3F realPosDiff  =  - predictedPos - realPos2 ;
												VxVector posNew(predictedPos.x,predictedPos.y,predictedPos.z);
												//VxVector posNew2  = realPos + ( posNew - realPos )*0.5;
												ghostDebug->SetPosition(&posNew);
												//////////////////////////////////////////////////////////////////////////

											}
										}
										break;
									}
									//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
									case E_DC_3D_NP_WORLD_ROTATION:
									{
										if (bindObject && dobj3D->getInterfaceFlags() & E_DO_BINDED && dobj3D->getUserID() != nInterface->getConnection()->getUserID()  )
										{

											updateLocalRotation(dobj,bindObject,prop);
										}

										if (ghostDebug)
										{

											xDistributedQuatF*propValue = static_cast<xDistributedQuatF*>(prop);
											if (propValue)
											{
												
												/*
													Point3F currentPos = propValue->mLastServerValue;
													Point3F velocity = propValue->mLastServerDifference;
													VxVector realPos;
													ghostDebug->GetPosition(&realPos);
													Point3F realPos2(realPos.x,realPos.y,realPos.z);

													float oneWayTime =  (dobj3D->GetNetInterface()->GetConnectionTime() / 1000.0f);
													oneWayTime +=oneWayTime * prop->getPredictionSettings()->getPredictionFactor();
													Point3F predictedPos  = currentPos + velocity * oneWayTime;
													Point3F realPosDiff  =  - predictedPos - realPos2 ;
													VxVector posNew(predictedPos.x,predictedPos.y,predictedPos.z);
													//VxVector posNew2  = realPos + ( posNew - realPos )*0.5;
													ghostDebug->SetPosition(&posNew);
												*/
											}
										}
										break;
									}
								}
								
								
								dobj->getGhostUpdateBits().set(1 << prop->getBlockIndex(),false);
							}
						}
					}
				}
			}
		}
		begin++;
	}
}

void
vtNetworkManager::UpdateDistributedObjects(DWORD flags)
{

	//////////////////////////////////////////////////////////////////////////
	//this is to store states in our dist objects

	xNetInterface *nInterface = GetClientNetInterface();
	if (!nInterface) return;
	
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = nInterface->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	nInterface->setObjectUpdateCounter(0);
	if (nInterface->IsServer())return;
	//////////////////////////////////////////////////////////////////////////
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		
		if (dobj && dobj->getOwnershipState().test(1<<E_DO_OS_OWNER) )
		{
			xDistributedClass *_class = dobj->getDistributedClass();

			if (_class != NULL )
			{
				XString name(dobj->GetName().getString()); 
				XString cName  = _class->getClassName().getString();
				int entType  = _class->getEnitityType();
				
				if (_class->getEnitityType() == E_DC_BTYPE_SESSION)
				{

					dobj->setUpdateState( E_DO_US_OK );
				}

				if (_class->getEnitityType() == E_DC_BTYPE_3D_ENTITY )
				{
					xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(dobj);
					if (dobj3D)
					{
						CK3dEntity *entity  = (CK3dEntity*)m_Context->GetObject(dobj3D->getEntityID());
						//////////////////////////////////////////////////////////////////////////
						//if the object is ours then store the last known position in the state info : 
						int oID = dobj3D->getUserID();
						int myID = nInterface->getConnection()->getUserID();
						
						DWORD oFlags = dobj->getObjectFlags();
                        DWORD iFlags = dobj3D->getInterfaceFlags();
						
						
						if (entity)
						{
							TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();
							dobj->setUpdateState( E_DO_US_OK );

							xDistributedPropertyArrayType &props = *dobj3D->getDistributedPorperties();
							for (unsigned int i = 0  ;  i < props.size() ; i++ )
							{
								xDistributedProperty *prop  = props[i];
								xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
								int blockIndex =  prop->getBlockIndex();
								if (propInfo)
								{
									switch(propInfo->mNativeType)
									{										
										case E_DC_3D_NP_WORLD_POSITION :
										{
											xDistributedPoint3F * dpoint3F  = (xDistributedPoint3F*)prop;
											if (dpoint3F)
											{
												VxVector posW;
												entity->GetPosition(&posW);
												bool update = dpoint3F->updateValue(xMath::getFrom(posW),currentTime);
											}
											break;
										}
										case E_DC_3D_NP_WORLD_ROTATION:
										{
											xDistributedQuatF * dpoint3F  = (xDistributedQuatF*)prop;
											if (dpoint3F)
											{
												VxQuaternion vquat;
												entity->GetQuaternion(&vquat);
												bool update = dpoint3F->updateValue(xMath::getFrom(vquat),currentTime);
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		begin++;
	}
}










