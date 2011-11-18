/*--------------------------------------------------------------------- NVMH5 -|----------------------
Path:  Sdk\Demos\Direct3D9\src\GetGPUAndSystemInfo\
File:  GetGPUAndSystemInfo.cpp

Copyright NVIDIA Corporation 2003
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS* AND NVIDIA AND
AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA
OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THIS
SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Comments:
See GetGPUAndSystemInfo.h for additional comments.


-------------------------------------------------------------------------------|--------------------*/


#include <windows.h>
#include <d3dx9.h>

#include "GetGPUAndSystemInfo.h"

#include "shared\NV_Common.h"
#include "shared\NV_Error.h"


/*------------------------------------------------------------------
Retrieve various data through the IDxDiagContainer interface.

The DXDiagNVUtil class provides a few convenient wrapper functions,
and direct querries using the IDxDiagContainer COM object names are
also supported.  For a list of all the container and property COM
object names, call ListAllDXDiagPropertyNames(..)
------------------------------------------------------------------*/
HRESULT	GetGPUAndSystemInfo::GetData()
{
	HRESULT hr = S_OK;
	//----------------------------------------------------------------------------
	hr = m_DXDiagNVUtil.InitIDxDiagContainer();
	MSG_AND_RET_VAL_IF( FAILED(hr), "Couldn't initialize DXDiagNVUtil!\n", hr );
	wstring wstr;
	// Get the computer's machine name:
	m_DXDiagNVUtil.GetProperty( L"DxDiag_SystemInfo", L"szMachineNameEnglish", &wstr );
	m_strMachineName = m_DXDiagNVUtil.WStringToString( &wstr );

	FMsg("\n\n");
	FMsg("------------------------------------------------------------------------\n");
	FMsgW(L"machine name:  %s\n", wstr.c_str() );

	// Get info about physcial memory:
	m_DXDiagNVUtil.GetPhysicalMemoryInMB( & m_fSystemPhysicalMemoryMB );
	FMsg("physical memory: %g MB\n", m_fSystemPhysicalMemoryMB );
	
	// Get info about logical disks
	IDxDiagContainer * pContainer, *pChild;
	DWORD dwNumDisks;
	hr = m_DXDiagNVUtil.GetChildContainer( L"DxDiag_LogicalDisks", & pContainer );
	if( SUCCEEDED(hr))
	{
		pContainer->GetNumberOfChildContainers( & dwNumDisks );

		wstring drive, space;
		FMsgW(L"logical disks: %d\n", dwNumDisks );
		for( DWORD n=0; n < dwNumDisks; n++ )
		{
			hr = m_DXDiagNVUtil.GetChildByIndex( pContainer, n, &pChild );
			if( SUCCEEDED(hr) )
			{
				m_DXDiagNVUtil.GetProperty( pChild, L"szDriveLetter", &drive );
				m_DXDiagNVUtil.GetProperty( pChild, L"szFreeSpace", &space );
				FMsgW(L"  %s  Free space = %s\n", drive.c_str(), space.c_str() );			
				SAFE_RELEASE( pChild );
			}
		}
		SAFE_RELEASE( pContainer );
	}

	FMsg("------------------------------------------------------------------------\n");

	m_DXDiagNVUtil.GetDirectXVersion( &m_dwDXVersionMajor, &m_dwDXVersionMinor, &m_cDXVersionLetter );
	FMsg("DirectX Version: %d.%d%c\n", m_dwDXVersionMajor, m_dwDXVersionMinor, m_cDXVersionLetter );

	hr = m_DXDiagNVUtil.GetNumDisplayDevices( & m_dwNumDisplayDevices );
	MSG_AND_RET_VAL_IF( FAILED(hr), "Couldn't GetNumDisplayDevices()\n", hr );
	FMsg("Num Display Devices:  %d\n", m_dwNumDisplayDevices );

	string str;

//	for( DWORD dev=0; dev < m_dwNumDisplayDevices; dev ++ )
	if( m_dwNumDisplayDevices > 0 )
	{
		DWORD dev = 0;

		// get info from display devices
		m_DXDiagNVUtil.GetDisplayDeviceDescription(		dev, & m_wstrDeviceDesc );
		m_DXDiagNVUtil.GetDisplayDeviceNVDriverVersion( dev, & m_fDriverVersion );
		m_DXDiagNVUtil.GetDisplayDeviceMemoryInMB(		dev, & m_nDeviceMemoryMB );

		// report the info via OutputDebugString
		FMsgW(L"Device %d Description:    %s\n", dev, m_wstrDeviceDesc.c_str() );
		FMsg( "Device %d Driver version:  %g\n", dev, m_fDriverVersion );
		FMsg( "Device %d Physical mem:    %d MB\n", dev, m_nDeviceMemoryMB );

		// Get info about AGP memory:
		wstring wstrAGPEnabled, wstrAGPExists, wstrAGPStatus;
		hr = m_DXDiagNVUtil.GetDisplayDeviceAGPMemoryStatus( dev, &wstrAGPEnabled, &wstrAGPExists, &wstrAGPStatus );
		if( SUCCEEDED(hr))
		{
			// create a string from the AGP status strings
			m_strAGPStatus = "";
			str = m_DXDiagNVUtil.WStringToString( &wstrAGPEnabled );
			m_strAGPStatus += "AGP Enabled = "; m_strAGPStatus += str; m_strAGPStatus += ",  ";
			str = m_DXDiagNVUtil.WStringToString( &wstrAGPExists );
			m_strAGPStatus += "AGP Exists = "; m_strAGPStatus += str; m_strAGPStatus += ",  ";
			str = m_DXDiagNVUtil.WStringToString( &wstrAGPStatus );
			m_strAGPStatus += "AGP Status = "; m_strAGPStatus += str;
		}
		FMsg("%s\n", m_strAGPStatus.c_str() );

		wstring wstrNotes, wstrTestD3D9;
		m_DXDiagNVUtil.GetProperty( L"DxDiag_DisplayDevices", L"0", L"szTestResultD3D9English", &wstrTestD3D9 );
		m_DXDiagNVUtil.GetProperty( L"DxDiag_DisplayDevices", L"0", L"szNotesEnglish",			&wstrNotes );
	
		str = m_DXDiagNVUtil.WStringToString( &wstrTestD3D9 );
		FMsg("Device 0 szTestResultD3D9English = \"%s\"\n", str.c_str() );
		str = m_DXDiagNVUtil.WStringToString( &wstrNotes );
		FMsg("Device 0 szNotesEnglish = \"%s\"\n", str.c_str() );
	}

	m_DXDiagNVUtil.GetDebugLevels( & m_wstrDxDebugLevels );
	FMsg("DirectX Debug Levels:\n");
	FMsgW(L"%s\n", m_wstrDxDebugLevels.c_str() );


	// ListAllDXDiagPropertyNames() is slow
	// It prints out all nodes, child nodes, properties and their values
//	m_DXDiagNVUtil.ListAllDXDiagPropertyNames();

	// Use a call like that below to print out a specific node and it's children.
	//   For example, to list all the display device property names.
/*
	m_DXDiagNVUtil.GetChildContainer( L"DxDiag_DisplayDevices", &pContainer );
	m_DXDiagNVUtil.ListAllDXDiagPropertyNames( pContainer, L"DxDiag_DisplayDevices" );
	SAFE_RELEASE( pContainer );
// */	

	m_DXDiagNVUtil.FreeIDxDiagContainer();
	return( hr );
}


