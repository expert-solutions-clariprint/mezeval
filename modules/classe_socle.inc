<? 

if ($_POST["export_csv"])
{
	if ($c = CSocle::objectFromFile($_POST["export_csv"]))
	{
		header("Content-Type: application/csv-coma-delimited-table");
		header("Content-disposition: filename=socle.csv");
		$c->csv();
		die();
	}
}
include("classe_header.inc");

if ($_POST["update_socle"])
{
	$classe->socle = $_POST["nouveau_socle"];
	$classe->update();
} else if ($_POST["update_socle_b2i"])
{
	$classe->socle_b2i = $_POST["nouveau_socle"];
	$classe->update();
} else if ($_POST["update_socle_langue"])
{
	$classe->socle_langue = $_POST["nouveau_socle"];
	$classe->update();
}

function showDom($node,$deep=1,$details=false)
{
	$classe = $_SESSION["classe"];
	$openfolder = "";
	if ((count($node->childNodes) > 0) && ($deep < 2)) $openfolder =" openfolder";
		
	?>
	<tr class="niveau<?= $deep ?><? if (count($node->childNodes) == 0) echo " nofolder"; ?><?= $openfolder ?>" 
		treeviewdeep="<?= $deep ?>" 
		onmouseover="addCssClass(this,'highlight');" 
		onmouseout="rmCssClass(this,'highlight');" 
		<? if ($deep > 2) { ?>style="display:none;"<? } ?>
		onclick="switchTreeView(event,this)">
		<td class="code"><?= $node->code ?></td>
		<td class="title icon"><?= $node->intitule ?></td>
		<? if ($details) { 
			$np = CProgression::countForClasseAndSocle($classe->id,$node->code,true);
			$ne = CElement::countEvaluationsForClasseAndSocle($classe->id,$node->code,true);
//			$n = CElement::countForClasseAndSocle();
			?>
		<td style="text-align: right"><?= $np ?></td>
		<td style="text-align: right"><?= $ne ?></td>
		<? } ?>
	</tr><? 		
	$deep++;
	foreach($node->childNodes as $sub) {
		showDom($sub,$deep,$details);
	}
}

?>
<div id="body" class="classe_socle">
	<h1>Socle commun de compétences, B2i  associé au groupe</h1>
	<h2>Socle commun</h2>
	<form method="post" action="?module=classe_socle">
		Socle commun : <select name="nouveau_socle">
									<option value="">Choisir le socle des compétences :</option>
								<? foreach(CSocle::objects() as $s) { ?>
									<option <? if ($classe->socle == $s) echo "selected"; ?> value="<?= $s ?>"><?= $s ?></options>
								<? } ?>
								</select><input type="submit" name="update_socle" value="enregistrer">
	</form>
	<?
	if ($socle = $classe->socle())
	{
		?>
		<table id="nav" class="control">
			<tr>
				<td id="back"><input type="button"  onclick="window.location='?module=periodes'" value="<< précédent : Périodes"></td>
				<td></td>
				<td id="next"><input type="button" onclick="window.location='?module=progressions'" value="Suivant : définir les progressions >>"></td>
			</tr>
		</table>

		<table class='liste alternate treeview'>
			<thead>
				<tr>
					<th>Code</th>
					<th>Intitulé</th>
					<th>Nb. progressions</th>
					<th>Nb évaluations</th>
				</tr>
			</thead>
			<tbody>
				<? showDom($socle,1,1); ?>
			</tbody>
		</table>
		<form method="post" action="export.php?module=classe_socle">
		<input type="hidden" name="export_csv" value="<?= $classe->socle ?>">
		<input type=submit value="Export en CSV (Excel)">
		</form>
		<?
	}
	?>
	<h2>B2i</h2>
	<form method="post" action="?module=classe_socle">
		<select name="nouveau_socle">
									<option value="">Choisir un b2i :</option>
								<? foreach(CSocle::b2iObjects() as $s) { ?>
									<option <? if ($classe->socle_b2i == $s) echo "selected"; ?> value="<?= $s ?>"><?= $s ?></options>
								<? } ?>
								</select><input type="submit" name="update_socle_b2i" value="enregistrer">
	</form>
	<? if ($socle = $classe->b2i())
	{ ?>
		<table class='liste alternate treeview'>
			<thead>
				<tr>
					<th>Code</th>
					<th>Intitulé</th>
					<th>Nb. progressions</th>
					<th>Nb évaluations</th>
				</tr>
			</thead>
			<tbody>
				<? showDom($socle,1,1); ?>
			</tbody>
		</table><?

	} ?>

	<h2>Langue étrangère A1</h2>
	<form method="post" action="?module=classe_socle">
		<select name="nouveau_socle">
									<option value="">Choisir un langue :</option>
								<? foreach(CSocle::A1Objects() as $s) { ?>
									<option <? if ($classe->socle_langue == $s) echo "selected"; ?> value="<?= $s ?>"><?= $s ?></options>
								<? } ?>
								</select><input type="submit" name="update_socle_langue" value="enregistrer">
	</form>
	<? if ($socle = $classe->langue())
	{ ?>
		<table class='liste alternate treeview'>
			<thead>
				<tr>
					<th>Code</th>
					<th>Intitulé</th>
					<th>Nb. progressions</th>
					<th>Nb évaluations</th>
				</tr>
			</thead>
			<tbody>
				<? showDom($socle,1,1); ?>
			</tbody>
		</table><?

	} ?>
</div>
