#include "ckall.h"

CKERROR CreateMeshAddProto(CKBehaviorPrototype **pproto);
int MeshAdd(const CKBehaviorContext& context);

extern void CleanUp(CKBehavior *beh);
extern void Load(CKBehavior *beh,CKScene *scn);

CKObjectDeclaration *FillMeshAddDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Mesh Addition");	
	od->SetDescription("Duplicates objects");
	od->SetCategory("Mesh Modifications/Multi Mesh");
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xc73102f,0x4f8c574b));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateMeshAddProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


CKERROR CreateMeshAddProto(CKBehaviorPrototype **pproto)
{
	
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Mesh Addition");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Original", CKPGUID_3DENTITY);
	proto->DeclareInParameter("Additional", CKPGUID_3DENTITY);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(MeshAdd);
	
	
	*pproto = proto;
	return CK_OK;

}
int MeshAdd(const CKBehaviorContext& behcontext)
{
	 
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;

	CK3dEntity *oe = (CK3dEntity *)beh->GetInputParameterObject(0);
	CKMesh *om=oe->GetCurrentMesh();

	CK3dEntity *ae = (CK3dEntity *)beh->GetInputParameterObject(1);
	CKMesh *am=ae->GetCurrentMesh();

	VxVector oePos;
	oe->GetPosition(&oePos,NULL);
	
	int oVCount = om->GetVertexCount();
	int aVCount = am->GetVertexCount();

	int oFCount = om->GetFaceCount();
	int aFCount = am->GetFaceCount();


	om->SetVertexCount(oVCount+aVCount);
	om->SetFaceCount(oFCount+aFCount);


	VxVector oVPos,oVNormals;
	float u,v;
	

	VxVector aePos,Delta;
	ae->GetPosition(&aePos,NULL);
	
	Delta = aePos-oePos;

	for (int j = 0; j< aVCount; j++){

		am->GetVertexPosition(j,&oVPos);
		oVPos += Delta;
		om->SetVertexPosition(j+oVCount,&oVPos);

		am->GetVertexNormal(j,&oVNormals);
		om->SetVertexNormal(j+oVCount,&oVNormals);
	
		am->GetVertexTextureCoordinates(j,&u,&v,-1);
		om->SetVertexTextureCoordinates(j+oVCount,u,v,-1);
	}

	for ( int l=0;l< aFCount;l++){
	
		int a,b,c;
		VxVector FN;
		am->GetFaceVertexIndex(l,a,b,c);
		om->SetFaceVertexIndex(l+oFCount,a+oVCount,b+oVCount,c+oVCount);
		CKMaterial *OMAT = am->GetFaceMaterial(l);
		om->SetFaceMaterial(l+oFCount,OMAT);
	}


	om->BuildFaceNormals();
	beh->ActivateOutput(0);
	return CKBR_OK;
}


