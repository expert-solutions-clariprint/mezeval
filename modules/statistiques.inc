<?
include("classe_header.inc");
$PERIODES = $classe->periodes();
?>
<script language="javascript">
	function presentDetails(progression_id,periode_id)
	{
//		alert('export.php?module=statistiques_details&progression='+ id);
		window.open('index.php?no_body_header=1&module=statistiques_details&progression='+ progression_id +'&periode=' + periode_id,null,"menubar=off;statusbar=off");
//		displayDialog('export.php?module=statistiques_details&progression='+ id,200,200);
	}
	
</script>
<div id="body" class="competences">
	<h1>Statistiques</h1>
	<?
	$mode_affichage=$_GET["mode_affichage"];
	if (!$mode_affichage) $mode_affichage = "competences_gobal";
	$modes_affichage = array(
					"competences_gobal" => "Global",
					"competences_individuel" => "Individuel");


	$mode_referentiel=$_GET["mode_referentiel"];
	
	$modes_referentiel = array(
					"referentiel_socle" => "Socle",
					"referentiel_socle_b2i" => "B2i",
					"referentiel_socle_langue" => "Langue",
					"referentiel_progressions" => "Progressions");
	if (!$mode_referentiel) $mode_referentiel = "referentiel_progressions";

	$mode_periode = $_GET["mode_periode"];
	if (!$mode_periode) $mode_periode = array('a' => 1);
	?>
	
	<form method="GET">
	<input type="hidden" name="module" value="statistiques">
	<table class="control">
		<thead>
			<tr>
				<th colspan="4">Options :</th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<th>Élèves</th>
				<th>Périodes d’affichage :</th>
				<th>Options</th>
			<tr>
				
				<td style="vertical-align: top">
					<? foreach($modes_affichage as $k => $m) { ?>
						<input type="radio" name="mode_affichage" value="<?= $k ?>" <? if ($k == $mode_affichage) echo "checked"; ?>><?= $m ?><br><? } ?>
				</td>
				<td style="vertical-align: top" rowspan="3">
					<? foreach($classe->periodes() as $p) { ?>
					<input type="checkbox" value="<?= $p->id ?>" name="mode_periode[<?= $p->id ?>]" <? if ($mode_periode[$p->id]) echo "checked"; ?>><?= $p->intitule ?> : du <?= $p->debut() ?> au <?= $p->fin() ?><br><? } ?>
					<input type="checkbox" value="-1" name="mode_periode[p]" <? if ($mode_periode['p']) echo "checked"; ?>> Personnalisées du <input type="text" size="10" name="periode_perso_debut"> au <input type="text" size="10" name="periode_perso_fin"><br>
					<input type="checkbox" value="-1" name="mode_periode[a]" <? if ($mode_periode['a']) echo "checked"; ?>> Année
				</td>
				<td>
					
				<td>
					<input type="submit" name="" value="Afficher">
				</td>
			</tr>
			<tr>
				<th>Référentiel :</th>
			</tr>
			<tr>
				<td style="vertical-align: top" colspan="3">
					<? foreach($modes_referentiel as $k => $m) { ?>
						<input type="radio" name="mode_referentiel" value="<?= $k ?>" <? if ($k == $mode_referentiel) echo "checked"; ?>><?= $m ?><br><? } ?>
								<br>Niveau de détail : <input type="text" name="niveau_detail"  value="<? if ($_GET["niveau_detail"]) echo $_GET["niveau_detail"]; else echo 2; ?>" style="width: 20px;" ><br>
						<input type="checkbox" name="masquer_non_evalue" value="1" <? if ($_GET["masquer_non_evalue"]) echo "checked"; ?>> Masquer les éléments non évalués<br>
						<input type="checkbox" name="masquer_niveau_un" value="1" <? if ($_GET["masquer_niveau_un"]) echo "checked"; ?>> Masquer les stat du premier niveau<br>
                        <? /*
                        <input type="button" name="hide_level_1" value="Masquer le niveau 1" id="hide_level_1" onclick="$('table_stat').select('.niveau1').each(function(row){ row.down('img.stat_img').hide(); }) "/> */ ?>
				</td>
			</tr>
		</tbody>
	</table>
	</form>
	<? 
	$periodes = array();
	if ($_GET["mode_periode"])
	{
		foreach($_GET["mode_periode"] as $k => $v)
		{
			if ($v > 0)
			{
				$periodes[] = CPeriode::objectWithId($v);
			} else if ($k == "p") {
				$pp = new CPeriode();
				$pp->intitule="Perso.";
				$periodes[] =  $pp;
			} else if ($k == "a") {
				$pp = new CPeriode();
				$pp->intitule="Annuel";
				$periodes[] =  $pp;
			}
		}
	}
	if ($mode_affichage == "competences_gobal")
	{
		$stats = new CEvaluationStats($classe->id);
		$stats->presentDetails = 1;
		
		$stats->periodes = $periodes;
		$stats->affichage_masquer_non_evalue = $_GET["masquer_non_evalue"];
		$stats->setColorsFromClasse($classe);
		if (count($periodes) > 0) $stats->stat_width = max(400 / count($periodes),50);
		?>
	<table class="programme alternate treeview" id="table_stat" style="width: 100%">
		<thead>
			<tr>
				<th><img src="/img/l16.gif"></th>
				<th>code</th>
				<th>Compétences <?= $mode_referentiel ?></th>
				<? if ($mode_referentiel == "referentiel_progressions") { ?>
					<th>socle</th>
					<th>B2i</th>
					<th>Langue</th>
					<? } ?>
				<? 
				foreach($periodes as $p) { ?>
				<th><?= $p->info() ?></th><? } ?>
		</thead>
		<tbody>
			<? 
			if ($_GET["niveau_detail"] > 0) $stats->deep2hide = $_GET["niveau_detail"];
			if ($mode_referentiel == "referentiel_socle_b2i")
			{
				$socle = $classe->b2i();
				if ($socle) $stats->showStatsForSocle($socle);
			} else if ($mode_referentiel == "referentiel_socle_langue")
			{
				$socle = $classe->langue();
				if ($socle) $stats->showStatsForSocle($socle);
			} else if ($mode_referentiel == "referentiel_socle")
			{
				$socle = $classe->socle();
				if ($socle) $stats->showStatsForSocle($socle);
			} else {
				$stats->build();
				foreach($periodes as $p)
				{
					$stats->keyprefix = "p" . $p->id . "|";
					$stats->date_debut = $p->debut;
					$stats->date_fin = $p->fin;
					$stats->updatesData();
				}
				$stats->keyprefix= "";
				$stats->affichage_lien_historique = true;
				$stats->showStatsAsTable($classe->id);
			} ?>
		</tbody>
		<tfoot>
		</tfoot>
	</table> 
	<? } else if ($mode_affichage == "competences_individuel")
	{ 
		$eleves = $classe->eleves();
		$stats = new CEvaluationStats($classe->id);
		$stats->affichage_masquer_non_evalue = $_GET["masquer_non_evalue"];
		$stats->affichage_lien_historique=true;
		$stats->presentDetails = 1;
		if ($_GET["niveau_detail"] > 0) $stats->deep2hide = $_GET["niveau_detail"];
		$stats->periodes = $periodes;
		$stats->setColorsFromClasse($classe);
		if (count($periodes) > 0) $stats->stat_width = max(400 / count($periodes),50);
		if ($mode_referentiel == "referentiel_socle")
		{
			$socle = $classe->socle();
		}
		else {
			$stats->mode_eleve = 1;
			$stats->build();
			foreach($periodes as $p)
			{
				$stats->keyprefix = "p" . $p->id . "|";
				$stats->date_debut = $p->debut;
				$stats->date_fin = $p->fin;
				$stats->updatesData();
			} 
		}
			$stats->deep2hide = 2;
		foreach($eleves as $eleve) {
			$stats->keyprefix = "e" . $eleve->id . "|";
			$stats->eleve_id = $eleve->id;
		?>
		<hr>
	<h2><?= $eleve->nom ?> <?= $eleve->prenom ?></h2>
	<table class="programme treeview alternate" style="width: 100%">
		<thead>
			<tr>
				<th><img src="/img/l16.gif"></th>
				<th>code</th>
				<th>Compétences</th>
				<? if ($mode_referentiel == "referentiel_progressions") { ?>
					<th>socle</th>
					<th>B2i</th>
					<th>Langue</th>
				<? }
				foreach($periodes as $p) { ?>
				<th><?= $p->info() ?></th><? } ?>
		</thead>
		<tbody>
			<? 
			if ($mode_referentiel == "referentiel_socle_b2i")
			{
				$socle = $classe->b2i();
				if ($socle) $stats->showStatsForSocle($socle);
			} else if ($mode_referentiel == "referentiel_socle_langue")
			{
				$socle = $classe->langue();
				if ($socle) $stats->showStatsForSocle($socle);
			} else if ($mode_referentiel == "referentiel_socle")
			{
				$socle = $classe->socle();
				if ($socle) $stats->showStatsForSocle($socle);
			} else 
				$stats->showStatsAsTable($classe->id); ?>
		</tbody>
		<tfoot>
		</tfoot>
	</table>
	<? }} ?>
</div>
