#include "StdAfx.h"
#include "pDriveLine.h"

#include "vtPhysXAll.h"
#include "pVehicle.h"
#include "pGearbox.h"
#include "pDifferential.h"


#include <xDebugTools.h>


/**********************
* Driveline component *
**********************/
pDriveLineComp::pDriveLineComp()
{
  // Init member variables
  parent=0;
  child[0]=child[1]=0;
  driveLine=0;
  name="<component>";

  inertia=0.01f;
  ratio=invRatio=1;
  effectiveInertiaDownStream=0;
  cumulativeRatio=0;
  tReaction=tBraking=tEngine=0;
  rotV=rotA=0;
}
pDriveLineComp::~pDriveLineComp()
{
}

//
// Attribs
//
void pDriveLineComp::SetParent(pDriveLineComp *_parent)
// Set a parent for this component. Mostly, this is done implicitly
// using AddChild().
{
  parent=_parent;
}
void pDriveLineComp::SetRatio(float r)
{
  if(fabs(r)<D3_EPSILON)
  { 
	  //qwarn("RDriveLineComp:SetRatio(%.2f); must be >0; adjusted to 1",r);
    r=1;
  }
  ratio=r;
  // Precalculate inverse ratio for faster calculations later
  invRatio=1.0f/r;
}
void pDriveLineComp::Reset()
// Reset variables for a fresh start (like when Shift-R is used)
{
  rotV=rotA=0;
}

pDriveLineComp *pDriveLineComp::GetChild(int n)
{
  switch(n)
  { case 0: return child[0];
    case 1: return child[1];
    default: /*qwarn("RDriveLineComp:GetChild(%d) out of range",n);*/ return 0;
  }
}

void pDriveLineComp::AddChild(pDriveLineComp *comp)
// Add 'comp' as a child
{
  if(!child[0])child[0]=comp;
  else if(!child[1])child[1]=comp;
  //else qwarn("RDriveLineComp:AddChild() failed; already has 2 children");

  // Declare us as parent of the child
  comp->SetParent(this);
}


/*****************
* Precalculation *
*****************/
void pDriveLineComp::CalcEffectiveInertia()
// Calculate the total effective inertia for this node plus the children
{

//qdbg("RDLC:CalcEffectiveInertia() '%s'\n",name.cstr());
  effectiveInertiaDownStream=inertia*cumulativeRatio*cumulativeRatio;
  if(child[0])
  {
    child[0]->CalcEffectiveInertia();
    effectiveInertiaDownStream+=child[0]->GetEffectiveInertia();
  }
  if(child[1])
  {
    child[1]->CalcEffectiveInertia();
    effectiveInertiaDownStream+=child[1]->GetEffectiveInertia();
  }
}
void pDriveLineComp::CalcCumulativeRatio()
// Calculate the cumulative ratio for this component and entire tree
{
  cumulativeRatio=ratio;
  if(child[0])
  {
    child[0]->CalcCumulativeRatio();
  }
  if(child[1])
  {
    child[1]->CalcCumulativeRatio();
  }
  // Take ratio if child[0] only, since only singular links are supported
  // (like engine->gear->driveshaft)
  if(child[0])
    cumulativeRatio*=child[0]->GetCumulativeRatio();
    //cumulativeRatio*=child[0]->GetRatio();
}
void pDriveLineComp::CalcReactionForces()
// Calculate reaction forces from the leaves up
{
  tReaction=tBraking=0;
  if(child[0])
  {
    child[0]->CalcReactionForces();
    tReaction+=child[0]->GetReactionTorque();
    tBraking+=child[0]->GetBrakingTorque();
  }
  if(child[1])
  {
    child[1]->CalcReactionForces();
    tReaction+=child[1]->GetReactionTorque();
    tBraking+=child[1]->GetBrakingTorque();
  }
  // Multiply by the inverse ratio for upstream
  tReaction*=invRatio;
  tBraking*=invRatio;
}

