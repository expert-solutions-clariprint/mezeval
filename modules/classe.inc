<? 
if ($_POST["update_and_continue"] && $_SESSION["classe"]) $_POST["update"] = true;

if ($_POST["periode_add"])
{
	CPeriode::createObject($_SESSION["classe"]->id,$_POST["periode_add_code"],
	$_POST["periode_add_intitule"],CDateUtil::unixDate($_POST["periode_add_debut"]),CDateUtil::unixDate($_POST["periode_add_fin"]));
}

if ($_POST["periode_delete"])
{
	foreach($_POST["periode_delete"] as $pid=>$z)
	{
		if ($p = CPeriode::objectWithIdAndClasse($pid,$_SESSION["classe"]->id))
			$p->del();
	}
}

if ($_POST["periode_set"])
	CPeriode::createFromSetForClasse($_POST["periode_set"],$_SESSION["classe"]->id);

if (count($_POST) && $_SESSION["classe"])
{
	$classe = $_SESSION["classe"];
	$classe->intitule = $_POST["intitule"];
	$classe->niveau = $_POST["niveau"];
	$classe->annee = intval($_POST["annee"]);
	$classe->annee2 = intval($_POST["annee2"]);
	$classe->description = $_POST["description"];
	$classe->a_color = $_POST["a_color"];
	$classe->b_color = $_POST["b_color"];
	$classe->c_color = $_POST["c_color"];
	$classe->z_color = $_POST["z_color"];

	$classe->a_title = stripslashes($_POST["a_title"]);
	$classe->b_title = stripslashes($_POST["b_title"]);
	$classe->c_title = stripslashes($_POST["c_title"]);
	$classe->z_title = stripslashes($_POST["z_title"]);

	$classe->a_code = $_POST["a_code"];
	$classe->b_code = $_POST["b_code"];
	$classe->c_code = $_POST["c_code"];
	$classe->z_code = $_POST["z_code"];


	if (!$classe->socle)
	{
		switch($classe->niveau)
		{
			case "CE1":
			case "CP":
			 	$classe->socle = "2008_PREMIER_PALIER.xml";
				break;
			case "CM2":
			case "CM1":
			case "CE2":
			 	$classe->socle = "2008_DEUXIEME_PALIER.xml";
				break;
		}
	}
	$classe->update();
		
	if ($_POST["update_and_continue"])
	{ ?>
		<script type="text/javascript" charset="utf-8">
			window.location = "?module=eleves";
		</script>
	<? }
}
if ($_GET["exportxml"])
{
	if ($classe = CClasse::objectWithId($_GET["exportxml"]))
	{
		header("Content-type: text/xml");
		header(sprintf('Content-Disposition: attachment; filename="%s_export.xml"',$classe->intitule));
		echo "<?xml version='1.0' ?>\n";
		echo "<?xml-stylesheet href=\"https://www.mezeval.fr/programme.xsl\" type=\"application/xml\"?>\n";
		$classe->print_xml();
	}
	die();
}

include("classe_header.inc");
?>
<script src="js/colorpicker/picker.js" type="text/javascript"></script>
<script type="text/javascript" charset="utf-8">
	function addPeriode()
	{
		if ($("periode_add_debut").value == '' || $("periode_add_fin").value == '')
		{
			alert("Attention : les dates sont obligatoires");
			return;
		}
	}
	
	function eleveRow(e)
	{
		var tr = new Element('TR');
		var tdNom = new Element(TD).insert(e.nom);
		var tdPrenom = new Element(TD).insert(e.nom);
		var tdDetail = new Element(TD).insert(new Element('A',{'href': "?module=eleve&eleve_id=" . e.id}).insert("détails"));
		var tdDelete = new Element(TD).insert(new Element('IMG',{'src':'/img/delete.png'}));
		return tr;
	}
	
	
	function addEleves()
	{
		var e = new Mz.Eleve(<?= $classe->id ?>,$("eleve_add_nom").value,$("eleve_add_prenom").value,$("eleve_add_date").value);
//		var r = eleveRow(e);
//		$('eleves').down('tbody').insert(r);
		
	}
	
	
	
	function alertePeriode()
	{
		return confirm("Attention, la suppression d’une période fera perdre les données associées (bilans de la période, association avec les progressions ... ) !");
	}
</script>

