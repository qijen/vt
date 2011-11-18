/******************************************************************************

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
    
      
        
******************************************************************************/
#include <windows.h>
#include <dxfile.h>
#include "rmxfguid.h"
#include "rmxftmpl.h"
#include "shared/nvfile.h"

using namespace std;

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(x) { if (x) { x->Release(); x = NULL; } }
#endif

HRESULT NVFile::LoadFrame( IDirect3DDevice9* pDevice,
                           LPD3DXFILEDATA pFileData,
                           NVFrame* pParentFrame )
{
    LPD3DXFILEDATA   pChildData = NULL;
    GUID guid;
    DWORD       cbSize;
    NVFrame*  pCurrentFrame;
    HRESULT     hr;

    // Get the type of the object
    if( FAILED( hr = pFileData->GetType( &guid ) ) )
        return hr;

    if( guid == TID_D3DRMMesh )
    {
        hr = LoadMesh( pDevice, pFileData, pParentFrame );
        if( FAILED(hr) )
            return hr;
    }
    if( guid == TID_D3DRMFrameTransformMatrix )
    {
        D3DXMATRIX* pmatMatrix;
        hr = pFileData->Lock(&cbSize, (LPCVOID*)&pmatMatrix );
        if( FAILED(hr) )
            return hr;

        // Update the parent's matrix with the new one
        pParentFrame->SetMatrix( pmatMatrix );
    }
    if( guid == TID_D3DRMFrame )
    {
        // Get the frame name
        TCHAR strAnsiName[512] = _T("");
        SIZE_T dwNameLength = 512;
        SIZE_T cChildren;

#ifdef UNICODE

		CHAR tmp[512];
        if( FAILED( hr = pFileData->GetName( tmp, &dwNameLength ) ) )
            return hr;

		MultiByteToWideChar(CP_ACP,0,tmp,512,strAnsiName,512);
#else
        if( FAILED( hr = pFileData->GetName( strAnsiName, &dwNameLength ) ) )
            return hr;
#endif


        // Create the frame
        pCurrentFrame = new NVFrame( strAnsiName );
        if( pCurrentFrame == NULL )
            return E_OUTOFMEMORY;

        pCurrentFrame->m_pNext = pParentFrame->m_pChild;
        pParentFrame->m_pChild = pCurrentFrame;

        // Enumerate child objects
        pFileData->GetChildren(&cChildren);
        for (UINT iChild = 0; iChild < cChildren; iChild++)
        {
            // Query the child for its FileData
            hr = pFileData->GetChild(iChild, &pChildData );
            if( SUCCEEDED(hr) )
            {
                hr = LoadFrame( pDevice, pChildData, pCurrentFrame );
                SAFE_RELEASE( pChildData );
            }

            if( FAILED(hr) )
                return hr;
        }
    }

    return S_OK;
}

HRESULT NVFile::LoadMesh( IDirect3DDevice9* pDevice,
                          LPD3DXFILEDATA pFileData,
                          NVFrame* pParentFrame )
{
    // Currently only allowing one mesh per frame
    if( pParentFrame->m_pMesh )
        return E_FAIL;

    // Get the mesh name
    TCHAR  strAnsiName[512] = {0};
    SIZE_T dwNameLength = 512;

    HRESULT hr;
#ifdef UNICODE

	CHAR tmp[512];
    if( FAILED( hr = pFileData->GetName( tmp, &dwNameLength ) ) )
        return hr;

	MultiByteToWideChar(CP_ACP,0,tmp,512,strAnsiName,512);
#else
    if( FAILED( hr = pFileData->GetName( strAnsiName, &dwNameLength ) ) )
        return hr;
#endif

    // Create the mesh
    pParentFrame->m_pMesh = new NVMesh( strAnsiName );
    if( pParentFrame->m_pMesh == NULL )
        return E_OUTOFMEMORY;
    pParentFrame->m_pMesh->Create( pDevice, pFileData );

    return S_OK;
}

