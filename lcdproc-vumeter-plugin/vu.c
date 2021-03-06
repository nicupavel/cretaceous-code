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
#include "vu.h"

static void get_vu_info();

int snd = 0, 
    parm, 
    status, 
    max_volume = 126;
    
unsigned char volume[BUF_SIZE];
unsigned int peak_left,peak_right;

static void get_vu_info() 
{
    int len;
    unsigned int i;
    
    /* this is ok only in 8 bit sample data */
    if (snd) { 
	if ( (len = read (snd,&volume,BUF_SIZE)) == -1 ) {
	    fprintf (stderr,"error reading data from %s\n",SOUND_DEVICE);
	    exit (1);
	}
	
	peak_left = (unsigned int) volume[0];
	peak_right = (unsigned int) volume[1];
	
	for (i=1;i< len-2;) {
	    if (volume[i++] > peak_left)
		peak_left = (unsigned int) volume[i];
	    
	    if (volume[i++] > peak_right)
		peak_right = (unsigned int) volume[i];
		
	    //fprintf (stderr,"Left:%d,Right:%d\n",peak_left,peak_right);
	}
    }
}


int vu_init()
{
    if (!snd)
    {
	snd = open (SOUND_DEVICE, O_RDONLY, 0);
	if ( snd < 0 ) {
	    fprintf (stderr,"error opening %s\n",SOUND_DEVICE);
	    exit (1);
	}
    
	parm = 2;
        status = ioctl(snd,SNDCTL_DSP_CHANNELS,&parm);
    
        if ( status < 0 ) {
    	    fprintf (stderr,"error setting channels number to %s\n",SOUND_DEVICE);
	    exit (1);
        }
    
	parm = AFMT_U8;
        status = ioctl(snd,SNDCTL_DSP_SETFMT,&parm);
    
	if ( status < 0 ) {
	    fprintf (stderr,"error setting 8 bit samples to %s\n",SOUND_DEVICE);
	    exit (1);
	}
    
	parm = 8000;
	status = ioctl(snd, SNDCTL_DSP_SPEED, &parm);
    
	if ( status < 0 ) {
	    fprintf (stderr,"error sampling rate to %s\n",SOUND_DEVICE);
	    exit (1);
	}
    }
    return 0;
}

int vu_close()
{
    if (snd) 
	close (snd);
    snd = 0;
    return 0;
}


int vu_screen(int rep, int display)
{
  int n;
  static int first = 1;
  static int which_title=0;
  float value;
  
  if(first)
  {
     first = 0;

     sock_send_string(sock, "screen_add V\n");
     sprintf(buffer, "screen_set V priority 64 name {vu meter %s}\n", host);
     sock_send_string(sock, buffer);

     if(lcd_hgt >= 2 )
     {
	sock_send_string(sock, "widget_add V l string\n");
	sock_send_string(sock, "widget_add V r string\n");
	if(lcd_wid >= 20)
	{
	   sock_send_string(sock, "widget_set V l 1 1 {L [           ]}\n");
	   sock_send_string(sock, "widget_set V r 1 2 {R [           ]}\n");
	}
	else
	{
	   sock_send_string(sock, "widget_set V l 1 1 {L [       ]}\n");
	   sock_send_string(sock, "widget_set V r 1 2 {R [       ]}\n");
	}
	sock_send_string(sock, "widget_add V left% string\n");
	sock_send_string(sock, "widget_add V right% string\n");
	sock_send_string(sock, "widget_set V left% 16 1 { 0.0%}\n");
	sock_send_string(sock, "widget_set V right% 16 2 { 0.0%}\n");
	
     }

     sock_send_string(sock, "widget_add V lgauge hbar\n");
     sock_send_string(sock, "widget_add V rgauge hbar\n");
     
     sock_send_string(sock, "widget_set V lgauge 4 1 10\n");
     sock_send_string(sock, "widget_set V rgauge 4 2 13\n");
  }


  get_vu_info();
  
     
     /* it should be 127 but in practice is 122 */
     if (peak_left >= max_volume) {
        value = ((float)peak_left-max_volume)/(float)max_volume;
     }
     else 
        value = 0;
	
     n = (int)((lcd_cellwid * (float)(lcd_wid-10))*(value));
     /* Left channel */
     sprintf(tmp, "widget_set V lgauge 4 1 %i\n", n);
     if(display) sock_send_string(sock, tmp);

     value *= 100.0;
     if (value >= 99.9) { sprintf(buffer, "100%%"); } 
     else { sprintf(buffer, "%4.1f%%", value); }
     sprintf(tmp, "widget_set V left%% %i 1 {%s}\n", lcd_wid-4, buffer);
     if(display) sock_send_string(sock, tmp);
    
     /* Right channel */
     
     if (peak_right >= max_volume) {
        value = ((float)peak_right-max_volume)/(float)max_volume;
     }
     else 
        value = 0;	
    
     n = (int)((lcd_cellwid * (float)(lcd_wid-10))*(value));		

     sprintf(tmp, "widget_set V rgauge 4 2 %i\n", n);
     if(display) sock_send_string(sock, tmp);

     value *= 100.0;
     if (value >= 99.9) { sprintf(buffer, "100%%"); } 
     else { sprintf(buffer, "%4.1f%%", value); }
     sprintf(tmp, "widget_set V right%% %i 2 {%s}\n", lcd_wid-4, buffer);
     if(display)  sock_send_string(sock, tmp);

    return 0;
} // End vu_screen()

   

