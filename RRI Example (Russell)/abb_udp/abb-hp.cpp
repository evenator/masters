/*       _\|/_
         (o o)
 +----oOO-{_}-OOo--------------------------------------------------------+
 |     abb-hp.cpp : Defines the entry point for the console application. |
 |     Eric Young, June 2011                                             |
 |                                                                       |
 |     Designed to optimize throughput for the ABB                       |
 | Project: CWRU/robosurgery                                             |
 +-----------------------------------------------------------------------*/
#ifdef WIN32
#include "stdafx.h"
#else
#include "abb-hp.h"
#include <pthread.h>
#include <time.h>
#endif 
#include <list>


#define BUFF_SIZE 16384
#define DELAY_MIN 10

//#define UNDEBUG

#ifndef UNDEBUG
#define display(something) cout << something << endl;
#else
#define display(something) (void) 0;
#endif
int Sleep(unsigned long);
#define LOOP_QUIT_MAX 10
// main server/uplink thread
#ifdef WIN32
DWORD __stdcall Processor (LPVOID param);
#else
void* Processor(void* param);
#endif /* ifdef WIN32 */

#ifndef WIN32
void* log_thread(void* param);
#endif

#ifndef WIN32
int __nsleep(const struct timespec *req, struct timespec *rem)
{
	struct timespec temp_rem;
	if(nanosleep(req,rem)==-1)
		return __nsleep(rem,&temp_rem);
	else
		return 1;
}

int Sleep(unsigned long milisec)
{
	struct timespec req={0,0},rem={0,0};
	time_t sec=(int)(milisec/1000);
	milisec=milisec-(sec*1000);
	req.tv_sec=sec;
	req.tv_nsec=milisec*1000000L;
	__nsleep(&req,&rem);
	return 1;
}
#endif

istream& operator>> (istream& aStream, objectTypes& type) 
{
	int x;
	aStream >> x;
	type = static_cast<objectTypes> (x);
	return aStream;
} 

struct bundle : 
	public _cbundle::base
{

	struct server_t
	{
		int port;
		char address[128];
		bool connected;
		bool connect;
		bool disconnect;
		bool send_data;

		//! proc_data indicates if there is data
		//! from the server waiting to be sent to the
		//! robot.
		bool proc_data;
	} server;

	struct abb_t
	{
		int port;
		char address[128];
		bool connected;
		bool disconnect;

		//! the ABB socket's file descriptor.
		//! must be kept by the program to send data.
		//int fd;
		//! unused
		bool send_data;
	} abb;


	snet::transducer	abb_server;
	snet::transducer	uplink;

	bool  listener_running;

	int max_retries;

	Logger logr;
	pthread_mutex_t log_mutex;

	bundle()
	{
		pthread_mutex_init(&log_mutex, NULL);
		robot = new robotStruct;
		server.connected	= 
			server.send_data = 
			server.proc_data = 
			server.connect	= 
			server.disconnect = 
			listener_running = false;
		abb.connected =
			abb.disconnect = 
			abb.send_data = false;
		//abb.fd = -2;
	}


	~bundle()
	{
		pthread_mutex_destroy(&log_mutex);
	}
};


void die(char *msg)
{
	cerr << msg << endl;
	exit(1);
}



