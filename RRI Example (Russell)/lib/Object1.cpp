///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Robot Surgery
//  Subsystem:       Object
//  Workfile:        Object1.cpp
//  Revision:        1.0 - 24 September, 2010
//  Author:          R. Jackson
//
//  Description
//  ===========
//  Object Management functions for Robot Communication protocals.
//
///////////////////////////////////////////////////////////////////////////////




#include "Object1.h"
#include <string.h>



objectClass::objectClass(){
    
	prevObject = NULL;
    nextObject = NULL;
    validCommands = NULL;
    validReportables = NULL;
    comTypeIn = nothing;
    comTypeOut = nothing;
	serverSide = 0;
	cmdTypeIn  = noMove;
	cmdTypeOut = noMove;
	cmdCount = 0;
	repCount = 0;
    reportTypeIn = noReport;
    reportTypeOut= noReport;
	objectFlag = 0;
    xPos = 0.0f;
    yPos = 0.0f;
    zPos = 0.0f;
    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;
    grip = gripOpen;
    xForce = 0.0f;
    yForce = 0.0f;
    zForce = 0.0f;
    xRotForce = 0.0f;
    yRotForce = 0.0f;
    zRotForce = 0.0f;
    objID = 0;
    targetID = 0;
    sourceID = 0;
	objState = NAState;
    reportableData = NULL;
    reportableDataSize = 0;
    objType = NAType;
	askForObjList = false;
	setMoveTime = false;
  
	logst = false;
    sprintf(name,"\n");
	pthread_mutex_init(&objectMutex, NULL);
}

