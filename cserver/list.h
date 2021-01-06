#include <stdlib.h>

typedef struct _list_element 
{
    void *data;
    struct _list_element *next;
} _element;

typedef struct _list_tag  
{
    unsigned long int size;
    void (*destroy)(void *data);
    _element *head;
    _element *tail;
} _list;


/* public functions */
void list_initialize(void (*destroy)(void *data));
void list_destroy(_list *list);
int list_ins_next(_list *list, _element *element, const void *data);
int list_rem_next(_list *list, _element *element, void **data);
_list * list_get_list(void);

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)
