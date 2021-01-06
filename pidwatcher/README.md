# pidwatcher

pidwatcher is a kernel module which scans the list of running tasks on demand and returns informations about the processes running. 
Althrough this can be done is user-space too the kernel method was preffered because it was much faster. pidwatcher kernel module is used on mediabox systems (iTuner Mediabox) as a process monitor/watchdog.

The ideea behind pidwatcher watchdog is to start the tasks with a special enviroment variable (which is inherited by the process threads or other processes spawned from the main one).
The pidwatcher kernel module permits getting this data (enviroment strings process id, cpu utilisation) thru a character device which can be queried by user-space application.
The pidwatcher kernel module and monitoring suite was also the subject of my thesis for Faculty Of Computer Science

You can find ptrace examples and examples of processes that fail on usual watchdogs that I used on my thesis here.

## Instalation 

To compile the pidwatcher kernel module you can issue make in the source directory.
After the compilation process has finished you can create the character device used to query data from the kernel module to the userspace and then insert the kernel module. 
You can do this by running:

	mknod /dev/pidwatcher c 254 0 
	insmod ./pidwatcher.o debug=1

On kernel log (dmesg) you should see:

	pidwatcher: begin loading. pidwatcher: module registered with 254 major number.

Note: On some kernel version you should export some symbols if when inserting the module you get unknown symbol errors.
The easiest to do this is to edit /usr/src/linux/kernel/ksyms.c and add:
EXPORT_SYMBOL(nr_threads); EXPORT_SYMBOL(access_process_vm); EXPORT_SYMBOL(mmput);

## Usage

The simple way to use pidwather kernel module is to do a cat /dev/pidwatcher which should return the data from the kernel module. 
In the source directory you will find 2 other applications used to query the module mps.pl (a perl script) and pspidwatcher (written in C). 
You can use these utils to get a formated output from the kernel module.

To use watchdog capability use the applications from the archive below. 
You need to create a file with the command line parameters for your binary, watchdog timer (the delay between process queries) and the module name (the name of you binary usualy). 
After that you can use startmodule modulename and stopmodule modulename. 