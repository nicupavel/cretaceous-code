
$mailprog = '/usr/lib/sendmail';
@referers = ('www.ecards.ro','www.infoiasi.ro');
$auto_type = 'image/gif';
$auto_encoding = 'base64';
$auto_body = '../templates/mail_body.tmpl';
$message_type = 'multipart/mixed';
$email_log = 'ecards.log';

use MIME::Entity; 

open (TXT,"$auto_body") || &endIt;
flock (TXT,2); seek (TXT,0,0);
@Message_Text = <TXT>;
close (TXT);
$Body = "@Message_Text";

#$RealFile = $Config{'file'};
#if ($RealFile =~ /\//) { ($RealFile) = $Config{'file'} =~ /^.*\/([^\/]+)$/g; }

$Config{'recipient'}="ecard@ecards.ro";
$Config{'email'}="root";
$Config{'subject'}="You have received a new postcard";

sub send_attach_mail {
    local($print_config,$key,$sort_order,$sorted_field,$env_report);

    $top = build MIME::Entity Type => $message_type,
    From => $Config{'recipient'},
    To => $Config{'email'},
    Subject => $Config{'subject'};

    attach $top Data=> $Body;


    attach $top Path => "../cards/misc/1.jpg",
    Type => $auto_type,
    Encoding => $auto_encoding;

    open MAIL, "|$mailprog -t -i" or die "open: $!";
    $top->print(\*MAIL);
    close MAIL;
    &update_log;
}

sub update_log {

open (LOG, ">>$email_log") || &endIt;
print LOG "$email\n"; 
close (LOG);

}

1;