#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
    int status,i=0;
    pid_t pid;

    pid=fork();
    if (pid==-1) perror("fork");
    else if (pid==0){
    /* procesul copil */
    for (;;) {
	i++;
	system ("echo ruleaza >> /tmp/tforknb");
	sleep (1);
    }
 	
    } else {  
    /* procesul parinte nu asteapta terminarea procesului copil */
    }
}
 
