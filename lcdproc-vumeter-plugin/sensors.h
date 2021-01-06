#ifndef SENSORS_H
#define SENSORS_H

#include <sys/types.h>
#include <sys/ioctl.h>

#define SENSORS_CHIPS "/proc/sys/dev/sensors/chips"
#define SENSORS "/proc/sys/dev/sensors"
#define NA "N/A"

int sensors_init();
int sensors_close();

int sensors_screen(int rep, int display);

#endif