HRESULT GetGPUAndSystemInfo::IsFPBlendingSupported( IDirect3D9 * pD3D9, FloatingPointBlendModes * pOutResult )
{
	FMsg("There is a bug with this code in that is does not report that blending works for ordinary backbuffer formats\n");
	assert( false );

	HRESULT hr = S_OK;
	FAIL_IF_NULL( pD3D9 );
	FAIL_IF_NULL( pOutResult );

	bool * pResult;

	hr = pD3D9->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
									D3DFMT_A8R8G8B8,
									D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
									D3DRTYPE_SURFACE,
									D3DFMT_R16F );
	pResult = &pOutResult->m_bR16f;
	if( hr == D3D_OK )
		*pResult = true;
	else if( hr == D3DERR_NOTAVAILABLE )
		*pResult = false;
	else
		FMsg("Unknown return result for D3DFMT_R16F : %u\n", hr );


	hr = pD3D9->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
									D3DFMT_A8R8G8B8,
									D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
									D3DRTYPE_SURFACE,
									D3DFMT_A16B16G16R16F );
	pResult = &pOutResult->m_bA16B16G16R16f;
	if( hr == D3D_OK )
		*pResult = true;
	else if( hr == D3DERR_NOTAVAILABLE )
		*pResult = false;
	else
		FMsg("Unknown return result for D3DFMT_A16B16G16R16F : %u\n", hr );

	hr = pD3D9->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
									D3DFMT_A8R8G8B8,
