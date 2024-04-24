<?
header("Content-type: image/png");

include("../config/config.inc");

class CGraphPercent
{
	var $width = 100;
	var $height = 20;
	var $color = "#00FF00";
	var $value = 0;
	var $total = 100;
	var $border_color = "#004040";
	var $border_width = 1;

	var $fifty_color = "#A0A0A0";
	var $fifty_width = 1;
	
	var $bgcolor = "#F0F0F0";
	
	var $title = null;
	var $title_font = null;
	var $title_size = 8.0;
	var $title_color  = "#000000";

	var $img = null;
	
	function setColorFromString($str)
	{
		$r = hexdec(substr($str,-6,2));
		$v = hexdec(substr($str,-4,2));
		$b = hexdec(substr($str,-2,2));
		CDebug::TRACE(0,"color : $str -> ($r,$v,$b)");
		return imagecolorallocate($this->img, $r, $v, $b);
	}
	
	function parseGetParameters()
	{
		if ($_GET["value"]) $this->value = (int)$_GET["value"];
		if ($_GET["color"]) $this->color = $_GET["color"];
		if ($_GET["total"]) $this->total = (int)$_GET["total"];
		if ($_GET["width"]) $this->width = (int)$_GET["width"];
		if ($_GET["height"]) $this->height = (int)$_GET["height"];

		if ($_GET["border_color"]) $this->border_color = $_GET["border_color"];
		if ($_GET["border_width"]) $this->border_width = (int)$_GET["border_width"];

		if ($_GET["fifty_color"]) $this->fifty_color = $_GET["fifty_color"];
		if ($_GET["fifty_width"]) $this->border_width = (int)$_GET["fifty_width"];

		if ($_GET["title"]) $this->title = urldecode($_GET["title"]);
	}
	
	function init()
	{
		$this->title_font = FONTS_PATH . "/Verdana.ttf";
		$this->parseGetParameters();
		$this->initImage();
	}
	function initImage()
	{
		$this->img = & imagecreate($this->width,$this->height);
		$this->setColorFromString($this->bgcolor);
	}
	
	function drawBorders()
	{
		if ($this->border_width)
		{
			imagesetthickness($this->img,$this->border_width);
			$c = $this->setColorFromString($this->border_color);
			imagerectangle($this->img,$this->border_width / 2,$this->border_width / 2,$this->width - $this->border_width / 2,$this->height - $this->border_width / 2,$c);
		}
	}
	function drawRules()
	{
		if ($this->fifty_width)
		{
			imagesetthickness($this->img,$this->fifty_width);
			$c = $this->setColorFromString($this->fifty_color);
			imageline($this->img, $this->width / 2, 3 * $this->height / 4, $this->width / 2, $this->height,$c);
//			imageline($this->img, $this->width / 2, 0, $this->width / 2, $this->height / 4,$c);
		}
	}
	
	function drawTitle()
	{
		if ($txt = $this->title)
		{
			$box = imagettfbbox($this->title_size,0.0,$this->title_font,$txt);
			$c = $this->setColorFromString($this->title_color);
			imagettftext($this->img,
						$this->title_size,
						0.0,
						$this->width / 2 - $box[2] / 2, //  int $x,
						$this->height / 2 - $box[5] / 2, // int $y,
						$c, // int $color,
						$this->title_font, //string $fontfile,
						$txt); // string $text);
		}
	}
	function draw()
	{
		if ($this->total > 0)
		{
			$right = ($this->width * $this->value) / $this->total;
			$c = $this->setColorFromString($this->color);
			imagefilledrectangle($this->img,0,0,$right,$this->height,$c);
		}
		$this->drawRules();
		$this->drawBorders();
		$this->drawTitle();
	}
	
	function png()
	{
		imagepng($this->img);
	}
	
	function reset()
	{
		imagedestroy($this->img);
	}
}
$x = new CGraphPercent();
$x->init();
$x->draw();
$x->png();

