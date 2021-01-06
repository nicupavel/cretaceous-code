#!/usr/bin/perl

$| = 1;
#----------------------------------------------------------------------------------------------------------
# Utility functions
#----------------------------------------------------------------------------------------------------------
sub WebAdminVersion()
{
    my $VersionString = "WebAdmin - 2.6.6- 12-09-2003";
    return $VersionString;
}

sub InsertFileDate($)
{
    my $file_name = shift;
    $file_name = $file_name ? $file_name : "untitled";
    
    my $pos = 0;
    my ($month,$day,$year,$hour,$min,$sec)=(localtime)[4,3,5,2,1,0];
    
    $month++;
    $year+=1900;
     
    my $datestr="[" . $month . "-" . $day . "-" . $year . "-" . $hour ."-" . $min . "-" . $sec . "]";
    
    if (($pos = rindex($file_name, ".")) == -1) {
	$pos = length ($file_name);
    }
    
    my $newstr = substr ($file_name,0,$pos) . $datestr . substr($file_name,$pos);
    
    return $newstr;
}

sub PathExists($)
{
    return (stat(@_))[0] ? 1 : 0;
}
#----------------------------------------------------------------------------------------------------------
# AddLCDKeys(LCD_KEYS_FILE, LCD_KEY, MODULE_CATEGORY, MODULE_NAME)
#----------------------------------------------------------------------------------------------------------
sub AddLCDKeys($$$$)
{
    my ($file, $key, $module_category, $module_name) = @_;
    my @LCDKeys = ();
    @LCDKeys = GetLCDKeys($file);
    open (FILE, ">$file") || die "Cannot open: '$file': $!\n";
    
    print FILE "KEY=$key\n";
    print FILE "MODULE_CATEGORY=$module_category\n";
    print FILE "MODULE_NAME=$module_name\n";
    print FILE "\n";
    
    for (my $i=0;$LCDKeys[$i];$i++) {
	if ($LCDKeys[$i]{'KEY'} eq $key) { next; }
	print FILE "KEY=$LCDKeys[$i]{'KEY'}\n";
	print FILE "MODULE_CATEGORY=$LCDKeys[$i]{'MODULE_CATEGORY'}\n";
	print FILE "MODULE_NAME=$LCDKeys[$i]{'MODULE_NAME'}\n";
	print FILE "\n";
    }
    close (FILE);
}


#----------------------------------------------------------------------------------------------------------
# DeleteLCDKeys(LCD_KEYS_FILE, LCD_KEY) Very similar to Add but we don't have so many params
#----------------------------------------------------------------------------------------------------------
sub DeleteLCDKeys($$)
{
    my ($file, $key) = @_;
    my @LCDKeys = ();
    
    @LCDKeys = GetLCDKeys($file);
    open (FILE, ">$file") || die "Cannot open: '$file': $!\n";
    for (my $i=0;$LCDKeys[$i];$i++) {
	if ($LCDKeys[$i]{'KEY'} eq $key) { next; }
	print FILE "KEY=$LCDKeys[$i]{'KEY'}\n";
	print FILE "MODULE_CATEGORY=$LCDKeys[$i]{'MODULE_CATEGORY'}\n";
	print FILE "MODULE_NAME=$LCDKeys[$i]{'MODULE_NAME'}\n";
	print FILE "\n";
    }
    close (FILE);
}

#----------------------------------------------------------------------------------------------------------
# GetLCDKeys(LCD_KEYS_FILE); returns an array of hashes with data.
#----------------------------------------------------------------------------------------------------------
sub GetLCDKeys($)
{
    my $file = shift;
    my @LCDKeys = ();
    my $i = -1;
    
    open (FILE, "< $file") || return;
    while (<FILE>){
	chomp;                  # no newline
	s/#.*//;                # no comments
	s/^\s+//;               # no leading white
	s/\s+$//;               # no trailing white
	next unless length;     # anything left?
	my ($var, $value) = split(/\s*=\s*/, $_, 2);
	if ($var eq "KEY") {  $i++; }
	$LCDKeys[$i]{$var} = $value;
    }
    
    close (FILE);
    return @LCDKeys;
}

