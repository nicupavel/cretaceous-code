void udpinit (void);
int udpconnect (char * address, int port);
int udpsend (int sock, char * message);
int udprecv (int sock, char * buffer, long int buffer_size);
void udpshutdown (void);
