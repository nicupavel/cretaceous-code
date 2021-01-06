#include <stdio.h>
#include <unistd.h>     
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>  
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>

#include "cserver.h"
#include "util.h"
#include "list.h"
#include "client.h"
#include "config.h"

#include "tcp.h"

static int listener;

static struct sockaddr_in sa;
static socklen_t sa_len;

static pthread_mutex_t mutex;



/* private functions */
static void * tcp_client_process (tcp_pthread_args *t_args);
static int tcp_client_logon(client_info *cinfo);

int tcp_server_initialize (const server_config_tag * const configuration)
{
    
    listener = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listener < 0) 
	return TCP_NOSOCK;
    

    sa_len = sizeof(sa);
    memset(&sa, 0, sa_len);
    
    if (!inet_aton(configuration->bind_address, &sa.sin_addr)) 
	return TCP_ERROR;
    else {
	sa.sin_family = AF_INET;
	sa.sin_port = htons(configuration->port);
    }

    if (bind(listener, (struct sockaddr *)&sa, sa_len) < 0) 
	return TCP_NOBIND;
    

    if (listen(listener, 5) < 0) 
	return TCP_NOLISTEN;

    return 0;
}

int tcp_server_process (_list *clients)
{
    int client;
    pthread_t thread_id;
    tcp_pthread_args *t_args;
    client_info * cinfo;

    pthread_mutex_init(&mutex, NULL);
    
    if (util_timed_wait_for_fd(listener, 10000) <= 0) {
    	return TCP_TIMEOUT;
    }

    client = accept(listener, (struct sockaddr *)&sa, &sa_len);
    
    if (client < 0) {
	close (client);
        return TCP_NOACCEPT;
    }
    
    if ((cinfo = (client_info *) malloc (sizeof(client_info))) == NULL)
	return -1;

    if ((cinfo->ip = (char *) malloc (16)) == NULL)
	return -1;
    
    cinfo->descriptor = client;
    
    memset(cinfo->ip,16,0);
    inet_ntop(AF_INET, &sa.sin_addr, cinfo->ip, 15);
    fprintf(stderr,"Received connection from %s.\n", cinfo->ip);
    
    if (tcp_client_logon(cinfo) < 0)
	return -1;

    if ((t_args = (tcp_pthread_args *) malloc (sizeof(tcp_pthread_args))) == NULL)
	return -1;
	
    t_args->client  = cinfo;
    t_args->clients = clients;
    
    pthread_create(&thread_id, NULL,(void *) &tcp_client_process, (tcp_pthread_args *)t_args);    

    return 0;
}

static int tcp_client_logon (client_info *cinfo)
{

    char *commbuf;
    int length;
    
    if ((commbuf = (char *) malloc (COMM_SIZE)) == NULL)
        return -1;
	    
    memset(commbuf, 0, COMM_SIZE);
	    
    if (recvfrom(cinfo->descriptor, commbuf, COMM_SIZE, 0, (struct sockaddr *)&sa, &sa_len) <= 0) {
#ifdef DEBUG
        fprintf (stderr,"Logon:Unable to read all data.\n");
#endif
    	return -1;
    }
    
#ifdef DEBUG
    printf("Logon:Got packet.\n");
#endif
    
    memcpy(&cinfo->id, commbuf, sizeof(int));	
    memset(commbuf, 0, COMM_SIZE);
    snprintf(commbuf, strlen(COMM_LOGON_OK)+1, "%s", COMM_LOGON_OK);
    length = strlen(commbuf);
	
    if (sendto(cinfo->descriptor, commbuf, length + 1, 0, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) <= length) {
#ifdef DEBUG
        fprintf (stderr,"Logon:Unable to send all data.\n");
#endif
	return -1;
    }
    
#ifdef DEBUG
    printf("Logon:Sent packet.\n");
#endif
    free(commbuf);
    return 0;
}


static void * tcp_client_process (tcp_pthread_args *t_args)
{
    char *recvbuf, *data;
    int length;
    
    client_info *cinfo;
    _element *cclient;
    _list *clients;
    
    cinfo = t_args->client;
    clients = t_args->clients;
    
    pthread_mutex_lock(&mutex);
    list_ins_next(clients, NULL, cinfo);
    pthread_mutex_unlock(&mutex);
    
    while (global.running == CSERVER_RUNNING) {
    
	if ((recvbuf = (char *) malloc (COMM_SIZE)) == NULL)
	    break;
	    
	memset(recvbuf, 0, COMM_SIZE);
	
	if (recvfrom(cinfo->descriptor, recvbuf, COMM_SIZE, 0, (struct sockaddr *)&sa, &sa_len) <= 0) {
#ifdef DEBUG
	    fprintf (stderr,"Unable to read all data.\n");
#endif
    	    break;
	}
    
	length = strlen(recvbuf);
	if ((data = (char *) malloc (length + sizeof(int) + 1)) == NULL)
	    break;
	    
	memset(data,0,length + sizeof(int) + 1);
	snprintf(data, length + sizeof(int) + 1, "%d:%s", cinfo->id, recvbuf);
	free(recvbuf);
    
	length = strlen(data);
	
	if (sendto(cinfo->descriptor, data, length + 1, 0, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) <= length) {
#ifdef DEBUG
	    fprintf (stderr,"Unable to send all data.\n");
#endif
	    free(data);
    	    break;
	}
    
	free(data);
    }
    
    close (cinfo->descriptor);
    pthread_mutex_lock(&mutex);
    cclient = list_head(clients);
    
    if (client_find_next_client_id(clients, &cclient, cinfo->id) == -1) 
	fprintf (stderr, "BUG: cannot find client in list.\n");
    else {
	if (list_rem_next(clients, cclient, (void **)&cinfo) == -1)
	    fprintf (stderr, "BUG: Cannot remove client from list.\n");

	free(cinfo);
    }
    pthread_mutex_unlock(&mutex);
    fprintf(stderr, "list size now is : '%lu'\n", list_size(clients));
    free (t_args);
    pthread_exit(0);
}

void  tcp_server_shutdown (void)
{
    close (listener);
}
