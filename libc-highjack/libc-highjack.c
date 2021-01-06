/* Compile with gcc -c libc-highjack.c and add the so to ld.so.preload */
/* Politically correct compiling:
gcc -Wall -c libc-highjack.c -shared -Wl,-soname,highjack.so -o highjack.so -lc
*/

/* npavel@linuxconsulting.ro */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* change this to point to your libc shared lib path */
#define LIB_PATH "/lib/x86_64-linux-gnu/libc.so.6"

int   connect(int   sockfd,  const struct  sockaddr  *serv_addr,  
		socklen_t addrlen)
{
	void *handle;
	char *p;
	int   (*real_connect)(int   , const struct  sockaddr  *, socklen_t);
	struct sockaddr_in highjack;
	handle = dlopen(LIB_PATH, 1);
	if(!handle)
		return -1;
	#if __linux__ || (__svr4__ && __sun__) || sgi || __osf__
		real_connect = dlsym(handle, "connect");
	#else
		real_connect = dlsym(handle, "_connect");
	#endif
	if(!real_connect || ((struct sockaddr_in*)serv_addr)->sin_family !=AF_INET)
		return -1;
	highjack.sin_family=AF_INET;
	highjack.sin_port=(p=getenv("HIGHJACK_PORT"))?htons(atoi(p)):0;
	highjack.sin_addr.s_addr=(p=getenv("HIGHJACK_IP"))?inet_addr(p):0;

      
	bind(sockfd, (struct sockaddr *)&highjack,sizeof(highjack));
	return real_connect(sockfd,serv_addr,addrlen);
	                                    
}

