#include <stdio.h>

#include "sighandler.h"
#include "config.h"
#include "util.h"
#include "list.h"
#include "client.h"
#include "sql.h"
#include "tcp.h"
#include "security.h"

#include "cserver.h"

_global global;

int main(void)
{
	int result;
	_list *clients;
	
	server_config_tag *configuration;
	
	sighandler_initialize();
	config_initialize();
	
	config_parse_file("conf/cserver.xml");
	configuration = config_get_config();
	
#ifdef DEBUG
	config_dump(configuration);
#endif

	security_initialize(configuration);
	
	list_initialize(free);
	clients = list_get_list();
	
	global.running = CSERVER_RUNNING;
	
#ifdef HAVE_MYSQL
	if (sql_initialize()) {
	    fprintf (stderr,"Cannot connect to mysql database.\n");
	    return 1;
	}
#endif

	if ((result = tcp_server_initialize(configuration)) < 0) {
	    fprintf (stderr,"Cannot start server: %d.\n",result);
	    return 1;
	}
	
	while (global.running == CSERVER_RUNNING) {
	    if ((result = tcp_server_process(clients)) < 0) 
		fprintf (stderr,"Cannot process client: %d\n",result);
	}
	    
	tcp_server_shutdown();
	list_destroy(clients);
	
	return 0;
}
