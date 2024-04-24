<?
if ($_GET["ajax_action"] == "update_element_eleve")
{
	CDebug::trace(0,"ajax_action:update_element_eleve");
	if ($classe = $_SESSION["classe"])
	{
		if ($eleve = $classe->eleveWithId($_GET["eleve"]))
		{
			if ($evaluation = $_SESSION["evaluation"])
			{
				if ($element = $evaluation->elementWithId($_GET["element"]))
				{
					if ($_GET["value"] != "")
						CEvaluationEleve::createObject(
								$element->id,
								$eleve->id,
								$_GET["value"],
								$element->mode,
								$element->valeur_max,
								$element->valeur_max);
					else CEvaluationEleve::deleteWithElementAndEleve($element->id,$eleve->id);

				} else CDebug::trace(0,"erreur : élèment inconnue");
			} else CDebug::trace(0,"erreur : évaluation inconnue");
		} else CDebug::trace(0,"erreur : élève inconnu");
	} else CDebug::trace(0,"erreur : pas de classe");
	die();
} else if ($_GET["ajax_action"] == "update_info_eleve")
{
	CDebug::trace(0,"ajax_action:update_element_eleve");
	if ($classe = $_SESSION["classe"])
	{
		if ($eleve = $classe->eleveWithId($_GET["eleve"]))
		{
			if ($evaluation = $_SESSION["evaluation"])
			{
				CEvaluationEleveInfo::createObject(
								$evaluation->id,
								$eleve->id,
								stripslashes($_GET["value"]));
			} else CDebug::trace(0,"erreur : évaluation inconnue");
		} else CDebug::trace(0,"erreur : élève inconnu");
	} else CDebug::trace(0,"erreur : pas de classe");
	die();
}


if ($_GET["export_feuille_saisie"])
{
	if ($evaluation = CEvaluation::objectWithid($_GET["evaluation_id"]))
	{
		
	}
}
define("DIV_BODY_CLASS","evaluation_notes");
include("evaluation_header.inc");