objectClass::objectClass(objectClass*  previousObject, objectClass* followingObject)
{
    prevObject = NULL;
    nextObject = NULL;
    validCommands = NULL;
    validReportables = NULL;
	cmdCount = 0;
	repCount = 0;
    comTypeIn = nothing;
    comTypeOut = nothing;
	cmdTypeIn  = noMove;
	cmdTypeOut = noMove;
	serverSide = 0;
    reportTypeIn = noReport;
    reportTypeOut= noReport;
	objectFlag = 0;
    xPos = 0.0f;
    yPos = 0.0f;
    zPos = 0.0f;
    xRot = 0.0f;
    yRot = 0.0f;
    zRot = 0.0f;
    grip = gripOpen;
    xForce = 0.0f;
    yForce = 0.0f;
    zForce = 0.0f;
    xRotForce = 0.0f;
    yRotForce = 0.0f;
    zRotForce = 0.0f;
    objID = 0;
    targetID = 0;
    sourceID = 0;
	objState = NAState;
    reportableData = NULL;
    reportableDataSize = 0;
    objType = NAType;
	AckIn = NACK;
	AckOut = NACK;
	askForObjList = false;
	setMoveTime = false;


  
    sprintf(name,"\n");
	pthread_mutex_init(&objectMutex, NULL);
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



objectClass::~objectClass()
  {
    pthread_mutex_destroy(&objectMutex);
	if(validCommands != NULL)
    {
      // delete [] validCommands;
    }
    if(validReportables != NULL)
    {
      // delete [] validReportables;
    }
	if(reportableData != NULL)
	{
		// delete [] reportableData;
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



//RCJ, For now i am nuking the assignment operator.
//Some parts of it are ill defined. 
//What i will make is a copier that will copy the important data.
/*
objectClass objectClass::operator=(objectClass &source)
{
	AckIn = source.AckIn;
	AckOut = source.AckOut;
	cmdCount = source.cmdCount;
  objectMutex = CreateMutex (NULL, false, NULL);
    cmdTypeIn = source.cmdTypeIn;
	cmdTypeOut = source.cmdTypeOut;
    comTypeIn = source.comTypeIn;
	comTypeOut = source.comTypeOut;
    strcpy(name, source.name);
    nextObject = source.nextObject;
    objID = source.objID;
    objState = source.objState;
    objType = source.objType;
    prevObject = source.prevObject;
    repCount = 0;
    cmdCount = 0;
    reportableCount = 0;
    //repCount = source.repCount;

    if(repCount > 0)
    {
      if( validReportables != NULL)
      {
        delete validReportables;
      }
      validReportables = new reportCaps[repCount];
      for(int i = 0; i < repCount; i++)
      {
        validReportables[i] = source.validReportables[i];
      }
    }
    reportableDataSize = source.reportableDataSize;
    if( reportableDataSize > 0)
    {
      if( reportableData != NULL)
      {
        delete reportableData;
      }
      reportableData = new double[reportableDataSize];
      for(int i = 0; i < reportableDataSize; i++)
      {
        reportableData[i] = source.reportableData[i];
      }
    } 
    if(cmdCount > 0)
    {
      if( validCommands != NULL)
      {
        delete validCommands;
      }
      validCommands = new commandCaps[cmdCount];
      for(int i = 0; i < reportableDataSize; i++)
      {
        validCommands[i] = source.validCommands[i];
      }
    }

    reportTypeIn = source.reportTypeIn;
	  reportTypeOut = source.reportTypeOut;
    sourceID = source.sourceID;
    targetID = source.targetID;
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
  */

 
void objectClass::remove()
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


objectClass* objectClass::findObjectByNumber(int objNum)
  {
    objectClass *found = NULL;
    found = this;
	while(objNum != found->objID)
	{
		if(found->objID < objNum && found->nextObject != NULL)
		{
			if(objNum < found->nextObject->objID)
			{
				found = NULL;
				break;
			}
		} //if(tempObject ->objID < target && tempObject->nextObject != NULL)
		if(found->objID > objNum)
		{
			found = found->prevObject;
		}
		else
		{
			found = found->nextObject;
		}
		if(found == NULL)
		{
			break;
		}
		if(found->objID == objNum)
		{
			break;
		}
	}//while(target != tempObject->objID)
	return found;  
}





objectClass* objectClass::findObjectByName(const char* objName)
  {
    objectClass *found = NULL;
    found = findFowardObjectByName(objName);
    if(found == NULL)
    {
      found = findPrevObjectByName(objName);
    }
    return found; 
  }




objectClass* objectClass::findFowardObjectByName(const char* objName)
  {
    objectClass *found;
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


objectClass* objectClass::findPrevObjectByName(const char* objName)
  {
    objectClass *found;
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



void objectClass::allocateCustomData(int count)
{
	this->reportableDataSize = count;

	if(this->reportableData != NULL)
	{
		delete [] this->reportableData;
	}
	this->reportableData = new double[count];

};

void objectClass::allocateCustomData(int count, double* sourceData)
{
	if(count > 0)
	{
		this->reportableDataSize = count;

		if(this->reportableData == NULL)
		{
			delete this->reportableData;
		}
		this->reportableData = new double[count];

		for(int i = 0; i < count; i++)
		{
			this->reportableData[i] = sourceData[i];
		}
	}

};




void objectClass::cleanObjectList()
  {
	//!This function cleans the object list
	  objectClass* tempObject = this->nextObject;
	  this->nextObject = NULL;
	  while(tempObject != NULL)
	  {
		  objectClass * oldTempObject = tempObject;
		  tempObject = tempObject->nextObject;
		  delete oldTempObject;   //!This line has problems, check what is being deleted
	  }
	  tempObject = this->prevObject;
	  while(tempObject != NULL)
	  {
		objectClass * oldTempObject = tempObject;
		tempObject = tempObject->prevObject;
		delete oldTempObject;
	  }
}



void objectClass::allocateCommands(int cmdCountIn, commandCaps *validCmds)
{
	this->cmdCount = cmdCountIn;
	this->validCommands = new commandCaps[cmdCount];
	for(int i = 0; i < cmdCount; i++)
	{
		validCommands[i] = validCmds[i];
	}
}



void objectClass::allocateReportables(int reportCountIn, reportCaps *validReports)
{
	this->repCount = reportCountIn;
	this->validReportables = new reportCaps[repCount];
	for(int i = 0; i < repCount; i++)
	{
		validReportables[i] = validReports[i];
	}
}


objectClass* objectClass::objectInsertBehind()
{
	objectClass* tempObject = new objectClass(this,this->nextObject);
	return tempObject;
}




objectClass* objectClass::objectInsertFront()
{
	objectClass* tempObject = new objectClass(this->prevObject,this);
	return tempObject;
}


void objectClass::add(objectClass *obj)
  {
    if (nextObject != NULL)
    {
      nextObject->add(obj);
    }
    else
    {
      nextObject = obj;
      obj->prevObject = this;
    }
  }





void objectClass::prepObjMoveCommand(int newTargetID,commandCaps newCmd,reportCaps  newReport,double  newXPos, double  newYPos, double  newZPos, double  newXRot, double  newYRot, double  newZRot )
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = newXPos;
	  this->yPos = newYPos;
	  this->zPos = newZPos;
	  this->xRot = newXRot;
	  this->yRot = newYRot;
	  this->zRot = newZRot;

  }
//RCJ, I commented this out to remove ambiguity with the function overloads
/*void objectClass::prepObjMoveCommandQ(int newTargetID,commandCaps newCmd,reportCaps  newReport,double  newXPos, double  newYPos, double  newZPos, double  newXRot, double  newYRot, double  newZRot, double q1, double q2, double q3, double q4 )
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = newXPos;
	  this->yPos = newYPos;
	  this->zPos = newZPos;
	  this->xRot = newXRot;
	  this->yRot = newYRot;
	  this->zRot = newZRot;
	  this->q1 = q1;
	  this->q2 = q2;
	  this->q3 = q3;
	  this->q4 = q4;

  } */  


void objectClass::prepObjectUpdateCommand(int newTargetID, reportCaps newReport)
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = noMove;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = 0.0;
	  this->yPos = 0.0;
	  this->zPos = 0.0;
	  this->xRot = 0.0;
	  this->yRot = 0.0;
	  this->zRot = 0.0;

  }





void objectClass::copyWorkingDataFrom(objectClass& source)
{
  AckIn = source.AckIn;
  objectFlag = source.objectFlag;
	AckOut = source.AckOut;
	cmdCount = source.cmdCount;
  cmdTypeIn = source.cmdTypeIn;
	cmdTypeOut = source.cmdTypeOut;
  comTypeIn = source.comTypeIn;
	comTypeOut = source.comTypeOut;
  strcpy(name, source.name);
  objID = source.objID;
  objState = source.objState;
  objType = source.objType;
  repCount = source.repCount;
  cmdCount = source.cmdCount;
  moveTime = source.moveTime;
  if(repCount > 0)
  {
    if( validReportables != NULL)
     {
        delete [] validReportables;
      }
      validReportables = new reportCaps[repCount];
      for(int i = 0; i < repCount; i++)
      {
        validReportables[i] = source.validReportables[i];
      }
    }
    reportableDataSize = source.reportableDataSize;
    if( reportableDataSize > 0)
    {
      if( reportableData != NULL)
      {
        delete [] reportableData;
      }
      reportableData = new double[reportableDataSize];
      for(int i = 0; i < reportableDataSize; i++)
      {
        reportableData[i] = source.reportableData[i];
      }
    } 
    if(cmdCount > 0)
    {
      if( validCommands != NULL)
      {
        delete [] validCommands;
      }
      validCommands = new commandCaps[cmdCount];
	  for(int i = 0; i < cmdCount; i++)
      {
        validCommands[i] = source.validCommands[i];
      }
    }

    reportTypeIn = source.reportTypeIn;
	  reportTypeOut = source.reportTypeOut;
    sourceID = source.sourceID;
    targetID = source.targetID;
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
    q1 = source.q1;
    q2 = source.q2;
    q3 = source.q3;
    q4 = source.q4;
  }
  


void objectClass::deAllocateCustomData()
{
	if(this->reportableData != NULL)
	{
		delete [] reportableData;
		reportableData = NULL;
		this->reportableDataSize = 0;
	}
}


void objectClass::prepCameraStart(int newTargetID)
{
	this->targetID = newTargetID;
	this->sourceID = this->objID;
	this->comTypeOut = commandCommunique;
	this->cmdTypeOut = track;
	this->AckOut = NACK;
	this->reportTypeOut = noReport;

	this->moveTime = 1;
	this->setMoveTime = true;

}

void objectClass::prepCameraStop(int newTargetID, double x[3])
{
	this->targetID = newTargetID;
	this->sourceID = this->objID;
	this->comTypeOut = commandCommunique;
	this->cmdTypeOut = track;
	this->AckOut = NACK;
	this->reportTypeOut = noReport;
	this->xPos = x[0];
	this->yPos = x[1];
	this->zPos = x[2];

	this->moveTime = 0;
	this->setMoveTime = true;
}


void objectClass::prepObjMoveCommandQ(int newTargetID,commandCaps newCmd,reportCaps  newReport,double  newXPos, double  newYPos, double  newZPos, double q1, double q2, double q3, double q4, double time )
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = newXPos;
	  this->yPos = newYPos;
	  this->zPos = newZPos;
	  this->q1 = q1;
	  this->q2 = q2;
	  this->q3 = q3;
	  this->q4 = q4;
	  this->setQuaterion = true;
	  this->moveTime = time;

	  this->setMoveTime = true;


  }

void objectClass::prepObjMoveCommandQ(int newTargetID,commandCaps newCmd,reportCaps  newReport,double  x[3], double q[4])
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = x[0];
	  this->yPos = x[1];
	  this->zPos = x[2];
	  this->q1 = q[0];
	  this->q2 = q[1];
	  this->q3 = q[2];
	  this->q4 = q[3];
	  this->setQuaterion = true;
	  this->moveTime = -1;

	  this->setMoveTime = false;

  }

void objectClass::prepObjMoveCommandQ(int newTargetID,commandCaps newCmd,reportCaps  newReport,double  x[3], double q[4], double time)
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = x[0];
	  this->yPos = x[1];
	  this->zPos = x[2];
	  this->q1 = q[0];
	  this->q2 = q[1];
	  this->q3 = q[2];
	  this->q4 = q[3];
	  this->setQuaterion = true;
	  this->moveTime = time;

	  this->setMoveTime = true;

  }



