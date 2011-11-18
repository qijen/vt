#if !defined(CUSTOMPLAYERSTATICDLLS_H)
#define CUSTOMPLAYERSTATICDLLS_H


#if defined(CUSTOM_PLAYER_STATIC)


#ifdef WIN32
#define CDECL_CALL __cdecl
#else
#define CDECL_CALL
#endif


//----- Registration functions	
inline void CustomPlayerRegisterRenderEngine(CKPluginManager& iPluginManager);
inline void CustomPlayerRegisterReaders(CKPluginManager& iPluginManager);
inline void CustomPlayerRegisterManagers(CKPluginManager& iPluginManager);
inline void CustomPlayerRegisterBehaviors(CKPluginManager& iPluginManager);

//----- Behaviors
inline void Register3DTransfoBehaviors(CKPluginManager& iPluginManager);
inline void RegisterBBAddonsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterBBAddons2Behaviors(CKPluginManager& iPluginManager);
inline void RegisterCamerasBehaviors(CKPluginManager& iPluginManager);
inline void RegisterCharactersBehaviors(CKPluginManager& iPluginManager);
inline void RegisterCollisionsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterControllersBehaviors(CKPluginManager& iPluginManager);
inline void RegisterGridsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterInterfaceBehaviors(CKPluginManager& iPluginManager);
inline void RegisterLightsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterLogicsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterMaterialsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterMeshesBehaviors(CKPluginManager& iPluginManager);
inline void RegisterMidiBehaviors(CKPluginManager& iPluginManager);
inline void RegisterNarrativesBehaviors(CKPluginManager& iPluginManager);
inline void RegisterParticleSystemsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterShaderBehaviors(CKPluginManager& iPluginManager);
inline void RegisterSoundsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterVSLBehaviors(CKPluginManager& iPluginManager);
inline void RegisterVideoBehaviors(CKPluginManager& iPluginManager);
inline void RegisterVisualsBehaviors(CKPluginManager& iPluginManager);
inline void RegisterWorldEnvBehaviors(CKPluginManager& iPluginManager);
// network
inline void RegisterNetworkBehaviors(CKPluginManager& iPluginManager);
inline void RegisterNetworkServerBehaviors(CKPluginManager& iPluginManager);
inline void RegisterMultiPlayerBehaviors(CKPluginManager& iPluginManager);
inline void RegisterDownloadBehaviors(CKPluginManager& iPluginManager);
inline void RegisterDatabaseBehaviors(CKPluginManager& iPluginManager);
inline void RegisterCryptoBehaviors(CKPluginManager& iPluginManager);
// physics
inline void RegisterPhysicsBehaviors(CKPluginManager& iPluginManager);

//----- Managers 
inline void RegisterParamOpManager(CKPluginManager& iPluginManager);
inline void RegisterInputManager(CKPluginManager& iPluginManager);
inline void RegisterSoundManager(CKPluginManager& iPluginManager);
inline void RegisterCKFEMgrManager(CKPluginManager& iPluginManager);
inline void RegisterVideoManager(CKPluginManager& iPluginManager);
inline void RegisterDX8VideoManager(CKPluginManager& iPluginManager);
inline void RegisterDX9VideoManager(CKPluginManager& iPluginManager);

//----- Readers 
inline void RegisterVirtoolsReader(CKPluginManager& iPluginManager);
inline void RegisterImageReader(CKPluginManager& iPluginManager);
inline void RegisterAVIReader(CKPluginManager& iPluginManager);
inline void RegisterPNGReader(CKPluginManager& iPluginManager);
inline void RegisterJPGReader(CKPluginManager& iPluginManager);
inline void RegisterWAVReader(CKPluginManager& iPluginManager);
inline void RegisterTIFFReader(CKPluginManager& iPluginManager);



//-----------------------------------------------------------
// When behaviors and plugins are compiled in a static library : 
// List of declaration functions for every possible plugins...

struct	CKRasterizerInfo;
struct	CKPluginInfo;
class	CKDataReader;

/*******************************************
+ There is only one function a rasterizer Dll is supposed
to export :"CKRasterizerGetInfo", it will be used by the render engine 
to retrieve information about the plugin :
- Description 
******************************************/
typedef void  (*CKRST_GETINFO)(CKRasterizerInfo*); 

