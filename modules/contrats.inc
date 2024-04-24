<?

if ($_GET["bon_commande_pdf"])
{
	if ($contrat = CContrat::objectWithId($_GET["bon_commande_pdf"]))
	{
		include(APP_PATH . "/bons_commande/" . $contrat->modele);
		die();
	}
}
$etablissement = CEtablissement::uniqueObject();

if ($_POST["print_bc"])
{
	if ($contrat =  $etablissement->contratOuvert())
	{
		$contrat->items = unserialize(urldecode($_POST["bc_items"]));
		$pdf = $contrat->pdfBonDeCommande();
		$pdf->Output("bon_commade_mezeval.pdf","D");
//		$contrat->pdfBonDeCommande();
		die();
	}
}


CTemplate::draw_main_menu("contrats");


if ($etablissement)
{
	?>
<script language="javascript">
	function calculate(form)
	{
		if (form.nb_classes.value != '')
		{
			var tot = 29 * parseInt(form.nb_classes.value);
			form.montant.value = tot.toFixed(2);
		}
	}
	function imprimer_bon_commande()
	{
		if (parseInt($('nb_classes').value) > 0)
		{
		} else 
		{
			alert("Veuillez indiquer un nombre positif de classe !");
			$('nb_classes').focus();
			return false;
		}
		if ($('accepter_cg').checked == false)
		{
			return confirm("Acceptez les conditions générales de ventes ?");
		}
		return true;
	}
	
</script>
<div id="body" class="contrats">
	<h2>Commande en cours :</h2>
	<?
	$contrat =  $etablissement->contratOuvert();
	if ($contrat)
	{
		$classes = $contrat->users();
		$nclasses = count($classes);
		?>
	<form method="post" action="export.php?module=contrats">
	<table class="liste commandes">
		<thead>
			<tr>
				<th colspan="2">classe</th>
				<th>date céation</th>
				<th>expire le</th>
				<th>Prolongation jusqu’au</th>
				<th>Montant HT</th>
				<th>TVA (19.6%)</th>
				<th>Montant</th>
			</tr>
		</thead>
		<tbody>
			<? 
			$total_ht=0;
			$totol_tva=0;
			$prix = CContrat::prix();

			foreach($classes as $c) {
				$total_ht += $prix["ht"];
				$totol_tva += $prix["tva"];
				$contrat->addItem( $c->login,
								"CLASSE",
								sprintf("%s du %s au %s",
										$c->login,
										$c->created_date(),
										$c->date_prolongation()),
								$prix["ht"],
								$prix["tva"]);
				?>
			<tr>
				<td><?= $c->nom ?></td>
				<td><?= $c->login ?></td>
				<td><?= $c->created_date() ?></td>
				<td><?= $c->expire() ?></td>
				<td><?= $c->date_prolongation() ?></td>
				<td class="action">
					<input type="hidden" name="commande_type[<? $c->login ?>]" value="CLASSE">
					<input type="hidden" name="commande_intitule[<? $c->login ?>]" value="">
					<input type="hidden" name="commande_ht[<? $c->login ?>]" value="<?= $prix["ht"] ?>">
					<?= number_format($prix["ht"],2,",","") ?> €
				</td>
				<td class="action">
					<input type="hidden" name="commande_tva[<? $c->login ?>]"  value="<?= $prix["ht"] ?>">
					<?= number_format($prix["tva"],2,",","") ?> €
				</td>
				<td class="action">
					<input type="hidden" name="commande_ttc[<? $c->login ?>]" value="<?= $prix["ht"] ?>">
					<?= number_format($prix["ttc"],2,",","") ?> €
				</td>
			</tr><? } ?>
			<? 
			if ($total < 30) {
				$total_ht += 20.00;
				$totol_tva += 3.92;
				$contrat->addItem("",
								"FRAIS",
								"Frais de gestion (offert pour 2 classes et plus)",
								20.00,
								3.92);
			?>
			<tr>
				<td colspan="5">Frais de gestion (offert pour 2 classes et plus)</td>
				<td class="action">
					20,00 €
				</td>
				<td class="action">
					3,92 €
				</td>
				<td class="action">
					23,92  €
				</td>
			</tr><? } ?>
		</tbody>
		<tfoot>
			<tr>
				<td colspan="5">Totaux :
				<td><?= $total_ht ?> €HT</td> 
				<td><?= $totol_tva ?> €</td> 
				<td><?= $totol_tva + $total_ht ?> €TTC</td> 
		</tfoot>
	</table>
	<input type="hidden" name="bc_items" value="<?= urlencode(serialize($contrat->items)) ?>">
	<input type="submit" name="print_bc" value="Imprimer le bon de commande">
	
	</form>
	<? }
	else { ?>
		<p class="info">Vous n’avez aucune classe d’ouverte. Pour ouvrir des classes, utilisez le formulaire ci-dessous.</p>
	<? } ?>
	
	
	<h2>Historique :</h2>
	<p>
		Pour tout demande commerciale, contactez notre service commercial au : 05.56.74.26.90 ou par email : <a href="mailto:contact@mezeval.fr?subject=Demande d'informations commerciales">contact@mezeval.fr</a>.
	</p>
	<? 
	echo $_POST["new_bc"];
	if ($_POST["new_bc"])
	{
		$c = new CContrat();
		$c->etablissement = $etablissement->id;
		$c->valide = 0;
		$c->facture = 0;
		$c->nombre_classe = $_POST["nb_classes"];
		$c->montant = $_POST["montant"];
		$c->debut_validite = strtotime("now");
		$c->fin_validite = strtotime("+1 year");
		$c->modele = FICHIER_BON_DE_COMMANDE;
		$c->create($DB);
	}
	?>
	<form method="post">
	<table id="contrats" class="liste">
		<thead>
			<tr>
				<th>n°</th>
				<th>état</th>
				<th>nombre de classe</th>
				<th>montant TTC</th>
				<th>Période de validité</th>
				<th colspan="2">Documents</td>
				<th colspan="2">Actions</td>
				<th>Actions en cours</th>
			</tr>
		</thead>
		<tbody>
			<? 
			$contrats = CContrat::objectsForEtablissement($etablissement->id);
			if ($contrats) {
				foreach($contrats as $c) { ?>
					<tr>
						<td><?= $c->id ?></td>
						<td><?= $c->etat() ?></td>
						<td><?= $c->nombre_classe ?></td>
						<td><?= $c->montant ?></td>
						<td><?= $c->debut_validite() ?> - <?= $c->fin_validite() ?></td>
						<td class="action">
							<? if ($contrat->valide == 0) { ?><a target="_blank" href="export.php?module=contrats&bon_commande_pdf=<?= $c->id ?>">bon de commande</a><? } ?>
						</td>
						<td class="action">facture</td>
						<td class="action">
							<? if ($contrat->valide == 0) { ?><a href="?module=contrats&supprimer=<?= $c->id ?>">supprimer</a><? } ?>
						</td>
						<td class="action">
							<? if ($contrat->valide == 0) { ?><a href="?module=contrats&supprimer=<?= $c->id ?>">supprimer</a><? } ?>
						</td>
					</tr><? }
			} else { ?><tr><td  colspan="7" style="text-align: center; font-style: italic">Aucun documents</td></tr><? } ?>
		</tbody>
	</table>
	</form>
</div>
<? }
