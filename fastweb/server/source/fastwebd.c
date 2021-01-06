#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int	errno;

#define UDP_SERV	0
#define TCP_SERV	1

#define NOSOCK		-1

struct service {
    char *sv_name;
    char sv_useTCP;
    int sv_sock;
    int (*sv_func)(int);
};

int TCPcomm(int);

int passiveTCP(const char *service,int qlen);
int passiveUDP(const char *service);

int errexit(const char *format,...);

void doTCP (struct service *psv);
void reaper(int sig);

struct service svent[] = 
	{	
		{ "9090", TCP_SERV, NOSOCK, TCPcomm },
		//{ "9191", TCP_SERV, NOSOCK, TCPcommsec },  //secure transfer to be coded later
		{ 0, 0, 0, 0 },
	};
	

#ifndef MAX
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#endif

#define QLEN		5

#define LINELEN		128

extern u_short		portbase; //kkt passivesock() - where are the docs ?

//****************************************************************************
//* The very nice fastweb super server main  programm
//****************************************************************************


int main  (int argc, char *argv[])
{

    struct service *psv,
	    *fd2sv[NOFILE];
    int		fd,nfds;
    fd_set	afds, rfds;
    
    switch (argc) {
    case 1:
	break;

    case 2:
	portbase = (u_short) atoi (argv[1]);
	break;
	
    default:
	errexit("Usage: fastwebd [portbase]\n");
	
    }
    
    nfds = 0;
    FD_ZERO(&afds);
    for (psv = &svent[0]; psv->sv_name; ++psv) {
	    if (psv->sv_useTCP)
		    psv->sv_sock = passiveTCP(psv->sv_name, QLEN);
	    else
		    psv->sv_sock = passiveUDP(psv->sv_name);
	
	    fd2sv[psv->sv_sock] = psv;
	    nfds = MAX(psv->sv_sock+1,nfds);
	    FD_SET(psv->sv_sock, &afds);
	    
	}
	(void) signal(SIGCHLD, reaper);
	
	while (1) {
	    memcpy(&rfds, &afds, sizeof(rfds));
	    if (select (nfds, &rfds, (fd_set *)0, (fd_set *)0,
					(struct timeval *)0) < 0) {
			    if (errno == EINTR)
				    continue;
			    errexit ("Select error: %s\n", strerror(errno));
		    }
		    for (fd=0; fd<nfds; ++fd)
			    if (FD_ISSET(fd, &rfds)) {
				psv = fd2sv[fd];
				if (psv->sv_useTCP)
					doTCP(psv);
				else
				    psv->sv_func(psv->sv_sock);
		    }
	}
}

void doTCP (struct service *psv)
{

	struct sockaddr_in fsin;
	int alen;
	int fd,ssock;
	
	alen = sizeof(fsin);
	ssock = accept(psv->sv_sock, (struct sockaddr *)&fsin, &alen);
	if (ssock < 0)
	    errexit("Accept: %s\n",strerror(errno));
	else 
	    fprintf (stdout,"Connection from %s:%d \n",inet_ntoa(fsin.sin_addr.s_addr),ntohs(fsin.sin_port));
	    
	switch (fork()) {
	case 0:
	    break;
	case -1:
	    errexit("Unable to create a new process : %s\n",strerror(errno));
	default:
	    (void) close(ssock);
	    return;
	}
	
	// hmmm ... bleah ... don't know what to say here ... something about child
	
	for (fd = NOFILE; fd >= 0; --fd);
		if (fd != ssock) 
			(void) close(fd);
	exit (psv->sv_func(ssock));
}

// zombie children :)

void reaper(int sig)
{
    int status;
    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
    
}

