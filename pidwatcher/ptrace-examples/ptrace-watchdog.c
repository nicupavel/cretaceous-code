#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <dlfcn.h>

/* binarul pentru care se va face monitorizarea */
#define PROCESS "./tforknb"

int main(int ARGC, char *ARGV[]){
    int  pid, cpid, status;

    pid=fork();
    if (pid==-1) perror("fork");
    else if (pid==0){
    /* Parte de cod care va fi executata doar de copilul procesului principal */
    signal(SIGTRAP, SIG_IGN); 
    if ( ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
	perror("ptrace");
	exit (1);
    }
    cpid = getpid();
    printf("\n[COPIL]Proces Controlat\n");
    printf ("[COPIL]PID-ul processului copil:%d\n",pid);
    printf ("[COPIL]PID-ul processului parinte:%d\n",cpid);
    if (setsid()==-1) perror("setsid");
    if (execlp(PROCESS, PROCESS, NULL)== -1) perror("execlp");
    } else { 

    /* Parte de cod care va fi executat doar de catre processul parinte */
    /* Procesul parinte va astepta pana cand un semnal va fi primit  */
    /* de la procesul copil */
 
    wait(&status);
    if ( WIFSTOPPED(status) ) printf ("\nS-a primit semnalul: %i\n",WSTOPSIG(status) );
    /* Procesul monitorizat este oprit cu sigtrap si procesul parinte */
    /* primeste semnalul.  Procesul este oprit inainte de a reporni   */
    /* Aici se poate adauga cod pentru a schimba comportarea procesului*/
    /* copil. Cu functia sigaction se pot controla si alte semnale care */
    /* sunt primite                                                     */
    
    
    /* Executia este continuata cu PTRACE_CONT care va opri procesul la */
    /* primirea unui alt semnal                                         */
    printf ("[PARINTE]PID-ul procesului:%d\n",pid);
    if (ptrace(PTRACE_CONT, pid, 0, WSTOPSIG(status))==-1) 
	perror("ptrace");
    wait(&status);
     
 /* Se asteapta pana cand procesul copil isi termina executia */
    return 0;
  }
}
 
