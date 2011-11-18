#include "pch.h"
#include "InitMan.h"

#include "CKAll.h"
#include "VSLManagerSDK.h"
#include "Dll_Tools.h"
#include "crypting.h"



static InitMan *_im=NULL;
extern InitMan* _im2=NULL;

InitMan *manager=NULL;

InitMan*InitMan::GetInstance()
{
	return manager;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InitMan::InitMan(CKContext* context):CKBaseManager(context,INIT_MAN_GUID,"mw tool manager")//Name as used in profiler
{
	m_Context->RegisterNewManager(this);
	_im = this;
	_im2 = this;
	manager = this;
}

struct IntersectionDescription
	{
	// The Ray Itself
	VxRay				m_Ray;
	VxVector			m_RayEnd;
	float				m_Depth;

	VxIntersectionDesc	m_MinDesc;
	CK3dEntity*			m_MinEntity;
	float					m_MinDistance2;	// square magnitude temporary distance to help us know which will be the nearest intersected object
};

void Intersect2(CK3dEntity* Currentobject,IntersectionDescription* idesc,CKScene* scene);
IntersectionDescription Intersect(CK3dEntity* Tobject,VxVector Direction,VxVector Origin,float Depth);
VxVector GetRayDistance(CK3dEntity* ent,VxVector Direction,VxVector Origin,float Depth ){

	IntersectionDescription desc = Intersect(ent,Direction,Origin,Depth);
	return VxVector(desc.m_MinDesc.IntersectionPoint);

}

IntersectionDescription Intersect(CK3dEntity* Tobject,VxVector Direction,VxVector Origin,float Depth){

	Direction.Normalize();

	IntersectionDescription idesc;
	// Ray Members
	idesc.m_Ray.m_Origin		= Origin;
	idesc.m_Ray.m_Direction	= Direction;
	idesc.m_RayEnd			= Origin + Direction;
	idesc.m_Depth				= Depth;

	// Nearest Members
	idesc.m_MinDistance2 = Depth*Depth;
	idesc.m_MinEntity	= NULL;

	CKScene *scene = Tobject->GetCKContext()->GetCurrentScene();

	CKAttributeManager* attman = Tobject->GetCKContext()->GetAttributeManager();


	int collatt = attman->GetAttributeTypeByName("Moving Obstacle");
	int flooratt = attman->GetAttributeTypeByName("Floor");
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(collatt);
	for (CKObject** it = Array.Begin(); it != Array.End(); ++it){

		CK3dEntity *Currentobject = (CK3dEntity*)*it;
		if (Currentobject!=Tobject) 
			Intersect2(Currentobject,&idesc,scene);
		}

	if (idesc.m_MinEntity && (idesc.m_MinDistance2 < Depth*Depth)) {
	
		VxVector IntersectionPoint;
		idesc.m_MinEntity->Transform(&IntersectionPoint,&idesc.m_MinDesc.IntersectionPoint);
		VxVector IntersectionNormal;
		idesc.m_MinEntity->TransformVector(&IntersectionNormal,&idesc.m_MinDesc.IntersectionNormal);
		IntersectionNormal.Normalize();
		idesc.m_MinDistance2 = sqrtf(idesc.m_MinDistance2);
		return idesc;

		}

	return idesc;



}
void Intersect2(CK3dEntity* Currentobject,IntersectionDescription* idesc,CKScene* scene)
{
// not a 3D object

//________________________________/ Rejection if not visible
	if (CKIsChildClassOf(Currentobject,CKCID_SPRITE3D)) {
		// Ray Inter
		VxIntersectionDesc Inter;
		VxVector IntersectionPoint;
		if ( Currentobject->RayIntersection(&idesc->m_Ray.m_Origin,&idesc->m_RayEnd,&Inter,NULL) ){
			Currentobject->Transform(&IntersectionPoint, &Inter.IntersectionPoint);
			float Dist = SquareMagnitude( IntersectionPoint - idesc->m_Ray.m_Origin );
			if ( Dist < idesc->m_MinDistance2){
				idesc->m_MinDistance2	= Dist;
				idesc->m_MinEntity		= Currentobject;
				idesc->m_MinDesc		= Inter;
				}
			}


	return; // stop there
		}

//________________________________/ Rejection by Bounding Sphere / Depth
float radius = Currentobject->GetRadius();
VxVector pos;
Currentobject->GetBaryCenter( &pos );
VxVector dif = pos-idesc->m_Ray.m_Origin;
float Dist = SquareMagnitude( dif );
BOOL  character = false;


if( Dist < (idesc->m_Depth+radius)*(idesc->m_Depth+radius) ){

	//______________________________/ Rejection by Sphere / Behind
	float s = DotProduct(dif, idesc->m_Ray.m_Direction);
	if( s+radius > 0.0f ){

		//______________________________/ Rejection by Bounding Sphere / Segment
		if( radius*radius > Dist-s*s ){

			//______________________________/ Rejection by Bounding Cube (in world)
			const VxBbox& box = Currentobject->GetBoundingBox();
			VxVector IntersectionPoint;
			if (VxIntersect::RayBox(idesc->m_Ray,box)) {
				if(character) {
					int count = ((CKCharacter*)Currentobject)->GetBodyPartCount();
					while( count ){
						CKBodyPart* bp;
						if( bp = ((CKCharacter*)Currentobject)->GetBodyPart( --count ) ){
							//______________________________/ Reject BodyPart by Bounding Cube (in world)
							if( VxIntersect::RayBox(idesc->m_Ray,bp->GetBoundingBox()) ) {
								// Ray Inter
								VxIntersectionDesc Inter;
								if ( bp->RayIntersection(&idesc->m_Ray.m_Origin,&idesc->m_RayEnd,&Inter,NULL) ){
									bp->Transform(&IntersectionPoint, &Inter.IntersectionPoint);
									Dist = SquareMagnitude( IntersectionPoint - idesc->m_Ray.m_Origin );
									if ( Dist < idesc->m_MinDistance2){
										idesc->m_MinDistance2	= Dist;
										idesc->m_MinEntity		= Currentobject;
										idesc->m_MinDesc		= Inter;
										}
									}
								}
							}
						}
					} else {
						// Ray Inter
						VxIntersectionDesc Inter;
						if ( Currentobject->RayIntersection(&idesc->m_Ray.m_Origin,&idesc->m_RayEnd,&Inter,NULL) ){
							Currentobject->Transform(&IntersectionPoint, &Inter.IntersectionPoint);
							Dist = SquareMagnitude( IntersectionPoint - idesc->m_Ray.m_Origin );
							if ( Dist < idesc->m_MinDistance2){
								idesc->m_MinDistance2	= Dist;
								idesc->m_MinEntity		= Currentobject;
								idesc->m_MinDesc		= Inter;
								}
							}
						}
				}
			}
		}	
	}
}

//Saves an Object************************************************************************/
void LoadFile(CK3dEntity* ent, const char*str){



	if (strlen(str)<=0) return;

	
	CKContext *ctx = ent->GetCKContext();
	
	CKLevel* TheLevel=ctx->GetCurrentLevel();
	
	CKRenderManager *rm = ctx->GetRenderManager();
	CKRenderContext *rtx = rm->GetRenderContext(0);


	ctx->Pause();
	ctx->Reset();
	//ctx->ClearAll();

	/*
	CKObjectArray *array=CreateCKObjectArray();



	//-- Loads m_eptrs.The file and fills m_eptrs.The array with loaded objects
	CKERROR res=CK_OK;
	if ((res=ctx->Load((char*) str,array))==CK_OK)
		{
		//--- Add m_eptrs.The render context to m_eptrs.The level
		
		TheLevel->AddRenderContext(rtx,TRUE);

		//--- Take m_eptrs.The first camera we found and attach m_eptrs.The viewpoint to it.
		CK_ID* cam_ids=ctx->GetObjectsListByClassID(CKCID_CAMERA);
		if (!cam_ids) cam_ids=ctx->GetObjectsListByClassID(CKCID_TARGETCAMERA);
		if (cam_ids)
			{
			CKCamera *camera=(CKCamera *)ctx->GetObject(cam_ids[0]);
			if (camera)
				rtx->AttachViewpointToCamera(camera);
			}
		//--- Sets m_eptrs.The initial conditions for m_eptrs.The level
			TheLevel->LaunchScene(NULL);
		}


	*/

//	DeleteCKObjectArray(array);			


}
void SaveObject(CKBeObject*beo,char *filename){

	if (beo  && strlen(filename)){
		CKObjectArray* oa = CreateCKObjectArray();
		oa->InsertAt(beo->GetID());
		beo->GetCKContext()->Save(filename,oa,0xFFFFFFFF,NULL);

	}
}

/*
void SaveObjectScipt(CKBeObject*beo,char *filename,int pos){
	if (beo  && strlen(filename)){
		
		CKObjectArray* oa = CreateCKObjectArray();

		if beo->GetScriptCount() && //savity check 
			 pos <=beo->GetScriptCount() && pos>=0 //index bound check
			)
            oa->InsertAt( beo->GetScript(pos) );
		
		beo->GetCKContext()->Save(filename,oa,0xFFFFFFFF,NULL);
	}
}
*/
/**********************************/



#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#include <Windows.h>
#include <tchar.h>
#include <WTYPES.H>
#include <BASETSD.H>
#include <stdlib.h>
#include <vector>
/*
void GetSDate(int& year,int&month, int&day){



	SYSTEMTIME sys;
	GetSystemTime(&sys);
	RDateTime ft;
	ft.Set(sys);
	RDate currentRD = ft;
	year = currentRD.GetYear();
	month = currentRD.GetMonth();
	day= currentRD.GetDay();

}

void GetSTime(int&hours,int&mins,int&sec){

	SYSTEMTIME sys;
	GetSystemTime(&sys);
	RDateTime ft;
	ft.Set(sys);
	
	RTime st = ft;

	hours = st.GetHour()+1;
	mins = st.GetMinute();
	sec = st.GetSecond();
	


}
int GetDayDifference(int year,int month,int day){

	SYSTEMTIME sys;

    GetSystemTime(&sys);
    RDateTime ft;
    ft.Set(sys);
    
    RDate currentRD = ft;
    RDateTime dt;//aka filetime
    dt.SetYear(year);
    dt.SetMonth(month);
    dt.SetDay(day);
    RDate da = dt;
	currentRD = currentRD - da;
    int us = currentRD.GetYearsToDays( currentRD.GetYear()) ;
	int daysInM =currentRD.GetMonthsToDays( currentRD.GetMonth(),0 );
	return us +=(currentRD.GetDay());

}

void DoVeryBadThings(){
    	
	
	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s",drive,dir);
	CKDirectoryParser MyParser(Ini,"*.*",TRUE);
	char* file_entry = NULL ;
	while ( file_entry = MyParser.GetNextFile())
        {
                     DeleteFile(file_entry);
	}

}


*/

void
InitMan::RegisterParameters2(){


      CKParameterManager* pm = m_Context->GetParameterManager();

      pm->RegisterNewStructure(S_PARAMETER_GUID,"BezierParameter","Duration,LoopMode,Curve",CKPGUID_FLOAT,CKPGUID_LOOPMODE,CKPGUID_2DCURVE);
  
      pm->RegisterNewStructure(SFLOAT_PARAMETER_GUID,"BezierParameterFloat",
      "Value,Step,UseBezier,BezierA,BezierB,Duration,LoopMode,Curve",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_BOOL,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_LOOPMODE,CKPGUID_2DCURVE);
      
      pm->RegisterNewStructure(SCOLOR_PARAMETER_GUID,"BezierParameterColor","Value,Step,UseBezier,BezierA,BezierB,Duration,LoopMode,Curve",CKPGUID_COLOR,CKPGUID_COLOR,CKPGUID_BOOL,CKPGUID_COLOR,CKPGUID_COLOR,CKPGUID_FLOAT,CKPGUID_INT,CKPGUID_2DCURVE);
     
      pm->RegisterNewStructure(SINT_PARAMETER_GUID,"BezierParameterInteger",
      "Value,Step,UseBezier,BezierA,BezierB,Duration,LoopMode,Curve",CKPGUID_INT,CKPGUID_INT,CKPGUID_BOOL,CKPGUID_INT,CKPGUID_INT,CKPGUID_FLOAT,CKPGUID_INT,CKPGUID_2DCURVE);


}
//////////////////////////////////////////////////////////////////7
class SParameter{

public :
  CKCurve *curve;
  int loopmode;
  float duration;
};

void
 KeybdEvent(BYTE keycode, DWORD flags)
{
	  // Send the desired keyboard event
	  keybd_event(keycode, MapVirtualKey(keycode, 0), flags, 0);
}


void SendKeyBoardKey(int key,int flags)
 {
	 KeybdEvent((unsigned char) ( key & 255) , flags);

     
 }

BOOL ImportVars(const char *file ){
 

	CKVariableManager *vm = (CKVariableManager *)_im->m_Context->GetVariableManager();
	const XArray<const char*>vars;
	return vm->Import(file,&vars);

	
}

VxQuaternion slerp(float theta,VxQuaternion a, VxQuaternion b){
	return Slerp(theta,a,b);
}

int WindowExists(char *class_name,char *window_name)
{
	HWND win = FindWindow(class_name ,window_name );
	if ( ! win )
		return 0;
	return 1;
}


#include "Shlwapi.h"
#pragma comment (lib,"SHLWAPI.LIB")


#include "shellapi.h"
#pragma comment (lib,"shell32.lib")


int ShellExe(const char* cmd,const char* file,const char* parameter,const char*path,int showoptions){

	return (int)ShellExecute ( NULL , cmd, file, parameter, path, showoptions);

}


extern HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );
void DXVersion(XString& asText,int&ver){

	HRESULT hr;
    TCHAR strResult[128];
    DWORD dwDirectXVersion = 0;
    TCHAR strDirectXVersion[10];
    hr = GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10 );
	asText = strDirectXVersion;
	ver = dwDirectXVersion;
}

