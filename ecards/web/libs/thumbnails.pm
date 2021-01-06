#Make thumbnails from bigger images ... like images.altavista.com :) 
#Coded by PaNiC  

$frame_image="../cards/included/frame.gif";

sub make_thumbnail()
{

$image_dir=shift;
$image=shift;
$where=shift;
$text=shift;

$full_image_path=$image_dir.$image;

$model=Image::Magick->new;
$frame=Image::Magick->new;
$x=$model->ReadImage("$full_image_path");
warn "$x" if "$x";
$model->Sample(geometry=>'100x110>');

$temp_image=$model->Write("/tmp/tmp-$image");


$xbase=9;
$ybase=4;
($width,$height) = $model->Get('columns','rows');
$xdelta=int(100-$width)/2;
$ydelta=int(100-$height)/2;

$xbase+=$xdelta;
$ybase+=$ydelta;


$x=$frame->ReadImage("$frame_image");
warn "$x" if "$x";
$frame->Draw(primitive=>'image',points=>"$xbase,$ybase /tmp/tmp-$image");
$frame->Border(geometry=>'1x1',color=>'black');
$frame->Annotate(text=>"$text",geometry=>'+4+147',font=>'@tahomabd.ttf',pen=>'white',gravity=>'Center',pointsize=>12);
$x=$frame->Write("$where/t-$image");
warn "$x" if "$x";



}





1;
