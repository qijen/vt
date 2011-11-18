/********************************************************************
	created:	2007/10/29
	created:	29:10:2007   15:19
	filename: 	f:\ProjectRoot\current\vt_plugins\vt_toolkit\src\xSystem3D.cpp
	file path:	f:\ProjectRoot\current\vt_plugins\vt_toolkit\src
	file base:	xSystem3D
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/

/*#ifndef USEDIRECTX9
	#define USEDIRECTX9
#endif*/

#include <CPStdAfx.h>

#include "xSystem3D.h"
#include "d3d9.h"
#include "CKAll.h"
#include "3d/DXDiagNVUtil.h"

//////////////////////////////////////////////////////////////////////
//
//      DirectX-Helpers
//
//

namespace xSystem3DHelper
{
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	void xSSaveAllDxPropsToFile(char*file)
	{
		
		DXDiagNVUtil nvutil;
		nvutil.InitIDxDiagContainer();
		
		FILE *fp = fopen("x:\\dx.out","a");
		//nvutil.ListAllDXDiagPropertyNames();
		nvutil.ListAllDXDiagPropertyNamesToTxtFile(fp,true,NULL,NULL);



	}
	//int xSGetAvailableTextureMem();


	/*	int xSGetAvailableTextureMem(CKContext *ctx)
	{

		
		if (_im2 && _im2->m_Context)
		{
		
			CKContext * ctx1 = _im2->m_Context;
			CKRenderManager* rm = ctx1->GetRenderManager();
			CKRenderContext* rc = rm->GetRenderContext(0);
			VxDirectXData*	 dx = rc->GetDirectXInfo();

			int z_ = 0;
			IDirect3DDevice9 *d_ptr = ((IDirect3DDevice9*)dx->D3DDevice);
			
			if(dx)		{			return d_ptr->GetAvailableTextureMem();		}
		}

		
		return -1;
		
	}
	*/

	float xSGetPhysicalMemoryInMB()
	{
		DXDiagNVUtil nvutil;
		nvutil.InitIDxDiagContainer();
		

		float ret = -1;
		nvutil.GetPhysicalMemoryInMB(&ret);
		nvutil.FreeIDxDiagContainer();

		return ret;
	}

	int xSGetPhysicalGPUMemoryInMB(int device)
	{

	DXDiagNVUtil nvutil;
	nvutil.InitIDxDiagContainer();

	int ret = -1;
	nvutil.GetDisplayDeviceMemoryInMB(device, &ret);
	nvutil.FreeIDxDiagContainer();
	return ret;
	}
}