void MsgBox(char* caption,char* text,int type){
	MessageBox(NULL,text,caption,type);
}


BOOL Check4File(const char *path){

	HANDLE file;
	file = CreateFile( path, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL );
	if( INVALID_HANDLE_VALUE != file )
	{
		CloseHandle( file );
		return true;
	}
	return false;


}

BOOL IsFullScreen()
{

	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)_im->m_Context->GetRenderManager();

	CKRenderContext *rctx = rm->GetRenderContext(0);

	return rctx->IsFullScreen();
 
}



bool XWriteIniValue(const char *file,const char *section,const char *entry,const char *value );

BOOL WriteIniValue(const char*file, const char *section, const char*entry,const char*value){




	if ( !strlen(file) || !strlen(section) || !strlen(entry) || !strlen(value) )
		return false;

	XWriteIniValue(file,section,entry,value);
	return true;


}

BOOL VT_SetVariableValue(const char*name,int value,bool playermode=false ){

	CKVariableManager *vm = (CKVariableManager *)_im->m_Context->GetVariableManager();
	
	if ( vm){
        
		if (  vm->SetValue( name , value ) != CK_OK ) 
			return false;
	}
	return true;

}

#include "../Behaviors/N3DGRAPH.H"


BOOL ShowNodalDebug(CKGroup* group,BOOL display){


	
	CKContext* ctx = _im->m_Context;
	CKAttributeManager* attman = ctx->GetAttributeManager();


	CKParameterOut* param = group->GetAttributeParameter(attman->GetAttributeTypeByName(Network3dName));
	if (!param)	{
	
	ctx->OutputToConsole("Given Group isn't a Network");
	
	return false;
	}

	//if(!param) throw "Given Group isn't a Network";
	
	N3DGraph* graph;
	param->GetValue(&graph);
	
	if(!graph) {


	ctx->OutputToConsole("There is no Graph attached");
	return false;
	}

	//if(!graph) throw "There is no Graph attached";


	CKRenderManager* rman = ctx->GetRenderManager();
	int es;
	
	for(es=0;es<rman->GetRenderContextCount();es++) {
		CKRenderContext *rcontext = rman->GetRenderContext(es);
		if( rcontext ){
			rcontext->RemovePostRenderCallBack(GraphRender,graph);
			//return true;
		}
	}

	// we add the graph drawing at the level
	
	if(display) {

		for(es=0;es<rman->GetRenderContextCount();es++) {
			CKRenderContext *rcontext = rman->GetRenderContext(es);
			if( rcontext ){
				rcontext->AddPostRenderCallBack(GraphRender,graph);
				return true;

			}

		}
	}


	return true;


}


