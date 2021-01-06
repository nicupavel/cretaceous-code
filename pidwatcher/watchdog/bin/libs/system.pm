#!/usr/bin/perl -w
package system;
use warnings;
use strict;

# Copyright (C) 1996-2003 iTuner Corp.
# Nicu Pavel <npavel@ituner.com>
# Razvan Udrea <razvan@ituner.com>

# Some global variables
# You can set this from caller program $system::variable = 'whatever'

# Set this to 1 if you want debugging messages from subs
our $DEBUG = 0;

# Mount point where and to get/save network information and get/save users tree
# default set to root
our $MountPoint = "/";

# default shell ( path to shell , must be a absolute path )
our $DEFAULT_SHELL = "/bin/bash";

# every new created user will have GroupID users (100)
our $DEFAULT_GID = 100;
our @SYSTEM_USERS = ( "root", "admin" );
our @HIDDEN_USERS = ( "bin", "daemon", "adm", "lp", "sync", "shutdown", "halt", "mail", "nobody", "apache" );

our @HTTP_SYSTEM_USERS = ( "admin" );

####################################
# Network functions handling library

# validate an ip format string
# prototype: $ExitCode ValidateIpRequired($ip)
sub ValidateIpRequired($) {
        my $ip = shift;
        return 0 unless $ip =~ /^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$/
                && !grep { $_ < 0 || $_ > 255 } split(/\./, $ip);
}

# prototype: $ExitCode ValidateIp($ip)
sub ValidateIp($) {
        my $ip = shift;
        return 1 unless $ip;
        return 0 unless $ip =~ /^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$/
                && !grep { $_ < 0 || $_ > 255 } split(/\./, $ip);
}

# validate an hostname format string
# prototype: $ExitCode ValidateHostNameRequired($host)
sub ValidateHostNameRequired($) {
        my $host = shift;
        return 0 unless $host =~ /^[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,3}|[0-9]{1,3})$/
}

# prototype: $ExitCode ValidateHostName($host)
sub ValidateHostName($) {
        my $host = shift;
        return 1 unless $host;
        return 0 unless $host =~ /^[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,3}|[0-9]{1,3})$/
}

# Auto generate broadcast address from ip and netmask
# prototype: $broadcast CalcBroadcast($IP,$NETMASK)
sub CalcBroadcast($$) {
	my $IP = shift;
	my $NETMASK = shift;	
	
	my $broadcast;
	
	print "Invalid IP address!\n" if ( $DEBUG && !ValidateIp($IP) );
	print "Invalid Netmask address!\n" if ( $DEBUG && !ValidateIp($NETMASK) );
	
	return 0 if ( !ValidateIp($IP) || !ValidateIp($NETMASK) );
	
	if (open (OUT,"$MountPoint/bin/ipcalc $IP $NETMASK --broadcast |")) {
	    while (my $line=<OUT>) {
		# Better like that maybe we get some stupid error there.
		  if ($line =~ /^BROADCAST=(.*?)$/i) {
	  		$broadcast = $1;
	    	chomp($broadcast);
		    last;
		  };
	    }    
	} else { 
	    $broadcast = 0;
	    print "Couldn't open /bin/ipcalc :$!" if $DEBUG;
	}
	close (OUT);
	return $broadcast;
}

