
#ifndef __QLIB_TIMER_H
#define __QLIB_TIMER_H

#include <BaseMacros.h>
#include <time.h>
#ifdef linux
	#include <sys/time.h>
#endif

// Vertical trace resolution?
//#define USE_VTRACE

#if defined(__sgi)
	// UST nanosecond resolution?
	#define USE_UST
#endif

#if defined(linux)
// time() usage? Bad resolution and misplaced offsets within the second
	#define USE_GETTIMEOFDAY
#endif

#if defined(WIN32)
	#define USE_OS_TICK     // Use milliseconds NT timer
#endif

#ifndef ulong
	typedef unsigned long ulong;
#endif



class MODULE_API QTimer 
{
 private:
#ifdef USE_TIME
  time_t base_secs;
  int    ticks;     // Seconds (not very high frequency!)
#endif
#ifdef USE_GETTIMEOFDAY
  int    ticks;
  struct timeval tv;            // Should do microsecs
  int    base_usecs;            // Base time in usecs
  struct timeval tvBase;        // Base time
#endif
#ifdef USE_OS_TICK
  int   ticks;
  int   baseTicks;
#endif

#if defined(__sgi)
  ulong base_secs,base_micros;	// Intuition (Amiga) style
	#ifdef USE_UST
		uint64 ticks;			// UST ticks go FAST!
		uint64 baseUST;		// UST timing
	#endif
	#ifdef USE_VTRACE
		int   baseVCount;		// Vertical retraces base
		int   ticks;			// Current #ticks recorded
	#endif
#endif

  bool  isRunning;		// Recording time?
  //QClassType ID() const { return QOBJ_TIMER; }

 protected:
  void ResetBase();		// Base GLX vtrace counter
  void UpdateTicks();

 public:
  QTimer();
  ~QTimer();

  bool  IsRunning(){ return isRunning; }
  void  Reset();
  void  Start();
  void  Stop();

  // Get time
  void  GetTime(ulong *secs,ulong *micros);
  ulong GetSeconds();
  int   GetMilliSeconds();
  int   GetMicroSeconds();
#ifdef WIN32
  int    GetTicks();
#else
// SGI
#ifdef USE_UST
  uint64 GetTicks();
#else
  int    GetTicks();
#endif
#endif

  // Adjust time
  void AdjustMilliSeconds(int delta);

  // Higher level
  void WaitForTime(int secs,int msecs=0);
};

#endif
