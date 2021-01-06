#define CONFIG_DEFAULT_ADMIN "npavel@ituner.com"
#define CONFIG_DEFAULT_CLIENT_LIMIT 1024 
#define CONFIG_DEFAULT_CLIENT_TIMEOUT 30         
#define CONFIG_DEFAULT_HOSTNAME "localhost"
#define CONFIG_DEFAULT_PORT 6666

#define CONFIG_DEFAULT_BASE_DIR "/opt/cserver"
#define CONFIG_DEFAULT_LOG_DIR "/opt/cserver/logs"
#define CONFIG_DEFAULT_ACCESS_LOG "cserver_access.log"
#define CONFIG_DEFAULT_ERROR_LOG "cserver_error.log"

#define CONFIG_DEFAULT_CHROOT 0
#define CONFIG_DEFAULT_CHUID 0
#define CONFIG_DEFAULT_USER NULL
#define CONFIG_DEFAULT_GROUP NULL


typedef struct _server_config_tag
{
	char *admin;

	int client_limit;
	int client_timeout;

	char *hostname;
	int port;
	char *bind_address;

	char *base_dir;
	char *log_dir;

	char *access_log;
	char *error_log;

	int chroot;
        int chuid;
	char *user;
        char *group;
} server_config_tag;

/* public functions */
void config_initialize(void);
int config_parse_file(const char *filename);
server_config_tag *config_get_config(void);
void config_dump (server_config_tag *configuration);
