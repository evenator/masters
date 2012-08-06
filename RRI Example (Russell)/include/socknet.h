///////////////////////////////////////////////////////////////////////////////
//
//  Original System: Behavior Server
//  Subsystem:       CommSocket
//  Workfile:        socknet.h
//  Revision:        1.0 - 6 April, 2006
//                   1.1 - 8 August, 2006 : Modified methods to return boolean
//                                          values instead of integers.
//  Author:          J. Marvel
//
//  Description
//  ===========
//  Network connectivity - Socket class.  Note that the get and send functions
//  require a networkStruct object passed as an argument.  This way we can have
//  a single network object sending data to multiple destinations simply by
//  passing different network connection parameters.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef network_h
#define network_h

#include <conio.h>
#include <time.h>
#include "..\Reporter\reporter.h"
#include "../../portable.h"
#include "../../types.h"
#include "../../UDP_Implementation/shr/udpi.h"

using namespace Reporter;

//! @brief Connection information for networking
//!
//! Formerly known as socketStruct
//!
struct LIBRARY_API networkStruct
{
  //! @brief Client's socket file descriptor
  //!
  SOCKET cFd;

  //! @brief Server's socket file descriptor
  //!
  SOCKET sFd;

  //! @brief Current computer's IP address
  //!
  char* serverAddress;

  //! @brief /emy/server structure for udp
  //!	
  struct sockaddr_in serverInfo;

  //! @brief Attached computer's IP address
  //!
  char* address;

  //! @brief Client's port number
  //!
  unsigned int cP;

  //! @brief Server's port number
  //!
  unsigned int sP;

  //! @brief Whether we are currently connected (formerly in networkSettings)
  //!
  bool connected;

  //! @brief Whether the server file descriptor is currently bound
  //!
  bool bound;

  //! @brief Whether the socket is a server
  //!
  bool server;

  //! @brief Default constructor
  //!
  networkStruct() :
    connected(false),
    bound(false),
    server(false)
  {
    serverAddress = new char[16];
  }
  ~networkStruct()
  {
	  delete [] serverAddress;
  }
  
};



namespace Network
{
  //! @ingroup Network
  //!
  //! @brief Socket connectivity class
  //!
  class LIBRARY_API socketNet
  {
  public:

    //! @brief Constructor
    //!
    //! @param settings Pointer to the system settings structure owned by the
    //!                 parent process
    //! @param rep      Pointer to the reporter class owned by the parent
    //!                 process
    //!
    socketNet (networkSettings *settings, Logger *logPtr);

	socketNet (networkSettings *settings, Logger *logPtr, netProto protocol);

    //! @brief Destructor
    //!
    ~socketNet ();

    //! @brief Create the communication socket
    //!
    //! @param socketData Network connection parameters
    //!
    //! @return TRUE if socket created successfully, FALSE otherwise
    //!
    //! @note This function blocks.  Kill/pause any time-sensitive processes
    //!       before calling this function.
    //!       socketData->sP must be defined before calling this function
    //!       Set socketData->bound to TRUE if reusing a socket for multiple
    //!       clients.
    //!
    bool create (networkStruct& socketData);

    //! @brief Connect to a server communication socket
    //!
    //! @param socketData Network connection parameters
    //!
    //! @return True if we connected to the server, False otherwise
    //!
    //! @note socketData->port must be set before calling this function
	//! @note /emy/this function does not sent packets anymore
    //!
    bool attach (networkStruct &socketData);

	//! /emy/
	//! @brief Announce to the UDP server
    //!
	//! @param nData the network data structure
    //!
    //! @return 1 if we connected to the server, 0 if no data,
	//! -1 if bad read, -2 if bad response.  Looking for <ack/>
    //!
    //! @note Called after attach()
    //!
	int readack(networkStruct &nData);

    //! @brief Read from an open socket
    //!
    //! @param buffer The buffer to be filled with data from the socket
    //! @param socket The socket to query
    //!
    //! @return True if data was read successfully, False otherwise
    //!
    bool getData (char* buffer, networkStruct& socket);

    //! @brief Read raw bytes from an open socket
    //!
    //! @param bytes    TODO
    //! @param numBytes TODO
    //!
    //! @return True if data was read successfully. False otherwise
    //!
    bool getRawBytes (void *bytes, int &numBytes);

    //! @brief Query the socket to see if data is waiting to be read
    //!
    //! @param socket The specifics of the socket to be queried
    //!
    //! @return True if there is data waiting to be read from the socket
    //!
    bool dataWaiting (networkStruct &socket);

    //! @brief Write data to a socket connection
    //!
    //! @param buffer Data to be written to the socket
    //! @param socket Socket to be written to
    //!
    //! @return True if data was sent successfully, False otherwise
    //!
    bool sendData (char* buffer, networkStruct& socket);

    //! @brief Write raw bytes to an open socket connection
    //!
    //! @param bytes    TODO
    //! @param numBytes TODO
    //!
    //! @return True if data was sent successfully. False otherwise.
    //!
    bool sendRawBytes (void *bytes, int numBytes);

    //! @brief Close a specified socket connection
    //!
    //! @param socket The socket to be closed
    //!
    void closeConnection (networkStruct& socket);

	netProto protocol;

	int wait_timeout ( int filedes, unsigned int seconds );

	int socketNet::has_data (int filedes);

  private:

    //! @brief Display an error message
    //!
    //! @param where The location where the error occurred
    //! @param what  The error type
    //!
    void exception (const char *where, const char *what);

    //! @brief TODO
    //!
    Logger *logger_;

    //! @brief Shared pointer to system settings
    //!
    networkSettings *settings_;

	//! @brief Wait for available input on a socket, with a timeout
	//!
	//! @param filedes The socket file descriptor
	//! @param seconds The length of time (in seconds) to wait
	//! 
	//! /emy/a simple wrapper for select()
  };
}

#endif
