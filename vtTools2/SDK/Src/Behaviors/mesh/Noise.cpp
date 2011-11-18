/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            Noise
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKAll.h"


CKObjectDeclaration	*FillBehaviorNoiseDecl();
CKERROR CreateNoiseProto(CKBehaviorPrototype **);
int Noise(const CKBehaviorContext& behcontext);
//CKERROR MeshModificationsCallBack(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorNoiseDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Noise2");	
	od->SetDescription("Displaces the vertices of the mesh with a random vector.");
	/* rem:
	<SPAN CLASS=in>In: </SPAN>triggers the process.<BR>
	<SPAN CLASS=out>Out: </SPAN>is activated when the process is completed.<BR>
	<BR>
	<SPAN CLASS=pin>Seed: </SPAN>generates a random starting point from the number you set.<BR>
	<SPAN CLASS=pin>Scale: </SPAN>size of noise effect (not strength).<BR>
	<SPAN CLASS=pin>Axis: </SPAN>strength of the noise effect along each of three axes.<BR>
	<SPAN CLASS=pin>Reset Mesh: </SPAN>if TRUE, the mesh is reseted to its initial conditions at activation.<BR>
	*/
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6d300730,0x26c83f42));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00020000);
	od->SetCreationFunction(CreateNoiseProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Mesh Modifications/Deformation");
	return od;
}


CKERROR CreateNoiseProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = NULL;
	proto = CreateCKBehaviorPrototype("Noise2");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Seed", CKPGUID_FLOAT,"0.1" );
	proto->DeclareInParameter("Scale", CKPGUID_FLOAT ,"50.0");
	// To change by Radio buttons
	proto->DeclareInParameter("Axis", CKPGUID_VECTOR, "0.1,0,0" );
	proto->DeclareInParameter("Reset Mesh", CKPGUID_BOOL, "TRUE" );


	proto->DeclareInParameter("target", CKPGUID_3DENTITY);


	proto->DeclareLocalParameter("data", CKPGUID_VOIDBUF );

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(Noise);

	//proto->SetBehaviorCallbackFct(MeshModificationsCallBack);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	*pproto = proto;
	return CK_OK;

}

#define B 0x100
#define BM 0xff

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

static int p[B + B + 2];
static float g3[B + B + 2][3];
static float g2[B + B + 2][2];
static float g1[B + B + 2];
static int start = 1;

static void init(void);

#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )

#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
	t = vec[i] + N;\
	b0 = ((int)t) & BM;\
	b1 = (b0+1) & BM;\
	r0 = t - (int)t;\
	r1 = r0 - 1.0f;

float noise3(const VxVector &vect)
{
	const float *vec = vect.v;
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register int i, j;

	if (start) {
		start = 0;
		init();
	}

	setup(0, bx0,bx1, rx0,rx1);
	setup(1, by0,by1, ry0,ry1);
	setup(2, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}

static void normalize2(float v[2])
{
	float s;

	s = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] * s;
	v[1] = v[1] * s;
}

static void normalize3(float v[3])
{
	float s;

	s = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] * s;
	v[1] = v[1] * s;
	v[2] = v[2] * s;
}

static void init(void)
{
	int i, j, k;

	for (i = 0 ; i < B ; i++) {
		p[i] = i;

		g1[i] = (float)((rand() % (B + B)) - B) / B;

		for (j = 0 ; j < 2 ; j++)
			g2[i][j] = (float)((rand() % (B + B)) - B) / B;
		normalize2(g2[i]);

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (float)((rand() % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++) {
		p[B + i] = p[i];
		g1[B + i] = g1[i];
		for (j = 0 ; j < 2 ; j++)
			g2[B + i][j] = g2[i][j];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}

int Noise(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	
	// we get the 3d entity
	CK3dEntity *ent = (CK3dEntity *) beh->GetInputParameterObject(4);
	if(!ent) return 0;
	
	// we get the seed value
	float seed=0.1f;
	CKParameterIn *pin = beh->GetInputParameter(0);
	if( pin->GetGUID() == CKPGUID_INT ){ // back compatibility
		int seed_int = 0;
		pin->GetValue( &seed_int );
		seed = (float)seed_int;
	}	else {
		beh->GetInputParameterValue(0,&seed);
	}
	seed *= 0.01f;
	
	// we get the scale value
	float scale;
	beh->GetInputParameterValue(1,&scale);
	
	// we get the amount value
	VxVector axis;
	beh->GetInputParameterValue(2,&axis);
	
	// we get the mesh
	CKMesh *mesh = ent->GetCurrentMesh();
	CKDWORD vStride=0;
	BYTE* varray = (BYTE*)mesh->GetModifierVertices(&vStride);
	int pointsNumber = mesh->GetModifierVertexCount();

	CKBOOL resetmesh = TRUE;
	beh->GetInputParameterValue(3,&resetmesh);
	if (resetmesh) { // The Mesh must be reseted
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
		}
	}

	// we move all the points
	VxVector v,sp,d(0,0,0);
	VxVector offset(0.5f,0.5f,0.5f);
	VxVector noise_vect(0,0,seed);
	for(int i=0;i<pointsNumber;i++,varray+=vStride) {
		v = *(VxVector*)varray;
		sp = v * scale + offset;
		
		noise_vect.x = sp.y;
		noise_vect.y = sp.z;
		d.x = noise3(noise_vect);
		noise_vect.x = sp.x;
		d.y = noise3(noise_vect);
		noise_vect.y = sp.y;
		d.z = noise3(noise_vect);

		*(VxVector*)varray = (v+(d*axis));
	}
	
	mesh->ModifierVertexMove(TRUE,TRUE);

	beh->ActivateInput(0,FALSE);
	beh->ActivateOutput(0);
	
	return CKBR_OK;
}