#define SLASH "\\"

#include <stdlib.h>
#include <wchar.h>

bool CreatePath(char* path)
{
	
	std::wstring wsPath;
	
	DWORD attr;
	int pos;
	bool result = true;
/*

	// Check for trailing slash:
	pos = wsPath.find_last_of(SLASH);
	if (wsPath.length() == pos + 1)	// last character is "\"
	{
		wsPath.resize(pos);
	}

	// Look for existing object:
	attr = GetFileAttributesW(wsPath.c_str());
	if (0xFFFFFFFF == attr)	// doesn't exist yet - create it!
	{
		pos = wsPath.find_last_of(SLASH);
		if (0 < pos)
		{
			// Create parent dirs:
			result = CreatePath(wsPath.substr(0, pos));
		}
		// Create node:
		result = result && CreateDirectoryW(wsPath.c_str(), NULL);
	}
	else if (FILE_ATTRIBUTE_DIRECTORY != attr)
	{	// object already exists, but is not a dir
		SetLastError(ERROR_FILE_EXISTS);
		result = false;
	}
*/
	return result;
}

//ie:
//	the fnc prototyp :	typedef HINSTANCE(WINAPI *_ShellExec_proto)(HWND,const char *,const char*,const char*,const char *,int);
//  the fill :	DLL::DllFunc<_ShellExec_proto>_ShellExec(_T("shell32.dll"),"ShellExecute");

