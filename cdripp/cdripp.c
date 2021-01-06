#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/cdrom.h>

int main(int argc, char *argv[])
{
  int fd;                             /* CD-ROM file descriptor */
  int status;                         /* hold status of system calls */
  struct cdrom_read_audio read_audio; /* parameter for ioctl call */
  struct cdrom_tochdr tochdr;
  int nframes;                        /* number of frames to read */
  char *device;                       /* CD-ROM device name */

  /* check and parse command line arguments */
  device  = argv[1];
  /* open device */  
  status = fd = open(device, O_RDONLY | O_NONBLOCK);
  if (status < 0) {
    perror("open failed");
    exit(1);
  }
  /* set audio buffer size - only needed for SBPCD drives */
  status = ioctl(fd, CDROMAUDIOBUFSIZ, nframes);
  if (status <= 0) {
    perror("CDROMAUDIOBUFSIZ ioctl failed (ignore for non-SBPCD drives)");
    /* don't exit, in case it is a non-SBPCD drive */
  }
    if (ioctl(fd,CDROMREADTOCHDR,&tochdr) != 0 ) {
	perror ("error reading toc header");
	exit (1);
    }
    printf ("tracks: %d - %d",tochdr.cdth_trk0,tochdr.cdth_trk1);
    
    return (0);
}
