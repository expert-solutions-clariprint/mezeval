<?

if ($_GET["output_mode"] == "pdf")
{
	if ($type = $_GET["type"])
	{
		$eleves = $_SESSION["classe"]->eleves();
		foreach($eleves as $e)
		{
			if ($b = CBilanSocle::objectForEleve($e->id,$type))
			{
				$pdf = $b->pdf($pdf);
			}
		}
		if ($pdf)
		{
			$pdf->Output("bilan.pdf", "D");
			die();
		}
	}
	else 
	{
		$eleves = $_SESSION["classe"]->eleves();
		if ($periode = CPeriode::objectWithId($_GET["periode"]))
		{
			$pdf = null;
			foreach($eleves as $e)
			{
				if ($b = CBilanProgression::objectForEleveAndPeriode($e->id,$periode->id))
				{
					$pdf = $b->pdf($pdf);
				}
			}
			if ($pdf)
			{
				$pdf->Output("bilan.pdf", "D");
				die();
			}
		}
	}
}

include("classe_header.inc");
$PERIODES =  $classe->periodes();
?>
<div id="body" class="bilans">
	<form method="get">
		<input type="hidden" name="module" value="bilans"/>
		<table>
			<tbody>
				<tr>
					<td>
						<select name="bilan">
							<? foreach($classe->periodes() as $p) { ?>
								<option value="<?= $p->id ?>" <? if ($_GET["bilan"] == $p->id) echo "selected";?>>Bilan : <?= $p->info() ?></option><? } ?>
							<? if($classe->socle) { ?> 
								<option value="socle" <? if ($_GET["bilan"] == "socle") echo "selected";?>>Bilan de socle</option><? } ?>
							<? if($classe->socle_b2i) { ?> 
								<option value="socle_b2i" <? if ($_GET["bilan"] == "socle_b2i") echo "selected";?>>Brevet informatique & internet B2I</option><? } ?>
							<? if($classe->socle_langue) { ?> 
								<option value="socle_langue" <? if ($_GET["bilan"] == "socle_langue") echo "selected";?>>Bilan de langue (A1)</option><? } ?>
						</select>
					</td>
					<td>
						Mode :
						<select name="mode">
							<option value="" <? if ($_GET["mode"] == "") echo "selected"; ?>>Récapitulatif</option>
							<option value="progressions" <? if ($_GET["mode"] == "progressions") echo "selected"; ?>>Choix des progressions</option>
							<option value="saisie_eleve" <? if ($_GET["mode"] == "saisie_eleve") echo "selected"; ?>>Saisie par élève</option>
							<option value="saisie_programme" <? if ($_GET["mode"] == "saisie_programme") echo "selected"; ?>>Saisie par progression</option>
						</select>
					</td>
					<td>
						<input type="submit" value="choisir"/>
					</td>
				</tr>
			</tbody>
		</table>
	</form>
	<? if ($_GET["mode"] == "") { 
		include("bilan_progression_eleves.inc");
		?>
	<? } elseif ($_GET["mode"] == "progressions") { 
		include("bilan_progressions.inc");
 	} elseif ($_GET["mode"] == "saisie_eleve") {
		include("bilan.inc");
		 ?>
		Saisie par élève
	<? } elseif ($_GET["mode"] == "saisie_programme") { 
		include("bilan_p_saisie.inc");
		?>
		Saisie par progression
	<? } else { 
		
	} ?>
	<div id="PrintSheet" class="PrintSheet" style="display:none">
		
		<h2>Options d'impressions</h2>
		<?
			$options = CUserOption::arrayWithName("IMPRESSION","BILAN");
			if ($_POST["update_options"])
			{
				$options["colonne_precision"] = ($_POST["colonne_precision"]) ? 1 : 0;
				$options["couleurfond_details_evaluations"] = ($_POST["couleurfond_details_evaluations"]) ? 1 : 0;
				$options["ajout_page_blanche"] = ($_POST["ajout_page_blanche"]) ? 1 : 0;
				CUserOption::setValueForName($options,"IMPRESSION","BILAN");
			}
		?>
		<form method="post" >
			<table class="control">
				<tr>
					<td>
						<input type="checkbox" name="afficher_premiere_page" value="1" <? if ($options["afficher_premiere_page"]) echo "checked"; ?>/>	
						Première page : <select>
							<option value="0">Cadre d'appréciations de la période uniquement</option>
							<option value="1">les cadres d'appréciations de toutes les périodes</option>
						</select>
					</td>
				</tr>
				<tr>
					<td><input type="checkbox" name="colonne_precision" value="1" <? if ($options["colonne_precision"]) echo "checked"; ?>>Masquer les informations nominatives</td>
				</tr>
				<tr>
					<td><input type="checkbox" name="colonne_precision" value="1" <? if ($options["colonne_precision"]) echo "checked"; ?>>Afficher les précisions</td>
				</tr>
				<tr>
					<td><input type="checkbox" name="couleurfond_details_evaluations" value="1" <? if ($options["couleurfond_details_evaluations"]) echo "checked"; ?>>Couleur de fond sur les détails d'évaluations</td>
				</tr>
				<tr>
					<td><input type="checkbox" name="ajout_page_blanche" value="1" <? if ($options["ajout_page_blanche"]) echo "checked"; ?>>Ajouter une page blanche pour assurer un nombre pair</td>
				</tr>
				<tr>
					<td>
						<input type="button" name="update_options" value="enregistrer les options" />
						<input type="button" name="" onclick="doPrint()" value="Imprimer avec les options" />
					</td>
				</tr>
			</table>
		</form>
	</div>
	<script type="text/javascript" charset="utf-8">
		function printAllWithOptions()
		{
			alert("ici");
		}
		
		function showPrintEleve(bilan,periode,eleve)
		{
			$("PrintSheet").show();
		}
		function showPrintBilan(bilan,periode)
		{
			$("PrintSheet").show();
		}
		
		function doSavePrintOption()
		{
		}
		function doPrint()
		{
			alert("ici");
			$("PrintSheet").hide();
		}
	</script>
</div>
