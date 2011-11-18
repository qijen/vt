#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pConfig.h"

#ifdef DONGLE_VERSION

	#include "matrix32.h"

#endif


long  DataIn[256];    /* Buffer to read the Dongle data       */
long  DataOut[256];   /* Buffer for data to be stored         */
long  DataBlock[2];   /* Data buffer for Encrypt/Decrypt      */
short RetCode;        /* Return value                         */
long  API_Version;    /* Version number of the Matrix-API     */
long  DNG_Version;    /* Dongle version number                */
short DNG_LPTADR;     /* Adress of LPT port                   */
short DNG_Count;      /* Number of Dongles connected          */
short DNG_Mem;        /* Memory size of Dongle                */
short DNG_MaxVar;     /* Maximum number of data fields        */
short AppSlot;        /* Application-Slot for Network access  */
short i;



int netMode = 0;
short DNG_Port = 1;
long  UserCode = 4002529;

int DONGLE_BASIC_VERSION_KEY_1 = 28071977;
int DONGLE_BASIC_VERSION_KEY_2 = 77917082;

int DONGLE_BASIC_VERSION_KEY_1_ENC = 364898188;
int DONGLE_BASIC_VERSION_KEY_2_ENC = 930141357;



int DONGLE_ADVANCED_VERSION_KEY1 = 56143954;
int DONGLE_ADVANCED_VERSION_KEY2 = 45934165;


extern PhysicManager*manager;


#ifdef DONGLE_VERSION


#pragma comment(lib,"matrix32.lib")



void PhysicManager::_initResources(int flags){

	

	// Init Matrix-API                                  
	RetCode = Init_MatrixAPI();
	if(RetCode < 0)
	{
		printf("Init_MatrixAPI failed  %d \n", RetCode);
		exit;
	}

	API_Version = GetVersionAPI();
	if(API_Version == 0)
	{
		printf("Cannot read API-Version! \n");
		Release_MatrixAPI();
		return;
	}
	
	// Search for number of Dongles at DNG_Port          
	DNG_Port = Dongle_Find();
	DNG_Count = Dongle_Count(DNG_Port);

	long test = 0 ;
	if(DNG_Count > 0)
	{
		//manager->m_Context->OutputToConsoleEx("Matrix-Modules at Port %d: %d \n", DNG_Port, DNG_Count);
	}
	else
	{
		XString donglePath = _getConfigPath();

		//manager->m_Context->OutputToConsoleEx("Cannot find Matrix-Modules at Port %d ! \n", DNG_Port);
		
		//return;
		//goto NETWORK_CHECK;

		int ret  = SetConfig_MatrixNet(1,donglePath.Str());
		int DNG_NR = 1;
		AppSlot = 1;
		ret = LogIn_MatrixNet(UserCode, AppSlot, DNG_NR);
		if (ret<=0)
		{
			MessageBox(NULL,"Couldn't find Dongle!",0,MB_OK|MB_ICONERROR);
			this->DongleHasBasicVersion=0;
			this->DongleHasAdvancedVersion=0;
			//Release_MatrixAPI();
			return;
		}


		netMode = 1;
		DNG_Port = Dongle_Find();
		DNG_Count = Dongle_Count(DNG_Port);

		if(DNG_Count == 0)
		{
			manager->m_Context->OutputToConsoleEx("Couldn't find Dongle");
		}

	}

	
	DNG_Mem = Dongle_MemSize(DNG_Count, DNG_Port);
	if(DNG_Mem > 0)
	{
		//manager->m_Context->OutputToConsoleEx("MemSize of Matrix-Module %d at Port %d: %d Bytes \n", DNG_Count, DNG_Port, DNG_Mem);

	}
	else
	{
		//manager->m_Context->OutputToConsoleEx("Cannot read MemSize! \n");
		Release_MatrixAPI();
		return;
	}

	RetCode = Dongle_ReadData(UserCode, DataIn,6, DNG_Count, DNG_Port);
	if(RetCode < 0)
	{
		m_Context->OutputToConsoleEx("Data Read-Error! \n");
		Release_MatrixAPI();
		return;
	}

	
	DataBlock[0] = 28071977;  /* Clear Data */
	DataBlock[1] = 77917082;  /* Clear Data */


	long DataBlockKeyBasic[2];
	DataBlockKeyBasic[0]  = DataIn[2];
	DataBlockKeyBasic[1]  = DataIn[3];

	long DataBlockKeyAdvanced[2];
	DataBlockKeyAdvanced[0]  = DataIn[4];
	DataBlockKeyAdvanced[1]  = DataIn[5];

	RetCode = Dongle_DecryptData(UserCode, DataBlockKeyBasic, DNG_Count, DNG_Port);
	RetCode = Dongle_DecryptData(UserCode, DataBlockKeyAdvanced, DNG_Count, DNG_Port);

	


	if (netMode)
	{
	
		if (DataBlockKeyBasic[0]==DONGLE_BASIC_VERSION_KEY_1_ENC && DataBlockKeyBasic[1]==DONGLE_BASIC_VERSION_KEY_2_ENC )
		{
			DongleHasBasicVersion= 1;
		}

	}else
	{
		if (DataBlockKeyBasic[0]==DONGLE_BASIC_VERSION_KEY_1 && DataBlockKeyBasic[1]==DONGLE_BASIC_VERSION_KEY_2 )
		{
			DongleHasBasicVersion= 1;
		}

		if (DataBlockKeyAdvanced[0]==DONGLE_ADVANCED_VERSION_KEY1 && DataBlockKeyAdvanced[1]==DONGLE_ADVANCED_VERSION_KEY2)
		{
			DongleHasAdvancedVersion=1;
		}
	}

	Release_MatrixAPI();


}

