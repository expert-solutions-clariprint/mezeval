<?

if (($classe =  $_SESSION["classe"]) && $_GET["export_pdf"])
{
	require_once(APP_PATH . "/libs/tcpdf/tcpdf.php");
	$pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true); 
	$etablissement = $classe->etablissement();
	// set document information
	$pdf->SetCreator(PDF_CREATOR);
	$u = CUser::user();
	$pdf->SetAuthor($u->nom);
	if ($etablissement)
		$pdf->SetTitle($etablissement->nom . "/".  $classe->intitule);
	else $pdf->SetTitle($classe->intitule);
	$pdf->SetSubject("Lise des élèves");
	
	// set default header data
	if ($etablissement) $etab_nom = $etablissement->nom;
	else $etab_nom = "";
	$pdf->SetHeaderData("", 0, $classe->intitule,$etab_nom);
	
	// set header and footer fonts
	$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
	$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));
	
	//set margins
	$pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
	$pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
	$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);
	
	//set auto page breaks
//	$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);
	
	//set image scale factor
//	$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO); 
	
	//set some language-dependent strings
	$pdf->setLanguageArray($l); 
	
	//initialize document
	$pdf->AliasNbPages();
	
	// add a page
	$pdf->AddPage();
	
	// ---------------------------------------------------------
	
	// set font
	$pdf->SetFont("dejavusans", "BI", 16);
	
	// print a line using Cell()	
	$pdf->Cell(0,0,"Liste des élèves",1,1,'C');

	$pdf->Ln(20);

	$pdf->SetLineWidth(0.1);
	$pdf->SetFillColor(220,220,220);
	$pdf->SetFont("dejavusans", "B", 11);
	$pdf->Cell(70,0,"Nom",1,0,'L',1);
	$pdf->Cell(70,0,"Prénom",1,0,'L',1);
	$pdf->Cell(40,0,"Date naissance",1,1,'L',1);
	$pdf->SetFont("dejavusans", "", 11);
	foreach($classe->eleves() as $e)
	{
	$pdf->Cell(70,0,$e->nom,1,0,'L');
	$pdf->Cell(70,0,$e->prenom,1,0,'L');
	$pdf->Cell(40,0,$e->date_naissance(),1,1,'C');	
	}	
	// ---------------------------------------------------------
	
	//Close and output PDF document
	$pdf->Output($classe->intitule . "_liste_eleves.pdf", "D");
	die();

} else if (($classe = $_SESSION["classe"]) && $_GET["export_csv"])
{
	header("Content-type: text/csv");
	header('Content-Disposition: attachment; filename="eleves ' . $classe->intitule . '.csv"'); 
	printf("Nom Elève;Nom d'usage Elève;Prénom Elève;Date naissance;\r\n");	
	foreach($classe->eleves() as $e) printf("%s;;%s;%s;\r\n",$e->nom,$e->prenom,$e->date_naissance());
	die();
}
include("classe_header.inc");

if ($_GET["delete"]) CEleve::deleteObjectWithId($_GET["delete"]);
if ($_POST["am_ajout"]) {
	for($i = 0; $i < 10; $i++) {
		if ($_POST["am_nom"][$i] || $_POST["am_prenom"][$i])
		{
			$eleve = new CEleve();
			$eleve->nom = $_POST["am_nom"][$i];
			$eleve->prenom = $_POST["am_prenom"][$i];
			if ($_POST["am_date_naissance"][$i])
				$eleve->date_naissance = CDateUtil::unixDate($_POST["am_date_naissance"][$i]);
			$eleve->classe = $classe->id;
			$eleve->update();
		}
	}
}
if ($_POST["csv_import_form"] && $_POST["csv_import_line"] && $_POST["csv_import_champ"])
{
	$n_eleves_import = 0;
	foreach($_POST["csv_import_line"] as $l)
	{
		$arr = $_POST["csv_import_line_data"][$l];
		$eleve = new CEleve();
		foreach($_POST["csv_import_champ"] as $k => $f)
		{
			if ($f == "nom") $eleve->nom = $arr[$k];
			else if ($f == "prenom") $eleve->prenom = $arr[$k];
			else if ($f == "naissance") $eleve->date_naissance = CDateUtil::unixDate($arr[$k]);
		}
		if ($eleve->nom || $eleve->prenom)
		{
			$eleve->classe = $classe->id;
			$eleve->update();
			$n_eleves_import ++;
		}
		
	}
}
if ($_POST["import_base_eleve"] && $_FILES["import_base_eleve_fichier"]) {
	$f = $_FILES["import_base_eleve_fichier"];
	if (!$f["error"])
	{
		$IMPORT_LINES = array();
		$file_name = $f["tmp_name"];

		// check encoding

		$content = file_get_contents($file_name);
		if (strpos($content,';') > 0) $sep = ";"; else $sep = ",";
		if (!CProgramme::utf8_compliant($content))
			file_put_contents($file_name,iconv("ISO_8859-15","UTF-8",$content));
		$file_handle = fopen($file_name,"r");
		
		while ($line = fgetcsv($file_handle,0,$sep))
		{
			$IMPORT_LINES[] = $line;
		}
		fclose($file_handle);
	}
}

function comboChampsImport($i)
{
	?><select class="select_import_colonne" name="csv_import_champ[<?= $i ?>]">
		<option value="">Ne pas importer</option>
		<option value="nom">Nom</option>
		<option value="prenom">Prénom</option>
		<option value="naissance">Date naissance</option>
		</select><?
}

