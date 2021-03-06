///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:
//  Workfile:        portable.h
//  Revision:        1.0 - 21 January, 2008
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Variable configuration defintions and declarations for portability between
//  operating systems and compilers.  Also contains miscellaneous utility
//  functions.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef PORTABLE_H
#define PORTABLE_H

#include <time.h>

//! Define the library import/export 
#ifdef WIN32
#ifdef LIBRARY_IMPORTS
#define LIBRARY_API __declspec(dllimport)
#else
#define LIBRARY_API __declspec(dllexport)
#endif


#define _WINSOCKAPI_
#include <windows.h>
#include "unixguard.h"
//! JAM - This is to get rid of an annoying (but harmless) warning MSVC belches
//!       out whenever vectors are used as function parameters.
#pragma warning (disable: 4786)
#pragma warning (disable: 4996)
#pragma warning (disable: 4251)
#else

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#endif


//! @brief Enumerated learning algorithm types
//!
enum LearnType {NEURAL_NET, NEURAL_TISSUE, GENETIC_ALGORITHM};

//! Defines
#define GAGENEVERSION 1
#define GAPARAMCOUNT 20
#define VERSION                1.0

//! Socket defines
#define INET_ADDR_LEN          128
#define SERVER_MAX_CONNECTIONS 8      //! max clients connected at a time 
#define REQUEST_MSG_SIZE       8192   //! max size of request message
#define BASE_CONNECTION_PORT   3000   //! default connection port
#define ERROR_F                -1     //! error signal

//! @brief Assembly default declarations
#define defaultTimeWeight        0.8
#define defaultForceWeight       0.2
#define defaultOutWeight         1.0
#define defaultMaxTime           20   //! Seconds
#define defaultMaxForce          80  //! Newtons
#define defaultPopulationSize    10
#define defaultSubpopulationSize 5
#define defaultClanCount         4
#define defaultEtaMinus          0.99
#define defaultEtaPlus           0.92

//! @brief ABB Robot communication flags
#define FLAG_NOCOMMAND           1
#define FLAG_NEWCOMMAND          2
#define FLAG_ROBMOVING           3
#define FLAG_ROBIDLE             4
#define FLAG_QUIT                5
#define FLAG_BUSY		 6

//! ABB Genetic Algorithms Predefined values
//! @brief Gene descriptor index values
enum {MOVEMETHOD = 0, MOVEID = 1};

//! @brief Mode and search identification values
//!        Position control, natural admittance control, or accommodation
//!        control, or reset the 
enum {RESET = -1, PC = 0, NAC = 1, AC = 2};
//!        Spiral search, radial search, or linear move
enum {SPIRAL = 0, RADIAL = 1, LINEAR = 2};

//! @brief Array index aliases
//! Spiral search parameters
enum {ACSprZForce = 2, ACSprZDepth = 3, ACSprSpeed = 4, ACSprRadius = 5,
      ACSprTurns = 6, ACSprTimeout = 7};
//! Radial search parameters
enum {ACRadZForce = 2, ACRadZDepth = 3, ACRadSpeed = 4, ACRadRange = 5,
      ACRadHopAmp = 6, ACRadHopPer = 7, ACRadCirSpeed = 8, ACRadCirRad = 9,
      ACRadTimeout = 10};
//! Linear move parameters
enum {PCXOffset = 2, PCYOffset = 3, PCZOffset = 4,
      PCXRot = 5, PCYRot = 6, PCZRot = 7};
enum {ACLinXForce = 2, ACLinYForce = 3, ACLinZForce = 4, ACLinXOffset = 5,
      ACLinYOffset = 6, ACLinZOffset = 7, ACLinMinXForce = 8,
      ACLinMaxXForce = 9, ACLinMinYForce = 10, ACLinMaxYForce = 11,
      ACLinMinZForce = 12, ACLinMaxZForce = 13, ACLinTimeout = 14};
enum {NACXPos = 0, NACYPos = 1, NACZPos = 2, NACSpeed = 3, NACTimeout = 4,
      NACMaxForce = 5};

enum statusFlag {None = 0, Move = 1, GetStatus = 2, Quit = 3, Error = 4};

enum methodFlag {PositionControl = 0, ForceControl = 1, Rotation = 2, Gripper = 3, NoMethod = 4, Compliance = 5, guidedRobMotion = 7, startRobPos = 8, QuaternionControl=10,JointControl=11};

//! Type definitions for the common communication base
enum communicationType {nothing, identifier, updateCommunique, commandCommunique, announce, ack};
enum objectTypes {NAType = -1, master, effector, observer};
//enum commandCaps {noMove, cMove, qMove , pMove, rMove, lMove, startPos, gOpen, gClose, guidedMotion, track, setInterrupts, resetInterrupts, setCompliance,disconnect}; 
enum commandCaps {noMove, cMove, qMove , pMove, rMove, lMove, startPos, gOpen, gClose, guidedMotion, track, setInterrupts, resetInterrupts, setCompliance, disconnect, cMoveRel, cMoveAbs, dLog, jMove};
enum reportCaps  {noReport, state, position, forces, custom_data}; 
enum currState {NAState = -1 ,off,on,moving,tracking};
enum gripState {gripClose = 0, gripOpen = 1};
enum ACKType {NACK = 0, ACK = 1};
enum netProto {TCP, UDP};
enum logState {endLog = 0, beginLog };




//! @brief Get the value of pi
//!
//! @return Pi to 54 decimal places
//!
inline double pi ()
{
  return 3.141592653589793238462643383279502884197169399375105820;
};


