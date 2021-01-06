#define PIDWATCHER_MAJOR_NUMBER 254
#define PIDWATCHER_VERSION "1.2.3"
#define SUCCESS 0
#define DEVICE_NAME "pidwatcher"

#define DEBUG(x) if (debug>=1) x;

static int debug = 1;

/* The process output. More will be added here in the future */

struct pid_info_struct {
/* enviroment of the process */
    char environ[8192];
/* cpu utilisation of the process */
    unsigned long cpu;
/* process pid */
    pid_t pid;
/* enviroment length of process needed for parsing \0 */
    unsigned int environ_length;
};

struct get_info_struct {
    struct pid_info_struct *data;
    unsigned int len;
};
