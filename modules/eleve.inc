<?
$module_classe = "eleves";
include("classe_header.inc");

if ($_GET["eleve_id"])
{
	$eleve = CEleve::objectWithId($_GET["eleve_id"]);
	$_SESSION["eleve"] = $eleve;
} else if ($_GET["nouv_eleve"])
{
	$eleve = new CEleve();
	$_SESSION["eleve"] = $eleve;
} else if ($_SESSION["eleve"])
	$eleve = $_SESSION["eleve"];
	
if (!$eleve) trigger_error ("Erreur interne : eleve inconnu", E_USER_ERROR);

if ($_POST["maj_eleve"])
{
	$eleve->nom = $_POST["nom"];
	$eleve->prenom = $_POST["prenom"];
	$eleve->contact = $_POST["contact"];
	if ($_POST["date_naissance"])
		$eleve->date_naissance = CDateUtil::unixDate($_POST["date_naissance"]);
	else $eleve->date_naissance = null;
	if ($_POST["fin_scolarite"])
		$eleve->fin_scolarite = CDateUtil::unixDate($_POST["fin_scolarite"]);
	else $eleve->fin_scolarite = null;
	$eleve->classe = $classe->id;
	$eleve->update();
}

function creationMoyenne($classe_id,$etudiant_id,$parent = 0)
{
	if ($parent == 0)
		$subs = CCompetence::parentsForClasse($classe_id);
	else 
		$subs = CCompetence::objectForParent($parent);
	$res = array();
	foreach($subs as $c) { 
		
	}
}
global $MAX_PROGRAMME_DEEP;
$MAX_PROGRAMME_DEEP = 2;

function percent($v,$t)
{
	if ($t)
	{
		$p = 100 * $v / $t;
		return number_format($p,"1",",","") . "%";
	}
	return "-";
}


function showProgrammeAsTable(&$stats,&$periodes,$classe_id,$etudiant_id,$parent = 0,$deep=1)
{
	global $MAX_PROGRAMME_DEEP;
	$classe = $_SESSION["classe"];
	if ($MAX_PROGRAMME_DEEP < $deep) { $mode_visible="display: none";  }
	
	if ($subs = $stats->childs["c". $parent])
	{
	foreach($subs as $c) {
		$openFolder = "";
		$has_subs = $stats->childs["c". $c->id];
		if (!$has_subs) $nofolder=" nofolder";
		else {
			$nofolder="";
			if ($MAX_PROGRAMME_DEEP > $deep) { $openFolder =" openfolder"; }
		} 
		 ?>
<tr class="niveau<?= $deep ?><?= $nofolder ?><?= $openFolder ?>" treeviewdeep="<?= $deep ?>" onmouseover="style.backgroundColor='#F0F0F0';" style="<?= $mode_visible ?>" onmouseout="style.backgroundColor='#FFFFFF';">
	<td class="code"  onclick="switchTreeView(event,this.parentNode)"><?= $c->code ?></td>
	<td class="intitule icon"  onclick="switchTreeView(event,this.parentNode)"><?= $c->intitule ?></td><? 
	
	if ($periodes) 
	{
		foreach($periodes as $p)
		{ 
			$k = sprintf("p%s|c%s",$p->id,$c->id);
			$d = $stats->datas[$k];
				$n_a = $d['a'];
				$n_b = $d['b'];
				$n_c = $d['c'];
				$n_x = $d['x'];
			$total= $n_a + $n_b + $n_c  + $n_x;
			$values= null;
			if (!$has_subs)
			{
				$values = implode("",$c->evolutionEvaluationForEleve($etudiant_id,$p->debut,$p->fin));
				
			}
			if ($total == 0) { ?><td ></td><? }
			else {
		?>
		<td><img class="graph" <? if ($values) { ?>
			src2="graph_abc.php?mode=HVALUES&values=<?= urlencode($values) ?>&<?= $classe->url_colors() ?>"
			onclick="switchImage(this)" <? } ?>
				src="graph_abc.php?a=<?= $n_a ?>&b=<?= $n_b ?>&c=<?= $n_c ?>&z=<?= $n_x ?>&<?= $classe->url_colors() ?>&fity_bar_color=909090">
				</td>
	<? }}}
			$k = sprintf("c%s",$c->id);
			$d = $stats->datas[$k];
			$n_a = $d['a'];
			$n_b = $d['b'];
			$n_c = $d['c'];
			$n_x = $d['x'];
			$n_eval = $d['n'];
			$total= $n_a + $n_b + $n_c  + $n_x;
			if ($total == 0) { ?><td></td><? }
			else {
				if (!$has_subs)
				{
					$values = implode("",$c->evolutionEvaluationForEleve($etudiant_id,0,0));
				}
			?>
	<td><img class="graph" <? if ($values) { ?>
			src2="graph_abc.php?mode=HVALUES&values=<?= urlencode($values) ?>&<?= $classe->url_colors() ?>"
			onclick="switchImage(this)" <? } ?> src="graph_abc.php?text=<?=$n_eval ?>&a=<?= $n_a ?>&b=<?= $n_b ?>&c=<?= $n_c ?>&z=<?= $n_x ?>&<?= $classe->url_colors() ?>&fity_bar_color=909090">
	
	<td>
	<? if (!$has_subs) { ?><div onclick="showProgessionEleve(this,<?= $c->id ?>,<?= $etudiant_id ?>)" style="cursor: pointer">
		<img src="img/historique.gif"  ></div><? } ?></td><? } ?>
</tr>
	<? showProgrammeAsTable($stats,$periodes,$classe_id,$etudiant_id,$c->id,$deep + 1);
	}}}
