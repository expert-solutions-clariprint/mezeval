<?
if ($_POST["export_xml"])
{
	$eva = $_SESSION["evaluation"];
	$eva->export($_POST["inclure_resultats"]);
	die();
}
if ($_GET["export_xml"])
{
	$eva = CEvaluation::objectWithId($_GET["export_xml"]);
	$eva->export($_GET["inclure_resultats"]);
	die();
}

define("DIV_BODY_CLASS","evaluation");
include("evaluation_header.inc");

?>
	<script language="javascript">
	function check_add_form()
	{
		var select = document.getElementById("add_progression");
		var option = select.options[select.selectedIndex];
		
		if (option.getAttribute("DISABLED"))
		{
			alert('Attention : la compétence sélectionnée n’est pas valide !');
			return false;
		}
		select = document.getElementById("add_mode");
		var input = document.getElementById("add_valeur_max");
		if (select.value == "NOTES" && input.value == "")
		{
			alert("Attention : vous devez définir une valeur de notation");
			return false;
		}
		if (select.selectedIndex == 0)
			return confirm("Attention : votre critère d’évaluation n'est pas chainé à une compétence, il ne sera pas pris en compte pour les annalyses ultérieures !");
		
		return true;
	}
	function selectModeEvaluation(select,input)
	{
		if (select.value == "NOTES" && input != null)
		{
			input.style.visibility = "visible";
			input.focus();
		} else input.style.visibility = "hidden";
		
	}
	<? if ($_POST["update_goto_elements"])
	{ ?>
	document.location="?module=evaluation_elements";
	<? } ?>
	</script>
	<form method="post" action="?module=evaluation"> 
		<table class="liste">
			<tbody>
				<tr>
					<td>Intitulé :<br><input size="80" type=text name="intitule" value="<?= htmlspecialchars($evaluation->intitule) ?>"></td>
					<td>Date :<br><input type=text name="date_examen" value="<?= $evaluation->date_examen() ?>"></td>
					<td>Durée :<br><input type="text" size="6" name="duree" value="<?= $evaluation->duree() ?>"></td>
				</tr>
				<tr>
					<td colspan="3">Commentaire : <br><textarea NAME="description" style="width: 700px; height: 30px;"><?= $evaluation->description ?></textarea></td>
				</tr>
				<tr>
					<td colspan="3">Classeur : <select name="classeur" onchange="if (this.value == '**') { $('autre_txt').show(); $('autre_txt').focus(); } else $('autre_txt').hide(); ">
									<option value="" >aucun</option>
									<? foreach(CEvaluation::classeursForClasse($classe->id) as $c) { ?>
									<option <? if ($c ==  $evaluation->classeur) echo("selected"); ?>><?= $c ?></option><? } ?>
									<option value="**">autre ...</option>
								</select>
								<input type="text" id="autre_txt" name="autre_txt" style="display: none"></t
					</td>
				</tr>
			</tbody>
		</table>
		<table>
			<tr>
				<td><input type="submit" name="update_evaluation" value="Enregistrer"></td>
				<td><input type="submit" name="update_goto_elements" value="Enregistrer & saisir les élements d'évaluation"></td>
			</tr>
		</table>
		<?
		$elems = $evaluation->elements();
		if (count($elems) > 0)
		{ 
			$indexes = array();
			foreach($elems as $e) {$indexes[$e->order_index] = $e; };
			if (count($indexes) <> count($elems))
			{
				$i=0;
				foreach($elems as $e)
				{
					$e->setOrderIndex($i);
					$i++;
				}
			}
			?>
		<h2>Compétences évaluées</h2>
		<table class="liste alternate">
			<thead>
				<tr>
					<th>Compétences</th>
					<th>Progression</th>
					<th style="width: 200px;">Mode d’évaluation</th>
				</tr>
			</thead>
			<tbody><?
				$mds = CEvaluation::modesCalcul();
				foreach($elems as $c) { 
					$prog = $c->progression();
				?>
				<tr>
					<td><?= $c->description ?></td>
					<td><?= $prog->intitule ?></td>
					<td><?= $c->mode() ?></td>
				</tr><? } ?>
			</tbody>
		</table>
				<td><input type="submit" name="update_goto_elements" value="Modifier les compétences évaluées >>"></td>
		<? }  ?>
	</form>

	<? if ($evaluation->id) { ?>
	<h1>Export</h1>
	<form method="POST" action="export.php?module=evaluation">
	<table class="control">
		<thead>
			<tr>
				<th>Type d’exportation</th>
				<th>Options</th>
				<th></th>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td>
					Export de l’évaluation pour partage et importation ultérieure dans Mezeval
				</td>
				<td>
					<input type="checkbox" name="inclure_resultats" value="1">Inclure les résultats
				</td>
				<td>
					<input type="submit" name="export_xml" value="Exporter en XML">
				</td>
			</tr>
		</tbody>
	</table>
	</form><? } ?>
</div>
