#include <stdio.h>
#include <stdlib.h>

#include <mysql/mysql.h>

#include "sql.h"

MYSQL mysql;

int sql_initialize(void)
{
    mysql_init( &mysql );
    if(!mysql_real_connect( &mysql, SQL_DEFAULT_HOST, SQL_DEFAULT_USER, SQL_DEFAULT_PASS, SQL_DEFAULT_DBASE, 0, NULL, 0 ))
	return 1;
    return 0;
}

int sql_add_user(char *user_id)
{
    char buf[1024];
    snprintf( buf, sizeof(buf), "INSERT INTO %s (user_id=%s)", SQL_DEFAULT_USER_TABLE, user_id );
    mysql_query( &mysql, buf );
    return 0;
}
