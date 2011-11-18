#include <StdAfx.h>
#include "vtPhysXAll.h"

#ifdef HAS_FLUIDS

pFluidEmitterDesc::pFluidEmitterDesc()
{
	setToDefault();
}

pFluidEmitterDesc::~pFluidEmitterDesc()
{
}

void pFluidEmitterDesc::setToDefault()
{
	frameShape						= NULL;
	type							= (pEmitterType)(NX_FE_CONSTANT_PRESSURE);
	maxParticles					= 0;
	shape							= (pEmitterShape)(NX_FE_RECTANGULAR);
	dimensionX						= 0.25f;
	dimensionY						= 0.25f;
	randomAngle						= 0.0f;
	randomPos = VxVector(0,0,0);
	fluidVelocityMagnitude			= 1.0f;
	rate							= 100.0f;
	particleLifetime				= 0.0f;
	repulsionCoefficient			= 1.0f;
	flags							= (pFluidEmitterFlag)(NX_FEF_ENABLED|NX_FEF_VISUALIZATION|NX_FEF_ADD_BODY_VELOCITY);
}

bool pFluidEmitterDesc::isValid() const
{

	if (dimensionX < 0.0f) return false;
	if (dimensionY < 0.0f) return false;

	if (randomPos.x < 0.0f) return false;
	if (randomPos.y < 0.0f) return false;
	if (randomPos.z < 0.0f) return false;
	if (randomAngle < 0.0f) return false;

	if (!(((shape & NX_FE_ELLIPSE) > 0) ^ ((shape & NX_FE_RECTANGULAR) > 0))) return false;
	if (!(((type & NX_FE_CONSTANT_FLOW_RATE) > 0) ^ ((type & NX_FE_CONSTANT_PRESSURE) > 0))) return false;

	if (rate < 0.0f) return false;
	if (fluidVelocityMagnitude < 0.0f) return false;
	if (particleLifetime < 0.0f) return false;
	if (repulsionCoefficient < 0.0f) return false;

	return true;
}
#endif