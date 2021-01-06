#!/usr/bin/perl

require "config.pm";

$action = $q->param('action');

if (!$action) {$action = 'start';}


if (&db_connect()) {

	
	if ($action eq 'start') {
		
	  &normal_type_header();
		$title="Welcome to www.ecards.ro Administration Area";
		&html_head($title);
		$date = &get_long_date(time);
		$sql = "select distinct category from ecards";
		@LoH = &db_select_fields('ecards',$sql);
		$list_category = "<select name=category_select>";
		for ($i=0;$LoH[$i];$i++) {
		
		$list_category.="<option value=$LoH[$i]{category}>$LoH[$i]{category}";
		
		}

		$list_category.="</select>";
		&render('../templates/admin.tmpl');
	 }

	
	if ($action eq 'create') {
		
	  &normal_type_header();
		$title="Welcome to www.ecards.ro Administration Area";
		&html_head($title);
		
		 }
	
	if ($action eq 'delete') {
	
   	&normal_type_header();
		$title="Deleting E-Cards";
		&html_head($title);
		$date = &get_long_date(time);
		$template = '../templates/admin_delete.tmpl';
		
		$sql = "select distinct category from ecards";
    my (@LoH_Category) = &db_select_fields('ecards',$sql);
    for ($i=0;$LoH_Category[$i];$i++){
	                $category_name=$LoH_Category[$i]{category};
									$sql = "select * from ecards where category='$category_name'";
									my (@LoH) = &db_select_fields('ecards',$sql);
			$files="<tr>";
			for ($j=0;$LoH[$j];$j++) {

				$files.="<td>$LoH[$j]{id}.<BR><b>select</b>&nbsp;&nbsp;<input type=radio name=ecards value=$LoH[$j]{id}><BR> <img src='../cards/$category_name/thumbnails/t-$LoH[$j]{file_name}' border=0></td>";
				if ($j eq 5) {$files.="</tr><tr>";}
				}

	   		&render($template);
	 }
	}
	
	if ($action eq ' delete ') {
	
   	&normal_type_header();
		$title="Deleting ecard";
		&html_head($title);
		$id=$q->param('ecards');
		&db_delete_row('ecards',$id);
		&redirect ('index.cgi?action=delete');		
	}
	if ($action eq 'upload') {
	
   	&normal_type_header();
		$title="Uploading ecard";
		&html_head($title);
		$fields{category}=$q->param('category_select');
		$fields{description}=$q->param('description');
		$picture=$q->param('picture');
		$upload=&image_upload($picture,'../cards/misc/test');
		print "$fields{category} <br>$fields{description}<br>$picture<br>";
		
	}
	
}
else {
	&normal_type_header();
	print "<h1>Connect to database $db_name at $db_host host failed !</h1>";
	}	


