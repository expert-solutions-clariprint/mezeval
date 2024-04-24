<?

if ($classe = CClasse::objectWithId($_GET["classe"]))
{
	if ($periode = CPeriode::objectWithIdAndClasse($_GET["periode"],$_GET["classe"]))
	{
		if ($progression = CProgression::objectWithIdAndClasse($_GET["progression"],$classe->id))
		{
			$eleves = $classe->eleves();
			?>
			<h1><?= $progression->info() ?></h1>
			<form onsubmit='update_progressions()'>
			<table class="liste alternate">
				<thead>
					<tr>
						<td>Élève</td>
						<td>Évaluation</td>
						<td>Commentaires</td>
					</tr>
				</thead>
				<tbody>
					<? foreach($eleves as $eleve) { ?>
					<tr>
						<td><?= $eleve->info() ?></td>
						<td><input autocomplete="off" class="eval" onkeyup="check_value(this,event)" type="text" size="2" maxlength="1" value="<?= $eval ?>" name="progression_value[<?= $eleve->id ?>]"></td>
						<td>
					</tr><?
					}?>
				</tbody>
			</table>
			</form>
			<?
		} else echo "Erreur de progression";
	} else echo "Erreur de periode";
} else echo "Erreur de classe";
