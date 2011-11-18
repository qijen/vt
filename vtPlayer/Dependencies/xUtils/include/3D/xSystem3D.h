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

#include <BaseMacros.h>

namespace xSystem3DHelper
{

		MODULE_API int xSGetAvailableTextureMem();
		MODULE_API float xSGetPhysicalMemoryInMB();
		MODULE_API int xSGetPhysicalGPUMemoryInMB(int device);
		MODULE_API void xSSaveAllDxPropsToFile(char*file);
		MODULE_API void xSGetDXVersion(char*& versionstring,int& minor);

}