/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            AddNodalLink
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "StdAfx.h"
#include "virtools/vtcxglobal.h"
#include "windows.h"
#include "Python.h"
#include "vt_python_funcs.h"
#include "pyembed.h"
#include "InitMan.h"




CKObjectDeclaration	*FillBehaviorLoadPythonDecl();
CKERROR CreateLoadPythonProto(CKBehaviorPrototype **);
int LoadPython(const CKBehaviorContext& behcontext);
CKERROR LoadPythonCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorLoadPythonDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("LoadPython");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x422f5f39,0x71d0452b));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateLoadPythonProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Python");
	return od;
}


CKERROR CreateLoadPythonProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("LoadPython");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("append libpath",CKPGUID_STRING);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( LoadPython );
	
	*pproto = proto;
	return CK_OK;
	
}


int LoadPython(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	beh->ActivateInput(0,FALSE);

	XString path((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	vt_python_man *pm = (vt_python_man*)ctx->GetManagerByGuid(INIT_MAN_GUID);

	if (PythonLoad())
	{
		syspath_append(path.Str());
		beh->ActivateOutput(0);
	}else
	{
		beh->ActivateOutput(1);
	}
	return CKBR_OK;
}
