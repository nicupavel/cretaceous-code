# HTML Lib written by PaNiC

sub normal_type_header () {

	print $q->header(-type=>'text/html',
			-status=>'200 OK',
                        -expires=>'+3d',
                        -cookie=>$cookie
                           );
}

sub html_head() {
	
	local ($title) = shift;
	if (@_) { $keywords = shift; }
	if (@_) { $style = shift;  }
	if (@_) { $bgcolor = shift; }

	print $q->start_html(-title=>$title,
                            -author=>'panic@n0i.net',
                            -base=>'true',
                            -meta=>{'keywords'=>$keywords,
                                    'copyright'=>'Copyright 2000 PaNiC',
                                    'expires'=>'0'},
                            -style=>{'src'=>$style},
                            -dtd=>1,
                            -BGCOLOR=>$bgcolor);

}

sub set_auth_cookie() {

        local ($cookie) = $q->cookie(-name=>'db_auth',
                                    -value=>\%db_auth_info,
                                    -expires=>'+1h',
                                    -path=>'/');

 	return ($cookie);
 	}
 	
sub read_auth_cookie() {

	local (%cookie) = $q->cookie('db_auth');
	
	return (%cookie);
	
	}
	
	
	
sub make_session_id () {

	$session_id = (time ^ ($$+($$<<35)+$<)) || (time || $$);
	return ($session_id);
	
	}

sub redirect(){
$url=shift;


$url =~ s/'/\'/g;

print "
<script language=javascript>
location.href=\'$url\';
</script>
";

}

sub alert(){
	$message=shift;

	print "
	<script language=javascript>
	alert (\'$message\');
	</script>
	";

}

1;