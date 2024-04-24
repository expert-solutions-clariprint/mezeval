<?
if ($_GET["ajax_action"] == "update_progression_code")
{
	if ($classe = $_SESSION["classe"])
	{
		if ($prg = CProgression::objectWithIdAndClasse($_GET["progression"],$classe->id))
		{
			$prg->code = $_GET["code"];
			$prg->update();
		}
	}
	die();
}
if ($_GET["ajax_action"] == "update_progression_intitule")
{
	if ($classe = $_SESSION["classe"])
	{
		if ($prg = CProgression::objectWithIdAndClasse($_GET["progression"],$classe->id))
		{
			$prg->intitule = $_GET["intitule"];
			$prg->update();
		}
	}
	die();
}
if ($_GET["ajax_action"] == "update_periode_progression")
{
	if ($_GET["mode"] == "true")
		CProgressionPeriode::createForProgressionAndPeriode($_GET["progression"],$_GET["periode"]);
	else 
		CProgressionPeriode::deleteForProgressionAndPeriode($_GET["progression"],$_GET["periode"]);
	die();
}
if ($_GET["ajax_action"] == "update_socle_progression")
{
	if ($o=CProgression::objectWithId($_GET["id"]))
		{
			if ($_GET["type"] == "b2i")
				$o->code_b2i = $_GET["code_socle"];
			else  if ($_GET["type"] == "langue")
				$o->code_langue = $_GET["code_socle"];
			else $o->code_socle = $_GET["code_socle"];
			
			$o->update(); }
	die();
}


if ($_GET["export_mode"] == "pdf")
{
	if ($classe = $_SESSION["classe"])
	{
		$periodes = null;
		if ($_POST["option_periodes"] > 0)
		{
			$periodes = array(CPeriode::objectWithId($_POST["option_periodes"]));
		} else if ($_POST["option_periodes"] == 0)
		{
			$periodes = $classe->periodes();
		}
		$pdf = new CProgressionPdf($_POST["option_orientation"]);
		$pdf->init();
		if ($_POST["option_code_socle"]) $pdf->affiche_code_socle = 1;
		if ($_POST["option_code"]) $pdf->affiche_code = 1;
		$pdf->pdfClasse($classe,null,$periodes);
		$pdf->Output($classe->info() . ".pdf","D");
		die();
	}
} else if ($_GET["export_mode"] == "xml")
{
	if ($classe = $_SESSION["classe"])
	{
		if ($_GET["progression_id"])
		{
			CProgramme::exportClasse($_SESSION["classe"],$_GET["progression_id"]);
			die();
		} else if ($_POST["selection_progression"])
		{
			CProgramme::exportClasse($_SESSION["classe"],$_POST["selection_progression"]);
			die();
		} else {
			CProgramme::exportClasse($_SESSION["classe"]);
			die();
		}
	}
	die();
} else if ($_GET["export_mode"] == "csv" || $_GET["export_mode"] == "csvwin")
{
	if ($classe = $_SESSION["classe"])
	{
		if ($_GET["progression_id"])
		{
			CProgramme::exportClasseCSV($_SESSION["classe"],$_GET["progression_id"],1,($_GET["export_mode"] == "csvwin"));
			die();
		} else if ($_POST["selection_progression"])
		{
			CProgramme::exportClasseCSV($_SESSION["classe"],$_POST["selection_progression"],1,($_GET["export_mode"] == "csvwin"));
			die();
		} else {
			CProgramme::exportClasseCSV($_SESSION["classe"],null,1,($_GET["export_mode"] == "csvwin"));
			die();
		}
	}
	die();
}


include("classe_header.inc");

if ($_GET["drop"] && $_GET["on"])
{
	$drop = CProgression::objectWithIdAndClasse($_GET["drop"],$classe->id);
	$drag = CProgression::objectWithIdAndClasse($_GET["on"],$classe->id);
	if ($drop && $drag)
	{
		if (!$drag->hasParents($drop->id))
		{
			$drop->setParent($drag->id);
			 $_GET["scrollto"] = 'tr_progression_'. $drag->id;
		}
	}
}

