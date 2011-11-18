/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            DeleteNodalLink
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKAll.h"
#include "N3dGraph.h"

CKObjectDeclaration	*FillBehaviorDeleteNodalLinkDecl();
CKERROR CreateDeleteNodalLinkProto(CKBehaviorPrototype **);
int DeleteNodalLink(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorDeleteNodalLinkDecl()
{                          
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DeleteNodalLink");	
	od->SetDescription("");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6ff16e0d,0x34561d5a));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDeleteNodalLinkProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("3D Transformations/Nodal Path");
	return od;
}


CKERROR CreateDeleteNodalLinkProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("DeleteNodalLink");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Nodal Path",CKPGUID_GROUP);
	proto->DeclareInParameter("Start Node",CKPGUID_3DENTITY);
	proto->DeclareInParameter("End Node",CKPGUID_3DENTITY);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction( DeleteNodalLink );
	
	*pproto = proto;
	return CK_OK;
	
}


int DeleteNodalLink(const CKBehaviorContext& behcontext)
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


		graph->DeleteEdge(s,e);
	
		graph->DeleteEdge(e,s);

	beh->ActivateOutput(0);
	return CKBR_OK;
}
