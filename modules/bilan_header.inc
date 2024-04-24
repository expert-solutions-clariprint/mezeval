<? 
$module_classe = "bilans";
include("classe_header.inc");
if ($_GET["bilan_id"])
{
	$bilan = CBilanProgression::objectWithid($_GET["bilan_id"]);
	$_SESSION["bilan"] = $bilan;
} else if ($_GET["nouveau_bilan"])
{	
	$bilan = new CBilanProgression();
	$evaluation->classe = $classe->id;
	$_SESSION["bilan"] = $bilan;
} else if ($_SESSION["evaluation"]) $evaluation = $_SESSION["evaluation"];

$MENUS_EVALUATION = array("bilan" => "Détails");
?>
<div id="body" class="<?= DIV_BODY_CLASS ?>">
	<div id="sousmenus">
		<? foreach($MENUS_EVALUATION as $m => $t) { 
				if ($m == $_GET["module"] || $module_classe == $m) $s = "selected_classe";
				else $s = "";
				?>
			<div class="<?= $s ?>"><a href="?module=<?= $m ?>"><?= $t ?></a></div><? } ?>
	</div>
	<h1><?= $bilan->intitule() ?></h1>
