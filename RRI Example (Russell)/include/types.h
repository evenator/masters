///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Behavior Server
//  Subsystem:       
//  Workfile:        types.h
//  Revision:        1.0 - 6 Apr, 2006
//                   1.1 - 2 July, 2007 : Modified for use on the cell phone
//                                        assembly configuration
//                   2.0 - 1 Sep, 2008  : Generalized for multiple projects
//                   2.1 - 24 May, 2010 : Moved common data types from GA
//                                        library to here
//                   2.2 - 1 Jun, 2010  : Added object structure for the robot
//                                        surgery project
//                   2.3 - 20 Jun, 2010 : Moved the GA data types to a separate
//                                        header file (included by this one)
//  Authors:         J. Marvel, R. Jackson
//
//  Description
//  ===========
//  System-wide type & structure definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>
#include "portable.h"
#include "GAtypes.h"
#include "Object1.h"

#ifdef WIN32


#else
#include <pthread.h>
#endif
///////////////////////////////////////////////////////////////////////////////
/**************************** data structures ********************************/
///////////////////////////////////////////////////////////////////////////////



//! @brief Inner-tag parameter attributes (ex <tag name="val">)
//!
struct xmlAttributes
{
  //! @brief The list of parameter names
  //!
  std::vector<std::string> name;

  //! @brief The list of parameter values
  //!
  std::vector<std::string> val;
};

//! @brief Application configuration options
//!
//! @note This is the only structure that should be populated by application
//!       options or configuration values, and should be ammended whenever
//!       new applications/options are added to the workspace rather than new
//!       structures being defined.  Not all options will be used, but it's
//!       better than having a million and one configuration structures.
//!
struct settingsAggregate
{
  /////////////////////////////////////////////////////////////////////////////
  /*                 GENETIC ALGORITHM APPLICATION OPTIONS                   */
  /////////////////////////////////////////////////////////////////////////////

  //! @brief The number of children per clan
  //!
  int numChildren;

  //! @brief The number of clans to maintain
  //!
  int numClans;

  //! @brief The ETA- value to use in our custom GA implementation
  //!
  double etaMinus;

  //! @brief The ETA+ value to use in our custom GA implementation
  //!
  double etaPlus;

  //! @brief The number of iterations to run per child test
  //!
  int numIterations;

  //! @brief The amount of importance we are placing on the time element
  //!        of the GA result vector
  //!
  double timeWeight;

  //! @brief The amount of importance we are placing on the force element
  //!        of the GA result vector
  //!
  double forceWeight;

  //! @brief The maximum amount of force we are allowing in any given
  //!        direction before we mark an assembly attempt as failed and
  //!        abort
  //!
  double maxForce;

  //! @brief The maximum amount of time we are allowing before we mark an
  //!        assembly attempt as failed and abort
  //!
  double maxTime;

  //! @brief Whether or not we should add a random perturbation once the
  //!        GA has finished executing a gene sequence
  //!
  bool randomOffset;

  //! @brief Whether or not we should dump a raw log file to disk
  //!
  bool logDump;

  //! @brief Default constructor
  //!
  settingsAggregate ()
  {
    timeWeight = defaultTimeWeight;
    forceWeight = defaultForceWeight;
    maxTime = defaultMaxTime;
    maxForce = defaultMaxForce;
    numChildren = defaultPopulationSize;
    numClans = defaultClanCount;
    etaMinus = defaultEtaMinus;
    etaPlus = defaultEtaPlus;
    randomOffset = false;
    logDump = false;
  }
};

//! @brief Cartesian pose information
//!
struct robotPose
{
  //! @brief Cartesian X axis coordinate
  //!
  double x;

  //! @brief Cartesian Y axis coordinate
  //!
  double y;

  //! @brief Cartesian Z axis coordinate
  //!
  double z;

  //! @brief Cartesian X axis rotation value
  //!
  double xrot;

  //! @brief Cartesian Y axis rotation value
  //!
  double yrot;

