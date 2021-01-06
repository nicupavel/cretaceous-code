#include <stdlib.h>
#include <string.h>

#include "list.h"

/* private vars */
static _list *list;

/* private functions */
static void list_set_defaults(_list *list);

void list_initialize (void (*destroy)(void *data))
{

    if ((list = (_list *) malloc (sizeof(_list))) == NULL) {
	return;
    }
    memset(list, 0, sizeof(_list));
    
    list_set_defaults(list);
    list->destroy = destroy;
}

static void list_set_defaults(_list *list)
{
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void list_destroy (_list *list)
{
    void *data;
    
    while (list_size(list) > 0) {
	if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
	    list->destroy(data);
	}
    
    }

}


int list_ins_next (_list *list, _element *element, const void * data)
{
    _element *new_element;
    
    if ((new_element = (_element *) malloc (sizeof(_element))) == NULL)
	return -1;
	
    new_element->data = (void *) data;
    
    if (element == NULL) {
	// head of the list
	if (list_size(list) == 0)
	    list->tail = new_element;
	
	new_element->next = list->head;
	list->head = new_element;
    }
    else {
	// tail of the list 
	if (element->next == NULL)
	    list->tail = new_element;
	
	new_element->next = element->next;
	element->next = new_element;
    }
    
    list->size++;
    return 0;
}

int list_rem_next (_list *list, _element *element, void **data)
{
    _element *old_element;
    
    if (list_size(list) == 0)
	return -1;

    if (element == NULL) {
	//head of the list
	*data = list->head->data;
	old_element = list->head;
	list->head = list->head->next;
	
	if (list_size(list) == 1)
	    list->tail = NULL;
    }
    else {
	if (element->next == NULL) 
	    return -1;
	    
	*data = element->next->data;
	old_element = element->next;
	element->next = element->next->next;
	
	if (element->next == NULL)
	    list->tail = element;
    }
    
    free (old_element);
    list->size--;
    
    return 0;
}

_list * list_get_list (void)
{
    return list;
}
