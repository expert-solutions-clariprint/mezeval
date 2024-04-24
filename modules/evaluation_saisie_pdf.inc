<?

if ($_GET["evaluation_id"])
{
	$evaluation = CEvaluation::objectWithid($_GET["evaluation_id"]);
	$_SESSION["evaluation"] = $evaluation;
} else if ($_SESSION["evaluation"]) $evaluation= $_SESSION["evaluation"]; 


$classe = $_SESSION["classe"];

// require_once(APP_PATH . "/libs/pdf/class.ezpdf.php");
if (!require_once(APP_PATH . '/libs/tcpdf/config/lang/eng.php')) die("TCPDF library needed");

if (!require_once(APP_PATH . '/libs/tcpdf/tcpdf.php')) die("TCPDF library needed");



class CSaisePdf extends TCPDF
{
	var $eleves;
	var $elements;
	var $largeur_colone_elements = 10;
	var $largeur_colone_eleve = 0;
}

$pdf = new CSaisePdf('P', 'mm', 'A4', true); 

$pdf->eleves = $classe->eleves();
$etablissement = $classe->etablissement();
$pdf->classe = $classe;
$pdf->evaluation = $evaluation;
$pdf->elements = $evaluation->elements();


// put a line top and bottom on all the pages
// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor("Xavier");

$pdf->SetTitle($evaluation->info());

$pdf->SetSubject("Fiche de saisie d’évaluation manuelle");

$pdf->SetKeywords("Mezeval,Fiche évaluation");

$pdf->SetHeaderData("", 0, $classe->info(),"Fiche de saisie d’évaluation");


// set header and footer fonts
$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

//set margins
$pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
$pdf->SetHeaderMargin(6);
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

//set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

//set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO); 

//set some language-dependent strings
$pdf->setLanguageArray($l); 

//initialize document
$pdf->AliasNbPages();

// set font
$pdf->SetFont("dejavusans", "BI", 11);


function isostr($str) { return $str; } 
// iconv("UTF-8","ISO-8859-15",$str); };


$pdf->SetFillColor(0,0,0,false,10);

$pdf->SetFontSize(11);
function print_header($pdf)
{
	$pdf->SetFillColor(0,0,0,10);
	$pdf->AddPage(); 
	$o = $pdf->Cell(0,0,strftime("%d/%m/%Y"),0,1,'R');
	$pdf->Cell(0,0,sprintf("%s",isostr($pdf->evaluation->intitule)),0,1,'C',1);
	
	// Calcul de la hauteur des textes
	$pdf->SetFontSize(9);
	$max_height=0;
	foreach($pdf->elements as $elem)
	{
		$w = $pdf->GetStringWidth($elem->info());
		$max_height = max($w,$max_height);
	}
	$max_height = 1  * $max_height; 
	$pdf->SetFont("dejavusans", "", 11);
	
	$pdf->Ln($max_height);
	$pdf->Cell($pdf->largeur_nom_eleve,0,isostr("Élèves"),1,0,'L',1);
	foreach($pdf->elements as $elem)
	{
		$txt = $elem->info();
		$cell = $pdf->Cell($pdf->largeur_colone_elements,0,"",0,0,'L',0);
		$x = $pdf->GetX();
		$y = $pdf->GetY();
		$pdf->SetFontSize(10);
		$w = $pdf->GetStringWidth($txt);
		$pdf->StartTransform();
		$pdf->Rotate(60,$x,$y);
		$pdf->Text($x - 5,$y ,$txt);
		$pdf->StopTransform();
		$pdf->SetFontSize(11);
		
	}	
	$pdf->Ln();
}
$pdf->largeur_nom_eleve = 60;
print_header($pdf);

$pdf->SetFillColor(0,0,0,5);
$b=1;
foreach($pdf->eleves as $eleve)
{
	$b=!$b;
	$pdf->Cell($pdf->largeur_nom_eleve,0,sprintf("%s %s",isostr($eleve->nom),isostr($eleve->prenom)),1,0,'L',$b);
	foreach($pdf->elements as $elem)
	{
		$pdf->Cell($pdf->largeur_colone_elements,0," ",1,0,'L',$b);
	}	
	$pdf->Ln();
}

$pdf->Output("saise_globale_evaluation.pdf", "D");
