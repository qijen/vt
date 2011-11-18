#include "StdAfx.h"
#include "pVehicleAll.h"

// Paint the stub wheel?
//#define DO_PAINT_STUB

#define DEF_LOCK  2
#define DEF_RADIUS  .20
#define DEF_XA      20

// Input scaling
//#define MAX_INPUT   1000

void pVehicleSteer::setToDefault()
{

	radius = 10.0;
	xa = 0.0f;
	lock = 80.0f;
}

pVehicleSteer::pVehicleSteer(pVehicle *_car)
{
  car=_car;
  angle=0;
  lock=5;
  xa=0;
  position.Set(0,0,0);
  radius=DEF_RADIUS;
  axisInput=0;
}
pVehicleSteer::~pVehicleSteer()
{
  
}

/*
bool pVehicleSteer::Load(QInfo *info,cstring path)
// 'path' may be 0, in which case the default "steer" is used
{
  char buf[128];
  
  if(!path)path="steer";

  // Location
  sprintf(buf,"%s.x",path);
  position.x=info->GetFloat(buf);
  sprintf(buf,"%s.y",path);
  position.y=info->GetFloat(buf);
  sprintf(buf,"%s.z",path);
  position.z=info->GetFloat(buf);
  sprintf(buf,"%s.radius",path);
  radius=info->GetFloat(buf,DEF_RADIUS);
  sprintf(buf,"%s.xa",path);
  xa=info->GetFloat(buf,DEF_XA);
  
  // Physical attribs
  sprintf(buf,"%s.lock",path);
  lock=info->GetFloat(buf,DEF_LOCK)/RR_RAD_DEG_FACTOR;

  // Model (was new'ed in the ctor)
  model->Load(info,path);
  if(!model->IsLoaded())
  {
    // Stub gfx
    quad=gluNewQuadric();
  }
//qdbg("RSteer: r=%f, xa=%f\n",radius,xa);
  return TRUE;
}
*/
/*******
* Dump *
*******/
/*
bool RSteer::LoadState(QFile *f)
{
  f->Read(&angle,sizeof(angle));
  // 'axisInput'?
  return TRUE;
}
bool RSteer::SaveState(QFile *f)
{
  f->Write(&angle,sizeof(angle));
  return TRUE;
}
*/
/**********
* Attribs *
**********/

/********
* Paint *
********/

/**********
* Animate *
**********/
void pVehicleSteer::Integrate()
{
//qdbg("RSteer: steerPos=%d => angle=%f deg\n",axisInput,angle*RR_RAD_DEG_FACTOR);
}

/********
* Input *
********/
void pVehicleSteer::SetInput(int steerPos)
// 'steerPos' is the angle of the input steering wheel
// ranging from -1000..+1000
{
  float a;

  // Clip
  if(steerPos<-1000)
	  steerPos=-1000;
  else if(steerPos>1000)
	  steerPos=1000;
  
  axisInput=steerPos;
  
  // Take controller position
  a=((float)axisInput)*lock/1000.0;
//qdbg("RSteer: axisInput=%d, a=%f,lock=%f\n",axisInput,a,lock);
  // Set this directly as the steering wheel's angles
  // But notice that because of counterclockwise angles being >0,
  // the sign is reversed
  angle=a;
}
