#include "StdAfx.h"
#include "virtools/vtcxglobal.h"
#include "windows.h"
#include "Python.h"

#include "vt_python_funcs.h"
#include "pyembed.h"
#include "InitMan.h"



CKObjectDeclaration	*FillBehaviorDestroyPythonDecl();
CKERROR CreateDestroyPythonProto(CKBehaviorPrototype **);
int DestroyPython(const CKBehaviorContext& behcontext);
CKERROR DestroyPythonCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorDestroyPythonDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DestroyPython");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x78634039,0x1d845726));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDestroyPythonProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Python");
	return od;
}


CKERROR CreateDestroyPythonProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("DestroyPython");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( DestroyPython );
	
	*pproto = proto;
	return CK_OK;
	
}


int DestroyPython(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	
	beh->ActivateInput(0,FALSE);
	
	vt_python_man *pm = (vt_python_man*)ctx->GetManagerByGuid(INIT_MAN_GUID);
	DestroyPython();

	beh->ActivateOutput(0);

	return CKBR_OK;
}
