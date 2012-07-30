#include "snet.h"

// #define DEBUG 1
using namespace std;

#ifdef WIN32
#define close(x) closesocket(x)
#endif


namespace snet
{
	int serverMailbox::get_data( char *in_buffer, const int fd )
	{
		int read = recv( fd, in_buffer, SNET_MSG_SIZE, 0);
		if (read >= 0)
			in_buffer[read] = '\0';
		return read;
	}

	bool serverMailbox::send_data( const char *out_buffer, const int fd)
	{
		if (send( fd, out_buffer, strlen(out_buffer) + 1, 0)
				== -1)
		{
			// error TODO
			return false;
		}
		return true;
	}

	bool serverMailbox::has_data( const int fd )
	{
		FD_ZERO(&t_sockets);
		FD_SET( fd, &t_sockets);

		select(	_fdmax + 1,
				&t_sockets,
				NULL,
				NULL,
				0);
		if ( FD_ISSET( fd, &t_sockets ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool serverMailbox::is_open(int fd)
	{
		if (FD_ISSET(fd, &sockets))
			return true;
		return false;
	}

	bool serverMailbox::connections_pending()
	{
		FD_ZERO(&t_sockets);
		FD_SET(_listener_fd, &t_sockets);

		if ( select(	_fdmax + 1,
						&t_sockets,
						NULL,
						NULL,
						&_no_time) < 1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void serverMailbox::close_connection(int fd)
	{

		close(fd);

		FD_CLR(fd, &sockets);
	}

	int serverMailbox::accept_connection()
	{
		int t;
		socklen_t addrlen = sizeof(sockaddr_in);
		t = accept(	_listener_fd, 
					(sockaddr *)&_client_addr, 
					&addrlen);
		FD_SET(t, &sockets);
		_clients++;
		_fdmax = ( t > _fdmax ? t : _fdmax );
		return t;
	}

	int serverMailbox::clients_connected() //TODO
	{
		return 0;
	}

	int serverMailbox::open_socket( const int port, const int max_connections)
	{
		FD_ZERO(&sockets);
		FD_ZERO(&t_sockets);
		_port = port;
		if ( (_listener_fd = socket(	AF_INET,
									SOCK_STREAM,
									0)) == -1)
		{
            cerr << "mailbox: could not get socket from system";
#ifdef WIN32
			cerr << ", WSAERRNO " << WSAGetLastError();
#endif
			cerr << "\n";  
			return -1;
		}
/*		if (setsockopt(	_listener_fd,
						SOL_SOCKET,
						SO_REUSEADDR,
						(char *) (&ph),
						sizeof(int)) == -1)
		{
			//error TODO
            cerr << "mailbox: could not set socket options";
#ifdef WIN32
			cerr << ", WSAERRNO " << WSAGetLastError();
#endif
			cerr << "\n";            
			return -1;
		}*/
		
		memset(&_server_addr, 0, sizeof(sockaddr_in));
		_server_addr.sin_family = AF_INET;
		_server_addr.sin_addr.s_addr = htons(INADDR_ANY);
		_server_addr.sin_port = htons( _port );

		if (bind(	_listener_fd, 
					(const sockaddr *) (&_server_addr), 
					sizeof(sockaddr_in))
			== -1)
		{
			// error TODO
            cerr << "mailbox: could not bind to socket";
#ifdef WIN32
			cerr << ", WSAERRNO " << WSAGetLastError();
#endif
			cerr << "\n";  
			return -1;
		}
		else 
		{
			cout << "Got socket from system.\n";
		}

		if ( listen(_listener_fd, max_connections) == -1)
		{
			// error TODO
            cerr << "mailbox: could not initiate listen";
#ifdef WIN32
			cerr << ", WSAERRNO " << WSAGetLastError();
#endif
			cerr << "\n";  
			return -1;
		}

		FD_SET(_listener_fd, &sockets);
		_fdmax = _listener_fd;
		return 1;
	}

	serverMailbox::serverMailbox()
	{
		_fdmax = 0;
		_clients = 0;
		_no_time.tv_sec = _no_time.tv_usec = 0;
	}

	serverMailbox::~serverMailbox()
	{
	}

	///////////////////////
	///// transducer //////
	///////////////////////

	transducer::transducer()
	{
		memset(&_in_addr, 0, sizeof(sockaddr_in));
		memset(&_out_addr, 0, sizeof(sockaddr_in));
		_socket_fd = -1;
		_ssize = sizeof(sockaddr_in);
		_no_time.tv_sec = _no_time.tv_usec = 0;
	}

	transducer::~transducer()
	{

	}

	bool transducer::create(const int port, const char *address)
	{

#ifdef WIN32
		WSADATA wsaData;
		if ( WSAStartup(0x101, &wsaData) != 0)
		{
			return false; //! Could not bind
		}
#endif
		printf ("connecting to %s:%d\n", address, port);
		if (
			(_socket_fd = socket(AF_INET, SOCK_DGRAM, 0))
			== -1 )
		{
			cerr << "transducer::create: could not get socket "
				<< "from system.";
#ifdef WIN32
			cerr << " WSA ERRNO " << WSAGetLastError();
#endif
			cerr << endl;

			// error
			return false;
		}

		_out_addr.sin_family = AF_INET;
		_out_addr.sin_port = htons(port);
		_out_addr.sin_addr.s_addr = inet_addr(address);

		return true;
	}

	bool transducer::create()
	{
#ifdef WIN32
		WSADATA wsaData;
		if ( WSAStartup(0x101, &wsaData) != 0)
		{
			return false; //! Could not bind
		}
#endif
		
		if (
			(_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
			== -1 )
		{
			cerr << "transducer::create: could not get socket "
				<< "from system.";
#ifdef WIN32
			cerr << " WSA ERRNO " << WSAGetLastError();
#endif
			cerr << endl;

			// error
			return false;
		}

		return true;
	
	}


	bool transducer::wait()
	{
		fd_set _fds;
		FD_ZERO(&_fds);
		FD_SET(_socket_fd, &_fds);

		if ( select(	FD_SETSIZE,
						&_fds,
						NULL,
						NULL,
						NULL) < 1)
		{
			return false;
		}
		return true;
	}


	bool transducer::bind_socket(const int port)
	{
		//construct the server sockaddr_in struct
		_in_addr.sin_family = AF_INET;
		_in_addr.sin_addr.s_addr = htonl(INADDR_ANY); //recieves messages from any IP address
		_in_addr.sin_port = htons(port);  //port to recieve messages on

		printf ("binding to localhost:%d\n", port);

		//Bind to the socket
		if(bind(_socket_fd, (struct sockaddr*) &_in_addr, sizeof(_in_addr)) < 0)
			return false;
		else
			return true;
	}

	int transducer::get_data(char *in_buffer)
	{
		if( recvfrom(	_socket_fd, 
						in_buffer, 
						SNET_MSG_SIZE,
						0,
						(sockaddr *)&_out_addr,
						&_ssize)
			< 1)
		{
			cerr << "transducer::get: could not get data.";
#ifdef WIN32
			cerr << " WSA ERRNO " << WSAGetLastError();
#endif
			cerr << endl;
			return -1;
		}
		else
		{
#ifdef DEBUG
			cout << "got: ";
			cout << in_buffer << "\n";
#endif
			return 1;
		}
	}

	bool transducer::send_data(const char *out_buffer)
	{
		_ssize = sizeof(sockaddr_in);
		if( sendto(	_socket_fd,
					out_buffer,
					// SNET_MSG_SIZE,
					strlen(out_buffer) + 1,
					0,
					(sockaddr *)&_out_addr,
					_ssize) < 0)
		{
			cerr << "transducer::send: could not send data: " ;
#ifdef WIN32
			cerr << "WSA ERRNO " << WSAGetLastError();
#else
			cerr << strerror(errno);
#endif

			cerr << endl;
			return false;
		}
#ifdef DEBUG
		cout << "sending: " << out_buffer << endl;
#endif
		return true;
	}

	bool transducer::has_data()
	{
		fd_set _fds;
		FD_ZERO(&_fds);
		FD_SET(_socket_fd, &_fds);

		if ( select(	FD_SETSIZE,
						&_fds,
						NULL,
						NULL,
						&_no_time) < 1)
		{
			return false;
		}
		return true;
	}

	bool transducer::wait_for_data(int secs)
	{
		struct timeval timeout;
		timeout.tv_sec = secs;
		timeout.tv_usec = 0;


		fd_set _fds;
		FD_ZERO(&_fds);
		FD_SET(_socket_fd, &_fds);

		if (select(	FD_SETSIZE,
					&_fds,
					NULL,
					NULL,
					&timeout) < 1)
		{
			return false;
		}
		return true;
	}
}
