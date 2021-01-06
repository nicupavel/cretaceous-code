#!/usr/bin/perl

use CGI qw (:all);


my @browser = cookie("Mediabox");
if (defined @browser) {
    print (header(),start_html("Cookie: $browser[0] $browser[1]"),end_html);
}
else {
    my $cookie = cookie(-name=>'Mediabox',
		    -value => ['cpu','ascendent']);
			    
    print (header(-cookie=>$cookie));
}