#include <StdAfx.h>
#include "vtPhysXAll.h"

#ifdef HAS_FLUIDS

#include "pFluid.h"

int RenderParticles_P(CKRenderContext *dev,CKRenderObject *obj,void *arg)
{
	CK3dEntity* ent = (CK3dEntity *)obj;
	pFluid *fluid  = (pFluid*)arg;

	if (!ent)
		return 0;




	if (!fluid)
		return 0;


	CKMesh *mesh  = ent->GetCurrentMesh();
	int vCount = mesh->GetVertexCount();

	VxDrawPrimitiveData* data = dev->GetDrawPrimitiveStructure(CKRST_DP_TR_CL_VC,vCount);
	VxMatrix oldmatrix  = dev->GetWorldTransformationMatrix();
	dev->SetWorldTransformationMatrix(oldmatrix*ent->GetInverseWorldMatrix());

	// we don't let write to the ZBuffer
	dev->SetTexture(NULL);
	dev->SetState(VXRENDERSTATE_ZWRITEENABLE , FALSE);
	dev->SetState(VXRENDERSTATE_SRCBLEND, VXBLEND_SRCALPHA);
	dev->SetState(VXRENDERSTATE_DESTBLEND, VXBLEND_ONE);
	dev->SetState(VXRENDERSTATE_ALPHABLENDENABLE, TRUE);
	dev->SetTextureStageState(CKRST_TSS_STAGEBLEND,0,1);

	float averageSize = 1 * 2.0f;
	float minSize = 4.0f;
	float maxSize = 10000.0f;

	float pointScaleA = 1.0f;
	float pointScaleB = 1.0f;
	float pointScaleC = 1.0f;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/


	dev->SetState(VXRENDERSTATE_SPECULARENABLE, FALSE);
	dev->SetState(VXRENDERSTATE_FILLMODE, VXFILL_SOLID);
	dev->SetState(VXRENDERSTATE_SHADEMODE, VXSHADE_GOURAUD);

	dev->SetTextureStageState(CKRST_TSS_TEXTUREMAPBLEND,VXTEXTUREBLEND_MODULATEALPHA);
	dev->SetTextureStageState(CKRST_TSS_MAGFILTER , VXTEXTUREFILTER_LINEAR);
	dev->SetTextureStageState(CKRST_TSS_MINFILTER , VXTEXTUREFILTER_LINEARMIPLINEAR);

	// States
	dev->SetState(VXRENDERSTATE_WRAP0 , 0);
	dev->SetState(VXRENDERSTATE_CULLMODE, VXCULL_NONE);
	dev->SetState(VXRENDERSTATE_SRCBLEND, VXBLEND_SRCALPHA);
	dev->SetState(VXRENDERSTATE_DESTBLEND, VXBLEND_ONE);
	dev->SetState(VXRENDERSTATE_ALPHABLENDENABLE, TRUE);
	dev->SetState(VXRENDERSTATE_ZWRITEENABLE , FALSE);

	dev->SetTextureStageState(CKRST_TSS_STAGEBLEND,0,1);
	dev->SetTextureStageState(CKRST_TSS_TEXTURETRANSFORMFLAGS, 0);
	dev->SetTextureStageState(CKRST_TSS_TEXCOORDINDEX, 0);

	dev->SetState(VXRENDERSTATE_POINTSPRITEENABLE,	TRUE);

	dev->SetState(VXRENDERSTATE_POINTSIZE,	*(DWORD*)&averageSize);
	dev->SetState(VXRENDERSTATE_POINTSIZE_MIN,*(DWORD*)&minSize);
	dev->SetState(VXRENDERSTATE_POINTSIZE_MAX,*(DWORD*)&maxSize);
	dev->SetState(VXRENDERSTATE_POINTSCALEENABLE,	TRUE);

	

	dev->SetState(VXRENDERSTATE_POINTSCALE_A,*(DWORD*)&pointScaleA);
	dev->SetState(VXRENDERSTATE_POINTSCALE_B,*(DWORD*)&pointScaleB);
	dev->SetState(VXRENDERSTATE_POINTSCALE_C,*(DWORD*)&pointScaleC);

	XPtrStrided<VxVector4> positions(data->Positions);
	XPtrStrided<DWORD>	colors(data->Colors);



	pParticle *particles = fluid->getParticles();

	for (int i = 0 ; i < vCount ; i++)
	{
		VxColor color;
		color.Set(1.0f);

		pParticle *p  = &(fluid->mParticleBuffer[i]);
		VxVector posi  = getFrom(p->position);
		*positions	= VxVector4(posi.x,posi.y,posi.z,0);
		*colors		= RGBAFTOCOLOR(&(color));
		// next point
		//p = p->next;

		++colors;
		++positions;
	}


	// The drawing
	dev->DrawPrimitive(VX_POINTLIST,(WORD*)NULL,vCount,data);
	dev->SetState(VXRENDERSTATE_ZWRITEENABLE , TRUE);
	dev->SetWorldTransformationMatrix(oldmatrix);

	return 0;
}
#endif