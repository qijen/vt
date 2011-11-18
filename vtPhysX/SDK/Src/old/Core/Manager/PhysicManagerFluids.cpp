#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "VSLManagerSDK.h"


#ifdef HAS_FLUID

void __newpFluidDescr(BYTE *iAdd)
{
	new(iAdd)pFluidDesc();
}

void __newpFluidEmitterDesc(BYTE *iAdd)
{
	new(iAdd)pFluidEmitterDesc();
}

void PhysicManager::_RegisterFluid_VSL()
{


	STARTVSLBIND(m_Context)



	/************************************************************************/
	/* pFluidFlags                                                                     */
	/************************************************************************/

	DECLAREENUM("pFluidFlag")
	DECLAREENUMVALUE("pFluidFlag", "PFF_DisableGravity" , 2)
	DECLAREENUMVALUE("pFluidFlag", "PFF_CollisionTwoway" , 4)
	DECLAREENUMVALUE("pFluidFlag", "PFF_Enabled" , 8)
	DECLAREENUMVALUE("pFluidFlag", "PFF_Hardware" , 16)
	DECLAREENUMVALUE("pFluidFlag", "PFF_PriorityMode" , 32)
	DECLAREENUMVALUE("pFluidFlag", "PFF_ProjectToPlane" , 64)

	DECLAREENUM("pFluidSimulationMethod")
	DECLAREENUMVALUE("pFluidSimulationMethod", "PFS_SPH" , 1)
	DECLAREENUMVALUE("pFluidSimulationMethod", "PFS_NoParticleInteraction" , 2)
	DECLAREENUMVALUE("pFluidSimulationMethod", "PFS_MixedMode" , 4)

	DECLAREENUM("pFluidCollisionMethod")
	DECLAREENUMVALUE("pFluidCollisionMethod", "PFCM_Static" , 1)
	DECLAREENUMVALUE("pFluidCollisionMethod", "PFCM_Dynamic" , 2)


	
	DECLAREOBJECTTYPE(pFluidDesc)
	DECLARECTOR_0(__newpFluidDescr)
	DECLAREMEMBER(pFluidDesc,int,maxParticles)
	DECLAREMEMBER(pFluidDesc,float,attractionForDynamicShapes)
	DECLAREMEMBER(pFluidDesc,float,attractionForStaticShapes)
	DECLAREMEMBER(pFluidDesc,float,collisionDistanceMultiplier)
	DECLAREMEMBER(pFluidDesc,int,collisionGroup)
	DECLAREMEMBER(pFluidDesc,pFluidCollisionMethod,collisionMethod)
	DECLAREMEMBER(pFluidDesc,float,collisionResponseCoefficient)
	DECLAREMEMBER(pFluidDesc,float,damping)
	DECLAREMEMBER(pFluidDesc,float,dynamicFrictionForDynamicShapes)
	DECLAREMEMBER(pFluidDesc,float,dynamicFrictionForStaticShapes)
	DECLAREMEMBER(pFluidDesc,VxVector,externalAcceleration)
	DECLAREMEMBER(pFluidDesc,float,fadeInTime)
	DECLAREMEMBER(pFluidDesc,float,kernelRadiusMultiplier)
	DECLAREMEMBER(pFluidDesc,float,packetSizeMultiplier)
	DECLAREMEMBER(pFluidDesc,int,maxParticles)
	DECLAREMEMBER(pFluidDesc,float,motionLimitMultiplier)
	DECLAREMEMBER(pFluidDesc,int,numReserveParticles)
	DECLAREMEMBER(pFluidDesc,int,packetSizeMultiplier)
	DECLAREMEMBER(pFluidDesc,float,restitutionForDynamicShapes)
	DECLAREMEMBER(pFluidDesc,float,restitutionForStaticShapes)
	DECLAREMEMBER(pFluidDesc,float,restParticlesPerMeter)
	DECLAREMEMBER(pFluidDesc,float,restDensity)
	DECLAREMEMBER(pFluidDesc,pFluidSimulationMethod,simulationMethod)
	DECLAREMEMBER(pFluidDesc,float,staticFrictionForDynamicShapes)
	DECLAREMEMBER(pFluidDesc,float,staticFrictionForStaticShapes)
	DECLAREMEMBER(pFluidDesc,float,stiffness)
	DECLAREMEMBER(pFluidDesc,float,surfaceTension)
	DECLAREMEMBER(pFluidDesc,float,viscosity)
	DECLAREMEMBER(pFluidDesc,pFluidFlag,flags)
	DECLAREMEMBER(pFluidDesc,CK_ID,worldReference)


	DECLAREMETHOD_0(pFluidDesc,void,setToDefault)
	DECLAREMETHOD_0(pFluidDesc,bool,isValid)


	/************************************************************************/
	/* emitter                                                                      */
	/************************************************************************/


	//////////////////////////////////////////////////////////////////////////
	//
	//		emitter flags : 
	//
	DECLAREENUM("pFluidEmitterFlag")
	DECLAREENUMVALUE("pFluidEmitterFlag", "PFEF_ForceOnBody" , 4)
	DECLAREENUMVALUE("pFluidEmitterFlag", "PFEF_AddBodyVelocity" , 8)
	DECLAREENUMVALUE("pFluidEmitterFlag", "PFEF_Enabled" , 16)

	DECLAREENUM("pEmitterShape")
	DECLAREENUMVALUE("pEmitterShape", "PFES_Rectangular" , 1)
	DECLAREENUMVALUE("pEmitterShape", "PFES_Ellipse" , 2)

	DECLAREENUM("pEmitterType")
	DECLAREENUMVALUE("pEmitterType", "PFET_ConstantPressure" , 1)
	DECLAREENUMVALUE("pEmitterType", "PFET_ConstantFlowRate" , 2)


	//////////////////////////////////////////////////////////////////////////
	//
	//		emitter descr : 
	//

	DECLAREOBJECTTYPE(pFluidEmitterDesc)
	DECLARECTOR_0(__newpFluidEmitterDesc)
	DECLAREMEMBER(pFluidEmitterDesc,CK3dEntity*,frameShape)
	DECLAREMEMBER(pFluidEmitterDesc,pEmitterType,type)
	DECLAREMEMBER(pFluidEmitterDesc,int,maxParticles)
	DECLAREMEMBER(pFluidEmitterDesc,pEmitterShape,shape)
	DECLAREMEMBER(pFluidEmitterDesc,float,dimensionX)
	DECLAREMEMBER(pFluidEmitterDesc,float,dimensionY)
	DECLAREMEMBER(pFluidEmitterDesc,VxVector,randomPos)
	DECLAREMEMBER(pFluidEmitterDesc,float,randomAngle)
	DECLAREMEMBER(pFluidEmitterDesc,float,fluidVelocityMagnitude)
	DECLAREMEMBER(pFluidEmitterDesc,float,rate)
	DECLAREMEMBER(pFluidEmitterDesc,float,randomAngle)
	DECLAREMEMBER(pFluidEmitterDesc,float,particleLifetime)
	DECLAREMEMBER(pFluidEmitterDesc,float,repulsionCoefficient)
	DECLAREMEMBER(pFluidEmitterDesc,pFluidEmitterFlag,flags)
	DECLAREMETHOD_0(pFluidEmitterDesc,void,setToDefault)
	DECLAREMETHOD_0(pFluidEmitterDesc,bool,isValid)
	DECLAREMEMBER(pFluidEmitterDesc,CK_ID,entityReference)



	DECLAREPOINTERTYPE(pFluidEmitter)
	
	/************************************************************************/
	/* fluid                                                                      */
	/************************************************************************/
	
	DECLAREPOINTERTYPE(pFluid)
	DECLAREMETHOD_0(pFluid,CK3dEntity*,getParticleObject)

	DECLAREMETHOD_2(pFactory,pFluid*,createFluid,CK3dEntity*,pFluidDesc)

	DECLAREMETHOD_1(pFluid,pFluidEmitter*,createEmitter,const pFluidEmitterDesc&)



	STOPVSLBIND



}

#endif // HAS_FLUID