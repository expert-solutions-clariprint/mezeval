<?
if ($classe= $_SESSION["classe"])
{if ($progression = CProgression::objectWithId($_GET["progression"]))
{
	$eleve = null;
	if (array_key_exists("eleve",$_GET))
		$eleve = CEleve::objectWithId($_GET["eleve"]);
		?>
<h1><img src="img/historique.gif">&nbsp;<?= $progression->info() ?></h1>
<? if ($eleve) { ?><h2><?= $eleve->info() ?></h2><? } ?>
<table class="liste alternate">
	<thead>
		<tr>
			<td>Date</td>
			<td>
				Évaluation<br>
				<i>Élément</i>
			</td>
			<? if ($eleve) { ?>
			<td>
				Résultat
			</td>
			<td>
				Commentaires
			</td>
			<? } ?>
			<td>
				Classe
			</td>
		</tr>
	</thead>
	<tbody>
		<? 
		$last_eval = 0;
		$url_colors = $classe->url_colors();

		foreach(CElement::objectsForProgression($progression->id) as $elem)
		{
			$eval = $elem->evaluation();
			$a = CEvaluationEleve::countForElementAndNiveau($elem->id,"a");
			$b = CEvaluationEleve::countForElementAndNiveau($elem->id,"b");
			$c = CEvaluationEleve::countForElementAndNiveau($elem->id,"c");
			$x = CEvaluationEleve::countForElementAndNiveau($elem->id,"x");
			
		?>
		<tr>
			<td><? if ($last_eval != $eval->id)  echo $eval->date() ?></td>
			<td><? if ($last_eval != $eval->id)  printf("%s<br>", $eval->info()); ?>
				<i><?= $elem->info() ?></i>
				
			</td><?
			if ($eleve)
			{ 
				$result = CEvaluationEleve::objectForEleveAndElement($eleve->id,$elem->id);
				$info = CEvaluationEleveInfo::valueForEleveEvaluation($eleve->id,$eval->id); ?>
			<td style="text-align: center; background-color: <?= $classe->colorForEval($result->valeur) ?>">
				<?= $classe->userCode($result->valeur) ?><br>
			</td>
			<td><?= $info ?></td><? } ?>
			<td><img src="graph_abc.php?a=<?=$a ?>&b=<?=$b ?>&c=<?=$c ?>&x=<?=$x ?>&<?= $url_colors ?>" />
		</tr>
		<?
			$last_eval = $eval->id;
		} ?>
	</tbody>
</table>

<?
/* </div> */
//	} else echo "erreur élève";
} else echo "erreur évaluation";
}
