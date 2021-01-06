#define SQL_DEFAULT_USER "cserver"
#define SQL_DEFAULT_HOST "localhost"
#define SQL_DEFAULT_PASS  "stupid"
#define SQL_DEFAULT_DBASE "cserver"
#define SQL_DEFAULT_USER_TABLE "users"
#define SQL_DEFAULT_USER_INFO_TABLE "users_info"

/* public functions */
int sql_initialize (void);
int sql_add_user(char *user_id);
