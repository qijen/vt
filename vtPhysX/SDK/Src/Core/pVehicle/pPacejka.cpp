#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>
#include "pVehicleAll.h"


pPacejka::pPacejka()
{
  // Initial vars
  a0=a1=a2=a3=a4=a5=a6=a7=a8=a9=a10=a111=a112=a12=a13=0;
  b0=b1=b2=b3=b4=b5=b6=b7=b8=b9=b10=0;
  c0=c1=c2=c3=c4=c5=c6=c7=c8=c9=c10=c11=c12=c13=c14=c15=c16=c17=0;
  
  camber=0;
  sideSlip=0;
  slipPercentage=0;
  Fz=0;
  
  // Output
  Fx=Fy=Mz=0;
  longStiffness=latStiffness=0;
  maxForce.Set(0,0,0);
}
void pPacejka::setToDefault()
{
		//; Pacejka constants
		//Lateral coefficients

		a0=1.30000f;
		a1=-49.00f;
		a2=1216.00f;
		a3=1632.00f;
		a4=11.00f;
		a5=0.00600f;
		a6=-0.04000f;
		a7=-0.40000f;
		a8=0.00300f;
		a9=-0.00200f;
		
		//;a10=0.16000

		a10=0.f;
		a111=-11.00f;
		a112=0.04500;
		a12=0.00f;
		a13=0.0f;
		
		//;a13=-23.50000
		
		//; Longitudinal coefficients
		b0=1.57000f;
		b1=-48.00f;
		b2=1338.00f;
		b3=6.80000;
		b4=444.00f;
		b5=0.00f;
		b6=0.00340f;
		b7=-0.00800f;
		b8=0.66000f;
		b9=0.00f;
		b10=0.00f;
		
		//; Aligning moment coefficients
		c0=2.46000f;
		c1=-2.77000f;
		c2=-2.90000f;
		c3=0.00f;
		c4=-3.60000f;
		c5=-0.10000f;
		c6=0.00040f;
		c7=0.22000f;
		c8=-2.31000f;
		c9=3.87000f;
		c10=0.00070f;
		c11=-0.05000f;
		c12=-0.00600f;
		c13=0.33000f;
		c14=-0.04000f;
		c15=-0.40000f;
		c16=0.09200f;
		c17=0.01140f;
		
		//Statistical data (SR, SA in radians)
		/*optimal_slipratio=0.09655
		optimal_slipangle=0.18296
		*/

}
pPacejka::~pPacejka()
{
}



/**********
* Physics *
**********/
void pPacejka::Calculate()
{
  // Calculate long. force (and long. stiffness plus max long. force)
  Fx=CalcFx();
  // Calculate lateral force, cornering stiffness and max lateral force
  Fy=CalcFy();
  // Aligning moment (force feedback)
  Mz=CalcMz();
}