int main()
{
	ifstream settings("settings.dat");
	bundle ginfo;


	char intbuff[128];
	settings >> ginfo.abb.port >> ginfo.server.port;
	settings >> ginfo.abb.address >> ginfo.server.address;

	// object settings
	settings >> ginfo.masterObj->name
		>> ginfo.masterObj->objType
		>> intbuff;
	ginfo.masterObj->objTypeStr = strdup(intbuff);

	// start positions
	settings >> ginfo.masterObj->xPos >> ginfo.masterObj->yPos
		>> ginfo.masterObj->zPos >> ginfo.masterObj->xRot
		>> ginfo.masterObj->yRot >> ginfo.masterObj->zRot;

	settings >> ginfo.max_retries;

	ginfo.masterObj->cmdCount = 5;
	ginfo.masterObj->validCommands = new commandCaps[ginfo.masterObj->cmdCount];
	ginfo.masterObj->validCommands[0] = cMove;
	ginfo.masterObj->validCommands[1] = lMove;
	ginfo.masterObj->validCommands[2] = rMove;
	ginfo.masterObj->validCommands[3] = gOpen;
	ginfo.masterObj->validCommands[4] = gClose;
	//ginfo.masterObj->validCommands[5] = qMove; ???

	ginfo.masterObj->repCount = 3;
	ginfo.masterObj->validReportables = new reportCaps[ginfo.masterObj->repCount];
	ginfo.masterObj->validReportables[0] = state;
	ginfo.masterObj->validReportables[1] = position;
	ginfo.masterObj->validReportables[2] = forces;

	ginfo.robotTarget->flag = FLAG_NOCOMMAND;
	ginfo.robot->flag = FLAG_NOCOMMAND;

	/*snet::transducer abb_server;
	if(!abb_server.create())//ginfo.abb.port, ginfo.abb.address))
		die("Can't create socket");
	if(!abb_server.bind_socket(ginfo.abb.port))
		die("Can't bind socket");
	char buffer[BUFF_SIZE];
	
	while(1)
	{
		if(abb_server.has_data())
		{
			memset(buffer,'\0', BUFF_SIZE);
			if(abb_server.get_data(buffer))
				display(buffer);
		}
	}*/

	// this will need a pthread implementation 
	// in order to be portable. Of course,
	// there are about 5 other source files 
	// that also need porting
#ifdef WIN32
	unsigned long cthread_id;
	CreateThread(	0,
			0,
			Processor,
			static_cast<void *> (&ginfo),
			0,
			&cthread_id);
#else
	pthread_t cthread_id, logthread_id;
	pthread_create(&cthread_id, NULL, Processor, static_cast<void *> (&ginfo));
	pthread_create(&logthread_id, NULL, log_thread, static_cast<void *> (&ginfo));
	
#endif

	Sleep(100);
	ginfo.server.connect = true;

	string inbuff;

	int i;

	for(i = 0; i < 3 && !ginfo.listener_running; i++, Sleep(500))
		display("listener try" << i);
	if ( i == 3 )
	{
		cerr << "could not spawn listener. quitting\n";
		exit(0);
	}

#ifdef WIN32
	cout << "\n---= Processing requests. Press F11 to quit. =---\n";

	while ( ginfo.listener_running )
	{
		if (GetAsyncKeyState (VK_F11) != 0)
		{
			break;
		}
		/*		if (curr_abb_fd != ginfo.abb.fd
				&& ginfo.abb.fd == -1)
				{
				cout << "ABB disconnected.\n";
				curr_abb_fd = ginfo.abb.fd;
				}
				else if (curr_abb_fd != ginfo.abb.fd)
				{
				cout << "ABB " 
				<< (++abb_connections > 1 ? "re" : "")
				<< "connected.\n";
				curr_abb_fd = ginfo.abb.fd;
				} 
				Sleep(50);*/
	}
#else
	cout << "[MAIN] Processing. Press enter to exit.\n";
	getchar();
	cout << "[MAIN] Key Pressed. Exiting!\n";
#endif

	// ginfo.running = false; 
	//ginfo.server.disconnect = true;
	 ginfo.abb.disconnect = true;
#ifdef WIN32
	for(; ginfo.listener_running;);
	cout << "[MAIN] Press enter to quit.\n";
	getchar();
#else
	void* something;
	pthread_join(cthread_id, &something);
	pthread_join(logthread_id, &something);
#endif


	delete [] ginfo.masterObj->validCommands;
	delete [] ginfo.masterObj->validReportables;
	ginfo.masterObj->validCommands = NULL;
	ginfo.masterObj->validReportables = NULL;
	display("[MAIN] Good Bye ..."); 
	return 0;
}





