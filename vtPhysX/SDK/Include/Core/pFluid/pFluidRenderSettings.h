#ifndef __P_FLUID_RENDER_SETTINGS_H__
	#define __P_FLUID_RENDER_SETTINGS_H__

#include "pTypes.h"


class MODULE_API pFluidRenderSettings
{
public:

	pFluidRenderSettings();

	// Constructor
	pFluidRenderSettings(CKContext* ctx,CK_ID entity,char* name);

	// the emitter 3d entity
	CK_ID		m_Entity;
	CK_ID		m_Texture;
	CK_ID		m_Group;
	int			m_MessageType;
	VxBbox		m_EntityBbox;

	pParticleRenderType mRenderType;

	int m_MaximumParticles;
	// total number of particles
	int totalParticles;
	// particles already emitted
	int particleCount;
	// emits per frame
	int emitsPerFrame;
	// emits variation
	int emitsVar;
	// medium lifeSpan
	float m_Life;
	// life variation
	float m_LifeVariation;

	// trailing particles
	int	m_TrailCount;
	// historic of recent particles.

	// render callback
	int (*m_RenderParticlesCallback)(CKRenderContext *dev,CKRenderObject *mov,void *arg);

	void SetState(CKRenderContext* dev,CKBOOL gouraud = FALSE);
	// NULL terminated linked list
	
	// the particles pool
	BYTE*		m_BackPool;


	// start size
	float m_StartSize;
	float m_StartSizeVar;
	// end size
	float m_EndSize;
	float m_EndSizeVar;


	// Blend Modes
	VXBLEND_MODE m_SrcBlend;
	VXBLEND_MODE m_DestBlend;

	// Colors
	/////////////
	VxColor m_StartColor;
	VxColor m_StartColorVar;
	VxColor m_EndColor;
	VxColor m_EndColorVar;

	// Texture
	/////////////
	int m_InitialTextureFrame;
	int m_InitialTextureFrameVariance;
	int m_SpeedTextureFrame;
	int m_SpeedTextureFrameVariance;
	int m_TextureFrameCount;
	int m_TextureFrameloop;

	// FLAGS
	int m_EvolutionsFlags;
	int m_VariancesFlags;
	int m_InteractorsFlags;
	int m_DeflectorsFlags;
	int m_RenderMode;

	// Mesh
	CK_ID	m_Mesh;


	// Context
	CKContext*	m_Context;

	pFluidEmitter* mEmitter;
	
	pFluidEmitter* getEmitter() const { return mEmitter; }
	void setEmitter(pFluidEmitter* val) { mEmitter = val; }

	void setToDefault();

	struct	ParticleHistoric
	{
		inline ParticleHistoric() {}
		inline ParticleHistoric(unsigned int size) :
		start(0),
			count(0)
		{
			particles.Resize(size);
		}

		int		start;
		int		count;
		XArray<pParticle*>	particles;
	};
	// Old particles.
	XClassArray<ParticleHistoric> old_pos;
	ParticleHistoric	&GetParticleHistoric(pParticle *part);
	
	
	CKBehavior*			m_Behavior;
	
	volatile bool hasBeenRendered;  //used for cases where we compute once and render twice
	volatile bool hasBeenEnqueud;  //used for cases where we compute once and render twice



protected:
private:
};



#endif