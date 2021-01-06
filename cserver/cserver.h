#define CSERVER_RUNNING 1
#define CSERVER_STOPPING 0

typedef struct _global_tag {
    int running;
} _global;

extern _global global;
