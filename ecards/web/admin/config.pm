#!/usr/bin/perl

use Mysql;
use CGI;
use Image::Magick;

$q=new CGI;

$libs_dir="../libs";
$templates_dir="../templates";

$db_host="localhost";
$db_name="ecards";
$db_user="root";
$db_pass="";

$keywords = "ecards , postcards by email";
$style = "/css/ecards.css";
$bgcolor = "#9097c8";

require "$libs_dir/dblib.pm";
require "$libs_dir/htmlib.pm";
require "$libs_dir/template.pm";
require "$libs_dir/misc.pm";
require "$libs_dir/thumbnails.pm";
#require "$libs_dir/mail.pm";

