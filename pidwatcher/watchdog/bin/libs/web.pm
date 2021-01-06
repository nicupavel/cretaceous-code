#!/usr/bin/perl
$| = 1;
#----------------------------------------------------------------------------------------------------------
# WebRelated functions
#----------------------------------------------------------------------------------------------------------
sub normal_type_header ($) 
{
    
    my $cgi = shift;
    print $cgi->header(-type=>'text/html',
                     -status=>'200 OK',
                     -expires=>'0',
                     -cookie=>$cookie
                     );
}

sub redirect()
{

        local ($url) = shift;
        print "Location: $url\n\n";
}

sub js_redirect()
{
        local ($url) = shift;
        print "<script language=Javascript>
        <!--
                if (location.href == top.location.href) {
                        top.location.href = \"$url\" }
        // -->
        </script>"
}

# Render a HTML Template replacing tags like <!!KEY!!> with KEY value.
sub render()
{

        local($template) = shift;
        %vars = @_;
        my $buffer;

        open TEMPLATE,$template or print "[web.pm -- template $template error :   $!]";
        while (<TEMPLATE>){ $buffer .=$_;};
        close TEMPLATE;


        local(@keys);
        local($key);
        local($params);
        local(@param);

        @keys= ($buffer =~ /<!!(.*?)!!>/g);


        foreach $key(@keys){

                 if (defined($$key)){ $buffer =~ s/<!!$key!!>/$$key/g; }
                 elsif (defined($vars{$key})){ $buffer =~s/<!!$key!!>/$vars{"$key"}/g; }
                 elsif (defined($template_buffer{$key})){ $buffer =~s/<!!$key!!>/$template_buffer{"$key"}/g; }
                 else { $buffer =~s/<!!$key!!>//g; }
        }

        print $buffer;
}
#----------------------------------------------------------------------------------------------------------
1; 