void objectClass::prepObjMoveCommand(int newTargetID,commandCaps newCmd,reportCaps  newReport,double x[3], double r[3])
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = x[0];
	  this->yPos = x[1];
	  this->zPos = x[2];
	  this->xRot = r[0];
	  this->yRot = r[1];
	  this->zRot = r[2];
	  this->moveTime = -1;
	  this->setQuaterion = false;

	  this->setMoveTime = false;

  }


void objectClass::prepObjMoveCommand(int newTargetID,commandCaps newCmd,reportCaps  newReport,double x[3], double r[3],double time)
  {
	  this->targetID = newTargetID;
	  this->sourceID = this->objID;
	  this->comTypeOut = commandCommunique;
	  this->cmdTypeOut = newCmd;
	  this->AckOut = NACK;
	  this->reportTypeOut = newReport;
	  this->xPos = x[0];
	  this->yPos = x[1];
	  this->zPos = x[2];
	  this->xRot = r[0];
	  this->yRot = r[1];
	  this->zRot = r[2];
	  this->moveTime = -1;
	  this->setQuaterion = false;
	  this->moveTime = time;
	  this->setMoveTime = true;

  }

/*	CMB added section	*/
void objectClass::prepMoveQ(objectClass* target,commandCaps cmd,reportCaps report,double x[3],double q[4],double time){
		//set fields for message transmission
	this->targetID = target->objID;
	this->sourceID = this->objID;
	this->comTypeOut = commandCommunique;
	this->cmdTypeOut = cmd;
	this->AckOut = NACK;
	this->reportTypeOut = report;
	this->xPos = x[0];
	this->yPos = x[1];
	this->zPos = x[2];
	this->q1 = q[0];
	this->q2 = q[1];
	this->q3 = q[2];
	this->q4 = q[3];
	this->setQuaterion = true;
	this->setMoveTime = true;
	this->moveTime = time;
}