<link rel="stylesheet" type="text/css" href="js/colorpicker/picker.css"></link>
<div id="body" class="classes">

	<form method="POST">
		<table>
			<tbody>
				<tr>
					<th>Intitulé :</th>
					<th>Niveau :</th>
					<th>Année :</th>
				</tr>	
				<tr>
					<td><input type="text" size="60" name="intitule" value="<?= $classe->intitule ?>"></td>
					<td><select name="niveau"><?
							$a = array("CM2","CM1","CE2","CE1","CP","GS","MS","PS");
							foreach($a as $n) {
								if ($n == $classe->niveau) $sel = "selected"; else $sel = "";
								?><option <?= $sel ?> value="<?= $n ?>"><?= $n ?></option><?
							}
						?></select></td>
					<td><input type="text" size="5" name="annee" value="<?= $classe->annee() ?>">-<input type="text" name="annee2"  size="5" value="<?= $classe->annee2() ?>"></td>
					<td rowspan="3"><input type="submit" name="update" value="Enregistrer"/></td>
				</tr>
				<tr>
					<th colspan="3">Description (optionnel) :</th>
				</tr>
				<tr>
					<td colspan="3"><textarea rows="4" cols="80" name="description"><?= $classe->description ?></textarea></td>
				</tr>
			</tbody>
		</table>
		<hr>
		<h2>Niveaux de compétences</h2
		<table class="liste alternate">
			<thead>
				<tr>
					<td>Code</td>
					<td>Intitulé</td>
					<td>Couleur</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td><input type="text" name="a_code" value="<?= $classe->a_code() ?>" style="width: 20px"></td>
					<td><input type="text" name="a_title" value="<?= $classe->a_title() ?>" style="width: 200px"></td>
					<td><input type="text" name="a_color" id="a_color" value="<?= $classe->a_color ?>" onclick="openPicker('a_color');"  style="cursor: pointer; background-color: <?= $classe->a_color() ?>; width: 40px"></td>
				</tr>

				<tr>
					<td><input type="text" name="b_code" value="<?= $classe->b_code() ?>" style="width: 20px"></td>
					<td><input type="text" name="b_title" value="<?= $classe->b_title() ?>" style="width: 200px"></td>
					<td><input type="text" name="b_color" id="b_color" value="<?= $classe->b_color ?>" onclick="openPicker('b_color');"  style="cursor: pointer; background-color: <?= $classe->b_color() ?>; width: 40px"></td>
				</tr>

				<tr>
					<td><input type="text" name="c_code" value="<?= $classe->c_code() ?>" style="width: 20px"></td>
					<td><input type="text" name="c_title" value="<?= $classe->c_title() ?>" style="width: 200px"></td>
					<td><input type="text" name="c_color" id="c_color" value="<?= $classe->c_color ?>" onclick="openPicker('c_color');"  style="cursor: pointer; background-color: <?= $classe->c_color() ?>; width: 40px"></td>
				</tr>

				<tr>
					<td><input type="text" name="z_code" value="<?= $classe->z_code() ?>" style="width: 20px"></td>
					<td><input type="text" name="z_title" value="<?= $classe->z_title() ?>" style="width: 200px"></td>
					<td><input type="text" name="z_color" id="z_color" value="<?= $classe->z_color ?>" onclick="openPicker('z_color');"  style="cursor: pointer; background-color: <?= $classe->z_color() ?>; width: 40px"></td>
				</tr>
			</tbody>
			<tfoot>
				<tr>
					<td colspan="3"><input type="submit" name="update" value="Enregistrer"/></td>
				</tr>
			</tfoot>
		</table>
		<hr/>
		<h2>Périodes</h2>
		<table class="liste alternate">
			<thead>
				<tr>
					<td>Code (court)</td>
					<td>Intitulé</td>
					<td>du</td>
					<td>au</td>
					<td style="width: 20px;"></td>
				</tr>
			</thead>
			<tbody>
				<? 
				$periodes = CPeriode::objectsForClasse($classe->id);
				if (count($periodes) > 0)
				{
					foreach(CPeriode::objectsForClasse($classe->id) as $a) {
						if ($_POST["periode_update"])
						{
							if ($_POST["periode_update"][$a->id])
							{
								$a->intitule = $_POST["periode_intitule"][$a->id];
								$a->code = $_POST["periode_code"][$a->id];
								if ($d = $_POST["periode_debut"][$a->id])
									$a->debut = CDateUtil::unixDate($d);
								if ($d = $_POST["periode_fin"][$a->id])
									$a->fin = CDateUtil::unixDate($d);
								$a->update();
							}
						}
						?>
					<tr>
						<td><input type="text"  size="6" maxlength="5" name="periode_code[<?= $a->id ?>]" value="<?= $a->code ?>"></td>
						<td><input type="text"  size="40" name="periode_intitule[<?= $a->id ?>]" value="<?= $a->intitule ?>"></td>
						<td><input type="text" class="XLDatePicker" size="10" name="periode_debut[<?= $a->id ?>]" value="<?= $a->debut() ?>"></td>
						<td><input type="text" class="XLDatePicker" size="10" name="periode_fin[<?= $a->id ?>]" value="<?= $a->fin() ?>"></td>
						<td class="action">
							<input onclick="return alertePeriode();" type="image" name="periode_delete[<?= $a->id ?>]" src="img/delete.png"/>
							<input type="hidden" name="periode_update[<?= $a->id ?>]" value="1" >
						</td>
					</tr>
					<? } 
				} else { 
					?>
				<tr>
					<td colspan="4">Aucune période de définie</td>
				<? } ?>
			</tbody>
			<tfoot>
				<tr>
					<th colspan="2">Nouvelle période :</th>
					<th>(jj/mm/aa)</th>
					<th>(jj/mm/aa)</th>
				</tr>
				<tr>
					<td colspan="1"><input type="text" size="6" name="periode_add_code" id="periode_add_debut"/></td>
					<td colspan="1"><input type="text" size="40" name="periode_add_intitule" /></td>
					<td><input  class="XLDatePicker" type="text" size="10" name="periodes_add_debut" id="periode_add_debut"/></td>
					<td><input class="XLDatePicker" type="text" size="10" name="periode_add_fin" id="periode_add_fin"/></td>
					<td class="action"><input type="image" src="/img/add.png" name="periode_add" value="ajouter">
				</tr>
				<tr>
					<td colspan="5">
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
</div>
