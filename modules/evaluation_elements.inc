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

if ($_POST["update"] || $_POST["ajout_progression"] || $_POST["delete_progression"])
{

	if ($_POST["delete_progression"])
	{
		foreach($_POST["delete_progression"] as $op)
		{
			CElement::deleteWithId($op);
		}
	}
	
	if ($_POST["ajout_progression"])
	{
		CElement::createObject(
			$evaluation->id,
			$_POST["add_progression"],
			$_POST["add_mode"],
			$_POST["add_valeur_max"],
			$_POST["add_description"],
			$_POST["seuil_a"],
			$_POST["seuil_b"]
			);
			//			$evaluation_id,$progression_id,$mode="STANDARD",$valeur_max="",$desc="")
	}
}
if ($_POST["up_index"])
{
	foreach($_POST["up_index"] as $k => $bob)
		if ($o = CElement::objectWithId($k)) $o->up();
}
if ($_POST["down_index"])
{
	foreach($_POST["down_index"] as $k => $bob)
		if ($o = CElement::objectWithId($k)) $o->down();
}
if ($_GET["up_index"])
{
	if ($o = CElement::objectWithId($_GET["up_index"])) $o->up();
}
if ($_GET["down_index"])
{
	if ($o = CElement::objectWithId($_GET["down_index"])) $o->down();
}

