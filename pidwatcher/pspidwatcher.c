#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

#define BUF (32*sizeof(struct proc_struct))

#define DEBUG 1


struct proc_struct {
	/* not always true PAGE_SIZE != 4K */
        char environ[8192];
	unsigned long cpu;
        pid_t pid;
	unsigned int environ_length;
        /* etc */
};
float pcpu;

int main(void)
{
        int fd, len, i,j;
        char buf[BUF];
	
	char *string;
	char *match="MediaBoxModuleName=";
	
	string = (char *) malloc (8192);
	memset (string,0,8192);

        if ((fd = open("/dev/pidwatcher/pidwatcher", O_RDONLY)) == -1) {
                perror("open");
                exit(EXIT_FAILURE);
        }

        while ((len = read(fd, buf, BUF)) > 0)
                for (i=0; i<len / sizeof(struct proc_struct); i++) {
			pcpu = (float)((struct proc_struct *)buf)[i].cpu/10;
#ifdef ALL
                        printf("PID=%d, CPU=%.1f, ENVIRON=",
                               ((struct proc_struct *)buf)[i].pid,
			       pcpu);
#ifdef DEBUG
			printf ("environ length=%d \n",((struct proc_struct *)buf)[i].environ_length);
#endif
#endif
			int k=0;
			for (j=0;j < ((struct proc_struct *)buf)[i].environ_length;j++) {
			    if (((struct proc_struct *)buf)[i].environ[j] != '\0') {
				string[k] = ((struct proc_struct *)buf)[i].environ[j];
				k++;
			    }
			    else {
				k = 0;
				string = &string[0];
				if (strstr(string,match) != NULL) { 
				    printf("PID=%d, CPU=%.1f ",((struct proc_struct *)buf)[i].pid, pcpu);
				    printf ("%s\n",string);
				    break;
				}
				else {
#ifdef ALL
					printf ("%s\n",string);
#endif
				    memset(string,0,8192);
				}
			    }
			    
                             //printf ("%c", ((struct proc_struct *)buf)[i].environ[j]);
			}
#ifdef ALL
			printf ("\n");
#endif
		}
		
	free (string);
        if ((fd = close(fd)) == -1) {
                perror("close");
                exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
}
