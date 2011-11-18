#include <StdAfx.h>
#include "vtPhysXAll.h"

#ifdef HAS_FLUIDS



pFluid *pWorld::getFluid(CK_ID id)
{

	int nbFluids= getScene()->getNbFluids();
	NxFluid** fluids = getScene()->getFluids();
	while(nbFluids--)
	{
		NxFluid* fluid = *fluids++;
		if(fluid->userData != NULL)
		{
			pFluid *vFluid  = static_cast<pFluid*>(fluid->userData);
			if (vFluid)
			{
				
				if (vFluid->getEntityID()  == id)
				{
					return vFluid;

				}
			}
		}
	}

	return NULL;
}

pFluid*pWorld::getFluid(CK3dEntity* entity)
{
	if (!entity)
	{
		return NULL;
	}

	return getFluid(entity->GetID());

}
void pWorld::updateFluids()
{
	

	int nbFluids= getScene()->getNbFluids();
	NxFluid** fluids = getScene()->getFluids();
	while(nbFluids--)
	{
		NxFluid* fluid = *fluids++;
		if(fluid->userData != NULL)
		{
			pFluid *vFluid  = static_cast<pFluid*>(fluid->userData);
			if (vFluid)
			{
				vFluid->updateVirtoolsMesh();

			}
		}
	}

}
#endif