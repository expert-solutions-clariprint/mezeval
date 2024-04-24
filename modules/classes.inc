<? 
CTemplate::draw_main_menu("classes");
$_SESSION["classe"] = null;
$_SESSION["eleve"] = null;
$_SESSION["evaluation"] = null;

if (!$_SESSION["etablissement"])
{
	$ets = CEtablissement::firstObjectInDatabase() /* CPTicket::etablissementsForCurrentUser() */ ; 
	if (count($ets) == 1) {
		$etablissement = $ets[0];
		$_SESSION["etablissement"]=$etablissement;
	}
} else $etablissement = $_SESSION["etablissement"];

if ($_POST["delete_classe"])
{
	foreach($_POST["delete_classe"] as $k => $v) CClasse::deleteObjectWithId($k);
}
if ($_POST["import_class_action"] && $_FILES["import_class_file"])
{
	CClasseXmlImporter::importFromFile($_FILES["import_class_file"]["tmp_name"]);
}
if ($_GET["archiver_classe"])
{
	if ($classe = CClasse::objectWithId($_GET["archiver_classe"]))
	{
		ob_start();
		$classe->print_xml();
		$data = ob_get_contents();
		ob_end_clean();
		CArchive::createForUserKindDescriptionData(CUser::user()->id,"CLASSE",$classe->info(),$data);
	}	
}

if ($_GET["archive_exporter"])
{
	if ($ar = CArchive::objectWithId($_GET["archive_exporter"]))
	{
		header("Content-type: text/xml");
		header(sprintf('Content-Disposition: attachment; filename="archive_%s_%s.xml"',$ar->description(),$ar->date("%d_%m_%Y")));
		echo "<?xml version='1.0' ?>\n";
		echo "<?xml-stylesheet href=\"https://www.mezeval.fr/xsl/classe.xsl\" type=\"application/xml\"?>\n";
		print($ar->data());
		die();
	}
}
if ($_GET["archive_restaurer"])
{
	if ($ar = CArchive::objectWithId($_GET["archive_restaurer"]))
	{
		echo $ar->data();
		if ($classe = CClasseXmlImporter::importFromData($ar->data()))
		{
			$classe->intitule .= sprintf(" au %s restauré le %s",$ar->date(),strftime("%d/%m/%Y %H:%M"));
			$classe->update();
		}
	}
}

if ($_GET["nouvelle_classe"])
{
	?><script type="text/javascript" charset="utf-8">
			window.location = '?module=classe&nouvelle_classe=1';
		</script><?
}

?>
<div id="body" class="classes">
	<?
	if ($etablissement)
	{ 
		$lclasse = CUser::classes();
		if ($lclasses) { ?>
		<div id="info">Dans Mézeval, une classe est constituée de groupes de niveau différents. Pour les classes ne comportant qu’un groupe homogène, veuillez simplement ajouter une seul groupe.</div>
			<? } ?>
	<h1>Groupes :</h1>
	<form method="post">
		<table class="liste alternate">
			<thead>
				<tr>
					<td></td>
					<td>Intitulé</td>
					<td>Niveau</td>
					<td>Année</td>
					<td colspan="10">
						<?	if (CUser::checkWriteToModule()) { ?>
						<input type="button" onclick="location.href='?module=classe&nouvelle_classe=1'" value="Nouveau groupe"><? } ?>
				</tr>	
			</thead>
			<tbody>
				<? foreach(CUser::classes() as $c) { ?>
				<tr onmouseover="addCssClass(this,'highlight')" onmouseout="rmCssClass(this,'highlight')">
					<td><input type="image" src="img/delete.gif" name="delete_classe[<?= $c->id ?>]" onclick="return confirm('Attention : opération irréversible, suppression de toutes les données concernant ce groupe (notes, élèves ...)')">
					<td><?= $c->intitule ?></td>
					<td><?= $c->niveau ?></td>
					<td><?= $c->annee ?></td>
					<?	if (CUser::checkAccessToModule("classe")) { ?>
					<td class="action"><a href="?module=classe&id=<?= $c->id ?>">Info.</a></td><? } ?>
					<?	if (CUser::checkAccessToModule("eleves")) { ?>
					<td class="action"><a href="?module=eleves&id=<?= $c->id ?>">Élèves</a><? } ?>
					<td class="action"><a href="?module=periodes&id=<?= $c->id ?>">Périodes</a></td>
					<td class="action"><a href="?module=classe_socle&id=<?= $c->id ?>">Docs&nbsp;cycle</a></td>
					<?	if (CUser::checkAccessToModule("progressions")) { ?>
					<td class="action"><a href="?module=progressions&id=<?= $c->id ?>">Progressions</a><? } ?>
					<?	if (CUser::checkAccessToModule("evaluations")) { ?>
					<td class="action"><a href="?module=evaluations&id=<?= $c->id ?>">Évaluations</a><? } ?>
					<td class="action"><a href="?module=statistiques&id=<?= $c->id ?>">Stats</a></td>
					<?	if (CUser::checkAccessToModule("bilans")) { ?>
					<td class="action"><a href="?module=bilans&id=<?= $c->id ?>">Bilans</a><? } ?>
					<?	if (CUser::checkAccessToModule("classe")) { ?>
					<td class="action"><a href="index_export.php?module=classe&exportxml=<?= $c->id ?>" title="Exporter aux format XML">Exporter</a></td><? } ?>
					<td class="action"><a href="?module=classes&archiver_classe=<?= $c->id ?>" title="Archiver">Archiver</a></td>
				</tr><? } ?>
			</tbody>
		</table>
	</form>
	<? } ?>
	<h2>Importer :</h2>
	<form method="POST" enctype="multipart/form-data">
		<input type="file" name="import_class_file" > <input name="import_class_action" type="submit" value="Continue &rarr;">
	</form>
	<h2>Archives : </h2>
	<? $archives = CArchive::lightObjectsWithUser(CUser::user()->id,'CLASSE');
	if ($archives) { ?>
		<table class="liste alternate">
			<thead>
				<tr>
					<th>Description</th>
					<th>Date</th>
					<th colspan="3">actions</th>
				</tr>
			</thead>
			<tbody>
				<? foreach($archives as $a) { ?>
				<tr>
					<td><?= $a->description() ?></td>
					<td><?= $a->date() ?></td>					
					<td class="action"><a href="?module=classes&archive_restaurer=<?=$a->id ?>">Restaurer</a></td>
					<td class="action"><a href="export.php?module=classes&archive_exporter=<?=$a->id ?>">Exporter</a></td>
					<td class="action">Supprimer</td>
				</tr><? } ?>
			</tbody>
		</table><?
	} else { ?><p>Aucune groupe n'est archivée</p><? } ?>
</div>
