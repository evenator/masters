///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Neural Tissue
//  Subsystem:       XML
//  Workfile:        abb2.cpp
//  Revision:        1.0 - 24 March, 2010
//
//  Author:          J. Marvel
//
//  Description
//  ===========
//  ABB GSI XML parser definitions.
//
///////////////////////////////////////////////////////////////////////////////


#include "abb2.h"

using namespace std;

namespace Xml
{

	ABB2Parse::ABB2Parse (robotStruct *robot,
			Logger *logPtr,
			robotStruct *target) :
		robot_(robot),
		logger_(logPtr),
		target_(target)
	{
	}


	ABB2Parse::~ABB2Parse()
	{
		robot_ = NULL;
		logger_ = NULL;
		target_ = NULL;
	}



	//***************************************************************************
	//                       BEGIN CONTEXT-SPECIFIC CODE
	//   Modify these functions to reflect the structure of the input XML code
	//***************************************************************************

	//! <RobData Id="111" Ts="1.202">
	//!   <RobMode>Auto</RobMode>
	//!   <Ts_act>1.200</Ts_act>
	//!   <P_act X="1620.0" Y="1620.0" Z="1620.0" Rx="100.0" Ry="100.0" Rz="100.0" />
	//!   <J_act J1="1.0" J2="1.0" J3="1.0" J4="1.0" J5="1.0" J6="1.0" />
	//!   <Ts_des>1.200</Ts_des>
	//!   <P_des X="1620.0" Y="1620.0" Z="1620.0" Rx="100.0" Ry="100.0" Rz="100.0" />
	//!   <J_des J1="1.0" J2="1.0" J3="1.0" J4="1.0" J5="1.0" J6="1.0" />
	//!   <AppData X1="1" X2="1620.000" X3="1620.000" X4="1620.000" X5="1620.000"
	//!     X6="1620.000" X7="1620.000" X8="1620.000" X9="1620.000" X10="1620.000"
	//!     X11="1620.000" X12="1620.000" X13="1620.000" X14="1620.000"
	//!     X15="1620.000" X16="1620.000" X17="1620.000" X18="1620.000" />
	//! </RobData>


