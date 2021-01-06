#define SIGNATURE  0xFF

#define HOST "games.turbozone.ro"
#define PORT 27015

#define COMM_SIZE 2048
#define PLAYER_MAX_LENGTH 255
#define INFO_MAX_LENGTH 255
#define MAX_PACKET_SIZE 30
#define SERVER_INFO_FIELDS 7

char header[] = {
    SIGNATURE,
    SIGNATURE,
    SIGNATURE,
    SIGNATURE
};

const char * server_info_fields_name [] = {
    "Server address and port",
    "Server Description",
    "Current played map",
    "Game type",
    "Game name",
    "Current players",
    "Maximum players",
    NULL
};

typedef struct _player_tag {
    short int index;
    char * name;
    int frags;
    float time;
} player_tag;


typedef struct _server_info_tag {
    int current_players;
    player_tag * player;
    char * server_info_fields[SERVER_INFO_FIELDS];
} server_info_tag;

server_info_tag * server_info;
