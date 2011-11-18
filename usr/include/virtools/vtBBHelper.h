#ifndef __VT_BB_HELPER_H__
	#define __VT_BB_HELPER_H__

#include <xLogger.h>

namespace vtTools
{

	namespace BehaviorTools
	{

		struct BBParameter
		{
			int ID; 	CKGUID guid;		XString name;	XString defaultValue;	int condition;	int settingsID; int inputIndex;
			CKObject *par;
			bool fixed;
			BBParameter()
			{
				condition = -1; 		ID = 0;		guid = CKGUID(0,0);		name = "";		defaultValue = ""; settingsID = -1;		par = NULL;
				inputIndex= -1;
				fixed = false;

			}

			BBParameter(int _ID,CKGUID _guid,XString _name,XString _defaultValue) : 
			ID(_ID), guid(_guid) , name(_name) , defaultValue(_defaultValue)
			{
				condition = -1;
				settingsID = 0;
				par = NULL;
				inputIndex= -1;
				fixed = false;
			}

			BBParameter(int _ID,bool _fixed,CKGUID _guid,XString _name,XString _defaultValue,int _condition) : 
			ID(_ID), guid(_guid) , name(_name) , defaultValue(_defaultValue)
			{
				condition = _condition;
				settingsID = 0;
				par = NULL;
				inputIndex= -1;
				fixed = _fixed;
			}

			BBParameter(int _ID,CKGUID _guid,XString _name,XString _defaultValue,int _condition) : 
			ID(_ID), guid(_guid) , name(_name) , defaultValue(_defaultValue), condition(_condition)
			{
				settingsID = 0;
				par = NULL;
				inputIndex= -1;
				fixed = false;
			}
		};


		class BBParArray
		{

		public : 
			BBParArray() : isDuplicat(false) , isLoaded(false) , states(0){} 
			std::vector<BBParameter*>pars;

			std::vector<BBParameter*>&getArray()
			{
				return pars;
			}
			bool isDuplicat;
			bool isLoaded;
			int states;
			int bbId;


			enum {
                BBArrayIsLoaded=1,
				BBArrayIsInitiated=2
			};
		};

		class BBPOHelper
		{

		public :

			static int getIndex(CKBehavior *beh,BBParameter *par)
			{
				if (!par)return -1;
				if (par && !par->par )return -1;

				for (int i = 0 ; i< beh->GetOutputParameterCount() ; i++)
				{
					if (par->par == beh->GetOutputParameter(i)) return i;
				}
				return -1;
			}

			static void remapIndex(CKBehavior *beh,BBParArray *pAarray,int size )
			{
				for (int i = 0 ; i < size ; i ++)
				{
					BBParameter *p=pAarray->getArray().at(i);
					p->inputIndex = p->par !=NULL ? getIndex(beh,p) : -1;
				}
			}

			static int getIndex(CKBehavior *beh,BBParArray *pAarray,int indicator )
			{

#ifdef _DEBUG
				assert(beh);
				assert(pAarray);
#endif
				return pAarray->getArray().at(indicator)->inputIndex;

			}

			static int getIndexInv(CKBehavior *beh,BBParameter pInMap[],int indicator )
			{

#ifdef _DEBUG
				assert(beh);
#endif

				for ( int i = 0 ; i < beh->GetOutputParameterCount() ; i++ )
				{
					if (!strcmp(beh->GetOutputParameter(i)->GetName(),pInMap[indicator].name.Str() ))
					{
						return i;
					}
				}
				return -1;
			}

			static void loadPOMap(CKBehavior *beh,BBParArray *dst,BBParameter pInMap[],int size,int start)
			{
				for (int i = 0 ; i < size ; i ++)
				{
					BBParameter *p=new BBParameter(pInMap[i].ID,pInMap[i].guid,pInMap[i].name,pInMap[i].defaultValue,pInMap[i].condition);
					dst->getArray().push_back(p);
					CKParameterLocal *lp = beh->GetLocalParameter(start + i );	
					int v=0 ; 
					lp->GetValue(&p->condition);
					if (p->condition)
					{
						int index = getIndexInv(beh,pInMap,i);
						p->par = ((CKObject*)beh->GetOutputParameter(index));
						p->condition = 1;
						p->inputIndex = index;
					}else
					{
						p->par = NULL;
						p->condition=-1;
						p->inputIndex = -1;
					}

				}

				dst->isLoaded=true;
			}
			
