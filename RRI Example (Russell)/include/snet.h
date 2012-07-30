#ifndef SNET_INC_
#define SNET_INC_ 1


#ifdef WIN32

#include <winsock2.h>
#include <windows.h>
typedef int socklen_t;
//#define close(x) closesocket(x)

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>

#endif 


#include <string.h>
#include <iostream>
#include <fstream>

#define SNET_MSG_SIZE 8192

namespace snet
{
	// a TCP client class
	// that is easy to use
	// but not impossible to control.
	class serverMailbox
	{
	public:
		
		// returns the number of bytes read
		int get_data( char *in_buffer, const int fd );

		bool send_data( const char *out_buffer, const int fd );

		bool has_data(const int fd);

		bool is_open(int fd);

		bool connections_pending();

		void close_connection(int fd);

		int accept_connection();

		int clients_connected();

		int open_socket( const int port, const int max_connections);

		serverMailbox();

		~serverMailbox();

	private:

		int _fdmax;

		int _listener_fd;

		int _port;

		int _clients;

		struct sockaddr_in _server_addr;
		struct sockaddr_in _client_addr;

		fd_set sockets;
		fd_set t_sockets;

		timeval _no_time;
	};


// the transducer class is for UDP connections. The name
// comes from the audio device, because both directions
// are equally important

	class transducer
	{
	public:
		//create a socket and initialize server location
		bool create(const int port, const char *address);

		//simply create a socket
		bool create();

		//binds to the port specified in create
		bool bind_socket(const int port);

		// returns the number of bytes read
		int get_data( char *in_buffer );

		bool send_data( const char *out_buffer );

		bool wait_for_data(int secs);

		bool wait();

		bool has_data();

		transducer();

		~transducer();

	private:

		int _socket_fd;
		socklen_t _ssize;

		timeval _no_time;

		struct sockaddr_in _in_addr, _out_addr;

	};
}

#endif
