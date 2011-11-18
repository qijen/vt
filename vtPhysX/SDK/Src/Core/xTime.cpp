#include <StdAfx.h>


#include "xTime.h"
#include <pch.h>
xTime::xTime()
{
  // Default time span
  span=0.01;
  spanMS=10;
  tmr=new QTimer();
  Reset();
}
xTime::~xTime()
{
  SAFE_DELETE(tmr);
}

/**********
* Attribs *
**********/
void xTime::AddSimTime(int msecs)
// Another 'msecs' of time was simulated
{
  curSimTime+=msecs;
}
#ifdef OBS_INLINED
void xTime::SetLastSimTime(int t)
// 't' is the time to which the sim has been simulated
// This is set after a number of integrations to remember the last
// time for the next graphics frame (calculating the distance in time to 'now')
{
  lastSimTime=t;
}
#endif
void xTime::SetSpan(int ms)
// Set integration step time in milliseconds
{
  spanMS=ms;
  span=((float)ms)/1000.0f;
}



/*************
* Start/stop *
*************/
void xTime::Start()
// Start the real timer
{
  tmr->Start();
}
void xTime::Stop()
// Stop the real timer
{
  tmr->Stop();
}
void xTime::Reset()
// Reset the real timer (to 0)
{
  tmr->Reset();
  curRealTime=0;
  curSimTime=0;
  lastSimTime=0;
}

/*********
* Update *
*********/
void xTime::Update()
// Update the real timing
{
  curRealTime=tmr->GetMilliSeconds();
}
