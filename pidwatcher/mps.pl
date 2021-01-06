#!/usr/bin/perl

$typedef =  'i A8192 I L';
$sizeof = length pack($typedef, () );
use IO::File;
open(WTMP, '/dev/pidwatcher/pidwatcher') or die "can't open /dev/pidwatcher/pidwatcher: $!";
seek(WTMP, 0, SEEK_END);
for (;;) {
while (read(WTMP, $buffer, $sizeof) == $sizeof) {
    ($pid, $environ, $environ_length , $cpu,)
        = unpack($typedef, $buffer);
    next unless $pid;
    #print "PID:$pid\nEnviron:$environ\nCPU%:$cpu\nEnvironLength:$environ_length\n";
    print "PID:$pid EnvironLength:$environ_length\n";
    }
    for ($size = -s WTMP; $size == -s WTMP; sleep 1) {}
    WTMP->clearerr();
}
										