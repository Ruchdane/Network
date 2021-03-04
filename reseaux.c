#include "reseaux.h"

SOCKET createSocket(char* port,char *adr,int type){
    int sock,connection;
    struct addrinfo hints,*addr;
    
    memset(&hints, 0, sizeof(hints));


    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if(getaddrinfo(adr ? adr:INADDR_ANY,port,&hints,&addr)){
        log("getaddrinfo() echec");
        return 1;
    }
        
    sock = socket(addr->ai_family,addr->ai_socktype,addr->ai_protocol);
    if(ISINVALIDSOCKET(sock)){
        log("Ereur Lors de la Creation du socket");
        return -1;
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&(int){1},SOCK_STREAM);

    if(!type) connection = bind(sock,addr->ai_addr,addr->ai_addrlen);
    
    else connection = connect(sock,addr->ai_addr,addr->ai_addrlen);
    
    if(connection){
        CLOSESOCKET(sock);
        log("Erreur Lors de la conection");
        return -1;
    }
    InfoSocket(sock);

    return sock;
}

int InfoSocket(int sock){
    struct sockaddr_in adresse;
    socklen_t longueur;
    longueur = sizeof(struct sockaddr_in);
    if (getsockname(sock, (struct sockaddr*)&adresse, &longueur) < 0)
    {
        log("Erreur getsockname ");
        return -1;
    }
    printf("IP = %s, Port = %u\n", inet_ntoa(adresse.sin_addr),ntohs(adresse.sin_port));
    
    return 0;
}

char *GetIPadress()
{
    
}