//services.c 

#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BUFFERSIZE	4096
#define LINELEN		72

extern int		errno;
const char		*request;

int sendfile (const char *request,int fd);
int TCPcomm(int);
int errexit(const char *format, ...);



int TCPcomm (int fd)
{
    int comm;
    char request[BUFFERSIZE], *ctime();
    time_t now;
    (void) time(&now);
    
    while (1) {
    comm = read (fd, request, sizeof(request));
    
    fprintf (stdout,"%s\t-- Client requested : %s\n",ctime(&now), request);
    
	if ( sendfile( request, fd ) ) {
    	    fprintf (stdout,"\t-- %s -- success sending file \n\n",request);
	    break;
	    }
        else {
	    fprintf (stderr,"\t-- %s -- error sending file\n\n",request);
	    break;
	    }
	
    }
    return 0;
}
    
// don't know if this is the fastest way

int sendfile (const char *request, int fd)
{

    char *buf;
    char line[BUFFERSIZE];
    int lineno = 0;    
    int i;

    FILE *fp;
    
    if ((fp = fopen (request,"r")) == NULL ) 
	return 0;
    else {	
	while(fgets(line, sizeof(line), fp)) {
            lineno++;
	    buf = line;
	    write(fd, buf, strlen(buf));
	    }
    fclose (fp);
    return 1;
    }
}