#Handle html templates ...

sub render(){

	local($template) = shift;
	%vars = @_;
	my $buffer;

	open TEMPLATE,$template or return "[template.pm -- template error : $_ ]";
        while (<TEMPLATE>){ $buffer .=$_;};
        close TEMPLATE;
        	

	local(@keys);
	local($key);
	local($params);
	local(@param);
	
	@keys= ($buffer =~ /<!!(.*?)!!>/g);


	foreach $key(@keys){

        	 if (defined($$key)){ $buffer =~ s/<!!$key!!>/$$key/g; }
	         elsif (defined($vars{$key})){ $buffer =~ s/<!!$key!!>/$vars{"$key"}/g; }
	         elsif (defined($template_buffer{$key})){ $buffer =~ s/<!!$key!!>/$template_buffer{"$key"}/g; }
        	 else { $buffer =~ s/<!!$key!!>/\&lt;none\&gt;/g; }
        }

	print $buffer;
}


1;