float  pPacejka::CalcFx()
// Calculates longitudinal force
// From G. Genta's book, page 63
// Note that the units are inconsistent:
//   Fz is in kN
//   slipRatio is in percentage (=slipRatio*100=slipPercentage)
//   camber and slipAngle are in degrees
// Resulting forces are better defined:
//   Fx, Fy are in N
//   Mz     is in Nm
{
  float  B,C,D,E;
  float  Fx;
  float  Sh,Sv;
  float  uP;
  float  FzSquared;

  // Calculate derived coefficients
  FzSquared=Fz*Fz;
  C=b0;
  uP=b1*Fz+b2;
  D=uP*Fz;
  
  // Avoid div by 0
  if((C>-D3_EPSILON&&C<D3_EPSILON)||
     (D>-D3_EPSILON&&D<D3_EPSILON))
  {
    B=99999.0f;
  } else
  {
#ifdef OBS
qdbg("b34=%f, b5Fz=%f, exp=%f, C*D=%f, C=%f, D=%f\n",
b3*FzSquared+b4*Fz,-b5*Fz,expf(-b5*Fz),C*D,C,D);
#endif
    B=((b3*FzSquared+b4*Fz)*expf(-b5*Fz))/(C*D);
  }
  
  E=b6*FzSquared+b7*Fz+b8;
  Sh=b9*Fz+b10;
  Sv=0;
  
  // Notice that product BCD is the longitudinal tire stiffness
  longStiffness=B*C*D; // *RR_RAD2DEG;    // RR_RAD2DEG == *360/2PI
  
  // Remember the max longitudinal force (where sin(...)==1)
  maxForce.x=D+Sv;

  // Calculate result force
  Fx=D*sinf(C*atanf(B*(1.0f-E)*(slipPercentage+Sh)+
            E*atanf(B*(slipPercentage+Sh))))+Sv;

#ifdef OBS
qdbg("CalcFx; atan(0)=%f\n",atanf(0.0f));
qdbg("  B=%f, C=%f, D=%f, E=%f, Sh=%f, Sv=%f, slipPerc=%f%%\n",
B,C,D,E,Sh,Sv,slipPercentage);
#endif

  return Fx;
}
float  pPacejka::CalcFy()
// Calculates lateral force
// Note that BCD is the cornering stiffness, and
// Sh and Sv account for ply steer and conicity forces
{
  float  B,C,D,E;
  float  Fy;
  float  Sh,Sv;
  float  uP;
  
  // Calculate derived coefficients
  C=a0;
  uP=a1*Fz+a2;
  D=uP*Fz;
  E=a6*Fz+a7;
  
  // Avoid div by 0
  if((C>-D3_EPSILON&&C<D3_EPSILON)||
     (D>-D3_EPSILON&&D<D3_EPSILON))
  {
    B=99999.0f;
  } else
  {
    if(a4>-D3_EPSILON&&a4<D3_EPSILON)
    {
      B=99999.0f;
    } else
    {
      // Notice that product BCD is the lateral stiffness (=cornering)
      latStiffness=a3*sinf(2*atanf(Fz/a4))*(1-a5*fabs(camber));
      B=latStiffness/(C*D);
    }
  }
  
  Sh=a8*camber+a9*Fz+a10;
  Sv=(a111*Fz+a112)*camber*Fz+a12*Fz+a13;
  
  // Remember maximum lateral force
  maxForce.y=D+Sv;
  
  // Calculate result force
  Fy=D*sinf(C*atanf(B*(1.0f-E)*(sideSlip+Sh)+
            E*atanf(B*(sideSlip+Sh))))+Sv;

#ifdef OBS
qdbg("CalcFy=%f\n",Fy);
qdbg("  B=%f, C=%f, D=%f, E=%f, Sh=%f, Sv=%f, slipAngle=%f deg\n",
B,C,D,E,Sh,Sv,sideSlip);
#endif

  return Fy;
}
float  pPacejka::CalcMz()
// Calculates aligning moment
{
  float  Mz;
  float  B,C,D,E,Sh,Sv;
  //float  uP;
  float  FzSquared;
  
  // Calculate derived coefficients
  FzSquared=Fz*Fz;
  C=c0;
  D=c1*FzSquared+c2*Fz;
  E=(c7*FzSquared+c8*Fz+c9)*(1-c10*fabs(camber));
  if((C>-D3_EPSILON&&C<D3_EPSILON)||
     (D>-D3_EPSILON&&D<D3_EPSILON))
  {
    B=99999.0f;
  } else
  {
    B=((c3*FzSquared+c4*Fz)*(1-c6*fabs(camber))*expf(-c5*Fz))/(C*D);
  }
  Sh=c11*camber+c12*Fz+c13;
  Sv=(c14*FzSquared+c15*Fz)*camber+c16*Fz+c17;
  
  Mz=D*sinf(C*atanf(B*(1.0f-E)*(sideSlip+Sh)+
            E*atanf(B*(sideSlip+Sh))))+Sv;
  return Mz;
}


#ifdef OBS


