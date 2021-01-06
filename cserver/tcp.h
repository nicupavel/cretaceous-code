
#define TCP_ERROR	-1
#define TCP_NOSOCK	-2
#define TCP_NOBIND	-3
#define TCP_NOLISTEN	-4
#define TCP_NOACCEPT	-5
#define TCP_NOSEND	-6
#define TCP_NORECV	-7
#define TCP_TIMEOUT	-8

#define COMM_SIZE  255
#define COMM_LOGON_OK "Logon OK"

typedef struct _tcp_pthread_args 
{
    client_info *client;
    _list *clients;
} tcp_pthread_args;

/* public functions */
int tcp_server_initialize (const server_config_tag * const configuration);
int tcp_server_process (_list * clients);
void tcp_server_shutdown (void);
