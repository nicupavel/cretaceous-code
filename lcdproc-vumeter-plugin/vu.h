#ifndef VU_H
#define VU_H

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#define SOUND_DEVICE "/dev/imux16"
#define BUF_SIZE 512

int vu_init();
int vu_close();

int vu_screen(int rep, int display);

#endif
