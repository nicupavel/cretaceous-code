#include "list.h"

typedef struct _protocol_atom_data 
{
    char * basedir;
    char * filename;
    long int total_filesize;
    long int current_filesize;
    size_t offset;
} protocol_atom_data;

typedef struct _protocol_meta_data
{
    long int atoms;
        
} protocol_meta_data