  //! @brief Cartesian Z axis rotation value
  //!
  double zrot;

  //! @brief Default constructor
  //!
  robotPose ()
  {
    x = y = z = xrot = yrot = zrot = 0.0;
  }
};


//! @brief Image-space coordinates of the cell phone components
//!
struct point
{
  //! @brief Cartesian X axis coordinate
  //!
  double x;

  //! @brief Cartesian Y axis coordinate
  //!
  double y;

  //! @brief Cartesian Z axis coordinate
  //!
  double z;

  //! @brief Default constructor
  //!
  point ()
  {
    x = y = z = 0.0;
  }

  //! @brief Assignmet constructor
  //!
  point (double px, double py, double pz) :
    x(px),
    y(py),
    z(pz)
  {
  }

  //! @brief Point assignment function
  //!
  //! @param source An existing point that will be used to populate this point
  //!               instance
  //!
  point & operator=(const point &source)
  {
    if (this != &source)
    {
      x = source.x;
      y = source.y;
      z = source.z;
    }
    return *this;
  }


  //! @brief Point adjustment function
  //!
  //! @param pB A second reference point to be added to the current point
  //!
  //! @return The updated point in space
  //!
  point operator+(const point &pB)
  {
    point pC;
    pC.x = x + pB.x;
    pC.y = y + pB.y;
    pC.z = z + pB.z;
    return pC;
  }

  //! @brief Compute the Cartesian distance to another point
  //!
  //! @param dest The comparitive point to which we are finding the distance
  //! @param root Whether or not to take the square root (false makes the
  //!             equation faster--useful when only doing nearest-neighbor
  //!             comparisons)
  //!
  //! @return The Cartesian distance between this point and a destination
  //!         point
  //!
  double distance (const point &dest, bool root = false)
  {
    static double temp;
    temp = (dest.x - x) * (dest.x - x);
    temp += (dest.y - y) * (dest.y - y);
    temp += (dest.z - z) * (dest.z - z);
    return (root ? sqrt (temp) : temp);
  }
};

//! @brief Robot configuration data for virtual sensor output
//!
struct robotStruct
{
  //! @brief The robot's current Cartesian X coordinate
  //!
  double x;

  //! @brief The robot's current Cartesian Y coordinate
  //!
  double y;

  //! @brief The robot's current Cartesian Z coordinate
  //!
  double z;

  //! @brief The robot's current Cartesian X axis rotation
  //!
  double rx;

  //! @brief The robot's current Cartesian Y axis rotation
  //!
  double ry;

  //! @brief The robot's current Cartesian Z axis rotation
  //!
  double rz;

  //! @brief The robot's sensed Cartesian X force
  //!
  double fx;

  //! @brief The robot's sensed Cartesian Y force
  //!
  double fy;

  //! @brief The robot's sensed Cartesian Z force
  //!
  double fz;

  //! @brief The robot's sensed Cartesian X axis torque
  //!
  double frx;

  //! @brief The robot's sensed Cartesian Y axis torque
  //!
  double fry;

  //! @brief The robot's sensed Cartesian Z axis torque
  //!
  double frz;

  //! @brief The tool's current quaternions q1
  //!
  double q1;

   //! @brief The tool's current quaternions q2
  //!
  double q2;

   //! @brief The tool's current quaternions q3
  //!
  double q3;

   //! @brief The tool's current quaternions q4
  //!
  double q4;

  //! @brief Timestamp
  //!
  double ts;

  double moveTime;

  //! @brief state of the Gripper
  //!
  gripState gripper;

  //! @brief TODO
  //!
  int flag;

  int sendComFlag;  //! RCJ! I added this get rid of sending doubles commands to the ABB.

  //! @brief Internal flag to be set and interpreted in different ways
  //!
  int intflag;

  int ackflag1;
  int ackflag2;

  //! @brief Program control status (move, quit, etc.)
  //!
  statusFlag status;