//									D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
									0 | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,		// per the DX90SDK docs
									D3DRTYPE_SURFACE,
									D3DFMT_A8R8G8B8 );
	pResult = &pOutResult->m_bA8R8G8B8;
	if( hr == D3D_OK )
		*pResult = true;
	else if( hr == D3DERR_NOTAVAILABLE )
		*pResult = false;
	else
		FMsg("Unknown return result for D3DFMT_A8R8G8B8 : %u\n", hr );


	if( pOutResult->m_bA16B16G16R16f == true )
		FMsg("D3DFMT_A16B16G16R16F POSTPIXELSHADER_BLENDING is supported\n");
	else
		FMsg("D3DFMT_A16B16G16R16F POSTPIXELSHADER_BLENDING is not supported\n");

	if( pOutResult->m_bR16f == true )
		FMsg("D3DFMT_R16F POSTPIXELSHADER_BLENDING is supported\n");
	else
		FMsg("D3DFMT_R16F POSTPIXELSHADER_BLENDING is not supported\n");

	if( pOutResult->m_bA8R8G8B8 == true )
		FMsg("D3DFMT_A8R8G8B8 POSTPIXELSHADER_BLENDING is supported\n");
	else
		FMsg("D3DFMT_A8R8G8B8 POSTPIXELSHADER_BLENDING is not supported\n");

/*
Use IDirect3D9::CheckDeviceFormat, with usage (D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADERBLENDING).
HRESULT CheckDeviceFormat(          UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    DWORD Usage,
    D3DRESOURCETYPE RType,
    D3DFORMAT CheckFormat
);

D3DFMT_R16F 111 16-bit float format using 16 bits for the red channel. 
D3DFMT_G16R16F 112 32-bit float format using 16 bits for the red channel and 16 bits for the green channel. 
D3DFMT_A16B16G16R16F 

D3DFMT_R32F 114 32-bit float format using 32 bits for the red channel. 
D3DFMT_G32R32F 115 64-bit float format using 32 bits for the red channel and 32 bits for the green channel. 
D3DFMT_A32B32G32R32F 

D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING

D3DUSAGE_QUERY_FILTER (more than just point sampling)

Query the resource to verify support for post pixel shader blending support. 
If IDirect3D9::CheckDeviceFormat fails with D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
post pixel blending operations are not supported. These include alpha test, pixel fog, 
render-target blending, color write enable, and dithering.

MSFT examples from SDK
    if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,1) )
    {
        if( FAILED( m_pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, 
            pCaps->DeviceType, adapterFormat, D3DUSAGE_RENDERTARGET,
            D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8 ) ) )
        {
            return E_FAIL;
        }
    }

    // Need to support post-pixel processing (for alpha blending)
    if( FAILED( m_pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
        adapterFormat, D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
        D3DRTYPE_SURFACE, backBufferFormat ) ) )
    {
        return E_FAIL;
    }

HRESULT CMyD3DApplication::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
{
    // Need to support post-pixel processing (for alpha blending)
    if( FAILED( m_pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
        adapterFormat, D3DUSAGE_RENDERTARGET | D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
        D3DRTYPE_SURFACE, backBufferFormat ) ) )
    {
        return E_FAIL;
    }

*/


	
	return( hr );
}