#----------------------------------------------------------------------------------------------------------
# GetLCDKey(LCD_KEYS_FILE, LCD_KEY) Test if parameter a key exists as a LCDKEY with his values.
#----------------------------------------------------------------------------------------------------------
sub GetLCDKey($$)
{
    my ($file,$key) = @_;
    my @LCDKeys = GetLCDKeys($file);
    my %LCDKey = ();    
    for (my $i=0;$LCDKeys[$i];$i++) {
	if ($LCDKeys[$i]{'KEY'} eq $key) {
	    %LCDKey = % { $LCDKeys[$i] };
	    return %LCDKey;
	}
    }
    return;
}

#----------------------------------------------------------------------------------------------------------
# This makes the params for starting the module, It's a Hash which contains 2 keys and 2 arrays.
#----------------------------------------------------------------------------------------------------------
sub MakeParamsFile($)
{
    my $pParams = shift;
    
    my $PARAMS_FILE="/mediabox/webadmin/job-control/modules/".$pParams->{module_name}.".params";
    
    open (PARAMS, ">$PARAMS_FILE") or die "Cannot write to $PARAMS_FILE: $!";
    foreach my $key (keys %$pParams) {
	if ($key eq "devices" || $key eq "devices_path") {
	    next;
	}
	print PARAMS "$key=$pParams->{$key}\n";
    }
    
    for (my $i=0;$pParams->{devices}[$i];$i++) {
	print PARAMS "device=".$pParams->{devices}[$i]."\n";
	print PARAMS "device_path=".$pParams->{devices_path}[$i]."\n";
    }
    
    close (PARAMS);

}
#----------------------------------------------------------------------------------------------------------
# This makes the populates the %params from an array usualy ARGV (used by Scheduling operation).
#----------------------------------------------------------------------------------------------------------
sub MakeParamsFromArgv($)
{
    my $pArgv = shift;
    my %params = ();
    my $j = 0;
    
    $params{module_name} = $pArgv->[$i];
    print "In MakeParamsFromArgv\n";
    for (my $i = 1; $pArgv->[$i]; $i++) {

	if ($pArgv->[$i] =~ /^module_watchdog=(.*?)$/) {
	    $params{module_watchdog} = $1;
	    next;
	}
	# Command will be unquoted in hash due to assignment
	if ($pArgv->[$i] =~ /^module_command=(.*?)$/) {
	    $params{module_command} = $1;
	    print "Module Command is: $1\n";
	    next;
	}
	if ($pArgv->[$i] =~ /^devices=(.*?)$/) {
	    $params{devices}[$j] = $1;
	    $i++;
	    (my $tmp, $params{devices_path}[$j]) = split(/=/,$pArgv->[$i]);
	    $j++;
	    next;
	}
    }
    return %params;    
}

#----------------------------------------------------------------------------------------------------------
# This makes the a command line (string)  for starmodule from %params (used by Scheduling operation).
#----------------------------------------------------------------------------------------------------------
sub MakeArgvFromParams($)
{
    my $pParams = shift;
    #First parameter should be module name
    my $argv =$pParams->{module_name}." ";
    
    for (my $i=0;$pParams->{devices}[$i];$i++) {
	$argv .= "devices=".$pParams->{devices}[$i]." ";
	$argv .= "devices_path=".$pParams->{devices_path}[$i]." ";
    }
        
    $argv .= "module_watchdog=".$pParams->{module_watchdog}." ";
    # Quoting command here to be consideres as 1 argument
    $argv .= "module_command=\"".$pParams->{module_command}."\" ";

    return $argv;
}
#----------------------------------------------------------------------------------------------------------
sub ReadParamsFile($)
{
    my $module = shift;
    my %params=();
    my $PARAMS_FILE="/mediabox/webadmin/job-control/modules/".$module.".params";
    
    open (PARAMS, "$PARAMS_FILE") or return undef;
    while (<PARAMS>) {
	chomp;                  # no newline
	s/#.*//;                # no comments
	s/^\s+//;               # no leading white
	s/\s+$//;               # no trailing white
	next unless length;     # anything left?
	my ($var, $value) = split(/\s*=\s*/, $_, 2);
	if ($var eq "device") { 
	    push ( @{ $params{devices}},$value); 
	    next;
	}
	if ($var eq "device_path") { 
	    push ( @{ $params{devices_path}},$value); 
	    next;    
	}
	$params{$var}=$value;
    }
    close (PARAMS);
    
    return %params;
}

