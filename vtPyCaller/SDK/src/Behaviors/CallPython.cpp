/********************************************************************
	created:	2006/11/17
	created:	17:11:2006   7:11
	filename: 	f:\ProjectRoot\current\vt_plugins\vt_PyCaller\behaviors\CallPython.cpp
	file path:	f:\ProjectRoot\current\vt_plugins\vt_PyCaller\behaviors
	file base:	CallPython
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/

#include "vt_python_funcs.h"
//#include "vt_python_funcs.h"
#include <iostream>
#include "pyembed.h"
#include "InitMan.h"
using std::cout;
#include <sstream>

#include "vtTools.h"

using namespace vtTools;

CKObjectDeclaration	*FillBehaviorCallPythonFuncDecl();
CKERROR CreateCallPythonProto(CKBehaviorPrototype **);
int CallPythonFunc(const CKBehaviorContext& behcontext);
CKERROR CallPythonFuncCB(const CKBehaviorContext& behcontext);

#define BEH_IN_INDEX_MIN_COUNT 3
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1



CKObjectDeclaration	*FillBehaviorCallPythonFuncDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("CallPythonFunc");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x75251b1b,0x382129b8));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateCallPythonProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Python");
	return od;
}


CKERROR CreateCallPythonProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("CallPythonFunc");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("File",CKPGUID_STRING);
	proto->DeclareInParameter("Func",CKPGUID_STRING);
	proto->DeclareInParameter("Reload",CKPGUID_BOOL);
	//proto->DeclareOutParameter("return",CKPGUID_STRING);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS|CKBEHAVIOR_VARIABLEPARAMETERINPUTS|CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS|CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS|CKBEHAVIOR_VARIABLEOUTPUTS|CKBEHAVIOR_VARIABLEINPUTS));
	//proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS|CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	proto->SetFunction( CallPythonFunc );
	proto->SetBehaviorCallbackFct(CallPythonFuncCB);
	
	*pproto = proto;
	return CK_OK;
	
}


int CallPythonFunc(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	//beh->ActivateInput(0,FALSE);

	XString File((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	XString Func((CKSTRING) beh->GetInputParameterReadDataPtr(1));
	int reload=false;  //= BehaviorTools::GetInputParameterValue<bool>(beh,2);
	
	beh->GetInputParameterValue(2,&reload);

	vt_python_man *pm = (vt_python_man*)ctx->GetManagerByGuid(INIT_MAN_GUID);
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	
	//////////////////////////////////////////////////////////////////////////
	//python env test : 
	if (!pm->pLoaded)
	{
		pm->m_Context->OutputToConsoleEx("You must load Python before !");
		beh->ActivateOutput(1,false);
		return CKBR_BEHAVIORERROR;

	}
	
	//////////////////////////////////////////////////////////////////////////
	//creating args : 
	Arg_mmap argsout; 
	using namespace vtTools::Enums;
	for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
	{
	
		CKParameterIn *ciIn = beh->GetInputParameter(i);
		CKParameterType pType  = ciIn->GetType();
		SuperType sType = ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
		const char *pname  = pam->ParameterTypeToName(pType);


		switch (sType)
		{
			case vtSTRING:
				{
					argsout.insert(argsout.end(),std::make_pair(ParameterTools::GetParameterAsString(ciIn->GetRealSource()),Py_string));
					break;
				}
			case vtFLOAT:
				{
					argsout.insert(argsout.end(),std::make_pair(ParameterTools::GetParameterAsString(ciIn->GetRealSource()),Py_real));					
					break;
				}
			case vtINTEGER:
				{
					argsout.insert(argsout.end(),std::make_pair(ParameterTools::GetParameterAsString(ciIn->GetRealSource()),Py_long));					
					break;
				}
			
			default :
				XString err("wrong input parameter type: ");
				err << ciIn->GetName() << "Only Types derivated from Interger,Float or String are acceptable";
				ctx->OutputToConsole(err.Str(),FALSE );
				return CKBR_BEHAVIORERROR;
		}
	}


		
	
	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(0) )
	{
		try // always check for Python_exceptions
		{

			Python *py = pm->py;
			if (Func.Length()==0)
			{
				py->run_file(File.CStr());
				beh->ActivateOutput(0,TRUE);
				return CKBR_OK;

			}

			PyObject *module = pm->InsertPModule(beh->GetID(),File,reload);
			PyObject* val = PyInt_FromLong(beh->GetID());
			//pm->CurrentId(beh->GetID());
			PyObject_SetAttrString( module , "bid", val);

			int bOutCount = beh->GetOutputParameterCount();
			if (bOutCount)
			{
			
				CKParameterOut *ciIn = beh->GetOutputParameter(0);
				CKParameterType pType  = ciIn->GetType();
				SuperType sType = ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
				const char *pname  = pam->ParameterTypeToName(pType);

				switch (sType)
				{
					case vtSTRING:
					{
						std::string ret;
                        pm->CallPyModule(beh->GetID(),Func,argsout,ret);
						CKParameterOut * pout = beh->GetOutputParameter(0);
						XString retx(ret.c_str());
						pout->SetStringValue(retx.Str());
						break;
					}
					case vtFLOAT:
					{
						double ret=0.0f;
						pm->CallPyModule(beh->GetID(),Func,argsout,ret);
						float retx = (float)ret;
						beh->SetOutputParameterValue(0,&retx);
						break;
					}
					case vtINTEGER:
					{
						long ret=0;
						pm->CallPyModule(beh->GetID(),Func,argsout,ret);
						int retx = (int)ret;
						beh->SetOutputParameterValue(0,&retx);
						break;
					}
					default :
						XString err("wrong output parameter type: ");
						err << ciIn->GetName() << "Only Types derivated from Interger,Float or String are acceptable";
						ctx->OutputToConsole(err.Str(),FALSE );
						return CKBR_BEHAVIORERROR;
					}
				}
				else
				{
					pm->py->call(Func.Str(),argsout);
				}

				beh->ActivateOutput(0,TRUE);
			}
			catch (Python_exception ex)
			{
				pm->m_Context->OutputToConsoleEx("PyErr : \t %s",(CKSTRING)ex.what());
				std::cout << ex.what() << "pyexeption in beh : " << beh->GetName();
				PyErr_Clear();
				beh->ActivateOutput(1,TRUE);
			}
		beh->ActivateInput(0,false);
	}

	return CKBR_OK;
}

//////////////////////////////////////////////////////////////////////////
CKERROR CallPythonFuncCB(const CKBehaviorContext& behcontext)
{
	/************************************************************************/
	/*  collecting data :													*/
	/*  																	*/
	CKBehavior *beh = behcontext.Behavior;
	CKContext* ctx = beh->GetCKContext();
	CKParameterManager *pm = static_cast<CKParameterManager*>(ctx->GetParameterManager());
	/************************************************************************/
	/*	process virtools callbacks		:																			*/
	/*  																														*/

	switch(behcontext.CallbackMessage)
		{


		case CKM_BEHAVIOREDITED:
		case CKM_BEHAVIORSETTINGSEDITED:
		{

			assert(beh && ctx);
			
			int p_count  =  beh->GetOutputParameterCount();

			/*while(   (BEH_OUT_MIN_COUNT )   < p_count )
			{ 
				CKDestroyObject( beh->RemoveOutputParameter( --p_count) );
			}*/
			
			XString name("PyFunc: ");
			name << ((CKSTRING) beh->GetInputParameterReadDataPtr(1));
			if ( strlen(((CKSTRING) beh->GetInputParameterReadDataPtr(1))) ==0)
			{
				XString name("PyFile: ");
				name << ((CKSTRING) beh->GetInputParameterReadDataPtr(0));
				beh->SetName(name.Str());
				break;
			}
			beh->SetName(name.Str());
			break;
		}
		case CKM_BEHAVIORLOAD : 
		{
			XString name("PyFunc: ");
			name << ((CKSTRING) beh->GetInputParameterReadDataPtr(1));
			beh->SetName(name.Str());
			if ( strlen(((CKSTRING) beh->GetInputParameterReadDataPtr(1))) ==0)
			{
				XString name("PyFile: ");
				name << ((CKSTRING) beh->GetInputParameterReadDataPtr(0));
				beh->SetName(name.Str());
				break;
			}
		}
		case CKM_BEHAVIORATTACH:
			{
				/*CKObject *bobj = (CKObject *)beh;
				CK_ID idb =  bobj->GetID();
				ctx->OutputToConsoleEx("beh id1 : %d",idb);*/
				break;

			}

	}
	return CKBR_OK;
}