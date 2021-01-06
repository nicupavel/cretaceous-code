typedef struct _query_list_tag {
    char * host;
    int port;
    struct _query_list_tag *next;
} query_list_tag;

typedef struct _config_tag {
    int timeout;
    query_list_tag * servers;
} config_tag;