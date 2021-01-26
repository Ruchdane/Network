#include "reseaux.h"
int sock;
void end(int SIG)
{
	close(sock);
	exit(0);
}

int main(int argc, char *argv[])
{ 
	Data data;
	
	if(argc != 3 && argc != 2)
    {
        printf("recieve usage\n recieve <IP address>\n");
        printf("send  <IP address> <Pasword>\n");
        exit(0);
    }
    sock = createSocket(36002,argv[1],1);
	printf("Connecter\n");
	if(argc == 3)
        SendText(sock,argv[2]);
    else
        SendText(sock,"Ok");
    read(sock,&data,sizeof(data));
    if(data.type == File)
    {
        printf("Incoming File ...\n");
        GetFile(sock);
    }
    else
        printf("Conection refused by server\n");
	close(sock);
	return 0;
}
