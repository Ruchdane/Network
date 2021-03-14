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

struct AdapterList *GetIPadress(int *count)
{
    struct AdapterList *result;
    #if defined(_WIN32)//initialise winsocket
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        log("Failed to initialize winsoket api");
        return NULL;
    }
    #endif

    #if defined(_WIN32)//get adapters or adresses
    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        /*TO DO create verification method for malloc*/

        // if (!adapters) {
        //     log("Couldn't allocate %ld bytes for adapters");
        //     WSACleanup();
        //     return -1;
        // }

        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0,adapters, &asize);
        if (r == ERROR_BUFFER_OVERFLOW) {
            free(adapters);
            adapters == NULL;
        }

        else if (r == ERROR_SUCCESS) break;

        else {
            printf("Error from GetAdaptersAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            return -1;
        }
    } while (!adapters);
    #else
    struct ifaddrs *adapters;

    if (getifaddrs(&adapters) == -1) {
        log("getifaddrs call failed\n");
        return NULL;
    }
    #endif
    
    #if defined(_WIN32)
    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while (adapter) {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            printf("\t%s",
                    address->Address.lpSockaddr->sa_family == AF_INET ?
                    "IPv4" : "IPv6");

            char ap[100];

            getnameinfo(address->Address.lpSockaddr,
                    address->Address.iSockaddrLength,
                    ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);

            address = address->Next;
        }

        adapter = adapter->Next;
    }

    #else
    struct ifaddrs *address = adapters;
    while(address) {
        if (address->ifa_addr == NULL) { 
            address = address->ifa_next;
            continue;
        }
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {

            printf("%s\n\t", address->ifa_name);
            printf("%s ", family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];
            const int family_size = family == AF_INET ?
                sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,
                    family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("%s\n", ap);

        }
        address = address->ifa_next;
    }


    #endif
    
    #if defined(_WIN32)//clean up
    free(adapters);
    WSACleanup();
    #else
    freeifaddrs(adapters);
    #endif
    
}