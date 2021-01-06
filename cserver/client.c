#include <stdio.h>

#include "list.h"
#include "client.h"

int client_find_next_client_id(_list *list, _element **element, int id)
{
    client_info *cinfoptr;
    _element *selement;
    
    fprintf (stderr,"Client: Searching for:%d\n", id);
    
    if (list_size(list) == 0)
	return -1;

    selement = list_head(list);
    cinfoptr = selement->data;
    
    if (cinfoptr->id == id) {
	*element = NULL;
	return 0;
    }    

    while (!list_is_tail(selement)) {
	*element = selement;
	selement = selement->next;
	cinfoptr = selement->data;
	if (cinfoptr->id == id)
	    return 0;
    }
    return -1; 
}