// Stub base class function
void pDriveLineComp::CalcForces(){}
// Stub base class function
void pDriveLineComp::CalcAccelerations(){}
// Stub base class function
void pDriveLineComp::Integrate()
{
//qdbg("RDLC:Integrate() base class\n");
  rotV+=rotA*lastStepTimeSec;
  //qdbg("  rotV=%.2f (rotA=%.2f)\n",rotV,rotA);
}

pDriveLine::pDriveLine(pVehicle *_car)
{
  // Init members
  car=_car;
  root=0;
  preClutchInertia=postClutchInertia=totalInertia=0;
  prepostLocked=true;
  diffs=0;

  // Clutch defaults
  clutchMaxTorque=500;
  clutchLinearity=DEFAULT_CLUTCH_LINEARITY;
  SetClutchApplication(1);
  tClutch=0;
  // Handbrake defaults
  handbrakeApplication=0;
  autoClutch=FALSE;
  prepostLocked=FALSE;
}
pDriveLine::~pDriveLine()
{
}
// Set the root component
void pDriveLine::SetRoot(pDriveLineComp *comp)

{
  root=comp;
}

void pDriveLine::Reset()
{
  autoClutch=false;
  prepostLocked=false;
}

void pDriveLine::CalcCumulativeRatios()
{
  if(root)
    root->CalcCumulativeRatio();
}

void pDriveLine::CalcEffectiveInertiae()
// Calculate all effective inertiae in the driveline
{
  if(root)
    root->CalcEffectiveInertia();
}

void pDriveLine::CalcPreClutchInertia()
// Calculate all inertia before the clutch (engine)
{
  // Engine should be the root
  if(!root)
  {
    //qwarn("pDriveLine:CalcPreClutchInertia(); no root component");
    return;
  }
  preClutchInertia=root->GetInertia();
}

void pDriveLine::CalcPostClutchInertia()
// Calculate all inertia after the clutch (gearbox, diffs, wheels)
{
  // Check for a driveline to be present
  if(!root)
  {
    //qwarn("pDriveLine:CalcPreClutchInertia(); no root component");
    return;
  }
  pDriveLineComp *com  =root->GetChild(0);
  float f  = com->GetEffectiveInertia();
  postClutchInertia=root->GetChild(0)->GetEffectiveInertia();

  // Also calculate total inertia
  totalInertia=preClutchInertia+postClutchInertia;

}

void pDriveLine::SetClutchApplication(float app)
// Set clutch application.
{
  if(app<0)app=0; else if(app>1.0f)app=1.0f;
  // Make it undergo severe linearity; I couldn't get
  // a smooth take-off without stalling the engine.
  app=clutchLinearity*app+(1.0f-clutchLinearity)*(app*app*app);
  clutchApplication=app;
  clutchCurrentTorque=app*clutchMaxTorque;
}
void pDriveLine::DbgPrint(XString& s)
{
  //qdbg("Driveline (%s); clutch: app=%.2f, maxT=%.2f, T=%.2f\n",s,clutchApplication,clutchMaxTorque,clutchCurrentTorque);
  // Print tree
  /*
  if(root)
    root->DbgPrint(0,s);
  */
}

/********
* Input *
********/
void pDriveLine::SetInput(int ctlClutch,int ctlHandbrake)
// Inputs controller state. May be overruled though is some assisting is on.
{

 //float a = car->getEngine()->getAuto
  // Clutch
  bool isAc  = IsAutoClutchActive();
  if(IsAutoClutchActive())
  {
    // Automatic assist is on; don't accept user input for a while
    //SetClutchApplication(car->getEngine()->GetAutoClutch());
  } else
  {
    // Manual
    SetClutchApplication(((float)ctlClutch)/1000.0f);
  }

  // Handbrakes
  handbrakeApplication=((float)ctlHandbrake)/1000.0f;
  if(handbrakeApplication<0)handbrakeApplication=0;
  else if(handbrakeApplication>1)handbrakeApplication=1;
}