	bool ABB2Parse::startElement (const string& tagName, 
			const xmlAttributes& attr)
	{
		bool flag = true;
		std::vector<std::string>::const_iterator nameiter = attr.name.begin();
		std::vector<std::string>::const_iterator valiter = attr.val.begin();

		try
		{
			//! Robot request for information
			if (strcmp (tagName.c_str(), "RobData") == 0)
			{
				for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				{
					//RCJ!!!!-pthread_mutex_lock(&robot_->robotMutex);
					//! <RobData Id="111" Ts="1.202">
					if (strcmp(nameiter->c_str(), "Id") == 0)
					{
						robot_->seq = target_->seq = atoi(valiter->c_str());

						// printf("Message ID: %d\n", robot_->seq);
					}
					if (strcmp(nameiter->c_str(), "Ts") == 0)
					{
						robot_->ts = target_->ts = atof(valiter->c_str());
						robot_->sysTimestamp = target_->sysTimestamp = getCurrentTime(); //conflict part exist on abb machine
						// printf("Message Ts: %f\n", robot_->ts);
					}
					//RCJ!!!!-pthread_mutex_unlock(&robot_->robotMutex);
				}
			}

			if (strcmp (tagName.c_str(), "AppData") == 0)
			{
				for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				{
					//RCJ!!!!-pthread_mutex_lock(&robot_->robotMutex);
					//if (strcmp(nameiter->c_str(), "X1") == 0) // bpasu : force report from NacReadForeeGSI to fcreadforce (slower but q support)
					if (strcmp(nameiter->c_str(), "X13") == 0)
					{
						robot_->fx = atof(valiter->c_str());
					}
					//if (strcmp(nameiter->c_str(), "X2") == 0) // bpasu
					if (strcmp(nameiter->c_str(), "X14") == 0)
					{
						robot_->fy = atof(valiter->c_str());
					}
					//if (strcmp(nameiter->c_str(), "X3") == 0) // bpasu
					if (strcmp(nameiter->c_str(), "X15") == 0)
					{
						robot_->fz = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X4") == 0)
					{
						robot_->frx = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X5") == 0)
					{
						robot_->fry = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X6") == 0)
					{
						robot_->frz = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X7") == 0)
					{
						//robot_->flag = atoi(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X9") == 0)
					{
						robot_->q1 = atof(valiter->c_str());
					// 	cout << "q1 = " << robot_->q1 << endl;
					}
					if (strcmp(nameiter->c_str(), "X10") == 0)
					{
						robot_->q2 = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X11") == 0)
					{
						robot_->q3 = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X12") == 0)
					{
						robot_->q4 = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X19") == 0)
					{
						robot_->ackflag1 = atoi(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "X20") == 0)
					{
						robot_->ackflag2 = atoi(valiter->c_str());
					}

					//RCJ!!!!-pthread_mutex_unlock(&robot_->robotMutex);
				}
			}

			//! <P_act X="-478.12" Y="344.992" Z="531.579" Rx="1.42955"
			//!  Ry="-0.00886112" Rz="1.57589"/>
			if (strcmp (tagName.c_str(), "P_act") == 0)
			{
				for (; nameiter != attr.name.end(); ++nameiter, ++valiter)
				{
					//RCJ!!!!-pthread_mutex_lock(&robot_->robotMutex);
					if (strcmp(nameiter->c_str(), "X") == 0)
					{
						robot_->x = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "Y") == 0)
					{
						robot_->y = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "Z") == 0)
					{
						robot_->z = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "Rx") == 0)
					{
						robot_->rx = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "Ry") == 0)
					{
						robot_->ry = atof(valiter->c_str());
					}
					if (strcmp(nameiter->c_str(), "Rz") == 0)
					{
						robot_->rz = atof(valiter->c_str());
					}
					//RCJ!!!!-pthread_mutex_unlock(&robot_->robotMutex);
				}
			}
		} // try
		catch (...)
		{
			exception ("XML:ABB startElement", "Unknown error");
			flag = false;
		}

		return flag;
	}


	bool ABB2Parse::endElement (const string&/* tagName */)
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


	bool ABB2Parse::encode (char *line)
	{
		//! preset declarations - debugging purposes only
		const int move = 2, nomove = 0;// killmove = -1;
		// const int modeId = 2, moveId = 2; //! linear move in AC
		 // const double xforce = 0.0, yforce = 0.0,
		//       xdisp = 0.0, ydisp = 0.0, zdisp = 50.0;
		// const double posz = 10.0, negz = -10.0;
		// const double timeout = 20.0;
		// static int count = 0, lastcom = 0;
		int msgL = 0;
		//target_->seq++;

		double timestamp = getCurrentTime();
		timestamp = timestamp - target_->sysTimestamp + target_->ts;

		if (target_ == NULL)
		{
			//! Don't move
			pthread_mutex_lock(&target_->robotMutex);
			msgL = sprintf(line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%d\" X4=\"%d\" X5=\"%d\" X6=\"%d\" X7=\"%d\" X8=\"%d\" X9=\"%d\" X10=\"%d\" X11=\"%d\" X12=\"%d\" X13=\"%d\" X14=\"%d\" X15=\"%d\" X16=\"%d\" X17=\"%d\" X18=\"%d\" X19=\"%d\" X20=\"%d\"/></SensData>",
					target_->seq, timestamp,
					nomove, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
			pthread_mutex_unlock(&target_->robotMutex);
			return true;
		}

		try
		{
			pthread_mutex_lock(&target_->robotMutex);
			if (target_->flag == FLAG_QUIT)
			{
				
				//! Signal ABB controller to quit
				//pthread_mutex_lock(&target_->robotMutex);
				msgL = sprintf(line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%d\" X4=\"%d\" X5=\"%d\" X6=\"%d\" X7=\"%d\" X8=\"%d\" X9=\"%d\" X10=\"%d\" X11=\"%d\" X12=\"%d\" X13=\"%d\" X14=\"%d\" X15=\"%d\" X16=\"%d\" X17=\"%d\" X18=\"%d\" X19=\"%d\" X20=\"%d\"/></SensData>",
						target_->seq, timestamp,
						3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
				//pthread_mutex_unlock(&target_->robotMutex);
				//return true;
			}
			else if (target_->flag == FLAG_NEWCOMMAND)
			{
				//! {PositionControl = 0, ForceControl = 1, Rotation = 2, NoMethod = 3};
				switch (target_->method)
				{
					case JointControl:
						//Joint-controlled move
						//Fall through

					case PositionControl:
						//! Position-controlled Cartesian move
						//! Fall-through
					case Rotation:
						//! Position-controlled Cartesian rotation
						//! Fall-through
					case guidedRobMotion:
						//! Hitting the table
						//! Fall-through
					case startRobPos:        
						//! Goto starting position.
						//! Fall-through			
					case ForceControl:
						//! Force-controlled Cartesian linear move
						msgL  = sprintf (line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%f\" X4=\"%f\" X5=\"%f\" X6=\"%f\" X7=\"%f\" X8=\"%f\" X9=\"%f\" X10=\"%f\" X11=\"%f\" X12=\"%f\" X13=\"%f\" X14=\"%f\" X15=\"%f\" X16=\"%f\" X17=\"%f\" X18=\"%f\" X19=\"%f\" X20=\"%f\"/></SensData>",
								target_->seq,    //! Id
								timestamp,       //! Ts
								move,            //! Issue move command
								target_->method, //! P.C. move/rotation, or F.C. linear move
								target_->x,      //! Absolute X coordinate
								target_->y,      //! Absolute Y coordinate
								target_->z,      //! Absolute Z coordinate
								target_->rx,     //! Absolute X axis rotation
								target_->ry,     //! Absolute Y axis rotation
								target_->rz,     //! Absolute Z axis rotation
								0.0f,            //! 9
								0.0f,            //! 10
								0.0f,            //! 11
								0.0f,            //! 12
								0.0f,            //! 13
								0.0f,            //! 14
								0.0f,            //! 15
								0.0f,            //! 16
								0.0f,            //! 17
								0.0f,            //! 18
								0.0f,            //! 19
								0.0f);           //! 20
						break;	
					case QuaternionControl: //! These are robot commands
						msgL = sprintf (line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%f\" X4=\"%f\" X5=\"%f\" X6=\"%f\" X7=\"%f\" X8=\"%f\" X9=\"%f\" X10=\"%f\" X11=\"%f\" X12=\"%f\" X13=\"%f\" X14=\"%f\" X15=\"%f\" X16=\"%f\" X17=\"%f\" X18=\"%f\" X19=\"%f\" X20=\"%f\"/></SensData>",
								target_->seq,    //! Id
								timestamp,       //! Ts
								move,            //! Issue move command
								target_->method, //! P.C. move/rotation, or F.C. linear move
								target_->x,      //! Absolute X coordinate
								target_->y,      //! Absolute Y coordinate
								target_->z,      //! Absolute Z coordinate
								target_->rx,     //! Absolute X axis rotation
								target_->ry,     //! Absolute Y axis rotation
								target_->rz,     //! Absolute Z axis rotation
								target_->q1,     //! Absolute Q1
								target_->q2,     //! Absolute Q2
								target_->q3,     //! Absolute Q3
								target_->q4,     //! Absolute Q4
								target_->moveTime,//! 13
								0.0f,            //! 14
								0.0f,            //! 15
								0.0f,            //! 16
								0.0f,            //! 17
								0.0f,            //! 18
								0.0f,            //! 19
								0.0f);           //! 20
						break;
					case Gripper:
						//! Gripper open/close
						msgL = sprintf (line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%f\" X4=\"%f\" X5=\"%f\" X6=\"%f\" X7=\"%f\" X8=\"%f\" X9=\"%f\" X10=\"%f\" X11=\"%f\" X12=\"%f\" X13=\"%f\" X14=\"%f\" X15=\"%f\" X16=\"%f\" X17=\"%f\" X18=\"%f\" X19=\"%f\" X20=\"%f\"/></SensData>",
								target_->seq,    //! Id
								timestamp,       //! Ts
								move,          //! Issue move command
								target_->method, //! Gripper command
								target_->x,      //! Open/closed
								0.0f,            //! 4
								0.0f,            //! 5
								0.0f,            //! 6
								0.0f,            //! 7
								0.0f,            //! 8
								0.0f,            //! 9
								0.0f,            //! 10
								0.0f,            //! 11
								0.0f,            //! 12
								0.0f,            //! 13
								0.0f,            //! 14
								0.0f,            //! 15
								0.0f,            //! 16
								0.0f,            //! 17
								0.0f,            //! 18
								0.0f,            //! 19
								0.0f);           //! 20
						break;
					case Compliance:
						//! Gripper open/close
						sprintf (line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%f\" X4=\"%f\" X5=\"%f\" X6=\"%f\" X7=\"%f\" X8=\"%f\" X9=\"%f\" X10=\"%f\" X11=\"%f\" X12=\"%f\" X13=\"%f\" X14=\"%f\" X15=\"%f\" X16=\"%f\" X17=\"%f\" X18=\"%f\" X19=\"%f\" X20=\"%f\"/></SensData>",
								target_->seq,    //! Id
								timestamp,       //! Ts
								move,          //! Issue move command
								target_->method, //! compliance command
								target_->x,      //! turn on/turn off
								0.0f,            //! 4
								0.0f,            //! 5
								0.0f,            //! 6
								0.0f,            //! 7
								0.0f,            //! 8
								0.0f,            //! 9
								0.0f,            //! 10
								0.0f,            //! 11
								0.0f,            //! 12
								0.0f,            //! 13
								0.0f,            //! 14
								0.0f,            //! 15
								0.0f,            //! 16
								0.0f,            //! 17
								0.0f,            //! 18
								0.0f,            //! 19
								0.0f);           //! 20
						break;
					case NoMethod:
						//! Should not be here.  Treat as though no command was sent.
						msgL = sprintf(line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%d\" X4=\"%d\" X5=\"%d\" X6=\"%d\" X7=\"%d\" X8=\"%d\" X9=\"%d\" X10=\"%d\" X11=\"%d\" X12=\"%d\" X13=\"%d\" X14=\"%d\" X15=\"%d\" X16=\"%d\" X17=\"%d\" X18=\"%d\" X19=\"%d\" X20=\"%d\"/></SensData>",
								target_->seq, timestamp,
								nomove, //! move/don't move
								0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
						break;
					default:
						//! Error, should never get here!
						break;
				} // switch (robot_->method)
			} // else if (target_->flag == FLAG_NEWCOMMAND)
			else
			{
				//! No new information from the controller.  Just send updated sequence
				//! number and do not move (i.e. liveman)
				msgL = sprintf(line, "<SensData Id=\"%d\" Ts=\"%f\"><AppData X1=\"%d\" X2=\"%d\" X3=\"%d\" X4=\"%d\" X5=\"%d\" X6=\"%d\" X7=\"%d\" X8=\"%d\" X9=\"%d\" X10=\"%d\" X11=\"%d\" X12=\"%d\" X13=\"%d\" X14=\"%d\" X15=\"%d\" X16=\"%d\" X17=\"%d\" X18=\"%d\" X19=\"%d\" X20=\"%d\"/></SensData>",
						target_->seq, timestamp,
						nomove, //! move/don't move
						0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
				// printf("Sent Message ID: %d\n", target_->seq);
			} // if (robot_->flag == FLAG_NEWCOMMAND) ... else

			pthread_mutex_unlock(&target_->robotMutex);
		} // try
		catch (...)
		{
			exception ("XML:ABB encode", "Unknown error");
		}
		target_->iMsgL = msgL;
		return true;

	}


	void ABB2Parse::exception (const char *where, const char *what)
	{
		static char message[1024];
		if (logger_ != NULL)
		{
			sprintf (message, "Exception in \" %s \" : %s", where, what);
			logger_->error (message);
		}
	}




int ABB2Parse::grabMutex(int* count){

	pthread_mutex_lock(&robot_->robotMutex);


	return 0;
}


int ABB2Parse::releaseMutex(){

	pthread_mutex_unlock(&robot_->robotMutex);


	return 0;
}

}