#----------------------------------------------------------------------------------------------------------
sub GetAllDevices($)
{
    # The first param is the path without ext. to the file(s) containing the devices
    my $DeviceBasePath = shift;
    
    my $DEVS_ALL_FILE  = "$DeviceBasePath.all";
    
    # This variable is used to get the list with all devices
    my @AllDevices = ();
    
    # Read the contents of file containing all devices and store
    #  each line as a new found device into 'AllDevices' array
    open(FILE_ALL, "< $DEVS_ALL_FILE") || warn "Unable to open devs all file: $!\n"; 
    while (my $LineA = <FILE_ALL>) {
	chomp $LineA;
	
	if ($LineA ne "") {
	    push(@AllDevices, $LineA);
	}
    }
    close(FILE_ALL);    
    
    # Return the 'AllDevices' array we just constructed
    return @AllDevices;
}
#----------------------------------------------------------------------------------------------------------
sub GetFreeDevices($)
{
    # The first param is the path without ext. to the file(s) containing the devices
    my $DeviceBasePath = shift;
    
    my $DEVS_ALL_FILE  = "$DeviceBasePath.all";
    my $DEVS_USED_FILE = "$DeviceBasePath.used";
    
    # This variable is used to get the list with all free devices
    my @FreeDevices = ();
    
    # These are temporary variables
    my($Used, $LineA, $LineU);

    # Read the list containing all devices into 'FileAllContent'
    open(FILE_ALL, "< $DEVS_ALL_FILE")
	|| warn "Unable to open devs all file: $!\n"; 
    my @FileAllContent = <FILE_ALL>;
    close(FILE_ALL);    
    
    # Read the list containing used devices into 'FileUsedContent'
    open(FILE_USED, "< $DEVS_USED_FILE")
	|| warn "Unable to open devs used file: $!\n"; 
    my @FileUsedContent = <FILE_USED>;
    close(FILE_USED);
    
    # Construct the list with all free devices
    foreach $LineA(@FileAllContent) {
	# Some line processing
        chomp($LineA);
	
	# If the current line is empty after processing
	# then skip to the next line
	if ($LineA eq "") {
	    next;
	}
	
	$Used = 0;
	
	# Check if the line is contained by 'DEVS_USED_FILE'
        foreach $LineU(@FileUsedContent) {
	    chomp($LineU);
	    if ($LineA eq $LineU) {
		$Used = 1;
		last;
	    }    	
	}
	
	# If it isn't, then the corresponding device is free
	# and we must append it to 'FreeDevices' array
	if (!$Used) {
	    push(@FreeDevices, $LineA);	    
	}
    }
    
    # Return the 'FreeDevices' array we just constructed
    return@FreeDevices;
}
#----------------------------------------------------------------------------------------------------------
sub FindDevice($$)
{
    # The first param is a pointer to an array that contains 
    # the a list with devices
    # The second param is a string that contains a specific
    # the device that must be searched within this list
    my ($pDevices, $Device) = @_;
    
    my $Found = 0;
    for (my $i = 0; $pDevices->[$i]; ++$i) {
	if ($pDevices->[$i] eq $Device) {
	    $Found = 1;
	    last;
	}
    }
    
    return $Found;
}
#----------------------------------------------------------------------------------------------------------
sub DisplayFreeDevices($$$)
{
    # The first param is the path without ext. to the file(s) containing the devices
    # The second param is the name of the select HTML object
    # The third param is a string describing the device that must be shown as 'selected'
    # Note the desired selected device may not be available to show anymore, if
    # another encoder has occupied it in the meantime
    my ($DeviceBasePath, $SelectName, $SelectedDevice) = @_;

    # Get the list with all free devices
    my @FreeDevices = GetFreeDevices($DeviceBasePath);
    
    my $FreeDevicesExist = $FreeDevices[0] ? 1 : 0;
	    
    # If no free device exists, display 'No free device' standard message
    if (!$FreeDevicesExist) {
	print "<font color=red><b>No free device</b></font>";
    }
    # If we have at least one free device, generate a '<select>...<option>' sequence
    else {
        print "<select name=\"$SelectName\">";
	
	foreach my $FreeDevice(@FreeDevices) {
	    print "<option value=\"$FreeDevice\"";
	    
	    if ($FreeDevice eq $SelectedDevice) {
		print " selected";
	    }
	    
	    print ">$FreeDevice</option>\n";
	}
	                                                                
	print "</select>";
    }
    
    print "\n";
    return $FreeDevicesExist;
}
#----------------------------------------------------------------------------------------------------------
sub DeclareDeviceAsUsed($$)
{
    # The first param is the path without ext. to the file(s) containing the devices
    # The second param is a string describing the device that must become used
    my ($DeviceBasePath, $UsedDevice) = @_;

    # Append the a new line containing the new device that must be declared as used
    open (USED,">>$DeviceBasePath.used") or warn "Cannot append to $DeviceBasePath.used: $!";
    print USED "$UsedDevice\n";
    close (USED);
    
}
#----------------------------------------------------------------------------------------------------------
sub DeclareDeviceAsFree($$)
{
    # The first param is the path without ext. to the file(s) containing the devices
    # The second param is a string describing the device that must become free
    my ($DeviceBasePath, $MustBeFreeDevice) = @_;

    my $DEVS_USED_FILE = "$DeviceBasePath.used";
    
    # Get the current time and construct the name of a temporary file 
    # which will be used to write the updated list of currently used devices
    my $time = time;
    
    # Open this newly created file for writing
    open(UPDATED_USED_FILE, "> $DEVS_USED_FILE.$time") || die "Unable to open timed devs used file: $!\n";
     
    # These are temporary variables
    my $LineU;
    
    # Parse the list containing the currently used devices, by reading 'UsedFile'
    open(USED_FILE, "< $DEVS_USED_FILE") || die "Unable to open devs used file: $!\n"; 
    while ($LineU = <USED_FILE>) {
	chomp($LineU);	
	
	print UPDATED_USED_FILE "$LineU\n" if ($LineU ne $MustBeFreeDevice);
    }
    
    close(USED_FILE);
    close(UPDATED_USED_FILE);
    
    # Now we can overwrite the old 'UsedFile' with 'UpdatedUsedFile'
    system "cat \"$DEVS_USED_FILE.$time\" > \"$DEVS_USED_FILE\"";
    system "rm -f \"$DEVS_USED_FILE.$time\"";
}
#----------------------------------------------------------------------------------------------------------
sub EncodeURL($)
{
    my $input = shift;
    
    # Disable warnings
    no warnings;
    
    $input =~ s/([^a-zA-Z0-9_.-])/uc sprintf("%%%02x", ord($1))/eg;
    return $input;
}
#----------------------------------------------------------------------------------------------------------
sub EmptyFile($)
{
    my $file = shift;
    open (TMP, ">$file") or die "Can't access $file to remove contents: $!\n";
    close(TMP);
}

1; 
