///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       Reporter
//  Workfile:        Logger.cpp
//  Revision:        1.0 - 21 January, 2008
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Event logger definition file.
//
///////////////////////////////////////////////////////////////////////////////

#include "reporter.h"

namespace Reporter
{
  Logger::Logger ()
  {
    char filename[24];
    sprintf (filename, "%s.log", NumDateStr ());
    out_.open (filename, ios_base::app);
    pthread_mutex_init(&logMutex_, NULL);
  }

  Logger::Logger (const char* fname)
  {
    char filename[1024];
    sprintf (filename, "%s.log", fname);
    out_.open (filename, ios_base::app);
    pthread_mutex_init(&logMutex_, NULL);
  }

  Logger::~Logger ()
  {
    out_.close ();
  }


  void Logger::log (const char* line)
  {
    pthread_mutex_lock(&logMutex_);
    out_ << TimeStr () << " " << line << endl;
    pthread_mutex_unlock(&logMutex_);
  }


  void Logger::error (const char* line)
  {
    pthread_mutex_lock(&logMutex_);
    out_ << TimeStr() << " *** ERROR *** " << line << endl;
    pthread_mutex_unlock(&logMutex_);
  }
}
