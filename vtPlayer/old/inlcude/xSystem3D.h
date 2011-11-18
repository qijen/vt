 /********************************************************************
	created:	2007/10/29
	created:	29:10:2007   15:16
	filename: 	f:\ProjectRoot\current\vt_plugins\vt_toolkit\src\xSystem3D.h
	file path:	f:\ProjectRoot\current\vt_plugins\vt_toolkit\src
	file base:	xSystem3D
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#include "base_macros.h"

namespace xSystem3DHelper
{

#ifdef __cplusplus
	extern "C" {
#endif

		API_EXPORT int xSGetAvailableTextureMem();
		API_EXPORT float xSGetPhysicalMemoryInMB();
		API_EXPORT int xSGetPhysicalGPUMemoryInMB(int device);
		API_EXPORT void xSSaveAllDxPropsToFile(char*file);
		API_EXPORT void xSGetDXVersion(char*& versionstring,int& minor);


#ifdef __cplusplus
	}
#endif

}