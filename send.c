#include "reseaux.h"
char *path;
void *ClientThread(void *);
struct MS
{
	int server;
	int count;
	Client **clients;
} *ManagerServer;

void end(int SIG)
{

	printf("End called\n");
	for (int i = 0; i < ManagerServer->count; i++)
	{
		close(ManagerServer->clients[i]->sock);
		pthread_cancel(ManagerServer->clients[i]->cthread);
		sem_close(&ManagerServer->clients[i]->lock);
		free(ManagerServer->clients[i]);
	}
	free(ManagerServer->clients);
	close(ManagerServer->server);
	free(ManagerServer);
	exit(0);
}

int main(int argc, char const *argv[])
{

	struct sockaddr_in addrClient;
	int client;
	Client *tmp;
	Data data;
	socklen_t size = sizeof(struct sockaddr);
	if(argc != 2 && argc != 3)
	{
		printf("send usage\n-send <File name>\n-send <File name> <Pasword>\n");
		exit(0);
	}
	path = (char *)argv[argc - 1];
	signal(SIGINT,end);
	// signal(SIGSEGV,end);
	ManagerServer = malloc(sizeof *ManagerServer);
	ManagerServer->clients = NULL;
	ManagerServer->count = 0;
	ManagerServer->server = createSocket(36002,NULL,0);    
	printf("Votre addrese IPv4 sur ");
	GetIPadress();
	printf("Binded\n");
	while (true)
	{

		listen(ManagerServer->server,5);
		printf("Listening\n");
		client = accept(ManagerServer->server, (struct sockaddr*)&addrClient, &size);
		if(client == -1)
		{
			printf("Socket occupe");
			end(0);
		}
		read(client,&data,sizeof(data));
		if(data.type == Test && (argc == 2 || strcmp(GetText(client,data),argv[2])))
		{
			ManagerServer->clients = AddClient(ManagerServer->clients,"Ok",client,&ManagerServer->count);
			tmp = ManagerServer->clients[ManagerServer->count - 1];
			if(sem_init(&tmp->lock, 0,1) != 0) fprintf(stderr,"Ereur lors de la creation de la semaphore\n");
			printf("nouvelle conection\n",tmp->name);
			pthread_create(&tmp->cthread,NULL,ClientThread,tmp);
		}
		else
		{
			SendText(client,"Sorry");
			close(client);
		}
	}
	end(0);
	return 0;
}

void *ClientThread(void *param)
{
	Data data;
	Client *client = (Client *)param;
	SendFile(client->sock,path,path);
}