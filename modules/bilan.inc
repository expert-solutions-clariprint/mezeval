<?
$periode = CPeriode::objectWithId($_GET["bilan"]);
if (!$periode) {
	echo "erreur";
	return;
}

if ($_GET["output_mode"] == "pdf" )
{
	if ($bilan=CBilanProgression::objectForEleveAndPeriode($_GET["eleve"],$_GET["periode"]))
	{
		$pdf = $bilan->pdf();
		$pdf->Output("bilan.pdf", "D");
		die();
	}
}
// $module_classe = "bilans";
// include("classe_header.inc");
$ELEVES = $classe->eleves();

if (!$_GET["eleve"])
{
	$_GET["eleve"] = $ELEVES[0]->id;
}

if ($_GET["eleve"] && $_GET["bilan"])
{
	if ($bilan=CBilanProgression::objectForEleveAndPeriode($_GET["eleve"],$_GET["bilan"]))
	{
	} else
	{
		$bilan = new CBilanProgression();
		$bilan->eleve=$_GET["eleve"];
		$bilan->periode=$_GET["bilan"];
	}
	$_SESSION["bilan"] = $bilan;
} else if ($bilan = $_SESSION["bilan"]) {
	
} else die("error");

if ($_POST["update_bilan"] || $_POST["update_bilan_and_print"])
{
	if ($_POST["date_bilan"]) $bilan->date_bilan  = CDateUtil::unixDate($_POST["date_bilan"]);
	else $bilan->date_bilan = null;
	
	$bilan->description = $_POST["bilan_description"];
	$bilan->update();
	$bilan->deleteEvaluations();
	$pgs = $_POST["progressions"];
	if ($pgs)
	{
		foreach($pgs as $prog)
		{
			$elem = new CBilanProgressionElement();
			$elem->eleve = $bilan->eleve;
			$elem->periode = $bilan->periode;
			$elem->progression = $prog;
			$elem->commentaire = $_POST["progression_commentaire"][$prog];
			$elem->affichage = $_POST["progression_affichage"][$prog];
			$elem->evaluation = $classe->internalCode($_POST["progression_value"][$prog]);
			$elem->create();
		}
	}	
}
if ($_POST["update_bilan_and_print"]) 
{
	?>
	<script type="text/javascript" charset="utf-8">
		$(document).observe("dom:loaded", function()
		{

		document.location.href = "export.php?module=bilan&eleve=<?= $_GET["eleve"] ?>&periode=<?= $_GET["periode"] ?>&output_mode=pdf";
		});
		
//		alert(document.location.href + "&output_mode=pdf");
 	</script>
	
	<?
}
function affiche_bilan($progression)
{
	
}
global $PERIODE;
$PERIODE = $bilan->periode();
$bilan->loadElements();

