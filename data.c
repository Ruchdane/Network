#include "data.h"

ssize_t SendText(int sock,char *message)
{
    Data data;
    data.type = Test;
    data.len = strlen(message) + 1;
    write(sock, &data, sizeof(data));
    return write(sock, message,data.len);
}
ssize_t SendClient(int sock,Client *client)
{
    Data data;
    char message[225];

    sprintf(message,"%d:%s",client->id,client->name);
    data.type = Id;
    data.len = strlen(message) + 1;
    
    write(sock, &data, sizeof(data));
    return write(sock, message,data.len);
}

ssize_t SendFile(int sock,char *pathname)
{
    Data data;
    data.type = File;
    int fd = open(pathname, O_RDONLY);
    int  sizeSent,sizeRemaining;
    char *filename = strrchr(pathname,'/');
    off_t offset = 0;
    if (fd == -1)
    {
        printf("%d:File %s not found",errno,pathname);
        return -1;
    }
    struct stat fileStats;

    fstat(fd,&fileStats);
    write(sock,&data,sizeof(data));
    write(sock,&fileStats,sizeof(struct stat));
    filename = filename == NULL ? pathname: filename + 1;
    SendText(sock,filename);
    sizeSent = sendfile(sock,fd,NULL,fileStats.st_size);
    close(fd);
}

void GetFile(int sock)
{
    Data data;
    struct stat fileStats;
    int fd ;
    off_t sizeRemaining,sizeSent,offset;
    char *file,*filename;

    read(sock,&fileStats,sizeof(struct stat));
    file = malloc(fileStats.st_size);
    read(sock,&data,sizeof(data));
    filename = GetText(sock,data);
    fd = creat(filename,O_WRONLY | S_IRWXU | S_IRWXG | S_IRWXO);
    for(sizeRemaining = fileStats.st_size; sizeRemaining;sizeRemaining -= sizeSent)
    {
        sizeSent = recv(sock,file,fileStats.st_size,0);
        write(fd,file,sizeSent);
        printf("\rreceived %3d%%",100 - (sizeRemaining * 100)/fileStats.st_size);
    }
    printf("%s downloaded sucessfully\n",filename);
    free(filename);
    free(file);
}
char *GetText(int sock,Data data)
{
    char *result = malloc(data.len);
    read(sock, result, data.len);
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
    nouveau->name = name;
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