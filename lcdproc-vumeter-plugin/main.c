/* This work is derived from lcdproc client written by 
   William Ferrell and Scott Scriven.
   Customised and new modules by Nicu Pavel <npavel@ituner.com>
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <termios.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

#include "main.h"
#include "mode.h"
#include "../../shared/sockets.h"
#include "../../shared/debug.h"


// TODO: Commenting...  Everything!


int Quit = 0;
int sock;

char *version = VERSION;
char *build_date = __DATE__;

int lcd_wid;
int lcd_hgt;
int lcd_cellwid;
int lcd_cellhgt;


/*
  Mode List:
    See below...  (default_sequence[])
*/

void HelpScreen();
void exit_program(int val);
void main_loop(mode *sequence);

#define MAX_SEQUENCE 256
// 1/8th second is a single time unit...
#define TIME_UNIT 125000


// Contains a list of modes to run
mode default_sequence[] = 
{
// which on  off  inv  timer/visible  
  { 'C', 1,   2,   0,  0xffff,0,},  // [C]PU
  { 'M', 4,   16,  0,  0xffff,0,},  // [M]emory
  { 'V', 2,   4,   0,  0xffff,0,},   // [V]uMeter
  { 'N', 4,   64,  0,  0xffff,0,},   // se[N]sors
// { 'X', 64,  128, 1,  0xffff,0,}, // [X]-load (load histogram)
  { 'T', 4,   64,  0,  0xffff,0,},  // [T]ime/Date
//  { 'A', 999,9999, 0,  0xffff,0,},// [A]bout (credits)
  {  1 , 0,   0,   0,  0,0,},       // Modes after this line will not be run by default...
                                    // ... all non-default modes must be in here!
                                    // ... they will not show up otherwise.
  { 'O', 4,   64,  0,  0xffff,0,},  // [O]ld Timescreen
  { 'K', 4,   64,  0,  0xffff,0,},  // big cloc[K] 
  { 'U', 4,   128, 0,  0xffff,0,},  // Old [U]ptime Screen
//  { 'B', 32,  256, 1,  0xffff,0,},// [B]attery Status
//  { 'G', 1,   2,   0,  0xffff,0,},// Cpu histogram [G]raph
  { 'S', 16,  256, 1,  0xffff,0,},  // [S]ize of biggest programs
  { 'D', 256, 256, 1,  0xffff,0,},  // [D]isk stats
  { 0,	0,    0,       0,0,},       // No more..  all done.
};

// TODO: Clean up main()...  It is still way too big.

// TODO: Config file; not just command line


int main(int argc, char **argv)
{
   mode sequence[MAX_SEQUENCE];
   char cfgfile[256] = "/etc/lcdproc.cf";
   char server[256] = "localhost";
   int port = LCDPORT;
   int i, j, k;
   int tmp;

   memset(sequence, 0, sizeof(mode) * MAX_SEQUENCE);

   // Ctrl-C will cause a clean exit...
   signal(SIGINT, exit_program);
   // and "kill"...
   signal(SIGTERM, exit_program);
   // and "kill -HUP" (hangup)...
   signal(SIGHUP, exit_program);
   // and just in case, "kill -KILL" (which cannot be trapped; but oh well)
   signal(SIGKILL, exit_program);
  

  
   // Command line
   memcpy(sequence, default_sequence, sizeof(default_sequence));
   for(i=1, j=0; i<argc; i++)
   {
      if(argv[i][0] == '-') switch(argv[i][1])
      {
	 // s is for server
	 case 'S':
	 case 's': if(argc < i+1) HelpScreen();
	    strcpy(server, argv[++i]);
	    break;
	 // p is for port
	 case 'P':
	 case 'p': if(argc < i+1) HelpScreen();
	    port = atoi(argv[++i]);
	    if(port < 1  &&  port > 0xffff)
	       fprintf(stderr, "Warning:  Port %i outside of standard range\n",
		       port);
	    break;
      
	    // otherwise...  Get help!
	 default: HelpScreen(); break;
      }
      // Parse command line here...  read the man page.
      else if(strlen(argv[i]) == 1)
      {
	 // Grab the mode letter...
	 sequence[j].which = argv[i][0];

	 // If we have just a letter with no numbers...
	 // Set the defaults...
	 for(tmp=0, k=0; default_sequence[k].which; k++)
	 {
	    if(toupper(sequence[j].which) == default_sequence[k].which)
	    {
	       memcpy(&sequence[j], &default_sequence[k], sizeof(mode));
	       tmp=1;
	       break;
	    }
	 }
	 if(!tmp) { printf("Invalid Mode: %c\n", argv[i][0]); exit(0); }

	 j++;
	 // Set the last element to 0...
	 memset(sequence + j, 0, sizeof(mode));
      } // End if(strlen(argv == 1))
      else
      {
	 // A multicharacter parameter by itself
	 //  is assumed to be a config file..
	 strcpy(cfgfile, argv[i]);
	 printf("Ignoring config file: %s\n", cfgfile);
      }
   }


   // Connect to the server...
   usleep(500000); // wait for the server to start up
   sock = sock_connect(server, port);
   if (sock <= 0)
   {
      printf("Error connecting to server %s on port %i.\n",
	     server, port);
      return 0;
   }
   sock_send_string(sock, "hello\n");
   debug("Sent hello to server\n");
   usleep(500000); // wait for the server to say hi.
   
   
   // We grab the real values below, from the "connect" line.
   lcd_wid = 20;
   lcd_hgt = 2;
   lcd_cellwid = 5;
   lcd_cellhgt = 8;
   
   // Init the status gatherers...
   mode_init(sequence);

   // And spew stuff!
   main_loop(sequence);
  
   // Clean up
   exit_program(0);
   return 0;
}


