/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            Bend
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorBendDecl();
CKERROR CreateBendProto(CKBehaviorPrototype **);
int Bend(const CKBehaviorContext& behcontext);
//CKERROR MeshModificationsCallBack(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorBendDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Bend2");	
	od->SetDescription("Bends a mesh uniformly along an axis with an angle and a direction of bending.");
	/* rem:
	<SPAN CLASS=in>In : </SPAN>triggers the process.<BR>
	<SPAN CLASS=out>Exit : </SPAN>is activated when the process is completed.<BR>
	<BR>
	<SPAN CLASS=pin>Angle : </SPAN>angle to bend from the vertical plane.<BR>
	<SPAN CLASS=pin>Direction : </SPAN>direction of the bend relative to the horizontal plane.<BR>
	<SPAN CLASS=pin>Axis : </SPAN>vector that specifies the axis that the object will bend along.<BR>
	<SPAN CLASS=pin>Reset Mesh: </SPAN>if TRUE, the mesh is resetted to its initial conditions at activation.<BR>
	<BR>
	The model need to be somewhat facetted for a good result.<BR>
	*/
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7c9a7008,0x6b0f1d1f));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00020000);
	od->SetCreationFunction(CreateBendProto);
	od->SetCompatibleClassId(CKCID_3DOBJECT);
	od->SetCategory("Mesh Modifications/Deformation");
	return od;
}


CKERROR CreateBendProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("Bend");
	if(!proto) 	return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareOutput("Exit");
	
	proto->DeclareInParameter("Angle", CKPGUID_ANGLE, "0:90" );
	proto->DeclareInParameter("Direction", CKPGUID_FLOAT ,"0.0");
	// To change by Radio buttons
	proto->DeclareInParameter("Axis", CKPGUID_VECTOR, "0,0,1" );
	proto->DeclareInParameter("Reset Mesh", CKPGUID_BOOL, "TRUE" );
	proto->DeclareInParameter("Normal Recalculation", CKPGUID_BOOL, "TRUE" );

	proto->DeclareLocalParameter(NULL, CKPGUID_VOIDBUF );	// Data
	proto->DeclareLocalParameter(NULL, CKPGUID_BOX );		// Initial Box

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(Bend);

//	proto->SetBehaviorCallbackFct(MeshModificationsCallBack);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	*pproto = proto;
	return CK_OK;

}


int Bend(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	
	// we get the  entity
	CK3dEntity *ent = (CK3dEntity *) beh->GetTarget();

	// we get the angle
	float angle;
	beh->GetInputParameterValue(0,&angle);
	angle *= 0.5f;

	// we get the direction
	float direction;
	beh->GetInputParameterValue(1,&direction);

	// we get the axis
	VxVector axis;
	beh->GetInputParameterValue(2,&axis);
	int naxis=0;
	while (axis[naxis]==0.0f) naxis++;

	// we get the mesh
	CKMesh *mesh = ent->GetCurrentMesh();
	CKDWORD vStride=0;
	BYTE* varray = (BYTE*)mesh->GetModifierVertices(&vStride);
	int pointsNumber = mesh->GetModifierVertexCount();
/*
	CKBOOL resetmesh = TRUE;
	beh->GetInputParameterValue(3,&resetmesh);
	if (resetmesh) { // The Mesh must be resetted
		if(beh->GetVersion() < 0x00020000) { // Old Version with vertices stuffed inside
			// we get the saved position
			VxVector* savePos = (VxVector*)beh->GetLocalParameterWriteDataPtr(0);
			
			BYTE* temparray = varray;
			for(int i=0;i<pointsNumber;i++,temparray+=vStride) {
				*(VxVector*)temparray = savePos[i];
			}
		} else { // new version : based on ICs
			CKScene *scn  = behcontext.CurrentScene;
			CKStateChunk *chunk = scn->GetObjectInitialValue(mesh);				
			if(chunk) mesh->LoadVertices(chunk);
			varray			= (BYTE*)mesh->GetModifierVertices(&vStride);
			pointsNumber	= mesh->GetModifierVertexCount();
			const VxBbox& bbox = mesh->GetLocalBox();
			beh->SetLocalParameterValue(1,&bbox);
		}
	}

	VxBbox bbox;
	beh->GetLocalParameterValue(1,&bbox);

	VxMatrix mat;
	switch (naxis) {
	case 0: { VxVector vz(0.0f,1.0f,0.0f); Vx3DMatrixFromRotation(mat,vz,-HALFPI); } break; //x
	case 1:	{ VxVector vx(0.0f,0.0f,1.0f); Vx3DMatrixFromRotation(mat,vx,-HALFPI);  } break;  //y
	case 2: { mat = VxMatrix::Identity(); } break;   //z
	}

	// attention inversion y / z
	if (direction) {
		VxMatrix rot;
		VxVector vz(0.0f,0.0f,1.0f); 
		Vx3DMatrixFromRotation(rot,vz,direction);
		Vx3DMultiplyMatrix(mat,rot,mat);
	}

	float r,len;
	switch (naxis) {
	case 0:  len = bbox.Max.x - bbox.Min.x; break;
	case 1:	 len = bbox.Max.y - bbox.Min.y; break;
	case 2:  len = bbox.Max.z - bbox.Min.z; break;
	}
	
	// Skip the singularity
	if (fabs(angle) <0.0001) {
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
		return CKBR_OK;
	} else {
		r = len/angle;
	}	
	
	
	VxMatrix invmat;
	Vx3DInverseMatrix(invmat,mat);
	
	// we move all the points
	float x,y,c,s,yr;
	VxVector v,n;
	
	float invr = 1.0f/r;

	CKBOOL normalc = TRUE;
	beh->GetInputParameterValue(4,&normalc);

	CKDWORD nStride=0;
	BYTE* narray = (BYTE*)mesh->GetNormalsPtr(&nStride);
	if (CKIsChildClassOf(mesh,CKCID_PATCHMESH) || !normalc) narray = NULL;
	
	for(int i=0;i<pointsNumber;i++,varray+=vStride) {
		Vx3DMultiplyMatrixVector(&v,mat,(VxVector*)varray);
		
		x = v.x;
		y = v.y;
		yr = y*invr;
		c = XCos(PI-yr);
		s = XSin(PI-yr);
		v.x = r*c + r - x*c;
		v.y = r*s - x*s;

		Vx3DMultiplyMatrixVector((VxVector *)varray,invmat,&v);


		// normal calculation
		if (narray) {
			Vx3DRotateVector(&n,mat,(VxVector*)narray);

			n.x += x;
			n.y += y;

			x = n.x;
			y = n.y;
			yr = y*invr;
			c = XCos(PI-yr);
			s = XSin(PI-yr);
			n.x = r*c + r - x*c;
			n.y = r*s - x*s;

			n.x -= v.x;
			n.y -= v.y;

			Vx3DRotateVector((VxVector *)narray,invmat,&n);
			narray += nStride;
		}
	}
	
	// we only rebuild the faces normals
	mesh->ModifierVertexMove(FALSE,TRUE);	
	
	beh->ActivateInput(0,FALSE);
	beh->ActivateOutput(0,TRUE);
	*/
	return CKBR_OK;
}