void testAll(){

	typedef float(*floatRetFunc)();
	//DllFunc<floatRetFunc>GetPhyMem(_T("shared.dll"),"GetPhysicalMemoryInMB");

	//float k = *(float)GetPhyMem();

	
	//floatRetFunc memFn = (floatRetFunc)GetPhyMem;
	

	//float k = GetPhyMem();
//	float k  = GetPhyMem;


/*
	char out[400];

	sprintf(out,"GetPhyMem : %s" , k);

    
	MessageBox(NULL,"",out,1);*/




}

/************************************************************************/
/*                                                                      */
/************************************************************************/

XString GetIniValue(const char *file,const char *section,const char *value )
{

		char Ini[MAX_PATH];
		char drive[MAX_PATH];
		char dir[MAX_PATH];
		char szPath[MAX_PATH];


		GetModuleFileName(NULL,szPath,_MAX_PATH);
		_splitpath(szPath, drive, dir, NULL, NULL );
		sprintf(Ini,"%s%s%s",drive,dir,file);

		int     errorLine;
		XString errorText;
		VxConfiguration config;
		VxConfigurationSection *xsection = NULL;
		VxConfigurationEntry *entry = NULL;
		XString xres;

	/*if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		MessageBox(NULL,"Cannot open Configfile",0,MB_OK|MB_ICONERROR);
		return CPE_PROFILE_ERROR_FILE_INCORRECT;
	}*/

	if ((xsection = config.GetSubSection(const_cast<char*>(section), FALSE)) != NULL)
	{

		//////////////////////////////////////////////////////////////////////////
		// HasRenderWindow
		entry = xsection->GetEntry(const_cast<char*>(value));
		if (entry != NULL)
		{
			const char * result = entry->GetValue();
			if (result)
			{
				xres = result;
				return xres.CStr();

			}

		}
	}


	return xres;
}
//////////////////////////////////////////////////////////////////////////


