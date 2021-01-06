#include <stdio.h>
#include <unistd.h>

#define CMDDELAY 2

int main (void)
{
    int fdpipe[2];
    FILE *fpipe;
    
    char const * commands[] = {
        "pause",
        "osd",
        "volume",
        "quit",
        NULL
    };
    
    char const ** cmd = commands;
    
    if (pipe(fdpipe) == -1) 
        return (1);
    
    if ((fpipe = fdopen(fdpipe[1], "w")) == NULL)
        return (1);  
    
    while (*++cmd != NULL) {
        fprintf(fpipe, "%s\n", *cmd);
        fflush(fpipe);
        sleep(CMDDELAY);        
    }
    
    close(fdpipe[1]);
    return (0);    
}
