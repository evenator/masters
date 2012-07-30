///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Behavior Server
//  Subsystem:       
//  Workfile:        GAtypes.h
//  Revision:        1.0 - 20 Jun, 2010
//  Authors:         J. Marvel
//
//  Description
//  ===========
//  Type and structure definitions used by the Genetic Algorithms code
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GATYPES_H
#define GATYPES_H

#include "portable.h"


//! @brief Genetic heritage data of a given gene vector
//!
struct GenData
{
  //! @brief The epoch number for which a given gene or result value belongs
  //!
  int generation;

  //! @brief The parent/clan identifier specifying which clan a given gene
  //!        belongs to
  //!
  int parent;

  //! @brief The child number (identity) within a clan
  //!
  int identity;

  //! @brief The score assigned to a given child/result value
  //!
  double score;

  //! @brief Default constructor
  //!
  GenData () :
    generation(0),
    parent(0),
    identity(0),
    score(0.0)
  {
  }
};


//! @brief Gene definition for the Genetic Algorithms implementation
//!
struct GAGene
{
  //! @brief Whether or not this gene is COMPLETELY immune to mutation
  //!
  bool readOnly;

  //! @brief The number of elements in the search parameter vector
  //!
  const int params;

  //! @brief The search parameter vector that is mutated and optimized
  //!
  double searchParam[GAPARAMCOUNT];

  //! @brief Parameter vector version number.  Versions are not forwards- or
  //!        backwards-compatible due to the RAPID code having to be modified
  //!        to accommodate more or fewer elements being sent in the GSI XML
  //!        strings.
  //!
  int version;

  //! @brief The score that a particular gene was awarded based on a specific
  //!        fitness function
  //!
  double score;

  //! @brief Generic constructor
  //!
  GAGene () :
    params(GAPARAMCOUNT),
    version(GAGENEVERSION)
  {
    score = 0.0;
    readOnly = false;
  };

  //! @brief Copy constructor
  //!
  //! @param g An existing gene value that will be used to initialize the
  //!          new gene.
  //!
  GAGene (const GAGene &g) :
    readOnly(g.readOnly),
    params(g.params),
    version(g.version)
  {
    int i;
    for (i = 0; i < params; ++i)
    {
      searchParam[i] = g.searchParam[i];
    }
  };

  //! @brief Default destructor
  //!
  ~GAGene ()
  {
  }

  //! @brief Gene assignment function
  //!
  //! @param source An existing gene value that will be used to 
  //!               populate this gene instance
  //!
  GAGene & operator=(const GAGene &source)
  {
    if (this != &source)
    {
      version = source.version;
      readOnly = source.readOnly;
      score = source.score;
      for (int x = 0; x < params; ++x)
      {
        searchParam[x] = source.searchParam[x];
      }
    }
    return *this;
  }
};


//! @brief Results of the application process
//!
struct GAResult
{
  //! @brief Generalized functional output (for simulators and
  //!        undefined/generalized outputs)
  //!
  double output;

  //! @brief The actual output of the function (without noise)
  //!
  double actual;

  //! @brief How much weight the output should have in the fitness function
  //!
  double outWeight;

  //! @brief The standard deviation of multiple iterations of a single gene
  //!        being executed
  //!
  double outStdev;

  //! @brief The length of time a given stage took to complete
  //!
  double time;

  //! @brief The force value associated with the stage
  //!
  double force;

  //! @brief The distance traveled by the robot during the attempt
  //!        (Intermediate scoring metric)
  double distance;

  //! @brief How much weight the time should have in the fitness function
  //!
  double timeWeight;

  //! @brief How much weight the force value should have in the fitness
  //!        function
  //!
  double forceWeight;

  //! @brief The standard deviation of multiple iterations of a single gene
  //!        being executed
  //!
  double timeStdev;

  //! @brief Generic constructor
  //!
  GAResult ()
  {
    time = 0.0f;
    force = 0.0f;
    timeStdev = 0.0f;
    distance = 0.0f;
    output = 0.0f;
    actual = 0.0f;
    outStdev = 0.0f;

    outWeight = defaultOutWeight;
    timeWeight = defaultTimeWeight;
    forceWeight = defaultForceWeight;
  };

