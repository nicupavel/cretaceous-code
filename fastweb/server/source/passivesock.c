// passivesock.c 

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

extern int errno;

int errexit(const char *format, ...);

u_short portbase = 0;


int passivesock(const char *service, const char *transport, int qlen)
{

    struct servent *pse;
    struct protoent *ppe;
    struct sockaddr_in sin;
    int s, type;
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    
    // service name to port number
    
    if ( pse = getservbyname(service, transport) )
	    sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);
    else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
	    errexit("Can't get \"%s\" service entry\n",service);
	    

    // protocol name to protocol number
    
    if ( (ppe = getprotobyname(transport)) == 0)
	    errexit("Can't get \"%s\" protocol entry\n",transport);
	    

    // chose socket by protocol name
    
    if (strcmp(transport,"udp") == 0 )
	    type = SOCK_DGRAM;
    else 
	    type = SOCK_STREAM;
	    

    // allocate a socket ... or at least try :)
    
    s = socket (PF_INET, type, ppe->p_proto);
    if (s < 0)
	errexit("Can't get socket entry %s\n",strerror(errno));
	
    // bind the socket 
    
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	    errexit("Can't bind to %s port: %s \n",service,strerror(errno));
	
    if (type == SOCK_STREAM && listen(s, qlen) < 0 )
	    errexit("Can't listen on %s port: %s \n",service,strerror(errno));
    
    fprintf (stdout,"FASTWEB Server succesfuly binded to %s port and waiting for connections \n\n",service);

    return s;

}