void objectClass::prepUpdate(objectClass* target, reportCaps report){
		//set fields for message transmission
	this->targetID = target->objID;
	this->sourceID = this->objID;
	this->comTypeOut = commandCommunique;
	this->cmdTypeOut = noMove;
	this->AckOut = NACK;
	this->reportTypeOut = report;
	this->xPos = 0.0;
	this->yPos = 0.0;
	this->zPos = 0.0;
	this->xRot = 0.0;
	this->yRot = 0.0;
	this->zRot = 0.0;
}
void objectClass::clearResponseFields(){
	this->comTypeIn = nothing;
	this->cmdTypeIn = noMove;
	this->reportTypeIn = noReport;
	this->sourceID = 0;
	this->AckIn = NACK;
	this->deAllocateCustomData();
}

void objectClass::copyMsgDataFrom(const objectClass *source){

		//basic
	this->prevObject= source->prevObject;
	this->nextObject= source->nextObject;
	this->objectFlag= source->objectFlag;
	this->objID		= source->objID;
	this->objState	= source->objState;
	this->objType	= source->objType;
	this->serverSide= source->serverSide;
	this->logst		= source->logst;

		//signaling
	this->askForObjList	= source->askForObjList;
	this->reportTypeIn	= source->reportTypeIn;
	this->reportTypeOut = source->reportTypeOut;
	this->setQuaterion	= source->setQuaterion;
	this->setMoveTime	= source->setMoveTime;
	this->AckIn		= source->AckIn;
	this->AckOut	= source->AckOut;
	this->cmdCount	= source->cmdCount;
	this->cmdTypeIn = source->cmdTypeIn;
	this->cmdTypeOut= source->cmdTypeOut;
	this->comTypeIn = source->comTypeIn;
	this->comTypeOut= source->comTypeOut;
    this->sourceID	= source->sourceID;
    this->targetID	= source->targetID;
	this->repCount	= source->repCount;
	this->cmdCount	= source->cmdCount;
	
		//basic data
    this->xPos		= source->xPos;
    this->yPos		= source->yPos;
    this->zPos		= source->zPos;
    this->xRot		= source->xRot;
    this->yRot		= source->yRot;
    this->zRot		= source->zRot;
    this->q1		= source->q1;
    this->q2		= source->q2;
    this->q3		= source->q3;
    this->q4		= source->q4;
    this->grip		= source->grip;
    this->xForce	= source->xForce;
    this->yForce	= source->yForce;
    this->zForce	= source->zForce;
    this->xRotForce = source->xRotForce;
    this->yRotForce = source->yRotForce;
    this->zRotForce = source->zRotForce;
	this->moveTime	= source->moveTime;
		//dynamic data
	strcpy(this->name, source->name);

	if(this->repCount > 0){
		if(this->validReportables != NULL){
			delete [] this->validReportables;
		}
		this->validReportables = new reportCaps[this->repCount];
		for(int i = 0; i < this->repCount; i++){
			this->validReportables[i] = source->validReportables[i];
		}
	}
	this->reportableDataSize = source->reportableDataSize;
	if(this->reportableDataSize > 0){
		if(this->reportableData != NULL){
			delete [] this->reportableData;
		}
		this->reportableData = new double[this->reportableDataSize];
		for(int i = 0; i < this->reportableDataSize; i++){
			this->reportableData[i] = source->reportableData[i];
		}
	} 
	if(this->cmdCount > 0){
		if(this->validCommands != NULL){
			delete [] this->validCommands;
		}
		this->validCommands = new commandCaps[this->cmdCount];
		for(int i = 0; i < this->cmdCount; i++){
			this->validCommands[i] = source->validCommands[i];
		}
	}
}



