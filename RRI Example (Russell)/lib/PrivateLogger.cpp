///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       Reporter
//  Workfile:        PrivateLogger.cpp
//  Revision:        1.0 - 21 January, 2008
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Private disk logger definition file.
//
///////////////////////////////////////////////////////////////////////////////

#include "reporter.h"
#ifndef WIN32
#include "winguard.h"
#endif

namespace Reporter
{
  PrivateLogger::PrivateLogger (const char* id)
  {
    char filename[64];
    sprintf (filename, "%s\\%s-%s.log", id, id, NumDateStr ());
    out_.open (filename, ios_base::app);

#ifdef WIN32
    logMutex_ = CreateMutex (NULL, false, NULL);
#else
	pthread_mutex_init(&logMutex_, NULL);
#endif
  }


  PrivateLogger::~PrivateLogger ()
  {
    out_.close ();
  }


  void PrivateLogger::log (const char* line)
  {
    WaitForSingleObject (logMutex_, INFINITE);
    out_ << TimeStr () << " " << line << endl;
    ReleaseMutex (logMutex_);
  }


  void PrivateLogger::error (const char* line)
  {
    WaitForSingleObject (logMutex_, INFINITE);
    out_ << TimeStr() << " *** ERROR *** " << line << endl;
    ReleaseMutex (logMutex_);
  }
}
