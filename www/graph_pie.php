<?
// header("Content-type: image/png");

include("../config/config.inc");

require(LIB_PATH ."/pChart/pData.class");
require(LIB_PATH ."/pChart/pChart.class");

class CGraphPie
{
	var $width = 200;
	var $height = 200;
	var $a_color = "#00FF00";
	var $b_color = "#FF9010";
	var $c_color = "#FF2010";
	var $z_color = "#F0F0F0";

	var $a_value = 0;
	var $b_value = 0;
	var $c_value = 0;
	var $z_value = 0;

	var $border_color = "#004040";
	var $border_width = 1;

	var $fifty_color = "#A0A0A0";
	var $fifty_width = 1;
	
	var $bgcolor = "#F0F0F0";


	var $dataset = null;
	var $graph = null;
	
	var $chart_mode="PIE";
	
	function total()
	{
		return $this->a_value + $this->b_value + $this->c_value +$this->z_value;
	}
	
	function setColorFromString($str)
	{
		$r = hexdec(substr($str,-6,2));
		$v = hexdec(substr($str,-4,2));
		$b = hexdec(substr($str,-2,2));
		CDebug::TRACE(0,"color : $str -> ($r,$v,$b)");
		return imagecolorallocate($this->img, $r, $v, $b);
	}
	function setPaletteColorFromString($str,$index)
	{
		$r = hexdec(substr($str,-6,2));
		$v = hexdec(substr($str,-4,2));
		$b = hexdec(substr($str,-2,2));
		CDebug::TRACE(0,"color : $str -> ($r,$v,$b)");
		$this->graph->setColorPalette($index,$r,$v,$b);
	}

	
	function parseGetParameters()
	{
		if ($_GET["a"]) $this->a_value = (int)$_GET["a"];
		if ($_GET["a_color"]) $this->a_color = $_GET["a_color"];
		if ($_GET["b"]) $this->b_value = (int)$_GET["b"];
		if ($_GET["b_color"]) $this->b_color = $_GET["b_color"];
		if ($_GET["c"]) $this->c_value = (int)$_GET["c"];
		if ($_GET["c_color"]) $this->c_color = $_GET["c_color"];
		if ($_GET["z"]) $this->z_value = (int)$_GET["z"];
		if ($_GET["z_color"]) $this->z_color = $_GET["z_color"];

		if ($_GET["width"]) $this->width = (int)$_GET["width"];
		if ($_GET["height"]) $this->height = (int)$_GET["height"];

		if ($_GET["border_color"]) $this->border_color = $_GET["border_color"];
		if ($_GET["border_width"]) $this->border_width = (int)$_GET["border_width"];

		if ($_GET["chart_mode"]) $this->chart_mode = $_GET["chart_mode"];
	}
	
	function init()
	{
		$this->parseGetParameters();
		$this->initGraph();
	}
	function initGraph()
	{
		$this->graph = new pChart($this->width,$this->height);
		$this->graph->setFontProperties(FONTS_PATH  . "/Verdana.ttf",10);
		$this->dataset = new pData;
		$this->dataset->AddPoint(array("Acquis","En cours d'acquisition","Non Acquis","Non répondu"),"labels");
		$this->dataset->AddPoint(array($this->a_value,
										$this->b_value,
										$this->c_value,
										$this->z_value),
									"values");
		$this->dataset->AddSerie("values");
		$this->dataset->SetAbsciseLabelSerie("labels");
		$this->setPaletteColorFromString($this->a_color,0);
		$this->setPaletteColorFromString($this->b_color,1);
		$this->setPaletteColorFromString($this->c_color,2);
		$this->setPaletteColorFromString($this->z_color,3);
	}
	
	function drawBorders()
	{
	}
	function drawRules()
	{
	}
	function draw()
	{
		$this->drawRules();
		$this->drawBorders();
		CDebug::TRACE(0,"draw()");
		
		switch ($this->chart_mode)
		{
			case "BAR":
 				$this->graph->setGraphArea(2,2,$this->width,$this->height);
 				$this->graph->drawScale($this->dataset->GetData(),$this->dataset->GetDataDescription(),SCALE_ADDALL,213,217,221,TRUE,0,2,TRUE);
 				$this->graph->drawStackedBarGraph($this->dataset->GetData(),$this->dataset->GetDataDescription(),70);
				break;
			default:
				$this->graph->drawFlatPieGraph($this->dataset->GetData(),$this->dataset->GetDataDescription(),$this->width / 2,$this->height / 2,0.25 * $this->height,PIE_PERCENTAGE,10);
		}
		
	}
	
	function png()
	{
		$this->graph->Stroke();
	}
	
}
$x = new CGraphPie();
$x->init();
$x->draw();
$x->png();


