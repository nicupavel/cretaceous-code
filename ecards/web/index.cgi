#!/usr/bin/perl

require "config.pm";

$action = $q->param('action');

if (!$action) {$action = 'start';}

if (&db_connect()) {


        if ($action eq 'start') {
		&normal_type_header();
		$title="Welcome to www.ecards.ro your electronic postcard service";
		&html_head($title);
		$date = &get_long_date(time);
		$sql = "select distinct category from ecards";
		@LoH = &db_select_fields('ecards',$sql);
		$list_category = "<select name=category size=$results>";
		for ($i=0;$LoH[$i];$i++) {
		
		$list_category.="<option value=$LoH[$i]{category}>$LoH[$i]{category}";
		
		}

		$list_category.="</select>";
		$template = 'templates/index.tmpl';
		&render($template);
	
	

	}
	if ($action eq 'list') {
		$category = $q->param ('category');
		&normal_type_header();
		$title="Select your preffered e-card";
		&html_head($title);
		$sql = "select * from ecards where category=\'$category\'";
		@LoH = &db_select_fields('ecards',$sql);
		$files = "<tr>";
		$checked="checked";
		for ($i=0;$LoH[$i];$i++) {
		
		$files.="<td bgcolor=white width=120 align=center valign=middle><BR><b>select</b><BR><input type=radio name=ecard value=$LoH[$i]{id} $checked><br><img src='./cards/$category/thumbnails/t-$LoH[$i]{file_name}' border=0><br><hr>$LoH[$i]{description}</td>";
		$checked="";
		if ($i eq 5) { $files.="</tr><tr>";}
		}

		
		$template = 'templates/category.tmpl';
		&render($template);
	
	

	}
	

	if ($action eq 'Send')
	    {
		$ecard = $q->param('ecard');
		$sql = "select * from ecards where id=$ecard";
		
		@LoH = &db_select_fields('ecards',$sql);
		$category_name=$LoH[0]{category};
		$img = "<tr><td bgcolor=white width=120 align=center valign=middle><BR><img src='./cards/$category_name/$LoH[$i]{file_name}' border=0><BR>&nbsp;</td></tr>";
		&normal_type_header();
		$title="Sending E-Card";
		&html_head($title);
		$template = 'templates/sending.tmpl';
		&render($template);
	}

	if ($action eq 'Done')
	    {
		$id = $q->param('id');
		$category = $q->param ('category');
		$to = $q->param ('to');
		$from = $q->param ('from');
		$from_name = $q->param ('from_name');
		$body = $q->param('body');
		
		$sql = "select file_name from ecards where id=\'$id\'";
		@LoH = &db_select_fields('ecards',$sql);
		&normal_type_header();
		$title="Mail Sent ...";
		&html_head($title);
		$file= $LoH[0]{file_name};
		&send_attach_mail ($file,$category,$to,$from,$from_name,$body,$format);
		&alert ("Mail to $to has been queued.");
		&redirect ('index.cgi');
	}
}

else {
	&normal_type_header();
	print "<h1>Connect to database $db_name at $db_host host failed !</h1>";
	}	