?>
<style>
	img.stat_img_evolution
	{
		display: none;
	}
	iframe.presenter
	{
		position: fixed;
		width: 100%;
		height: 100%;
		margin-top: 0px;
		margin-bottom: 0px;
		margin-left: auto;
		margin-right: auto;
		margin-top: auto;
		top : 0px;
		background-color: white;
		border: red 0px solid;
		opacity: 0.9;
		-moz-opacity: 0.8;
		left: 0px;
		zIndex: 90;
	}
	div.presenter
	{
		position: fixed;
		border: 1px solid red;
		top: 10px;
		width: 500px;
		margin-left: auto;
		margin-right: auto;
		height: auto;
		zIndex: 100;
	}
</style>
<script language="javascript">
	function switchImage(im)
	{
		var i = im.src;
		im.src = im.getAttribute("src2");
		im.setAttribute("src2",i);
	}
	function afficheEvolution(eleve,periode)
	{
		var iframe = document.createElement('iframe');

		iframe.className="presenter";
		document.body.insert(iframe);
		var div = document.createElement('div');
		div.className="presenter";
		var img = document.createElement('img');
		img.src = ""
		div.insert('<h1>Bob</h1>');
		div.insert(img);
		document.body.insert(div);
	}
	
	function afficherCompetencesEvaluees(ok)
	{
		var b = $('body_resultats');
		for(var i = 0;  i < b.rows.length; i++)
		{
			var r = $(b.rows[i]);
			if (ok)
			{
				if (r.select('img.graph').length > 0)
					r.show();
				else r.hide();
			} else r.show();
		}
		b.fire("xl:updatecontent");
	}
	
</script>
<div id="body" class="eleve">
	élève : <select name="select_eleve" onchange="document.location='?module=eleve&eleve_id=' + this.value;">
			<? foreach($classe->eleves() as $e) { ?>
				<option value="<?= $e->id ?>" <? if ($eleve->id == $e->id) echo "selected"; ?>><?= $e->info() ?></option><? } ?>
			</select>
	<form method="post" action="?module=eleve">
	<table class="liste">
		<tbody>
			<tr>
				<td><label> nom : <br><input type="text" xl_validate="required" name="nom" value="<?= $eleve->nom ?>"></label><br>
				<label> prénom : <br><input type="text" xl_validate="required" name="prenom" value="<?= $eleve->prenom ?>"></label></td>
				<td><label> date de naissance :<br><input type="text" name="date_naissance" value="<?= $eleve->date_naissance() ?>"><?= $eleve->age() ?></label>
				<br><label>fin de scolarité :<br>
				<input type="text" name="fin_scolarite" value="<?= $eleve->fin_scolarite() ?>"></label></td>
				<td><label> remarques :<br>
				<textarea name="contact" rows="4" cols="30"><?= $eleve->contact ?></textarea></label></td>
				<td><input type=submit name="maj_eleve" onclick="return xl_validate_form(this.form)" value="Enregistrer"></td>

			</tr>
		</tbody>
	</table>
	</form><?
	if ($eleve->id) { 
		$periodes = $classe->periodes();
		$stats = new CEvaluationStats($classe->id,null,null);
		$stats->setColorsFromClasse($classe);
		$stats->eleve_id = $eleve->id;
		$stats->build();
		$stats->stat_width = 100;
		foreach($periodes as $p)
		{
			$stats->keyprefix = "p" . $p->id . "|";
			$stats->date_debut = $p->debut;
			$stats->date_fin = $p->fin;
			$stats->updatesData();
		} 
		?>
		<h1>Synthèse des résultats</h1>
		<p class="notice">En cliquant sur les diagrammes de résultat, un diagramme chronologique est présenté.</p>
		<input type="button" onclick="afficherCompetencesEvaluees(true)" value="Masquer les compétences non évaluées"> <input type="button" value="Tout afficher" onclick="afficherCompetencesEvaluees(false)">
		<table class="programme liste treeview alternate" width="100%">
			<thead>
				<tr>
					<th width="20px"></th>
					<th width="400px">Progressions</th>
				<? foreach($periodes as $p) { ?>
				<th ><?= $p->intitule() ?></th><? } ?>
					<th colspan="2">Global</th>
				</tr>
			</thead>
			<tbody id="body_resultats">
			<? 
				showProgrammeAsTable($stats,$periodes,$eleve->classe,$eleve->id);
			?>
			</tbody>
		</table><?
	} ?>
	<h1>Résultats récents</h1>
	<table class="liste">
		<thead>
			<tr>
				<td>Appréciation</td>
				<td>Évaluation</td>
				<td>Classeur</td>
				<td>Date</td>
				<td>Action</td>
			</tr>
		</thead>
		<tbody>
		<?
		if ($_GET["action"] == "supp_eval_comment")
		{
			CEvaluationEleveInfo::deleteWithEvaluationEleve($_GET["eval"],$eleve->id);
		}
		foreach(CEvaluationEleveInfo::objectsForEleve($eleve->id) as $eval)
		{
			$evaluation = $eval->evaluation();
			?>
			<tr>
				<td><?= $eval->description ?></td>
				<td style="cursor: pointer" onclick="document.location.href = '?module=evaluation&evaluation_id=<?= $eval->evaluation ?>'"><?= $evaluation->intitule ?></td>
				<td><?= $evaluation->classeur ?></td>
				<td><?= $evaluation->date() ?></td>
				<td class="action"><a href="?module=eleve&action=supp_eval_comment&eval=<?= $eval->evaluation ?>">Supprimer</a></td>
			</tr><?
		} ?>
		</tbody>
	</table>
</div>