if ($_POST["import_programme"] && $_FILES['programme_file'])
{
	if (strcasecmp(substr($_FILES['programme_file']['name'],-3),"csv") == 0 )
		CProgramme::importCSV($classe->id,$_FILES['programme_file']['tmp_name']);
	else if (strcasecmp(substr($_FILES['programme_file']['name'],-3),"xml") == 0 )
		CProgramme::importFromFileToClasse($_FILES['programme_file']['tmp_name'],$classe->id);
	else echo("Format de fichier inconnu !<br>");
}
if ($_POST["delete_progression"])
{
	foreach($_POST["delete_progression"] as $k => $c) CProgression::deleteObjectWithId($k);
}
if ($_POST["import_programme_officiel"])
{
	if (strcasecmp(substr($_POST['programme_object'],-3),"csv") == 0 )
		CProgramme::importCSV($classe->id,CProgramme::path($_POST['programme_object']));
	else if (strcasecmp(substr($_POST['programme_object'],-3),"xml") == 0 )
		CProgramme::importFromFileToClasse(CProgramme::path($_POST['programme_object']),$classe->id);
	else echo("Format de fichier inconnu !<br>");
	
//	CProgramme::importFromFileToClasse(CProgramme::path($_POST['programme_object']),$classe->id);
}
if ($_POST["suppression_element_coches"] && $_POST["selection_progression"])
{
	foreach($_POST["selection_progression"] as $pid) CProgression::deleteObjectWithId($pid);
		
} else if ($_POST["impression_element_coches"] && $_POST["selection_progression"])
{
	echo "Impression de";
	print_r($_POST["selection_progression"]);
}

if ($_GET["down"])
{
	CDebug::trace(0,"down");
	if ($p = CProgression::objectWithIdAndClasse($_GET["down"],$classe->id))
	{
		CDebug::trace(0,"progression order_index:". $p->order_index);
		if ($p->order_index > 1) $p->setOrderIndex($p->order_index - 1);
	} else	CDebug::trace(0,"progression ".$_GET["down"]."introuvable");
} else if ($_GET["up"])
{
	CDebug::trace(0,"up" . $_GET["up"]);
	if ($p = CProgression::objectWithIdAndClasse($_GET["up"],$classe->id))
	{
		CDebug::trace(0,"up" . $p->order_index );
		$max = CProgression::countForClasseAndParent($classe->id,$p->parentcp);
		CDebug::trace(0,"max:" . $max );
		if ($p->order_index < $max)
			$p->setOrderIndex($p->order_index +  1);
	}
}

global $PERIODES;
$PERIODES = $classe->periodes();

function showProgramme($classe_id,$parent = 0,$deep=1)
{
	global $PERIODES;

	if ($parent == 0)
		$subs = CProgression::parentsForClasse($classe_id);
	else 
		$subs = CProgression::objectForParent($parent);
	foreach($subs as $c) { ?>
<h<?= $deep ?>><?= $c->code ?> <?= $c->intitule ?></h<?= $deep ?>>
<p><?= $c->description ?></p>
<a href="?module=progression&progression_id=<?= $c->id ?>">modifier</a>
<div class="detail_<?= $deep ?>" id="<?= $c->id ?>" style="margin-left: <?= $deep ?>cm;">
	<?
	showProgramme($classe_id,$c->id,$deep + 1);
	?>
</div>
	<a href="?module=progression&nouvelle_progression=1&parentcp=<?= $c->id ?>">Nouveau...</a><? 
 	}
}

global $SOCLE_COMMUN;
$SOCLE_COMMUN = $classe->socle();
global $SOCLE_LANGUE;
$SOCLE_LANGUE = $classe->langue();
global $SOCLE_B2I;
$SOCLE_B2I = $classe->b2i();

