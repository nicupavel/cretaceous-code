/* 
Simple lm_sensors query without using libsensors.
Copyright <npavel@ituner.com> iTuner Networks corp 2002
Only gets fan[1-3] and temp[1-3] voltages must be 
computed different for each type of sensors.
*/

#include <stdio.h>

#define SENSORS_CHIPS "/proc/sys/dev/sensors/chips"
#define SENSORS "/proc/sys/dev/sensors"
size_t SIZE = 128;

struct _sensors {
    char *chip;
    char *id;
    char *sdata[6];
    struct _sensors *nextchip;
};

char sfiles [][6] = {
    "fan1",
    "fan2",
    "fan3",
    "temp1",
    "temp2",
    "temp3"
};

int main () 
{

    FILE *fd;
    char *buffer,*tmp,*sfile;
    size_t sfilel;
    int i;
    
    struct _sensors * sensors = (struct _sensors *) malloc (sizeof (struct _sensors));
    memset (sensors,0, sizeof (struct _sensors));
    
    if ((buffer = (char *) malloc (SIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }
    memset (buffer, 0, SIZE);
    if ((sensors->chip = (char *) malloc (SIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }

    if ((sensors->id = (char *) malloc (SIZE)) == NULL) {
	fprintf (stderr,"allocation error.");
	return 1;
    }
    
    if ((fd = fopen(SENSORS_CHIPS,"r")) == NULL) {
	fprintf (stderr,"error opening %s.\n",SENSORS_CHIPS);
	return 1;
    }
    /* For now we are interested in the last chip for sensors */
    while (getline(&buffer, &SIZE, fd) != -1 ) {
	tmp = (char *) strchr(buffer,'\t') + 1;
	memcpy (sensors->id,buffer,strlen(buffer)-strlen(tmp)-1);
	memcpy (sensors->chip,tmp,strlen(tmp)-1);
	sensors->nextchip = NULL;
	fprintf (stdout,"id is %s.\n",sensors->id);
	fprintf (stdout,"chip is %s.\n",sensors->chip);
    }
    
    fclose (fd);
    
    for (i=0;i<6;i++) {
	sfilel = strlen(SENSORS)+strlen(sensors->chip)+strlen(sfiles[i]) + 3;
	if ((sfile = (char *) malloc (sfilel)) == NULL) {
	    fprintf (stderr,"allocation error.");
	    return 1;
	}
	snprintf (sfile,sfilel,"%s/%s/%s", SENSORS, sensors->chip, sfiles[i]);
	fprintf (stdout,"path is %s.\n",sfile);
	
	if ((fd = fopen(sfile,"r")) == NULL) {
	    fprintf (stderr,"error opening %s.\n",sfile);
	    return 1;
	}
	/* Get the line from the sensor file */
	getline(&buffer, &SIZE, fd);
	/* Get only the last value - sensor curent reading */
	tmp = (char *) strrchr(buffer,' ') + 1;
	/* Use this to make get rid of end of the string */
	if ((sensors->sdata[i] = (char *) malloc (strlen(tmp))) == NULL) {
	    fprintf (stderr,"allocation error.");
	    return 1;
	}
	snprintf (sensors->sdata[i],strlen(tmp),"%s",tmp);
	fprintf (stdout,"%s is %s on %d location.\n",sfiles[i],sensors->sdata[i],i);
	
	free (sfile);
	fclose(fd);
    }
    free (buffer);
    free (sensors);
    return 0;
}
