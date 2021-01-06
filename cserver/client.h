typedef struct _client_info 
{
    int descriptor;
    int  id;
    char * ip;
} client_info;

/* public functions */

int client_find_next_client_id (_list *list, _element **element, int id);