/**********
* Physics *
**********/
// Determine forces throughout the driveline.
// Assumes the wheel reaction forces are already calculated.
void pDriveLine::CalcForces()
{
  
	if(root==0||gearbox==0)
	{
		/*qwarn("pDriveLine:CalcForces(); driveline not built yet");*/
		return;
	}

  // In neutral gear the pre and post parts are always unlocked,
  // and no clutch torque is applied.
  
  
  if(car->getGearBox()->IsNeutral())
  {
    UnlockPrePost();
    //tClutch=0;
    clutchCurrentTorque=0;
    //goto skip_clutch_calc;
  }
  

  // Calculate current clutch torque (including torque direction)
  if(!IsPrePostLocked())
  {
    // Engine is running separately from the rest of the driveline.
    // The clutch works to make the velocity of pre-clutch (engine) and
    // post-clutch (rest) equal.
	  float rVel = root->GetRotationVel();
	  float rBox = gearbox->GetRotationVel();
	  float ratio = gearbox->GetRatio();

    if(root->GetRotationVel()>gearbox->GetRotationVel()*gearbox->GetRatio())
	{
      tClutch=GetClutchCurrentTorque();
	}
	else{
      tClutch=-GetClutchCurrentTorque();
	}

  } // else acceleration will be given by the driveline (engine rotates
    // along with the rest of the driveline as a single assembly)

  // Spread wheel (leaf) reaction forces all the way to the engine (root)
  root->CalcReactionForces();

  if(IsPrePostLocked())
  {
    // Check if pre and post are still locked
    float Te,Tr,Tb,Tc;
    Te=root->GetEngineTorque();
    Tr=root->GetReactionTorque();
    Tb=root->GetBrakingTorque();
    Tc=GetClutchCurrentTorque();

   if(fabs(Te-(Tr+Tb))>Tc)
    {
//qdbg("  pre-post gets UNLOCKED\n");
      UnlockPrePost();
    }
  } // else it will get locked again when velocity reversal happens
    // of the engine vs. rest of the drivetrain (=gearbox in angular velocity)

  if(IsSingleDiff())
  {
  
    // Special case where some optimizations can be done
    pDifferential *diff=car->getDifferential(0);
//qdbg("Single diff case:\n");
    if(IsPrePostLocked())
    {
     
	
		float Te,r;
		Te=root->GetEngineTorque();
		r=root->GetCumulativeRatio();
#ifdef LTRACE
	qdbg("  Diff gets Te=%.2f * ratio %.2f = %.2f\n",Te,r,Te*r);
#endif
	
		diff->CalcSingleDiffForces(Te*r,root->GetEffectiveInertia());
    } else
    {
	
		// Engine spins at a different rate from the rest of the driveline
		// In this case the clutch fully works on getting the pre- and post-
		// clutch angular velocities equal.
		// Note that if the clutch is fully depressed (engine totally decoupled
		// from the rest of the driveline), this torque will be 0, and the
		// postclutch driveline will just rotate freely.
#ifdef LTRACE
	qdbg("  SingleDiff and prepost unlocked.\n");
#endif
	
		float Tc,r;
		Tc=GetClutchTorque();
		r=root->GetCumulativeRatio();
		float a1 = root->GetEffectiveInertia();

#ifdef LTRACE
	qdbg("  Diff gets Tc=%.2f * ratio %.2f = %.2f\n",Tc,r,Tc*r);
#endif
		
		diff->CalcSingleDiffForces(Tc*r,root->GetEffectiveInertia());
	  
	}
  }
}
void pDriveLine::CalcAccelerations()
{

#ifdef LTRACE
	qdbg("pDriveLine::CalcAccelerations()\n");
#endif

  if(IsSingleDiff())
  {
	// Special case with speedier calculations
	pDifferential *diff=car->getDifferential(0);
	pDriveLineComp *comp;
	float acc=diff->GetAccIn();
	if(IsPrePostLocked())
	{
		//qdbg("Single diff, prepost LOCKED\n");
		// Everything is decided by the differential acceleration
		// Wheels got their acceleration in RWheel::CalcAccelerations()
		// Pass acceleration up the tree; mind the ratios
		for(comp=diff;comp;comp=comp->GetParent())
		{
			comp->SetRotationAcc(acc);
			acc*=comp->GetRatio();
#ifdef LTRACE
		qdbg("  comp '%s' acc %.2f (ratio %.2f)\n",comp->GetName(),acc,comp->GetRatio());
#endif
	
      }
    } else
    {
      // Separate pre- and postclutch accelerations
      // First calculate the engine's acceleration.
      root->CalcAccelerations();
      // Rest of the driveline takes its acceleration from the diff
      // Wheels got their acceleration in RWheel::CalcAccelerations()
      // Pass acceleration up the tree, EXCEPT for the engine; mind the ratios
      // May combine this into the loop above by a sentinel component
      // which is either '0' or 'root'. (for the PrePostLocked case)
      for(comp=diff;comp!=root;comp=comp->GetParent())
      {
        comp->SetRotationAcc(acc);
        acc*=comp->GetRatio();
		//qdbg("  comp '%s' acc %.2f (ratio %.2f)\n",//comp->GetName(),acc,comp->GetRatio());
      }
    }
  }
}

