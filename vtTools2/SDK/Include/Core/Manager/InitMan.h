#ifndef InitMAN_H
#define InitMAN_H


#include "CKBaseManager.h"


#include <stdlib.h>
#include <map>



#define S_PARAMETER_GUID CKGUID(0x7a7104e2,0x72c56435)

#define SFLOAT_PARAMETER_GUID CKGUID(0x6a5b2ec3,0x63f84a40)

#define SCOLOR_PARAMETER_GUID CKGUID(0x47cc15d2,0xf366299)
#define SINT_PARAMETER_GUID CKGUID(0x31d3196f,0x787306e1)



#define CKPGUID_LOOPMODE		CKDEFINEGUID(0x63942d15,0x5ac51a7)


typedef enum QAD_OBJECT_TYPE {

	CHARACTER =  1,
	PUSHBOX		 = 2,
	BALL			 = 3,
	STONE			= 4,
	COIN			 = 5,
	FUEL			  = 6,
	ROCKET		   = 7,
	MOVINGBOARD = 8,
	SPRINGPAIR		=9,
}
QAD_OBJECT_TYPE;
//////////////////////////////////////////////////////////////////////////
#define VLEFT VxVector(-1.0f,0.0f,0.0f)
#define VRIGHT VxVector(1.0f,0.0f,0.0f)
#define VUP VxVector(1.0f,1.0f,0.0f)
#define VDOWN VxVector(0.0f,-1.0f,0.0f)
#define VZERO VxVector(0.0f,0.0f,0.0f)

#define PHYSIC_OBJECT_2D_PARAMETER  CKDEFINEGUID(0x57ba4ee6,0x4d8740a9)

#define PHYSIC_OBJECT_SIMULATION_FILTER  CKDEFINEGUID(0x248f1f51,0x72070f85)

#define PHYSIC_OBJECT_2D_WORLDSPRING_PARAMETER  CKDEFINEGUID(0x1b8e268b,0x11041fa0)



//////////////////////////////////////////////////////////////////////////

#include "typedefs.h"
#include "ZipDll.h"
#include "UnzipDll.h"
#include "ZCallBck.h"

BOOL __stdcall DefaultZipCallback(CZipCallbackData *pData);//thanks


#include "sharedStructs.h"



#define INIT_MAN_GUID		CKGUID(0x35824c8a,0x4e320ac4)

class InitMan : public CKBaseManager
{

	public:
		//Ctor
		InitMan(CKContext* ctx);
		//Dtor
		~InitMan();
		static InitMan* GetInstance();


		//////////////////////////////////////////////////////////////////////////
		//virtual file mapping , used for command pipe:
		HANDLE m_hMMFile;
		vtExternalEvent          *m_pData;

		typedef XHashTable<vtExternalEvent*,unsigned long>vtExternalEventQueueType;
		vtExternalEventQueueType incomingEvents;
		void PerformMessages();
		void InitMessages(int flags,XString name);

		// Initialization	
		virtual CKERROR OnCKInit();
		virtual CKERROR OnCKEnd();
		virtual CKERROR OnCKReset();
		CKERROR PreProcess();
		CKERROR PostProcess();
		virtual CKERROR PostClearAll();
		virtual CKERROR OnCKPlay();


	CKDWORD	GetValidFunctionsMask()	
	{ return CKMANAGER_FUNC_OnCKInit|
		CKMANAGER_FUNC_OnCKEnd|
		CKMANAGER_FUNC_OnCKReset|
		CKMANAGER_FUNC_PreProcess|
		CKMANAGER_FUNC_PostProcess|
		CKMANAGER_FUNC_PostClearAll|
		CKMANAGER_FUNC_OnCKPlay; 
	}

	
	/************************************************************************/
	/* Parameter Functions																	                                        */
	/************************************************************************/

	int move_object_att;	int moving_board_att;	int rocket_att; 	int keyboard_config_att;	int att_character_keyboard_config;	int att_character_anim_messages;	int att_character_anims;	int att_character_ckof;	int att_character_object_set;	int att_spring_pair;


	VxVector Position(CK3dEntity *ent);

	int att_rigid_body_2D;
	int att_rigid_body_2D_worldspring;
	int att_need_update;
	int att_do_physics;
	int att_sim_filter;


	void RegisterVSL();
	void RegisterRacknetVSL();
	
	void RegisterCEGUI_VSL(CKContext *ctx);

    void RegisterParameters();
	void RegisterParameters2();
	//void RegisterHUD();

        
	


	/************************************************************************/
	/* zip lib                                                                     */
	/************************************************************************/

	

	ZipJobList zili;

		
	bool AddFileEntry(XString ArchiveName,XString FileEntry);

		
	BOOL LoadZipDll();//from the projects resource
	BOOL UnLoadZipDll();
	void SetDefaultZipValues(CZipParams *pParams);
	int GetZipDllVersion();
		

	/************************************************************************/
	/* decompressing funcs																																			*/
	/************************************************************************/

	BOOL LoadUnzipDll();
	BOOL UnLoadUnZipDll();
	void SetDefaultUnZipValues(CUnzipParams * pParams);
	int GetUnzipDllVersion();
			

	UINT m_uiLastError;//unused
	
	/************************************************************************/
	/* compress																										                                                                     */
	/************************************************************************/
	HINSTANCE m_ZipDllHandle;
	char ZipDllTempFile[MAX_PATH];
	CZipDllExec m_ZipDllExec;
	CGetZipDllVersion   m_GetZipDllVersion;

	
	/************************************************************************/
	/* decompress																																						*/
	/************************************************************************/

	HINSTANCE m_UnzipDllHandle;
	char UnZipDllTempFile[MAX_PATH];
	CGetUnzipDllVersion m_GetUnzipDllVersion;
	CUnzipDllExec m_UnzipDllExec;


	private:

	
};

#define GetIManager() InitMan::GetInstance()


#endif
