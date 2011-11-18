#include "crypting.h"
#include <Windows.h>
#include <StdIO.h>
#include <Math.h>

int EncryptPassword(char* pcPassword)
{
	
	int aiKey[50] = {0x02, 0x03, 0x05, 0x07, 0x11, 0x13, 0x17, 0x19, 0x23, 0x29,
	 0xA2, 0xB3, 0xC5, 0xD7, 0xE1, 0xF3, 0xA7, 0xB9, 0xC3, 0xD9,
	 0x93, 0xA4, 0xB6, 0xC8, 0xD2, 0xE4, 0x98, 0xA8, 0xB4, 0xC8,
	 0x46, 0x58, 0x63, 0x67, 0x74, 0x78, 0x57, 0x57, 0x68, 0x67,
	 0xA9, 0xBC, 0xC9, 0xDF, 0xF6, 0x0C, 0xBF, 0xCF, 0xFC, 0xFF};


	char acPassword[256];
	char acTemp[3];


	if(pcPassword == NULL) return TRUE;
	ZeroMemory(acPassword, 256 * sizeof(char));


	srand(strlen(pcPassword) * 17);
	for(int a = 0; a < (int)(strlen(pcPassword)); a++)
	{
		srand(rand() + strlen(pcPassword) * a);
		srand((rand() % (aiKey[a % 50])) + (rand() % (aiKey[(a * 23) % 50])));
		srand((rand() % (aiKey[(a + 2305) % 50])) + (rand() % (aiKey[(17 + a * 133) % 50])) * 177);


		acPassword[a] = pcPassword[a] + (rand() % 256);

		srand(((BYTE)(acPassword[a]) + 1) * (a + 23) + (rand() % 1381));
		//ok, that´s crazy enough
	}

	ZeroMemory(pcPassword, strlen(pcPassword) * sizeof(char));


	
	for(int a = 0; a < (int)(strlen(acPassword)); a++)
	{
		sprintf(acTemp, "%02x", (byte)(acPassword[a]));
		strcat(pcPassword, acTemp);
	}
	
	
	return FALSE;
}
