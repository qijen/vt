/*--------------------------------------------------------------------- NVMH5 -|----------------------
Path:  Sdk\Demos\Direct3D9\src\GetGPUAndSystemInfo\
File:  GetGPUAndSystemInfo.h

Copyright NVIDIA Corporation 2003
TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS* AND NVIDIA AND
AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA
OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THIS
SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.


Comments:
A simple class to demonstrate the use of DXDiagNVUtil which is a convenient wrapper for
the IDxDiagContainer interface from Microsoft.

GetGPUAndSystemInfo gets only a small fraction of the information that IDxDiagContainer
makes available.

Since this class is simple to create and destroy, there is no specific handling for when
the D3D device is lost.  Simply Free() and re Initialize() the class if that should happen.

-------------------------------------------------------------------------------|--------------------*/


#ifndef H_GETGPUANDSYSTEMINFO_GJ_H
#define H_GETGPUANDSYSTEMINFO_GJ_H

#include "NV_D3DCommon\DXDiagNVUtil.h"

#include <string>				//  for wstring

struct IDirect3D9;


struct FloatingPointBlendModes
{
	bool m_bR16f;
	bool m_bR32f;
	bool m_bG16R16f;
	bool m_bG32R32f;
	bool m_bA16B16G16R16f;
	bool m_bA32B32G32R32f;
	bool m_bA8R8G8B8;		// not an fp mode, but there for good measure
};


class GetGPUAndSystemInfo
{
public:
	// DXDiagNVUtil is a utility class for creating and querring the IDxDiagContainer interface
	DXDiagNVUtil	m_DXDiagNVUtil;

	// Data fields that will be filled in by the GetData() function
	DWORD		m_dwNumDisplayDevices;
	float		m_fDriverVersion;
	wstring		m_wstrDeviceDesc;
	int			m_nDeviceMemoryMB;
	float		m_fSystemPhysicalMemoryMB;
	string		m_strAGPStatus;
	string		m_strMachineName;

	DWORD		m_dwDXVersionMajor;
	DWORD		m_dwDXVersionMinor;
	char		m_cDXVersionLetter;

	wstring		m_wstrDxDebugLevels;

	FloatingPointBlendModes		m_FPBlendModes;

	// ---- Main interface functions --------
	HRESULT		GetData();
	HRESULT		IsFPBlendingSupported( IDirect3D9 * pD3D9, FloatingPointBlendModes * pOutResult );

	// --------------------------------------

protected:
	void	SetAllNull()
	{
	};
public:
	GetGPUAndSystemInfo()
	{
		SetAllNull();
	};
	~GetGPUAndSystemInfo()
	{
		SetAllNull();
	}
};


#endif
