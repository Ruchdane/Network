#include "reseaux.h"
SOCKET sock;
static void end(int SIG)
{
	CLOSESOCKET(sock);
	exit(0);
}

static void print_usage(){
	printf(" Usage : receive  [-s] [-p  <Port>] [-d <destination path>] <IP address> \n");
	printf(" Options:\n");
	printf("  -s secure download\n  -p set alternative port used by the sender\n  -d set destination directory\n");
}

int main(int argc, char *argv[])
{ 
	ssize_t result;
	Data data;  
	char option,*port = strdup("36002"), *passwd,*path = NULL;
	bool isSecure = false;
	ApiInit();
	//Argparsing
	if(argc == 1){
		print_usage();
		return 0;
	}
	while((option = getopt(argc,argv,"sp:d:"))!= -1) 
		switch (option){
			case 's':
				isSecure = true;
				break;
			case 'p':
				free(port);
				port = strdup(optarg);
				break;
			case 'd':
				printf("directory is %s\n",optarg);
				path = strdup(optarg);
				break;
			default:
				printf("Unknown option '%c'\n",option);
				print_usage();
				return 0;
				break;
			}
	if(!argv[optind]) {
		printf("The ip addres is necessary\n");
		print_usage();
		return 0;
	}
	//Argparsing end
	if(path && !IsDir(path)){
		printf("%s is not a directory\n");
		free(port);
		return 0;
	}
	sock = createSocket(port,argv[optind],1);
	free(port);
	if(ISINVALIDSOCKET(sock)){
		Log("create socket failed ");
		return EXIT_FAILURE;
	}
	printf("Connecter %d\n",sock);
	SendText(sock,getenv("USER"));
	if(isSecure){
		passwd = getpass("Entrez le mot de passe :");
		SendText(sock,passwd);
	}
	else
		SendText(sock,"Ok");
	recv(sock,&data,sizeof(data),0);


	if(data.type == File)
	{
		printf("Incoming File ...\n");
		GetFile(sock,path);
	}
	else
		printf("Conection refused by server\n");
	CLOSESOCKET(sock);
	RPlatform(WSACleanup(),);
	return 0;
}