function showProgrammeAsTable(&$bilan,&$stats,$classe_id,$subs=null,$deep=1)
{
	global $PERIODE;
	$classe = $_SESSION["classe"];
	$eleve = $_SESSION["eleve"];
	if ($subs == null) $subs = CProgression::parentsForClasse($classe_id);
	foreach($subs as $c) {
		if (CProgressionPeriode::existForProgressionAndPeriode($c->id,$bilan->periode))
		{
			$mysubs = CProgression::objectForParent($c->id);
			$hassub = (count($mysubs) > 0);
			$affiche = false;
			$eval = "";
			$com = "";
			$cssnoFolder= "";
			if (!$hassub) $cssnoFolder = " nofolder"; 
			if ($elem = $bilan->elements[$c->id])
			{
				$affiche = $elem->affichage;
				$eval = $classe->userCode($elem->evaluation);
				$com = $elem->commentaire;
			} else $affiche = 1;			
		?>
		
<tr class="niveau<?= $deep ?><?= $cssnoFolder ?>"
	<? if ($deep > 10) { ?>style="display: none" <? } ?>
	treeviewdeep="<?= $deep ?>"
	onmouseover="addCssClass(this,'highlight');"
	onmouseout="rmCssClass(this,'highlight');">
	<td class="code">
		<input type="hidden" name="progressions[]" value="<?= $c->id ?>">
		<input type="checkbox" name="progression_affichage[<?= $c->id ?>]" value="1" <? if ($affiche) echo "checked"; ?>></td>
	<td class="folderstatus"  <? if ($hassub) { ?>onclick="switchTreeView(event,this.parentNode)"<? } ?>></td>
	<td class="code"><?= $c->code ?></td>
	<td onclick="switchTreeView(event,this.parentNode)" style="cursor: pointer" class="intitule icon"><?= $c->intitule ?></td>
	<td class="modifier" style="text-align: right">
		<? if (!$hassub ) { ?><input autocomplete="off" class="eval" onkeyup="check_value(this,event)" type="text" size="2" maxlength="1" value="<?= $eval ?>" name="progression_value[<?= $c->id ?>]">
		<? } ?></td>
	<td><?
		if ($r = $stats->datas["c".$c->id])
		{
			if (!$hassub)
			 	$z = implode("",CEvaluationEleve::evolutionForProgression($c->id,0,$bilan->eleve,$PERIODE->debut,$PERIODE->fin));
			else $z = null;
			?><img <?
			if ($z)
			{
				printf(" src2='graph_abc.php?mode=HVALUES&values=%s&%s'",$z, $classe->url_colors()); 
				printf(" onclick=\"swapImageSrc(this,'src2')\"");
			}
			?> src="graph_abc.php?a=<?= $r["a"] ?>&b=<?= $r["b"] ?>&c=<?= $r["c"] ?>&z=<?= $r['x'] ?>&<?= $classe->url_colors() ?>"><?
		} ?></td>
		<td>
		<? if (!$hassub && $stats->datas["c".$c->id]) { ?>
		<div onclick="showProgessionEleve(this,<?= $c->id ?>,<?= $_GET["eleve"] ?>)" style="cursor: pointer"><img src="img/historique.gif"></div>
		<? } ?></td>
	<td class="modifier"><input type="text" size="20" value="<?= $com ?>" name="progression_commentaire[<?= $c->id ?>]"></td>
	<td class="code"><?= $c->code_socle ?></td>
</tr>
	<? if ($mysubs) showProgrammeAsTable($bilan,$stats,$classe_id,$mysubs,$deep + 1); ?>
<? 		}
 	}
}
?>
<div id="body" class="bilan">
	
	<script language="javascript">
		Element.prototype.firstAncestor = function(cssRule) 
			{
				return this.ancestors().detect(function(elem) { return elem.match(cssRule);});
			}
	
		function check_value(input,event)
		{
			var val = input.value.toLowerCase();
			var possibles = <?= json_encode($classe->codes()) ?>;
			if (possibles.indexOf(val) >= 0 || val == '')
			{	input.value = val; }
			else {
				
				alert("Attention seul les valeurs suivantes sont autorisées : " + possibles.toString());
				input.value = "";
				return;
			}
		}
		
		
		
		function afficherNiveau1()
		{
			var tb = $('tbody');
			for(var i =0; i < tb.rows.length; i++ )
			{
				var r = $(tb.rows[i]);
				if (r.hasClassName("niveau1")) r.show();
				else r.hide();
			}
		}
		function afficherNiveau2()
		{
			var tb = $('tbody');
			for(var i =0; i < tb.rows.length; i++ )
			{
				var r = $(tb.rows[i]);
				if (r.hasClassName("niveau2")) r.show();
				else r.hide();
			}
		}
		function afficherTous()
		{
			var tb = $('tbody');
			for(var i =0; i < tb.rows.length; i++ )
			{
				var r = $(tb.rows[i]);
				r.show();
			}
		}
		function afficherEditable()
		{
			var tb = $('tbody');
			for(var i =0; i < tb.rows.length; i++ )
			{
				var r = $(tb.rows[i]);
				if (r.hasClassName("nofolder")) r.show();
				else r.hide();
			}
		}
		
	</script>
	<h1>Bilan de progression</h1>
	<form method="post" id="form_bilan">
		<table class="informations" width="100%">
			<tr>
				<th>Élève : </th>
				<th>Date : </th>
			</tr>
			<tr>
				<td><select onchange="document.location='?module=bilans&mode=saisie_eleve&bilan=<?= $_GET["bilan"] ?>&eleve=' + this.value;">
					<? 
					foreach($classe->eleves() as $e)
					{
						if ($e->id == $bilan->eleve) $sel = "selected"; else $sel = "";
						printf("<option value='%s' %s > %s </option>", $e->id,$sel, $e->info());
					} ?>
					</select></td>
				<td><input type="text" name="date_bilan" size="10" value="<?= $bilan->date_bilan() ?>" class="XLDatePicker">
			</tr>
			<tr>
				<th colspan="3">Commentaire :</th>
			</tr>
			<tr>
				<td colspan="3"><textarea rows="5" name="bilan_description" style="width: 100%"><?= $bilan->description ?></textarea></td>
			</tr>
		</table>
		<table>
			<tbody>
				<tr>
					<td><input type="submit" name="update_bilan" value="Enregistrer les modifications"></td>
					<td><input type="submit" name="update_bilan_and_print" value="Enregistrer et imprimer">
				</tr>
			</tbody>
		</table>
		<h2>Détails :</h2>
		<?
		$stats = new CEvaluationStats($classe->id,null,null);
		$stats->setColorsFromClasse($classe);
		$stats->eleve_id = $bilan->eleve;
		$periode = $bilan->periode();
		$stats->date_debut= $periode->debut;
		$stats->date_fin= $periode->fin;
		$stats->build();
		$stats->stat_width = 100;
		?>
		<table class="bilans alternate liste treeview keyboardNavigate">
			<thead>
				<tr>
					<th colspan="4">Progression | 
						<a style="cursor: pointer" onclick="setCheckboxes($('form_bilan'),'progression_affichage*',true)">tout cocher</a> | 
						<a style="cursor: pointer" onclick="setCheckboxes($('form_bilan'),'progression_affichage*',false);">tout décocher</a> |
						<br>Afficher :
						<a style="cursor: pointer" onclick="afficherNiveau1()"> Synthèse 1 </a> |
						<a style="cursor: pointer" onclick="afficherNiveau2()"> Synthèse 2 </a> |
						<a style="cursor: pointer" onclick="afficherEditable()"> À saisir  </a> |
						<a style="cursor: pointer" onclick="afficherTous()"> Tout  </a> |
					</th>
					<th>Niveaux de compétences</th>
					<th colspan="2">Résultats évaluations</th>
					<th>Commentaire</th>
					<th>Socle</th>
			</thead>
			<tbody id="tbody">
				<? showProgrammeAsTable($bilan,$stats,$classe->id); ?>
			</tbody>
		</table>
		<input type="submit" name="update_bilan" value="Enregistrer les modifications">
	</form>
	<p class="notice">
	IMPORTANT : Les barres colorées sont affichées à titre indicatif, les bilans ne se remplissent pas automatiquement. Lors de l'édition des bilans, les histogrammes ainsi que les niveaux de compétence affichés ne sont générés qu'à partir des saisies de la colonne "Niveaux de compétence". Il est donc obligatoire de renseigner la colonne "Niveaux de compétences" avant de lancer une impression. (Mezeval fournit une aide à l'évaluation mais c'est bien l'enseignant, et non la machine, qui renseigne le bilan de l'élève).</p>

</div>
