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
    #ifdef _WIN32
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,"1",SOCK_STREAM);
    #else
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,NULL,SOCK_STREAM);
    #endif // DEBUG
    log("sdqd");
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

struct AdapterList *GetAdapter(int *count)
{
    struct AdapterList *result = NULL;
	char ap[100];
    *count = 0;
    #if defined(_WIN32)//initialise winsocket
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        log("Failed to initialize winsoket api");
        return NULL;
    }
    

    //get adapters or adresses
    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        /*TO DO create verification method for malloc*/

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
            return NULL;
        }
    } while (!adapters);
    PIP_ADAPTER_ADDRESSES adapter = adapters;
    #else
    struct ifaddrs *adapters;

    if (getifaddrs(&adapters) == -1) {
        log("getifaddrs call failed\n");
        return NULL;
    }
    struct ifaddrs *adapter = adapters;
    #endif

    while (adapter) {
        result = realloc(result,(++(*count)) * sizeof(* result));
        #if defined(_WIN32)//Looping through adresses
        result[*count - 1].nom = malloc(strlen((char*)adapter->FriendlyName));
        sprintf(result[*count - 1].nom,"%S",adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            getnameinfo(address->Address.lpSockaddr,address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
			if(address->Address.lpSockaddr->sa_family == AF_INET) result[*count - 1].IPV4 = strdup(ap);
			else result[*count - 1].IPV6 = strdup(ap);
            address = address->Next;
        }

        adapter = adapter->Next;
    }

        #else
        if (adapter->ifa_addr == NULL) { 
            adapter = adapter->ifa_next;
            (*count)--;
            continue;
        }
        int family = adapter->ifa_addr->sa_family;
        socklen_t size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

        if(getnameinfo(adapter->ifa_addr, size , ap, sizeof(ap), 0, 0, NI_NUMERICHOST)){
            result[*count - 1].nom = strdup(adapter->ifa_name);
            result[*count - 1].IPV6 = NULL;
            result[*count - 1].IPV4 = NULL;
            goto skip;//If the the host nam is unable to be resolved continue
        }
        else for(int i = 0;i < *count;i++)
            if(!strcmp(result[i].nom,adapter->ifa_name)){
            if(family == AF_INET6) result[i].IPV6 = strdup(ap);
            else result[i].IPV4 = strdup(ap);
            (*count)--;
            goto skip;//If the adapter was already in just input the IPv6 output and continue
        }
        
        result[*count - 1].nom = strdup(adapter->ifa_name);
        result[*count - 1].IPV6 = NULL;
        result[*count - 1].IPV4 = NULL;
        
        if(family == AF_INET6) result[*count - 1].IPV6 = strdup(ap);
        else result[*count - 1].IPV4 = strdup(ap);
        
        skip:adapter = adapter->ifa_next;
    }

    #endif
    
    #if defined(_WIN32)//clean up
    free(adapters);
    WSACleanup();
    #else
    freeifaddrs(adapters);
    #endif
    return result;
}

void DisplayNetworkInfo(){
	int count;
	struct AdapterList *adapter = GetAdapter(&count);
	for (int i = 0; i < count; i++){
		printf("\n%s\n",adapter[i].nom,adapter[i].IPV6);
        if(adapter[i].IPV4) printf("\tIPv4: %s\n",adapter[i].IPV4);
        if(adapter[i].IPV6) printf("\tIPv6: %s\n",adapter[i].IPV6);
		free(adapter[i].IPV6);free(adapter[i].nom);free(adapter[i].IPV4);
	}
	free(adapter);
}