# get_network_settings - get current network settings by using standart unix tools
# see at begining for setting those tools
# If the autodetection fail put some defaults 
# HASH NetworkInfo (
#			hostname 	= $hostname,
#			ip		 	= $ip,
#			broadcast	= $broadcast,
#			netmask		= $netmask,
#			dns1		= $dns1,
#			dns2		= $dns2,
#			gateway		= $gateway
#		);
# prototype %NetworkInfo GetNetworkSetting();
sub LoadNetworkSettings() {
	
	my %NetworkInfo = ();
	
	my $resolv_path = $MountPoint."/etc/resolv.conf";
	my $network_path = $MountPoint."/etc/config/network";
	my $eth_path = $MountPoint."/etc/config/ifcfg-eth0";	
	
	
	# fetch hostname and gateway from etc/configs/network file
	# is not found use some defaults
	my ($hostname,$gateway);

	if ( open(NETWORK,$network_path) ) {
		while (<NETWORK>) {
			# skip commented lines		
			next if ( $_ =~ /^(.*\#+)(.*\s+)(\S+).*/ );		
	    	if (/HOSTNAME/i) {
				(undef,$hostname) = split /=/;
				#strip white spaces
				$hostname=~s/^\s+//;
				$hostname=~s/\s+$//;
				next;
			}
	    	if (/GATEWAY/i) {
				(undef,$gateway) = split /=/;
				#strip white spaces
				$gateway=~s/^\s+//;
				$gateway=~s/\s+$//;
				next;
			}
		}	
	}else {
		print "Could not open $network_path:$!\n" if $DEBUG;	
	}

	if ( !$hostname ) {
		print "Hostname not found ... using default!\n" if $DEBUG;
		$hostname = "imedia.your.domain";
	}
	if ( !$gateway ) {
		print "Gateway not found ... using default!\n" if $DEBUG;
		$gateway = "192.168.0.1";
	}
	close (NETWORK);
	

	# fetch interface details from etc/configs/ifcfg-eth0 file
	# is not found use some defaults
	my ($ip,$broadcast,$netmask,$bootproto);

	if ( open(ETH0,$eth_path) ) {
		while (<ETH0>) {
			# skip commented lines		
			next if ( $_ =~ /^(.*\#+)(.*\s+)(\S+).*/ );		
	    	if (/IPADDR/i) {
				(undef,$ip) = split /=/;
				#strip white spaces
				$ip=~s/^\s+//;
				$ip=~s/\s+$//;
				next;
			}
	    	if (/NETMASK/i) {
				(undef,$netmask) = split /=/;
				#strip white spaces
				$netmask=~s/^\s+//;
				$netmask=~s/\s+$//;
				next;
			}
	    	if (/BROADCAST/i) {
				(undef,$broadcast) = split /=/;
				#strip white spaces
				$broadcast=~s/^\s+//;
				$broadcast=~s/\s+$//;
				next;
			}
	    	if (/BOOTPROTO/i) {
				(undef,$bootproto) = split /=/;
				#strip white spaces
				$bootproto=~s/^\s+//;
				$bootproto=~s/\s+$//;
				next;
			}
			
		}	
	}else {
		print "Could not open $eth_path:$!\n" if $DEBUG;	
	}

	if ( !$ip ) {
		print "Cannot get IP ... using default!\n" if $DEBUG;
		$ip = "192.168.0.2";
	}
	if ( !$netmask ) {
		print "Cannot get NETMASK ... using default!\n" if $DEBUG;
		$netmask = "255.255.255.0";
	}
	if ( !$broadcast ) {
		print "Cannot get BROADCAST ... using default!\n" if $DEBUG;
		$broadcast = "192.168.0.255";
	}
	
	close (ETH0);
	
	# fetch dnsservers from etc/resolv.conf file
	# is not found use some defaults
	my @ns = ();
    if ( open(RESOLV,$resolv_path) ) {
		my $i = 0;
		while (<RESOLV>) {
			# skip commented lines
			next if ( $_ =~ /^(.*\#+)(.*\s+)(\S+).*/ );
	    	if (/nameserver\s+(\S+)/) { 
			$ns[$i]=$1;
			#strip white spaces
			$ns[$i]=~s/^\s+//;
			$ns[$i]=~s/\s+$//;
			$i++;
	    	}
		};
	} else {
		print "Could not open $resolv_path:$!\n" if $DEBUG;
		
		# only if resolv.conf could not be opened put some 
		# defaults ( in this case suppose we are in install mode
		# otherwise means that user don't want to use nameservers
		if (!$ns[0]) { $ns[0] = "ns1.your.domain"; }
		if (!$ns[1]) { $ns[1] = "ns2.your.domain"; }
	}
	
	
    close (RESOLV);

	$NetworkInfo{hostname} = $hostname;
	$NetworkInfo{ip} = $ip;
	$NetworkInfo{broadcast} = $broadcast;
	$NetworkInfo{netmask} = $netmask;
	$NetworkInfo{dns1} = $ns[0];
	$NetworkInfo{dns2} = $ns[1];	
	$NetworkInfo{gateway} = $gateway;
	$NetworkInfo{bootproto} = $bootproto;	
	
	return %NetworkInfo;
}

# put_network_settings - writes network settings in their coresponding files
# $ExitCode 0 if any error encounter in the files update process.
# if error encountered push also an ErrorMsg
# prototype: ($ExitCode,$ErrorMsg) SaveNetworkSetting(\%NetworkInfo)
sub SaveNetworkSettings($) {
	my $NetworkInfo = shift;
	
	my $resolv_path = $MountPoint."/etc/resolv.conf";
	my $hosts_path = $MountPoint."/etc/hosts";
	my $eth0_path = $MountPoint."/etc/config/ifcfg-eth0";
	my $network_path = $MountPoint."/etc/config/network";

	if ( ${$NetworkInfo}{bootproto}=~/static/i ) {
		foreach my $key (keys %{$NetworkInfo} ) {
			# nameservers and gateway are not a must
			next if ( $key eq "dns1" || $key eq "dns2" || $key eq "gateway" );
			return (0,"Network settings incomplete") if (!${$NetworkInfo}{$key});
		}
	}
	
	${$NetworkInfo}{dns1} = "" if ( ! ${$NetworkInfo}{dns1} );
	${$NetworkInfo}{dns2} = "" if ( ! ${$NetworkInfo}{dns2} );	
	${$NetworkInfo}{gateway} = "" if ( ! ${$NetworkInfo}{gateway} );	
	
    my $ExitCode=1;
    
	my $host;
	my $domain;
	
    # If this fails it might mean that user has entered only a hostname
    if (${$NetworkInfo}{hostname} !~ /(\w+)\.(\S+)/m) {
		$host = ${$NetworkInfo}{hostname};
		$domain = "";
    } else {
		$host = $1;
		$host =~ s/\.//;
		$domain = $2;
    }

	# write hosts file    
    if (open (HOSTS,">$hosts_path")) {
        print HOSTS	"127.0.0.1\t\tlocalhost loopback\n".
					"${$NetworkInfo}{ip}\t$host\t${$NetworkInfo}{hostname}";
		close(HOSTS);
    } else { 
		return (0,"Couldn't open $hosts_path :$!");
	}
	
	# write resolv.conf file
	# if after a nameserver follows a blank is corrent
	# in case user didn't specified any ns
	if (open(RESOLV, ">$resolv_path")) {
		print RESOLV	"domain $domain\n".
						"search $domain\n";
		print RESOLV	"nameserver ${$NetworkInfo}{dns1}\n" if ( ${$NetworkInfo}{dns1} );
		print RESOLV	"nameserver ${$NetworkInfo}{dns2}" if ( ${$NetworkInfo}{dns2} );
		close RESOLV;
	} else {
		return (0,"Couldn't open $resolv_path :$!");
	}
	
	# write network adapter config file
    if (open (ETH0, ">$eth0_path")) {
		print ETH0	"DEVICE=eth0".
					"\nBOOTPROTO=${$NetworkInfo}{bootproto}".
					"\nIPADDR=${$NetworkInfo}{ip}".
					"\nNETMASK=${$NetworkInfo}{netmask}".
					"\nBROADCAST=${$NetworkInfo}{broadcast}";
					
       close (ETH0);
	} else { 
		return(0,"Couldn't open $eth0_path :$!");
    }
	
	# write network setting
	if (open (NETWORK,">$network_path")) {
		print NETWORK	"NETWORKING=yes\n".
						"FORWARD_IPV4=yes\n".
						"DEFRAG_IPV4=no\n".
						"HOSTNAME=${$NetworkInfo}{hostname}\n".
						"GATEWAY=${$NetworkInfo}{gateway}";
	 	close (NETWORK);
    } else { 
		return (0,"Couldn't open $network_path :$!");
    }

    return $ExitCode;
}

# End of network functions

########################
# Users handling library

# Funtion for scanning passwd file to get users info
# USERS = (
#		login 	= $login,
#		passwd 	= $passwd,
#		disabled= 0 | 1,
#		uid		= $uid,
#		gid		= $gid,
#		name	= $name,
#		home	= $home,
#		shell	= $shell,
#		modified= * | ' '
#		hidden  = 0|1 ( if it is hidden like adm,halt,shutdown etc )
#		system  = 0|1 ( for this users cannot modify home,shell and login )
#		newuser - 0|1 ( if is added new or not from Interface )
#	);
# prototype: @USERS GetUsersTree()
sub GetUsersTree() {

	my @USERS=();
	my $i = 0;
	
	my $PASSWD_FILE = $MountPoint."/etc/passwd";

	if ( ! open(PASSWD, "$PASSWD_FILE") ) {
		if ( $DEBUG ) {
			die "Cannot open $PASSWD_FILE.:$!";
		} else {
			return undef;
		}
	}
	
	while (<PASSWD>) {
		my ($login,$passwd,$uid,$gid,$name,$home,$shell) = split(/:/);
	
		next if ( $login eq "" || $passwd eq "" || $uid eq "" || $gid eq "");
		#chomp the last variable to exclude \n
		chomp $shell;

		if ($passwd ne "*") {
		    $USERS[$i]{disabled} = 0;
		}
		else {
		    $USERS[$i]{disabled} = 1;
		}
	    		$USERS[$i]{login} = $login;
	    		$USERS[$i]{passwd} = $passwd;
	    		$USERS[$i]{uid} = $uid;
	    		$USERS[$i]{gid} = $gid;
	    		$USERS[$i]{name} = $name;
	    		$USERS[$i]{home} = $home;
	    		$USERS[$i]{shell} = $shell;
			$USERS[$i]{hidden} = 0;
			# detect if it's a hidden user
			for  my $j (0..$#HIDDEN_USERS ) {
				if ( $login eq $HIDDEN_USERS[$j] ) {
					$USERS[$i]{hidden} = 1;
					last;
				}
			}
		$USERS[$i]{system} = 0;
		# detect if it's a system user
		for  my $j (0..$#SYSTEM_USERS ) {
			if ( $login eq $SYSTEM_USERS[$j] ) {
				$USERS[$i]{system} = 1;
				last;
			}
		}
		$USERS[$i]{modified} = " ";
		$USERS[$i]{newuser} = 0;
		$i++;
	}
	close (PASSWD);
	
	return @USERS;
}

# Function for adding a user home dir
# passwd part in handled by SaveUsersTree
# userdata is a hash described above with user information
sub UserAdd($) {

	my $userdata  = shift;

	print "Adding user: $$userdata{login}\n" if ( $DEBUG );
	system("mkdir -p $MountPoint/$$userdata{home} >/dev/null 2>&1");
	system("chown $$userdata{uid}.$$userdata{gid} $MountPoint/$$userdata{home} >/dev/null 2>&1");
	system("chmod 700 $MountPoint/$$userdata{home} >/dev/null 2>&1");
	
	return 1;
}

# Function to modify a user (home)
# passwd (login/name/shell ) part in handled by SaveUsersTree
# userdata is a hash described above with user information
sub UserMod($$) {
	
	my $old_userdata = shift;
	my $new_userdata = shift;

	print "Changing home for user:$$new_userdata{login} from $MountPoint/$$old_userdata{home} --> to $MountPoint/$$new_userdata{home}\n" if ( $DEBUG );
	system("mv $MountPoint/$$old_userdata{home} $MountPoint/$$new_userdata{home}  >/dev/null 2>&1");
	system("chown $$new_userdata{uid}.$$new_userdata{gid} $MountPoint/$$new_userdata{home} >/dev/null 2>&1");
	
	return 1;
}

# Function for deleting a user home dir
# passwd part in handled by SaveUsersTree
# userdata is a hash described above with user information
sub UserDel($) {

	my $userdata  = shift;
	
	print "Deleting user: $$userdata{login}\n" if ( $DEBUG );
	system("rm -fr $MountPoint/$$userdata{home} >/dev/null 2>&1");

	return 1;
}

# Function for saving entire USERS tree to $PASSWD_FILE
# prototype: SaveUsersTree(\@USERS)
sub SaveUsersTree($) {
    
    my $USERS = shift;

	my $PASSWD_FILE = $MountPoint."/etc/passwd";
	
    return 0 if ( ! $PASSWD_FILE || ! $USERS );
	
    open(PASSWD, ">$PASSWD_FILE") or die "Cannot open $PASSWD_FILE for writing: $!";
	
    for (my $i=0;${$USERS}[$i];$i++){
    print PASSWD 	
	    join (':',${$USERS}[$i]{login},${$USERS}[$i]{passwd},${$USERS}[$i]{uid},
	    ${$USERS}[$i]{gid},${$USERS}[$i]{name},${$USERS}[$i]{home},${$USERS}[$i]{shell});
    print PASSWD "\n";
    }
    close (PASSWD);
}

# Function for scanning $HTTP_PASSWD_FILE for http users info
# HTTP_USERS = (
#		login 	= $login,
#		passwd 	= $passwd,
#		disabled= 0 | 1,
#		modified= * | ' '
#	);
# prototype: @HTTP_USERS GetHTTPUsersTree()

sub GetHttpUsersTree() {
	
    my @HTTP_USERS=();
    my $i=0;
	
    my $HTTP_PASSWD_FILE = $MountPoint."/etc/httpd/access/.htpasswd";
	
    if ( ! open(PASSWD, "$HTTP_PASSWD_FILE") ) {
		if ( $DEBUG ) {
			die "Cannot open $HTTP_PASSWD_FILE: $!";
		} else {
			return undef;
		}
	}

	
    while (<PASSWD>) {
	my ($login,$passwd) = split(/:/);
	
	#chomp the last variable to exclude \n
	chomp $passwd;
	
	next if ( $login eq "" || $passwd eq "" );
	
	if ($passwd ne "*") {
	    $HTTP_USERS[$i]{disabled} = 0;
	}
	else {
	    $HTTP_USERS[$i]{disabled} = 1;
	}
	    $HTTP_USERS[$i]{login} = $login;
	    $HTTP_USERS[$i]{passwd} = $passwd;
	    $HTTP_USERS[$i]{modified} = " ";
		
		# detect if it's a system http user
		$HTTP_USERS[$i]{system} = 0;
		for  my $j (0..$#HTTP_SYSTEM_USERS ) {
		  if ( $login eq $HTTP_SYSTEM_USERS[$j] ) {
			$HTTP_USERS[$i]{system} = 1;
			last;
		  }
		}
	    $i++;
    }
    close (PASSWD);
	
	return @HTTP_USERS;
}

# Function for saving entire HTTP_USERS tree to $HTTP_PASSWD_FILE
# prototype: SaveHTTPUsersTree(\@HTTP_USERS)
sub SaveHttpUsersTree($) {
    my $HTTP_USERS = shift;
	
    my $HTTP_PASSWD_FILE = $MountPoint."/etc/httpd/access/.htpasswd";	
	
    open(PASSWD, ">$HTTP_PASSWD_FILE") or die "Cannot open $HTTP_PASSWD_FILE for write: $!";
	
    for (my $i=0;${$HTTP_USERS}[$i];$i++){
    	print PASSWD join (':',${$HTTP_USERS}[$i]{login},${$HTTP_USERS}[$i]{passwd});
		print PASSWD "\n";
    }
    close (PASSWD);
}

# Function for crypting password
# prototype: $password EncodePasswd($plain_text_password)
sub EncodePasswd($) {
    my $plain_text_password = shift;
    my ($salt, $salt1, $salt2, $encrypted);
    
    $salt1 = join '', ('.', '/', 0..9, 'A'..'Z', 'a'..'z')[rand 64, rand 64];
    $salt2 = substr($salt1, rand(length($salt1)), rand(length($salt1)));
    $salt = join ('', $salt1,$salt2);
    $encrypted = crypt($plain_text_password, $salt);
    return ($encrypted);
}

# When modifing a user or creating a new one
# must have login and homedir unique
# prototype ValidateChanges($username,$homedir,$userindex,\@USERS,\%duplicate)
sub ValidateUserChanges($$$$$) {
	my $username = shift;
	my $homedir = shift;
	my $userindex = shift;
	my $USERS = shift;
	my $duplicate = shift;
	
	my $valid = 1;
	chomp $username;
	chomp $homedir;
	
	for my $i (0..$#{$USERS}) {
		next if ($i == $userindex);
		if ( ${$USERS}[$i]{login} eq $username) {
			$valid = 0;
			${$duplicate}{username} = ${$USERS}[$i]{login};
			${$duplicate}{property} = "Username";
		}
		if ( ${$USERS}[$i]{home} eq $homedir) {
			$valid = 0;
			${$duplicate}{username} = ${$USERS}[$i]{login};
			${$duplicate}{property} = "Home Directory";
		}
	}
	return $valid;
}

# Funtion to generate a valid UID
# parse @USERS get maxim uid from it
# return maxim+1 or -1 if an error occurs
# prototype: $uid GenerateUID(\@USERS)
sub GenerateUID($) {
	my $USERS = shift;
	
	my $uid = -1;
	for my $i (0..$#{$USERS}) {
		$uid = ${$USERS}[$i]{uid} if ( ${$USERS}[$i]{uid} > $uid );
	}
	return ($uid+1);
}

# Function for validating shell
# prototype: $ExitCode ValidateShell($SHELL)
sub ValidateShell($) {
	my $SHELL = shift;
	return 0 unless  -e $SHELL;
	return 1 ;
}
################
# Debug Tools
################

# Print UsersTree to screen
# usefull in script and debugging
# prototype: PrintUsersTree(\@USERS)
sub PrintUsersTree($) {
	my $USERS = shift;
	
	for (my $i=0;${$USERS}[$i];$i++){
		print "$i -  ${$USERS}[$i]{login} : ${$USERS}[$i]{passwd} : ${$USERS}[$i]{uid}\n";
	}
}

# End of users functions
1;