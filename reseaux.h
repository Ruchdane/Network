#ifndef RESEAUXRUCHDANE
#define RESEAUXRUCHDANE

#define HOME "127.0.0.1"
#define MAX_LENGTH 225

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "reseaux.h"
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/sendfile.h>

#endif



typedef struct User{
    char nom[MAX_LENGTH];
    int macAdresse;
} User;

int createSocket(int port,char *adr,int type);
int InfoSocket(int sock);
char *GetIPadress();
// int cree_socket_tcp_ip();

#endif // !RESEAUXRUCHDANE