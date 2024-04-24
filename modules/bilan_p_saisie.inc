<?
$periode = CPeriode::objectWithId($_GET["bilan"]);
if (!$periode) {
	echo "erreur";
	return;
}
if(!$classe)
	$classe= $_SESSION["classe"];

if ($_GET["progression"])
{
	$progression = CProgression::objectWithId($_GET["progression"]);
	?>
	<h1><?= $progression->info() ?></h1>
	<form method="post">
		<input type="hidden" name="progression" value="<?= $progression->id ?>" id="progression">
		<table class="alternate liste">
			<thead>
				<tr>
					<th>Nom</th>
					<th>Prénom</th>
					<th>Age</th>
					<th>Résultats</th>
					<th>Evaluations</th>
					<th>Commentaire</th>
				</td>
				</tr>
			</thead>
			<tbody>
				<? foreach($classe->eleves() as $e) { ?>
				<tr>
					<td><?= $e->nom ?></td>
					<td><?= $e->prenom ?></td>
					<td><?= $e->age() ?></td>
					<td><img ></td>
					<td><input type="text"/></td>
					<td><input type="text"/></td>
				</tr>
				<? } ?>
			</body>
		</table>
	</form><?
	die();
}

function showProgression($classe,$prg,$periode,$niveau=0)
{
	if ($prg != null)
	{
		if ($prg->checkForPeriode($periode->id))
		{
			$childs = $prg->childNodes();
			$count = $prg->countEvaluations($periode->debut,$periode->fin);
			$results=CBilanProgressionElement::resultsForPeriodeProgression($periode->id,$prg->id);
			if (count($childs) == 0) $k= "nofolder";
			?>
			<tr class="niveau<?= $niveau ?> <?= $k ?>" xldeep="<?= $niveau ?>">
				<td><?= $prg->code ?></td>
				<td class="icon intitule"><?= $prg->intitule ?></td>
				<td class="number"><?= $count ?></td>
				<td><input type="checkbox" class="progression_periode" name="prog[<?= $prg->id ?>]" value="<?= $prg->id ?>" <?
						if ($prg->checkForPeriode($periode->id)) echo "checked"; ?> mzCount="<?= $count ?>"/></td>
				<td></td>
				<td><a class="saisie_result" href="export.php?module=bilan_p_saisie&bilan=<?= $periode->id ?>&progression=<?= $prg->id ?>">Saisie</a></td>	
			</tr>
		<? }
	} else $childs = $classe->progressions();
	$niveau++;
	if ($childs)
		foreach($childs as $c) showProgression($classe,$c,$periode,$niveau);
}

?>
<script type="text/javascript" charset="utf-8">
	Event.observe(document,"dom:loaded",function() {
		$$('a.saisie_result').each(function(x){
			new Control.Modal(x,{
				className: 'simple_popup',
				'fade': true,
				'afterOpen': function(){
						console.log("afterOpen");
						Event.fire(this.container,'dom:loaded');
					}
			});
		});
	});
</script>
<form method="POST">
	<table class="alternate liste treeview" id="progressions">
		<thead>
			<tr>
				<th colspan="2">Progression</th>
				<th>Nb. évaluations</th>
				<td><th>
			</tr>
		</thead>
		<tbody>
			<? showProgression($classe,null,$periode); ?>
		</body>
		<tfoot>
			<tr>
				<td colspan="2">
					<input type="submit" name="update" value="Enregistrer" onclick="updateParents();"/>
				</td>
				<td colspan="2">
					<input type="button" value="Cocher les progressions évaluées" onclick="checkEvaluees()"/>
				</td>
		</tfoot>
	</table>
	
</form>