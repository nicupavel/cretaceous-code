#Send E-Mail Functions written by PaNiC MIME::Entity is needed for attachaments mails
#
#ChangeLog:
# ver 0.0.1 - endIt(),send_attach_mail() and update_email_log ()
# ver 0.0.2 - send_attach_mail()
# ver 0.0.3 - send_mail()
# ver 0.0.4 - get_email_footer()

use MIME::Entity;

$reference_dir='/home/httpd/www.ecards.ro/html/';
$mailprog = '/usr/lib/sendmail';
$auto_type = 'image/gif';
$auto_encoding = 'base64';
$auto_body = "$reference_dir/templates/mail_body_html.tmpl";
$message_type = 'multipart/mixed';
$email_log = "$reference_dir/log/ecards.log";

$from_ecards = 'ecards@ecards.ro';
$from_extended = 'e-Cards Service <ecards@ecards.ro>';

$date = &get_long_date(time);

sub send_attach_mail () {

$file = shift;
$category = shift;
$to = shift;
$from = shift;
$from_name = shift;
$body = shift;




    $footer=&get_email_footer;

    if ($file =~ /\//) { ($file) =~ /^.*\/([^\/]+)$/g; }
    $path = "$reference_dir/cards/$category/$file",

    $HTML_Body .= $body;
    $HTML_Body .= "<BR><BR><b>This postcard was sent by <i> $from_name</i>.You can reply to <a href=\"mailto:$from\">$from</a>.</b>";

    if    ($file =~ /\.gif$/i)   { $graphic_type = "image/gif" }
    elsif ($file =~ /\.jpe?g$/i) { $graphic_type = "image/jpeg" }
    elsif ($file =~ /\.png$/i)   { $graphic_type = "image/png" }
    else                            { die "unknown type for: $file\n"; }
   
    my $gid = "ecard";
    my $msg = MIME::Entity->build(To      => $to,
				  Subject => "$from_name has sended you a postcard.",
				  Type    => 'multipart/mixed',
				  From	  => $from_extended); #multipart/alternative
				

   my $HTML = $msg->attach(Type => 'multipart/mixed'); #multipart/related
      $HTML->attach(Type => 'text/html',
		      Data => $HTML_Body);
           
      $HTML->attach(Type => $graphic_type,
                   Encoding => $auto_encoding,
		   Path => $path,
                   Id   => $gid);
      $HTML_Body = $footer;

      $HTML->attach(Type => 'text/html',
	            Data => $HTML_Body);


    open MAIL, "|$mailprog -t -i -oi -oem -f $from_ecards" or die "open: $!";
    $msg->print(\*MAIL);
    close MAIL;
    $log ="$date -- $ENV{'REMOTE_ADDR'}-- $from -- $to -- $category -- $file\n";
    &update_email_log($log);

}


sub send_mail(){

	$to = shift;
	$subj = shift;
	$mesg = shift;
	
	&get_email_footer;
	
	open MAIL , "|$mailprog -f $from $to >/dev/null";
	print MAIL "From: $from_extended\nSubject: $subj\n\n\n $mesg \n\n.\n";
	close MAIL;
  $log ="$date -- $ENV{'HTTP_HOST'}-- $to \n";
	&update_email_log($log);

}


sub update_email_log {
  $log = shift;
	open (LOG, ">>$email_log") or die "log: $!";
	print LOG $log;
	close (LOG);

}

sub get_email_footer (){
	open (TXT,"$auto_body") or die "get_footer: $!";
	flock (TXT,2); seek (TXT,0,0);
	@Message_Text = <TXT>;
	close (TXT);
	$Body = "@Message_Text";
	return ($Body);
}

1;