/*********
* Extras *
*********/
float  pPacejka::CalcMaxForce()
// Calculates maximum force that the tire can produce
// If the longitudinal and lateral force combined exceed this,
// a violation of the friction circle (max total tire force) is broken.
// In that case, reduce the lateral force, since the longitudinal force
// is more prominent (this simulates a locked braking wheel, which
// generates no lateral force anymore but does maximum longitudinal force).
{
  float  uP;

  // Calculate derived coefficients
  uP=b1*Fz+b2;

  return uP*Fz;
}
#endif

/**********
* Loading *
**********/
/*
bool pPacejka::Load(QInfo *info,cstring path)
// Get parameters from a file
{
	char buf[128];

	// Fx
	sprintf(buf,"%s.a0",path); a0=info->GetFloat(buf);
	sprintf(buf,"%s.a1",path); a1=info->GetFloat(buf);
	sprintf(buf,"%s.a2",path); a2=info->GetFloat(buf);
	sprintf(buf,"%s.a3",path); a3=info->GetFloat(buf);
	sprintf(buf,"%s.a4",path); a4=info->GetFloat(buf);
	sprintf(buf,"%s.a5",path); a5=info->GetFloat(buf);
	sprintf(buf,"%s.a6",path); a6=info->GetFloat(buf);
	sprintf(buf,"%s.a7",path); a7=info->GetFloat(buf);
	sprintf(buf,"%s.a8",path); a8=info->GetFloat(buf);
	sprintf(buf,"%s.a9",path); a9=info->GetFloat(buf);
	sprintf(buf,"%s.a10",path); a10=info->GetFloat(buf);
	sprintf(buf,"%s.a111",path); a111=info->GetFloat(buf);
	sprintf(buf,"%s.a112",path); a112=info->GetFloat(buf);
	sprintf(buf,"%s.a13",path); a13=info->GetFloat(buf);

	// Fy
	sprintf(buf,"%s.b0",path); b0=info->GetFloat(buf);
	sprintf(buf,"%s.b1",path); b1=info->GetFloat(buf);
	sprintf(buf,"%s.b2",path); b2=info->GetFloat(buf);
	sprintf(buf,"%s.b3",path); b3=info->GetFloat(buf);
	sprintf(buf,"%s.b4",path); b4=info->GetFloat(buf);
	sprintf(buf,"%s.b5",path); b5=info->GetFloat(buf);
	sprintf(buf,"%s.b6",path); b6=info->GetFloat(buf);
	sprintf(buf,"%s.b7",path); b7=info->GetFloat(buf);
	sprintf(buf,"%s.b8",path); b8=info->GetFloat(buf);
	sprintf(buf,"%s.b9",path); b9=info->GetFloat(buf);
	sprintf(buf,"%s.b10",path); b10=info->GetFloat(buf);

	// Mz
	sprintf(buf,"%s.c0",path); c0=info->GetFloat(buf);
	sprintf(buf,"%s.c1",path); c1=info->GetFloat(buf);
	sprintf(buf,"%s.c2",path); c2=info->GetFloat(buf);
	sprintf(buf,"%s.c3",path); c3=info->GetFloat(buf);
	sprintf(buf,"%s.c4",path); c4=info->GetFloat(buf);
	sprintf(buf,"%s.c5",path); c5=info->GetFloat(buf);
	sprintf(buf,"%s.c6",path); c6=info->GetFloat(buf);
	sprintf(buf,"%s.c7",path); c7=info->GetFloat(buf);
	sprintf(buf,"%s.c8",path); c8=info->GetFloat(buf);
	sprintf(buf,"%s.c9",path); c9=info->GetFloat(buf);
	sprintf(buf,"%s.c10",path); c10=info->GetFloat(buf);
	sprintf(buf,"%s.c11",path); c11=info->GetFloat(buf);
	sprintf(buf,"%s.c12",path); c12=info->GetFloat(buf);
	sprintf(buf,"%s.c13",path); c13=info->GetFloat(buf);
	sprintf(buf,"%s.c14",path); c14=info->GetFloat(buf);
	sprintf(buf,"%s.c15",path); c15=info->GetFloat(buf);
	sprintf(buf,"%s.c16",path); c16=info->GetFloat(buf);
	sprintf(buf,"%s.c17",path); c17=info->GetFloat(buf);

	return TRUE;
}
*/