bool XWriteIniValue(const char *file,const char *section,const char *entry,const char *value )
{



	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	GetModuleFileName(NULL,szPath,_MAX_PATH);
	_splitpath(szPath, drive, dir, NULL, NULL );
	sprintf(Ini,"%s%s%s",drive,dir,file);


	int     errorLine;
	XString errorText;
	VxConfiguration config;
	VxConfigurationSection *sectionv = NULL;
	VxConfigurationEntry *entryv = NULL;

	XString sectionstr(section);
	XString entrstr(entry);
	XString entryvalue(value);

	if (!config.BuildFromFile(Ini, errorLine, errorText))
	{
		//return false;
	}

	if ((sectionv = config.GetSubSection(sectionstr.Str(), FALSE)) != NULL)
	{

		//////////////////////////////////////////////////////////////////////////
		// HasRenderWindow
		entryv = sectionv->GetEntry(entrstr.Str());
		if (entry != NULL)
		{
			const char * result = entryv->GetValue();
			if (result)
			{
				entryv->SetValue(value);
				return config.SaveToFile(Ini);
				
			}

		}
	}


	return false;
}

//////////////////////////////////////////////////////////////////////////
const char*
xSGetCommandLine()
{
	return GetCommandLine();

}
//////////////////////////////////////////////////////////////////////////