void pDriveLine::Integrate()
{

  float deltaVel,newDeltaVel;

  // Remember difference between engine and gear rotation
  float a = gearbox->GetRotationVel();
  float ab  = gearbox->GetRatio();
  deltaVel=root->GetRotationVel()-gearbox->GetRotationVel()*gearbox->GetRatio();
  if(IsPrePostLocked())
  {
    // Check for the engine and gearbox (ratio'd)
    // to rotate just about equally.
    // If not, the driver may have speedshifted without
    // applying the clutch. Unfortunately, this is possible
    // but should result in damage in the future, since
    // you get a lot of gear noise.
    if(fabs(deltaVel)>DELTA_VEL_THRESHOLD)
    {
      
      // Unlock pre-post to let things catch up again
      UnlockPrePost();
    }
  }

#ifdef LTRACE

  qdbg("rotV: engine=%.3f, gearbox=%.3f\n",root->GetRotationVel(),gearbox->GetRotationVel());
  qdbg("  engine=%p, root=%p, gearbox=%p\n",car->getEngine(),root,gearbox);
  qdbg("  rpm=%f\n",car->getEngine()->getRPM());

  int cGear  =car->getGearBox()->GetGear();
  qdbg("  currentGear=%d %s \n",cGear,car->getGearBox()->GetGearName(cGear).CStr());

#endif


  // Engine
  if(root)root->Integrate();
  if(gearbox)gearbox->Integrate();

  if(!IsPrePostLocked())
  {
    // Check if gearbox is catching up with engine
    newDeltaVel=root->GetRotationVel()-gearbox->GetRotationVel()*gearbox->GetRatio();
//qdbg("Check lock; oldDV=%.3f, newDV=%.3f\n",deltaVel,newDeltaVel);
    if((deltaVel>0&&newDeltaVel<0)||
       (deltaVel<0&&newDeltaVel>0))
    {

#ifdef LTRACE
qdbg("  RE-LOCKED!\n");
#endif
		
		LockPrePost();
      // Force engine and gearbox velocity to be the same
#ifdef LTRACE
	qdbg("  engine rotV=%.3f, gearbox rotV=%.3f\n",root->GetRotationVel(),gearbox->GetRotationVel());
#endif
		
		float gVel = gearbox->GetRotationVel();
		float gRatio = gearbox->GetRatio();
		float a = gVel * gRatio;
		root->SetRotationVel(gearbox->GetRotationVel()*gearbox->GetRatio());
//	    root->SetRotationVel(gRatio * gVel);

    }
  }
}


