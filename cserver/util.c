#include <sys/poll.h>

int util_timed_wait_for_fd(int fd, int timeout)
{
    struct pollfd ufds;

    ufds.fd = fd;
    ufds.events = POLLIN;
    ufds.revents = 0;

    return poll(&ufds, 1, timeout);
}