/***************************************************/
/**** RENDER ENGINE ********************************/
CKPluginInfo* CDECL_CALL CK2_3DGetPluginInfo(int Index);

/***************************************************/
/***** RASTERIZERS *********************************/
void CDECL_CALL CKDX7RasterizerGetInfo(CKRasterizerInfo* Info);
void CDECL_CALL CKDX8RasterizerGetInfo(CKRasterizerInfo* Info);
void CDECL_CALL CKDX9RasterizerGetInfo(CKRasterizerInfo* Info);
void CDECL_CALL CKGL15RasterizerGetInfo(CKRasterizerInfo* Info);

/***************************************************/
/**** READERS **************************************/
//---- Virtools Reader (4)
CKPluginInfo* CDECL_CALL CKGetVirtoolsPluginInfo(int index);
CKDataReader* CDECL_CALL CKGetVirtoolsReader(int index);

//---- Image Reader (3)
CKPluginInfo* CDECL_CALL CKGet_ImageReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_ImageReader_Reader(int pos);

//---- Avi Reader (1)
CKPluginInfo* CDECL_CALL CKGet_AviReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_AviReader_Reader(int pos);

//---- PNG Reader (1)
CKPluginInfo* CDECL_CALL CKGet_PngReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_PngReader_Reader(int pos);

//---- JPG Reader (1)
CKPluginInfo* CDECL_CALL CKGet_JpgReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_JpgReader_Reader(int pos);

//---- Wav Reader (3)
CKPluginInfo* CDECL_CALL CKGet_WavReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_WavReader_Reader(int pos);

//---- Tif Reader (1)
CKPluginInfo* CDECL_CALL CKGet_TifReader_PluginInfo(int index);
CKDataReader* CDECL_CALL CKGet_TifReader_Reader(int pos);

/***************************************************/
/**** EXTENSIONS ***********************************/
CKPluginInfo* CDECL_CALL CKGet_ParamOp_PluginInfo(int Index);


/***************************************************/
/**** MANAGERS ***********************************/
CKPluginInfo* CDECL_CALL CKGet_InputManager_PluginInfo(int Index);
CKPluginInfo* CDECL_CALL CKGet_SoundManager_PluginInfo(int Index);
CKPluginInfo* CDECL_CALL CKGet_VideoManager_PluginInfo(int Index);
CKPluginInfo* CDECL_CALL CKGet_CKFEMgr_PluginInfo(int Index);

CKPluginInfo* CDECL_CALL CKGet_Dx8VideoManager_PluginInfo(int Index);
CKPluginInfo* CDECL_CALL CKGet_Dx9VideoManager_PluginInfo(int Index);
void CDECL_CALL Register_Dx8VideoManager_BehaviorDeclarations(XObjectDeclarationArray *reg);
void CDECL_CALL Register_Dx9VideoManager_BehaviorDeclarations(XObjectDeclarationArray *reg);


/***************************************************/
/**** BEHAVIORS ***********************************/



