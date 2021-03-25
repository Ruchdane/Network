#include "data.h"

ssize_t SendText(int sock,char *message)
{
    Data data;
    data.type = Test;
    data.len = strlen(message) + 1;
    if(send(sock,(char*)&data, sizeof(data),0) == -1){
        printf("sock = %d",sock);
        Log("Error at write test to socket because ");
        return -1;
    }
    return send(sock, message,data.len,0);
}
ssize_t SendClient(int sock,Client *client)
{
    Data data;
    char message[225];

    sprintf(message,"%d:%s",client->id,client->name);
    data.type = Id;
    data.len = strlen(message) + 1;
    
    if(send(sock,(char*)&data, sizeof(data),0)== -1){
        Log("Error at write test to socket because ");
        return -1;
    }
    return send(sock, message,data.len,0);
}

ssize_t SendFile(int sock,char *pathname)
{
    Data data;
    struct stat fileStats;
    int fd = open(pathname, O_RDONLY);
    int  sizeSent = 0,sizeRemaining;
    #if defined(_WIN32)
    int sizeRead;
    char buff[BUFFSIZE];
    #endif
    char *filename = strrchr(pathname,'/');
    off_t offset = 0;

    data.type = File;
    if (fd == -1){
        printf("File %s not found",pathname);
        return -1;
    }

    fstat(fd,&fileStats);
    send(sock,(char*)&data,sizeof(data),0);
    printf("File Size  %d b\n",fileStats.st_size);
    send(sock,(char*)&fileStats,sizeof(struct stat),0);
    filename = filename == NULL ? pathname: filename + 1;
    SendText(sock,filename);
    #if defined(_WIN32)
    do
    {   
        sizeRead = read(fd,buff,min(fileStats.st_size - sizeSent,BUFFSIZE));
        printf("%s",buff); 
        sizeSent += send(sock,buff,min(fileStats.st_size - sizeSent,BUFFSIZE),0);
        printf("\rsent %3d%%\n",(sizeSent*100)/fileStats.st_size);
    } while (sizeSent < fileStats.st_size);
    
    #else
    sizeSent = sendfile(sock,fd,NULL,fileStats.st_size);
    #endif
    close(fd);
}

void GetFile(int sock)
{
    Data data;
    struct stat fileStats;
    int fd ;
    off_t sizeRemaining,sizeReceived,offset;
    char buff[BUFFSIZE],*filename;

    recv(sock,(char*)&fileStats,sizeof(struct stat),0);
    recv(sock,(char*)&data,sizeof(data),0);

    filename = GetText(sock,data);
    printf("File size %d octet\n",fileStats.st_size);
    fd = creat(filename,O_WRONLY | S_IRWXU | S_IRWXG | S_IRWXO);
    for(sizeRemaining = fileStats.st_size; sizeRemaining;){
        sizeReceived = recv(sock,buff,min(sizeRemaining,BUFFSIZE),0);
        write(fd,buff,min(sizeRemaining,BUFFSIZE));
        
        sizeRemaining -= sizeReceived;
        printf("\rreceived %3d%%",100 - (sizeRemaining * 100)/fileStats.st_size);
    }

    printf("\r\n%s downloaded sucessfully\n",filename);
    free(filename);
}
char *GetText(int sock,Data data)
{
    char *result = malloc(data.len);
    recv(sock, result, data.len,0);
    return result;
}
Client *GetClient(int sock,Data data)
{
    char *message;
    Client *client = malloc(sizeof *client);
    client->name = malloc(data.len);
    message = GetText(sock,data);
    // if(message == NULL);
    //     return NULL;
    sscanf(message,"%d:%s",&client->id,client->name);
    free(message);
    return client;
}

Client **AddClient(Client **clients,char *name,int sock,int *count)
{
    Client *nouveau;
    nouveau = malloc(sizeof(*nouveau));
    nouveau->id = *count;
    nouveau->name = strdup(name);
    nouveau->sock = sock;
    return AddClientPointer(clients,nouveau,count);
}

Client **AddClientPointer(Client **clients,Client *client,int *count)
{
    if(client == NULL)
        return clients;
    if(clients == NULL)
        {
            clients = malloc(sizeof(*clients));
            *count = 0;
        }
    else
        clients = realloc(clients, (*count + 1) * sizeof(*clients));
    clients[*count] = client;
    (*count)++;
    return clients;
}