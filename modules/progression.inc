<?
$module_classe = "progressions";
include("classe_header.inc");

if ($_GET["progression_id"])
{
	$progression = CProgression::objectWithId($_GET["progression_id"]);
	$_SESSION["progression"] = $progression;
} else if ($_GET["nouvelle_progression"])
{
	$progression = new CProgression();
	$progression->parentcp = $_GET["parentcp"];
	$progression->classe = $classe->id;
	$_SESSION["progression"] = $progression;
} else if ($_SESSION["progression"])
	$progression = $_SESSION["progression"];

// if (!$progression) $progression = new CProgression();

if ($_POST["maj_progression"] || $_POST["maj_progression_liste"])
{
	$progression->code = $_POST["code"];
	$progression->intitule = $_POST["intitule"];
	$progression->description = $_POST["description"];
	$progression->code_socle = $_POST["code_socle"];
	$progression->parentcp = $_POST["parent_cp"];
	$progression->update();
}

function selectProgrammeWithout(&$classe, $prog=null,$without=null,$deep=0,$selected_id=0)
{
	if ($prog == null)
	{
		foreach($classe->progressions() as $p) selectProgrammeWithout($classe,$p,$without,$deep,$selected_id);
	} else if ($prog->id !=  $without)
	{
		?><option value="<?= $prog->id ?>" <? if ($prog->id == $selected_id) echo "selected"; ?>><?= str_repeat("&nbsp;",$deep * 4) ?><?= $prog->info() ?></option><?
		$deep += 1;
		foreach($prog->childNodes() as $p) selectProgrammeWithout($classe,$p,$without,$deep,$selected_id);
	}
}
?>
<div id="body" class="progression">
	<h1>Progression : <?= $progression->intitule ?> </h1>
	<form method="post" action="?module=progression">
		<table style="width: 100%;">
			<tbody>
				<tr>
					<th>intitulé :</th>
					<th>code :</th>
				</tr>
				<tr>
					<td><input type="text" style="width: 100%;" name="intitule" value="<?= $progression->intitule ?>"></td>
					<td><input type="text" name="code" size="5" value="<?= $progression->code ?>"></td>
				</tr>
				<tr>
					<th colspan="2">Sous élément de (choisir 'aucun' pour ne pas inclure cette progression dans un thème existant) :</th>
				</tr>
				<tr>
					<td colspan="2">
								<select name="parent_cp"  style="width: 500px;">
									<option value="0">Aucun</option>
									<? selectProgrammeWithout($classe,null,$progression->id,0,$progression->parentcp) ?>
								</select></td>
				</tr>
				<? if ($progression->intitule_officel) { ?>
				<tr>
					<th colspan="2">Intitulé officiel :<th>
				</tr>
				<tr>
					<td colspan="2"><?= $progression->intitule_officel ?></td>
				</tr><? } ?>
				<tr>
					<th colspan="2">Socle :</th>
				</tr>
				<tr>
					<td colspan="2"><? if ($socle = $classe->socle()) { ?>
						<select name="code_socle" style="width: 500px;">
							<option value="">Choisir une compétence :</option>
							<optgroup label="Socle coummun :">
							<? $socle->htmlOptions(0,$progression->code_socle,$classe->id) ?>
							</optgroup>
							
							<? if ($b2i = $classe->b2i()) 
							{
								?><optgroup label="B2i"><?
								$b2i->htmlOptions(0,$progression->code_socle,$classe->id);	
								?></optgroup><?
							} ?>
							</select>
						<? } else { ?><input type="text" readonly name="code_socle" value="<?= $progression->code_socle ?>"> Veuillez sélectionner un socle pour cette classe dans le menu "Socle".<? } ?></td>
				</tr>
				<tr>
					<th colspan="2">description :</th>
				</tr>
				<tr>
					<td colspan="2"><textarea name="description" style="width: 100%; height: 100px;"><?= $progression->description ?></textarea></th>
				</tr>
			</tbody>
		</table>
		<table width="100%">
			<tr>
				<td width="33%" align="center"><input type="button" value="Retourner à la liste" onclick="document.location='?module=progressions'"></td>
				<td width="33%" align="center"><input type="submit" name="maj_progression_liste" value="Enregistrer et retourner à la liste"></td>
				<td width="33%" align="center"><input type="submit" name="maj_progression" value="Enregistrer"></td>
			</tr>
		</table>
	</form>
	<script language="javascript">
		<? if ($_POST["maj_progression_liste"]) { ?>
		document.location = "?module=progressions&focus_on=progression_<?= $progression->id ?>&highlight=tr_progression_<?= $progression->id ?>";
		<? } ?>		
	</script>
</div>
