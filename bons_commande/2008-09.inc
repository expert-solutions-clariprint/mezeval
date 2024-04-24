<?

if (!require_once(APP_PATH . '/libs/tcpdf/config/lang/eng.php')) die("TCPDF library needed");

if (!require_once(APP_PATH . '/libs/tcpdf/tcpdf.php')) die("TCPDF library needed");

$pdf = new TCPDF('P', 'mm', 'A4', true); 

// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor("Xavier");
$pdf->SetTitle("Evaluation");
$pdf->SetSubject("");
$pdf->SetKeywords("TCPDF, PDF, example, test, guide");

$pdf->SetHeaderData("", 0, "BON DE COMMANDE");

// set header and footer fonts
$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

//set margins
$pdf->SetMargins(PDF_MARGIN_LEFT, 25, PDF_MARGIN_RIGHT);
$pdf->SetHeaderMargin(6);
$pdf->SetFooterMargin(10);
$pdf->setPrintFooter(false);
//set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

//set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO); 

//set some language-dependent strings
$pdf->setLanguageArray($l); 

//initialize document
$pdf->AliasNbPages();

// set font
$pdf->SetFont("dejavusans", "", 11);

$pdf->AddPage();

$pdf->Cell(85,0,"Commanditaire",1,0,'L',1);
$pdf->Cell(0,0,"Fournisseur",1,1,'L',1);

$etab = $contrat->etablissement();
$txt = $etab->nom . "\n".
	$etab->contact_nom . "\n" .
	$etab->contact_adresse_1  . "\n" .
	$etab->contact_adresse_2  . "\n" .
	$etab->contact_codepostal . " " . $etab->ville;
$pdf->MultiCell(85,50,$txt,1,'L',0,0);

$pdf->MultiCell(0,50,"EXPERT SOLUTIONS SARL\n19 bis Rue Marc Tallavi\n33 310 LORMONT\n\nSIRET : 424 836 989 00022\nN.A.F. : 5829C\nTVA : FR43424836989\n\nTél : 05 56 74 26 90\nFax : 09 57 18 74 72",1,'L',0);

$pdf->SetFont("dejavusans", "BI", 13);
$pdf->Ln(10);

$pdf->Cell(0,0,"BON DE COMMANDE",0,1,'C');
$pdf->Ln(10);

$pdf->SetFont("dejavusans", "", 10);

$pdf->Cell(60,0,"N° de bon de commande",1,0,'C',1);
$pdf->Cell(40,0,"Date",1,0,'C',1);
$pdf->Cell(60,0,"Référence à rappeler",1,1,'C',1);

$pdf->Cell(60,0,"BC" . $contrat->id,1,0,'C');
$pdf->Cell(40,0,$contrat->created("%d/%m/%Y"),1,0,'C');
$pdf->Cell(60,0,"MEZEVAL-BC-". $contrat->id,1,1,'C');

$pdf->Ln(10);


$pdf->Cell(30,0,"Référence",1,0,'C',1);
$pdf->Cell(70,0,"Désignation",1,0,'C',1);
$pdf->Cell(25,0,"Nb. Classes",1,0,'C',1);
$pdf->Cell(25,0,"PU TTC",1,0,'C',1);
$pdf->Cell(28,0,"MONTANT TTC",1,1,'C',1);

$pdf->Cell(30,15,"MEZEVAL1AN",1,0,'C');
$pdf->MultiCell(70,15,"Location avec hébergement de Mezeval pour une année du ". $contrat->debut_validite() . " au ". $contrat->fin_validite(),1,'L',0,0);
$pdf->Cell(25,15,$contrat->nombre_classe,1,0,'C');
$pdf->Cell(25,15,"29,00",1,0,'R');
$pdf->Cell(28,15,$contrat->montant() . "€",1,1,'R');
$pdf->Cell(100 ,15,"",0,0,'R');
$pdf->Cell(50 ,15,"Dont TVA (19.6%) : ",1,0,'R');
$pdf->Cell(28 ,15,$contrat->tva(). "€",1,1,'R');

$pdf->Ln(10);
$pdf->Cell(0,0,"Conditions de réglement : réglement à reception de facture.",0,1,'L');
$pdf->Cell(0,0,"Conditions générales de locations à consulter sur le site web http://www.mezeval.fr.",0,1,'L');

$pdf->SetLeftMargin(80);
$pdf->Ln(10);
$pdf->MultiCell(115,70,"Pour le commanditaire : \nNom du signataire : ________________________________________\n\nSignature avec la mention 'bon pour accord':",1,'L',0,1);
$pdf->SetLeftMargin(PDF_MARGIN_LEFT);
$pdf->Ln(8);
/*
$pdf->Cell(5,0,"N° de bon de commande : BC" . $contrat->id,0,1,'L');
$pdf->Cell(0,0,"Date de commande : " . $contrat->created("%d/%m/%Y"),0,1,'L');
*/
$pdf->MultiCell(0,0,"Mezeval est un service de expert-solutions sarl au capital de 15200€, 19 bis rue marc tallavi, 33310 Lormont.",0,'C');
$pdf->Output("bon_de_commande.pdf", "I");