if ($_POST["update_notes"])
{
	$evaluation->deleteNotes();
	$evals = $_POST["evaluation"];
	$refs = $_POST["references"];
	if (!$refs) $refs = array();
	$hascomment = false;
	if ($_POST["commentaire"]) $hascomment = true;
	if (!$hascomment)
		CEvaluationEleveInfo::deleteWithEvaluation($evaluation->id);
	if ($evals ||$hascomment)
	{
		$elements = $evaluation->elements();
		$eleves = $classe->eleves();
		foreach($eleves as $eleve)
		{
			foreach($elements  as $element)
			{
				$idref = $eleve->id . "-".  $element->id;
				$val = $evals[$idref];
				$ref = $refs[$idref];
				if (strpos($val,",")) $val = str_replace(",",".",$val);
				if ($val)
				{
					if ($element->mode == "NOTES")
						CEvaluationEleve::createObject(
									$element->id,
									$eleve->id,
									$val,
									$element->mode,
									$element->valeur_max,
									$ref);
					else 
						CEvaluationEleve::createObject(
									$element->id,
									$eleve->id,
									$classe->codeUser2Interne($val),
									$element->mode,
									null,
									null);
				}
			}
			if ($hascomment)
			{
				if ($val = $_POST["commentaire"][$eleve->id])
					CEvaluationEleveInfo::createObject($evaluation->id,$eleve->id,stripslashes($val));
				else 
					CEvaluationEleveInfo::deleteWithEvaluationEleve($evaluation->id,$eleve->id);
			}
		}
	}
}
?>
	<script language="javascript">
	function setEvalValue(obj)
	{
		var par = obj.parentNode;
		var i=0;
		var codes_map = <?= json_encode($classe->codesUserToInternalMap()) ?>;
		for(i=0; i < par.childNodes.length ; i++ )
		{
			par.childNodes[i].className = "";
		}
		var input = document.getElementById(par.getAttribute('TARGET_ID'));
		if (input != null)
		{
			if (input.value == obj.getAttribute("VAL"))
			{
				input.value = '';
			} else {
				input.value = obj.getAttribute("VAL");
				obj.className = "selected";
			}
			updateValue(input.readAttribute("ELEMENT"),input.readAttribute("ELEVE"),codes_map[input.value]);
		}
	}
	function setModeSaisie(mode)
	{
		if (mode == "SOURIS")
		{
			$$("input.SAISIE_TEXTE").each(function(n) {n.hide();});
			$$("div.standard_note").each(function(n) {n.show();});
		} else 
		{
			$$("input.SAISIE_TEXTE").each(function(n) {n.show();});
			$$("div.standard_note").each(function(n) {n.hide();});
		}
	}
	function check_value(input,event)
	{
		var val = input.value.toLowerCase();
		input.value = val;
		var codes = <?= json_encode($classe->codes()) ?>;
		var codes_map = <?= json_encode($classe->codesUserToInternalMap()) ?>;
		if ((codes.indexOf(val) != -1)  || (val == ""))
		{
			var internal_val = "";
			if (val != "") internal_val = codes_map[val];
			input.value = val;
			updateValue(input.readAttribute("element"),input.readAttribute("eleve"),internal_val);
		}
		else {
			Event.stop(event);
			alert("Attention seul les valeurs suivantes sont autorisées : " + codes);
			input.value = "";
		}
	}
	
	function updateValue(element,eleve,value)
	{
		new Ajax.Request("export.php",{
				method: "get",
				parameters : {
					"module" : "evaluation_notes",
					"ajax_action" : "update_element_eleve",
					"element": element,
					"eleve" : eleve,
					"value" : value
				}
		});
	}
	function updateInfo(elem)
	{
		updateInfoValue(elem.readAttribute("evaluation"),elem.readAttribute("eleve"),elem.value);
	}
	function updateInfoValue(eval,eleve,value)
	{
		new Ajax.Request("export.php",{
				method: "get",
				parameters : {
					"module" : "evaluation_notes",
					"ajax_action" : "update_info_eleve",
					"evaluation": eval,
					"eleve" : eleve,
					"value" : value
				}
		});
	}
	</script>
	<form method="post" action="?module=evaluation_notes&mode_eleve=<?= $GET["mode_eleve"] ?>"> 
		<h2>Saisie des évaluations</h2>
		Mode de saisie : <select onchange="setModeSaisie(this.value)">
			<option value="SOURIS">souris</option>
			<option value="TEXTE">texte</option>
		</select>
		
		<?
		if (!$GET["mode_eleve"])
		{
		
		$eleves = CEleve::objectsForClasse($classe->id); 
		$elements = $evaluation->elements();
		$mds = CEvaluation::modesCalcul();
		$notes = $evaluation->notesByEleveElements();
		if (count($eleves) == 0)
		{ ?>
		<p class="info">Vous devez ajouter des élèves afin de pouvoir saisir leurs évaluations</p><? }
		else {
		?>
		<table class="alternate" style="width: 100%">
			<thead>
				<tr>
					<th>Elément évalué :</th>
					<? 
					foreach($elements as $c) { 
						$comp = $c->progression();	?>
					<th style="min-width: 60px; " title="<?= $comp->intitule ?>"><?= $c->description ?></th><? } ?>
					<th>Commentaires</th>
				</tr>
			</thead>
			<tbody><?
				foreach($eleves as $e) {
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td><?= $e->nom ?> <?= $e->prenom ?></td>
					<?
					foreach($elements as $c) {
						$idref = $e->id . "-".  $c->id;
						$note = $notes[$idref];
						
						if ($c->mode == "NOTES")
							printf("<td style='width: 90px'>");
						else 
							printf("<td>");
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $c->valeur_max;
						}
						if ($c->mode == "STANDARD") { ?>
							<input type="text" onblur="check_value(this,event)" name="evaluation[<?= $idref ?>]" element="<?= $c->id ?>" eleve="<?= $e->id ?>" id="<?= $idref ?>" size="1" maxlength="1" value="<?= $classe->userCode($val) ?>" class="SAISIE_TEXTE" style="display: none">
							<div class="standard_note" target_id="<?= $idref ?>">
								<div title="<?= $classe->a_title() ?>"  onclick="setEvalValue(this)" val="<?= $classe->a_code() ?>" class="<? if ($val == 'a') echo "selected"; ?>"><?= strtoupper($classe->a_code()) ?></div>
								<div title="<?= $classe->b_title() ?>"  onclick="setEvalValue(this)" val="<?= $classe->b_code() ?>" class="<? if ($val == 'b') echo "selected"; ?>"><?= strtoupper($classe->b_code()) ?></div>
								<div title="<?= $classe->c_title() ?>"  onclick="setEvalValue(this)" val="<?= $classe->c_code() ?>" class="<? if ($val == 'c') echo "selected"; ?>"><?= strtoupper($classe->c_code()) ?></div>
								<div title="<?= $classe->z_title() ?>"  onclick="setEvalValue(this)" class="<? if ($val == 'x') echo "selected"; ?>" val="<?= $classe->z_code() ?>"><?= strtoupper($classe->z_code()) ?></div>
							</div><? }
						else { ?>
							<input type="text" size="5" class="numberformat" onblur="setEvalValue(this)" name="evaluation[<?= $idref ?>]" value="<?= $val ?>">/<input type="text" class="numberformat" name="references[<?= $idref ?>]" size="3" value="<?= $ref ?>"><? } ?>
						</td><? } ?>
					<td >
						<input type="text"
								style="width: 400px; border : 1px solid #F0F0F0"
								evaluation="<?= $evaluation->id ?>"
								eleve="<?= $e->id ?>"
								name="commentaire[<?= $e->id ?>]" value="<?= CEvaluationEleveInfo::valueForEleveEvaluation($e->id,$evaluation->id) ?>"
								onblur="updateInfo(this);" >
					</td>
				</tr><? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table>
		<input type="submit" name="update_notes" value="Enregister">
		<? }} else { ?>
		<? } ?>
	</form>
</div>