class CClasseProgramme
{
	var $data = array();
	var $tree = array();
	var $count = array();
	var $count_non_evalue= 0;
	var $count_a_evaluer = 0;
	var $total_eval= 0;
	function objectWithClasse($classe_id)
	{
		$x = new CClasseProgramme;
		$x->initWithClasse($classe_id);
		return $x;
	}
	function initWithClasse($classe_id)
	{
		foreach(CProgression::parentsForClasse($classe_id) as $p)
			$this->addProgression($p);
	}
	function addProgression($p)
	{
		$count = 0;
		$key = sprintf("p%d",$p->parentcp);
		if (!$this->tree[$key]) $this->tree[$key] = array();
		$this->tree[$key][] = $p;
		$subs = CProgression::objectsForParent($p->id);
		if (count($subs) > 0)
		{
			foreach($subs as $sp)
			{
				$count += $this->addProgression($sp);
			}
		} else
		{
			$this->count_a_evaluer++;
			$count = CElement::countEvaluationsForProgression($p->id);
			$this->total_eval += $count;
			if ($count == 0) $this->count_non_evalue++;
		} 
		$this->count["p".$p->id] = $count;
		return $count;
	}
	function children($pid)
	{
		if ($arr = $this->tree["p".$pid]) return $arr;
		return array();
	}
	function countEval($pid)
	{
		return (int) $this->count["p".$pid];
	}
}

function loadProgrammeWithCount($classe_id,$parent = 0)
{
	if ($parent == 0)
		$subs = CProgression::parentsForClasse($classe_id);
	
}


function showProgrammeAsTable($prg,$classe_id,$parent = 0,$deep=1,$subs=null)
{
	global $SOCLE_COMMUN;
	global $SOCLE_LANGUE;
	global $SOCLE_B2I;
	$max_deep = 2;
	$hide_deep = ($deep > $max_deep);
	global $PERIODES;
	if ($parent == 0)
		$subs = $prg->children(0);;
	
	global $ALTERNATE;
	foreach($subs as $c) {
		$nochild = false;
		// $mysubs = CProgression::objectsForParent($c->id);
		$mysubs = $prg->children($c->id);
		if (count($mysubs) == 0) { $nochild= true; $cssclass = "nofolder"; }
		else if ($deep < $max_deep) $cssclass="openfolder";
		?>
<tr class="niveau<?= $deep ?> <?= $ALTERNATE ?> <?= $cssclass ?>" treeviewdeep="<?= $deep ?>" 
	<? if ($hide_deep) echo "style='display: none'"; ?>
	id="tr_progression_<?= $c->id ?>"
	progression='<?= $c->id ?>'
	onmouseover="addCssClass(this,'highlight');" onmouseout="rmCssClass(this,'highlight');">
	<td><input type="checkbox" class="selectbox" name="selection_progression[]" value="<?= $c->id ?>"></td>
	<td class="code"><?= $c->code ?></td>
	<td class="folderstatus"  <? if (!$nochild) { ?>onclick="switchTreeView(event,this.parentNode)"<? } ?>></td>
	<td <? if (!$nochild) { ?>onclick="switchTreeView(event,this.parentNode)" style="cursor: pointer" <? } ?>class="intitule icon"><div class="draggable_progression" p="<?= $c->id ?>" id="intitule_<?= $c->id ?>"><?= $c->intitule ?></div></td>
	<td class="code code_socle"
		onclick="selectSocle(this,'<?= $c->code_socle ?>','socle')"
		<? 
		if ($c->code_socle != "" && $SOCLE_COMMUN) {
			if ($v = $SOCLE_COMMUN->getElementWithCode($c->code_socle))
			{
				printf(" title='%s' ",addslashes($v->info()));
			}
			else CDebug::trace(0,"code socle not found");
		} ?>
		progression="<?= $c->id ?>"
		><? if($cs = $c->code_socle) echo($cs); elseif ($nochild) echo("<img src='/img/search.gif' />") ?></td>

	<td class="code code_b2i" onclick="selectSocle(this,'<?= $c->code_b2i ?>','b2i')"
		<? 
		CDebug::trace(0,"search  :" . $c->code_b2i);
		if ($c->code_b2i != "" && $SOCLE_B2I) {
			if ($v = $SOCLE_B2I->getElementWithCode($c->code_b2i))
			{
				CDebug::trace(0,"ok :" . print_r($v,true));
				printf(" title='%s' ",addslashes($v->intitule));
			}
			else CDebug::trace(0,"code socle not found");
		} ?>
		progression="<?= $c->id ?>"><? if($cs = $c->code_b2i) echo($cs); elseif ($nochild) echo("<img src='/img/search.gif' />") ?></td>

	<td class="code code_langue" onclick="selectSocle(this,'<?= $c->code_langue ?>','langue')"
		<? 
		if ($c->code_langue != "" && $SOCLE_LANGUE) {
			if ($v = $SOCLE_LANGUE->getElementWithCode($c->code_langue))
			{
				printf(" title='%s' ",addslashes($v->info()));
			}
			else CDebug::trace(0,"code socle not found");
		} ?>
		
		progression="<?= $c->id ?>"><? if($cs = $c->code_langue) echo($cs); elseif ($nochild) echo("<img src='/img/search.gif' />") ?></td>
	<? $n_evals = $prg->countEval($c->id); ?>
	<td class="number"><?= $n_evals ?></td>
	<td><?
		if ($nochild && $n_evals)
			printf("<div onclick='showProgessionEleve(this,%d,null)' style='cursor: pointer'><img src='img/historique.gif'></div>",$c->id); 
		?>
	</td>
	
	<?
	/*
	if ($PERIODES) $periodes = CProgressionPeriode::arrayForProgression($c->id);
	else $periodes = array();
	foreach($PERIODES as $p) { ?>
	<td><input type="checkbox"
			class="periodeCheck"
			<? 
			if ($periodes[$p->id]) echo "checked";
			?>
			periode="<?= $p->id ?>"
			parent="<?= $parent ?>"
			name="periodes[<?= $c->id ?>][]"
			value="<?= $p->id ?>" progression="<?= $c->id ?>"
			onclick="onCheckPeriode(event)"
			onclick2="setProgressionPeriode(this)"></td><? }
		*/	
			 ?>

	<td class="action"><a href="?module=progression&progression_id=<?= $c->id ?>" id="progression_<?= $c->id ?>">modifer</a></td>
	<td class="action"><a href="?module=progression&nouvelle_progression=1&parentcp=<?= $c->id ?>">insérer</a></td>
	<td class="action" style="width: 30px; padding: 2px">
		<a href="?module=progressions&down=<?= $c->id ?>">↑</a>&nbsp;
		<a href="?module=progressions&up=<?= $c->id ?>">↓</a></td>
</tr>
	<? if (!$nochild) showProgrammeAsTable($prg,$classe_id,$c->id,$deep + 1,$mysubs); ?>
<? 
 	}
}


