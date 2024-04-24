<?

if ($_GET["evaluation_id"])
{
	$evaluation = CEvaluation::objectWithid($_GET["evaluation_id"]);
	$_SESSION["evaluation"] = $evaluation;
} else if ($_SESSION["evaluation"]) $evaluation= $_SESSION["evaluation"]; 

$etablissement=$_SESSION["etablissement"];
$classe = $_SESSION["classe"];

require_once(APP_PATH . "/libs/pdf/class.ezpdf.php");

$pdf = new Cezpdf('a4','portrait');

$pdf->ezSetMargins(50,70,50,50);
// put a line top and bottom on all the pages


$all = $pdf->openObject();
$pdf->saveState();
$pdf->setStrokeColor(0,0,0,1);
$pdf->line(20,40,578,40);
$pdf->line(20,822,578,822);
$pdf->addText(50,34,6,$etablissement->nom);
$pdf->addText(50,824,6,$etablissement->nom);
$pdf->addText(500,824,6,"acquismetre.fr");
$pdf->restoreState();
$pdf->closeObject();

// note that object can be told to appear on just odd or even pages by changing 'all' to 'odd'
// or 'even'.
$pdf->addObject($all,'all');


$eleves = $classe->eleves();

function isostr($str) { return iconv("UTF-8","ISO-8859-15",$str); };

foreach($eleves as $e)
{
	$pdf->newPage();
	$pdf->ezText(strftime("%d/%m/%Y"),10,array('justification' => 'right'));
	$pdf->ezText(sprintf("%s %s",isostr($e->nom),isostr($e->prenom)),10,array('justification' => 'left'));
	$pdf->ezText(sprintf("%s",isostr($evaluation->intitule)),10,array('justification' => 'center'));
	
}


