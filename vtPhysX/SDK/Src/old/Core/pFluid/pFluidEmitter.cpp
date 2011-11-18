#include <StdAfx.h>
#include "vtPhysXAll.h"

#ifdef HAS_FLUIDS

#include "pFluidEmitterDesc.h"
#include "pFluidEmitter.h"

pFluidEmitter::pFluidEmitter()
{
	mFluid = NULL;
	mEmitter  = NULL;
	mEntityReference = 0;
	mRenderSettings  = NULL;
}
#endif // HAS_FLUIDS