?>
<script type="text/javascript" charset="utf-8">

//	var print_ctrl = null;
	function showImport()
	{
		Control.Modal.open($('import'),{'closeOnClick':$('import').down('.close')});
	}
	function showPrint()
	{
		Control.Modal.open($('print'),{'closeOnClick':$('print').down('.close')});
	}

	function onCheckPeriode(event) {
		setProgressionPeriode(event.element(),event.shiftKey);
	}
	function setProgressionPeriode(input,propagate)
	{
		var prog = input.getAttribute("progression");
		var periode = input.value;
		var req = '?module=progressions&ajax_action=update_periode_progression&periode=' + periode +"&progression="+ prog + "&mode=" +  input.checked;
		new Ajax.Request(req,{ method: 'get'});
		if (propagate)
		{
			var select_rule = 'input[parent="' + prog + '"]';
			var childs = $('table_progressions').select(select_rule);
		
			var i = 0;
			for(i = 0; i < childs.length; i++)
			{
				var child = childs[i];
				if (child.getAttribute("periode") == periode)
				{
					child.checked = input.checked;
					setProgressionPeriode(child,true);
				}
			}
		}
	}
	var selectCodeObject = null;
	function selectSocle(cell,value,type)
	{
		selectCodeObject = cell;
		window.open('?no_body_header=1&module=socle_selector&type=' + type +'&value='+value,'SOCLE_SELECTOR','width=920,menubar=no,status=no,toolbar=no,scrollbars=yes,resizable=yes');
	}
	function socle_selector_handler(code,type,title)
	{
		$(selectCodeObject).update(code);
		id = $(selectCodeObject).readAttribute("progression");
		$(selectCodeObject).writeAttribute("title",title);
		var req = '?module=progressions&ajax_action=update_socle_progression&type='+type+'&id=' + id +"&code_socle="+ code;
		new Ajax.Request(req,{ method: 'get'});
	}
	
	function checkPeriode(evt)
	{
		var elem = Event.element(evt);
		var periode = elem.readAttribute("periode");
		var table = $("table_progressions");
		var inputs = table.select("input.periodeCheck");
		for(var i = 0; i < inputs.length; i++)
		{
			var input = inputs[i];
			if (input.readAttribute("periode") == periode)
			{
				input.checked = !event.shiftKey;
				setProgressionPeriode(input,false);
			}
		}
	}