  //! @brief The method by which the robot should be moving
  //!
  methodFlag method;

  //! @brief Packet sequence number
  //!
  int seq;

  //! @brief Packet TimeStamp
  //!
  double inTimestamp;

  //! @brief system TimeStamp
  //!
  double sysTimestamp;

  //! @brief outputmessage Length
  //!
  int iMsgL;

  //! @brief Image grab flag
  //!
  int grabImg;

  //! @brief Move ready flag
  //!
  int moveFlag;

  //! @brief Mutex for protecting against access violations
  //!
  pthread_mutex_t robotMutex;

  //! @brief Default constructor
  //!
  robotStruct ()
  {
	  pthread_mutex_init(&robotMutex, NULL);
    seq = grabImg = moveFlag = 0;
    flag = intflag = ackflag1 = ackflag2 = 0;
    status = None;
  }

  //! @brief Default destructor
  //!
  ~robotStruct ()
  {
//    CloseHandle(robotMutex);
	pthread_mutex_destroy(&robotMutex);
  }

  //! @brief RobotStruct assignment function
  //!
  //! @param source An existing robot struct that will be used to populate this
  //!               structure instance
  //!
  robotStruct & operator=(const robotStruct &source)
  {
    if (this != &source)
    {
      x = source.x;
      y = source.y;
      z = source.z;
      rx = source.rx;
      ry = source.ry;
      rz = source.rz;
      fx = source.fx;
      fy = source.fy;
      fz = source.fz;
      frx = source.frx;
      fry = source.fry;
      frz = source.frz;
      gripper = source.gripper;
      flag = source.flag;
      sendComFlag = source.sendComFlag;
      intflag = source.intflag;
      ackflag1 = source.ackflag1;
      ackflag2 = source.ackflag2;
      status = source.status;
      method = source.method;
      seq = source.seq;
      grabImg = source.grabImg;
      moveFlag = source.moveFlag;

      //robotMutex = source.robotMutex;
    }
    return *this;
  }
};


//! @brief State setting variables for the server
//!
struct networkSettings
{
  //! @brief Whether we are currently connected
  //!
  bool connected;

  //! @brief Whether the server is currently running
  //!
  bool globalRunServer;

  //! @brief Whether a disconnect request has been issued
  //!
  bool disconnectRequest;

  //! @brief Internal debugging string
  //!
  char globalString[1024];

  //! @brief Internal debugging socket string
  //!
  char sockString[1024];

  //! @brief Run process flag
  //!
  unsigned short runFlag;

  //! @brief Whether or not to commit the GSI logs to disk
  //!
  bool log;

  //! @brief Default settings constructor
  //!
  networkSettings  () : 
    connected(false),
    globalRunServer(false),
    disconnectRequest(false),
    log(false)
  {
  }
};


struct matrix
{
  //! @brief Pointer to the matrix data
  //!
  double **data;
  
  //! @brief The number of rows in the defined matrix
  //!
  int rows;

  //! @brief The number of columns in the defined matrix
  //!
  int cols;

  //! @brief Default constructor
  //!
  matrix () :
    data(NULL),
    rows(0),
    cols(0)
  {
  }

  //! @ brief Default destructor
  //!
  ~matrix ()
  {
    int y;
    if (data != NULL)
    {
      for (y = 0; y < rows; ++y)
      {
        delete [] data[y];
      }
      delete [] data;
    }
  }

  //! @brief Specialized constructor that specifies the dimensions
  //!
  //! @param r The number of rows
  //! @param c The number of columns
  //!
  matrix (int r, int c)
  {
    int y, x;
    rows = r;
    cols = c;

    data = new double*[rows];
    for (y = 0; y < rows; ++y)
    {
      data[y] = new double[cols];
      for (x = 0; x < cols; ++x)
      {
        data[y][x] = 0.0f;
      }
    }
  }