#ifdef WIN32
DWORD __stdcall Processor( LPVOID param )
#else
void* Processor(void* param)
#endif
{
	bundle *ginfo = static_cast<bundle *> (param);

	Logger logr;

	pthread_mutex_lock(&ginfo->log_mutex);
	bool log_mutx = true; // indicates to this thread if we have the logging mutex

	Xml::ABB2Parse					abb_parser(ginfo->robot, &ginfo->logr, ginfo->robotTarget);
	Xml::XmlParse<Xml::ABB2Parse>	abb_xml(&abb_parser, &logr);

	Xml::SurgeryParse					 up_parser(ginfo->masterObj, &ginfo->logr);
	Xml::XmlParse<Xml::SurgeryParse>	 uplink_xml(&up_parser, &ginfo->logr);



	//to make the code more readable- less dereferencing
	snet::transducer &uplink = ginfo->uplink;
	snet::transducer &abb_server = ginfo->abb_server;
	//int &abb_fd = ginfo->abb.fd;
	bundle::abb_t &abb = ginfo->abb;
	bundle::server_t &server = ginfo->server;
	robotStruct *robot = ginfo->robot;
	robotStruct *robotTarget = ginfo->robotTarget;
	objectClass *masterObj = ginfo->masterObj;
	// end convenience derefs 
	


	//connect a udp client to server location
	if(!uplink.create( server.port, server.address )) {
		pthread_mutex_unlock(&ginfo->log_mutex);
		pthread_exit(NULL);
	}
	
	//create a udp server on host machine
	if(!abb_server.create()) {
		pthread_mutex_unlock(&ginfo->log_mutex);
		pthread_exit(NULL);
	}
	if(!abb_server.bind_socket(abb.port)) {
		pthread_mutex_unlock(&ginfo->log_mutex);
		pthread_exit(NULL);
	}

	//create the incoming and outgoing buffers
	char* sinbuffer	 = new char[BUFF_SIZE];
	char* soutbuffer = new char[BUFF_SIZE];
	char* ainbuffer  = new char[BUFF_SIZE];
	char* aoutbuffer = new char[BUFF_SIZE];

	sinbuffer[0] 	= '\0';
	soutbuffer[0] 	= '\0';
	ainbuffer[0] 	= '\0';
	aoutbuffer[0] 	= '\0';

	//inform the main loop that we are running
	ginfo->listener_running = true;

	int tries = 0;
	//int delayCount = 0;
	//bool delayed = false;

	struct request_t //for tracking server connection
	{
		bool sent;
		bool parsed;
		request_t()
			: sent(false), parsed(false) {}
	} request;

	list<char*> inQueue;

	int quitloop = 0;
	bool abb_disconnect_message_sent = false;
	while( ginfo->running ) // main handler loop
	{
		//Check to see if the abb sent any data
		if ( abb_server.has_data() ) 
		{
			// display("[ABB] Has information");
			if(!abb.connected)
			{
				abb.connected = true;
				display("[ABB] Connected!")
			}

			memset( ainbuffer, '\0', BUFF_SIZE );
			
			if ( abb_server.get_data(ainbuffer) )
			{
				//display("[ABB] Message Recieved!");
				
				//Parse the message sent by the abb
				// ainbuffer[read] = '\0';
			    //display("[ABB] Message in: " << ainbuffer)
				abb_xml.parse( ainbuffer );

				
				/*
				 * this means that the robot has
				 * acknowledged a command.
				 */
				if (robot->ackflag1 == 0 && robot->ackflag2 == 1)
				{
					if (robot->flag != FLAG_ROBMOVING)
					{
						display("[ABB] Robot is now moving");
						robot->flag = FLAG_ROBMOVING;
						robotTarget->flag = FLAG_BUSY;
						masterObj->objState = moving;
						logr.log("ABB ACK");
					 }
				}

				/*
				 * this means that the robot has
				 * finished a command.
				 */
				else if (robot->ackflag1 == 1 && robot->ackflag2 == 0) 
				{
					if (robot->flag == FLAG_ROBMOVING)
					{
						//delayCount=0;
						display("[ABB] Robot finished command");
						robot->flag = FLAG_ROBIDLE;
						robotTarget->flag = FLAG_NOCOMMAND;
						masterObj->objState = on;
						logr.log("ABB DONE MOVING");
					}
					masterObj->objState = on;
				}
					

				if (abb.disconnect || robot->flag == FLAG_QUIT)
				{
					display("[ABB] Sending ABB quit message.");
					robotTarget->flag = FLAG_QUIT;
					abb.disconnect = false;
					abb_disconnect_message_sent = true;
				}
			
				
				//if(delayCount < DELAY_MIN && robotTarget->flag == FLAG_NEWCOMMAND)
				//{
				//	robotTarget->flag = FLAG_NOCOMMAND;
				//	delayed = true;
				//}

				//abb_xml.encode(aoutbuffer);
				//abb_server.send_data(aoutbuffer);
				
				//if(delayCount < DELAY_MIN)
				//	delayCount++;

				if (robotTarget->flag == FLAG_NEWCOMMAND || robotTarget->flag == FLAG_QUIT)
				{	
					abb_xml.encode(aoutbuffer);
					abb_server.send_data(aoutbuffer);

					display("[ABB] Sending command.");
					robotTarget->flag = FLAG_BUSY;
					logr.log(aoutbuffer);
				}

				//if(delayed)
				//{
				//	robotTarget->flag = FLAG_NEWCOMMAND;
				//	delayed = false;
				//}

			} // if ( abb_server.get_data(ainbuffer) > 0 )
			else
			{
				display("[ABB] Error recieving data!");
			} // if ( abb_server.get_data(ainbuffer) > 0 )
		} // if ( abb_server.has_data() )
		else if (abb.disconnect & !abb.connected)
		{
			ginfo->running = false;
		}
		else if (abb_disconnect_message_sent)
		{
			Sleep(50);
			quitloop++;
			if (quitloop > LOOP_QUIT_MAX)
				ginfo->running = false;
		}
		
		if (server.connect)
		{
			masterObj->comTypeOut = announce;
			uplink_xml.encode( soutbuffer );
			request.sent = uplink.send_data( soutbuffer );
			if (!request.sent)
			{
				tries++;
				cerr << "connection attempt " << tries << " failed\n";
				if ( tries > ginfo->max_retries )
				{
					server.connect = false;
					cerr << "could not send data."
						<< " aborting operation.\n";
					ginfo->listener_running = false;
#ifdef WIN32
					return 1;
#else
					return NULL;
#endif
				}
			}

			Sleep(100);
		} // if (server.connect)


		if (uplink.has_data())
		{
			memset(sinbuffer, 0, BUFF_SIZE);

			if (uplink.get_data( sinbuffer ) )
			{
				display( "[COM] Received: " << sinbuffer );
				logr.log(sinbuffer);
				inQueue.push_back( strdup(sinbuffer) );
				display( "\n[COM] " << inQueue.size() << " commands in the queue.\n");

			} // if ( uplink.get_data( sinbuffer ) )
			else
			{
				display("[COM] uplink.get_data error");
				// disconnect from server? error
			}
		} // if (uplink.has_data())

		if ( !inQueue.empty() && (robotTarget->flag == FLAG_NOCOMMAND || !server.connected)) //(robotTarget->flag != FLAG_BUSY || !server.connected))
		{
			display("[COM] Parsing: " << inQueue.front());
			request.parsed = uplink_xml.parse( inQueue.front() );
			inQueue.pop_front();
			
			if(masterObj->objID == -2)
			{
				display("[COM] Server asked client to quit");
				server.connected = false;
				abb.disconnect = true;
			}
			else if (masterObj->objID != 0)
			{
				// int temp = masterObj->targetID;
				masterObj->targetID = masterObj->sourceID;
				masterObj->sourceID = masterObj->objID;

				//if (abb.connected)
				//{
					if (masterObj->cmdTypeIn != noMove)
					{
						// translate data from masterObj,
						// set during the last parse,
						// over to the robotTarget
						switch(masterObj->cmdTypeIn)
						{
							case cMove:
								//! Position-controlled Cartesian move
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = PositionControl;
								robotTarget->x = masterObj->xPos;// - objectBase.xPos;
								robotTarget->y = masterObj->yPos;// - objectBase.yPos;
								robotTarget->z = masterObj->zPos;// - objectBase.zPos;
								robotTarget->rx = masterObj->xRot;// - objectBase.xRot;
								robotTarget->ry = masterObj->yRot;// - objectBase.yRot;
								robotTarget->rz = masterObj->zRot;// - objectBase.zRot;
								break;
							case qMove:
								//! Position-controlled in Quaternion move
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = QuaternionControl;
								robotTarget->x = masterObj->xPos;// - objectBase.xPos;
								robotTarget->y = masterObj->yPos;// - objectBase.yPos;
								robotTarget->z = masterObj->zPos;// - objectBase.zPos;
								robotTarget->rx = 0.0f;// - objectBase.xRot;
								robotTarget->ry = 0.0f;// - objectBase.yRot;
								robotTarget->rz = 0.0f;// - objectBase.zRot;
								robotTarget->q1 = masterObj->q1;// - objectBase.q1;
								robotTarget->q2 = masterObj->q2;// - objectBase.q2;
								robotTarget->q3 = masterObj->q3;// - objectBase.q3;
								robotTarget->q4 = masterObj->q4;// - objectBase.q4;
								robotTarget->moveTime = masterObj->moveTime;
								break;
							case rMove:
								//! Position-controlled rotation
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = Rotation;
								robotTarget->x = 0.0f;
								robotTarget->y = 0.0f;
								robotTarget->z = 0.0f;
								robotTarget->rx = masterObj->xRot;// - objectBase.xRot;
								robotTarget->ry = masterObj->yRot;// - objectBase.yRot;
								robotTarget->rz = masterObj->zRot;// - objectBase.zRot;
								break;
							case lMove:
								//! Force-controlled linear move
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = ForceControl;
								robotTarget->x = masterObj->xPos;// - objectBase.xPos;
								robotTarget->y = masterObj->yPos;// - objectBase.yPos;
								robotTarget->z = masterObj->zPos;// - objectBase.zPos;
								robotTarget->rx = 0.0f;
								robotTarget->ry = 0.0f;
								robotTarget->rz = 0.0f;
								break;
							case gOpen:
								//! Gripper open
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = Gripper;
								robotTarget->x = masterObj->grip = gripOpen;
								break;
							case gClose:
								//! Gripper close
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = Gripper;
								robotTarget->x = masterObj->grip = gripClose;
								break;
								//!RCJ I added this so that the ABB can lower itself onto the table gently
							case guidedMotion:
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = guidedRobMotion;
								break;
							case startPos:
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = startRobPos;
								robotTarget->y = masterObj->yPos;  //!0 PC, 1 FC
								break;
							case setCompliance:
								//!turn on and off compliant motion
								//! this command uses x as the on off flag.
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = Compliance;
								robotTarget->x = masterObj->xPos;  //!0 PC, 1 FC
								break;
							case jMove: 
								//! Position-controlled Joint space move
								display("[COM] processing Joint space command\n");
								robotTarget->flag = robot->flag = FLAG_NEWCOMMAND;
								robotTarget->method = JointControl;
								robotTarget->x = masterObj->xPos;// - objectBase.xPos;
								robotTarget->y = masterObj->yPos;// - objectBase.yPos;
								robotTarget->z = masterObj->zPos;// - objectBase.zPos;
								robotTarget->rx = masterObj->xRot;// - objectBase.xRot;
								robotTarget->ry = masterObj->yRot;// - objectBase.yRot;
								robotTarget->rz = masterObj->zRot;// - objectBase.zRot;
								break;
							case dLog:
								display("[listener] processing log command\n");
								if (masterObj->logst) {
									if (log_mutx) {
										log_mutx = false;
										pthread_mutex_unlock(&ginfo->log_mutex);
									}
								} else {
									if (!log_mutx) {
										log_mutx = true;
										pthread_mutex_lock(&ginfo->log_mutex);
									}
								}
								break;
							default:
								//! Unknown/unsupported move command;
								robotTarget->method = NoMethod;
								break;
						} // switch(masterObj->cmdTypeIn)
					} // if (masterObj->cmdTypeIn != noMove)

					if (masterObj->reportTypeIn != noReport)
					{
						masterObj->xPos = masterObj->yPos = masterObj->zPos = masterObj->xRot =
							masterObj->yRot = masterObj->zRot = masterObj->xForce =
							masterObj->yForce = masterObj->zForce = 
							masterObj->xRotForce = masterObj->yRotForce = 
							masterObj->zRotForce = 0.0f;
						switch (masterObj->reportTypeIn)
						{
							case state:
								//! Return both position and forces
								masterObj->xPos = robot->x;
								masterObj->yPos = robot->y;
								masterObj->zPos = robot->z;
								masterObj->xRot = robot->rx;
								masterObj->yRot = robot->ry;
								masterObj->zRot = robot->rz;
								masterObj->xForce = robot->fx;
								masterObj->yForce = robot->fy;
								masterObj->zForce = robot->fz;
								masterObj->xRotForce = robot->frx;
								masterObj->yRotForce = robot->fry;
								masterObj->zRotForce = robot->frz;
								masterObj->q1 = robot->q1;
								masterObj->q2 = robot->q2;
								masterObj->q3 = robot->q3;
								masterObj->q4 = robot->q4;
								//cout << "q1 = " << robot->q1<<endl;
								//cout << "q2 = " << robot->q2<<endl;
								//cout << "q3 = " << robot->q3<<endl;
								//cout << "q4 = " << robot->q4<<endl;break;
							case position:
								//! Return the robot pose
								masterObj->xPos = robot->x;
								masterObj->yPos = robot->y;
								masterObj->zPos = robot->z;
								masterObj->xRot = robot->rx;
								masterObj->yRot = robot->ry;
								masterObj->zRot = robot->rz;
								masterObj->q1 = robot->q1;
								masterObj->q2 = robot->q2;
								masterObj->q3 = robot->q3;
								masterObj->q4 = robot->q4;
								break;
							case forces:
								//! Return the sensed forces
								masterObj->xForce = robot->fx;
								masterObj->yForce = robot->fy;
								masterObj->zForce = robot->fz;
								masterObj->xRotForce = robot->frx;
								masterObj->yRotForce = robot->fry;
								masterObj->zRotForce = robot->frz;
								break;
							default:
								//! Unknown/unsupported report type
								break;
						} // switch (masterObj->reportType)
						masterObj->comTypeOut = updateCommunique;
						masterObj->AckOut = ACK; 
						uplink_xml.encode(soutbuffer);
						uplink.send_data(soutbuffer);
						logr.log(soutbuffer);
					}
				//} // if (abb.connected)
			} // if (masterObj->sourceID != 0 ...
		}


		if (request.sent && request.parsed)
		{
			if (masterObj->comTypeIn == ack)
			{
				cout << "[COM] Connected to server.\n";
				server.connected = true;
				server.connect =
					request.sent =
					request.parsed = false;
			}
		} // if (request.sent && request.parsed)
	}// while (ginfo->running)

	display("[PROC] While loop exited.");

	if ( server.connected )
	{
		masterObj->comTypeOut = commandCommunique;
		masterObj->cmdTypeOut = disconnect;
		uplink_xml.encode( soutbuffer );
		uplink.send_data( soutbuffer );
		display("[COM] Disconnected from the server");
	
	}

	delete [] ainbuffer;
	delete [] aoutbuffer;
	delete [] sinbuffer;
	delete [] soutbuffer;

	ginfo->listener_running = false;
#ifdef WIN32
	return NULL;
#else
	if (log_mutx) { pthread_mutex_unlock(&ginfo->log_mutex); }
	pthread_exit(NULL);
#endif
}

