/******************************************************************************

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
    
      
        
******************************************************************************/
#ifndef __NVFILE_H
#define __NVFILE_H

//#include "NVDX8Macros.h"
#include "nvmesh.h"
#include "nvframe.h"
#include <D3DX9.h>
#include <TCHAR.H>
typedef std::basic_string<TCHAR> tstring; 


class NVFile : public NVFrame
{
	HRESULT LoadMesh( IDirect3DDevice9* pDevice, LPD3DXFILEDATA pFileData, 
					  NVFrame* pParentFrame );
	HRESULT LoadFrame( IDirect3DDevice9* pDevice, LPD3DXFILEDATA pFileData, 
		               NVFrame* pParentFrame );
public:

	void GetBoundingInfo(NVBounds* pBounds);
	HRESULT Create( IDirect3DDevice9* pDevice, const tstring& strFilename );
	HRESULT Render( IDirect3DDevice9* pDevice );

	NVFile() : NVFrame( _T("NVFile_Root") ) {}

	tstring m_strFilePath;
};

#endif



