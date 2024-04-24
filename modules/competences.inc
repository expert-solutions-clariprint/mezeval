<?
include("classe_header.inc");

if ($_POST["import_programme"])
{
	CProgramme::importFromFileToClasse($_POST["programme"],$classe->id);
}
if ($_POST["delete_competence"])
{
	foreach($_POST["delete_competence"] as $k => $c) CCompetence::deleteObjectWithId($k);
}

$PERIODES = $classe->periodes();

function showProgramme($classe_id,$parent = 0,$deep=1)
{
	if ($parent == 0)
		$subs = CCompetence::parentsForClasse($classe_id);
	else 
		$subs = CCompetence::objectForParent($parent);
	foreach($subs as $c) { ?>
<h<?= $deep ?>><?= $c->code ?> <?= $c->intitule ?></h<?= $deep ?>>
<p><?= $c->description ?></p>
<a href="?module=competence&competence_id=<?= $c->id ?>">modifier</a>
<div class="detail_<?= $deep ?>" id="<?= $c->id ?>" style="margin-left: <?= $deep ?>cm;">
	<? showProgramme($classe_id,$c->id,$deep + 1); ?>
</div>
	<a href="?module=competence&nouvelle_competence=1&parentcp=<?= $c->id ?>">Nouveau...</a><? 
 	}
}
function showProgrammeAsTable($classe_id,$parent = 0,$deep=1)
{
	if ($parent == 0)
		$subs = CCompetence::parentsForClasse($classe_id);
	else 
		$subs = CCompetence::objectForParent($parent);
	foreach($subs as $c) { ?>
<tr class="niveau<?= $deep ?>" onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
	<td><input type="image" src="img/delete.png" name="delete_competence[<?= $c->id ?>]">
	<td class="code"><?= $c->code ?></td>
	<td class="intitule"><?= $c->intitule ?></td>
	<td class="modifier"><a href="?module=competence&competence_id=<?= $c->id ?>">modifer</a></td>
	<td class="souselement"><a href="?module=competence&nouvelle_competence=1&parentcp=<?= $c->id ?>">+</a><td>
</tr>
	<? showProgrammeAsTable($classe_id,$c->id,$deep + 1); ?>
</tr><? 
 	}
}

?>
<div id="body" class="competences">
	<h1>Progression</h1>
	<a href="?module=competence&nouvelle_competence=1&parentcp=0">Nouveau Thème</a>
	<?  // showProgramme($classe->id) 
	?>
	<form method="post" action="?module=competences">
	<table class="programme">
		<thead>
			<tr>
				<th></th>
				<th>code</th>
				<th>intitulé</th>
		</thead>
		<tbody>
			<? showProgrammeAsTable($classe->id) ?>
		</tbody>
		<tfoot>
		</tfoot>
	</table>
	<h2>Importer</h2>
		<select name="programme">
			<? foreach(CProgramme::objects() as $p) { ?>
			<option><?= $p ?></option><? } ?>
		</select><input type="submit" name="import_programme">
	</form>
</div>