			static void printPOMap(CKBehavior *beh,BBParArray *dst,BBParameter pInMap[],int size,int start)
			{
				XString header;
				if (dst->isLoaded)
				{
					header << "IsLoaded:" << "TRUE";
				}

				header << "|Size:" << dst->getArray().size();
				xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,header.Str());
				for (int i = 0 ; i < size ; i ++)
				{
					BBParameter *par = dst->getArray().at( i );

					XString o;
					o << par->name << " : iIndex" << par->inputIndex << " | cond:" << par->condition;
					if (par->par !=NULL)
					{
                        o << "| Par=1";
					}
						
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,o.Str());

				}
			

			}

			static void initPMap(CKBehavior *beh,BBParArray *dst,BBParameter pInMap[],int size,int start)
			{
				if (dst==NULL)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"No Array supplied");

				}
			
				if (dst->isLoaded)
				{
					return;
				}
				for (int i = 0 ; i < size ; i ++)
				{

					BBParameter *p=new BBParameter(pInMap[i].ID,pInMap[i].guid,pInMap[i].name,pInMap[i].defaultValue,pInMap[i].condition);
					dst->getArray().push_back(p);
					if (pInMap[i].condition ==1 )
					{
						CKParameterLocal *lp = beh->GetLocalParameter(start + i );	lp->SetValue(&pInMap[i].condition);
						p->par = (CKObject*)beh->CreateOutputParameter(pInMap[i].name.Str(),pInMap[i].guid);
						int index = getIndex(beh,p);
					}
				}
				BBPOHelper::remapIndex(beh,dst,size);
			}

			static void remapArray(CKBehavior *beh,BBParArray *pArray,int size,int settingStartIndex)
			{
				for (int sI = settingStartIndex; sI <  size + settingStartIndex; sI ++ )
				{
					CKParameterLocal *lp = beh->GetLocalParameter(sI ); int val = 0;		lp->GetValue(&val);
					BBParameter *par = pArray->getArray().at( sI - settingStartIndex);

					//////////////////////////////////////////////////////////////////////////
					// Settings ON : 
					if (val)
					{
						if ( par->par == NULL )
						{
							par->condition = 1;
							par->par = (CKObject*)beh->CreateOutputParameter(par->name.Str(),par->guid);
						}
					}
					//////////////////////////////////////////////////////////////////////////
					// Settings ON : 
					if (!val)
					{
						if ( par->par )
						{
							CKDestroyObject(par->par);
							par->par = NULL;
							par->condition = -1;
						}
					}
				}
				remapIndex(beh,pArray,size);
			}

			static void destroyPMap(CKBehavior *beh,BBParArray *pArray)
			{
				BBParArray *pMap  = static_cast<BBParArray *>(beh->GetAppData());
				if (pMap)
				{
					while (	pMap->getArray().size() )
					{
						BBParameter *p=pMap->getArray().at(0);
						pMap->getArray().erase(pMap->getArray().begin());
						if (p)
						{
							p->par = NULL;
							delete p;
							p = NULL;
						}
					}
					beh->SetAppData(NULL);
					pMap->getArray().clear();
					delete pMap;
					pMap = NULL;
				}
			}
		};

		class BBHelper
		{

		public :

			

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static int getIndex(CKBehavior *beh,BBParameter *par)
			{
				if (!par)return -1;
				if (par && !par->par )return -1;

				for (int i = 0 ; i< beh->GetInputParameterCount() ; i++)
				{
					if (par->par == beh->GetInputParameter(i)) return i;
				}
				return -1;
			}

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static void remapIndex(CKBehavior *beh,BBParArray *pAarray,int size )
			{
				for (int i = 0 ; i < size ; i ++)
				{
					BBParameter *p=pAarray->getArray().at(i);
					p->inputIndex = p->par !=NULL ? getIndex(beh,p) : -1;
				}
			}

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static int getIndex(CKBehavior *beh,BBParArray *pAarray,int indicator )
			{

				#ifdef _DEBUG
					assert(beh);
					assert(pAarray);
				#endif
				return pAarray->getArray().at(indicator)->inputIndex;

			}

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static int getIndexInv(CKBehavior *beh,BBParameter pInMap[],int indicator )
			{

		#ifdef _DEBUG
				assert(beh);
		#endif

				for ( int i = 0 ; i < beh->GetInputParameterCount() ; i++ )
				{
					if (!strcmp(beh->GetInputParameter(i)->GetName(),pInMap[indicator].name.Str() ))
					{
						return i;
					}
				}
				return -1;
			}

			static void loadPIMap(CKBehavior *beh,BBParArray *dst,BBParameter pInMap[],int size,int start)
			{
				for (int i = 0 ; i < size ; i ++)
				{
					BBParameter *p=new BBParameter(pInMap[i].ID,pInMap[i].fixed,pInMap[i].guid,pInMap[i].name,pInMap[i].defaultValue,pInMap[i].condition);
					dst->getArray().push_back(p);

					if (!p->fixed)
					{
						int d  = i - start ;
						CKParameterLocal *lp = beh->GetLocalParameter( i - start);
						
						lp->GetValue(&p->condition);
					}

					if (p->condition)
					{

						int index = getIndexInv(beh,pInMap,i);
						p->par = ((CKObject*)beh->GetInputParameter(index));
						p->condition = 1;
						p->inputIndex = index;
					}else
					{
						p->par = NULL;
						p->condition=-1;
						p->inputIndex = -1;
					}

				}

				dst->isLoaded = true;
			}
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static void initPIMap(CKBehavior *beh,BBParArray *dst,BBParameter pInMap[],int size,int start)
			{
				if (dst->isLoaded)
				{
					return;
				}

				for (int i = 0 ; i < size ; i ++)
				{

					BBParameter *p=new BBParameter(pInMap[i].ID,pInMap[i].guid,pInMap[i].name,pInMap[i].defaultValue,pInMap[i].condition);
					dst->getArray().push_back(p);
					if (pInMap[i].condition ==1 )
					{
						CKParameterLocal *lp = beh->GetLocalParameter(start + i );	lp->SetValue(&pInMap[i].condition);
						p->par = (CKObject*)beh->CreateInputParameter(pInMap[i].name.Str(),pInMap[i].guid);
						int index = getIndex(beh,p);
					}
				}
				BBHelper::remapIndex(beh,dst,size);
			}

			static void remapArray(CKBehavior *beh,BBParArray *pArray,int size,int settingStartIndex)
			{
				for (int sI = 0; sI <  size ; sI ++ )
				{
					BBParameter *par = pArray->getArray().at(sI);
					if (par->fixed)
						continue;
					
					CKParameterLocal *lp = beh->GetLocalParameter(sI - settingStartIndex ); int val = 0;		lp->GetValue(&val);

					//////////////////////////////////////////////////////////////////////////
					// Settings ON : 
					if (val)
					{
						if ( par->par == NULL )
						{
							par->condition = 1;
							par->par = (CKObject*)beh->CreateInputParameter(par->name.Str(),par->guid);
						}
					}
					//////////////////////////////////////////////////////////////////////////
					// Settings ON : 
					if (!val)
					{
						if ( par->par )
						{
							CKDestroyObject(par->par);
							par->par = NULL;
							par->condition = -1;
						}
					}
				}
				remapIndex(beh,pArray,size);
			}

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			static void destroyPIMap(CKBehavior *beh,BBParArray *pArray)
			{
				BBParArray *pIMap  = static_cast<BBParArray *>(beh->GetAppData());
				if (pIMap)
				{
					while (	pIMap->getArray().size() )
					{
						BBParameter *p=pIMap->getArray().at(0);
						pIMap->getArray().erase(pIMap->getArray().begin());
						if (p)
						{
							p->par = NULL;
							delete p;
							p = NULL;
						}
					}
					beh->SetAppData(NULL);
					pIMap->getArray().clear();
					delete pIMap;
					pIMap = NULL;
				}
			}
		};
	}

}
#endif