/**********
* Physics *
**********/
/*
bool pPacejka::Load(QInfo *info,cstring path)
// Get parameters from a file
{
	char buf[128];

	// Fx
	sprintf(buf,"%s.a0",path); a0=info->GetFloat(buf);
	sprintf(buf,"%s.a1",path); a1=info->GetFloat(buf);
	sprintf(buf,"%s.a2",path); a2=info->GetFloat(buf);
	sprintf(buf,"%s.a3",path); a3=info->GetFloat(buf);
	sprintf(buf,"%s.a4",path); a4=info->GetFloat(buf);
	sprintf(buf,"%s.a5",path); a5=info->GetFloat(buf);
	sprintf(buf,"%s.a6",path); a6=info->GetFloat(buf);
	sprintf(buf,"%s.a7",path); a7=info->GetFloat(buf);
	sprintf(buf,"%s.a8",path); a8=info->GetFloat(buf);
	sprintf(buf,"%s.a9",path); a9=info->GetFloat(buf);
	sprintf(buf,"%s.a10",path); a10=info->GetFloat(buf);
	sprintf(buf,"%s.a111",path); a111=info->GetFloat(buf);
	sprintf(buf,"%s.a112",path); a112=info->GetFloat(buf);
	sprintf(buf,"%s.a13",path); a13=info->GetFloat(buf);

	// Fy
	sprintf(buf,"%s.b0",path); b0=info->GetFloat(buf);
	sprintf(buf,"%s.b1",path); b1=info->GetFloat(buf);
	sprintf(buf,"%s.b2",path); b2=info->GetFloat(buf);
	sprintf(buf,"%s.b3",path); b3=info->GetFloat(buf);
	sprintf(buf,"%s.b4",path); b4=info->GetFloat(buf);
	sprintf(buf,"%s.b5",path); b5=info->GetFloat(buf);
	sprintf(buf,"%s.b6",path); b6=info->GetFloat(buf);
	sprintf(buf,"%s.b7",path); b7=info->GetFloat(buf);
	sprintf(buf,"%s.b8",path); b8=info->GetFloat(buf);
	sprintf(buf,"%s.b9",path); b9=info->GetFloat(buf);
	sprintf(buf,"%s.b10",path); b10=info->GetFloat(buf);

	// Mz
	sprintf(buf,"%s.c0",path); c0=info->GetFloat(buf);
	sprintf(buf,"%s.c1",path); c1=info->GetFloat(buf);
	sprintf(buf,"%s.c2",path); c2=info->GetFloat(buf);
	sprintf(buf,"%s.c3",path); c3=info->GetFloat(buf);
	sprintf(buf,"%s.c4",path); c4=info->GetFloat(buf);
	sprintf(buf,"%s.c5",path); c5=info->GetFloat(buf);
	sprintf(buf,"%s.c6",path); c6=info->GetFloat(buf);
	sprintf(buf,"%s.c7",path); c7=info->GetFloat(buf);
	sprintf(buf,"%s.c8",path); c8=info->GetFloat(buf);
	sprintf(buf,"%s.c9",path); c9=info->GetFloat(buf);
	sprintf(buf,"%s.c10",path); c10=info->GetFloat(buf);
	sprintf(buf,"%s.c11",path); c11=info->GetFloat(buf);
	sprintf(buf,"%s.c12",path); c12=info->GetFloat(buf);
	sprintf(buf,"%s.c13",path); c13=info->GetFloat(buf);
	sprintf(buf,"%s.c14",path); c14=info->GetFloat(buf);
	sprintf(buf,"%s.c15",path); c15=info->GetFloat(buf);
	sprintf(buf,"%s.c16",path); c16=info->GetFloat(buf);
	sprintf(buf,"%s.c17",path); c17=info->GetFloat(buf);

	return TRUE;
}
*/