inline double getCurrentTime ()
{

#ifdef WIN32
  unsigned long time_ll = 0;   //was long
  //! Get time from processor in ticks
  time_ll = GetTickCount ();
  //time_ll = timeGetTime ();
  //! Set the current time in ms
  return ((double)time_ll);///1000.0; // got rid of a division by 500000.0;
#elif defined VXWORKS
  unsigned long time_ll = 0;   //was long
  pentiumTscGet64 (&time_ll);
  return ((double)time_ll);///1000.0; // got rid of a division by 500000.0;
#else
  double time_ll;
  struct timespec time_s;

  clock_gettime(CLOCK_REALTIME, &time_s);
  time_ll = ((double) time_s.tv_sec) + (((double)time_s.tv_nsec) / 1000000000.0);

  return time_ll;
#endif

}


//! @brief Return a formatted string specifying the current date
//!
//! @return The date in the form YYYY-MM-DD
//!
inline const char* NumDateStr ()
{
  static char buffer[12];
  static time_t rawtime;
  static struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  //! YYYY-MM-DD
  strftime (buffer, 12, "%Y-%m-%d", timeinfo);

  return buffer;
};


//! @brief Return a formatted string specifying the time of day
//!
//! @return The time in the form HH:MM:SS
//!
inline const char* TimeStr ()
{
  static char buffer[12];
  static time_t rawtime;
  static struct tm * timeinfo;

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  //! HH:MM:SS
  strftime (buffer, 12, "%H:%M:%S", timeinfo);

  return buffer;
};


//! @brief Timer class, includes stopwatch and alarm functionality
//!
//! Note:  For Win32 applications, Winmm.lib must be included in the
//!        linked library list
//!
/*
class timer
{
public:
  //! @brief Default constructor
  //!
  inline timer () :
    running_(false)
  {
#ifdef WIN32
    highRes_ = (QueryPerformanceFrequency (&frequency_) ? true : false);
    timeBeginPeriod (1);
#endif
  };

  //! @brief Default destructor
  //!
  inline ~timer ()
  {
    timeEndPeriod (1);
  };

  //! @brief Start the timer if it isn't already running, otherwise
  //!        let it continue to run.
  //!
  inline void start ()
  {
    if (!running_)
    {
      running_ = true;
#ifdef WIN32
    if (highRes_)
    {
      QueryPerformanceCounter (&hRstart_);
    }
    else
    {
      lRstart_ = (long)getCurrentTime ();
    }
#endif
    }
  };

  //! @brief Turn off the timer and start it again from 0
  //!
  inline void restart ()
  {
    running_ = true;
#ifdef WIN32
    if (highRes_)
    {
      QueryPerformanceCounter (&hRstart_);
    }
    else
    {
      lRstart_ = (long)getCurrentTime ();
    }
#endif
  };

  //! @brief Stop the timer and return the elapsed time in ms
  //!
  //! @return The number of milliseconds since the timer was started,
  //!         0.0 if the timer is not currently running
  //!
  inline double stop ()
  {
    if (!running_)
    {
      return 0.0;
    }

#ifdef WIN32
    running_ = false;
    if (highRes_)
    {
      QueryPerformanceCounter (&hRsample_);
      timeDiff_ = hRsample_.QuadPart - hRstart_.QuadPart;
      return ((double)timeDiff_ * 1000.0 /
              (double)frequency_.QuadPart);
    }
    else
    {
      lRsample_ = (long)getCurrentTime ();
      return (lRsample_ - lRstart_);
    }
#endif    
  };

  //! @brief Sample the clock and report the length of time that has
  //!        passed since the timer was started/restarted.
  //!
  //! @return The number of milliseconds since the timer was started,
  //!         0.0 if the timer is not currently running
  //!
  inline double elapsedTime ()
  {
    if (!running_)
    {
      return 0.0;
    }

#ifdef WIN32
    if (highRes_)
    {
      QueryPerformanceCounter (&hRsample_);
      timeDiff_ = hRsample_.QuadPart - hRstart_.QuadPart;
      return ((double)timeDiff_ * 1000.0 /
              (double)frequency_.QuadPart);
    }
    else
    {
      lRsample_ = (long)getCurrentTime ();
      return (lRsample_ - lRstart_);
    }
#endif
  };

  //! @brief Wait until a specific amount of time has passed
  //!
  //! @param ms The time to wait in milliseconds
  //!
  inline void waitUntil (double ms)
  {
    if (highRes_)
    {
      restart ();
      while (elapsedTime () < ms)
      {
        Sleep (1);
      }
      stop ();
    }
    else
    {
      Sleep ((DWORD)ms);
    }
  };

private:
  //! @brief High resolution timer start time
  //!
  LARGE_INTEGER hRstart_;

  //! @brief High resolution timer sample time
  //!
  LARGE_INTEGER hRsample_;

  //! @brief High resolution system timer frequency
  //!
  LARGE_INTEGER frequency_;

  //! @brief Low resolution timer start time
  //!
  long lRstart_;

  //! @brief Low resolution timer sample time
  //!
  long lRsample_;

  //! @brief High resolution time sample difference variable
  //!
  LONGLONG timeDiff_;

  //! @brief Whether the timer is currently running
  //!
  bool running_;

  //! @brief Whether the system is capable of handling high
  //!        resolution clock sampling
  //!
  bool highRes_;
}; // class timer
*/

inline double sgn (double a)
{
  return (a >= 0.0) ? 1.0 : -1.0;
}

#endif
