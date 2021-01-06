#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "cserver.h"

#include "sighandler.h"

static void _sig_hup(int signo);
static void _sig_die(int signo);

void sighandler_initialize(void)
{
	signal(SIGHUP, _sig_hup);
	signal(SIGINT, _sig_die);
	signal(SIGTERM, _sig_die);
	signal(SIGPIPE, SIG_IGN);
}

static void _sig_hup(int signo)
{
	fprintf(stderr,"Caught signal %d, rehashing config and reopening logfiles (unimplemented)...\n", signo);

	/* reread config file */

	/* reopen logfiles */

	signal(SIGHUP, _sig_hup);
}

static void _sig_die(int signo)
{
	fprintf(stderr,"Caught signal %d, shutting down ...\n", signo);
	global.running = CSERVER_STOPPING;
}

void _sig_block(void)
{
        sigset_t ss;

        sigfillset(&ss);

        /* These ones we want */
        sigdelset(&ss, SIGKILL);
        sigdelset(&ss, SIGSTOP);
        sigdelset(&ss, SIGTERM);
        sigdelset(&ss, SIGSEGV);
        sigdelset(&ss, SIGBUS);
        if (pthread_sigmask(SIG_BLOCK, &ss, NULL) != 0)
                fprintf(stderr,"pthread_sigmask() failed for blocking signals");
}
