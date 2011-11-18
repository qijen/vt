/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            SetNodalDifficult
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKAll.h"
#include "N3dGraph.h"

CKObjectDeclaration	*FillBehaviorSetNodalDifficultDecl();
CKERROR CreateSetNodalDifficultProto(CKBehaviorPrototype **);
int SetNodalDifficult(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorSetNodalDifficultDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("SetNodalDifficult");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x77a61036,0x47496bde));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSetNodalDifficultProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("3D Transformations/Nodal Path");
	return od;
}


CKERROR CreateSetNodalDifficultProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("SetNodalDifficult");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Nodal Path",CKPGUID_GROUP);
	proto->DeclareInParameter("Node",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Difficult",CKPGUID_FLOAT);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( SetNodalDifficult );
	
	*pproto = proto;
	return CK_OK;
	
}


int SetNodalDifficult(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	CKAttributeManager* attman = ctx->GetAttributeManager();

	beh->ActivateInput(0,FALSE);
	beh->ActivateOutput(0);

	CKGroup* group = (CKGroup*)beh->GetInputParameterObject(0);			
	CKParameterOut* param = group->GetAttributeParameter(attman->GetAttributeTypeByName(Network3dName));
	if(!param) throw "Given Group isn't a Network";
	N3DGraph* graph;
	param->GetValue(&graph);

	CK3dEntity* s = (CK3dEntity*)beh->GetInputParameterObject(1);
	
	float b;
	beh->GetInputParameterValue(2,&b);

	graph->SetDifficult(s,b);
	beh->ActivateOutput(0);

	return CKBR_OK;
}
