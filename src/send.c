#include "reseaux.h"
char *path;
void *ClientThread(void *);
struct MS
{
	int server;
	int count;
	Client **clients;
} *ManagerServer;

static void end(int SIG)
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
	RPlatform(WSACleanup(),);
	exit(0);
}

static void print_usage(){
	printf(" Usage : send  [-s] [-p  <Port>]  <File Path>\n");
	printf(" Options:\n");
	printf("  -s secure upload\n  -p set alternative port to use\n");
}

int main(int argc, char *argv[])
{

	struct sockaddr_in addrClient;
	int client;
	Client *tmp;
	Data data;
	char option,*port = strdup("36002"), *passwd,*sentpasswd,*name;
	bool isSecure = false;
	socklen_t size = sizeof(struct sockaddr);
	if(argc == 1){
		print_usage();
		return 0;
	}
	while((option = getopt(argc,argv,"sp: "))!= -1) 
		switch (option){
			case 's':
				isSecure = true;
				break;
			case 'p':
				free(port);
				port = strdup(optarg);
				break;
			default:
				printf("Unknown option '%c'\n",option);
				print_usage();
				return 0;
				break;
			}
	if(!argv[optind]) {
		printf("The file path is necessary\n");
		print_usage();
		return 0;
	}

	path = argv[optind];
	if(access(path,F_OK)){
		printf("File does't exist\n");
		free(port);
		return 0;
	}

	if(isSecure) passwd = getpass("Entrez le mot de passe :");

	signal(SIGINT,end);
	ApiInit();
	// signal(SIGSEGV,end);
	ManagerServer = malloc(sizeof *ManagerServer);
	ManagerServer->clients = NULL;
	ManagerServer->count = 0;
	ManagerServer->server = createSocket(port,NULL,0);    
	
	DisplayNetworkInfo();
	printf("Binded to port %s\n",port);
	while (true)
	{
		listen(ManagerServer->server,5);
		printf("Listening\n");
		client = accept(ManagerServer->server, (struct sockaddr*)&addrClient, &size);
		printf("New CLient trying to connect... \n");
		
		if(client == -1)
		{
			Log("Socket occupe");
			end(0);
		}
		recv(client,&data,sizeof(data),0);
		if(data.type == Test)
			name = GetText(client,data);
		recv(client,&data,sizeof(data),0);
		if(data.type == Test) 
		{
			if(isSecure){
				sentpasswd = GetText(client,data);
				if(strcmp(passwd,sentpasswd)){
					printf("%s gave the wrong password \nConection  refused\n",name);
					SendText(client,"Sorry");
					close(client);
					free(name);
					continue;
				}
				free(sentpasswd);
			}
			ManagerServer->clients = AddClient(ManagerServer->clients,name,client,&ManagerServer->count);
			tmp = ManagerServer->clients[ManagerServer->count - 1];
			if(sem_init(&tmp->lock, 0,1) != 0) fprintf(stderr,"Ereur lors de la creation de la semaphore\n");
			pthread_create(&tmp->cthread,NULL,ClientThread,tmp);
			free(name);
		}
	}
	end(0);
	return 0;
}
 
void *ClientThread(void *param)
{
	Data data;
	Client *client = (Client *)param;
	char * filename = strrchr(path,'/');
	filename = filename == NULL ? path: filename + 1;
	printf("Sending %s to %s\n",filename,client->name);
	SendFile(client->sock,path);
	pthread_exit(NULL);
}