  //! @brief Assignment constructor
  //!
  //! @param t Recorded time value to be assigned to the results
  //! @param f Recorded force value to be assigned to the results
  //!
  GAResult (double t, double f)
  {
    time = t;
    force = f;

    timeWeight = defaultTimeWeight;
    forceWeight = defaultForceWeight;
  }

  //! @brief Assignment constructor
  //!
  //! @param t Recorded output value to be assigned to the results
  //!
  GAResult (double o)
  {
    output = o;
    outWeight = 1.0f;
  }

  //! @brief Generic destructor
  //!
  ~GAResult ()
  {
  }

  //! @brief Assignment operator
  //!
  //! @param source An existing result structure that will be used to
  //!               populate this structure instance
  //!
  GAResult & operator=(const GAResult &source)
  {
    if (this != &source)
    {
      time = source.time;
      force = source.force;
      output = source.output;
      distance = source.distance;
      outStdev = source.outStdev;
      timeStdev = source.timeStdev;
      outWeight = source.outWeight;
      timeWeight = source.timeWeight;
      forceWeight = source.forceWeight;
    }
    return *this;
  }

  //! @brief Addition operator used to combine two result values
  //!
  //! @param source An existing result structure that will be added to
  //!               the current results for analysis
  //!
  GAResult & operator+=(const GAResult &source)
  {
    if (this != &source)
    {
      time += source.time;
      force += source.force;
      output += source.output;
      distance += source.distance;
    }
    return *this;
  }

  //! @brief Division-gets operator used to scale this result value
  //!
  //! @param div The divisor by which the time and force values are modified
  //!
  //! @note Currently used for averaging data
  //!
  GAResult & operator/=(const double div)
  {
    time /= div;
    force /= div;
    output /= div;
    distance /= div;

    return *this;
  }
};


//! @brief Inter-process shared communicator structure
//!
//! @note In the context of the ABB genetic algorithm assembly optimization
//!       project, we have to communicate with the robot via the GSI.  The
//!       communication process is threaded since the GSI is constantly
//!       polling the PC for new updates.  As such, we must have the GA
//!       running in its own thread and must therefore have a means of
//!       passing messages between the two threads via a common interface.
//!
struct GAComm
{
  //! @brief Whether or not the GA communication buffer is ready to receive
  //!        new commands
  //!
  bool readyFlag;

  //! @brief Whether or not the GA gene evaluator should reset its state
  //!
  bool reset;

  //! @brief Connection status
  //!
  //! 0 = disconnected
  //! 1 = connected (idle)
  //! 2 = connected (active)
  //!
  int connStatus;

  //! @brief Status indicator value specifying the current average assembly
  //!        time for a given gene sequence
  //!
  double curTimeAvg;

  //! @brief Status indicator value specifying the current standard deviation
  //!        of the gene sequence's sample assembly times
  //!
  double curTimeStdev;

  //! @brief Status indicator value specifying the current average output for
  //!        a given gene sequence
  //!
  double curOutAvg;

  //! @brief Status indicator value specifying the current standard deviation
  //!        of the gene sequence's sample outputs
  //!
  double curOutStdev;

  //! @brief Status indicator for the current iteration number
  //!
  int curIter;

  //! @brief Semaphore used to protect the shared variables used by both sides
  //!        of the communication connection
  //!
  pthread_mutex_t commMutex;

  //! @brief Handle pointer to the current gene sequence that is to be run
  //!        by the robot controller
  //!
  GAGene *genePtr;

  //! @brief Handle pointer to the result structure that is used by the main
  //!        application to populate the interface's learning vectors
  //!
  GAResult *resultPtr;

  //! @brief Default cosntructor
  //!
  GAComm ()
  {
    readyFlag = false;
    reset = false;
    genePtr = NULL;
    resultPtr = NULL;
    curIter = 0;
    curTimeAvg = 0.0f;
    curTimeStdev = 0.0f;
    curOutAvg = 0.0f;
    curOutStdev = 0.0f;

    pthread_mutex_init(&commMutex, NULL);
  };

  //! @brief Default destructor
  //!
  ~GAComm ()
  {
    readyFlag = false;
    reset = false;
    genePtr = NULL;
    resultPtr = NULL;
  };
};

#endif
