#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef IRIX
#include <strings.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include "../../shared/sockets.h"
#include "../../shared/LL.h"

#include "main.h"
#include "mode.h"
#include "sensors.h"


struct _sensors {
    char *chip;
    char *sid;
    char *sdata[6];
    struct _sensors *nextchip;
};
static char sfiles [][6] = {
    "fan1",
    "fan2",
    "fan3",
    "temp1",
    "temp2",
    "temp3"
};

FILE *fd;
static char *sbuffer,*stmp,*sfile;
size_t sfilel, SSIZE = 128;
int i;


int sensors_init()
{
    return 0;
}

int sensors_close()
{
    return 0;
}

int sensors_screen(int rep, int display)
{
  int n;
  static int first = 1;
  static int which_title=0;
  float value;
  struct _sensors * sensors = (struct _sensors *) malloc (sizeof (struct _sensors));
      
    
    
  if(first)
  {
     first = 0;

     sock_send_string(sock, "screen_add V\n");
     sprintf(buffer, "screen_set V priority 64 name {sensors %s}\n", host);
     sock_send_string(sock, buffer);
     
     if (lcd_hgt >= 2)
     {

	sock_send_string(sock, "widget_add V lfan1 string\n");
	sock_send_string(sock, "widget_add V lfan2 string\n");
	sock_send_string(sock, "widget_add V lfan3 string\n");
	sock_send_string(sock, "widget_add V lcputemp string\n");
	
	sock_send_string(sock, "widget_set V lfan1 1 1 {FAN1:     }\n");
	sock_send_string(sock, "widget_set V lfan2 1 2 {FAN2:     }\n");
	sock_send_string(sock, "widget_set V lfan3 11 1 {FAN3:    }\n");
	sock_send_string(sock, "widget_set V lcputemp 11 2 {CPU :    }\n");
	
	sock_send_string(sock, "widget_add V fan1 string\n");
	sock_send_string(sock, "widget_add V fan2 string\n");
	sock_send_string(sock, "widget_add V fan3 string\n");
	sock_send_string(sock, "widget_add V cputemp string\n");
    }
  }
  

  memset (sensors,0,sizeof (struct _sensors));          
  
    if ((sbuffer = (char *) malloc (SSIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }
    
    if ((sensors->chip = (char *) malloc (SSIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }
    
    memset (sensors->chip,0,SSIZE);          
    
    if ((sensors->sid = (char *) malloc (SSIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }
    
    memset (sensors->sid,0,SSIZE);          
    
    if ((fd = fopen(SENSORS_CHIPS,"r")) == NULL) {
	fprintf (stderr,"error opening %s.\n",SENSORS_CHIPS);
	return 1;
    }
    
    /* For now we are interested in the last chip for sensors */
    while (getline(&sbuffer, &SSIZE, fd) != -1 ) {
	stmp = (char *) strchr(sbuffer,'\t') + 1;
	memcpy (sensors->sid,sbuffer,strlen(sbuffer)-strlen(stmp)-1);
	memcpy (sensors->chip,stmp,strlen(stmp)-1);
	sensors->nextchip = NULL;
    }
    
    fclose (fd);    
    
    for (i=0;i<6;i++) {
	sfilel = strlen(SENSORS)+strlen(sensors->chip)+strlen(sfiles[i]) + 3;
	if ((sfile = (char *) malloc (sfilel)) == NULL) {
	    fprintf (stderr,"allocation error.");
	    return 1;
	}
	snprintf (sfile,sfilel,"%s/%s/%s", SENSORS, sensors->chip, sfiles[i]);
	
	if ((fd = fopen(sfile,"r")) == NULL) {
	    fprintf (stderr,"error opening %s.\n",sfile);
	    if ((sensors->sdata[i] = (char *) malloc (strlen(NA) + 1)) == NULL) {
		fprintf (stderr,"allocation error.");
		return 1;
	    }
	    snprintf (sensors->sdata[i],strlen(NA) + 1,"%s",NA);
	}
	else {
	    /* Get the line from the sensor file */
	    if (getline(&sbuffer, &SSIZE, fd) == -1) {
		fprintf (stderr,"error reading line.\n");
	    }
	    /* Get only the last value - sensor curent reading */
	    stmp = (char *) strrchr(sbuffer,' ') + 1;
	    /* Use this to make get rid of end of the string */
	    if ((sensors->sdata[i] = (char *) malloc (strlen(stmp))) == NULL) {
		fprintf (stderr,"allocation error.");
		return 1;
	    }
	    /* Some temp sensors report incorect data */
	    if (i >= 3 && atof(stmp) >= 100.0) 
		snprintf (sensors->sdata[i],strlen(NA) + 1,"%s",NA);
	    else
		snprintf (sensors->sdata[i],strlen(stmp),"%s",stmp);
		
	    free (sfile);
	    fclose(fd);
	}
    }
    
    sprintf(tmp, "widget_set V fan1 6 1 {%s}\n",sensors->sdata[0]);
    sock_send_string(sock, tmp);
    sprintf(tmp, "widget_set V fan2 6 2 {%s}\n",sensors->sdata[1]);
    sock_send_string(sock, tmp);
    sprintf(tmp, "widget_set V fan3 16 1 {%s}\n",sensors->sdata[2]);
    sock_send_string(sock, tmp);
    sprintf(tmp, "widget_set V cputemp 16 2 {%s}\n",sensors->sdata[4]);
    sock_send_string(sock, tmp);
    free (sbuffer);
    free (sensors);
    return 0;
} // End sensors_screen()

   

