<?
include("classe_header.inc");

if ($_GET["competence_id"])
{
	$competence = CCompetence::objectWithId($_GET["competence_id"]);
	$_SESSION["competence"] = $competence;
} else if ($_GET["nouvelle_competence"])
{
	$competence = new CCompetence();
	$competence->parentcp = $_GET["parentcp"];
	$competence->classe = $classe->id;
	$_SESSION["competence"] = $competence;
} else if ($_SESSION["competence"])
	$competence = $_SESSION["competence"];

// if (!$competence) $competence = new CCompetence();

if ($_POST["maj_competence"])
{
	$competence->code = $_POST["code"];
	$competence->intitule = $_POST["intitule"];
	$competence->description = $_POST["description"];
	$competence->update();
}
print_r($competence);
?>
<div id="body" class="competence">
	<form method="post" action="?module=competence">
	<table style="width: 800px">
		<tr>
			<th>code :<br>
				<input type="text" name="code" size="5" value="<?= $competence->code ?>"></th>
		</tr>
		<tr>
			<th>intitulé : <br>
			<input type="text" style="width: 100%;" name="intitule" value="<?= $competence->intitule ?>"></th>
		</tr>
		<tr>
			<th>intitulé offciel : <br>
			<?= $competence->intitule_officel ?></th>
		</tr>
		<tr>
			<th>description : <br>
			<textarea name="description" style="width: 100%; height: 100px;"><?= $competence->description ?></textarea></th>
		</tr>
	</table>
	<input type=submit name="maj_competence" value="Enregistrer">
	</form>
</div>
