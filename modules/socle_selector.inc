<? 
function printtree($o,$niveau=0)
{
	$subs=$o->childNodes();
	
	?>
		<tr class="niveau<?=$niveau?><? if (!$subs) echo " nofolder"; if ($_GET["value"] == $o->code) echo " current"; ?>">
			<td class="icon" <? if (!$subs) { ?> onclick="select_competence('<?= $o->code ?>')" <? } ?>><?= $o->info() ?></td>
			<td class="code"><?= $o->code ?></td>
			<? if (!$subs) {?>
			<td class="action" onclick="select_competence('<?= $o->code ?>','<?= addslashes($o->info()) ?>')">choisir</td><? } ?>
		</tr><?
	$niveau++;
	foreach($subs as $s) printtree($s,$niveau);
}

if ($classe= $_SESSION["classe"]) {
	?>
<script type="text/javascript" charset="utf-8">
	function select_competence(code,title)
	{
		if (window.opener.socle_selector_handler)
		{
			window.opener.socle_selector_handler(code,'<?= $_GET["type"] ?>',title);
		}
		window.close();
	}
</script>
<style type="text/css" media="screen">
	table.treeview tr.current td
	{
		border: 1px solid #999;
	}
	table.treeview tr td.icon,
	table.treeview tr td.action
	{
		cursor: pointer;
	}
	table.treeview tr.niveau0 td
	{
		border: 2px solid #999;
		font-weight: bold;
		font-size: 14px;
	}	
</style>
<div id="body" class="socleselector">
<div class="control">Recherche : <input type="text" target="tsocle" target_cols="0 1" class="searchbox" size="20"></div>
	<h1>Choisir un élément du socle</h1>
	<table class="alternate treeview" id="tsocle">
		<?
		if ($_GET["type"] == "socle" && $socle = $classe->socle()) printtree($socle); 
		if ($_GET["type"] == "b2i" && $x = $classe->b2i()) printtree($x);
		if ($_GET["type"] == "langue" && $x = $classe->langue()) printtree($x);
		
		?>
	</table>
</div> <? }
