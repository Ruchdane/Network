#include "reseaux.h"
SOCKET sock;
static void end(int SIG)
{
	CLOSESOCKET(sock);
	exit(0);
}

int main(int argc, char *argv[])
{ 
    ssize_t result;
	Data data;
	ApiInit();
	if(argc != 3 && argc != 2)
    {
        printf("recieve usage\n\trecieve <IP address> [Password] [-p  <Port>]\n");
        exit(0);
    }
    sock = createSocket("36002",argv[1],1);
    if(ISINVALIDSOCKET(sock)){
        Log("create socket failed ");
        return EXIT_FAILURE;
    }
	printf("Connecter %d\n",sock);
	if(argc == 3)
        SendText(sock,argv[2]);
    else
        SendText(sock,"Ok");
    recv(sock,&data,sizeof(data),0);
    if(data.type == File)
    {
        printf("Incoming File ...\n");
        GetFile(sock);
    }
    else
        printf("Conection refused by server\n");
    CLOSESOCKET(sock);
    RPlatform(WSACleanup(),);
	return 0;
}
