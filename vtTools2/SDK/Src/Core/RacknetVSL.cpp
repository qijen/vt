#include "InitMan.h"
#include "CKAll.h"

#include "VSLManagerSDK.h"

#ifdef RACKNET

#include <network/rNetServer.h>
#include "network/racknet/NetworkStructures.h"
#include "network/racknet/PacketEnumerations.h"

#include "network/laser_point.h"
#include "network/rNetStructs.h"

#pragma comment (lib,"RakNet.lib")

extern InitMan *_im;
static rNetServer *server = NULL;

unsigned char GetPacketIdentifier(Packet *p);
laser_pointers *lPointers;

lpoint pointers[5];
boolean recieved =false;
int packetDelay;
int prevPacketCount;

int pCount =0;
int GetPacketDT();


//////////////////////////////////////////////////////////////////////////
BOOL rNetServerCreate(const char*server_name,int server_port);
BOOL rNetServerStart(){
	return server->start();
}
void rNetServerDestroy(){
	if(server->realInterface){
		server->destroy();
	}
}
int rNetServerGetClientCount(){

	
	return server->GetClientCount();


}

int rNetServerGetBytesReceivedPerSecond(){
	
		return server->realInterface->GetBytesReceivedPerSecond();

	return -1;

}

int GetPacketOutputBufferSize(){
	return server->realInterface->GetPacketOutputBufferSize();
	}



int rGetPacketsPerSecond(){


	int just = server->realInterface->GetReceivedPacketCount();
	int res = just - prevPacketCount;
	prevPacketCount = just;
	
	return res;

}
int rGetPaketCount(){
	
	return server->realInterface->GetReceivedPacketCount();
	
}

//////////////////////////////////////////////////////////////////////////
lpoint GetCoord(int index){
	
	if(recieved){
		return pointers[index];
	}
	return lpoint(-1,-1,-1);

}

VxRect rNetRecieve(){

	Packet* p = server->realInterface->Receive();
	
	if (p!=0){
        recieved =true;
		lPointers = ((laser_pointers*)p->data);
        
		/*
		for (int  i = 0 ; i < 10 ; i++){
            pointers[i].x = lPointers->LArray[i].x;
			pointers[i].y = lPointers->LArray[i].y;

		}
    
		//Vx2DVector coord = GetLPCoord(0);
		char buffer[400];
		lpoint coord  = GetCoord(0);
		sprintf(buffer,"x : %d, y: %d",coord.x,coord.y);
		_im->m_Context->OutputToConsole(buffer,false);
		*/
                
		VxRect m (lPointers->LArray[0].x,lPointers->LArray[0].y,lPointers->LArray[0].x,lPointers->LArray[0].x);

// 		server->realInterface->DeallocatePacket(p);
// 		server->realInterface->DesynchronizeAllMemory();
		pCount =1;
		return m;

	}
	pCount = -1;
	recieved = false;

	return VxRect(-1,-1,-1,-1);
	}


/************************************************************************/
/*                                                                      */
/************************************************************************/
Vx2DVector GetLPCoord(int index){

	if (recieved)
		return Vx2DVector((const float)pointers[index].x,(const float)pointers[index].y);
	return Vx2DVector(-1,-1);
	}



laser_pointers& GetLaserPointerArray(){
	/*
	Packet* p = server->realInterface->Receive();

	if (p!=0){
			recieved =true;
			laser_pointers *tmpArray = ((laser_pointers*)p->data);
			
			for (int  i = 0 ; i < 99 ; i++){
				pointers[i].x = tmpArray->LArray[i].x;
				pointers[i].y = tmpArray->LArray[i].y;
			}
			
			char buffer[400];
			SYSTEMTIME time;
			GetSystemTime(&time);
			packetDelay  = time.wMilliseconds - tmpArray->sStamp.miliseconds;
			WORD sDT  = time.wSecond - tmpArray->sStamp.seconds;


			//sprintf(buffer,"dtSec : %d, dtMSec: %d", sDT, msDT );
			sprintf(buffer,"x : %f, y: %f", pointers[0].x ,pointers[0].y);
			_im->m_Context->OutputToConsole(buffer,false);


			server->realInterface->DeallocatePacket(p);
			server->realInterface->DesynchronizeAllMemory();

			return laser_pointers();

		}
	recieved = false ; */
	
	return laser_pointers();
}



void
laser_pointers::GetCoord(Vx2DVector& target,int index){
	
	if (lPointers){
		target.x = (float)pointers[index].x;
		target.y = (float)pointers[index].y;
		}
}


void
InitMan::RegisterRacknetVSL(){

	STARTVSLBIND(m_Context)



		/************************************************************************/
		/* Variable|Parameter Stuff                                                                     */
		/************************************************************************/
		DECLAREFUN_C_2(BOOL,rNetServerCreate,const char*, int )
		DECLAREFUN_C_0(BOOL,rNetServerStart)
		DECLAREFUN_C_0(void,rNetServerDestroy)
		DECLAREFUN_C_0(int,rGetPaketCount)
		
		DECLAREFUN_C_0(int,GetPacketDT)

		DECLAREFUN_C_0(int,rGetPacketsPerSecond)
		
		DECLAREFUN_C_0(int,rNetServerGetClientCount)
		DECLAREFUN_C_0(int,rNetServerGetBytesReceivedPerSecond)
		DECLAREFUN_C_0(VxRect,rNetRecieve)
		DECLAREFUN_C_0(int,GetPacketOutputBufferSize)

		
		DECLAREOBJECTTYPE(lpoint)
		DECLAREMEMBER(lpoint,int,x)
		DECLAREMEMBER(lpoint,int,y)
		DECLAREFUN_C_1(lpoint,GetCoord,int)
		
		DECLAREPOINTERTYPE(laser_pointers)
		
		DECLAREMETHODC_1(laser_pointers,lpoint,GetPointByIndex,int)

		DECLAREFUN_C_0(laser_pointers&,GetLaserPointerArray) 

		DECLAREMETHODC_2(laser_pointers,void,GetCoord,Vx2DVector&,int)
		DECLAREFUN_C_1(Vx2DVector,GetLPCoord,int)

	STOPVSLBIND

}


CKERROR InitMan::PostProcess(){
	
	/*
	if (server->realInterface &&  server->isStarted){

		Packet* p = server->realInterface->Receive();

		if (p!=0){
			lPointers = ((laser_pointers*)p->data);
			
			m_Context->OutputToConsole("processing incoming packets",false);
		
		/*
			
					char buffer[400];
					SYSTEMTIME time;
					GetSystemTime(&time);
					packetDelay  = time.wMilliseconds - lPointers->sStamp.miliseconds;
					WORD sDT  = time.wSecond - lPointers->sStamp.seconds;
		*/
		


/*
			for (int  i = 0 ; i < 4 ; i++)
			{
				//pointers[i].x = lPointers->LArray[i].x;
				//pointers[i].y = lPointers->LArray[i].y;
			}*/
/*
			char buffer[400];
			lpoint coord  = GetCoord(0);
			sprintf(buffer,"x : %d, y: %d",coord.x,coord.y);

			m_Context->OutputToConsole(buffer,false);
*/
			//server->realInterface->DeallocatePacket(p);
			//server->realInterface->DesynchronizeAllMemory();

		//}



		return CK_OK;

	//}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}

//////////////////////////////////////////////////////////////////////////
BOOL rNetServerCreate(const char*server_name,int server_port){

	server = new rNetServer(server_name,server_port);
	lPointers = new laser_pointers();
	return true;
}

//////////////////////////////////////////////////////////////////////////
int GetPacketDT(){
	return packetDelay;
}


#endif

