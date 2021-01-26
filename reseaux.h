#ifndef ReseauxReseaux
#define ReseauxReseaux

#define HOME "127.0.0.1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include "data.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>


#define MAX_LENGTH 225

typedef struct User{
    char nom[MAX_LENGTH];
    int macAdresse;
} User;

int createSocket(int port,char *adr,int type);
int InfoSocket(int sock);
char *GetIPadress();
// int cree_socket_tcp_ip();

#endif // !ReseauxReseaux