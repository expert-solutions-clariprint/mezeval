<?
$periode = CPeriode::objectWithId($_GET["bilan"]);
if (!$periode) {
	echo "erreur";
	return;
}
?>
	<table class="liste alternate">
		<thead>
			<th style="width: 10px"></th>
			<th>élèves</th>
			<th colspan="2">actions</th>
		</thead>
		<tbody>
			<? foreach($classe->eleves() as $eleve) { ?>
			<tr>
				<td><input type="checkbox"/></td>
				<td><?= $eleve->info() ?></td>
				<td class="action"><a href="?module=bilans&bilan=<?= $periode->id ?>&mode=saisie_eleve&eleve=<?= $eleve->id ?>">Saisie</a></td>
				<td class="action"><a href="/export.php?module=bilan&type=progression&output_mode=pdf&eleve=<?= $eleve->id ?>&periode=<?= $periode->id ?>">Imprimer</a></td>
			</tr>
			<? } ?>
		</tbody>
		<tfoot>
			<tr>
				<th colspan="2">Impression par lot :</th>
				<?
				foreach($PERIODES as $p)
				{ ?>
					<th><? if ($impression[$p->id]) { ?>
						<input type="button" onclick="document.location = 'export.php?module=bilans&output_mode=pdf&periode=<?= $p->id ?>'" value="Imprimer"><? } ?>
					</th><?
				} 
				foreach(array("socle","b2i","langue") as $socle)
				{ ?>
					<th><? if ($impression[$socle]) { ?>
						<input type="button" onclick="document.location = 'export.php?module=bilans&output_mode=pdf&type=<?= $socle ?>'" value="Imprimer"><? } ?>
					</th><?
				} 
				?>
			</tr>
		</tfoot>
	</table>
