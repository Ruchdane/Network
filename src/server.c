#include "reseaux.h"
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
		free(ManagerServer->clients[i]->name);
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
	signal(SIGINT,end);
	// signal(SIGSEGV,end);
	ManagerServer = malloc(sizeof *ManagerServer);
	ManagerServer->clients = NULL;
	ManagerServer->count = 0;
	ManagerServer->server = createSocket(36002,HOME,0);    
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
		if(data.type == Test)
		{
			ManagerServer->clients = AddClient(ManagerServer->clients,GetText(client,data),client,&ManagerServer->count);
			tmp = ManagerServer->clients[ManagerServer->count - 1];
			if(sem_init(&tmp->lock, 0,1) != 0) fprintf(stderr,"Ereur lors de la creation de la semaphore\n");
			printf("%s conected\n",tmp->name);
			for (int i = 0; i < ManagerServer->count - 1; i++)
			{
				sem_wait(&ManagerServer->clients[i]->lock);
				SendClient(ManagerServer->clients[i]->sock,tmp);
				sem_post(&ManagerServer->clients[i]->lock);
			}
			pthread_create(&tmp->cthread,NULL,ClientThread,tmp);
		}
		else
			close(client);
	}
	end(0);
	return 0;
}

void *ClientThread(void *param)
{
	Data data;
	Client *client = (Client *)param;
	data.type = ClientListe;
	data.len = ManagerServer->count - 1;

	sem_wait(&client->lock);
	write(client->sock, &data, sizeof data);
	for (int i = 0; i < data.len; i++)
		SendClient(client->sock, ManagerServer->clients[i]);
	sem_post(&client->lock);

	sem_wait(&client->lock);
	SendFile(client->sock,"Beastars");
	sem_post(&client->lock);
}