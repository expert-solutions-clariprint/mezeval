<?
if ($_GET["output_mode"] == "pdf")
{
	if ($bilan=CBilanSocle::objectForEleve($_GET["eleve"],$_GET["type"]))
	{
		$pdf = $bilan->pdf();
		$pdf->Output("bilan_socle.pdf", "D");
		die();
	}
}
$module_classe = "bilans";
include("classe_header.inc");

if ($_GET["eleve"])
{
	$type = "socle";
	if ($_GET["type"]) $type =$_GET["type"];
	if ($bilan=CBilanSocle::objectForEleve($_GET["eleve"],$type))
	{
	} else
	{
		$bilan = new CBilanSocle();
		if ($_GET["type"])
			$bilan->type=$_GET["type"];
		$bilan->eleve=$_GET["eleve"];
	}
	$_SESSION["bilan"] = $bilan;
} else if ($bilan = $_SESSION["bilan"]) {
} else die("error");

if ($_POST["update_bilan"])
{
	if ($_POST["date_bilan"]) $bilan->date_bilan  = CDateUtil::unixDate($_POST["date_bilan"]);
	else $bilan->date_bilan = null;	
	$bilan->commentaires = stripslashes($_POST["bilan_commentaires"]);
	$bilan->update();
	$bilan->deleteEvaluations();
	$pgs = $_POST["competences"];
	if ($pgs)
	{
		foreach($pgs as $prog)
		{
			$elem = new CBilanSocleElement();
			$elem->eleve = $bilan->eleve;
			$elem->type = $bilan->type;
			$elem->socle_ref = $prog;
			$elem->socle_intitule = $_POST["competence_intitule"][$prog];
			$elem->commentaire = $_POST["competence_commentaire"][$prog];
			$elem->affichage = $_POST["competence_affichage"][$prog];
			$elem->evaluation = $_POST["competence_value"][$prog];
			$elem->create();
		}
	}	
}
function affiche_bilan($progression)
{
	
}
$bilan->loadElements();
$SOCLE = $bilan->socle();

function showSocleAsTable(&$bilan,&$socle,&$stats,$competence = null,$deep=0)
{
	global $PERIODE;
	$classe = $_SESSION["classe"];
	if ($competence)
	{
		$subs = $competence->childNodes();
		$affiche = false;
		$eval = "";
		$com = "";
		if ($elem = $bilan->elementForKey($competence->code))
		{
			$affiche = $elem->affichage;
			$eval = $elem->evaluation;
			$com = $elem->commentaire;
		}
		?>
<tr class="niveau<?= $deep ?>" treeviewdeep="<?= $deep ?>"
	onmouseover="addCssClass(this,'highlight');" onmouseout="rmCssClass(this,'highlight');">
	<td class="code"><?= $competence->code ?><input type="hidden" name="competences[]" value="<?= $competence->code ?>"></td>
	<td onclick="switchTreeView(event,this.parentNode)" style="cursor: pointer" class="intitule"><?= $competence->intitule ?>
		<input type="hidden" name="competence_intitule[<?= $competence->code ?>]" value="<?= $competence->intitule ?>">
	</td>
	<td><?

		if (count($subs) == 0)
		{
			$a = CEvaluationEleve::countForEleveAndTypeSocleAndCodeSocleAndNiveau($bilan->eleve, $socle->type(),$competence->code,"a");
			$b = CEvaluationEleve::countForEleveAndTypeSocleAndCodeSocleAndNiveau($bilan->eleve, $socle->type(),$competence->code,"b");
			$c = CEvaluationEleve::countForEleveAndTypeSocleAndCodeSocleAndNiveau($bilan->eleve, $socle->type(),$competence->code,"c");
			$x = CEvaluationEleve::countForEleveAndTypeSocleAndCodeSocleAndNiveau($bilan->eleve, $socle->type(),$competence->code,"x");

			?><img src="graph_abc.php?a=<?= $a ?>&b=<?= $b ?>&c=<?= $c ?>&z=<?= $rx ?>&<?= $classe->url_colors() ?>"><?
		}  ?></td>
	<td class="modifier" style="text-align: center"><input type="checkbox" size="2" value="A" <? if ($eval == "A") echo "checked"; ?> name="competence_value[<?= $competence->code ?>]" ></td>
	<td class="modifier"><input type="text" size="20" value="<?= $com ?>" name="competence_commentaire[<?= $competence->code ?>]"></td>
</tr><? 
		
	} else $subs = $socle->childNodes(); 
	
	foreach($subs as $c) {
		showSocleAsTable($bilan,$socle,$stats,$c,$deep + 1);
		// showProgrammeAsTable($bilan,$stats,$classe_id,$c->id,$deep + 1);
 	}
}
?>
<div id="body" class="bilan">
	<h1>Bilan de compétences</h1>
	<form method="post" action="?module=bilansocle&type=<?= $_GET["type"]?>&eleve=<?= $_GET["eleve"] ?>" id="form_bilan">
		<table class="informations" width="100%">
			<tr>
				<th>Élève : </th>
				<th>Date : </th>
			</tr>
			<tr>
				<td><select onchange="document.location='?module=bilansocle&eleve=' + this.value;">
					<? 
					foreach($classe->eleves() as $e)
					{
						if ($e->id == $bilan->eleve) $sel = "selected"; else $sel = "";
						printf("<option value='%s' %s > %s </option>", $e->id,$sel, $e->info());
					} ?>
					</select></td>
				<td><input type="text" name="date_bilan" size="10" value="<?= $bilan->date_bilan() ?>">
			</tr>
			<tr>
				<th colspan="3">Commentaire :</th>
			</tr>
			<tr>
				<td colspan="3"><textarea rows="5" name="bilan_commentaires" style="width: 100%"><?= $bilan->commentaires ?></textarea></td>
			</tr>
		</table>
		<table width="100%">
			<tbody>
				<tr>
					<td>
						<input type="submit" name="update_bilan" value="Enregistrer les modifications">
					</td>
				</tr>
			</tbody>
		</table>
		<h2>Détails :</h2>
		<?
		$stats = new CEvaluationStats($classe->id,null,null);
		?>
		<table class="bilans liste">
			<thead>
				<tr>
					<th colspan="2">Compétences du socle commun</th>
					<th>Résultats bilans</th>
					<th>Acquis ?</th>
					<th>Commentaire</th>
			</thead>
			<tbody>
				<? 
					showSocleAsTable($bilan,$SOCLE,$stats);
				?>
			</tbody>
		</table>
		<input type="submit" name="update_bilan" value="Enregistrer les modifications">
	</form>
</div>
