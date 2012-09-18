#ifndef UDPPOC_H
#define UDPPOC_H 1


#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/xmlIO.h>


#define BUFLEN 8192
#define MSGLEN 32
#define MOD

enum servReturn { SANE , DANGLING_CLIENTS, NO_SOCKET, COULD_NOT_BIND, \
	COULD_NOT_RECIEVE };
typedef enum servReturn servReturn;

enum {FALSE, TRUE};

typedef int boolean;

typedef struct intercom {
	char data[BUFLEN];
	boolean running;
	int port;
	servReturn status;
} intercom;

typedef struct message {
	char data[MSGLEN];
	boolean killSignal;
} message;

typedef struct client_li {
	struct sockaddr_in si_in;
	struct client_li *next;
	struct client_li *prev;
	int id;
} client_li;

#endif /* !UDPPOC_H */
