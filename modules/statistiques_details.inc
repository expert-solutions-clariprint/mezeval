<?
if ($_GET["progression"] && $classe = $_SESSION["classe"])
{	$progression = CProgression::objectWithId($_GET["progression"]);
	$date_debut_ts = $date_fin_ts =0;
	if ($_GET["periode"])
	{
		$periode = CPeriode::objectWithId($_GET["periode"]);
		$date_debut_ts = $periode->debut;
		$date_fin_ts = $periode->fin;
	}
	$urlcolors = $classe->url_colors();

?>
<script type="text/javascript" charset="utf-8">
	function sortTable(table_id,key,mode)
	{
		var table = $(table_id);
		var nb=0;
		for(nb =0; nb < table.tBodies.length; nb ++ )
		{
			sortTBody(table.tBodies[nb],key,mode);
		}
		alternateTable(table);
	}
	function sortTBody(tbody,key,mode)
	{
		this.key = key;
		this.mode = mode;
		var sortmethod = function(a,b)
		{
			var va = a.readAttribute(this.key);
			var vb = b.readAttribute(this.key);
			if (this.key=='name')
			{	if (va == vb) return 0
				else if (vb > va ) return -1;
				return 1
			}
			else return vb - va;
		}
		var rows = $A(tbody.rows);
		rows.sort(sortmethod);
		var ri=0;
		while(tbody.rows.length > 0) tbody.removeChild(tbody.rows[0]);
		for(ri=0; ri < rows.length; ri++ )
		{
			tbody.appendChild(rows[ri]);
		}
	}
</script>
<div id="body" class="competences">
<h1><?= $progression->info() ?></h1>
<div class="control">
	Tri :
<select onchange="sortTable('table',this.value,0)">
	<option value="name">Nom prénom</option>
	<option value="na">a</option>
	<option value="nb">b</option>
	<option value="nc">c</option>
	<option value="nx">x</option>
	<option value="nab">a+b</option>
	<option value="ncx">c+x</option>
</select
</div>
<table class="list alternate" id="table">
	<thead>
		<tr>
			<th>élève</th>
			<th>évolution</th>
			<th onclick="sortTable('table','na',2);">résultats</th>
		</tr>
	<tbody>
	<? foreach($classe->eleves() as $e) {
		
		$v = CEvaluationEleve::resultatsForProgressionEleve($progression->id,$e->id,$date_debut_ts,$date_fin_ts,$cumul,$mode_eleve,true);
		$v = implode($v);
		$n = $na = $nb = $nc = $nx = 0;
		if ($v)
		{
			$n = strlen($v);
			$na = substr_count($v,"a");
			$nb = substr_count($v,"b");
			$nc = substr_count($v,"c");
			$nx = substr_count($v,"x");
		}
		
		?>
	<tr name="<?= $e->info() ?>" lastresult="<?= substr($v,-1) ?>" na="<?= $na ?>" nb="<?= $nb ?>" nc="<?= $nc ?>" nx="<?= $nx ?>" nab="<?= $na + $nb ?>" ncx="<?= $nc + $nx ?>">
		<td><?= $e->info() ?></td>
		<td><img src="/graph_abc.php?mode=HVALUES&width=100&values=<?=$v ?>&<?= $urlcolors ?>">
		<td><img src="/graph_abc.php?mode=HINDEPENDENT&width=200&values=<?=$v ?>&<?= $urlcolors ?>">
		
	</tr><? } ?>
	<tbody>
</table>
<?  } ?>
</div>