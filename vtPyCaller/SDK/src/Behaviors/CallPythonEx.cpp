#include "StdAfx.h"
#include "virtools/vtcxglobal.h"
#include "vt_python_funcs.h"
#include <iostream>
#include "pyembed.h"
#include "InitMan.h"
using std::cout;
#include <sstream>

#include "vtTools.h"

using namespace vtTools;

CKObjectDeclaration	*FillBehaviorCallPythonFuncDecl2();
CKERROR CreateCallPythonProto2(CKBehaviorPrototype **);
int CallPythonFunc2(const CKBehaviorContext& behcontext);
CKERROR CallPythonFuncCB2(const CKBehaviorContext& behcontext);

#define BEH_IN_INDEX_MIN_COUNT 3
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1



CKObjectDeclaration	*FillBehaviorCallPythonFuncDecl2()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("CallPythonFuncEx");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x67251d33,0x402868fb));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateCallPythonProto2);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Python");
	return od;
}


CKERROR CreateCallPythonProto2(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("CallPythonFuncEx");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("File",CKPGUID_STRING);
	proto->DeclareInParameter("Func",CKPGUID_STRING);
	proto->DeclareInParameter("Reload",CKPGUID_BOOL);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS|CKBEHAVIOR_VARIABLEPARAMETERINPUTS|CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS|CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS|CKBEHAVIOR_VARIABLEOUTPUTS|CKBEHAVIOR_VARIABLEINPUTS));
	proto->SetFunction( CallPythonFunc2 );
	proto->SetBehaviorCallbackFct(CallPythonFuncCB2);
	
	*pproto = proto;
	return CK_OK;
	
}


int CallPythonFunc2(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;


	XString File((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	XString Func((CKSTRING) beh->GetInputParameterReadDataPtr(1));
	int reload=false;  //= BehaviorTools::GetInputParameterValue<bool>(beh,2);
	
	beh->GetInputParameterValue(2,&reload);

	vt_python_man *pm = (vt_python_man*)ctx->GetManagerByGuid(INIT_MAN_GUID);
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	Python *py = pm->py;
    
	if (!pm->pLoaded)
	{
		pm->m_Context->OutputToConsoleEx("You must load Python before !");
		beh->ActivateOutput(1,false);
		return CKBR_BEHAVIORERROR;

	}


	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(0) )
	{
		try 
		{
			PyObject *module = pm->InsertPModule(beh->GetID(),File,reload);
			PyObject* val = PyInt_FromLong(beh->GetID());
			PyObject_SetAttrString( module , "bid", val);
            pm->CallPyModule(beh->GetID(),Func);
		
		}
		catch (Python_exception ex)
		{
			pm->m_Context->OutputToConsoleEx("PyErr : \t %s",(CKSTRING)ex.what());
			std::cout << ex.what() << "pyexeption in beh : " << beh->GetName();
			PyErr_Clear();
			beh->ActivateOutput(1,false);
		}
		beh->ActivateInput(0,false);

	}
	//////////////////////////////////////////////////////////////////////////
	else
	{
		for (int i  = 1 ; i < beh->GetOutputCount() ; i++ )
		{
			
			try 
			{
				PyObject *module = pm->GetPModule(beh->GetID());
				if(module)
					pm->CallPyModule(beh->GetID(),Func);
			}
			catch (Python_exception ex)
			{
				pm->m_Context->OutputToConsoleEx("PyErr : \t %s",(CKSTRING)ex.what());
				std::cout << ex.what() << "pyexeption in beh : " << beh->GetName();
				beh->ActivateOutput(1,TRUE);
				return CKBR_BEHAVIORERROR;
			}
			beh->ActivateInput(i,false);
		}

	}
	
	return CKBR_OK;

}
//////////////////////////////////////////////////////////////////////////
CKERROR CallPythonFuncCB2(const CKBehaviorContext& behcontext)
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
			
			XString name("PyFuncX: ");
			name << ((CKSTRING) beh->GetInputParameterReadDataPtr(1));
			if ( strlen(((CKSTRING) beh->GetInputParameterReadDataPtr(1))) ==0)
			{
				XString name("PyFileX: ");
				name << ((CKSTRING) beh->GetInputParameterReadDataPtr(0));
				beh->SetName(name.Str());
				break;
			}
			beh->SetName(name.Str());
			break;
		}
		case CKM_BEHAVIORATTACH:
		case CKM_BEHAVIORLOAD : 
		{
			XString name("PyFuncX: ");
			name << ((CKSTRING) beh->GetInputParameterReadDataPtr(1));
			beh->SetName(name.Str());
			if ( strlen(((CKSTRING) beh->GetInputParameterReadDataPtr(1))) ==0)
			{
				XString name("PyFileX: ");
				name << ((CKSTRING) beh->GetInputParameterReadDataPtr(0));
				beh->SetName(name.Str());
				break;
			}
		}
	}
	return CKBR_OK;
}