#!/usr/bin/perl

use strict;
use warnings;
use CGI qw/:standard/;
use CGI::Carp 'fatalsToBrowser';

# Global variables and functions

sub get_all_products();
sub render_products_form($);    # parameter to CGI object
sub add_product($);             # parameter is a hash with product information
sub delete_product($);		# paramater is a product_id
sub replace_product($$);	# paramater is a product_id and a ref to a product structure
sub save_products();
sub upload_all_products();
sub goto_location($);
sub check_file();

my $DEBUG = 0;

my %product_defaults = (
	product_url	=> "http://www.mini-box.com/",
	name 		=> "Product Name",
	description 	=> "Product Description",
	price		=> "Product Price",
	image_url	=> "http://www.mini-box.com/images/",
	category	=> "Computers > Parts"
);

my @products = ();

my $PRODUCTS_FILE = "mini_box.txt";
my $SCRIPT_LOCATION = "/cgi-bin/froogle.cgi";
my $FTP_ACCESS_FILE = "ftpaccess.txt";

#----------------------------------------------------------------------------------------------
sub check_file()
{
	my $line;
	if (! -e $PRODUCTS_FILE ) {
		open(PRODUCTS, ">$PRODUCTS_FILE") or die "File $PRODUCTS_FILE can't be initialised. Error was: $!";
		$line="product_url\tname\tdescription\tprice\timage_url\tcategory";
		print PRODUCTS "$line\n";
		close(PRODUCTS);
	}
}
#----------------------------------------------------------------------------------------------
sub get_all_products() 
{
	my $line;
	my $i = 0;
	my @data;
	my %product = ();
	
	@products = ();
	
	open(PRODUCTS, "$PRODUCTS_FILE") or warn "Cannot find $PRODUCTS_FILE. Error was: $!";
	while ($line = <PRODUCTS>) {
		next if ($line =~ /^#/);    # Comment 
		next if ($line =~ /^$/);    # Blank 
		next if ($line =~ /^\s*?$/);  # Spaces
			     
		@data = split('\t', $line);
		
		$product{product_url} = $data[0];
		$product{name} = $data[1];
		$product{description} = $data[2];
		$product{price} = $data[3];
		$product{image_url} = $data[4];
		$product{category} = $data[5];
		
		push(@products,  { %product });
		$i++;
	}
	close(PRODUCTS);
	
	if ($DEBUG > 1) {
		for ( my $i = 0; $products[$i]; $i++) {
			print "$i.";
			foreach my $key (keys %{ $products[$i] }) {
				print "$key = $products[$i]{$key}\n<br>";
			}
		}	
	}
}

#----------------------------------------------------------------------------------------------
sub render_products_form($)
{
	my @columns;
	my $q = shift;
	my $i;
	print $q->start_table({-border=>1, -cellspacing=>2, -cellpadding=>2, -width=>"100%", -class=>"listing"});
	print $q->Tr(
		td(b("Name")),
		td(b("Description")),
		td(b("Product URL")),
		td(b("Image URL")),
		td(b("Category")),
		td(b("Price")),
		td(b("Action")),
		td("&nbsp;"),
	);
	# at line 0 it's the header which is rendered above
	for ($i = 1; $products[$i]; $i++) {
		print $q->start_form;
		print $q->hidden('product_id', $i);
		print $q->Tr(
			td(textfield('name',$products[$i]{name})),
			td({-width=>200},textarea({-rows=>10,-name=>"description", -value=>"$products[$i]{description}"})),
			td(textfield('product_url',$products[$i]{product_url})),
			td(textfield('image_url',$products[$i]{image_url}),
				br, 
				img({-src=>"$products[$i]{image_url}", -width=>50, -height=>50})),
			td(textfield('category',$products[$i]{category})),
			td(textfield('price',$products[$i]{price})),
			td(submit('action','Save')),
			td(submit('action','Delete')),
		);		
		print $q->end_form;
	}
	print $q->start_form;
	print $q->Tr(
		td(textfield('name', $product_defaults{name})),
		td(textarea({-rows=>10,-name=>"description", -value=>"$product_defaults{description}"})),
		td(textfield('product_url', $product_defaults{product_url})),
		td(textfield('image_url', $product_defaults{image_url})),
		td(textfield('category', $product_defaults{category})),
		td(textfield('price', $product_defaults{price})),
		td(submit('action','Add')),
		td("&nbsp;"),
	);
	print $q->end_form;
	print $q->end_table;
	
	print $q->start_form;
	print $q->submit('action','Upload');
	print $q->end_form;
	
	
	
}

#----------------------------------------------------------------------------------------------
sub delete_product($)
{
	my $id = shift;
	splice(@products, $id, 1);
	
}

#----------------------------------------------------------------------------------------------
sub add_product($)
{
	my $prodref = shift;
	my %tmp_prod = ();	

	$tmp_prod{product_url} = $prodref->{product_url};
	$tmp_prod{name} = $prodref->{name};
	$tmp_prod{description} = $prodref->{description};
	$tmp_prod{price} = $prodref->{price};
	$tmp_prod{image_url} = $prodref->{image_url};
	$tmp_prod{category} = $prodref->{category};
	
	push (@products, { %tmp_prod });
}
#----------------------------------------------------------------------------------------------
sub replace_product($$)
{
	my $id = shift;
	my $prodref = shift;
	my $i;
	
	for ($i = 1; $products[$i]; $i++) {
		if ( $i == $id) {
			$products[$i]{product_url} = $prodref->{product_url};
			$products[$i]{name} = $prodref->{name};
			$products[$i]{description} = $prodref->{description};
			$products[$i]{price} = $prodref->{price};
			$products[$i]{image_url} = $prodref->{image_url};
			$products[$i]{category} = $prodref->{category};
		}
	}
}
#----------------------------------------------------------------------------------------------
sub save_products()
{
	my $i;
	my $line;
	open(PRODUCTS,">$PRODUCTS_FILE") or die "Cannot open $PRODUCTS_FILE. Error was: $!\n";
	for ($i = 0; $products[$i]; $i++) {
		$line="$products[$i]{product_url}\t$products[$i]{name}\t$products[$i]{description}\t$products[$i]{price}\t$products[$i]{image_url}\t$products[$i]{category}";
		#strip bad chars like \n from textareas
		$line=~s/\n//g;
		print PRODUCTS "$line\n";
	}
	close(PRODUCTS);
}
#----------------------------------------------------------------------------------------------
sub goto_location($)
{

	my $SCRIPT_LOCATION = shift;
	print "Location:$SCRIPT_LOCATION\n";
	print "Content-type:text/html\n\n";
	print qq {
		<HTML>
		<HEAD>
		<META HTTP-EQUIV="Refresh" Content=0;URL="$SCRIPT_LOCATION"
		</HEAD>
		</HTML>
	};
}

#----------------------------------------------------------------------------------------------

my $q = new CGI;
my $action;

check_file();
get_all_products();


$action = $q->param('action');

if ($action eq "Save") {
	my %product = ();
	my $id;
	
	$product{product_url} = $q->param('product_url');
	$product{name} = $q->param('name');
	$product{description} = $q->param('description');
	$product{price} = $q->param('price');
	$product{image_url} = $q->param('image_url');
	$product{category} = $q->param('category');
	
	$id = $q->param('product_id');
	
	replace_product($id, \%product);
	save_products();
	goto_location($SCRIPT_LOCATION);
}

if ($action eq "Delete") {
	my $id;
	
	$id = $q->param('product_id');
	
	delete_product($id);
	save_products();
	goto_location($SCRIPT_LOCATION);
}

if ($action eq "Add") {
	my %product = ();
	
	$product{product_url} = $q->param('product_url');
	$product{name} = $q->param('name');
	$product{description} = $q->param('description');
	$product{price} = $q->param('price');
	$product{image_url} = $q->param('image_url');
	$product{category} = $q->param('category');
	
	add_product(\%product);
	save_products();
	goto_location($SCRIPT_LOCATION);
}

if ($action eq "Upload") {
	$| = 1;
	print $q->header;
	print $q->start_html(-head=>meta({-http_equiv => 'expires', -content    => '0'}),
			-title=>'Froogle Products Manager',-style=>{'src'=>'/style.css'});
	
	print "Executing: ncftpput -d stdout -f $FTP_ACCESS_FILE . $PRODUCTS_FILE \n<br>";
	
	my $rez = system ("ncftpput -d stdout -f $FTP_ACCESS_FILE . $PRODUCTS_FILE ");
	
	print "<br><a href=\"$SCRIPT_LOCATION\">Goto Main Page</a>";
	sleep(200);
	goto_location($SCRIPT_LOCATION);
}

print $q->header;
print $q->start_html(-head=>meta({-http_equiv => 'expires', -content    => '0'}),
			-title=>'Froogle Products Manager',-style=>{'src'=>'/style.css'});
			
print $q->h3('Froggle Product Manager');
print $q->br;
print $q->br;
print $q->br;
render_products_form($q);

print $q->end_html;

