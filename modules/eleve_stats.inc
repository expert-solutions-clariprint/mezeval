<?
include("classe_header.inc");

if ($_GET["eleve_id"])
{
	$eleve = CEleve::objectWithId($_GET["eleve_id"]);
} else if ($_SESSION["eleve"]) $eleve = $_SESSION["eleve"];
} else die("erreur");

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
function showProgrammeAsTable(&$stats,&$periodes,$classe_id,$etudiant_id,$parent = 0,$deep=1)
{
	if ($subs = $stats->childs["c". $parent])
	{
	foreach($subs as $c) { ?>
<tr class="niveau<?= $deep ?>" treeviewdeep="<?= $deep ?>" onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';" onclick="switchTreeView(event,this)">
	<td class="code"><?= $c->code ?></td>
	<td class="intitule"><?= $c->intitule ?></td><? 
	
	if ($periodes) 
	{
		foreach($periodes as $p)
		{ 
			$k = sprintf("p%s|c%s",$p->id,$c->id);
			$d = $stats->datas[$k];
				$n_a = $d['a'];
				$n_b = $d['b'];
				$n_c = $d['c'];
				$n_z = $d['z'];
				$n_x = $d[''];
			$evol = $stats->evolutions[$k];
		?>
	<td>cc<? $stats->showtable($n_a,$n_b,$n_c,$n_z,$n_x); ?>ds<? 
			print_r($evol); ?>
		</td><? }} ?>
	<td>e<? 
			$k = sprintf("c%s",$c->id);
			$d = $stats->datas[$k];
			$n_a = $d['a'];
			$n_b = $d['b'];
			$n_c = $d['c'];
			$n_z = $d['z'];
			$n_x = $d[''];
			$stats->showtable($n_a,$n_b,$n_c,$n_z,$n_x); ?></td>
</tr>
	<? showProgrammeAsTable($stats,$periodes,$classe_id,$etudiant_id,$c->id,$deep + 1); ?>
</tr><? }
}}

?>
<div id="body" class="eleve">
	<?
	if ($eleve->id) { ?>	
	<h1>Résultats Globaux</h1>
	<? $periodes = $classe->periodes();
	$stats = new CEvaluationStats($classe->id,null,null);
	$stats->setColorsFromClasse($classe);
	$stats->eleve_id = $eleve->id;
	$stats->build();
	$stats->buildEvolutions();


	foreach($periodes as $p)
	{
		$stats->keyprefix = "p" . $p->id . "|";
		$stats->date_debut = $p->debut;
		$stats->date_fin = $p->fin;
		$stats->updatesData();
	}
	$radar_competences = array();
	if ($stats->childs["c0"])
	{
		foreach($stats->childs["c0"] as $c)
		{
			foreach($stats->childs["c" . $c->id] as $s)
			{
				$radar_competences[] = $s;
			}
		}
	}
	?>
	<table>
		<tr>		
			<? foreach($periodes as $p) { ?>
			<th><?= $p->intitule() ?></th><? } ?>
			<th>Global</th>
		</tr>
		<tr>
			<?
			$datas = array();
			$colnames = "colnames=";
			if (count($periodes) > 0) $stats->stat_width = 300 / count($periodes);
			foreach($periodes as $p) {
				$url ="mode=radar";
				foreach($radar_competences as $c)
				{
					$k = sprintf("p%s|c%s",$p->id,$c->id);
					if ($d = $stats->datas[$k])
					{
						$url .= sprintf("&serie_name[%s]=%s",$c->id,urlencode($c->intitule));
						$tot = array_sum($d);
						$n_a = $d['a'];
						$n_b = $d['b'];
						$n_c = $d['c'];
						$n_x = $d[''] + $d['z'];
						$purl = sprintf("&serie[%s]=1|%s|%s|%s",$c->id,
																($n_a + $n_b + $n_c) / $tot,
																($n_a + $n_b ) / $tot,
																($n_a) / $tot);
						$url .= $purl;
					}
				}
				?><td>
					<img src="radar.php?width=200&height=200&colors=<?= $classe->decimalColorForEval('z') ?>|<?= $classe->decimalColorForEval('c') ?>|<?= $classe->decimalColorForEval('b') ?>|<?= $classe->decimalColorForEval('a') ?>|0,165,0&<?= $colnames ?>&<?= $url ?>"></td><?
			}
			foreach($radar_competences as $c) {
				$k = sprintf("c%s",$c->id);
				$url .= sprintf("&serie_name[%s]=%s",$c->id,urlencode($c->intitule));
				if ($d = $stats->datas[$k])
				{
					$tot = array_sum($d);
					$n_a = $d['a'];
					$n_b = $d['b'];
					$n_c = $d['c'];
					$n_z = $d['z'];
					$n_x = $d[''];
					$url .= sprintf("&serie[%s]=1|%s|%s|%s|%s",$c->id,($n_a + $n_b + $n_c+ $n_z) / $tot,
																		($n_a + $n_b + $n_c) / $tot,
																		($n_a + $n_b ) / $tot,
																		($n_a) / $tot);
				} else $url .= sprintf("&serie[%s]=0|0|0|0|0",$c->id);
				$datas[] = $stats->datas[$k];
			}
			?><td><img src="radar.php?width=250&height=250&colors=<?= $classe->decimalColorForEval('z') ?>|<?= $classe->decimalColorForEval('c') ?>|<?= $classe->decimalColorForEval('b') ?>|<?= $classe->decimalColorForEval('a') ?>&<?= $colnames ?>&<?= $url ?>"></td>
		</tr>
	</table>
	<h1>Détails résultats</h1>
	<table class="programme liste">
		<thead>
			<tr>
				<th colspan="2">Compétence</th>
			<? foreach($periodes as $p) { ?>
			<th><?= $p->intitule() ?></th><? } ?>
				<th>Gobal</th>
			</tr>
		</thead>
		<tbody>
		<? 
			showProgrammeAsTable($stats,$periodes,$eleve->classe,$eleve->id);
		?>
		</tbody>
	</table><? } ?>
</div>