?>
<div id="body" class="eleves">
	<div>
		<? if ($_GET["base_eleve"]) { ?>
		<h1>Elèves : import de fichier</h1>
		<? if (!$IMPORT_LINES) { ?>
		<form method="post" enctype="multipart/form-data" action="?module=eleves&base_eleve=1">
			<div class="info">Remarque : Mezeval traite les fichiers issues d’un export CSV de base élève. Il ne tient compte que des noms, prénoms et date de naissance. Les autres informations sont ignorées et non conservées.</div>
			<table>
				<tr>
					<th>Choisir un fichier CSV (champs limités par des ";") :</th>
				<tr>
					<td>
						<input type="file" name="import_base_eleve_fichier"><input type="submit" value="Envoyer le fichier et choisir les données ..." name="import_base_eleve" >
			</table>
		</form><? } else { ?> 
		<h2>Choisir les lignes et les colonnes à importer :</h2>
		<form method="post" action="?module=eleves">
		<table>
			<thead>
				<tr>
					<td>
					<?
					$fl = $IMPORT_LINES[0];
					for($i= 0; $i< count($fl); $i++) {
						?><td><?
						comboChampsImport($i);
						?></td><?
					} ?>
					</tr>
			</thead>
			<tbody>
				<? 
				$li=0;
				foreach($IMPORT_LINES as $l) { ?>
				<tr>
					<td><input type="checkbox" value="<?= $li ?>" name="csv_import_line[]" <? if ($li > 0) echo "checked"; ?>></td>
					<? 
					$ci =0;
					foreach($l as $c) {  ?>
					<td><input type="text" value="<?= $c ?>" name="csv_import_line_data[<?= $li ?>][<?= $ci ?>]"/></td><?
						$ci++;
					} 
					?></tr><?
					$li++;
				} ?>
			</tbody>
		</table>
		<input type="submit" name="csv_import_form" value="Importer" onclick="return check_import_form();">
		<script type="text/javascript" charset="utf-8">
			function check_import_form()
			{
				var valide = false;
				$$('select.select_import_colonne').each(function(sel) {
					if (sel.value !="") valide = true;
				});
				if (valide == false)
					alert("Attention : au moins une colonne doit être sélectionnée !");
				return valide;
			}
		</script>
		</form>
		<? }} else if ($_GET["ajout_multiple"]) { ?>
		<h1>Elèves : ajouts multiples</h1>
		<form method="post" action="?module=eleves">
		<table class="alternate">
			<thead>
				<tr>
					<th>Nom</th>
					<th>Prénom</th>
					<th>Date Naissance</th>
				</tr>
			</thead>
		<? for($i = 0; $i < 10; $i++) { ?>
			<tr>			
				<td><input size="50" type="text" name="am_nom[<?= $i ?>]"></td>
				<td><input size="50" type="text" name="am_prenom[<?= $i ?>]"></td>
				<td><input size="10" type="text" name="am_date_naissance[<?= $i ?>]"></td>
			<tr><? } ?>
		</table>
		<input type="submit" name="am_ajout" value="ajouter">
		</form>
		<? } else { ?>
		<h1>Importer les élèves</h1>
		<ul class="control">
		<li><input type="button" onclick="location.href='?module=eleves&ajout_multiple=1'" value="Ajouts multiples" />
		<li><input type="button" onclick="location.href='?module=eleves&base_eleve=1'" value="importer CSV (Base élèves, Excel)" />
		<li><input type="button" onclick="location.href='export.php?module=eleves&export_csv=1'" value="export CSV" />
		<li><input type="button" onclick="location.href='export.php?module=eleves&export_pdf=1'" value="export PDF (impression)" />
		</ul>
		<table class="liste alternate">
			<thead>
				<tr>
					<th></th>
					<th style="width: 200px">Nom</th>
					<th style="width: 200px">Prénom</th>
					<th colspan="3"></th>
				</tr>
			</thead>
			<tbody>
				<?
				$eleves = CEleve::objectsForClasse($classe->id);
				if (count($eleves) > 0)
				{
					foreach(CEleve::objectsForClasse($classe->id) as $e) { ?>
				<tr onmouseover="addCssClass(this,'highlight')" onmouseout="rmCssClass(this,'highlight')">
					<td><img style="cursor: pointer" src="img/delete.png" onclick="if (confirm('Attention : la suppression de l’élève entrainera la suppression des toutes ses évaluations pour cette classe !')) location.href = '?module=eleves&delete=<?= $e->id ?>';"></td> 
					<td><?= $e->nom ?></td>
					<td><?= $e->prenom ?></td>
					<td class="action"><a href="?module=eleve&eleve_id=<?= $e->id ?>">détails</a>
				</tr><? }
				} else { ?>
				<tr><td colspan="4">Pour ajouter des élèves, vous pouvez les définir un par un en appuyant sur le bouton "Nouvel élève", utiliser l’ajout multiple en appuyant sur "Ajouts Multiples" ou importer un fichier CSV provenant de BaseElève (seuls les noms, prénoms et dates de naissance seront importés et conservés).   </td></tr><? } ?>
			</tbody>
		</table>
		
		<? } 
		 if (count($eleves) > 0) { ?>

		
		<table id="nav" class="control">
			<tr>
				<td id="back"><input type="button"  onclick="window.location='?module=classe'" value="<< Paramètres"></td>
				<td></td>
				<td id="next"><input type="button" onclick="window.location='?module=periodes'" value="Définir les périodes >>"></td>
			</tr>
		</table><? } ?>
	</div>
</div>