if ($_POST["update_element"])
{
	if ($element = CElement::objectWithId($_POST["update_element"]))
	{
		if ($element->evaluation == $evaluation->id)
		{
			$element->description= $_POST["element_description"];
			$element->progression = $_POST["element_progression"];
			$element->mode = $_POST["element_mode"];
			if ($element->mode == "NOTES")
			{
				$element->valeur_max = $_POST["element_valeur_max"];
				$element->seuil_a = $_POST["element_seuil_a"];
				$element->seuil_b = $_POST["element_seuil_b"];
			}
			else {
				$element->valeur_max = 0;
				$element->seuil_a = 0;
				$element->seuil_b = 0;
			}
			$element->update($DB);
		}
	}
}
?>
	<script language="javascript">
	var selectCodeObject = null;
	function selectSocle(cell,value,type)
	{
		selectCodeObject = cell;
		window.open('?no_body_header=1&module=socle_selector&type='+type+'&value='+value,'SOCLE_SELECTOR','width=920,menubar=no,status=no,toolbar=no,scrollbars=yes,resizable=yes');
	}
	function socle_selector_handler(code,type,title)
	{
		var prog_id = $(selectCodeObject).readAttribute("progression");
		var td_list = $$('td.code_' + type);
		for(var i = 0; i < td_list.length; i++ )
		{
			var td = td_list[i];
			if (td.readAttribute("progression") == prog_id)
			{
				td.update(code);
				td.writeAttribute("title",title);
			}
		}

		var req = '?module=progressions&ajax_action=update_socle_progression&type='+ type +'&id=' + prog_id +"&code_socle="+ code;
		new Ajax.Request(req,{ method: 'get'});
	}


	function check_add_form()
	{
		var selectProg = document.getElementById("add_progression");
		var option = selectProg.options[select.selectedIndex];

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
		if (selectProg.selectedIndex == 0)
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

	</script>
	<form method="post" action="?module=evaluation_elements"> 
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
		<p>Rq : cliquez sur la ligne pour modifier</p>
		<table class="liste alternate">
			<thead>
				<tr>
					<th></th>
					<th style="width: 300px;">Compétences de l'évaluation</th>
					<th style="width: 300px;">Compétences de ma progression</th>
					<th style="width: 25px;">Socle</th>
					<th style="width: 25px;">B2i</th>
					<th style="width: 25px;">Langue</th>
					<th style="width: 200px;">Mode d’évaluation</th>
					<th style="width: 25px; ">Ordre</th>
				</tr>
			</thead>
			<tbody><?
				$mds = CEvaluation::modesCalcul();
				$SOCLE_COMMUN = $classe->socle();
				$SOCLE_LANGUE = $classe->langue();
				$SOCLE_B2I = $classe->b2i();
				foreach($elems as $c) { 
					$prog = $c->progression();
				?>
				<tr>
					<td><input type=image src="img/delete.png" name="delete_progression[<?= $c->id ?>]" value="<?= $c->id ?>" onclick="return confirm('Attention : la suppression de l’élement d’évaluation entrainera la suppression des saisies d’évaluations attachées à cet élément !')">
					<td style="cursor: pointer" onclick="displayDialog('export.php?module=element&element_id=<?= $c->id ?>',400,200)"><?= $c->description ?></td>
					<td style="cursor: pointer" onclick="displayDialog('export.php?module=element&element_id=<?= $c->id ?>',400,200)">
						<?= $prog->intitule ?></td>
					<td  
					<? 
					if ($prog) {
						if ($prog->code_socle != "" && $SOCLE_COMMUN)
						{
							if ($cs = $SOCLE_COMMUN->getElementWithCode($prog->code_socle))
								printf(" title='%s'", addslashes($cs->info()));
							else echo "birarre";
						}
						
						 ?>onclick="selectSocle(this,'<?= $prog->code_socle ?>','socle')" progression="<?= $prog->id ?>" class="code_socle"<? } ?>>
						<? if ($prog) { if ($prog->code_socle != "") echo $prog->code_socle; else echo "<img src='img/search.gif'>"; } ?></td>

					<td  <? if ($prog) {
						if ($prog->code_b2i && $SOCLE_B2I)
						{
							if ($cs = $SOCLE_B2I->getElementWithCode($prog->code_b2i))
								printf(" title='%s'", addslashes($cs->info()));
						}
						 ?>onclick="selectSocle(this,'<?= $prog->code_b2i ?>','b2i')" progression="<?= $prog->id ?>" class="code_b2i"<? } ?>>
						<? if ($prog) { if ($prog->code_b2i != "") echo $prog->code_b2i; else echo "<img src='img/search.gif'>"; } ?></td>

					<td  <? if ($prog) {
						if ($prog->code_langue && $SOCLE_LANGUE)
						{
							if ($cs = $SOCLE_LANGUE->getElementWithCode($prog->code_langue))
								printf(" title='%s'", addslashes($cs->info()));
						}
						
						 ?>onclick="selectSocle(this,'<?= $prog->code_langue ?>','langue')" progression="<?= $prog->id ?>" class="code_langue"<? } ?>>
						<? if ($prog) { if ($prog->code_langue != "") echo $prog->code_langue; else echo "<img src='img/search.gif'>"; } ?></td>

					<td style="cursor: pointer" onclick="displayDialog('export.php?module=element&element_id=<?= $c->id ?>',400,200)"><?= $c->mode() ?></td>
					<td class="action" style="padding: 2px"><a href="?module=evaluation_elements&up_index=<?= $c->id ?>">↑</a>&nbsp;
						<a href="?module=evaluation_elements&down_index=<?= $c->id ?>">↓</a>
					</td>
				</tr><? } ?>
			</tbody>
		</table><? } ?>
		<br><br>
		<table class="control">
			<tbody>
				<tr>
					<th colspan="3" class="stdTh">Nouvelle compétence à évaluer :</th>
				</tr>
				<tr>
					<th>Compétence de l'évaluation :
					<th>Mode d’évaluation :</th>
				</tr>
				<tr>
					<td style="vertical-align: top">
						<textarea name="add_description" rows="3" style="width: 500px"></textarea>
					</td>
					<td style="vertical-align: top">
						<select name="add_mode" id="add_mode" onchange="selectModeEvaluation(this,$('add_valeur_max'))">
						<? foreach(CEvaluation::modesCalcul() as $k => $mc) { ?>
							<option value="<?= $k ?>"><?= $mc ?></option><? } ?>
						</select>
						<div id="add_valeur_max"  style="visibility: hidden">
						Total :  <input type="text" name="add_valeur_max" size="3"><br>
						Semble acquis si >= <input type="text" name="seuil_a" size="3"><br>
						En début d’acquisition si <= <input type="text" name="seuil_b" size="3"><br>
					</td>
				</tr>
				<tr>
					<th>Compétence correspondante dans ma progression :</th>
				</tr>
				<tr>
					<td style="vertical-align: top" colspan="2">
						<select name="add_progression" id="add_progression" Size="10" style="width: 700px;">
						<option value="">Choisir une compétence : </option>
							
							<? CProgression::asOptionsForClasse($classe->id); ?>
						</select><br>
						Rechercher : <input type="text" id="search_key" target="add_progression" onkeydown="if (event.keyCode == 13) {searchOption('search_key'); return false;};"/>
						<input type="button" value="rechercher" onclick="searchOption('search_key')" /><br>
						(Entre parenthèses le nombre de fois que ces compétences ont été évaluées, sinon pas d’indication)
					</td>
					<td style="vertical-align: top">
						<input type="submit" name="ajout_progression" onclick="return check_add_form()" value="Ajouter">
					</td>
				</tr>
			</tbody>
		</table>
	</form>
</div>
