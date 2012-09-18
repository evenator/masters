///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       Math
//  Workfile:        NumericalMath.h
//  Revision:        1.0 - 21 January, 2008
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Number-generating mathematical functions.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef Objects_H
#define Objects_H


#include "portable.h"
#include "types.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////


class objectClass{

	  //The most sensible way to use the linked list is by having the first object be ID 1. etc.
	  //This way it will be easy to search for the object by its ID number.
	//This is not necessary however.
public:
	  objectClass* prevObject;
	  objectClass* nextObject;

	  pthread_mutex_t objectMutex;

	  communicationType comTypeIn;
	  communicationType comTypeOut;

	  commandCaps cmdTypeIn;
	  commandCaps cmdTypeOut;

	  reportCaps reportTypeIn;
	  reportCaps reportTypeOut;

	  int targetID;
	  int sourceID;

	  int serverSide;

	  ACKType AckIn;
	  ACKType AckOut;

	  char name[1024];

	  int objID;
	  //! @brief TODO
      //!
      objectTypes objType;
	  const char *objTypeStr;

      //! @brief TODO
      //!
      int cmdCount;

		//! @brief TODO
		//!
	 commandCaps* validCommands;

	 int objectFlag;

	 bool askForObjList;

  //! @brief TODO
  //!
  int repCount;  

  //! @brief TODO
  //!
  reportCaps*  validReportables;
  bool setQuaterion;

  double xPos;
  double yPos;
  double zPos;

  double xRot;
  double yRot;
  double zRot;
  
  double q1;
  double q2;
  double q3;
  double q4;

  gripState grip;

  double xForce;
  double yForce;
  double zForce;
  
  double xRotForce;
  double yRotForce;
  double zRotForce;

  int reportableDataSize;
  double* reportableData;
  

  bool logst; /* whether or not to be logging */
  currState objState;


  //!Default Constructor
  objectClass(void);
  
  objectClass(objectClass*, objectClass*);
  objectClass operator=(objectClass &source);

  ~objectClass();
   void cleanObjectList();

   objectClass* objectInsertBehind();
   objectClass* objectInsertFront();
  
   void remove();
   void add(objectClass*);
  
  objectClass* findObjectByName(const char* objName);
  objectClass* findFowardObjectByName(const char* objName);
  objectClass* findPrevObjectByName(const char* objName);
  objectClass* findObjectByNumber(int objNum);

  void copyWorkingDataFrom(objectClass&);

  void allocateCustomData(int);
  void allocateCustomData(int,double*);
  void deAllocateCustomData();
  void allocateCommands(int,commandCaps*);
  void allocateReportables(int,reportCaps*);


  void prepObjectUpdateCommand(int,reportCaps);
  void prepObjMoveCommand(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double  newXRot = 0.0, double  newYRot = 0.0, double  newZRot = 0.0 );
  void prepObjMoveCommand(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double  newXRot = 0.0, double  newYRot = 0.0, double  newZRot = 0.0, double time = 0);
  void prepObjMoveCommandQ(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double q1 = 0.0,double q2 = 0.0,double q3 = 0.0,double q4 = 0.0, double time = 0 );
  //void prepObjMoveCommandQ(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double  newXRot = 0.0, double  newYRot = 0.0, double  newZRot = 0.0 ,double q1 = 0.0,double q2 = 0.0,double q3 = 0.0,double q4 = 0.0);
 
  void prepObjMoveCommand(int newTargetID,commandCaps newCmd, reportCaps  newReport, double x[3], double r[3]);
  void prepObjMoveCommand(int newTargetID,commandCaps newCmd, reportCaps  newReport, double x[3], double r[3], double time);
  void prepObjMoveCommandQ(int newTargetID,commandCaps newCmd, reportCaps  newReport, double x[3], double q[4]);
  void prepObjMoveCommandQ(int newTargetID,commandCaps newCmd, reportCaps  newReport, double x[3], double q[4], double time);

  bool setMoveTime;
  double moveTime;
 

  void prepCameraStart(int newTargetID);
  void prepCameraStop(int newTargetID, double x[3]);

  /*	CMB added section	*/
  void clearResponseFields(void);
  void prepUpdate(objectClass* target, reportCaps report);
  void prepMoveQ(objectClass* target,commandCaps cmd,reportCaps report,double x[3],double q[4],double time);
  void copyWorkingDataFrom(objectClass *source);
  void copyMsgDataFrom(const objectClass *source);
  /*	end CMB added section	*/
};
#endif


  //! @brief Specialized constructor
  //!
  //! @param previousObject  TODO
  //! @param followingObject TODO
  //!
  //!rcj - these constructors use the pointers which are used for the reportable update xml file. 
  //!
  //! @brief Copy operator
  //!
  //! @param source The original object that is being copied from
 

  //RCJ Add a list removal functions due to object drop out.
  //! @brief Remove the object (this) from the linked list
  //!
 

  //RCJ add the find object by name here.
  //Add recursion
  //! @brief Initiate a recursive search to get a pointer to a specific object
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!



  //RCJ! Recursive forward searching
  //! @brief Recursive forwards search
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!

  
  //RCJ! Recursive backwards searching
  //! @brief Recursive backwards search
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!


  //RCJ! Object Command formatting
  //! @brief This will automatically configure the object 
  //!        in preperation of sending a command
  //!
  //! @param objName TODO
  //!
  //! @return TODO
  //!
  /*int prepObjMoveCommand(int newTargetID,commandCaps newCmd= noMove,reportCaps  newReport= noReport,double  newXPos = 0.0, double  newYPos = 0.0, double  newZPos = 0.0, double  newXRot = 0.0, double  newYRot = 0.0, double  newZRot = 0.0 )
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
	



}; */



//RCJ --where should i put this function?
/*objectStruct* findObjectByName(objectStruct* startObj, char* objName);

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
