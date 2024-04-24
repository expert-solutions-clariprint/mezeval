<?

if ($_GET["evaluation_id"])
{
	$evaluation = CEvaluation::objectWithid($_GET["evaluation_id"]);
	$_SESSION["evaluation"] = $evaluation;
} else if ($_SESSION["evaluation"]) $evaluation= $_SESSION["evaluation"]; 

$etablissement=$_SESSION["etablissement"];
$classe = $_SESSION["classe"];

$pdf = new CPdf('P', 'mm', 'A4', true); 

$pdf->init();
$pdf->afficheHeader = false;
$pdf->SetCreator("Mezeval");
$pdf->SetAuthor(CUser::user()->info());
$pdf->SetTitle("Résultats individuels de l’évaluation");
$pdf->SetSubject("Résultats individuels de l’évaluation");
$pdf->SetTopMargin(15.);
$pdf->setHeaderMargin(0);
$pdf->setFooterMargin(0);
$pdf->setPrintFooter(false);

$eleves = $classe->eleves();

function isostr($str) { return $str; }

$elements = $evaluation->elements();

$pdf->titre = $evaluation->intitule;

$pdf->AddPage();
$PAGE_TOP = $pdf->getY(); 
$REAL_MID_PAGE = ($pdf->getPageHeight() - 30.) / 2 + 15.;
$REAL_PAGE_HEIGHT = ($pdf->getPageHeight() - 15.);

$COMP_WIDTH = $pdf->printableWidth() - 50;

CDebug::LOG("PAGE_TOP :$PAGE_TOP ");
CDebug::LOG("PAGE_HEIGHT:  " . $pdf->getPageHeight());

$OBJECT_HEIGHT=0;
$LAST_TOP=15;
foreach($eleves as $eleve)
{
	CDebug::LOG("Y:".$pdf->getY());
	$OBJECT_HEIGHT = $pdf->getY() - $LAST_TOP;
	$reste = $REAL_PAGE_HEIGHT - $pdf->getY();
	CDebug::LOG("================ reste = $reste  ; hauteur = $OBJECT_HEIGHT ; page = $REAL_PAGE_HEIGHT");
	if ($reste < $OBJECT_HEIGHT)
	{
		CDebug::LOG("==========================" . $pdf->getY() . " > ". $REAL_MID_PAGE);
		$pdf->AddPage();
	}
	$LAST_TOP= $pdf->getY();
	
	$pdf->Cell(0,0,sprintf("%s %s",isostr($eleve->nom),isostr($eleve->prenom)),0,0,'L');
	$pdf->Cell(0,0,$classe->info(),0,1,'R');
	$pdf->Ln();
	$pdf->Cell(0,0,$evaluation->info(),0,0,'L');
	$pdf->Cell(0,0,$evaluation->date(),0,1,'R');
	$pdf->Ln();
	$pdf->SetFillColor(0,0,0,10);
	$pdf->Cell($COMP_WIDTH,0,"Compétences évaluées",1,0,'L',1);
	$pdf->Cell(50,0,"Évaluation",1,1,'C',1);
	$vals = array("a" => 0, "b" => 0, "c" => 0, "x" => 0);
	$pdf->SetFontSize(11);
	foreach($elements as $elem)
	{
		$pdf->Cell($COMP_WIDTH,0,$elem->info(),1,0,'L');
		$val = CEvaluationEleve::objectForEleveAndElement($eleve->id,$elem->id);
		$v = $elem->getEvaluation($val->valeur);
		$vals[$v] += 1;
//		CDebug::TRACE(0,"v => $v");
//		CDebug::TRACE(0,print_r($val,true));
		$txt="";
		$color="";
		switch($v)
		{
			case "a":
				$txt = $classe->a_code();
				$color = $classe->a_color();
				break;
			case "b":
				$txt = $classe->b_code();
				$color = $classe->b_color();
				break;
			case "c":
				$txt = $classe->c_code();
				$color = $classe->c_color();
				break;
			case "x":
				$txt = $classe->z_code();
				$color = $classe->z_color();
				break;
			default :
				$color = "#000000";
				$txt = "";
		}
//		$pdf->SetStringTextColor($color);
		$pdf->Cell(50,0,$txt,1,1,'C');
//		$pdf->setTextBlack($color);
	}
	$pdf->Ln();
	if ($_GET["histogramme"])
	{
		
	//	CDebug::TRACE(0,print_r($vals,true));
	//	CDebug::TRACE(0,print_r($classe->colorsArray(),true));
		CDebug::LOG("Y2:".$pdf->getY());
		$pdf->verticalGraph(120,
							30,
							array($vals["a"],$vals["b"],$vals["c"],$vals["x"] + $vals[""]),
							$classe->colorsArray(),
							$classe->titles());
	}
	CDebug::LOG("Y3:".$pdf->getY());
}

$pdf->Output($classe->info() . "_" . $evaluation->info() . "_resultats_individuels.pdf", "D");
// echo $pdf->Output("/tmp/" . $classe->info() . "_" . $evaluation->info() . "_resultats_individuels.pdf", "F");