</script>
<style>
	tr td.code
	{
		text-align: center;
		cursor: pointer;
	}
	div.widget
	{
		background-color:#fff;
		padding:10px;
		border:1px solid #333;
	}
	div#print,
	div#import
	{
		width: 700px;
	}
	div.widget img.close
	{
		float:right;
	}
	
</style>
<div id="body" class="progressions">
	<h1>Progressions</h1>
	<p class="notice">Remarque : Pour faciliter la répartition de vos compétences sur les différentes périodes, vous avez la possibilité de (dé)cocher plusieurs éléments de votre progression en un seul clic. Pour ce faire, maintenez la touche SHIFT (majuscule temporaire) enfoncée puis cliquez sur l'élément désiré; tous les sous-éléments seront alors (dé)cochés.</p>

	<form method="post" action="?module=progressions" enctype="multipart/form-data">
	<table class="programme liste alternate treeview" id="table_progressions" style="width: 100%">
		<thead>
			<tr>
				<th width="30px"></th>
				<th width="30px">code</th>
				<th width="20px"></th>
				<th >intitulé</th>
				<th width="50px">socle</th>
				<th width="50px">B2i</th>
				<th width="50px">langue</th>
				<th width="50px" colspan="2">nb. éval</th>
				<th colspan="3">
					<input type="button" name="some_name" value="Imprimer" onclick="showPrint()"/>
					<input type="button" name="some_name" value="Import/export" onclick="showImport()"/>
				</th>
			</tr>
			<tr class="niveau1" treeviewdeep="1">
				<td><img src="img/ok.png" onclick="$$('input.selectbox').each(function (obj) {obj.checked = true; } )"></td> 
				<td>
				<td class="code"></td>
				<td class="intitule"><input type="search" class="searchbox" target="table_progressions" target_cols="2 3" /></td>
				<td class="code"></td>	
				<td></td>
				<td></td>
				<td></td>
				<td></td>
				<? /* foreach($PERIODES as $p) { ?>
				<td><img src="img/ok.png" periode="<?= $p->id ?>" onclick="checkPeriode(event)"></td> 
				<? }  */?>
				<td class="action" colspan="4"><a href="?module=progression&nouvelle_progression=1&parentcp=0">ajouter</a></td>
			</tr>
		</thead>
		<tbody>
			<? 
			$prg = CClasseProgramme::objectWithClasse($classe->id);
			showProgrammeAsTable($prg,$classe->id) ?>
		</tbody>
		<tfoot>
		</tfoot>
	</table>
	<table>
		<tbody>
			<tr>
				<td>Actions sur les éléments cochés: <td>
				<td><input type="submit" name="suppression_element_coches" value="Suppression" onclick="return confirm('Attention ! la suppression d’une progression entraine la suppression des éléments d’évaluation liés à celle-ci ainsi que les saisies d’évaluation ci rapportant');">
				<td><input type="submit" name="export_element_coches" value="Exporter" onclick="this.form.action='index_export.php?module=progressions&export_mode=xml'; form.submit(); this.form.action='index.php?module=progressions';">
			</tr>
		</tbody>
	</table>
	<? if ($prg->count_a_evaluer) { ?>
	<table>
		<thead>
			<tr>
				<td>Nombre de progression à évaluer</td>
				<td>Nombre de progression évaluées</td>
				<td>Nombre de progression non évaluées</td>
				<td>Nombre d'évaluations</td>
				<td>Couverture (éval. par progressions)</td>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td style="text-align:center"><? printf("%d",$prg->count_a_evaluer); ?></td>
				<td style="text-align:center"><? printf("%d (%.1f%%)",($prg->count_a_evaluer - $prg->count_non_evalue),
																		100*($prg->count_a_evaluer - $prg->count_non_evalue) / $prg->count_a_evaluer
				 														); ?></td>
				<td style="text-align:center"><? printf("%d (%.1f%%)",$prg->count_non_evalue,
															100* $prg->count_non_evalue / $prg->count_a_evaluer);
				 										?></td>
				<td style="text-align:center"><?= $prg->total_eval ?></td>
				<td style="text-align:center"><? printf("%.2f", ($prg->total_eval / ($prg->count_a_evaluer - $prg->count_non_evalue))) ?></td>
			</tr>
		</tbody>
	</table><? } ?>
	<div class="widget" id="import" style="display:none">
		<img class="close" src="/img/delete.png"/>
		<h2>Importer / Exporter</h2>
		<table class="control">
			<thead>
				<tr>
					<th>Progressions officielles :</th>
					<th>À partir d’un fichier :</th>
					<th>Export :</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
						<select name="programme_object">
						<? foreach(CProgramme::objects() as $key => $p) {
							?><option value="<?= $p ?>"><?= $p ?></option><? 
						} ?>
						</select>
						<input type="submit" name="import_programme_officiel" value="Importer">
					</td>
					<td>
						<input type="file" name="programme_file">
						<input type="submit" name="import_programme" value="Importer">
						<br>Des fichiers de progressions sont <a  target="_blank" href="http://forum.mezeval.fr/viewforum.php?f=8">disponibles sur le forum.</a>.
					</td>
					<td>
						<input type="button" onclick="location.href='export.php?module=progressions&export_mode=xml'" name="export_xml" value="XML"> export au format XML (pour importation dans MeZéval)<br>
						<input type="button" onclick="location.href='export.php?module=progressions&export_mode=csv'" name="export_csv" value="CSV Standard"> export au format CSV (pour utilisation dans OpenOffice)<br>
						<input type="button" onclick="location.href='export.php?module=progressions&export_mode=csvwin'" name="export_csv" value="CSV Microsoft"> export au format CSV (pour utilisation dans Excel)<br>
					</td>
				</tr>
			</tbody>
		</table>
	</div>
	</form>
	<div class="widget" id="print" style="display:none">
		<img class="close" src="/img/delete.png"/>
		<h2>Impression</h2>
		<form method="POST" action="export.php?module=progressions&export_mode=pdf">
		<table class="control">
			<thead>
				<tr>
					<th>Affichage des périodes</th>
					<th>Orientation du document</th>
					<th>Options</th>
					<th></th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
						<select name="option_periodes">
							<option value="0">Toutes les périodes</option>
							<option value="-1">Sans</option>
							<? foreach($classe->periodes() as $p) { ?>
							<option value="<?= $p->id ?>"><?= $p->info() ?></option><? } ?>
						</select>
					</td>
					<td>
						<select name="option_orientation">
							<option value="P">Portrait (à la française)</option>
							<option value="L">Paysage (à l’italienne)</option>
						</select>
					</td>
					<td>
						<input type="checkbox" value="1" name="option_code_socle"> Socle<br>
						<input type="checkbox" value="1" name="option_code"> Code<br>
					</td>
					<td>
						<input type="submit" value="Export en fichier PDF (impression)">
					</td>
				</tr>
			</tbody>
		</table>
		</form>
	</div>
	<script type="text/javascript" charset="utf-8">
		function onDropHanle(dragged,dropped,event)
		{
			document.location="?module=progressions&drop=" + dragged.getAttribute("p") + "&on=" + dropped.getAttribute("p");
		}
		$$('div.draggable_progression').each(function (td) {
			td.observe('dblclick',edit_intitule);
			td.observe('click',function(evt) { evt.stop(); });
			Droppables.add(td, {accept: 'draggable_progression',
								onDrop: onDropHanle});
			new Draggable(td,{'revert': true,scroll: window, constraint: 'vertical'}); });

		$$('td.code').each(function(td){
				td.observe('dblclick',edit_code);
				});

		$$('td.intitule').each(function(td){
				td.observe('dblclick',edit_intitule);
				});

		function edit_intitule(evt)
		{
			var elem = Event.element(evt);
			while(elem.parentNode.getAttribute("progression") == null)
				elem = elem.parentNode;
			if (elem.onEdit) return;
			elem.onEdit = true;
			var pgid = elem.parentNode.getAttribute("progression");
			var div = $("intitule_" + pgid);
			var w = elem.getWidth() * 0.8;
			var input = new Element("INPUT",{"type" : "text"});
			input.div = div;
			div.hide();
			input.style.width = w + "px";
			input.value = div.innerHTML;
			input.oldValue = div.innerHTML;			
			elem.insert(input);
			input.focus();
			input.select();
			input.ok = false;
			input.observe('blur', function(evt) {
					var elem = Event.element(evt);
					if (elem.ok == false)
					{
						Event.stopObserving(elem,'keydown');
						Event.stopObserving(elem,'blur');
						elem.parentNode.onEdit = false;
						elem.remove();
						elem.div.show();
					}
					});
			input.observe('keydown', function(evt) {
				if (evt.keyCode == Event.KEY_RETURN || evt.keyCode == Event.KEY_TAB)
				{
					var elem = Event.element(evt);
					var div = elem.div;
					var val = elem.value;
					elem.parentNode.onEdit = false;
					div.update();
					div.update(val);
					elem.remove();
					div.show();
					Event.stopObserving(elem,'keydown');
					Event.stopObserving(elem,'blur');
					elem.ok= true;
					update_intitule(val,div.getAttribute("p"));
					
				} else if (evt.keyCode == Event.KEY_ESC)
				{
					var elem = Event.element(evt);
					var div = elem.div;
					var val = elem.oldValue;
					div.update();
					div.update(val);
					elem.parentNode.onEdit = false;
					elem.remove();
					div.show();
					
					elem.remove();
					elem.ok= true;
					Event.stopObserving(elem,'keydown');
					Event.stopObserving(elem,'blur');
					
				}
				 	});			
		}
		
		function update_intitule(val,pg_id)
		{
			new Ajax.Request("export.php",
				{
					method: 'get',
					parameters : {
						"module":"progressions",
						"ajax_action": "update_progression_intitule",
						"progression": pg_id,
						"intitule": val
					}
				});
		}
		
		function update_code(val,pg_id)
		{
			new Ajax.Request("export.php",
				{
					method: 'get',
					parameters : {
						"module":"progressions",
						"ajax_action": "update_progression_code",
						"progression": pg_id,
						"code": val
					}
				});
		}
		
		function edit_code(evt)
		{
			var elem = Event.element(evt);
			var w = elem.getWidth();
			var input = new Element("INPUT",{"type" : "text"});
			input.style.width = w + "px";
			input.value = elem.innerHTML;
			input.oldValue = elem.innerHTML;
			elem.update(input);
			input.focus();
			input.select();
			input.ok = false;
			input.observe('blur', function(evt) {
					var elem = Event.element(evt);
					if (elem.ok == false)
					{
						var elem = Event.element(evt);
						elem.parentNode.update(elem.oldValue);
						Event.stopObserving(elem,'keydown');
						Event.stopObserving(elem,'blur');
					}
					});
			input.observe('keydown', function(evt) {
				if (evt.keyCode == Event.KEY_RETURN || evt.keyCode == Event.KEY_TAB)
				{
					var elem = Event.element(evt);
					var td = elem.parentNode;
					var val = elem.value;
					td.update();
					td.update(val);
					Event.stopObserving(elem,'keydown');
					Event.stopObserving(elem,'blur');
					elem.ok= true;
					update_code(val,td.parentNode.getAttribute("progression"));
					
				} else if (evt.keyCode == Event.KEY_ESC)
				{
					var elem = Event.element(evt);
					var td = elem.parentNode;
					var val = elem.oldValue;
					td.update();
					td.update(val);
					elem.ok= true;
					Event.stopObserving(elem,'keydown');
					Event.stopObserving(elem,'blur');
					
				}
			});
		}
	</script>
</div>
