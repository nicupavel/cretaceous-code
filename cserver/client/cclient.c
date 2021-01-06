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

struct hostent *hostlist;  

char dotted_ip[15];

int port;                   
int sock;                   
struct sockaddr_in sa;      
socklen_t sa_len;

void signal_handler(int signum)
{
    switch (signum) {
    case SIGINT:
	printf("\nReceived interrupt signal. Exiting.\n");
	close(sock);
	exit(0);
			
    default:
	printf("\nUnknown signal received. Ignoring.\n");
    }
}

int client_login(int sock, int id)
{
    char *sendbuf, recvbuf[1024];
    int length;
    int _id;
    
    sendbuf = (char *) malloc (1024);
    memset (sendbuf, 0, 1024);
    memcpy (sendbuf, &id, sizeof(int));
    length = strlen(sendbuf);
    
    if (sendto(sock, sendbuf, length + 1, 0, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) <= length) {
        close(sock);
        return -1;	
    }
    memcpy (&_id,sendbuf, sizeof(int));
    printf ("Sent id: %d...\n",_id);
    
    if (recvfrom(sock, recvbuf, 255, 0, (struct sockaddr *)&sa, &sa_len) < 0) {
        return -1;
    }

    printf("Got message: '%s'\n", recvbuf);
}

int main(int argc, char *argv[])
{
    int id;
    
    if (argc < 4) {
	printf("Gigi ICQ  client.\n");
	printf("Usage: %s <hostname or IP> <port> <id>\n", argv[0]);
	return 1;
    }

    printf("Looking up %s...\n", argv[1]);
    hostlist = gethostbyname(argv[1]);
    if (hostlist == NULL) {
	printf("Unable to resolve %s.\n", argv[1]);
	return 1;
    }
	
    if (hostlist->h_addrtype != AF_INET) {
	printf("%s doesn't seem to be an IPv4 address.\n",
	       argv[1]);
	return 1;
    }

    inet_ntop(AF_INET, hostlist->h_addr_list[0], dotted_ip, 15);
    printf("Resolved %s to %s.\n", argv[1], dotted_ip);

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
	printf("Unable to create a socket: %s\n",
	       strerror(errno));
	return 1;
    }

    memset(&sa, 0, sizeof(struct sockaddr_in));

    port = atoi(argv[2]);
    sa.sin_port = htons(port);
	
    memcpy(&sa.sin_addr, hostlist->h_addr_list[0], hostlist->h_length);
	
    sa.sin_family = AF_INET;

    printf("Trying %s on port %i...\n", dotted_ip, port);
    if (connect(sock, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
	printf("Unable to connect: %s\n",
	       strerror(errno));
	return 1;
    }

    printf("Connected! Reading data. Press Control-C to quit.\n");
	
    signal(SIGINT, signal_handler);
    
    id = atoi(argv[3]);
    
    printf ("Client id is : %d\n",id);
    
    if (client_login(sock, id) == -1) return;
    
    for (;;) {
	char *sendbuf="teste multe teste BLA BLA BLA BLA", recvbuf[1024];
	int length;

	length = strlen(sendbuf);
	if (sendto(sock, sendbuf, length + 1, 0, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) <= length) {
	    close(sock);
	    return -1;
	}
	printf ("Sent message...\n");
	if (recvfrom(sock, recvbuf, 255, 0, (struct sockaddr *)&sa, &sa_len) < 0) {
	    return -1;
	}

	printf("Got message: '%s'\n", recvbuf);
    
	//sendbuf[0] = '\0';
	//strncpy(sendbuf, recvbuf, 1023);

	sleep (2);
    }
    printf("Closing socket.\n");
    close(sock);

    return 0;
}
