#!/usr/bin/perl

require "config.pm";

$action = $q->param('action');

if (!$action) {$action = 'make';}

if (&db_connect()) {

	if ($action eq 'make') {
		
		&normal_type_header();
		$title="Making Thumbnails for E-Cards";
		&html_head($title);
		$date = &get_long_date(time);
		$template = '../templates/thumbnails.tmpl';
		
		$sql = "select distinct category from ecards";
                my (@LoH_Category) = &db_select_fields('ecards',$sql);
                for ($i=0;$LoH_Category[$i];$i++){
	                $category_name=$LoH_Category[$i]{category};
			$sql = "select * from ecards where category='$category_name'";
			my (@LoH) = &db_select_fields('ecards',$sql);
			$files="<tr>";
			for ($j=0;$LoH[$j];$j++) {
			 	$thumbnails_dir="../cards/$category_name/thumbnails";
				$image_dir = "../cards/$category_name/";
				&make_thumbnail($image_dir,$LoH[$j]{file_name},$thumbnails_dir,chr(169)." ecards.ro");
				$files.="<td>$LoH[$j]{id}.<BR><img src='../cards/$category_name/thumbnails/t-$LoH[$j]{file_name}' border=0></td>";
				if ($j eq 5) {$files.="</tr><tr>";}
				}
	   		&render($template);
	   	}
	   }
	

}
else {
	&normal_type_header();
	print "<h1>Connect to database $db_name at $db_host host failed !</h1>";
	}	