$pdf->stream();
/*

?>
<div id="body" class="evaluation_notes">
	<script language="javascript">
	function setEvalValue(obj)
	{
		var par = obj.parentNode;
		var i=0;
		for(i=0; i < par.childNodes.length ; i++ )
		{
			par.childNodes[i].className = "";
		}
		obj.className = "selected";
		var input = document.getElementById(par.getAttribute('TARGET_ID'));
		if (input != null)
		{
			input.value = obj.getAttribute("VAL");
		}
	}
	</script>
		<h1><?= $evaluation->intitule() ?></h1>
		<?
		$mode_affichage=$_GET["mode_affichage"];
		$modes_affichage = array(
						"" => "Global : par critéres d’évaluation",
						"competences_gobal" => "Global : par compétences",
						"individuel" => "Individuel : par critéres d’évaluation",
						"competences_individuel" => "Individuel : par compétences");
		?>
		<form method="GET">
		<input type="hidden" name="module" value="evaluation_stats">
		Affichage : <select name="mode_affichage" onchange="form.submit()">
						<? foreach($modes_affichage as $k => $m) { ?>
							<option value="<?= $k ?>" <? if ($k == $mode_affichage) echo "selected"; ?>><?= $m ?></option><? } ?>
					</select>
		</form>
		<h2>Notes</h2>
		<?
		if ($mode_affichage== "competences_gobal")
		{
		} else if ($mode_affichage == "individuel")
		{ 
			$eleves = CEleve::objectsForClasse($classe->id); 
			$competences = $evaluation->competences();
			$mds = CEvaluation::modesCalcul();
			$notes = $evaluation->notesByEleveCompetence();
			foreach($eleves as $eleve) {
		?>
		<h3><?= $eleve->nom ?> <?= $eleve->prenom ?></h3>
		<table>
			<thead>
				<tr>
					<th>Compétence evaluée</th>
					<th>évaluation</th>
				</tr>
			</thead>
			<tbody><?
				foreach($competences as $c) {
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td><?= $c->description ?></td>
					<?
						$idref = $e->id . "-".  $c->id;
						$note = $notes[$idref];
//						echo $idref;
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $c->valeur_max;
						}
						$stat_width = 100;
						if ($c->mode == "STANDARD") {
							$v = CEvaluationEleve::valeurForEleveAndEvaluationCompetence($eleve->id,$evaluation->id,$c->id);
							switch ($v)
							{
								case "a":
									$bgcolor = "green";
									$txt = "Semble aquis (A)";
									break;
								case "b":
									$bgcolor = "blue";
									$txt = "A renforcer (B)";
									break;
								case "c":
									$bgcolor = "orange";
									$txt = "En début d’aquisition (C)";
									break;
								case "z":
									$bgcolor = "red";
									$txt = "Non acquis (Z)";
									break;
								case "x":
									$bgcolor = "grey";
									$txt = "Non évalué (X)";
									break;
							}
							
						?>
							<td title="<?= $txt ?>" style="width: 100px; background-color: <?= $bgcolor ?>;"><?= $v ?></td>
							<? }
						else {
							$v = CEvaluationEleve::valeurForEleveAndEvaluationCompetence($eleve->id,$evaluation->id,$c->id);
							?>
							<td colspan="5"><?= $v ?> / <?= $c->valeur_max ?></td><? } ?>
				</tr><? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table><hr><? }

		} else if ($mode_affichage == "competences_individuel")
		{
				
		
		
		} else
		{
		$eleves = CEleve::objectsForClasse($classe->id); 
		$competences = $evaluation->competences();
		$mds = CEvaluation::modesCalcul();
		$notes = $evaluation->notesByEleveCompetence();

		$tot_eleve = count($eleves);
		?>
		<table>
			<thead>
				<tr>
					<th>Critère evalué</th>
					<th>Semble aquis (A)</th>
					<th>A renforcer (B)</th>
					<th>En début d’aquisition (C)</th>
					<th>Non acquis (Z)</th>
					<th>Non évalué (X)</th>
				</tr>
			</thead>
			<tbody><?
				foreach($competences as $c) {
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td><?= $c->description ?></td>
					<?
						$idref = $e->id . "-".  $c->id;
						$note = $notes[$idref];
//						echo $idref;
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $c->valeur_max;
						}
						$stat_width = 100;
						if ($c->mode == "STANDARD") {
							$n_a = CEvaluationEleve::countForEvaluationCompetenceAndNiveau($evaluation->id,$c->id,'a');
							$n_b = CEvaluationEleve::countForEvaluationCompetenceAndNiveau($evaluation->id,$c->id,'b');
							$n_c = CEvaluationEleve::countForEvaluationCompetenceAndNiveau($evaluation->id,$c->id,'c');
							$n_z = CEvaluationEleve::countForEvaluationCompetenceAndNiveau($evaluation->id,$c->id,'z');
							$tot = $tot_eleve; // $x + $z + $c + $b + $a;
							$n_x = $tot - ($n_z + $n_c + $n_b + $n_a);
						?>
							<td title="Semble aquis"><?= $n_a ?></td>
							<td title="A renforcer"><?= $n_b ?></td>
							<td title="En début d’aquisition"><?= $n_c ?></td>
							<td title="Non acquis"><?= $n_z ?></td>
							<td title="Non évalué"><?= $n_x ?></td>
							<td><table style="empty-cells:show; width: <?= $stat_width ?>px; height: 20px;">
									<tr>
										<? if ($n_a > 0) { ?><td style="width: <?= $stat_width * $n_a / $tot ?>px; background-color: green;"></td><? } ?>
										<? if ($n_b > 0) { ?><td style="width: <?= $stat_width * $n_b / $tot ?>px; background-color: blue;"></td><? } ?>
										<? if ($n_c > 0) { ?><td style="width: <?= $stat_width * $n_c / $tot ?>px; background-color: orange;"></td><? } ?>
										<? if ($n_z > 0) { ?><td style="width: <?= $stat_width * $n_z / $tot ?>px; background-color: red;"></td><? } ?>
										<? if ($n_x > 0) { ?><td style="width: <?= $stat_width * $n_x / $tot ?>px; background-color: grey;"></td><? } ?>
									</tr>
								</table>
							</td>
							<? }
						else {
							$moy =  CEvaluationEleve::moyenneForEvaluationCompetence($evaluation->id,$c->id);
							$non_eval = CEvaluationEleve::countForEvaluationCompetenceAndNiveau($evaluation->id,$c->id,'');
							$tot_eval = $tot_eleve - $non_eval;
							$tot = $tot_eleve;
							$ok = ($moy / $c->valeur_max) * $tot_eval;
							$nok = $tot_eval - $ok;
							?>
							<td colspan="5"><?= $moy ?> / <?= $c->valeur_max ?></td>
							<td><table  style="empty-cells:show; width: <?= $stat_width ?>px; height: 20px;">
										<? if ($ok > 0) { ?><td style="width: <?= $stat_width * $ok / $tot ?>px; background-color: green;"></td><? } ?>
										<? if ($nok > 0) { ?><td style="width: <?= $stat_width * $nok / $tot ?>px; background-color: orange;"></td><? } ?>
										<? if ($non_eval > 0) { ?><td style="width: <?= $stat_width * $non_eval / $tot ?>px; background-color: grey;"></td><? } ?>
								</table><? } ?>
				</tr><? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table>
		<? } ?>
</div>
*/