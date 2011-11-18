#include "StdAfx.h"
#include <qTimer.h>
#include <time.h>
#include <Windows.h>

#ifdef linux
	#include <sys/time.h>
#endif

// Screen Hertz; for Quizes, PAL should do the trick
#define TRACES_PER_SECOND	50

QTimer::QTimer()
{
	Reset();
}
QTimer::~QTimer()
{
}

void QTimer::ResetBase()
// Reset 0-base for time counting
{
#ifdef USE_VTRACE
  baseVCount=app->GetVTraceCount();
#endif

#ifdef USE_UST
  dmGetUST(&baseUST);
#endif

#ifdef USE_TIME
  time(&base_secs);
#endif

#ifdef USE_GETTIMEOFDAY
  struct timezone tz;
  gettimeofday(&tvBase,&tz);
  // Calculate time in usecs
  base_usecs=tvBase.tv_sec*1000000+tvBase.tv_usec;
#endif

#ifdef USE_OS_TICK
  //baseTicks=(int)GetTickCount();
  baseTicks=(int)timeGetTime();
#endif
}

/**********************
* STOPWATCH EMULATION *
**********************/
void QTimer::UpdateTicks()
// Updates 'ticks' to the last available time instant (NOW)
{
  if(!isRunning)
  { 
    return;
  }

#ifdef USE_VTRACE
  int vc;
  vc=app->GetVTraceCount();
  ticks+=vc-baseVCount;
  baseVCount=vc;
#endif

#ifdef USE_UST
  uint64 vc;
  dmGetUST(&vc);
  ticks+=vc-baseUST;
  baseUST=vc;
#endif

#ifdef USE_OS_TICK
  // Looks a lot like UST
  int t;
  //t=(int)GetTickCount();
  t=(int)timeGetTime();
  ticks+=t-baseTicks;
  baseTicks=t;
#endif

#ifdef USE_GETTIMEOFDAY
  int t;
  struct timezone tz;
  gettimeofday(&tv,&tz);
  t=tv.tv_sec*1000000+tv.tv_usec;
  ticks+=t-base_usecs;
  base_usecs=t;
#endif
}
void QTimer::Reset()
{ /** Starts timer at current time **/
  ResetBase();
  ticks=0;
  isRunning=FALSE;
}
void QTimer::Start()
// Turn timer on
{
  if(isRunning)return;
  ResetBase();
  isRunning=TRUE;
}
void QTimer::Stop()
// Stop time recording
{
  if(!isRunning)return;
  UpdateTicks();
  isRunning=FALSE;
}

/****************
* RETRIEVE TIME *
****************/
void QTimer::GetTime(ulong *secs,ulong *micros)
/** Get passed time since last reset **/
{

}
ulong QTimer::GetSeconds()
/** Get passed seconds since last Start() **/
{ 
#ifdef USE_VTRACE
  long s;
  int  vc;
  //vc=app->GetVTraceCount();
  //return (ulong)((vc-baseVCount)/TRACES_PER_SECOND);
  UpdateTicks();
  return ticks/TRACES_PER_SECOND;
#endif
#ifdef USE_UST
  UpdateTicks();
  return ticks/1000000000;		// 10^9 = nanoseconds
#endif
#ifdef USE_TIME
  time_t t;
  time(&t);
  return (ulong)(t-base_secs);
#endif
#ifdef USE_OS_TICK
  UpdateTicks();
  return ticks/1000;
#endif
#ifdef USE_GETTIMEOFDAY
  UpdateTicks();
  return ticks/1000000;
#endif
}
#ifdef USE_UST
uint64 QTimer::GetTicks()
#else
int QTimer::GetTicks()
#endif
// Get #ticks, 1 tick is 1 vertical blank interval, or nanosecond, or WHATEVER!
// Don't use this function in application code; too many variant
{
  UpdateTicks();
  return ticks;
  //int  vc;
  //vc=app->GetVTraceCount();
  //return (int)(vc-baseVCount);
}
int QTimer::GetMilliSeconds()
{
#ifdef USE_UST
  //uint64 div;
  //div=1000000;
  UpdateTicks();
//qdbg("ticks=%lld\n",ticks);
  return (int)(ticks/1000000);
#endif
#ifdef USE_TIME
  UpdateTicks();
  return (int)(ticks*1000);
#endif
#ifdef USE_OS_TICK
  UpdateTicks();
  return (int)ticks;
#endif
#ifdef USE_GETTIMEOFDAY
  UpdateTicks();
  return ticks/1000;
#endif
}
int QTimer::GetMicroSeconds()
// Returns time in microseconds
{
#ifdef USE_UST
  UpdateTicks();
  return (int)(ticks/1000);
#endif
#ifdef USE_TIME
  UpdateTicks();
  return (int)(ticks*1000000);
#endif
#ifdef USE_OS_TICK
  // Not really microseconds!
  UpdateTicks();
  return (int)ticks;
#endif
#ifdef USE_GETTIMEOFDAY
  UpdateTicks();
  return ticks;
#endif
}


void QTimer::WaitForTime(int secs,int msecs)
// Reasonably multitasking-friendly wait for the timer to pass the given
// time and return.
// On O2's and most low level SGI's, the accurary is ~10ms (scheduler slice)
// Starts the timer if it is not running (instead of just returning)
// On Win32, we might just use Sleep()
{ int n;
  secs=secs*1000+msecs;
  // Make sure timer is running
  if(!IsRunning())Start();
  while(1)
  { n=GetMilliSeconds();
    if(n>=secs)break;
    Sleep(10);
  }
}

/*****************
* TIME ADJUSTING *
*****************/
void QTimer::AdjustMilliSeconds(int delta)
// Adjust time in milliseconds
// 'delta' may be negative or positive. Note that this may underflow the
// timer, meaning it will be negative.
{
#ifdef USE_UST
  uint64 delta64;
  delta64=(uint64)delta;
  // Get time in nanoseconds
  delta64*=1000000;
  ticks+=delta64;
#endif
#ifdef USE_OS_TICK
  // Adjust msecs
  ticks+=delta;
#endif
#ifdef USE_GETTIMEOFDAY
  ticks+=delta*1000;
#endif
}

/** TEST **/

//#define NOTDEF_TEST
#ifdef NOTDEF_TEST

#include <stdio.h>

void main(void)
{ QTimer *qt;
  ulong s,m;
  QAppSetup();
  qt=new QTimer();
  while(!RMB())
  { qt->GetTime(&s,&m);
    printf("S=%d, M=%4d\n",s,m);
  }
  //delete qt;
  QAppQuit();
}

#endif