HRESULT NVFile::Create( IDirect3DDevice9* pDevice, const tstring& strPathname )
{
    LPD3DXFILE           pDXFile   = NULL;
    LPD3DXFILEENUMOBJECT pEnumObj  = NULL;
    LPD3DXFILEDATA       pFileData = NULL;
    HRESULT hr;
    SIZE_T cChildren;

	tstring::size_type Pos = strPathname.find_last_of(_T("\\"), strPathname.size());
	if (Pos != strPathname.npos)
	{
		// Make sure we are on the right path for loading resources associated with this file
		m_strFilePath = strPathname.substr(0, Pos);
		SetCurrentDirectory(m_strFilePath.c_str());
	}

    // Create a x file object
    if( FAILED( hr = D3DXFileCreate( &pDXFile ) ) )
        return E_FAIL;

    // Register templates for d3drm and patch extensions.
    if( FAILED( hr = pDXFile->RegisterTemplates( (VOID*)D3DRM_XTEMPLATES,
                                                 D3DRM_XTEMPLATE_BYTES ) ) )
    {
        SAFE_RELEASE( pDXFile );
        return E_FAIL;
    }
#ifdef UNICODE
 	int len = WideCharToMultiByte(CP_ACP,0,strPathname.c_str(),-1,NULL,NULL,NULL,NULL);
	char *tmp = new char[len];
	WideCharToMultiByte(CP_ACP,0,strPathname.c_str(),-1,tmp,len,NULL,NULL);


	hr = pDXFile->CreateEnumObject( (VOID*)tmp,
                                    D3DXF_FILELOAD_FROMFILE, &pEnumObj );
	
#else
    // Create enum object
    hr = pDXFile->CreateEnumObject( (VOID*)strPathname.c_str(),
                                    D3DXF_FILELOAD_FROMFILE, &pEnumObj );
#endif
	if (FAILED(hr))
    {
        SAFE_RELEASE( pDXFile );
        return hr;
    }

    // Enumerate top level objects (which are always frames)
    pEnumObj->GetChildren(&cChildren);
    for (UINT iChild = 0; iChild < cChildren; iChild++)
    {
        hr = pEnumObj->GetChild(iChild, &pFileData);
        if (FAILED(hr))
            return hr;

        hr = LoadFrame( pDevice, pFileData, this );
        SAFE_RELEASE( pFileData );
        if( FAILED(hr) )
        {
            SAFE_RELEASE( pEnumObj );
            SAFE_RELEASE( pDXFile );
            return E_FAIL;
        }
    }

    SAFE_RELEASE( pFileData );
    SAFE_RELEASE( pEnumObj );
    SAFE_RELEASE( pDXFile );

    return S_OK;
}

HRESULT NVFile::Render( IDirect3DDevice9* pDevice )
{
    // Setup the world transformation
    D3DXMATRIX matSavedWorld, matWorld;
    pDevice->GetTransform(D3DTS_WORLD, &matSavedWorld);
    D3DXMatrixMultiply( &matWorld, &matSavedWorld, &m_mat );
    pDevice->SetTransform(D3DTS_WORLD, &matWorld );

    // Render opaque subsets in the meshes
    if( m_pChild )
        m_pChild->Render( pDevice, TRUE, FALSE );

    // Enable alpha blending
    pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    pDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    // Render alpha subsets in the meshes
    if( m_pChild )
        m_pChild->Render( pDevice, FALSE, TRUE );

    // Restore state
    pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    pDevice->SetTransform(D3DTS_WORLD, &matSavedWorld );

    return S_OK;
}


bool CalcFileObjectSizeCB( NVFrame* pFrame, D3DXMATRIX* pMat, VOID* pfSize )
{
	NVBounds* pBounds = (NVBounds*)pfSize;
	NVMesh* pMesh = pFrame->m_pMesh;
	DWORD dwStride = 0;

	if (!pMesh)
	{
		return true;
	}
	
	// Get the bounds for the mesh and transform them by the local
	// world transform.
	NVBounds MeshBounds = *pMesh->GetBounds();
	MeshBounds.Transform(pMat);
	
	if (pBounds->m_vecMaxExtents.x < MeshBounds.m_vecMaxExtents.x)
		pBounds->m_vecMaxExtents.x = MeshBounds.m_vecMaxExtents.x;

	if (pBounds->m_vecMaxExtents.y < MeshBounds.m_vecMaxExtents.y)
		pBounds->m_vecMaxExtents.y = MeshBounds.m_vecMaxExtents.y;

	if (pBounds->m_vecMaxExtents.z < MeshBounds.m_vecMaxExtents.z)
		pBounds->m_vecMaxExtents.z = MeshBounds.m_vecMaxExtents.z;


	if (pBounds->m_vecMinExtents.x > MeshBounds.m_vecMinExtents.x)
		pBounds->m_vecMinExtents.x = MeshBounds.m_vecMinExtents.x;

	if (pBounds->m_vecMinExtents.y > MeshBounds.m_vecMinExtents.y)
		pBounds->m_vecMinExtents.y = MeshBounds.m_vecMinExtents.y;

	if (pBounds->m_vecMinExtents.z > MeshBounds.m_vecMinExtents.z)
		pBounds->m_vecMinExtents.z = MeshBounds.m_vecMinExtents.z;

	if (pBounds->m_fRadius < MeshBounds.m_fRadius)
		pBounds->m_fRadius = MeshBounds.m_fRadius;

    return true;
}

//-----------------------------------------------------------------------------
// Name: GetBoundingInfo()
// Desc:
//-----------------------------------------------------------------------------
void NVFile::GetBoundingInfo(NVBounds* pBounds)
{
	pBounds->m_vecCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pBounds->m_fRadius = 0.0f;
	pBounds->m_vecMinExtents = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	pBounds->m_vecMaxExtents = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	D3DXMATRIX Matrix;
	D3DXMatrixIdentity(&Matrix);
    WalkFrames(CalcFileObjectSizeCB, &Matrix, (VOID*)pBounds);

	pBounds->m_vecCenter = (pBounds->m_vecMaxExtents + pBounds->m_vecMinExtents) / 2.0f;
}

