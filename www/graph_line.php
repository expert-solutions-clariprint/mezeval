<?
//header("Content-type: image/png");

require(LIB_PATH ."/pChart/pData.class");
require(LIB_PATH ."/pChart/pChart.class");

class CGraphLine
{
	var $width = 500;
	var $height = 400;
	var $a_color = "#00FF00";
	var $b_color = "#FF9010";
	var $c_color = "#FF2010";
	var $z_color = "#F0F0F0";

	var $values = null;

	var $fifty_color = "#A0A0A0";
	var $fifty_width = 1;

	var $dataset = null;
	var $graph = null;
		
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
		if ($_GET["value"]) $this->values = $_GET["value"];

		if ($_GET["a_color"]) $this->a_color = $_GET["a_color"];
		if ($_GET["b_color"]) $this->b_color = $_GET["b_color"];
		if ($_GET["c_color"]) $this->c_color = $_GET["c_color"];
		if ($_GET["z_color"]) $this->z_color = $_GET["z_color"];

		if ($_GET["width"]) $this->width = (int)$_GET["width"];
		if ($_GET["height"]) $this->height = (int)$_GET["height"];
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


		
/*		

		$this->dataset->AddPoint(array("Acquis","En cours d'acquisition","Non Acquis","Non évalué"),"labels");
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
		*/
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
		$this->graph->drawFlatPieGraph($this->dataset->GetData(),$this->dataset->GetDataDescription(),$this->width / 2,$this->height / 2,0.25 * $this->height,PIE_PERCENTAGE,10);
	}
	
	function png()
	{
		$this->graph->Stroke();
	}
	
}

if ($_GET["evolution"] && $_GET["progression"] && $_GET["eleve"])
{
	if ($p_id = $_GET["periode"])
	{
		$periode = CPeriode::objectWithId($p_id);
		$date_debut_ts = $periode->debut;
		$date_fin_ts = $periode->fin;
	}
	$datas = CEvaluationEleve::evolutionForProgression($_GET["progression"],0,$_GET["eleve"],$date_debut_ts,$date_fin_ts);
	$r = array();
	foreach($datas as $res)
	{
		switch ($res->valeur)
		{
			case a 
			
		}
		
		if ($val =  )
		$datas 
	}

/* $DataSet->AddPoint(array(60,70,90,110,100,90),"Serie1");
 $DataSet->AddPoint(array(40,50,60,80,70,60),"Serie2");
 $DataSet->AddPoint(array("Jan","Feb","Mar","Apr","May","Jun"),"Serie3");
 $DataSet->AddSerie("Serie1");
 $DataSet->AddSerie("Serie2");
 $DataSet->SetAbsciseLabelSerie("Serie3");
 $DataSet->SetSerieName("Company A","Serie1");
 $DataSet->SetSerieName("Company B","Serie2");
 $DataSet->SetYAxisName("Product sales");
 $DataSet->SetYAxisUnit("k"); */

	
}

$x = new CGraphPie();
$x->init();
$x->draw();
$x->png();

