/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            AddNodalLink
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKAll.h"
#include "N3dGraph.h"

CKObjectDeclaration	*FillBehaviorAddNodalLinkDecl();
CKERROR CreateAddNodalLinkProto(CKBehaviorPrototype **);
int AddNodalLink(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorAddNodalLinkDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Create Nodal Edge");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x47ea2c5c,0x5b6b2b81));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateAddNodalLinkProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("3D Transformations/Nodal Path");
	return od;
}


CKERROR CreateAddNodalLinkProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("Create Nodal Edge");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Nodal Path",CKPGUID_GROUP);
	proto->DeclareInParameter("Start Node",CKPGUID_3DENTITY);
	proto->DeclareInParameter("End Node",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Difficult",CKPGUID_FLOAT);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( AddNodalLink );
	
	*pproto = proto;
	return CK_OK;
	
}


int AddNodalLink(const CKBehaviorContext& behcontext)
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
	CK3dEntity* e = (CK3dEntity*)beh->GetInputParameterObject(2);
	float b;
	beh->GetInputParameterValue(3,&b);
	graph->InsertEdge(s,e,b);
	beh->ActivateOutput(0);





	return CKBR_OK;
}
