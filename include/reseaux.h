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
#include <signal.h>
#include <semaphore.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "data.h"

#if defined(_WIN32)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#define ISINVALIDSOCKET(s) (s == INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
// #define GETSOCKETERRNO() (WSAGetLastError())
#define ApiInit(instruction)WSADATA d;\
    if (WSAStartup(MAKEWORD(2, 2), &d)) {\
        Log("Failed to initialize winsoket api");\
        instruction\
    }
//execute a single instruction the first on
// windows and the other on unix
#define RPlatform(windows,unix) windows;
#else
#define RPlatform(windows,unix) unix;
#define ApiInit(instruction)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/sendfile.h>

#define ISINVALIDSOCKET(s) (s < 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#endif
#define GETSOCKETERRNO() (errno)

#define Log(message)  printf("\t%s %d:%s\n",message, GETSOCKETERRNO(),strerror GETSOCKETERRNO() );

struct AdapterList { 
    RPlatform(PWCHAR nom,char *nom)
    char *IPV4, *IPV6;
};

typedef struct User{
    char nom[MAX_LENGTH];
    int macAdresse;
} User;

SOCKET createSocket(char* port,char *adr,int type);
int InfoSocket(int sock);
struct AdapterList *GetAdapter(int *count);
void DisplayNetworkInfo();
// int cree_socket_tcp_ip();


#endif // !RESEAUXRUCHDANE