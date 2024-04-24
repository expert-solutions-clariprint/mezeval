<?
$periode = CPeriode::objectWithId($_GET["bilan"]);
if (!$periode) {
	echo "erreur";
	return;
}

if ($_POST["update_progessions"])
{
	
}



function showProgression($classe,$prg,$periode,$niveau=0)
{
	if ($prg != null)
	{
		if ($_POST["update"])
		{
			if ($_POST["prog"][$prg->id])
				CProgressionPeriode::createForProgressionAndPeriode($prg->id,$periode->id);
			else CProgressionPeriode::deleteForProgressionAndPeriode($prg->id,$periode->id);
		}
		$childs = $prg->childNodes();
		$count = $prg->countEvaluations($periode->debut,$periode->fin);
		$results=CBilanProgressionElement::resultsForPeriodeProgression($periode->id,$prg->id);
		if (count($childs) == 0) $k= "nofolder";
		?>
		<tr class="niveau<?= $niveau ?> <?= $k ?>" xldeep="<?= $niveau ?>">
			<td><?= $prg->code ?></td>
			<td class="icon intitule"><?= $prg->intitule ?></td>
			<td class="number"><?= $count ?></td>
			<td><input type="checkbox" class="progression_periode" name="prog[<?= $prg->id ?>]" value="<?= $prg->id ?>" <?
					if ($prg->checkForPeriode($periode->id)) echo "checked"; ?> mzCount="<?= $count ?>"/></td>
			<td></td>
			<td><a class="saisie_result" href="export.php?module=bilan_p_saisie&bilan=<?= $periode->id ?>&progression=<?= $prg->id ?>">Saisie</a></td>	
		</tr>
		<? 
	} else $childs = $classe->progressions();

	$niveau++;
	if ($childs)
		foreach($childs as $c) showProgression($classe,$c,$periode,$niveau);
}

?>
<script type="text/javascript" charset="utf-8">
	function checkEvaluees()
	{
		var table = $('progressions');
		var inputs = table.select('input.progression_periode');
		var i =0;
		for(;i < inputs.length; i++)
		{
			var c = inputs[i];
			if (!c.checked)
			{
				if (c.readAttribute("mzCount") != "0")
					c.checked = true;
			}
		}
		updateParents();
	}
	
	function updateParent(r)
	{
		var deep = parseInt(r.readAttribute("xldeep"));
		var up = r.previous();
		while(up)
		{
			var ideep = parseInt(up.readAttribute("xldeep"));
			if (ideep < deep)
			{
				up.down('input.progression_periode').checked=true;
				deep--;
			}
			if (deep > 0) up = up.previous();
			else up = null;
		}
	}
	function updateParents()
	{
		var table = $('progressions');
		var rows = table.select('tr.nofolder');
		var i=0;
		for(;i < rows.length; i++)
		{
			var row = rows[i];
			var c = row.down('input.progression_periode');
			if (c.checked)
				updateParent(row);
		}
	}
	var DBG = null;
	var EvalModal = Class.create(Control.Modal,{
		position:function(evt)
		{
			console.log("la");
			var container_dimensions = this.container.getDimensions();
			DBG = this.container;
			console.log("la2");
			var viewport_dimensions = document.viewport.getDimensions();
			console.log(container_dimensions);
			console.log(viewport_dimensions);
			var off = document.viewport.getScrollOffsets();
			var top = Math.max(0, Math.floor((viewport_dimensions.height - container_dimensions.height) / 2)) + off[1];
			var left = Math.max(0, Math.floor((viewport_dimensions.width - container_dimensions.width) / 2)) + off[0];
			var left2=0;
			console.log("la4");
			Position.prepare();
			console.log("top:"+top + "  left:"+left  + " -- " + left2);
			this.container.setStyle({
				'top': top + 'px',
				'left': left + 'px'
			});
			if(this.iFrameShim)
				this.updateIFrameShimZIndex();
		}
		
	});
	
	function setupSaisie()
	{
		$$('a.saisie_result').each(function(x){ new EvalModal(x,{'className':'saisie_progression_popup'}); });
	}
	
	Event.observe(document,"dom:loaded",setupSaisie);
	
</script>
<form method="POST">
	<table class="alternate liste treeview" id="progressions">
		<thead>
			<tr>
				<th colspan="2">Progression</th>
				<th>Nb. évaluations</th>
				<td><th>
			</tr>
		</thead>
		<tbody>
			<? showProgression($classe,null,$periode); ?>
		</body>
		<tfoot>
			<tr>
				<td colspan="2">
					<input type="submit" name="update" value="Enregistrer" onclick="updateParents();"/>
				</td>
				<td colspan="2">
					<input type="button" value="Cocher les progressions évaluées" onclick="checkEvaluees()"/>
				</td>
		</tfoot>
	</table>
	
</form>