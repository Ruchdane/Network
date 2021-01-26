#include "reseaux.h"
int sock;
void end(int SIG)
{
	close(sock);
	exit(0);
}

int main(int argc, char *argv[])
{
	int  count;    
	char nom[225];
	Data data;
	Client **clients = NULL,*tmp;
	signal(SIGSEGV,end);
	signal(SIGINT,end);
	printf("Donnez un identifiant(votre nom) :");
	scanf("%s", nom);
	if(argc == 3)
		sock = createSocket(atoi(argv[1]),argv[2],1);
	else
		sock = createSocket(36002, HOME, 1);

	printf("Connecter\n");
	SendText(sock,nom);
	while (true)
	{
		read(sock,&data,sizeof(data));
		switch (data.type)
		{
			case Id:
				tmp = GetClient(sock,data);
				if(tmp != NULL)
				{
					AddClientPointer(clients, tmp, &count);
					printf("%s joined\n",tmp->name);
				}
			break;
			case ClientListe:
				for (int i = data.len ; i > 0 ; i--)
				{
					read(sock,&data,sizeof(data));
					tmp = GetClient(sock,data);
					if(tmp != NULL)
					{
						AddClientPointer(clients, tmp, &count);
						printf("%s Was here\n",tmp->name);
					}
				}
			break;
			case File:
			printf("Incoming File ...\n");
				GetFile(sock);
			break;
		}
	}
	close(sock);
	return 0;
}
