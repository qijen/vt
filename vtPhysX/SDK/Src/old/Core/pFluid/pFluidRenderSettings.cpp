#include <StdAfx.h>
#include "vtPhysXAll.h"

#ifdef HAS_FLUIDS

#include "pFluidRenderSettings.h"


pFluidRenderSettings::pFluidRenderSettings(CKContext* ctx,CK_ID entity,char* name)
{

	m_Context = ctx;
	m_InteractorsFlags = 0;

	m_StartSize = 0;
	m_StartSizeVar = 0;
	m_EndSize = 0;
	m_EndSizeVar = 0;

	m_StartColor.r = 0.6f;
	m_StartColor.g = 0.6f;
	m_StartColor.b = 0.8f;
	m_StartColor.b = 1.0f;
	m_StartColorVar.r = 0.0f;
	m_StartColorVar.g = 0.0f;
	m_StartColorVar.b = 0.0f;
	m_StartColorVar.a = 0.0f;
	m_EndColor.r = 0.0f;
	m_EndColor.g = 0.0f;
	m_EndColor.b = 0.0f;
	m_EndColor.a = 0.0f;
	m_EndColorVar.r = 0.0f;
	m_EndColorVar.g = 0.0f;
	m_EndColorVar.b = 0.0f;
	m_EndColorVar.a = 0.0f;
	m_InitialTextureFrame = 0;
	m_InitialTextureFrameVariance = 0;
	m_SpeedTextureFrame = 0;
	m_SpeedTextureFrameVariance = 0;
	m_TextureFrameCount = 0;
	m_TextureFrameloop = FALSE;
	m_EvolutionsFlags = 0;
	m_VariancesFlags = 0;
	m_InteractorsFlags = 0;
	m_DeflectorsFlags = 0;
	m_RenderMode = 3;
	m_Behavior = NULL;

	mRenderType = PRT_Point;


	m_Mesh = 0;
	m_Entity = entity;
	CK3dEntity* ent = (CK3dEntity*)m_Context->GetObject(m_Entity);
	if (ent)
		m_EntityBbox = ent->GetBoundingBox(TRUE);

	m_Texture = 0;
	m_Group = 0;
	m_MessageType = -1;

}



void pFluidRenderSettings::setToDefault()
{

}
#endif // HAS_FLUIDS