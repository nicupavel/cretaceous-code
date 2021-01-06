#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "udp.h"

#include "csstats.h"

int 
parse_players_info( char *buffer, long int buffer_size)
{
    int offset = 5, 
	length,
	count = 0;
    player_tag * pplayer;

    server_info->current_players = buffer[offset];
    server_info->player =  (player_tag *) malloc (server_info->current_players * sizeof(player_tag));
    pplayer = server_info->player;
    offset ++;
    printf ("Number of players:%d\n", server_info->current_players);
    
    while (offset < buffer_size) {    
	pplayer->index = buffer[offset];
	offset++;
	
	length = 0;
	while ( buffer[offset+length] != '\0') 
	    length++;
    
	pplayer->name = (char *) malloc (PLAYER_MAX_LENGTH * sizeof (char));
	memset (pplayer->name, 0, PLAYER_MAX_LENGTH);
	memcpy (pplayer->name, &buffer[offset], length);
	offset += length + 1;
	pplayer->frags = buffer[offset] + (buffer[offset+1]<<8) + (buffer[offset+2]<<16) + (buffer[offset+3]<<24);
	offset += 4;
	memcpy(&pplayer->time,&buffer[offset],4);
	offset += 4;
	printf ("%2d %-32s %3d %5.2f\n", pplayer->index, pplayer->name, pplayer->frags, pplayer->time/60);
	pplayer++;
	count++;
	
	if (count == server_info->current_players) break;
    }
    return 1;
}

int 
parse_server_info( char * buffer, long int buffer_size)
{
    int offset = 5, 
	length,
	field = 0;
    char * info;
    
    info = (char *) server_info->server_info_fields;
    
    while (field < 7 && offset < buffer_size) {    
	length = 0;
	
	info = (char *) malloc (INFO_MAX_LENGTH * sizeof (char));
	memset (info, 0, INFO_MAX_LENGTH);
	
	if (field < 5 ) {
	    while ( buffer[offset+length] != '\0') 
		length++;
	    
	    memcpy (info, &buffer[offset], length);
	    printf("%s:%s \n", server_info_fields_name[field], info);
	    offset += length + 1;
	}
	else {
	    memcpy (info, &buffer[offset], 1);
	    printf("%s:%d \n",server_info_fields_name[field], *info);
	    offset++;
	}
	info++;
	field++;
    }
    return 1;
}

int 
main ()
{
    char * packet, * buffer;
    int sock;
    
    if ((server_info = (server_info_tag *) malloc (sizeof(server_info_tag))) == NULL)
	return 1;
    
    if ((packet = (char *) malloc (MAX_PACKET_SIZE)) == NULL)
	return 1;
    
    if ((buffer = (char *) malloc (COMM_SIZE)) == NULL) 
	return 1;
	
    memset(packet, 0, MAX_PACKET_SIZE);
    memset(buffer, 0, COMM_SIZE);

    snprintf (packet, 12, "%s%s", header, "players");
    udpinit();
    sock = udpconnect (HOST, PORT);
    udpsend (sock, packet);
    udprecv(sock, buffer, COMM_SIZE);
    parse_players_info(buffer, COMM_SIZE);
    
    memset(buffer, 0, COMM_SIZE);
    memset(packet, 0, MAX_PACKET_SIZE);
    
    snprintf(packet, 9, "%s%s", header, "info");
    udpsend(sock, packet);
    udprecv(sock, buffer, COMM_SIZE);
    parse_server_info(buffer, COMM_SIZE);
    udpshutdown();
    
    return 0;
}
