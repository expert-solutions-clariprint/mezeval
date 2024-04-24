<?
//header("Content-type: image/png");


$width = 400;
if ($_GET["width"]) $width = $_GET["width"];
$height = 400;
if ($_GET["height"]) $height = $_GET["height"];

function set_color_from_string(&$img,$str)
{
	return imagecolorallocate($img, hexdec(substr($str,1,2)),hexdec(substr($str,3,2)),hexdec(substr($str,5,2)));		
}

$DATAS=$_GET["serie"];

$img = imagecreate($width,$height);
$background_color = imagecolorallocate($img, 255, 255, 255);

$colors = $_GET["colors"];
/*
if ($_GET["colors"])
{
	foreach($_GET["colors"] as $k => $c)
	{
		$colors[$k] = set_color_from_string($img,$c);
	}
}
*/
function heightForVal($val)
{
	switch ($val)
	{
		case 'a' : return 1;
		case 'b' : return 0.75;
		case 'c' : return 0.5;
	}
	return 0.25;
}
		
// print_r($DATAS);
$dwidth = $width / count($DATAS);
$x=0;
// $o = imagecolorallocate($img, 220, 210, 60);
foreach($DATAS as $d)
{
	$
	$color = set_color_from_string($img,$colors[$d]);
	$top = (1 - heightForVal($d)) *  $height;
//	$bot = $top + 0.25 * $height;
	$bot = $top + 0.5 * $height;
	imagefilledrectangle($img,$x,$top,($x + $dwidth - 2),$bot,$color);
	$x += $dwidth;
}

$o = imagecolorallocate($img,100,100,100);
imageline($img,0,$height / 2,$width,$height / 2,$o);
imageline($img,0,0,0,$height,$o);

imageline($img,$width - 1,$height / 2,$width - 4,$height / 2 - 3,$o);
imageline($img,$width - 1,$height / 2,$width - 4,$height / 2 + 3,$o);

imagepng($img);
imagedestroy($img);
