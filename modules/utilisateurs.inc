<?
CTemplate::draw_main_menu("utilisateurs");

?><div id="body" class="module_gestion_utilisateur"><?

if ($_GET["etablissement"]) {
	$etablissement = CEtablissement::objectWithId($_GET["etablissement"]);
	$_SESSION["etablissement"] = $etablissement;
} else if ($_SESSION["etablissement"]) {
	// check access
	$etablissement = $_SESSION["etablissement"];
} else $etablissement= null;
if ($etablissement == null)
{
	$ets = CPTicket::etablissementsForCurrentUser();
	if (count($ets) == 1) {
		$etablissement = $ets[0];
		$_SESSION["etablissement"]=$etablissement;
	}
}


if (!$etablissement)
{
	?><h1>Vous n’avez pas d’accés à la gestion des utilisateurs. Contactez le service technique si il s’agit d’un disfonctionnement.</h1><? 
} else {
	
	if ($_POST["delete_user"])
	{
		CUser::deleteObjectsWihtIds(array_keys($_POST["delete_user"]));
	}

	if ($_POST["add_user"]) $user = new CUser();
	
	if ($_POST["edit_user"] && ($arr = array_keys($_POST["edit_user"]))) $user = CUser::objectWithId($arr[0]);
	
	if ($_POST["save_user_and_back_to_list"]) $save_user = true;
	
	if ($save_user || $_POST["save_user"])
	{
		if ($_POST["id"] && ($user = CUser::objectWithId($_POST["id"])))
		{
			$user->initWithArray($_POST);
			if ($_POST["new_password"]) $user->setPassword($_POST["new_password"]);
			$user->update();
		}
		else {
			$user = CUser::createObjectWithArray($_POST);
			if ($_POST["new_password"]) $user->setPassword($_POST["new_password"]);
			CPTicket::registerUserForEtablissement($user->id,$etablissement->id);
		}
		CUserGroup::setUniqueGroupForUser($_POST["new_role"],$user->id);
	}
	
	if ($add_group && $selecteds) {
		foreach($selecteds as $user_id)
		{
			CUserGroup::addUserToGroup($user_id,$group_to_add);
		}
	}
	if ($save_user_and_back_to_list) $user = null;
	
	if ($new_mdp)
	{
		if ($new_mdp && ($arr = array_keys($new_mdp))) 
		{
			if ($my_user = CUser::objectWithId($arr[0]))
			{
				$pwd = $my_user->startNewPassword();
				?><div class="message">Nouveau mot de passe pour l’utilisateur <?= $my_user->nom ?> : <?= $pwd ?></div><?
			}
		}
	}
?><form method="post"><?

if (!$user) { 

?>
<h1><?= $etablissement->nom ?></h1>
<table class="user_list">
	<thead>
		<tr>
			<th colspan="6">Utilisateurs</th>
			<th colspan="2"><input type=submit name="add_user" value="Nouvel Utilisateur"> 
		</tr>
		<tr>
			<td>&nbsp;
			<td>nom</td>
			<td>email</td>
			<td>verrous</td>
			<td>admin.</td>
			<td>groupes</td>
		</tr>
	</thead>
	<tbody>
		<? 
		foreach(CPTicket::usersForEtablissement($etablissement->id) as $u)  { ?>
		<tr>
			<td><input type=checkbox value="<?= $u->id ?>" name="selecteds[]">
			<td><?= $u->nom ?></td>
			<td><?= $u->email ?></td>
			<td><?= $u->locked ?></td>
			<td><?= $u->administrator ?></td>
			<td><? $sep = ""; foreach($u->groups() as $g) { echo $sep . $g->name; $sep = ", "; }  ?></td>
			<td><input type=submit name="edit_user[<?= $u->id ?>]" value="modifier"></td>
			<td><input type=submit name="new_mdp[<?= $u->id ?>]" value="nouveau mot de passe"></td>
			<td><? if (($cu = CUser::getCurrentUser()) && $u->id != $cu->id) { ?><input type=submit name="delete_user[<?= $u->id ?>]" value="supprimer" onclick="return confirm('Attention : suppression définitive de l’utilisateur !')"><? } ?></td>
		</tr>
		<? } ?>
	</tbody>
	<tfoot>
	</tfoot>
</table>
<?
 } else {
	if ($user->id) { ?><input type="hidden" value="<?= $user->id ?>" name="id"><? }
	?>
<table class="edit_user">
	<thead>
		<tr>
		</tr>
	</thead>
	<tbody>
		<tr>
			<th>n° :</th>
			<td><?= $user->id ?> (<?= $user->get_creation() ?>)</td>
		</tr>
		<tr>
			<th>Role :</th>
			<td><select name="new_role"><?
				
				foreach(CUserGroup::allObjects() as $g)  { ?>
					<option value="<?= $g->id ?>" <? if (CUserGroup::isGroupOfUser($g->id,$user->id)) echo "selected"; ?>><?= $g->name ?></option>
				
				<? } ?>
				</select></td>
		</tr>
		<tr>
			<th>nom :</th>
			<td><input type="text" name="nom" value="<?= $user->nom ?>" style="width: 300px"></td>
		</tr>
		<tr>
			<th>email :</th>
			<td><input type="text" name="email" value="<?= $user->email ?>"  style="width: 300px"></td>
		</tr>
		<tr>
			<th>mot de passe :</th>
			<td><input type="text" name="new_password" style="width:300px"></td>
		</tr>
		<tr>
			<th>verrous :</th>
			<td><input type=checkbox value="1" name="locked" <?  if ($user->locked) echo "checked"; ?>></td>
		</tr>
		<? if (CUser::isAdministrator()) { ?>
		<tr>
			<th>super administrateur :</th>
			<td><input type=checkbox value="1" name="administrator" <?  if ($user->administrator) echo "checked"; ?>></td>
		</tr><? } ?>
		<tr>
			<th>Tél :</th>
			<td><input type="text" name="telephone" value="<?= $user->telephone ?>"></td>
		</tr>
		<tr>
			<th>Mobile :</th>
			<td><input type="text" name="mobile" value="<?= $user->mobile ?>"></td>
		</tr>
		<tr>
			<th>Fax :</th>
			<td><input type="text" name="fax" value="<?= $user->fax ?>"></td>
		</tr>
		<tr>
			<th>Adresse :</th>
			<td><textarea name="addresse"  style="width: 300px"><?= $user->addresse ?></textarea></td>
		</tr>
		<tr>
			<th colspan=2>
				<input type=submit name="back_to_list" value="retourner à la liste">
				<input type=submit name="save_user_and_back_to_list" value="enregitrer et retourner à la liste">
				<input type=submit name="save_user" value="enregitrer">
		</th>
		</tr>
	<tbody>
</table>
<? } ?>
</form><? } ?>
</div>
