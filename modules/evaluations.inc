<?php 
include("classe_header.inc");

?>
<div id="body" class="evaluations">
	<script language="javascript">
	function buildElementForm()
	{
	}
	
	function applyfilter(classeur)
	{
		var tb = $("tbody_evaluations");
		var i =0;
		for(i = 0; i < tb.rows.length; i++)
		{
			var row = tb.rows[i];
			if (row.getAttribute("CLASSEUR") == classeur || classeur == "*")  $(row).show();
			else $(row).hide();
		}
	}
	</script>
	<h1>Évaluations</h1>
	<?
	if ($_POST["import_submit"] )
	{
		if ($_FILES["import_file"] && !$_FILES["import_file"]["error"])
		{
			$x = new CEvaluation();
			$x->classe = $classe->id;
			$x->create($DB);
			if ($x->import($_FILES["import_file"]["tmp_name"]))
			{
				?><div class="success">Évaluation importée avec succèss.</div><?
			} else {
				$x->del();
				?><div class="alert">L’importation a été annulée suite à une erreur.</div><?
			}
		} else 
		{
			?><div class="alert">Attention, vous devez selectionner un fichier ".xml" en utilisant le bouton adéquat !</div><?
		}
	}
	?>
	<form action="?module=evaluations" method="post" accept-charset="utf-8" enctype="multipart/form-data">
 		<table class="control">
			<thead>
				<tr>
					<th>Création d’une nouvelle évaluation :</th>
					<th>Importer une évaluation à partir d’un fichier  :</th>
			</thead>
			<tbody>
				<tr>
					<td>
						<input type="button" onclick="location.href='?module=evaluation&nouvelle_evaluation=1'" value="Nouvelle évaluation">
					</td>
					<td>
						<input type="file" name="import_file">
						<label><input type="checkbox" name="import_results"> <label for="import_file">Importer également les résultats</label>
						<input type="submit" name="import_submit" value="Importer">
					</td>
				</tr>
			</tbody>
		</table>
	</form><?
	if ($_POST["delete_eval"]) {
		foreach($_POST["delete_eval"] as $k => $zz)
		{
			CEvaluation::deleteObjectWithId($k);
		}
	}
	
	$evals = $classe->evaluations();
	
	if (count($evals)) {  ?>
		
	<script type="text/javascript" charset="utf-8">
		function impression(select)
		{
			var o = select.options[select.selectedIndex];
			var action = o.getAttribute("action");
			if (action != null) {
				document.location = action;
			}
			o.selected = false;
			select.options[0].selected = true;
		}
	</script>
	
		<table class="control">
			<tr>
				<th>Recherche : </th>
				<td><select onchange="applyfilter(this.value)">
						<option value="*">Classeur :</option>
						<? foreach(CEvaluation::classeursForClasse($classe->id) as $c) { ?>
						<option><?= $c ?></option><? } ?>
					</select></td>
				<td>Contenant le texte :
				<td><input type="search" target="t_evaluations" target_cols="1 2" id="search_key" onkeydown="if (event.keyCode == 13) {searchTable('search_key'); return false;};"><input type="button" value="recherche" onclick="searchTable('search_key')">
			</tr>
		</table>
		<form method="POST">
		<table class="liste alternate" id="t_evaluations">
			<thead>
				<tr>
					<th></th>
					<th>Intitulé</th>
					<th>Classeur</th>
					<th>Date</th>
					<th colspan="6"></th>
			</thead>
			<tbody id="tbody_evaluations">
				<? foreach(CEvaluation::objectsForClasse($classe->id) as $c) { ?>
				<tr onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';" classeur="<?= $c->classeur ?>">
					<td><input type="image" src="img/delete.png" name="delete_eval[<?= $c->id ?>]" onclick="return confirm('Attention : suppression définitive de l’évaluation ?')">
					<td><?= $c->intitule ?></td>
					<td><?= $c->classeur ?></td>
					<td><?= $c->date_examen() ?></td>
					<td class="action"><a href="?module=evaluation&evaluation_id=<?= $c->id ?>">détail</a></td>
					<td class="action"><a href="?module=evaluation_elements&evaluation_id=<?= $c->id ?>">compétences</a></td>
					<td class="action"><a href="?module=evaluation_notes&evaluation_id=<?= $c->id ?>">saisie résultats</a></td>
					<td class="action"><a href="?module=evaluation_stats&evaluation_id=<?= $c->id ?>">résultats</a></td>
					<td class="action"><select onchange="impression(this)" style="width: 100px">
											<option>Imprimer : </option>
											<option action="export.php?module=evaluation_saisie_pdf&evaluation_id=<?= $c->id ?>">Grille
d'évaluation</option>
											<option action="export.php?module=evaluation_pdf&evaluation_id=<?= $c->id ?>&histogramme=1">fiches de résultats par élève</option>
											<option action="export.php?module=evaluation_pdf&evaluation_id=<?= $c->id ?>">fiches de résultats par élève sans histogramme</option>
										</select>
					</td>
					<td class="action"><a href="export.php?module=evaluation&export_xml=<?= $c->id ?>">export</a></td>
				</tr>
				<? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table><?
	} else { ?>
	<p>Pour créer une évaluation veuillez cliquer sur le bouton "Nouvelle évaluation".</p>
	<? } ?>
	</form>
	
</div>
