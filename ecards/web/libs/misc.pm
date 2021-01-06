#Misc Functions written by PaNiC
#ChangeLog:
# ver 0.0.1 - get_date and() get_long_date()
# ver 0.0.2 - image_upload

@LUNI=('Ian', 'Feb', 'Mar', 'Apr', 'Mai', 'Iun', 'Iul', 'Aug', 'Sep', 'Oct', 'Nov','Dec');
@LUNI_LONG=('Ianuarie', 'Februarie', 'Martie', 'Aprilie', 'Mai', 'Iunie', 'Iulie', 'August', 'Septembrie', 'Octombrie', 'Noiembrie','Decembrie');



sub get_date(){
	local($unix)=shift;
	local($min);
	local($ora);
	local($ziua);
	local($luna);
	local($an);


	(undef,$min,$ora,$ziua,$luna,$an)=localtime ($unix);
	local($data)="$ziua&nbsp;".$LUNI[$luna]."&nbsp;($ora:".sprintf("%02d",$min).")";

	return ($data);
}



sub get_long_date(){
	local($unix)=shift;
	local($min);
	local($ora);
	local($ziua);
	local($luna);
	local($an);


	(undef,$min,$ora,$ziua,$luna,$an)=localtime ($unix);
	$an += 1900;
	local($data)="$ziua&nbsp;".$LUNI_LONG[$luna]."&nbsp;$an&nbsp; ($ora:".sprintf("%02d",$min).")";

	return ($data);
}

sub image_upload{
	local($poza) = shift;
	local($dest) = shift;
	local($count)=0;
	
	 ($ext)= $poza =~ /\.(.*?)$/s;
	 open OUT_FILE, ">$dest.$ext";
	 while ($num = sysread($poza,$buf,1024)){
		 syswrite (OUT_FILE,$buf,$num) or die "Image Upload: $!";
		 $count +=$num;
 		}
	return ($count);
}

1;