<? 
include("classe_header.inc");

if ($_POST["del_periode"])
{
	foreach($_POST["del_periode"] as $k => $v)
	{
		CPeriode::deleteObjectWithId($k);
	}
} else if ($_POST["add_periode"])
{
	CPeriode::createObject(
			$classe->id,
			$_POST["add_code"],
			$_POST["add_intitule"],
			CDateUtil::unixDate($_POST["add_debut"]),
			CDateUtil::unixDate($_POST["add_fin"]));
			
} else if ($_POST["periode_set"])
{
	CPeriode::createFromSetForClasse($_POST["periode_set"],$classe->id);
}
?>
<div id="body" class="classes">
	<form method="post" action="?module=periodes">
		<h1>Périodes</h1>
		<p>Une période permet de définir une plage de temps de date à date comme un trimestre. Pour ajouter une période, entrez un intitulé (ex "Premier trimestre"), une date de début, une date de fin et appuyez sur "Ajouter" :</p>
		<table class="liste alternate">
			<thead>
				<tr>
					<td style="width: 20px;"></td>
					<td>Code (court)</td>
					<td>Intitulé</td>
					<td>du</td>
					<td>au</td>
				</tr>
			</thead>
			<tbody>
				<? 
				$periodes = CPeriode::objectsForClasse($classe->id);
				if (count($periodes) > 0)
				{
					foreach(CPeriode::objectsForClasse($classe->id) as $a) {
						if ($_POST["update_periodes"])
						{
							$a->intitule = $_POST["periode_intitule"][$a->id];
							$a->code = $_POST["periode_code"][$a->id];
							if ($d = $_POST["periode_debut"][$a->id])
								$a->debut = CDateUtil::unixDate($d);
							if ($d = $_POST["periode_fin"][$a->id])
								$a->fin = CDateUtil::unixDate($d);
							$a->update();
						}
						?>
					<tr onmouseover="addCssClass(this,'highlight')" onmouseout="rmCssClass(this,'highlight')">
						<td><input type="image" name="del_periode[<?= $a->id ?>]" src="img/delete.png"></td>
						<td><input type="text"  size="6" maxlength="5" name="periode_code[<?= $a->id ?>]" value="<?= $a->code ?>"></td>
						<td><input type="text"  size="40" name="periode_intitule[<?= $a->id ?>]" value="<?= $a->intitule ?>"></td>
						<td><input type="text"  size="10" name="periode_debut[<?= $a->id ?>]" value="<?= $a->debut() ?>"></td>
						<td><input type="text"  size="10" name="periode_fin[<?= $a->id ?>]" value="<?= $a->fin() ?>"></td>
					</tr>
					<? } 
				} else { 
					?>
				<tr>
					<td colspan="4">Aucune période de définie</td>
				<? } ?>
			</tbody>
			<tfoot>
				<? if (count($periodes) > 0) { ?>
				<tr>
					<td colspan="4" style="text-align: right"><input type="submit" name="update_periodes" value="Enregistrer les changements"></td>
				</tr><? } ?>
				<tr>
					<th colspan="3">Nouvelle période :</th>
					<th>du (jj/mm/aa)</th>
					<th>au (jj/mm/aa)</th>
				</tr>
				<tr>
					<td></td>
					<td colspan="1"><input type="text" size="6" name="add_code"></td>
					<td colspan="1"><input type="text" size="40" name="add_intitule"></td>
					<td><input onfocus="this.select();lcs(this)" type="text" size="10" name="add_debut" onclick="event.cancelBubble=true;this.select();lcs(this)"></td>
					<td><input  onfocus="this.select();lcs(this)" type="text" size="10" name="add_fin" onclick="event.cancelBubble=true;this.select();lcs(this)"></td>
					<td><input type="submit" name="add_periode" value="ajouter">
				</tr>
				<tr>
					<td colspan="3">
						<select name="periode_set" onchange="form.submit();">
							<option value="">Importer les périodes scolaires :</option>
							<? foreach(CPeriode::standardSets() as $p) {
								printf("<option  value='%s'>%s</option>",
									$p->filename,
									$p->name);
							} ?>
						</select>
					</td>
			</tfoot>
		</table>
	</form>	
	
		<? if (count($periodes) > 0) { ?>
		<table id="nav" class="control">
			<tr>
				<td id="back"><input type="button"  onclick="window.location='?module=eleves'" value="<< précédent : Élèves"></td>
				<td></td>
				<td id="next"><input type="button" onclick="window.location='?module=classe_socle'" value="Suivant : choix du socle commun >>"></td>
			</tr>
		</table><? } ?>
</div>
