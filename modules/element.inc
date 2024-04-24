<?
if ($element = CElement::objectWithId($_GET["element_id"]))
{
	$classe = $_SESSION["classe"];
	
	?>
			<h1>Modifier un élément d’évaluation :</h1>
			<form method="post">
				<input type="hidden" name="update_element" value="<?= $element->id ?>">
				<table class="element_edit">
					<tbody>
						<tr>
							<th>Description : <th>
						<tr>
							<td><textarea name="element_description" style="width: 100%"><?= $element->description ?></textarea></td>
						<tr>
							<th>Progression : <th>
						<tr>
							<td>
								<select name="element_progression" style="width: 100%">
									<option value="">Choisir une compétence : </option>
									<? CProgression::asOptionsForClasse($classe->id,null,1,$element->progression); ?>
								</select>
							</td>
						<tr>
							<th>Mode d’évaluation : <th>
						<tr>
							<td>
						<select name="element_mode" id="element_mode" style="width: 100%" onchange="selectModeEvaluation(this,$('element_valeur_max'))">
							<? foreach(CEvaluation::modesCalcul() as $k => $mc) { ?>
								<option value="<?= $k ?>" <? if ($k == $element->mode) echo "selected"; ?>><?= $mc ?></option><? } ?>
							</select>
						<tr>
							<td>
								<div id="element_valeur_max"  <? if ($element->mode != "NOTES") {?> style="visibility: hidden" <? } ?>>
							Total :  <input type="text" value="<?= $element->valeur_max ?>" name="element_valeur_max" size="3"><br>
							Semble acquis si >= <input type="text" name="element_seuil_a" size="3" value="<?= $element->seuil_a ?>"><br>
							En début d’acquisition si <= <input type="text" name="element_seuil_b" size="3" value="<?= $element->seuil_b ?>"><br>			
				</table>
				<table>
					<tr>
						<td><input type="button" value="Annuler" onclick="dialogCancel(this)">
						<td><input type="submit" value="Enregistrer">
					</tr>
				</table>
			</form><?
}
