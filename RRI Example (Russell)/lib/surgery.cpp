///////////////////////////////////////////////////////////////////////////////
//
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        surgery.cpp
//  Revision:        1.0 - 24 March, 2010
//
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Inter-computer robot state XML parser definitions.
//
///////////////////////////////////////////////////////////////////////////////


#include "surgery.h"
#include <string.h>
#include <iostream>

using namespace std;

namespace Xml
{

	SurgeryParse::SurgeryParse (objectClass *object,
			Logger *logPtr) :
		object_(object),
		logger_(logPtr)
	{
	}


	SurgeryParse::~SurgeryParse()
	{
		object_ = NULL;
		logger_ = NULL;
	}


	//***************************************************************************
	//                       BEGIN CONTEXT-SPECIFIC CODE
	//   Modify these functions to reflect the structure of the input XML code
	//***************************************************************************

	/*
	   <Mail Cmd="Status">
	   <Mail Cmd="Move" X="1.1" Y="1.1" Z="1.1" Roll="1.1" Pitch="1.1" Yaw="1.1"/>
	   <Mail Cmd="Quit">
	   */
	/*int SurgeryParse::lock()
	{
		objectClass *tempObject
		pthread_mutex_unlock(&object_->objectMutex)


		return 0;
	} */

	bool SurgeryParse::startElement (const string& tagName, 
			const xmlAttributes& attr)
	{
		bool flag = true;
		std::vector<std::string>::const_iterator nameiter = attr.name.begin();
		std::vector<std::string>::const_iterator valiter = attr.val.begin();
		//objectClass *tempObject; 
		//tempObject = object_;
		//If the object is a master, and the source was an observer or effector, go to the right spot for the signal decoding
		//
			if(tempObject == NULL){
				
			
			//Grab_Mutex
			if(object_->objType == master && object_->sourceID > 0 && object_->serverSide != 1)
			{
				tempObject = object_;
				while(tempObject != NULL) 
				{
					if( tempObject->objID == object_->sourceID )
					{
						addMutex();
						break;
					}
					tempObject = tempObject->nextObject;
				}
				
			} //if(object_->objType == master && object_->sourceID != 0 && object_->serverSide != 1)
			} //if(tempObject == NULL)

	    //RCJ and Connor attempted this to clean the mutex locking, Removing
		//for now.
		//	pthread_mutex_lock(&object_->objectMutex);
	    //  pthread_mutex_lock(&tempObject->objectMutex);
		
		try
		{

			//! Object Header...
			//Since the first component of the command tells the parser what the command is,
			//the rest of the code will reflect this process.

			// emy / this will break the functionality of what I've done so far
			//		let's avoid having <object> be a root node
			/*   if (strcmp (tagName.c_str(), "object") == 0)
			     {
			     object_->comTypeIn = identifier;
			     for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
			     {
			     pthread_mutex_lock(&object_->objectMutex);
			     if (strcmp (nameiter->c_str(), "type") == 0)
			     {
			     object_->objType = (objectTypes) atoi(valiter->data());
			     }

			     if (strcmp (nameiter->c_str(), "id") == 0)
			     {
			     object_->objID = atoi(valiter->data());
			     }
			     if (strcmp (nameiter->c_str(), "name") == 0)
			     {
			     sprintf(object_->name,valiter->data());
			     }
			     pthread_mutex_unlock(&object_->objectMutex);
			     } // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
			     } // if (strcmp (tagName.c_str(), "object") == 0) */ 
			//RCJ--Add the command communique and update communique selection here.
			//Command communique.
			if (strcmp( tagName.c_str(), "ack") == 0)
			{
				//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
#ifdef DEBUG
				cout << "surgery::parse: server ack recieved.\n";
#endif 
				object_->comTypeIn = ack;
				//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
			}
			/* else */if (strcmp (tagName.c_str(), "command") == 0)
			{
				//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
				object_->comTypeIn = commandCommunique;
				for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				{
					if (strcmp (nameiter->c_str(), "source") == 0)
					{
						object_->sourceID = atoi(valiter->data());
					} else 
					if (strcmp (nameiter->c_str(), "target") == 0)
					{
						object_->targetID = atoi(valiter->data());
					} else 
					if (strcmp (nameiter->c_str(), "dest") == 0)
					{
						object_->targetID = atoi(valiter->data());
					}
					object_->cmdTypeIn = noMove;
					object_->reportTypeIn = noReport;
				} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
			} // if (strcmp (tagName.c_str(), "command") == 0)

			//Update selection
			else if (strcmp (tagName.c_str(), "update") == 0)
			{
				object_->comTypeIn = updateCommunique;
				for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					if (strcmp (nameiter->c_str(), "source") == 0)
					{
						object_->sourceID = atoi(valiter->data());

					}
					if (strcmp (nameiter->c_str(), "target") == 0)
					{
						object_->targetID = atoi(valiter->data());
					}
					
					//If the object is a master, and the source was an observer or effector, go to the right spot for the signal decoding
					//
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
			} // if (strcmp (tagName.c_str(), "update") == 0)
			//This completes the processing of the identifier.

			if(object_->comTypeIn == identifier)
			{

				if (strcmp (tagName.c_str(), "position") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "X") == 0)
						{
							object_->xPos = atof (valiter->c_str ());
						}
						if (strcmp (nameiter->c_str(), "Y") == 0)
						{
							object_->yPos = atof (valiter->c_str ());
						}
						if (strcmp (nameiter->c_str(), "Z") == 0)
						{
							object_->zPos = atof (valiter->c_str ());
						}
						if (strcmp (nameiter->c_str(), "Roll") == 0)
						{
							object_->xRot = atof (valiter->c_str ());
						}
						if (strcmp (nameiter->c_str(), "Pitch") == 0)
						{
							object_->yPos = atof (valiter->c_str ());
						}
						if (strcmp (nameiter->c_str(), "Yaw") == 0)
						{
							object_->yPos = atof (valiter->c_str ());
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} // (strcmp (tagName.c_str(), "position") == 0)



				if (strcmp (tagName.c_str(), "cmd") == 0)
				{

					commandCaps* tempCmd = NULL;
					int tempCmdCount=0;
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if(strcmp(nameiter->c_str(),"count") == 0)
						{
							tempCmdCount = atoi(valiter->c_str());
							if(tempCmdCount > 0)
							{
								tempCmd = new commandCaps[tempCmdCount];
							}
						}
						else if(tempCmdCount > 0)
						{
							int i = atoi(nameiter->c_str());
							tempCmd[i] = (commandCaps) atoi(valiter->c_str());
						}
					}
					if(tempCmdCount > 0)
					{
						//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
						object_->allocateCommands(tempCmdCount,tempCmd);
						delete tempCmd;
						//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
					}
				}

				if (strcmp (tagName.c_str(), "report") == 0)
				{
					reportCaps* tempRpt = NULL;
					int tempRptCount=0;
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if(strcmp(nameiter->c_str(),"count") == 0)
						{
							tempRptCount = atoi(valiter->c_str());
							if(tempRptCount > 0)
							{
								tempRpt = new reportCaps[tempRptCount];
							}
						}
						else if(tempRptCount > 0)
						{
							int i = atoi(nameiter->c_str());
							tempRpt[i] = (reportCaps) atoi(valiter->c_str());
						}
					}
					if(tempRptCount > 0)
					{
						//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
						object_->allocateReportables(tempRptCount,tempRpt);
						delete tempRpt;
						//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
					}
				} 
			}// if(object_->comType == identifier)
			//This completes the header parsing
			else if (object_->comTypeIn == commandCommunique)
			{
				//RCJ-- Add translation code here
				//RCJ--THe update code is NOT DONE!!!!
				if (strcmp (tagName.c_str(), "move") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "type") == 0)
						{
							object_->cmdTypeIn = (commandCaps) atoi (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "X") == 0)
						{
							object_->xPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Y") == 0)
						{
							object_->yPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Z") == 0)
						{
							object_->zPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Roll") == 0)
						{
							object_->xRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Pitch") == 0)
						{
							object_->yRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Yaw") == 0)
						{
							object_->zRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "q1") == 0)
						{
							object_->q1 = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "q2") == 0)
						{
							object_->q2 = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "q3") == 0)
						{
							object_->q3 = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "q4") == 0)
						{
							object_->q4 = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Time") == 0)
						{
							object_->moveTime = atof (valiter->c_str());
						} 
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} // if (strcmp (tagName.c_str(), "Move") == 0)
				else if (strcmp(tagName.c_str(), "log") == 0) {
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter) {
						if (!strcmp(nameiter->c_str(), "state")) {
							object_->logst = (atoi(valiter->c_str())) ? true : false;
						}
					}
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} else 
				if (strcmp (tagName.c_str(), "report") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "type") == 0){
							object_->reportTypeIn = (reportCaps) atoi (valiter->c_str ());
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} // if (strcmp (tagName.c_str(), "report") == 0)
				if (strcmp (tagName.c_str(), "kill") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					object_->objID = -2;	
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				}//(strcmp (tagName.c_str(), "kill") == 0)
				if (strcmp (tagName.c_str(), "Ack") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "val") == 0)
						{
							object_->AckIn = (atoi (valiter->c_str ()) == 0) ? NACK : ACK;
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				} // if (strcmp (tagName.c_str(), "Ack") == 0)
			} // else if (object_->comType == commandCommunique)
			else if(object_->comTypeIn == updateCommunique)
			{
				if(tempObject != NULL && object_->objType == master){
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					//perform the switch here
					
					
					tempObject->comTypeIn = updateCommunique;
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				}
				/*if (strcmp (tagName.c_str(), "report") == 0)
				{
					pthread_mutex_lock(&tempObject->objectMutex);
					for(;nameiter != attr.name.end(); ++nameiter,++valiter)
					{
						if (strcmp (nameiter->c_str(),"type") == 0)
						{
							tempObject->reportTypeIn = atoi (valiter->c_str());
						}
					}
					pthread_mutex_unlock(&tempObject->objectMutex);
				}*/
				if (strcmp (tagName.c_str(), "position") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "X") == 0)
						{
							tempObject->xPos =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Y") == 0)
						{
							tempObject->yPos =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Z") == 0)
						{
							tempObject->zPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Roll") == 0)
						{
							tempObject->xRot =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Pitch") == 0)
						{
							tempObject->yRot =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Yaw") == 0)
						{
							tempObject->zRot =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Grip") == 0)
						{
							tempObject->grip = (gripState)(atoi (valiter->c_str ()));
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				} // if (strcmp (tagName.c_str(), "Position") == 0)
				if (strcmp (tagName.c_str(), "forces") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "X") == 0)
						{
							tempObject->xForce =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Y") == 0)
						{
							tempObject->yForce =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Z") == 0)
						{
							tempObject->zForce = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Roll") == 0)
						{
							tempObject->xRotForce =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Pitch") == 0)
						{
							tempObject->yRotForce =  atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Yaw") == 0)
						{
							tempObject->zRotForce =  atof (valiter->c_str ());
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				} // if (strcmp (tagName.c_str(), "forces") == 0)
				if (strcmp (tagName.c_str(), "custom") == 0)
				{
					double* tempData = NULL;
					int tempDataCount = 0;
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if(strcmp (nameiter->c_str(), "count") == 0)
						{
							tempDataCount = atoi(valiter->c_str());
							if(tempDataCount > 0)
							{
								tempData = new double[tempDataCount];
							}

						}				
						else if(tempData != NULL)
						{
							// emy: +1 because all the tag names are c1, c2; skip the "c"
							//Added to git by RCJ.
							int i;
							if (*(nameiter->c_str()) == 'c')
							{
								i = atoi(nameiter->c_str() + 1);
							}
							else
							{
								i = atoi(nameiter->c_str());
							}
							tempData[i] = atof(valiter->c_str());
							//int i = atoi(nameiter->c_str());
							//tempData[i] = atof(valiter->c_str());
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					tempObject->allocateCustomData(tempDataCount,tempData);
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				} // if (strcmp (tagName.c_str(), "custom") == 0)
				if (strcmp (tagName.c_str(), "Ack") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "val") == 0)
						{
							tempObject->AckIn = (atoi (valiter->c_str ()) == 0) ? NACK : ACK;
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				} // if (strcmp (tagName.c_str(), "Ack") == 0)
				if (strcmp (tagName.c_str(), "state") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "val") == 0)
						{
							tempObject->objState = (currState)  atoi (valiter->c_str ());
						}
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
				} // if (strcmp (tagName.c_str(), "state") == 0)
				if(strcmp (tagName.c_str(),"objlist") == 0)
				{
					if(object_->sourceID == 0 && object_->objType == master)
					{
						if(1) //emy/ignore acks for now
						{
							//!This command might need to be added more gracefully 
							//!to allow for the addion of an object list without forgetting the state of all the other objects.
							//Can this get hit?
							object_->cleanObjectList();
							tempObject = NULL;
						}
					}
				}
				if (strcmp (tagName.c_str(), "you") == 0) //emy/this is how the object will learn its own ID.
					// a simple <update><you id="x"/></update> will do
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp(nameiter->c_str(), "id") == 0)
						{
							object_->objID = atoi(valiter->c_str());
						}
					}
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);
				}
				if (strcmp (tagName.c_str(), "obj") == 0)
				{
					//RCJ!!!!-pthread_mutex_lock(&object_->objectMutex);
					//tempObject = NULL;
					for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
					{
						if (strcmp (nameiter->c_str(), "id") == 0)
						{
							int objID = atoi (valiter->c_str ());
							/*	  if(objID == 0 || objID == object_->objID)
								  {
							//!RCJ The server does not need to be stored as an Object
							break;
							} */ // emy/we don't need this check anymore, the server doesn't send these
							tempObject = object_;
							//!Add in new previously unseen object
							//!what happens if an object is lost.
							//!only add objects if the current object is a master.
							if(object_->objType != master)
							{
								break;
							} 
							//! it is easiest to go back and search from the first to the last object.
							while(tempObject->prevObject !=  NULL)
							{
								tempObject = tempObject->prevObject;
							}
							objectClass* tempObject2 = tempObject->objectInsertBehind();
							tempObject = tempObject2;
							tempObject->objID = objID;
							/*	  }
								  while(objID > tempObject->objID && tempObject->nextObject !=NULL )
								  {
								  if(objID < tempObject->nextObject->objID)
								  {

								  objectClass* tempObject2 = tempObject->objectInsertBehind();
								  tempObject = tempObject2;
								  tempObject->objID = objID;
								  break;
								  }
								  tempObject = tempObject->nextObject;
								  if(tempObject->nextObject = NULL)
								  {
								  break;
								  }
								  } // while(objID > tempObject->objID)
								  if(objID > tempObject->objID)
								  {
								  objectClass* tempObject2 = tempObject->objectInsertBehind();
								  tempObject = tempObject2;
								  tempObject->objID = objID;
							//break;
							} // if(objID > tempObject->objID) */
						} // if (strcmp (nameiter->c_str(), "ID") == 0)
						//!At this point the object has been defined and allocated.
						//! if the source was the object in the object list, then the 
						//data here can simply be copied over from the beginning of the updateCommunique
						if(object_->sourceID == tempObject->objID)
						{
							//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
							tempObject->grip = object_->grip;
							//! position Info
							tempObject->xPos = object_->xPos;
							tempObject->yPos = object_->yPos;
							tempObject->zPos = object_->zPos;
							tempObject->xRot = object_->xRot;
							tempObject->yRot = object_->yRot;
							tempObject->zRot = object_->zRot;
							//! quaternion copy?
							cout << "quaternion copy " << object_->q1 << endl;
							tempObject->q1 = object_->q1;
							tempObject->q2 = object_->q2;
							tempObject->q3 = object_->q3;
							tempObject->q4 = object_->q4;
							//! force info
							tempObject->xForce = object_->xForce;
							tempObject->yForce = object_->yForce;
							tempObject->zForce = object_->zForce;
							tempObject->xRotForce = object_->xRotForce;
							tempObject->yRotForce = object_->yRotForce;
							tempObject->zRotForce = object_->zRotForce;
							//! Custom data
							tempObject->reportableDataSize = object_->reportableDataSize;
							if(object_->reportableData != NULL && tempObject->reportableDataSize > 0)
							{
								if(tempObject->reportableData != NULL)
								{
									delete [] tempObject->reportableData;
									tempObject->reportableData = NULL;
								}
								tempObject->reportableData = new double[tempObject->reportableDataSize];
								for(int i = 0; i < tempObject->reportableDataSize; i++) 
								{
									tempObject ->reportableData[i] = object_->reportableData[i];
								}
							}
							//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
							break;
						} //if(object_->sourceID == tempObject->objID)
						//RCJ!!!!-pthread_mutex_lock(&tempObject->objectMutex);
						if (strcmp (nameiter->c_str(), "type") == 0)
						{
							tempObject->objType =  get_type_enum(valiter->c_str ());
							tempObject->objTypeStr = strdup( valiter->c_str() );
						} else 
						if (strcmp (nameiter->c_str(), "X") == 0)
						{
							tempObject->xPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Y") == 0)
						{
							tempObject->yPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Z") == 0)
						{
							tempObject->zPos = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Roll") == 0)
						{
							tempObject->xRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Pitch") == 0)
						{
							tempObject->yRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "Yaw") == 0)
						{
							tempObject->zRot = atof (valiter->c_str ());
						} else 
						if (strcmp (nameiter->c_str(), "name") == 0)
						{
							sprintf(tempObject->name,"%s", valiter->data());
						}
						//RCJ!!!!-pthread_mutex_unlock(&tempObject->objectMutex);
					} // for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				/*if(tempObject != NULL)  //! The master object's mutex acts as a mutex for the entire list on the master side
				  {
				  pthread_mutex_unlock(&tempObject->objectMutex);
				  }*/
					//RCJ!!!!-pthread_mutex_unlock(&object_->objectMutex);

				} // if (strcmp (tagName.c_str(), "obj") == 0)
		} // else if(object_->comType == updateCommunique)
	} // try
	catch (...)
	{
		exception ( "XML:ABB startElement", "Unknown error");
		flag = false;
	}
	//pthread_mutex_unlock(&object_->objectMutex);
	//pthread_mutex_unlock(&tempObject->objectMutex);
	return flag;
}


bool SurgeryParse::endElement(const string& )
{
	/*
	   if (tagName == "RobData")
	   {
	   }
	   else
	   {
	//! Unknown/unsupported XML tag
	return false;
	}
	*/
	return true;
}


bool SurgeryParse::encode (char *line)
{
	if (object_ != NULL)
	{
		pthread_mutex_lock(&object_->objectMutex);
		int charCount;
		objectClass* objPointer;
		switch (object_->comTypeOut)
		{
			case announce:
				charCount = sprintf(line, "<object seq=\"announce\"><type>%s</type><name>%s</name></object>", 
						object_->objTypeStr, 
						object_->name);
				break;
			case identifier:
				charCount = sprintf (line, "<object type=\"%d\" ID=\"%d\" name=\"%s\"><position X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\"/>",
						object_->objType,
						object_->objID,
						object_->name,
						object_->xPos,
						object_->yPos,
						object_->zPos,
						object_->xRot,
						object_->yRot,
						object_->zRot);
				if(object_->cmdCount > 0)
				{
					charCount += sprintf(line+charCount,"<cmd count = \"%d\" ",object_->cmdCount);
					for(int i = 0; i < object_->cmdCount; i++)
					{
						charCount += sprintf(line+charCount,"%d =\"%d\" ", i,object_->validCommands[i]);
					}
					charCount += sprintf(line+charCount,"/>");
				}
				if(object_->repCount > 0)
				{
					charCount += sprintf(line+charCount,"<report ");
					charCount += sprintf(line+charCount,"count = \"%d\" ",object_->repCount);
					for (int i = 0; i < object_->repCount; i++)
					{
						charCount += sprintf(line+charCount,"%d =\"%d\" ", i,object_->validReportables[i]);
					}
					charCount += sprintf(line+charCount,"/>");
				}
				charCount += sprintf(line+charCount,"</object>\r");
				break;
			case updateCommunique:
				//!the force information may be added here.

				if (object_->askForObjList)
				{
					charCount = sprintf(line, "<update dest=\"0\" type=\"list\"/>");
					break;
				}
				charCount = sprintf (line, "<update source=\"%d\" target=\"%d\">",
						object_->sourceID,
						object_->targetID);
				charCount += sprintf(line+charCount,"<Ack val=\"%d\"/>",object_->AckOut);
				if(object_->objType != -1)
				{
					charCount += sprintf (line+charCount, "<position X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\" Grip= \"%d\" q1=\"%f\" q2=\"%f\" q3=\"%f\" q4=\"%f\"/><forces X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\"/>",      
							object_->xPos,
							object_->yPos,
							object_->zPos,
							object_->xRot,
							object_->yRot,
							object_->zRot,
							object_->grip,
							object_->q1,
							object_->q2,
							object_->q3,
							object_->q4,
							object_->xForce,
							object_->yForce,
							object_->zForce,
							object_->xRotForce,
							object_->yRotForce,
							object_->zRotForce);
					// cout << line << endl;
					// need to add quaternion here?
					// why force is not reported here?
					if(object_->reportableDataSize > 0 && object_->reportableData != NULL)
					{
						charCount += sprintf(line+charCount,"<custom count=\"%d\" ", object_->reportableDataSize);
						for(int i = 0; i < object_->reportableDataSize; i++)
						{
							charCount += sprintf(line+charCount,"c%d=\"%f\" ",i,object_->reportableData[i]);
						}
						charCount += sprintf(line+charCount,"/>");
					}
					charCount += sprintf(line+charCount,"<state val=\"%d\"/>",object_->objState);
				}
				else
				{
					charCount += sprintf(line+charCount,"<objlist>");
					objPointer = object_;
					for(;objPointer->prevObject != NULL; objPointer = objPointer->prevObject)
					{
						//! Do nothing.  Just looking for the correct object.
					}

					for(; objPointer != NULL; objPointer = objPointer->nextObject)
					{
						if(objPointer->objID != 0)
							charCount += sprintf (line+charCount, "<obj ID=\"%d\" type=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\" name = \"%s\"/>",
									objPointer->objID,
									objPointer->objType,
									objPointer->xPos,
									objPointer->yPos,
									objPointer->zPos,
									objPointer->xRot,
									objPointer->yRot,
									objPointer->zRot,
									objPointer->name);
					}	
					charCount += sprintf (line+charCount, "</objlist>");
				}
				charCount += sprintf (line+charCount, "</update>\r");
				break;
			case commandCommunique:
				if (object_->cmdTypeOut == disconnect)
				{
					charCount = sprintf(line, "<detach/>");
					break;
				}
				if (object_->reportTypeOut == state && object_->targetID == 0)
				{
					charCount = sprintf(line, "<update dest=\"%d\"/>", object_->targetID );
					break;
				}

				charCount = sprintf (line, "<command source=\"%d\" dest=\"%d\">",
						object_->sourceID,
						object_->targetID);
				if (object_->cmdTypeOut != noMove)
				{
					if (object_->cmdTypeOut == qMove)
					{
						charCount += sprintf (line+charCount, "<move type=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\" q1=\"%f\" q2=\"%f\" q3=\"%f\" q4=\"%f\" Time=\"%f\" ",
								object_->cmdTypeOut,
								object_->xPos,
								object_->yPos,
								object_->zPos,
								object_->xRot, //To be commented?
								object_->yRot, //To be commented?
								object_->zRot, //To be commented?
								object_->q1,
								object_->q2,
								object_->q3,
								object_->q4,
								object_->setMoveTime ?
									object_->moveTime : 0.0f);

					}
					else if (object_->cmdTypeOut == dLog)
					{
						charCount += sprintf(line+charCount, "<move type=\"%d\" /><log state=\"%d\" ", 
								object_->cmdTypeOut,
									object_->logst ? 
								beginLog : endLog);
					} else {
						charCount += sprintf (line+charCount, "<move type=\"%d\" X=\"%f\" Y=\"%f\" Z=\"%f\" Roll=\"%f\" Pitch=\"%f\" Yaw=\"%f\" Time=\"%f\" ",
								object_->cmdTypeOut,
								object_->xPos,
								object_->yPos,
								object_->zPos,
								object_->xRot,
								object_->yRot,
								object_->zRot,
								object_->setMoveTime ? 
									object_->moveTime : 0);
					}

					//
					// add additional attributes here
					// 

					// close off the command tag
					charCount += sprintf(line + charCount, "/>");
				}
			//	if (object_->reportTypeOut != noReport)
			//	{
					charCount += sprintf (line+charCount, "<report type=\"%d\"/>",
							object_->reportTypeOut);
			//	}
				charCount += sprintf (line+charCount, "<Ack val=\"%d\"/></command>\r",
						object_->AckOut);

				break;
			default:
				sprintf(line,"</>");
				break;

		}
		object_->setMoveTime = false;
		pthread_mutex_unlock(&object_->objectMutex);
	}
	return true;
}


void SurgeryParse::exception (const char *where, const char *what)
{
	static char message[1024];
	if (logger_ != NULL)
	{
		sprintf (message, "Exception in \" %s \" : %s", where, what);
		logger_->error (message);
	}
}

objectTypes SurgeryParse::get_type_enum( const char *type )
{
	if ( !strncmp(type, "master", 6) )
		return master;
	if ( !strncmp(type, "effector", 8) )
		return effector;
	if ( !strncmp(type, "observer", 8) )
		return observer;
	return NAType;
}



int SurgeryParse::grabMutex(int* count){
	
	++count[0];
	object_->sourceID = -1;
	tempObject = NULL;
	pthread_mutex_lock(&object_->objectMutex);
	altMutex = 0;
	return 0;
}
			

int SurgeryParse::releaseMutex(){

	if(altMutex == 1){
		pthread_mutex_unlock(&tempObject->objectMutex);
		tempObject = NULL;
	}
	pthread_mutex_unlock(&object_->objectMutex);
	//pthread_mutex_unlock(&object_->objectMutex);
	altMutex = 0;
	return 0;
}


int SurgeryParse::addMutex(){

	if(altMutex == 0)
	{
		pthread_mutex_lock(&tempObject->objectMutex);
		altMutex = 1;
	}
	pthread_mutex_unlock(&object_->objectMutex);

	return 0;
}
}
