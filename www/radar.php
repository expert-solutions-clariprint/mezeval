<?
header("Content-type: image/png");

include("../config/config.inc");

class CChartSerie
{
	var $datas;
	var $color;
	var $title;
	
	function setDatasFromString($srt)
	{
		$this->datas = explode("|",$str);
	}
	function setColorFromString($srt)
	{
		$this->color  = explode("|",$str);
	}	
}

class  CChart
{
	var $mode;
	var $series;
	var $width = 200;
	var $height = 200;
	var $colnames;
	var $backcolor = "255,255,255";
	var $img=null;


	function CChart()
	{
		
	}
	function parserequest()
	{
		if ($_GET["width"]) $this->width = $_GET["width"];
		if ($_GET["height"]) $this->height = $_GET["height"];
		if ($_GET["colnames"]) $this->colnames = explode("|",$_GET["colnames"]);
		if ($_GET["backcolor"]) $this->colnames = explode("|",$_GET["colnames"]);
	}
	
	function parserSeries()
	{
		$this->series = array();
		foreach($_GET["serie"] as $k -> $v)
		{
			$x =  new CChartSerie();
			$x->setDatasFromString($v);
			if ($m = $_GET["serie_name"]) $x->name = $m[$k];
			if ($m = $_GET["serie_colors"]) $x->setColorFromString($m[$k]);
			$this->series[] = $x;
		}
	}
	
	function drawRadar()
	{
	
	}
}


$width = 400;
if ($_GET["width"]) $width = $_GET["width"];
$height = 400;
if ($_GET["height"]) $height = $_GET["height"];

$DATAS = array();

if ($_GET["serie"])
{
	foreach($_GET["serie"] as $r)
		$DATAS[] = explode("|",$r);
} else {
	$img = imagecreate($width,$height);
	$background_color = imagecolorallocate($img, 255, 255, 255);
	$text_color = imagecolorallocate($img, 0,0,0);
	imagettftext($img, 8, 0 , $width / 2, $height / 2 , $text_color,"Verdana",  "no data");
	imagepng($img);
	imagedestroy($img);
	die();
}


$img = imagecreate($width,$height);
$background_color = imagecolorallocate($img, 255, 255, 255);

$n_series = count($DATAS);

function set_color_from_string(&$img,$str)
{
	$x = explode(",",$str);
	return imagecolorallocate($img, $x[0], $x[1], $x[2]);
}
function utf8_substr($str,$from,$len)
{
# utf8 substr
# www.yeap.lv
	return preg_replace('#^(?:[\x00-\x7F]|[\xC0-\xFF][\x80-\xBF]+){0,'.$from.'}'. '((?:[\x00-\x7F]|[\xC0-\xFF][\x80-\xBF]+){0,'.$len.'}).*#s','$1',$str);
}

$n_datas = 3;

$colors=null;	
if ($x = $_GET["colors"])
{
		$colors = explode("|",$_GET["colors"]);
}
if ($colors) $n_datas = count($colors);

if ($_GET["mode"] == "percent_bar")
{
	$text_color = imagecolorallocate($img, 0,0,0);
	$colors = array();
	foreach(explode("|",$_GET["colors"]) as $str) $colors[] = set_color_from_string($img,$str);

	$series = $_GET["serie"];	
	$nseries = count($series);
	$serie_width = $width / $nseries;
	$left = 0;
	foreach($series as $k => $strvals)
	{
		$vals = explode("|",$strvals);
		$base = array_sum($vals);
		$top = 0;
		$i = 0;
		
		foreach($vals as $v)
		{
			$top = (1 - $v) * $height;
			$bottom = $height;
			imagefilledrectangle($img,$left,$top,$left + $serie_width - 2,$bottom,$colors[$i]);
			$i++;
		}
//		print_r($_GET["serie_name"]);
//		echo "<br>$k<br>";
		if (($names=$_GET["serie_name"]) && ($name = $names[$k]))
		{
			$val = $name;
			$angle = 90;
			$txt_bot = $height-4;
			$box = imagettfbbox(8.0,$angle,"Verdana",$val);
			$txt_left = $left + $serie_width / 2 - $box[2] / 2;
			
//			imagettftext($img, 8, 0 , $txt_left , $txt_bot, $text_color,"Verdana",  $val);
			imagettftext($img, 8, $angle , $left+ $serie_width / 2 , $height- 3, $text_color,"Verdana",  $val);
		}
		
		$left += $serie_width;
	}
} else {
	/* RADAR */
	$delta_angle = 2 * M_PI / $n_series;	
	$line_color = imagecolorallocate($img, 55, 55, 55);
	$angle = -M_PI / 2;	
	for($j=0; $j < $n_datas; $j++)
	{
		$color = null;
		if ($colors && $colors[$j])
		{
			$color= set_color_from_string($img,$colors[$j]);
		} else  $color = imagecolorallocate($img, 0,  255 * (1 - $j / $n_datas) , 255 * $j / $n_datas );
		$angle = -M_PI / 2;
		$points = array();
		for($i=0; $i< $n_series; $i++)
		{
			$val = $DATAS[$i][$j];
			if ($val == 0) $val = 0.01; 
			$points[] = $width / 2 + cos($angle) * $width * $val/ 2;
			$points[] = $height / 2  + sin($angle) * $height * $val / 2;
			$angle += $delta_angle;
		}
		@imagefilledpolygon($img, $points, $n_series, $color);
	}
	$angle = -M_PI / 2;
	$cols = null;
	
	if ($_GET["colnames"]) $cols = 		$cols =explode("|",$_GET["colnames"]);
	else $cols = $_GET["serie_name"];
	
	if ($cols)
	{
		$text_color = imagecolorallocate($img, 0,0,0);
		$fw = imagefontwidth(1) / 2;
		$fh = imagefontheight(1) / 2;
		foreach($cols as $colname)
		{
			imageline($img, $width / 2,$height / 2,
						 $width / 2 + cos($angle) * $width / 2 ,
						$height / 2  + sin($angle) * $height / 2  ,
						$line_color);
			// 180. * $angle / M_PI
			$val = utf8_substr($colname,0,10);
			$box = imagettfbbox(8.0,0,"Verdana",  $val);
			if ($_GET["debug"])
			{
				$top = $height / 2  +  sin($angle) * $height / 3 -  $fh;
				$left = $width / 2 + cos($angle) * $width / 3 - strlen($val) * $fw;
				echo "$val : top:$top left:top:$top<br>";
				print_r($b);
			}
			imagettftext($img, 8, 0 , $width / 2 + cos($angle) * $width / 3 - ($box[2] / 2),
			 $height / 2  +  sin($angle) * $height / 3 + ($box[7] / 2), $text_color,"Verdana",  $val);
			$angle += $delta_angle;
		}
	}
	else 
	{
		for($i=0; $i< $n_series; $i++)
		{
			imageline($img, $width / 2,$height / 2,
						 $width / 2 + cos($angle) * $width / 2 ,
						$height / 2  + sin($angle) * $height / 2  ,
						$line_color);
			$angle += $delta_angle;
		}
	}
}
imagepng($img);
imagedestroy($img);
