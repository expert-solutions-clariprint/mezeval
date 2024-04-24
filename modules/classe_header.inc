<? 
if (!$_SESSION["etablissement"]) {
	CTemplate::includeModule("classes");
	exit();
}
$etablissement = $_SESSION["etablissement"];
if ($_GET["nouvelle_classe"]) {
	$classe = new CClasse();
	$classe->etablissement = $etablissement->id;
	$classe->manager = CUser::user()->id;
	$_SESSION["classe"] = $classe;
} else if ($_GET["id"]) {
	$classe = CClasse::objectWithId($_GET["id"]);
	$_SESSION["classe"] = $classe;
} else if ($_SESSION["classe"]) $classe = $_SESSION["classe"];
else {
	echo "la";
	CTemplate::includeModule("classes");
	exit();
}
CTemplate::draw_main_menu("classes");
$MENUS_CLASSE = array("classe" => "Paramétres");

if ($classe->id)
	$MENUS_CLASSE = array(	"classe" => "Paramètres",
							"eleves" => "Élèves",
							"classe_socle" => "Doc. de cycle",
							"progressions" => "Progressions",
							"evaluations" => "Évaluations",
							"statistiques" => "Statistiques",
							"bilans" => "Bilans");

?>
<div id="menus" style="height:auto">
	<h1><?= $classe->intitule() ?></h1>
	<? foreach($MENUS_CLASSE as $m => $t) { 
			if ($m == $_GET["module"] || $module_classe == $m) $s = "selected_classe";
			else $s = "";
			?>
		<div class="<?= $s ?>"><a href="?module=<?= $m ?>"><?= $t ?></a></div>
		<? } ?>
</div>