#if !defined(CUSTOMPLAYERSTATICLIBS_H)
#define CUSTOMPLAYERSTATICLIBS_H


#if defined(CUSTOM_PLAYER_STATIC)

#pragma warning( disable : 4099)

#if defined(_DEBUG)
	#pragma comment(lib,"strmbase")
#else
	#pragma comment(lib,"strmbase")
#endif


// virtools base libraries
#pragma comment(lib,"CKZlibStatic")
#pragma comment(lib,"VxMathStatic") 
#pragma comment(lib,"CK2Static")
#pragma comment(lib,"AssemblerStatic")
#pragma comment(lib,"VSLStatic")
#pragma comment(lib,"VSLManagerStatic")
#pragma comment(lib,"CKFEMgrStatic")
#pragma comment(lib,"CKRasterizerLibStatic")
#pragma comment(lib,"CK2_3DStatic")

// opengl rasterizers
// If you need CGFX uncomment
//#pragma comment(lib,"cg")
//#pragma comment(lib,"cgGL")
#pragma comment(lib,"opengl32")
#pragma comment(lib,"CKGLRasterizerStatic")

// DirectX rasterizers
#pragma comment(lib,"dxguid")

#if defined(USE_DX8)
	#pragma comment(lib,"d3dx8")
	#pragma comment(lib,"d3d8")
	#pragma comment(lib,"CKDX8RasterizerStatic")
#else
	#pragma comment(lib,"dxerr9")
	#pragma comment(lib,"d3dx9")
	#pragma comment(lib,"d3d9")
	#pragma comment(lib,"CKDX9RasterizerStatic")
#endif


// virtools plugins
#pragma comment(lib,"VirtoolsLoaderStatic")
#pragma comment(lib,"ImageReaderStatic")
#pragma comment(lib,"TiffReaderStatic")
#pragma comment(lib,"Vfw32")
#pragma comment(lib,"Winmm")
#pragma comment(lib,"Msacm32")
#pragma comment(lib,"AviReaderStatic")
#pragma comment(lib,"WavReaderStatic")
#pragma comment(lib,"JpgLoaderStatic")
#pragma comment(lib,"PngLoaderStatic")
#pragma comment(lib,"AscLoaderStatic")
#if defined(USE_DX8)
	#pragma comment(lib,"Dx8VideoManagerStatic")
#else
	//#pragma comment(lib,"Dx9VideoManagerStatic")
#endif
#pragma comment(lib,"3dsLoaderStatic")
#if defined(USE_DX8)
	#pragma comment(lib,"DDSReaderStatic")
#else
	#pragma comment(lib,"DDSReaderStatic9")
#endif
#pragma comment(lib,"TiffReaderStatic")
#pragma comment(lib,"XLoaderStatic")

// thir party libraries used for plugins
#if defined(_DEBUG)
	#pragma comment(lib,"JpegLibD")
	#pragma comment(lib,"pnglibd")
#else
	#pragma comment(lib,"jpeglib")
	#pragma comment(lib,"pnglib")
	#pragma comment(lib,"LibTiff")
#endif


// virtools managers
#pragma comment(lib,"dsound")
#pragma comment(lib,"DX7SoundManagerStatic")
#pragma comment(lib,"dinput8")
#pragma comment(lib,"DX5InputManagerStatic")
#pragma comment(lib,"ParameterOperationsStatic")
//#pragma comment(lib,"VideoManagerStatic")

// virtools behaviors
#pragma comment(lib,"3DTransfoStatic")
#pragma comment(lib,"BuildingBlocksAddons1Static")
#pragma comment(lib,"BuildingBlocksAddons2Static")
#pragma comment(lib,"CamerasStatic")
#pragma comment(lib,"CharactersStatic")
#pragma comment(lib,"CollisionsStatic")
#pragma comment(lib,"ControllersStatic")
#pragma comment(lib,"GridsStatic")
#pragma comment(lib,"InterfaceStatic")
#pragma comment(lib,"LightsStatic")
#pragma comment(lib,"LogicsStatic")
#pragma comment(lib,"MaterialsStatic")
#pragma comment(lib,"MeshModifiersStatic")
#pragma comment(lib,"MidiManagerStatic")
#pragma comment(lib,"NarrativesStatic")
#pragma comment(lib,"ParticleSystemsStatic")
#pragma comment(lib,"SoundsStatic")		
//#pragma comment(lib,"VideoStatic")
#pragma comment(lib,"VisualsStatic")
#pragma comment(lib,"WorldEnvironmentsStatic")
// If you need CGFX uncomment this line and comment the next one
 
//#pragma comment(lib,"ShaderStatic")
/*#pragma comment(lib,"ShaderStaticHLSL")*/


#ifdef vtToolkit
	#pragma comment(lib,"vtToolkit")
#endif // vtTools

#ifdef vtWidgets
	#pragma comment(lib,"vtWidgets")
#endif

#ifdef vtPhysX
	#pragma comment(lib,"vtPhysXLib")
#endif


// physics libraries
#pragma comment(lib,"PhysicsStatic")


// virtools network
/*#pragma comment(lib,"VCryptStatic")
#pragma comment(lib,"CryptedLoaderStatic")
#pragma comment(lib,"nk2static")
#pragma comment(lib,"vsutilsstatic")
#pragma comment(lib,"vsmanager2static")
#pragma comment(lib,"VSServer2static") // NOTE: this lib is in the Additional Dependencies of the linker (for linking problems)
#pragma comment(lib,"MultiUserClientStatic")
#pragma comment(lib,"MultiUserServerStatic")//  NOTE: this lib is in the Additional Dependencies of the linker (for linking problems)
#pragma comment(lib,"DatabaseClientModule2Static")
#pragma comment(lib,"DownloadMediaClient2Static")
*/


#endif // CUSTOM_PLAYER_STATIC


#endif // CUSTOMPLAYERSTATICLIBS_H