void HelpScreen()
{
  printf("lcdstats, %s\n", version);
  printf("Usage: lcdstats [-s server] [-p port] [modelist]\n");
  printf("\tOptions in []'s are optional.\n");
  printf("\tmodelist is \"mode [mode mode ...]\"\n");
  printf("\tMode letters: \t[C]pu [T]ime [M]emory [V]uMeter [D]isk se[N]sors \n\t\t\tproc_[S]izes [O]ld_time big_cloc[K] [U]ptime\n");
  printf("\n");
  printf("Example:\n");
  printf("\tlcdstats -s my.lcd.server.com C M D -p 13666\n");
  printf("\n");
  exit(0);
}


///////////////////////////////////////////////////////////////////
// Called upon TERM and INTR signals...
//
void exit_program(int val)
{
  Quit = 1;
  sock_close(sock);
  mode_close();
  exit(0);
}


///////////////////////////////////////////////////////////////////
// Main program loop...
//
void main_loop(mode *sequence)
{
   int i=0, j;
   //int status=0;
   char buf[8192];
   char *argv[256];
   int argc, newtoken;
   int len;
   
   // Main loop
   // Run whatever screen we want, then wait.  Woo-hoo!
   while(!Quit)
   {
      // Check for server input...
      len = sock_recv(sock, buf, 8000);
       // Handle server input...
      while(len > 0)
      {
	 // Now split the string into tokens...
	 //for(i=0; i<argc; i++) argv[i]=NULL; // Get rid of old tokens
	 argc = 0; newtoken = 1;
	 for(i=0; i<len; i++)
	 {
	    if(buf[i])    // For regular letters, keep tokenizing...
	    {
	       switch(buf[i])
	       {
		  case ' ':
		     newtoken = 1;
		     buf[i] = 0;
		     break;
		  case '\n':
		     buf[i] = 0;
		  default:
		     if(newtoken)
		     {
			argv[argc] = buf+i;
			argc++;
		     }
		     newtoken = 0;
		     break;
	       }
	    }
	    else  // If we've got a zero, it's the end of a string...
	    {
	       if(argc > 0)
	       {
	       
	    	
	    	  
		  //printf("%s %s\n", argv[0], argv[1]);
		  if(0 == strcmp(argv[0], "listen"))
		  {
		     for(j=0; sequence[j].which; j++)
		     {
			if(sequence[j].which == argv[1][0])
			{
			   sequence[j].visible = 1;
			   debug("Listen %s\n", argv[1]);
			}
		     }
		  }
		  else if(0 == strcmp(argv[0], "ignore"))
		  {
		     for(j=0; sequence[j].which; j++)
		     {
			if(sequence[j].which == argv[1][0])
			{
			   sequence[j].visible = 1;
			   debug("Ignore %s\n", argv[1]);
			}
		     }
		    	     
		  }
		  else if(0 == strcmp(argv[0], "key"))
		  {
		     debug("Key %s\n", argv[1]);
		     /* Exit the loop if key is ESC */
		     if ( strcmp (argv[1],"E") == 0)
		        exit_program(0);
		    /* This is the escape key for VIA LCD version */
		    if ( strcmp (argv[1],"B") == 0)
			exit_program(0);
		  }
		  else if(0 == strcmp(argv[0], "menu"))
		  {
		  }
		  else if(0 == strcmp(argv[0], "connect"))
		  {
		     int a;
		     for(a=1; a<argc; a++)
		     {
			if(0 == strcmp(argv[a], "wid"))
			   lcd_wid = atoi(argv[++a]);
			else if(0 == strcmp(argv[a], "hgt"))
			   lcd_hgt = atoi(argv[++a]);
			else if(0 == strcmp(argv[a], "cellwid"))
			   lcd_cellwid = atoi(argv[++a]);
			else if(0 == strcmp(argv[a], "cellhgt"))
			   lcd_cellhgt = atoi(argv[++a]);
		     }
		  }
		  else if(0 == strcmp(argv[0], "bye"))
		  {
		     //printf("Exiting LCDproc\n");
		     exit_program(0);
		  }
		  else
		  {
		     int j;
		     for(j=0; j<argc; j++)
			printf("%s ", argv[j]);
		     printf("\n");
		  }
	       }
	       
	       argc = 0;  newtoken = 1;
	    }
	 }
	 
	 len = sock_recv(sock, buf, 8000);
	 debug("sock_recv\n");
      }
      // Gather stats...
      // Update screens...
      for(i=0; sequence[i].which > 1; i++)
      {
	 sequence[i].timer++;
	 if(sequence[i].visible)
	 {
	    if(sequence[i].timer >= sequence[i].on_time)
	    {
	       sequence[i].timer = 0;
	       // Now, update the screen...
	       debug("Update visible->on_time\n");
	       update_screen(sequence+i, 1);
	    }
	 }
	
	 else
	 {
	    
	    if(sequence[i].show_invisible)
	    { 
	    
	       if(sequence[i].timer >= sequence[i].off_time)
	       {
		  sequence[i].timer = 0;
		  // Now, update the screen...
		  debug("Update show_invisible->off_time\n");
		  update_screen(sequence+i, 1);
	       }
	    }
	    else
	    {
	       if(sequence[i].timer >= sequence[i].off_time)
	       {
		  sequence[i].timer = 0;
		  // Now, update the screen...
		  debug("Update not show_invisible->off_time\n");
		  update_screen(sequence+i, 0);
	       }
	    }
	 }
	 
      }
      // Now sleep...
      usleep(TIME_UNIT);
     }
}