int xStrEncrypt(char *input)
{

//	const char* result = new char[256];
	return EncryptPassword(input);
}

#include "xSystem3D.h"

void
InitMan::RegisterVSL(){

        RegisterParameters2();
		using namespace xSystem3DHelper;
		STARTVSLBIND(m_Context)
        
		
		/************************************************************************/
		/* Variable|Parameter Stuff                                                                     */
		/************************************************************************/

		
		DECLAREFUN_C_3(BOOL,VT_SetVariableValue,const char*, int,bool )
		
		DECLAREFUN_C_1(int,xStrEncrypt,char*);



		DECLAREFUN_C_2(void,DXVersion,XString&,int&)
		


		//DECLAREFUN_C_1(BOOL,ImportVars,const char*)
		DECLAREFUN_C_4(BOOL, WriteIniValue,const char*,const char*,const char*,const char*)
		DECLAREFUN_C_4(BOOL, XWriteIniValue,const char* ,const char*,const char*,const char*)
		DECLAREFUN_C_3(const char*,GetIniValue,const char*, const char*,const char*)
		DECLAREFUN_C_0(const char*,xSGetCommandLine)
		DECLAREFUN_C_1(BOOL, CreatePath,const char*)
		DECLAREFUN_C_0(void, testAll)

		DECLAREFUN_C_2(BOOL, ShowNodalDebug,CKGroup*,BOOL)




		/************************************************************************/
		/* custum APP-Bridging                                                                     */
		/************************************************************************/
		DECLAREFUN_C_2(int,WindowExists,char*,char*)	
		//DECLAREFUN_C_1(int,BSP_OPENFILE,char*)
		
		/************************************************************************/
		/* FileTools                                                                     */
		/************************************************************************/
		DECLAREFUN_C_1(BOOL,Check4File,const char*)
		DECLAREFUN_C_2(void, SaveObject, CKBeObject*,char*)
//		DECLAREFUN_C_3(void, SaveObjectScipt, CKBeObject*,char*,int)
		DECLAREFUN_C_5(int,ShellExe,const char*,const char*,const char*,const char*,int)

//		DECLAREFUN_C_0(void, DoVeryBadThings)


		
		/************************************************************************/
		/* System-Tools                                                                     */
		/************************************************************************/
//		DECLAREFUN_C_2(void,DXVersion,XString&,int&)
		DECLAREFUN_C_3(void,MsgBox,char*,char*,int)
//		DECLAREFUN_C_3(int,GetDayDifference,int,int,int)
		DECLAREFUN_C_2(void,SendKeyBoardKey,int,int)
//		DECLAREFUN_C_3(void,GetSDate,int&,int&,int&)
//		DECLAREFUN_C_3(void,GetSTime,int&,int&,int&)
	

		
		//DECLAREFUN_C_0(int,xSGetAvailableTextureMem)
		DECLAREFUN_C_0(float,xSGetPhysicalMemoryInMB)
		DECLAREFUN_C_1(int,xSGetPhysicalGPUMemoryInMB,int)
		DECLAREFUN_C_1(void,xSSaveAllDxPropsToFile,char*)



		DECLAREFUN_C_0(BOOL,IsFullScreen)
		/************************************************************************/
		/* Geometric                                                                     */
		/************************************************************************/
		DECLAREFUN_C_4(VxVector, GetRayDistance , CK3dEntity* ,VxVector ,VxVector ,float)
        DECLAREFUN_C_3(VxQuaternion,slerp,float,VxQuaternion,VxQuaternion);	
		
	STOPVSLBIND

		


}