void objectClass::copyWorkingDataFrom(objectClass *source)
{
  AckIn = source->AckIn;
  objectFlag = source->objectFlag;
	AckOut = source->AckOut;
	cmdCount = source->cmdCount;
  cmdTypeIn = source->cmdTypeIn;
	cmdTypeOut = source->cmdTypeOut;
  comTypeIn = source->comTypeIn;
	comTypeOut = source->comTypeOut;
  strcpy(name, source->name);
  objID = source->objID;
  objState = source->objState;
  objType = source->objType;
  repCount = source->repCount;
  cmdCount = source->cmdCount;
  moveTime = source->moveTime;
  if(repCount > 0)
  {
    if( validReportables != NULL)
     {
        delete [] validReportables;
      }
      validReportables = new reportCaps[repCount];
      for(int i = 0; i < repCount; i++)
      {
        validReportables[i] = source->validReportables[i];
      }
    }
    reportableDataSize = source->reportableDataSize;
    if( reportableDataSize > 0)
    {
      if( reportableData != NULL)
      {
        delete [] reportableData;
      }
      reportableData = new double[reportableDataSize];
      for(int i = 0; i < reportableDataSize; i++)
      {
        reportableData[i] = source->reportableData[i];
      }
    } 
    if(cmdCount > 0)
    {
      if( validCommands != NULL)
      {
        delete [] validCommands;
      }
      validCommands = new commandCaps[cmdCount];
	  for(int i = 0; i < cmdCount; i++)
      {
        validCommands[i] = source->validCommands[i];
      }
    }

    reportTypeIn = source->reportTypeIn;
	  reportTypeOut = source->reportTypeOut;
    sourceID = source->sourceID;
    targetID = source->targetID;
    xForce = source->xForce;
    xPos = source->xPos;
    xRot = source->xRot;
    xRotForce = source->xRotForce;
    yForce = source->yForce;
    yPos = source->yPos;
    yRot = source->yRot;
    yRotForce = source->yRotForce;
    zForce = source->zForce;
    zPos = source->zPos;
    zRot = source->zRot;
    zRotForce = source->zRotForce;
    grip = source->grip;
    q1 = source->q1;
    q2 = source->q2;
    q3 = source->q3;
    q4 = source->q4;
  }
/*	end CMB added section*/
