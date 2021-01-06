#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define CMDDELAY 5
#define CMDFIFO  "mplayer-fifo"

int main (void)
{
    char const * commands[] = {
        "pause",
        "osd",
        "volume",
	"pause",
        "quit",
        NULL
    };
    
    char const ** cmd = commands;
    
    int mode = S_IRUSR | S_IWUSR | S_IRGRP;
    //int open_flags = O_WRONLY | O_NONBLOCK;
    int open_flags = O_NONBLOCK;
    int rc;
    int fd = -1;
        
    rc = mkfifo(CMDFIFO, mode);
    
    if ( rc != -1) {
	fprintf(stderr, "File exists.\n");
    }
    
    while (*++cmd != NULL) {
	fd = open(CMDFIFO, open_flags, mode);
	if (fd == -1)  {
	    fprintf(stderr, "Cannot open fifo for writing.\n");
    	    return -1;  
	}
    
	fprintf(stderr,"%s%s\n", "Sending: ", *cmd);
	write (fd, *cmd, strlen(*cmd));
	close(fd);
        sleep(CMDDELAY);        
    }
    
    return 0;    
}
