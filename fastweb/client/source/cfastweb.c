#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern int		errno;


int TCPservice(const char *host, const char *service);
int errexit(const char *format, ...);
int connectTCP(const char *host,const char *service);
void parseHTML(const char *tag);

#define LINELEN		128

char *COMM = "server.dat";    

int main(int argc, char *argv[])
{
    char *host = "localhost";
    char *service = "9090";
    
    
    switch (argc) {
    case 1:
	    host = "localhost";
	    break;
    case 4:
	  service = argv[3];
	  host = argv [2];
	  COMM = argv[1];    
	  break;
    case 3:
	  host = argv [2];
	  COMM = argv[1];    
	  break;	    
    case 2:
	  COMM = argv[1];    
	  break;
    default:
	    fprintf(stderr,"Usage: cfastweb [file] [host [port]]\n");
	    exit(1);
    }
    

    TCPservice(host,service);
    exit(0);
}

int TCPservice(const char *host, const char *service)
{

    char buf[LINELEN+1];
    int s, n;
    
    s = connectTCP(host, service);
    
    (void) write (s,COMM,strlen(COMM));
    
    while ( (n=read(s, buf, LINELEN)) > 0) {
	    buf[n] = '\0';
	    parseHTML(buf);
    }
 exit (0);
}