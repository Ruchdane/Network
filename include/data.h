#ifndef DATARUCHDANE
#define DATARUCHDANE

#include "reseaux.h"

typedef enum  DataType {Id,Test,ClientListe,File} DataType;

#define BUFFSIZE  1024

typedef struct Data
{
    DataType type;
    size_t len;
}Data;

typedef struct Client
{
    int id;
    char *name;
    int sock;
    sem_t lock;
    pthread_t cthread;
} Client;

ssize_t SendText(int sock,char *message);
ssize_t SendClient(int sock,Client *client);
char *GetText(int sock,Data data);
Client **AddClient(Client **clients,char *name,int sock,int *count);
Client **AddClientPointer(Client **clients,Client *client,int *count);
Client *GetClient(int sock,Data data);
ssize_t SendFile(int sock,char *pathname);
void GetFile(int sock);



#endif // !DATARUCHDANE