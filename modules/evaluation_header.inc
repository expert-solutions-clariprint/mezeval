<? 
$module_classe = "evaluations";
include("classe_header.inc");
if ($_GET["evaluation_id"])
{
	$evaluation = CEvaluation::objectWithid($_GET["evaluation_id"]);
	$_SESSION["evaluation"] = $evaluation;
} else if ($_GET["nouvelle_evaluation"])
{	
	$evaluation = new CEvaluation();
	$evaluation->classe = $classe->id;
	$_SESSION["evaluation"] = $evaluation;
} else if ($_SESSION["evaluation"]) $evaluation = $_SESSION["evaluation"];

if ($_POST["update_evaluation"] || $_POST["update_goto_elements"])
{
	$evaluation->intitule = stripcslashes($_POST["intitule"]);
	$evaluation->date_examen = CDateUtil::unixDate($_POST["date_examen"]);
	$evaluation->duree = CDateUtil::userTimeToMinutes($_POST["duree"]);
	$evaluation->description = stripcslashes($_POST["description"]);
	if ($_POST["classeur"] == "**")
		$evaluation->classeur = $_POST["autre_txt"];
	else 
		$evaluation->classeur = $_POST["classeur"];
	$evaluation->update();
}

if ($evaluation->id)
$MENUS_EVALUATION = array("evaluation" => "Détails",
						"evaluation_elements" => "Compétences évaluées",
						"evaluation_notes" => "Saisie",
						"evaluation_stats" => "Résultats");
else 
$MENUS_EVALUATION = array("evaluation" => "Détails");
?>
<div id="body" class="<?= DIV_BODY_CLASS ?>">
	<? if ($evaluation->id) { ?>
	<div id="sousmenus">
		<div>
			<p>
			<select onchange="document.location = '?module=<?= $_GET["module"] ?>&evaluation_id=' + this.value;">
				<? foreach($classe->evaluations() as $ev) { 
						if ($ev->id == $evaluation->id) $sel = "selected";
						else $sel="";
						?>
					<option <?= $sel ?> value="<?= $ev->id ?>"><?= $ev->intitule() ?></option><? } ?>
			</select>
			</p>
		</div>
		<? foreach($MENUS_EVALUATION as $m => $t) { 
				if ($m == $_GET["module"] || $module_classe == $m) $s = "selected";
				else $s = "";
				?>
			<div class="<?= $s ?>"><p><a href="?module=<?= $m ?>"><?= $t ?></a></p></div><? } ?>
	</div><? } ?>
