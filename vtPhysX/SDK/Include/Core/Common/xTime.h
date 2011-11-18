// racer/time.h

#ifndef __X_TIME_H__
#define __X_TIME_H__
#include <qTimer.h>
#include <BaseMacros.h>


class MODULE_API xTime
// A notion of simulation time
{
 protected:
  float span;		// Time of integration (in seconds)
  int    spanMS;        // Time of integration in milliseconds
  QTimer *tmr;		// Actual real timer
  int curRealTime;	// Last recorded REAL time in msecs
  int curSimTime;	// Time calculated in the sim
  int lastSimTime;      // Last point of simulation

 public:
  xTime();
  ~xTime();

  // Attribs
  int  GetRealTime(){ return curRealTime; }
  int  GetSimTime(){ return curSimTime; }
  int  GetLastSimTime(){ return lastSimTime; }
  int  GetSpanMS(){ return spanMS; }
  inline float GetSpan(){ return span; }

  void AddSimTime(int msecs);
  void SetLastSimTime(){ lastSimTime=curSimTime; }
  void SetSpan(int ms);

  // Methods
  void Start();
  void Stop();
  void Reset();
  void Update();
};

#endif
