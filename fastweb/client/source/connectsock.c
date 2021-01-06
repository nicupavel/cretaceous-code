//No comments for now see if compiles ....

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

/* defining INADDR_NONE */
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif 

extern int errno;

int errexit (const char *format,...);

int connectsock (const char *host,const char *service, const char *transport)
{

    struct hostent *phe;
    struct servent *pse;
    struct protoent *ppe;
    struct sockaddr_in sin;
    int s, type;
    
    memset (&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    
    // Map service to port number
    
    if (( pse = getservbyname(service, transport) ))
	    sin.sin_port = pse->s_port;
    else if ( (sin.sin_port =htons((u_short)atoi(service)))==0)
	    errexit("Can't get \"%s\" service entry\n",service);
	    

    // Map host name to ip address
    
    if (( phe = gethostbyname(host) ))
	    memcpy (&sin.sin_addr,phe->h_addr,phe->h_length);
    else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
	    errexit ("Can't get \"%s\" host entry\n",host);
	    
    // Map transport protocol name to transport protocol number
    
    if ( (ppe = getprotobyname (transport)) == 0 )
	errexit ("Can't get \"%s \" protocol entry",transport);

    // Use protocol to chose a socket
    
    if (strcmp (transport, "udp") == 0 )
	    type = SOCK_DGRAM;
    else 
	    type = SOCK_STREAM;
	    
    // Allocate a socket
    
    s = socket (PF_INET, type, ppe->p_proto);
    if (s < 0) 
	errexit ("Can't create socket: %s\n", strerror(errno));
	

    // Connect the socket
    
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	    errexit ("Can't connect to %s.%s: %s\n", host, service, strerror(errno));
    return s;
    
}