#endif



/*
#ifdef REDIST
#endif
*/

/*
#ifdef DEMO_ONLY

	void FindResourceX()
	{

	}

#endif
*/
	
	

/*
#if defined (DONGLE_VERSION)


void PhysicManager::makeDongleTest()
{
	//FindResourceX();

}
*/



/*

void MODULE_API FindResourceX()
{
	

	RetCode = Init_MatrixAPI();
	if(RetCode < 0)
	{
		printf("Init_MatrixAPI failed  %d \n", RetCode);
		exit;
	}

	
	API_Version = GetVersionAPI();
	if(API_Version == 0)
	{
		printf("Cannot read API-Version! \n");
		Release_MatrixAPI();
		return;
	}
	else
	{
	//		printf("Version of Matrix-API: %d.%d \n", HIWORD(API_Version), LOWORD(API_Version));
	//		manager->m_Context->OutputToConsoleEx("Version of Matrix-API: %d.%d \n", HIWORD(API_Version), LOWORD(API_Version));
	}

	
	int port = Dongle_Find();
	DNG_Count = Dongle_Count(port);
	DNG_Port = port;

	if(DNG_Count > 0)
	{
		//manager->m_Context->OutputToConsoleEx("Matrix-Modules at Port %d: %d \n", DNG_Port, DNG_Count);
	}
	else
	{
		//goto NETWORK
		//manager->m_Context->OutputToConsoleEx("Cannot find Matrix-Modules at Port %d ! \n", DNG_Port);
		Release_MatrixAPI();
		return;
	}

	DNG_Mem = Dongle_MemSize(DNG_Count, DNG_Port);
	if(DNG_Mem > 0)
	{
	
		//manager->m_Context->OutputToConsoleEx("MemSize of Matrix-Module %d at Port %d: %d Bytes \n", DNG_Count, DNG_Port, DNG_Mem);

	}
	else
	{
		//manager->m_Context->OutputToConsoleEx("Cannot read MemSize! \n");
		Release_MatrixAPI();
		return;
	}

	RetCode = Dongle_ReadData(UserCode, DataIn,6, DNG_Count, DNG_Port);
	if(RetCode < 0)
	{
		manager->m_Context->OutputToConsoleEx("Data Read-Error! \n");
		Release_MatrixAPI();
		return;
	}

	
	
	DataBlock[0] = 28071977;  
	DataBlock[1] = 77917082;  



	long DataBlockKeyBasic[2];
	DataBlockKeyBasic[0]  = DataIn[2];
	DataBlockKeyBasic[1]  = DataIn[3];

	long DataBlockKeyAdvanced[2];
	DataBlockKeyAdvanced[0]  = DataIn[4];
	DataBlockKeyAdvanced[1]  = DataIn[5];

	RetCode = Dongle_DecryptData(UserCode, DataBlockKeyBasic, DNG_Count, DNG_Port);
	RetCode = Dongle_DecryptData(UserCode, DataBlockKeyAdvanced, DNG_Count, DNG_Port);

	//manager->m_Context->OutputToConsoleEx("Decrypted Data:  %lu  %lu \n", DataBlockKeyBasic[0], DataBlockKeyBasic[1]);
	//manager->m_Context->OutputToConsoleEx("Decrypted Data:  %lu  %lu \n", DataBlockKeyAdvanced[0], DataBlockKeyAdvanced[1]);


	if (DataBlockKeyBasic[0]==DONGLE_BASIC_VERSION_KEY_1 && DataBlockKeyBasic[1]==DONGLE_BASIC_VERSION_KEY_2 )
	{
//		DongleHasBasicVersion= 1;
	}

	if (DataBlockKeyAdvanced[0]==DONGLE_ADVANCED_VERSION_KEY1 && DataBlockKeyAdvanced[1]==DONGLE_ADVANCED_VERSION_KEY2)
	{
//		DongleHasAdvancedVersion= 1;
	}

	//manager->m_Context->OutputToConsoleEx("Clear Data:          %lu  %lu \n", DataBlock[0], DataBlock[1]);
	//manager->m_Context->OutputToConsoleEx("Encrypted Data:  %lu  %lu \n", DataBlock[0], DataBlock[1]);

	
	Release_MatrixAPI();
}



#endif

*/