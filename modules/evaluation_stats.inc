<?

define("DIV_BODY_CLASS","evaluation_notes");
include("evaluation_header.inc");
include(APP_PATH . "/libs/gchart/gChart2.php");

$OPTIONS = CUserOption::arrayWithName("EVALUATIONS_STATS");
if ($_GET["nouveau_trie"])
{
	$OPTIONS["mode_affichage"] = $_GET["mode_affichage"];
	$OPTIONS["mode_tri_elements"] = $_GET["mode_tri_elements"];
	$OPTIONS["mode_tri_eleves"] = $_GET["mode_tri_eleves"];
	$OPTIONS["mode_masque_a"] =  ($_GET["mode_affiche_a"] != "1");
	$OPTIONS["mode_masque_c"] =  ($_GET["mode_affiche_c"] != "1");
	$OPTIONS["mode_masque_b"] =  ($_GET["mode_affiche_b"] != "1");
	$OPTIONS["mode_masque_x"] =  ($_GET["mode_affiche_x"] != "1");
	$OPTIONS["mode_masque_"] =  ($_GET["mode_affiche_"] != "1");
	
	CUserOption::setValueForName($OPTIONS,"EVALUATIONS_STATS");
}
class CTriController
{
	var $options;
	var $values;
	function objectWithOptions($opts)
	{
		$x = new CTriController();
		$x->options = $opts;
		return $x;
	}
	function trie_elements($a,$b)
	{
		if (!$this->options["mode_tri_elements"])
			return $a->order_index - $b->order_index;
		else if ($this->options["mode_tri_elements"] == "alpha")
			return strcmp($a->description,$b->description);
		else 
		{
			$va = $this->values[$a->id];
			$vb = $this->values[$b->id];
			if ($this->options["mode_tri_elements"] == "abcx" || $this->options["mode_tri_elements"] == "ab cx")
			{
				if ($va == $vb) return strcmp($a->description,$b->description);
				else return strcmp($va,$vb);
			} else {// if ($this->options["mode_tri_elements"] = "(C + X) (A + B)")
				if ($va == $vb) return strcmp($a->description,$b->description);
				else return strcmp($vb,$va);
			}
		}
	}
	function trie_elemements_cumul($a,$b)
	{
		if (!$this->options["mode_tri_elements"])
			return $a->order_index - $b->order_index;
		else if ($this->options["mode_tri_elements"] == "alpha")
			return strcmp($a->description,$b->description);
		else {
			$aa = $this->values[$a->id]["a"];
			$ab = $this->values[$a->id]["b"];
			$ac = $this->values[$a->id]["c"];
			$ax = $this->values[$a->id]["x"];
			$at =  $ax + $ac + $ab + $aa;
			$ba = $this->values[$b->id]["a"];
			$bb = $this->values[$b->id]["b"];
			$bc = $this->values[$b->id]["c"];
			$bx = $this->values[$b->id]["x"];
			$bt =  $bx + $bc + $bb + $ba;
			if ($at)
			{
				$aa = $aa / $at;
				$ab = $ab / $at;
				$ac = $ac / $at;
				$ax = $ax / $at;
			}
			if ($bt)
			{
				$ba = $ba / $bt;
				$bb = $bb / $bt;
				$bc = $bc / $bt;
				$bx = $bx / $bt;
			}
			if ($this->options["mode_tri_elements"] == "abcx")
			{
				if ($aa == $ba) 
				{
					if ($ab == $bb)
					{
						if ($ac == $bc)
						{
							if ($ax == $bx) return  strcmp($a->description,$b->description);
						} else return ($ac > $bc);
					} else return ($ab == $bb);
				} else return ($aa == $ba);
			} else {
				$a1 = $aa + $ab;
				$a2 = $ac + $ax;
				$b1 = $ba + $bb;
				$b2 = $bc + $bx;
				if  ($this->options["mode_tri_elements"] == "ab cx")
				{	
					if ($a1 == $b1) return ($a2 < $b2);
					else return ($a1 < $b1);					
				} else 
				{
					if ($a2 == $b2) return ($a1 < $b1);
					else return ($a2 < $b2);
				}
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
		for(i=0; i < par.childNodes.length ; i++ )
		{
			par.childNodes[i].className = "";
		}
		obj.className = "selected";
		var input = document.getElementById(par.getAttribute('TARGET_ID'));
		if (input != null)
		{
			input.value = obj.getAttribute("VAL");
		}
	}
	</script>
		<?
		$mode_affichage=$OPTIONS["mode_affichage"];
		$modes_affichage = array(
						"" => "Global : par critères d’évaluation",
//						"competences_gobal" => "Global : par compétences",
						"individuel" => "Individuel : par critères d’évaluation"
//						"competences_individuel" => "Individuel : par compétences"
						);
		$modes_tri_elements = array(
								"" => "Originel",
								"alpha" => "alphabétique",
								"abcx" => sprintf("%s %s %s %s",$classe->a_code(),$classe->b_code(),$classe->c_code(),$classe->z_code()), //"A B C X",
								"ab cx" => sprintf("(%s + %s)  (%s + %s  )",$classe->a_code(),$classe->b_code(),$classe->c_code(),$classe->z_code()), //"(A + B) (C + X)",
								"cx ab" => sprintf("(%s + %s) (%s +  %s)",$classe->c_code(),$classe->z_code(),$classe->a_code(),$classe->b_code())); //"(C + X) (A + B)");
		$mode_tri_elements = $OPTIONS["mode_tri_elements"];
		$mode_tri_eleves = $OPTIONS["mode_tri_eleves"];
		$tri_controller = CTriController::objectWithOptions($OPTIONS);
		?>
		<div class="control">
		<form method="GET">
		<input type="hidden" name="module" value="evaluation_stats">
		<table>
			<thead>
				<tr>
					<th>Affichage :</th>
					<th>Tri des éléments ou progressions</th>
<!--					<th>Tri des élèves</th> -->
					<th>Options affichage</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td><select name="mode_affichage">
							<? foreach($modes_affichage as $k => $m) { ?>
							<option value="<?= $k ?>" <? if ($k == $mode_affichage) echo "selected"; ?>><?= $m ?></option><? } ?>
						</select>
					</td>
					<td>
						<select name="mode_tri_elements">
							<? foreach($modes_tri_elements as $k => $m) { ?>
							<option value="<?= $k ?>" <? if ($k == $mode_tri_elements) echo "selected"; ?>><?= $m ?></option><? } ?>
						</select>
					</td>
<!--					<td>
						<select name="mode_tri_eleves">
						</select>
					</td> -->
					<td>
						<input type="checkbox" value="1" name="mode_affiche_a" <? if (!$OPTIONS["mode_masque_a"]) echo "checked"; ?>> <?= $classe->a_code() ?> &nbsp;&nbsp;<input type="checkbox" value="1" name="mode_affiche_b" <? if (!$OPTIONS["mode_masque_b"]) echo "checked"; ?>> <?= $classe->b_code() ?> 
						<input type="checkbox" value="1" name="mode_affiche_c" <? if (!$OPTIONS["mode_masque_c"]) echo "checked"; ?>> <?= $classe->c_code() ?> &nbsp;&nbsp; <input type="checkbox" value="1" name="mode_affiche_x" <? if (!$OPTIONS["mode_masque_x"]) echo "checked"; ?>> <?= $classe->z_code() ?><br>
						<input type="checkbox" value="1" name="mode_affiche_" <? if (!$OPTIONS["mode_masque_"]) echo "checked"; ?>> non évalué
					</td>
					<td>
						<input type=submit name="nouveau_trie" value="trier">
					</td>	
				</tr>
			</tbody>
		</table>
		</form>
		</div>
		<?
		if ($mode_affichage== "competences_gobal")
		{
			
			
		} else if ($mode_affichage == "individuel")
		{ 
			$eleves = CEleve::objectsForClasse($classe->id); 
			$elements = $evaluation->elements();
			$mds = CEvaluation::modesCalcul();
			$notes = $evaluation->notesByEleveElements();
			$MODEGRAPIQUE = "VERTICAL";
			if ($OPTIONS["mode_tri_elements"] == "ab cx" || $OPTIONS["mode_tri_elements"] == "cx ab")
				$MODEGRAPIQUE = "VERTICALCUMUL";

			?><h2>Résultats individuels</h2><?
			foreach($eleves as $eleve) {
				$comment = CEvaluationEleveInfo::valueForEleveEvaluation($eleve->id,$evaluation->id);
		?>
		<h3><?= $eleve->nom ?> <?= $eleve->prenom ?></h3>
		<?  if ($comment) { printf("<p>%s</p>",$comment);} ?>
		<table>
			<thead>
				<tr>
					<th>Compétences évaluées</th>
					<th colspan="2" style="width: 240px">évaluation</th>
					<th>Cumuls</th>
				</tr>
			</thead>
			<tbody><?
				$first_row =true;
				$vals = array();
				$ovals = array();
				foreach($elements as $c) {
					$v =CEvaluationEleve::valeurForEleveAndElement($eleve->id,$c->id);
					$v = $c->getEvaluation($v);
					$vals[$c->id] = $v;
					$ovals[] = $v;
				}
				$tri_controller->values = $vals;
				usort($elements, array($tri_controller,"trie_elements"));
				foreach($elements as $element) {
//						$element = $element_val[0];
						$prog = $element->progression($DB);
						$v = $vals[$element->id];
						if (!$OPTIONS["mode_masque_$v"])
						{
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td title="<? if ($prog) echo addslashes($prog->info()); else echo("-"); ?>"><?= $element->description ?></td>
					<?
						$idref = $e->id . "-".  $element->id;
						$note = $notes[$idref];
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $element->valeur_max;
						}
						$stat_width = 100;
//						if ($c->mode == "STANDARD") {
						if (true) {
							
							switch ($v)
							{
								case "a":
									$bgcolor = $classe->a_color();
									$txt = sprintf("%s (%s)",$classe->a_title(),$classe->a_code());
									break;
								case "b":
									$bgcolor = $classe->b_color();
									$txt = sprintf("%s (%s)",$classe->b_title(),$classe->b_code());
//									$txt = "A renforcer (B)";
									break;
								case "c":
									$bgcolor = $classe->c_color();
									$txt = sprintf("%s (%s)",$classe->c_title(),$classe->c_code());
//									$txt = "En début d’acquisition (C)";
									break;
								case 'x':
									$bgcolor = $classe->z_color();
									$txt = sprintf("%s (%s)",$classe->z_title(),$classe->z_code());
//									$txt = "Absence de réponse (X)";
									break;
								default: 
									$bgcolor = "#FFFFFF";
									$txt = "non évalué";
							}
							if ($element->mode == "NOTES")
							{
								$vo = CEvaluationEleve::objectForEleveAndElement($eleve->id,$element->id);
								$txt .= sprintf(" (%s/%s)",$vo->valeur,$element->valeur_max);
							}
						?>
							<td title="<?= $txt ?>" style="width: 20px; background-color: <?= $bgcolor ?>;"></td>
							<td title="<?= $txt ?>"><?= $txt ?></td> 							<? } 
						else
						{
							
							$v = CEvaluationEleve::objectForEleveAndElement($eleve->id,$element->id);
							$val = $element->getEvaluation($v->valeur);
							?>
							<td colspan="6"><?= $v->valeur ?> / <?= $element->valeur_max ?> (<?= $val ?>) </td><?
						}
						if ($first_row)
						{
							$a=0;
							$b=0;
							$c=0;
							$z=0;
							
							foreach($elements as $e ) {
								$v = $vals[$e->id];
								if ($v == "a") $a++;
								else if ($v == "b") $b++;
								else if ($v == "c") $c++;
								else if ($v == "x") $z++;
							}
							$first_row = false;
							$graph_height = 15 * count($elements);
							?><td rowspan="<?= count($elements) + 1 ?>" style="padding-left: 10px">
								<img src="graph_abc.php?mode=<?= $MODEGRAPIQUE ?>&width=200&height=<?= $graph_height ?>&a=<?= $a ?>&b=<?= $b ?>&c=<?= $c ?>&z=<?= $z ?>&<?= $classe->url_colors() ?>">
							</td><? 
						} ?>
				</tr><? }} ?>
				<tr><td>
			</tbody>
			<tfoot>
			</tfoot>
		</table><hr><? }
		} else if ($mode_affichage == "competences_individuel")
		{
			$eleves = CEleve::objectsForClasse($classe->id); 
			$elements = $evaluation->elements();
			
			
		} else if ($mode_affichage == "")
		{
		$eleves = CEleve::objectsForClasse($classe->id); 
		$elements = $evaluation->elements();
		$mds = CEvaluation::modesCalcul();
		$notes = $evaluation->notesByEleveElements();

		$tot_eleve = count($eleves);
		?>
		<table>
			<tr>
				<td>
		<h2>Résultats globaux </h2>
		<? 
			$a = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'a');
			$b = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'b');
			$c = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'c');
			$z = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'x');
			$MODEGRAPIQUE = "VERTICAL";
			if ($OPTIONS["mode_tri_elements"] == "ab cx" || $OPTIONS["mode_tri_elements"] == "cx ab")
				$MODEGRAPIQUE = "VERTICALCUMUL";
			?><img src="graph_abc.php?mode=<?= $MODEGRAPIQUE ?>&width=400&height=200&a=<?= $a ?>&b=<?= $b ?>&c=<?= $c ?>&z=<?= $z ?>&<?= $classe->url_colors() ?>">
		<h2>Détails</h2>
		<table>
			<thead>
				<tr>
					<th>Critères évalués</th>
					<th>Résultats</th>
				</tr>
			</thead>
			<tbody><?
					$first_row = true;
				$tot_a = 0;
				$tot_b = 0;
				$tot_c = 0;
				$tot_z = 0;
				$values = array();
				foreach($elements as $c) {
					$values[$c->id] =
						array("a" => CEvaluationEleve::countForElementAndNiveau($c->id,'a'),
							"b" => CEvaluationEleve::countForElementAndNiveau($c->id,'b'),
							"c" => CEvaluationEleve::countForElementAndNiveau($c->id,'c'),
							"x" => CEvaluationEleve::countForElementAndNiveau($c->id,'x'));
				}
				
				$tri_controller->values = $values;
				
				usort($elements,array($tri_controller,"trie_elemements_cumul"));
				foreach($elements as $c) {
					$desc = $c->description;
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td><?= $c->description ?></td>
					<?
						$idref = $e->id . "-".  $c->id;
						$note = $notes[$idref];
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $c->valeur_max;
						}
						$stat_width = 100;
						if (true) { // $c->mode == "STANDARD") {
							$n_a = $values[$c->id]["a"]; //CEvaluationEleve::countForElementAndNiveau($c->id,'a');
							$n_b = $values[$c->id]["b"]; //CEvaluationEleve::countForElementAndNiveau($c->id,'b');
							$n_c = $values[$c->id]["c"]; //CEvaluationEleve::countForElementAndNiveau($c->id,'c');
							$n_x = $values[$c->id]["x"]; //CEvaluationEleve::countForElementAndNiveau($c->id,'x');
							$tot = $tot_eleve; // $x + $z + $c + $b + $a;
							$graph_data = array("a" => $n_a, "b" => $n_b, "c" => $n_c, "x" => $n_x, "tot" => $tot);
						?>
							<td><img src="graph_abc.php?mode=<?= $MODEGRAPIQUE ?>&width=200&height=50&a=<?= $n_a ?>&b=<?= $n_b ?>&c=<?= $n_c ?>&z=<?= $n_x ?>&<?= $classe->url_colors() ?>">
							<? } ?>
				</tr><? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table>

			<td style="vertical-align: top">
				<?
				$aps = CEvaluationEleveInfo::valuesForEvaluationByEleve($evaluation->id);
				if (count($aps) > 0)
				{ ?>
				<h2>Appréciations par élèves</h2>
				
				<table class="alternate">
					<thead>
						<tr>
							<td>Élèves</td>
							<td>Appréciations</td>
						</tr>
					</thead>
					<tbody>
						<?
						foreach($classe->eleves() as $e )
						{
							if ($com = $aps[$e->id])
							{ ?>
							<tr>
								<td><?= $e->prenom ?> <?= $e->nom ?>
								<td><?= $com ?></td>
							</tr>
							<?
							}
						}
						?>
					</tbody>
				</table><?
				} ?>
			 
			</td>
		</tr>
		</table>
		<? }
		else {
		$eleves = CEleve::objectsForClasse($classe->id); 
		$elements = $evaluation->elements();
		$mds = CEvaluation::modesCalcul();
		$notes = $evaluation->notesByEleveElements();

		$tot_eleve = count($eleves);
		?>
		
		<table>
			
			<thead>
				<tr>
					<th>Critères évalués</th>
					<th>Semble acquis (A)</th>
					<th>A renforcer (B)</th>
					<th>En début d’acquisition (C)</th>
					<th>Absence de réponse (X)</th>
					<th>Cumul</th>
				</tr>
			</thead>
			<tbody><?
					$first_row = true;
				$tot_a = 0;
				$tot_b = 0;
				$tot_c = 0;
				$tot_z = 0;
				$values = array();
				foreach($elements as $c) {
					$values[$c->id] =
						array("a" => CEvaluationEleve::countForElementAndNiveau($c->id,'a'),
							"b" => CEvaluationEleve::countForElementAndNiveau($c->id,'b'),
							"c" => CEvaluationEleve::countForElementAndNiveau($c->id,'c'),
							"x" => CEvaluationEleve::countForElementAndNiveau($c->id,'x'));
				}
				
				$tri_controller->values = $values;
				
				usort($element,array($tri_controller,"trie_elemements_cumul"));
				foreach($elements as $c) {
					$desc = $c->description;
				?>
				<tr  onmouseover="style.backgroundColor='#F0F0F0';" onmouseout="style.backgroundColor='#FFFFFF';">
					<td><?= $c->description ?></td>
					<?
						$idref = $e->id . "-".  $c->id;
						$note = $notes[$idref];
						if ($note) {
							$val =  $note->valeur;
							$ref = $note->valeur_ref;
						} else {
							$val = "";
							$ref = $c->valeur_max;
						}
						$stat_width = 100;
						if (true) { // $c->mode == "STANDARD") {
							$n_a = CEvaluationEleve::countForElementAndNiveau($c->id,'a');
							$n_b = CEvaluationEleve::countForElementAndNiveau($c->id,'b');
							$n_c = CEvaluationEleve::countForElementAndNiveau($c->id,'c');
							$n_x = CEvaluationEleve::countForElementAndNiveau($c->id,'x');
							$graph_data = array("a" => $n_a, "b" => $n_b, "c" => $n_c, "x" => $n_x, "tot" => $tot);
						?>
							<td title="Semble acquis : <?= "$n_a  / $tot" ?>"><img src="graph_percent.php?title=<?= urlencode("$n_a  / $tot") ?>&value=<?= $n_a ?>&total=<?= $tot ?>&color=<?= urlencode($classe->a_color()) ?>"></td>
							<td title="A renforcer : <?= "$n_b  / $tot" ?>"><img src="graph_percent.php?title=<?= urlencode("$n_b  / $tot") ?>&value=<?= $n_b ?>&total=<?= $tot ?>&color=<?= urlencode($classe->b_color()) ?>"></td>
							<td title="En début d’acquisition : <?= "$n_c  / $tot" ?>"><img src="graph_percent.php?title=<?= urlencode("$n_c  / $tot") ?>&value=<?= $n_c ?>&total=<?= $tot ?>&color=<?= urlencode($classe->c_color()) ?>"></td>
							<td title="Non répondu : <?= "$n_x  / $tot" ?>"><img src="graph_percent.php?title=<?= urlencode("$n_x  / $tot") ?>&value=<?= $n_x ?>&total=<?= $tot ?>&color=<?= urlencode($classe->z_color()) ?>"></td>
							<? }
						if ($first_row) {
							$a = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'a');
							$b = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'b');
							$c = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'c');
							$z = CEvaluationEleve::countForEvaluationAndNiveau($evaluation->id,'x');
							$first_row = false;
							$graph_height = 25 * count($elements);
							?><td rowspan="<?= count($elements) ?>">
								<img src="graph_abc.php?mode=VERTICAL&width=200&height=<?= $graph_height ?>&a=<?= $a ?>&b=<?= $b ?>&c=<?= $c ?>&z=<?= $z ?>&<?= $classe->url_colors() ?>">
							</td><?
								
						} ?>

				</tr><? } ?>
			</tbody>
			<tfoot>
			</tfoot>
		</table>
		<? } ?>
</div>