#ifndef WIN32
void* log_thread(void* param) {
	sleep(1);
	puts("[plogger] awaiting orders, sir!\n");
	fflush(stdout);
	bundle *ginfo = static_cast<bundle *> (param);

	char filename[BUFF_SIZE];
	char datestr[BUFF_SIZE];
	struct tm * timeinfo;
	time_t rawtime;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(datestr, BUFF_SIZE, "%y-%m-%d_%H-%m-%S", timeinfo);
	sprintf(filename, "positionlog.abb.%s" , datestr);

	Logger *logr = new Logger(filename);

	//bool write = false;
	robotStruct* robot = ginfo->robot;
	pthread_mutex_t &log_mutex = ginfo->log_mutex;
	char wbuff[BUFF_SIZE];

	double last_time = robot->ts;
	pthread_mutex_lock(&log_mutex);
	pthread_mutex_unlock(&log_mutex);
	while(true) {
		pthread_mutex_lock(&log_mutex);
		if (!ginfo->running)
			break;


		if (last_time < robot->ts) {
			// new update value
			last_time = robot->ts;

			struct timespec tp;
			clock_gettime(CLOCK_REALTIME, &tp);
			struct tm* tn = localtime(&tp.tv_sec);

#define NSEC_TO_3DECIMAL 1000000
			sprintf(wbuff, "[plogger] %02d/%02d/%02d %02d:%02d:%02d.%03ld seq %6d ts % 4.03f [ % 4.03f % 4.03f % 4.03f ] [ % 4.03f % 4.03f % 4.03f ] [ %01.05f %01.05f %01.05f %01.05f ]", 
						// timestamp values
						tn->tm_mon + 1, tn->tm_mday,  // mm/dd/yy
						tn->tm_year - 100, tn->tm_hour, tn->tm_min,
						tn->tm_sec, tp.tv_nsec / NSEC_TO_3DECIMAL, 
						// robot values ---
						robot->seq,
						robot->ts,
						robot->x, robot->y, robot->z,
						robot->rx, robot->ry, robot->rz,
						robot->q1,robot->q2,robot->q3,robot->q4);
			// log the new values
			logr->log(wbuff);
			fprintf(stderr, "%s", wbuff);


		}

		pthread_mutex_unlock(&log_mutex);
		Sleep(8);
	}
	pthread_mutex_unlock(&log_mutex);
	delete logr;
	pthread_exit(NULL);

}


#endif