  //! @brief Adjust the dimensions of the matrix
  //!
  //! @param r The number of rows
  //! @param c The number of columns
  //!
  void resize (int r, int c)
  {
    int x, y;
    if (data != NULL)
    {
      for (y = 0; y < rows; ++y) 
      {
        delete [] data[y];
      }
      delete [] data;
    }

    rows = r;
    cols = c;
    data = new double*[rows];
    for (y = 0; y < rows; ++y)
    {
      data[y] = new double[cols];
      for (x = 0; x < cols; ++x)
      {
        data[y][x] = 0.0f;
      }
    }
  }

  //! @brief Data accessor
  //!
  //! @param row The row that is being addressed
  //! @param col The column that is being addressed
  //!
  //! @return The value stored at (row, col)
  double& at (int row, int col)
  {
    if (row < rows && row >= 0 &&
        col < cols && col >= 0)
    {
      return data[row][col];
    }
    return data[0][0];
  }
};


//! @brief Generic object definition structure
//!
//! @note The purpose of this is to abstract a robot or a sensor to a larger unit.
//!
/*struct objectStruct
{
//rcj The object structure will contain a linked list.
//The linked list will include a pointer to the next object and the previous object
//The Constructor has been modified to include this functionality. 
//The most sensible way to use the linked list is by having the first object be ID 1. etc.
//This way it will be easy to search for the object by its ID number.
  //! @brief TODO
  //!
  objectStruct* prevObject;

  //! @brief TODO
  //!
  objectStruct* nextObject;

//rcj The variables below are used for determining the type of communication 
//along with

  //! @brief TODO
  //!
  HANDLE objectMutex;

  //! @brief TODO
  //!
  communicationType comType;

  //! @brief TODO
  //!
  commandCaps cmdType;

  //! @brief TODO
  //!
  reportCaps reportType;

  //! @brief TODO
  //!
  int targetID;

  //! @breif TODO
  //!
  int sourceID;

  //! @brief TODO
  //!
  ACKType Ack; 

//rcj the components below contain the actual descriptors of the system
  //! @brief TODO
  //!
  char name[1024];

  //! @brief TODO
  //!
  int objID;

  //! @brief TODO
  //!
  objectTypes objType;

  //! @brief TODO
  //!
  int cmdCount;

  //! @brief TODO
  //!
  commandCaps* validCommands;

  //! @brief TODO
  //!
  int repCount;  

  //! @brief TODO
  //!
  reportCaps*  validReportables;
  
    
//rcj The data below is what can be reported by the robot.
//The reportable data is meant to be an open ended method that would allow for an arbitrary amount of data to be reported
//As of right now, it is intended that the Master will somehow know how to interpret the data. 

  //! @brief The Cartesian X coordinate of the object
  //!
  double xPos;

  //! @brief The Cartesian Y coordinate of the object
  //!
  double yPos;

  //! @brief The Cartesian Z coordinate of the object
  //!
  double zPos;

  //! @brief The Cartesian X axis rotation of the object
  //!
  double xRot;

  //! @brief The Cartesian Y axis rotation of the object
  //!
  double yRot;

  //! @brief The Cartesian Z axis rotation of the object
  //!
  double zRot;

  gripState grip;

  //! @brief The measured Cartesian X axis force
  //!
  double xForce;

  //! @brief The measured Cartesian Y axis force
  //!
  double yForce;

  //! @brief The measured Cartesian Z axis force
  //!
  double zForce;

  //! @brief The measured Cartesian X axis torques
  //!
  double xRotForce;

  //! @brief The measured Cartesian Y axis torques
  //!
  double yRotForce;

  //! @brief The measured Cartesian Z axis torques
  //!
  double zRotForce;

  //! @brief TODO
  //!
  double* reportableData;

  //! @brief TODO
  //!
  int reportableDataSize;

  //! @brief TODO
  //!
  currState objState;

  //! @brief Default constructor
  //!
  objectStruct() :
    prevObject(NULL),
    nextObject(NULL),
    validCommands(NULL),
    validReportables(NULL),
    comType(nothing),
    xPos(0.0f),
    yPos(0.0f),
    zPos(0.0f),
    xRot(0.0f),
    yRot(0.0f),
    zRot(0.0f),
    grip(gripOpen),
    xForce(0.0f),
    yForce(0.0f),
    zForce(0.0f),
    xRotForce(0.0f),
    yRotForce(0.0f),
    zRotForce(0.0f),
    objID(0),
    targetID(0),
    sourceID(0),
  objState(NAState),
  reportableData(NULL),
  reportableDataSize(NULL),
  objType(NAType)

  
  {
    sprintf(name,"\n");
	objectMutex = CreateMutex (NULL, false, NULL);
  };

  //! @brief Specialized constructor
  //!
  //! @param previousObject  TODO
  //! @param followingObject TODO
  //!
  //!rcj - these constructors use the pointers which are used for the reportable update xml file. 
  //!
  objectStruct(objectStruct* previousObject, objectStruct* followingObject) :
    validCommands(NULL),
    validReportables(NULL),
    comType(nothing),
    xPos(0.0f),
    yPos(0.0f),
    zPos(0.0f),
    xRot(0.0f),
    yRot(0.0f),
    zRot(0.0f),
    grip(gripOpen),
    xForce(0.0f),
    yForce(0.0f),
    zForce(0.0f),
    xRotForce(0.0f),
    yRotForce(0.0f),
    zRotForce(0.0f),
    objID(0),
    sourceID(0),
    targetID(0),
    objState(NAState),
    reportableData(NULL),
    reportableDataSize(NULL),
    objType(NAType)
  {
    objectMutex = CreateMutex (NULL, false, NULL);
    sprintf(name,"\n");
    this->prevObject = NULL;
    if(previousObject != NULL)
    {
      previousObject->nextObject = this;
      this->prevObject = previousObject;
    }
    this->nextObject = NULL;
    if(followingObject != NULL)
    {
      followingObject->prevObject = this;
      this->nextObject = followingObject;
    }
  };

  //! @brief Default destructor
  //!
  ~objectStruct()
  {
    CloseHandle(objectMutex);
	if(validCommands != NULL)
    {
      delete [] validCommands;
    }
    if(validReportables != NULL)
    {
      delete [] validReportables;
    }

    if(nextObject != NULL && prevObject != NULL)
    {
      this->nextObject->prevObject = this->prevObject;
      this->prevObject->nextObject = this->nextObject;
    }
    else 
    { 
      if(nextObject != NULL)
      {
        this->nextObject->prevObject = NULL;
      }
      if(prevObject != NULL)
      {
        this->prevObject->nextObject = NULL;
      }
    }
  };

  //! @brief Copy operator
  //!
  //! @param source The original object that is being copied from
  //!
  //! @return Reference to the updated object
  //!
  objectStruct & operator=(const objectStruct &source)
  {
    Ack = source.Ack;
    cmdCount = source.cmdCount;
    cmdType = source.cmdType;
    comType = source.comType;
    strcpy(name, source.name);
    nextObject = source.nextObject;
    objectMutex = source.objectMutex;
    objID = source.objID;
    objState = source.objState;
    objType = source.objType;
    prevObject = source.prevObject;
    repCount = source.repCount;
    reportableData = source.reportableData;
    reportableDataSize = source.reportableDataSize;
    reportType = source.reportType;
    sourceID = source.sourceID;
    targetID = source.targetID;
    validCommands = source.validCommands;
    validReportables = source.validReportables;
    xForce = source.xForce;
    xPos = source.xPos;
    xRot = source.xRot;
    xRotForce = source.xRotForce;
    yForce = source.yForce;
    yPos = source.yPos;
    yRot = source.yRot;
    yRotForce = source.yRotForce;
    zForce = source.zForce;
    zPos = source.zPos;
    zRot = source.zRot;
    zRotForce = source.zRotForce;
    grip = source.grip;

    return *this;
  }


  //! @brief Add an object to the end of the linked list
  //!
  //! @param obj The object structure to be added to the linked list
  //!
  //! @note This function does not check for duplicates
  //!
  void add (objectStruct *obj)
  {
    if (nextObject != NULL)
    {
      nextObject->add (obj);
    }
    else
    {
      nextObject = obj;
      obj->prevObject = this;
    }
  }

  //RCJ Add a list removal functions due to object drop out.
  //! @brief Remove the object (this) from the linked list
  //!
  void remove()
  {
    if (prevObject != NULL)
    {
      this->prevObject->nextObject = this->nextObject;
    }

    if (nextObject != NULL)
    {
      this->nextObject->prevObject = this->prevObject;
    }
  }

  //RCJ add the find object by name here.
  //Add recursion
  //! @brief Initiate a recursive search to get a pointer to a specific object
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!
  objectStruct* findObjectByName(char* objName)
  {
    objectStruct *found = NULL;
    found = findFowardObjectByName(objName);
    if(found == NULL)
    {
      found = findPrevObjectByName(objName);
    }
    return found; 
  }


  //RCJ! Recursive forward searching
  //! @brief Recursive forwards search
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!
  objectStruct* findFowardObjectByName(char* objName)
  {
    objectStruct *found;
    if(strcmp (this->name, objName) == 0)
    {
      return this;
    }
    if(this->nextObject == NULL)
    {
      return NULL;
    }
    found = this->nextObject->findFowardObjectByName(objName);
    return found;
  }
  
  //RCJ! Recursive backwards searching
  //! @brief Recursive backwards search
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!
  objectStruct* findPrevObjectByName(char* objName)
  {
    objectStruct *found;
    if(strcmp (this->name, objName) == 0)
    {
      return this;
    }
    if(this->prevObject == NULL)
    {
      return NULL;
    }
    found = this->prevObject->findPrevObjectByName(objName);
    return found;
  }

  //RCJ! Object Command formatting
  //! @brief This will automatically configure the object 
  //!        in preperation of sending a command
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!
  int prepObjMoveCommand(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double  newXRot = 0.0, double  newYRot = 0.0, double  newZRot = 0.0 )
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comType = commandCommunique;
	  this->cmdType = newCmd;
	  this->Ack = NACK;
	  this->reportType = newReport;
	  this->xPos = newXPos;
	  this->yPos = newYPos;
	  this->zPos = newZPos;
	  this->xRot = newXRot;
	  this->yRot = newYRot;
	  this->zRot = newZRot;
	  return 0;
  }

  int prepObjectUpdateCommand(int newTargetID, reportCaps newReport)
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comType = commandCommunique;
	  this->cmdType = noMove;
	  this->Ack = NACK;
	  this->reportType = newReport;
	  this->xPos = 0.0;
	  this->yPos = 0.0;
	  this->zPos = 0.0;
	  this->xRot = 0.0;
	  this->yRot = 0.0;
	  this->zRot = 0.0;
	  return 0;
  }
	



};



//RCJ --where should i put this function?
objectStruct* findObjectByName(objectStruct* startObj, char* objName);

objectStruct* findObjectByName(objectStruct* startObj, char* objName){
  objectStruct* forwardTemp = NULL;
  objectStruct* backwardTemp = NULL;
  if(strcmp (startObj->name, objName) == 0)
  {
    return startObj;
  }
  forwardTemp = startObj->nextObject;
  backwardTemp = startObj->prevObject;
  while(true)
  {  
    bool Null = true;
    if(forwardTemp != NULL)
    {
      Null = false;
      if(strcmp(forwardTemp->name,objName) == 0)
      {
        return forwardTemp;
      }
    }
    if(backwardTemp != NULL)
    {
      Null = false;
      if(strcmp(backwardTemp->name,objName) == 0)
      {
        return backwardTemp;
      }
    }
    if(Null) return NULL;
  }
} */

#endif
