#include "reseaux.h"

int createSocket(int port,char *adr,int type)
{
    int sock,i;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&(int){1},SOCK_STREAM);
    if(sock == -1){
        fprintf(stderr,"Ereur Lors de la Creation du socket\n");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 
    if(adr == NULL)
        addr.sin_addr.s_addr = htons(INADDR_ANY);
    else
        inet_aton(adr, &addr.sin_addr);

    // InfoSocket(sock);
    if(!type) 
       i = bind(sock,(struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    else
       i = connect(sock,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
    if(i < 0)
    {
        close(sock);
        fprintf(stderr,"%d:%s\n",errno,strerror(errno));
        return -1;
    }
    InfoSocket(sock);

    return sock;
}

int InfoSocket(int sock)
{
    struct sockaddr_in adresse;
    socklen_t longueur;
    longueur = sizeof(struct sockaddr_in);
    if (getsockname(sock, (struct sockaddr*)&adresse, &longueur) < 0)
    {
        fprintf(stderr, "Erreur getsockname\n");
        return -1;
    }
    printf("IP = %s, Port = %u\n", inet_ntoa(adresse.sin_addr),ntohs(adresse.sin_port));
    
    return 0;
}

char *GetIPadress()
{
    struct ifaddrs *addrs,*tmp;
    getifaddrs(&addrs);
    tmp = addrs;

    while (tmp) 
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
        }

            tmp = tmp->ifa_next;
    }

    freeifaddrs(addrs);
    return NULL;
}