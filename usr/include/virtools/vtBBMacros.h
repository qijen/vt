#ifndef __VT_BB_MACROS_H__
#define __VT_BB_MACROS_H__

//#ifndef __VT_TOOLS_H__
//	#include <virtools/vt_tools.h>
//#endif

#ifndef __VT_BB_HELPER_H__
	#include <virtools/vtBBHelper.h>
#endif

//----------------------------------------------------------------
//
//	The directive _DOC_ONLY_ is just for forcing in/out- parameters creation in dynamic building blocks. 
//	This allows imagegen.exe to create the right pictures
//
//#define _DOC_ONLY_ 0


#ifdef _DOC_ONLY_
	#define BB_SPIN(I,G,N,D) BBParameter(I,G,N,D,1)
#else
	#define BB_SPIN(I,G,N,D) BBParameter(I,G,N,D)
#endif

#define BB_PIN(I,G,N,D) BBParameter(I,TRUE,G,N,D,1)

#define BB_SPIN_ON(I,G,N,D) BBParameter(I,G,N,D,1)


#define BB_DECLARE_PMAP	BBParArray *pMap  = static_cast<BBParArray *>(beh->GetAppData())


#define BB_CREATE_PMAP BBParArray *pMap  = static_cast<BBParArray *>(beh->GetAppData());\
	if(!pMap) pMap  = new BBParArray(); \
	beh->SetAppData(pMap)


#define BB_DELETE_OLD_MAP if (beh->GetAppData()!=NULL){\
	pMap = NULL;\
	beh->SetAppData(NULL);\
}\

#define BB_CKECK_MAP if (pMap==NULL)\
	{\
	pMap  = new BBParArray();\
	}\




#define BB_CLEANUP BB_DELETE_OLD_MAP;\
	BB_CKECK_MAP


#define BB_CREATE_PIMAP BBParArray *pIMap  = new BBParArray(); \
	beh->SetAppData(pIMap)

#define BB_DECLARE_PIMAP	BBParArray *pIMap  = static_cast<BBParArray *>(beh->GetAppData())

#define  BB_PMAP_SIZE(SOURCE_MAP)	(sizeof(SOURCE_MAP) / sizeof(SOURCE_MAP[0]))








#define BB_DESTROY_PIMAP BB_DECLARE_PIMAP;	\
	BBHelper::destroyPIMap(beh,pIMap);	\

#define BB_INIT_PIMAP(SOURCE_MAP,SETTING_START_INDEX)	BB_CREATE_PMAP; \
		BBHelper::initPIMap(beh,pMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX);
	

#define BB_REMAP_PIMAP(SOURCE_MAP,SETTING_START_INDEX) BB_DECLARE_PIMAP;\
	BBHelper::remapArray(beh,pIMap,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX)


/************************************************************************/
/*                                                                      */
/************************************************************************/
#define BB_EVALUATE_SETTINGS(A) for (int i = 0 ; i < BB_PMAP_SIZE(A) ; i ++)	{	\
	if(!A[i].fixed)proto->DeclareSetting(A[i].name.Str(),CKPGUID_BOOL, A[i].condition==-1  ? "FALSE" : "TRUE" ); } \

#define BB_EVALUATE_PINS(A) for (int i = 0 ; i < BB_PMAP_SIZE(A) ; i ++)	{	\
	if(A[i].fixed)proto->DeclareInParameter(A[i].name.Str(),A[i].guid,A[i].defaultValue.Str()); } \

#define BB_EVALUATE_OUTPUTS(A) for (int i = 0 ; i < BB_PMAP_SIZE(A) ; i ++)	{	\
	proto->DeclareOutParameter(A[i].name.Str(),A[i].guid,A[i].defaultValue.Str()); } \

#define BB_EVALUATE_INPUTS(A) for (int i = 0 ; i < BB_PMAP_SIZE(A) ; i ++)	{	\
	proto->DeclareInParameter(A[i].name.Str(),A[i].guid,A[i].defaultValue.Str()); } \

//################################################################
//
// 
//
#define BB_LOAD_PIMAP(SOURCE_MAP,SETTING_START_INDEX) BB_CLEANUP;\
	BBHelper::loadPIMap(beh,pMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX); \
	beh->SetAppData(pMap)


#define BB_LOAD_POMAP(SOURCE_MAP,SETTING_START_INDEX) BB_CLEANUP;\
	BBPOHelper::loadPOMap(beh,pMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX);\
	beh->SetAppData(pMap)



#define BB_IP_INDEX(A)	BBHelper::getIndex(beh,pIMap,A)

#define BBSParameter(A) int s##A;\
	beh->GetLocalParameterValue(A,&s##A)

#define BBSParameterM(A,B) int s##A;\
	beh->GetLocalParameterValue(A-B,&s##A)


/************************************************************************/
/*                                                                      */
/************************************************************************/
#define BB_SPOUT(I,G,N,D) BBParameter(I,G,N,D)
#define BB_SPOUT_ON(I,G,N,D) BBParameter(I,G,N,D,1)


#define BB_INIT_PMAP(SOURCE_MAP,SETTING_START_INDEX)	BB_CREATE_PMAP;\
	BBPOHelper::initPMap(beh,pMap,SOURCE_MAP,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX)

#define BB_REMAP_PMAP(SOURCE_MAP,SETTING_START_INDEX) BB_DECLARE_PMAP;\
	BBPOHelper::remapArray(beh,pMap,BB_PMAP_SIZE(SOURCE_MAP),SETTING_START_INDEX)

#define BB_DESTROY_PMAP BB_DECLARE_PMAP;	\
	BBPOHelper::destroyPMap(beh,pMap);	\

#define BB_OP_INDEX(A)	BBPOHelper::getIndex(beh,pMap,A)


#define BB_O_SET_VALUE_IF(T,I,V)		if (s##I)\
	SetOutputParameterValue<T>(beh,BB_OP_INDEX(I),V)


#endif