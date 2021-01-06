#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "udp.h"


struct sockaddr_in * sa;
socklen_t sa_len;

void 
udpinit (void)
{
    sa = (struct sockaddr_in *) malloc (sizeof (struct sockaddr_in));
}

int 
udpconnect (char * address, int port )
{

    struct hostent *hostlist;
    char *ip;
    int sock;
    
    if ((ip = (char *) malloc (16 * sizeof (char))) == NULL) {
	fprintf (stderr,"Unable to allocate memory for ip address.\n");
	return -1;
    }
    
#ifdef DEBUG
    fprintf(stderr,"Looking up %s...\n", address);
#endif
    hostlist = gethostbyname(address);
    if (hostlist == NULL) {
	fprintf(stderr,"Unable to resolve %s.\n", address);
	return -11;
    }
    
    if (hostlist->h_addrtype != AF_INET) {
	fprintf(stderr,"%s doesn't seem to be an IPv4 address.\n",
	       address);
	return -1;
    }
    
    inet_ntop(AF_INET, hostlist->h_addr_list[0], ip, 15);
#ifdef DEBUG
    fprintf(stderr,"Resolved %s to %s.\n", address, ip);
#endif
    
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    
    if (sock < 0) {
	printf("Unable to create a socket: %s\n",
	       strerror(errno));
	return -1;
    }

    memset(sa, 0, sizeof(struct sockaddr_in));

    sa->sin_port = htons(port);
    memcpy(&sa->sin_addr, hostlist->h_addr_list[0], hostlist->h_length);
    sa->sin_family = AF_INET;
    
    free (ip);
    return sock;    
}


int 
udpsend (int sock, char * message)
{

    if (message == NULL) 
	return -1;
	
    if (sendto(sock, message, strlen(message) + 1, 0, (struct sockaddr *)sa, 
	sizeof(struct sockaddr_in)) <= (int)strlen(message)) 
    {
	printf("Error sending packet: %s\n", strerror(errno));
	close(sock);
	return -1;
    }
    
    return 1;
}

int 
udprecv (int sock, char * buffer, long int buffer_size)
{

    if (recvfrom(sock, buffer, buffer_size, 0, (struct sockaddr *)sa, &sa_len) < 0) {
	    printf("Error receiving packet: %s\n", strerror(errno));
	    return -1;
    }
    
    return 1;
}

void 
udpshutdown (void)
{
    free(sa);
}