//--- 3D Transfo (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_3DTransfo_PluginInfo(int index);
void CDECL_CALL Register_3DTransfo_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ BuildingBlock Addons (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_BBAddons_PluginInfo(int Index);
void CDECL_CALL Register_BBAddons_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ BuildingBlock Addons 2 (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_BBAddons2_PluginInfo(int Index);
void CDECL_CALL Register_BBAddons2_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Cameras (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Cameras_PluginInfo(int Index);
void CDECL_CALL Register_Cameras_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Controllers (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Controllers_PluginInfo(int Index);
void CDECL_CALL Register_Controllers_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Characters (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Characters_PluginInfo(int Index);
void CDECL_CALL Register_Characters_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Collisions (1: Coll Manager, 2 : Floor Manager , 3 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Collisions_PluginInfo(int Index);
void CDECL_CALL Register_Collisions_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Grids (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_Grids_PluginInfo(int index);
void CDECL_CALL Register_Grids_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Interface (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_Interface_PluginInfo(int index);
void CDECL_CALL Register_Interface_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Lights (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Lights_PluginInfo(int Index);
void CDECL_CALL Register_Lights_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Logics (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Logics_PluginInfo(int Index);
void CDECL_CALL Register_Logics_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Materials (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Materials_PluginInfo(int Index);
void CDECL_CALL Register_Materials_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Meshes (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_MeshModifiers_PluginInfo(int Index);
void CDECL_CALL Register_MeshModifiers_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Midi  (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_MidiBehaviors_PluginInfo(int Index);
void CDECL_CALL Register_MidiBehaviors_BehaviorDeclarations(XObjectDeclarationArray *reg);

//------ Narratives (1 : Beh)
CKPluginInfo* CDECL_CALL CKGet_Narratives_PluginInfo(int Index);
void CDECL_CALL Register_Narratives_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Particles (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_Particles_PluginInfo(int Index);
void CDECL_CALL Register_Particles_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Physics (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_Physics_PluginInfo(int Index);
void CDECL_CALL Register_Physics_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Sounds (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_Sounds_PluginInfo(int Index);
void CDECL_CALL Register_Sounds_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Visuals (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_Visuals_PluginInfo(int Index);
void CDECL_CALL Register_Visuals_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- World Env (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_WorldEnvironment_PluginInfo(int Index);
void CDECL_CALL Register_WorldEnvironment_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- VSManager (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_VSManager_PluginInfo(int Index);
void CDECL_CALL Register_VSManager_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- VSServerManager (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_VSServerManager_PluginInfo(int Index);
void CDECL_CALL Register_VSServerManager_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- MultiPlayer (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_MP_PluginInfo(int Index);
void CDECL_CALL Register_MP_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- DownloadMedia (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_DLM_PluginInfo(int Index);
void CDECL_CALL Register_DLM_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Database (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_DBC_PluginInfo(int Index);
void CDECL_CALL Register_DBC_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- VSLManager (1: Beh, 2 : Manager)
CKPluginInfo* CDECL_CALL CKGet_VSLManager_PluginInfo(int Index);
void CDECL_CALL Register_VSLManager_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- ShaderManager ( 1 : Manager, 2 : Beh) 

CKPluginInfo* CDECL_CALL CKGet_Shaders_PluginInfo(int Index);
void CDECL_CALL Register_Shaders_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- CryptedLoader (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_CryptedLoader_PluginInfo(int Index);
void CDECL_CALL Register_CryptedLoader_BehaviorDeclarations(XObjectDeclarationArray *reg);

//--- Video (1: Beh)
CKPluginInfo* CDECL_CALL CKGet_Video_PluginInfo(int Index);
void CDECL_CALL Register_Video_BehaviorDeclarations(XObjectDeclarationArray *reg);



//--------------------- Implementation -------------------------------------//
//---------------------		  of       -------------------------------------//
//--------------------- registration functions -----------------------------//


/****************************************************************************
BEHAVIORS
*******************************************************************************/

inline void Register3DTransfoBehaviors(CKPluginManager& iPluginManager)
{
	//--- 3D Tranfo
	iPluginManager.RegisterPluginInfo(0,CKGet_3DTransfo_PluginInfo(0),Register_3DTransfo_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_3DTransfo_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("3DTransfo",2);
}

inline void RegisterBBAddonsBehaviors(CKPluginManager& iPluginManager)
{
	//--- BuildingBlock Addons (1 : Beh)
	iPluginManager.RegisterPluginInfo(0,CKGet_BBAddons_PluginInfo(0),Register_BBAddons_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("BBAddons",1);
}

inline void RegisterBBAddons2Behaviors(CKPluginManager& iPluginManager)
{
	//--- BuildingBlock Addons 2 (1 : Beh)
	iPluginManager.RegisterPluginInfo(0,CKGet_BBAddons2_PluginInfo(0),Register_BBAddons2_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("BBAddons2",1);
}

inline void RegisterCamerasBehaviors(CKPluginManager& iPluginManager)
{
	//-- Cameras
	iPluginManager.RegisterPluginInfo(0,CKGet_Cameras_PluginInfo(0),Register_Cameras_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Cameras",1);
}

inline void RegisterControllersBehaviors(CKPluginManager& iPluginManager)
{
	//-- Controllers
	iPluginManager.RegisterPluginInfo(0,CKGet_Controllers_PluginInfo(0),Register_Controllers_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Controllers",1);
}

inline void RegisterCharactersBehaviors(CKPluginManager& iPluginManager)
{
	//-- Characters
	iPluginManager.RegisterPluginInfo(0,CKGet_Characters_PluginInfo(0),Register_Characters_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Character",1);
}

inline void RegisterCollisionsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Collisions
	iPluginManager.RegisterPluginInfo(0,CKGet_Collisions_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Collisions_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterPluginInfo(2,CKGet_Collisions_PluginInfo(2),Register_Collisions_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Collision",3);
}

inline void RegisterGridsBehaviors(CKPluginManager& iPluginManager)
{
	//--- Grids
	iPluginManager.RegisterPluginInfo(0,CKGet_Grids_PluginInfo(0),Register_Grids_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Grids_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Grids",2);
}

inline void RegisterInterfaceBehaviors(CKPluginManager& iPluginManager)
{
	//--- Interface
	iPluginManager.RegisterPluginInfo(0,CKGet_Interface_PluginInfo(0),Register_Interface_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Interface_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Interface",2);
}

inline void RegisterLightsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Lights
	iPluginManager.RegisterPluginInfo(0,CKGet_Lights_PluginInfo(0),Register_Lights_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Lights",1);
}

inline void RegisterLogicsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Logics
	iPluginManager.RegisterPluginInfo(0,CKGet_Logics_PluginInfo(0),Register_Logics_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Logics",1);
}

inline void RegisterMaterialsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Materials
	iPluginManager.RegisterPluginInfo(0,CKGet_Materials_PluginInfo(0),Register_Materials_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Materials",1);
}

inline void RegisterMeshesBehaviors(CKPluginManager& iPluginManager)
{
	//--- Meshes
	iPluginManager.RegisterPluginInfo(0,CKGet_MeshModifiers_PluginInfo(0),Register_MeshModifiers_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_MeshModifiers_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Meshes",2);
}

inline void RegisterMidiBehaviors(CKPluginManager& iPluginManager)
{
	//--- Midi
	iPluginManager.RegisterPluginInfo(0,CKGet_MidiBehaviors_PluginInfo(0),Register_MidiBehaviors_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_MidiBehaviors_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Midi",2);
}

inline void RegisterNarrativesBehaviors(CKPluginManager& iPluginManager)
{
	//-- Narratives
	iPluginManager.RegisterPluginInfo(0,CKGet_Narratives_PluginInfo(0),Register_Narratives_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Narratives",1);
}

inline void RegisterParticleSystemsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Particle systems 
	iPluginManager.RegisterPluginInfo(0,CKGet_Particles_PluginInfo(0),Register_Particles_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Particles_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("ParticleSystems",2);
}

inline void RegisterPhysicsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Physics
	iPluginManager.RegisterPluginInfo(0,CKGet_Physics_PluginInfo(0),Register_Physics_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Physics_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Physics",2);
}

inline void RegisterSoundsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Sounds
	iPluginManager.RegisterPluginInfo(0,CKGet_Sounds_PluginInfo(0),Register_Sounds_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Sounds",1);
}

inline void RegisterShaderBehaviors(CKPluginManager& iPluginManager)
{
	//-- Shader Behaviors
	iPluginManager.RegisterPluginInfo(0,CKGet_Shaders_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Shaders_PluginInfo(1),Register_Shaders_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Shaders",2);
}

inline void RegisterVideoBehaviors(CKPluginManager& iPluginManager)
{
	//-- Visuals
	iPluginManager.RegisterPluginInfo(0,CKGet_Video_PluginInfo(0),Register_Video_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Video",1);
}

inline void RegisterVisualsBehaviors(CKPluginManager& iPluginManager)
{
	//-- Visuals
	iPluginManager.RegisterPluginInfo(0,CKGet_Visuals_PluginInfo(0),Register_Visuals_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Visuals",1);
}

inline void RegisterWorldEnvBehaviors(CKPluginManager& iPluginManager)
{
	//-- World Env
	iPluginManager.RegisterPluginInfo(0,CKGet_WorldEnvironment_PluginInfo(0),Register_WorldEnvironment_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("WorldEnv",1);
}

inline void RegisterNetworkBehaviors(CKPluginManager& iPluginManager)
{
	//-- Network Manager
	iPluginManager.RegisterPluginInfo(0,CKGet_VSManager_PluginInfo(0),Register_VSManager_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_VSManager_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Network",2);
}

inline void RegisterNetworkServerBehaviors(CKPluginManager& iPluginManager)
{
	//-- Network Server Manager
	iPluginManager.RegisterPluginInfo(0,CKGet_VSServerManager_PluginInfo(0),Register_VSServerManager_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_VSServerManager_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("NetworkServer",2);
}

inline void RegisterMultiPlayerBehaviors(CKPluginManager& iPluginManager)
{
	//-- MultiPlayer
	iPluginManager.RegisterPluginInfo(0,CKGet_MP_PluginInfo(0),Register_MP_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_MP_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("MultiPlayer",2);
}

inline void RegisterDownloadBehaviors(CKPluginManager& iPluginManager)
{
	//-- DownloadMedia
	iPluginManager.RegisterPluginInfo(0,CKGet_DLM_PluginInfo(0),Register_DLM_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("DownloadMedia",1);
}

inline void RegisterDatabaseBehaviors(CKPluginManager& iPluginManager)
{
	//-- Database
	iPluginManager.RegisterPluginInfo(0,CKGet_DBC_PluginInfo(0),Register_DBC_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("Database",1);
}

inline void RegisterVSLBehaviors(CKPluginManager& iPluginManager)
{
	//-- VSL
	iPluginManager.RegisterPluginInfo(0,CKGet_VSLManager_PluginInfo(0),Register_VSLManager_BehaviorDeclarations,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_VSLManager_PluginInfo(1),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("VSL",2);
}

inline void RegisterCryptoBehaviors(CKPluginManager& iPluginManager)
{
	//-- CryptedLoader
	iPluginManager.RegisterPluginInfo(0,CKGet_CryptedLoader_PluginInfo(0),Register_CryptedLoader_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("CryptedLoader",1);
}


/****************************************************************************
MANAGERS
*******************************************************************************/


inline void RegisterParamOpManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_ParamOp_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("CKParamOp",1);
}

inline void RegisterCKFEMgrManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_CKFEMgr_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("CKFEMgr",1);
}

inline void RegisterInputManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_InputManager_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("InputManager",1);
}

inline void RegisterSoundManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_SoundManager_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("SoundManager",1);
}

inline void RegisterVideoManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_VideoManager_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("VideoManager",1);
}

inline void RegisterDX8VideoManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_Dx8VideoManager_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Dx8VideoManager_PluginInfo(1),Register_Dx8VideoManager_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("DX8VideoManager",2);
}

inline void RegisterDX9VideoManager(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_Dx9VideoManager_PluginInfo(0),NULL,NULL);
	iPluginManager.RegisterPluginInfo(1,CKGet_Dx9VideoManager_PluginInfo(1),Register_Dx9VideoManager_BehaviorDeclarations,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("DX9VideoManager",2);
}


/****************************************************************************
READERS
*******************************************************************************/


inline void RegisterVirtoolsReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGetVirtoolsPluginInfo(0),NULL,CKGetVirtoolsReader);
	iPluginManager.RegisterPluginInfo(1,CKGetVirtoolsPluginInfo(1),NULL,CKGetVirtoolsReader);
	iPluginManager.RegisterPluginInfo(2,CKGetVirtoolsPluginInfo(2),NULL,CKGetVirtoolsReader);
	iPluginManager.RegisterPluginInfo(3,CKGetVirtoolsPluginInfo(3),NULL,CKGetVirtoolsReader);
	iPluginManager.RegisterNewStaticLibAsDll("Virtools Reader",4);
}

inline void RegisterImageReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_ImageReader_PluginInfo(0),NULL,CKGet_ImageReader_Reader);
	iPluginManager.RegisterPluginInfo(1,CKGet_ImageReader_PluginInfo(1),NULL,CKGet_ImageReader_Reader);
	iPluginManager.RegisterPluginInfo(2,CKGet_ImageReader_PluginInfo(2),NULL,CKGet_ImageReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("Image Reader",3);
}

inline void RegisterAVIReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_AviReader_PluginInfo(0),NULL,CKGet_AviReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("AVI Reader",1);
}

inline void RegisterPNGReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_PngReader_PluginInfo(0),NULL,CKGet_PngReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("PNG Reader",1);
}

inline void RegisterJPGReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_JpgReader_PluginInfo(0),NULL,CKGet_JpgReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("JPG Reader",1);
}

inline void RegisterWAVReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_WavReader_PluginInfo(0),NULL,CKGet_WavReader_Reader);
	iPluginManager.RegisterPluginInfo(1,CKGet_WavReader_PluginInfo(1),NULL,CKGet_WavReader_Reader);
	iPluginManager.RegisterPluginInfo(2,CKGet_WavReader_PluginInfo(2),NULL,CKGet_WavReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("Wav Reader",3);
}

inline void RegisterTIFFReader(CKPluginManager& iPluginManager)
{
	iPluginManager.RegisterPluginInfo(0,CKGet_TifReader_PluginInfo(0),NULL,CKGet_TifReader_Reader);
	iPluginManager.RegisterNewStaticLibAsDll("Tiff Reader",1);
}




inline void CustomPlayerRegisterRenderEngine(CKPluginManager& iPluginManager)
{
#if defined(USE_DX8)
	iPluginManager.AddRenderEngineRasterizer(CKDX8RasterizerGetInfo);
#else
	iPluginManager.AddRenderEngineRasterizer(CKDX9RasterizerGetInfo);
#endif
	iPluginManager.AddRenderEngineRasterizer(CKGL15RasterizerGetInfo);
	iPluginManager.RegisterPluginInfo(0,CK2_3DGetPluginInfo(0),NULL,NULL);
	iPluginManager.RegisterNewStaticLibAsDll("ck2_3d",1);
}

inline void CustomPlayerRegisterReaders(CKPluginManager& iPluginManager)
{
	RegisterVirtoolsReader(iPluginManager);
	RegisterImageReader(iPluginManager);
	RegisterAVIReader(iPluginManager);
	RegisterPNGReader(iPluginManager);
	RegisterJPGReader(iPluginManager);
	RegisterWAVReader(iPluginManager);
#if !defined(_DEBUG)
	RegisterTIFFReader(iPluginManager);	
#endif
}

inline void CustomPlayerRegisterManagers(CKPluginManager& iPluginManager)
{
	RegisterParamOpManager(iPluginManager);
	RegisterInputManager(iPluginManager);
	RegisterSoundManager(iPluginManager);
	RegisterCKFEMgrManager(iPluginManager);
	//RegisterVideoManager(iPluginManager);
#if defined(USE_DX8)
	RegisterDX8VideoManager(iPluginManager);
#else
	//RegisterDX9VideoManager(iPluginManager);
#endif
}

inline void CustomPlayerRegisterBehaviors(CKPluginManager& iPluginManager)
{

	Register3DTransfoBehaviors(iPluginManager);
	RegisterBBAddonsBehaviors(iPluginManager);
	RegisterBBAddons2Behaviors(iPluginManager);
	RegisterCamerasBehaviors(iPluginManager);
	RegisterCharactersBehaviors(iPluginManager);
	RegisterCollisionsBehaviors(iPluginManager);
	RegisterControllersBehaviors(iPluginManager);
	RegisterGridsBehaviors(iPluginManager);
	RegisterInterfaceBehaviors(iPluginManager);
	RegisterLightsBehaviors(iPluginManager);
	RegisterLogicsBehaviors(iPluginManager);
	RegisterMaterialsBehaviors(iPluginManager);
	RegisterMeshesBehaviors(iPluginManager);
	RegisterMidiBehaviors(iPluginManager);
	RegisterNarrativesBehaviors(iPluginManager);
	RegisterParticleSystemsBehaviors(iPluginManager);
	//RegisterShaderBehaviors(iPluginManager);
	RegisterSoundsBehaviors(iPluginManager);
	RegisterVSLBehaviors(iPluginManager);
	//RegisterVideoBehaviors(iPluginManager);
	RegisterVisualsBehaviors(iPluginManager);
	RegisterWorldEnvBehaviors(iPluginManager);


	RegisterPhysicsBehaviors(iPluginManager);
/*
	RegisterCryptoBehaviors(iPluginManager);
	RegisterNetworkBehaviors(iPluginManager);
	RegisterNetworkServerBehaviors(iPluginManager);
	RegisterMultiPlayerBehaviors(iPluginManager);
	RegisterDownloadBehaviors(iPluginManager);
	RegisterDatabaseBehaviors(iPluginManager);
	*/
	

}



#endif // CUSTOM_PLAYER_STATIC


#endif // CUSTOMPLAYERSTATICDLLS_H