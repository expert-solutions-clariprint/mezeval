<?
CTemplate::draw_main_menu("gestion_groupes");


if ($_POST["save_object_retour_liste"]) $save_object = true;

if ($_GET["delete_object"])	$object = CUserGroup::deleteObjectWithId($_GET["delete_object"]);
if ($_GET["edit_object"])	$object = CUserGroup::objectWithId($_GET["edit_object"]);
if ($_POST["add_object"]) $object = new CUserGroup();
if ($_POST["save_object"]) {
	$object = CUserGroup::objectWithArray($_POST);
	$object->update();
	if ($_POST["ticket_access"])
	{
		$arr = $_POST["ticket_access"];
		foreach(CTicket::configValues() as $m) {
			$val = $m[1];
			if ($arr[$val]) CTicket::createAccessForGroupToModule($object->id,$val,$arr[$val]);
			else CTicket::deleteAccessForGroupToModule($object->id,$val);
		}
	}
	if ($_POST["save_object_retour_liste"]) $object = null;
}
?><div id="body" class="module_gestion_utilisateur">
<form method="post" action="?module=gestion_groupes">
<?

if (!$object) { ?>
<table class="user_list">
	<thead>
		<tr>
			<th colspan="3">Groupes</th>
			<th><input type=submit name="add_object" value="Nouveau groupe"> 
		</tr>
		<tr>
			<td>id</td>
			<td>nom</td>
			<td>utilisateurs</td>
		</tr>
	</thead>
	<tbody>
		<? 
		foreach(CUserGroup::allObjects() as $u)  { ?>
		<tr> 
			<td><?= $u->id ?></td>
			<td><?= $u->name ?></td>
			<td>
			<td><a href="?module=gestion_groupes&edit_object=<?= $u->id ?>">modifier</a></td>
			<td><a onclick="return confirm('Suppression définitive du groupe ?')" href="?module=gestion_groupes&delete_object=<?= $u->id ?>">supprimer</a></td>
		</tr>
		<? } ?>
	</tbody>
</table>
<?
 } else { ?>
<table class="edit_user">
	<thead>
		<tr>
		</tr>
	</thead>
	<tbody>
		<tr>
			<th>n° :</th>
			<td><?= $object->id ?><? if ($object->id) { ?><input type=hidden name="id" value="<?= $object->id ?>"><? } ?></td>
		</tr>
		<tr>
			<th>nom :</th>
			<td><input type="text" name="name" value="<?= $object->name ?>" style="width:300px"></td>
		</tr>
		<? if ($object->id) { ?>
		<tr>
			<th>Accès aux modules</th>
		</tr>
		<?	foreach(CTicket::configValues() as $m) {
			$mode = CTicket::accessModeForGroupToModule($object->id,$m[1]);
			?> 
		<tr>
			<th><?= $m[0] ?>
			<td><select name="ticket_access[<?= $m[1] ?>]">
					<option value="">-</option>
					<option <? if ($mode == "ACCESS_READ") echo "selected"; ?> value="ACCESS_READ">lecture</option>
					<option <? if ($mode == "ACCESS_UPDATE") echo "selected"; ?> value="ACCESS_UPDATE">modification</option>
				</select>
			<td><?= $mode ?>
		</tr><? } ?>
		<?	} ?>
		<tr>
			<th colspan=2><? if ($object->id) { ?> 
				<input type=submit name="retour_liste" value="retour à la liste des groupes">
				<input type=submit name="save_object_retour_liste" value="enregistrer et retour à la liste"><? } ?>
				<input type=submit name="save_object" value="enregistrer">
		</th>
	<tbody>
</table>

<? } ?>
</form>
</div>
