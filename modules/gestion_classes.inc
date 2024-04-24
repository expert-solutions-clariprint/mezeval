<?

if ($_GET["bon_commande_pdf"])
{
	if ($contrat = CContrat::objectWithId($_GET["bon_commande_pdf"]))
	{
		include(APP_PATH . "/bons_commande/" . $contrat->modele);
		die();
	}
}

function prolonger($user,$etablissement)
{
	global $DB;
	$contrat = $etablissement->contratOuvert();
	if (!$contrat)
	{
		$contrat = new CContrat;
		$contrat->etablissement = $etablissement->id;
		$contrat->valide = 0;

		$contrat->debut_validite = mktime("now");
		$contrat->create($DB);
	}
	$user->setContrat($contrat->id);
}

function send_password($user,$pass)
{
	$subject = "Paramètres de connexions";
	$corps = "Bonjour Madame, Monsieur,

Veuillez trouver ci-joint les identifiants de connexion permettant afin d’accéder à un espace sécurisé de gestion de la progression pédagogique d'une classe d’élèves.

Veuillez conserver ces identifiants dans un endroit sûr, par exemple, en imprimant ce mail et en le supprimant. En cas de perte vous pouvez vous adresser au responsable de votre école, ou en vous connecter à www.mezeval.fr et suivre les instructions de connexion.

Pour tout renseignement, vous pouvez joindre le support utilisateur, du Lundi au Vendredi, de 9h à 12h et de 14h à 18h ou par email (support@mezeval.fr).

Vos identifiants :
Identifiant : ". $user->login ."
Mot de passe : $pass

Accés sécurisé : https://www.mezeval.fr

Cordialement,
L'équipe de mezeval.fr";
	return mail($user->email,$subject,$corps,"From: Support utilisateurs <support@mezeval.fr>\r\nReply-To: Support utilisateurs <support@mezeval.fr>");
}

if ($_POST["add_classes"])
{
	$n = $_POST["nb_classes"];
	$i = 0;
	for($i=0; $i < $n; $i++)
	{
		$x = new CUser();
		$x->locked = 0;
		$x->expire = mktime(0,0,0, date("m") + 3, date("d"), date("Y"));
		$x->administrator = 0;
		$x->manager = CUser::user()->id;
		$x->login;
		if ($x->create($DB))
		{
			$x->setLogin("CLASSE" . $x->id);
			$x->update($DB);
		}
	}
}
CTemplate::draw_main_menu("gestion_classes");

$etablissement = CEtablissement::uniqueObject();

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
	
	function confirm_delete_classe()
	{
		return confirm("La suppression d’une classe entraine la suppression de l’ensemble des données associées (evaluations ...). Vous pouvez utiliser ‘verrouiller’ pour simplement la rendre non modifiable mais consultable. Confirmez-vous la suppression ?");
	}
</script>
<div id="body" class="contrats">
	<?
	if ($_POST["rattachement_accept"])
	{
		$defaut_err = "Une erreur est survenue, votre demande de rattachement n’a pas été prise en compte.";
		foreach($_POST["rattachement_accept"] as $k => $none)
		{
			if ($o = CUser::objectWithId($k))
			{
				if ($o->attacheTo(CUser::user()))
				{
					?><div class="success">L’utilisateur <?= $o->nom ?> a été attaché avec succès. Une courriel de confirmation lui a été envoyé.</div><? 
				} else $present_error = $defaut_err;
			} else $present_error = $defaut_err;
		}
		$defaut_err = "Une erreur est survenue, votre demande d’annulation n’a pas été prise en compte.";
	}
	if ($_POST["rattachement_cancel"])
	{
		foreach($_POST["rattachement_cancel"] as $k => $none)
		{
			if ($o = CUser::objectWithId($k))
			{
				if ($o->cancelAttacheBy(CUser::user()))
				{
					?><div class="success">Vous avez annulé la demande de rattachement de <?= $o->nom ?>. Un courriel lui a été envoyé.</div><? 
				} else $present_error = $defaut_err;
			} else $present_error = $defaut_err;
		}
	}
	if ($present_error) { ?><div class="alert"><?= $present_error ?></div><? }
	
	if ($l = CUser::user()->demandesRattachement())
	{
		?>
		<h2>Demandes de rattachement</h2>
			<form method="post" action="?module=gestion_classes">
				<table>
					<thead>
						<tr>
							<th>nom</th>
							<th>email</th>
							<th>login</th>
							<th colspan="2">actions</th>
						</tr>
					</thead>
					<tbody>
					<? foreach($l as $o) { ?>
						<tr>
							<td><?= $o->nom ?></td>
							<td><?= $o->email ?></td>
							<td><?= $o->login ?></td>
							<td><input onclick="return confirm('Confirmez le rattachement ?')" type="submit" name="rattachement_accept[<?= $o->id ?>]" value="Accepter"></td>
							<td><input type="submit" name="rattachement_cancel[<?= $o->id ?>]" value="Refuser"></td>
						</tr><? } ?>
					</tbody>
				</table>
			</form>
		<table>
		<?
	} ?>
	<h2>Classes ouvertes :</h2>
	<p class="info">Pour chaque classe, vous disposez d’un identifiant de connexion et d’un mot de passe. Pour des raisons de sécurité les mots de passe ne sont pas enregistrés sur nos serveurs. Donc à chaque fois que vous utilisez le bouton "identifiants de connexions", un nouveau mot de passe est créé.</p>
	<?
	$classes =  CUser::unlockedObjectForManager(CUser::user()->id);
	if (count($classes) > 0)
	{
		foreach($classes as $c) {
			if ($_POST["prolonger"] && $_POST["prolonger"][$c->id])
			{
				prolonger($c,$etablissement);
				?><div class="success">La classe <?= $c->nom ?> a été ajoutée au bon de commande, une fois les classes désirées ajoutées, n’oubliez pas d’imprimer le bon de commande et de nous l’envoyer.</div><?
			}
		}
		?>
	<form method="post">
	<table class="liste">
		<thead>
			<tr>
				<th>X</th>
				<th>classe</th>
				<th>email professeur</th>
				<th>identifiant</th>
				<th>mot de passe</th>
				<th>date création</th>
				<th>expire le</th>
				<th colspan="5">actions</th>
			</tr>
		</thead>
		<tbody>
			<? foreach($classes as $c) {
				
				if (($_POST["update_classes"] || $_POST["update_mdp"]) && $_POST["select_user_" . $c->id])
				{
					$c->nom = $_POST["update_nom"][$c->id];
					$c->email = $_POST["update_email"][$c->id];
					$c->update($DB);
				}
				if ($_POST["update_mdp"] && $_POST["select_user_" . $c->id] )
				{
					if (true)
					{
						if (!$c->login) $c->setLogin("CLASSE" . $c->id);
						$p = $c->generatePassword();
						if (send_password($c,$p))
						{ ?><div class="success">  Un nouveau mot de passe a été envoyé à <?= $c->email ?>.</div><? }
						else 
						{ ?><div class="alert">Le mot de passe n’as pu être envoyé à <?= $c->email ?>.</div><? }
						
					}
				}
				if (!$c->expire) $c->updateValidity();
				?>
			<tr>
				<td><input type="checkbox" name="select_user_<?= $c->id ?>"></td>
				<td><input type="text" size="10" name="update_nom[<?= $c->id ?>]" value="<?= $c->nom ?>"></td>
				<td><input type="text" name="update_email[<?= $c->id ?>]" value="<?= $c->email ?>"></td>
				<td><?= $c->login ?></td>
				<td title="Mot de passe permettant de se connecter. Si non attribué, il est impossible de se connecter"><? if ($c->pass) echo "******"; else echo "non attribué"; ?></td>
				<td><?= $c->created_date() ?></td>
				<td style="color: <? if (!$c->contrat) echo "red"; else echo "green"; ?>;">
					<?= $c->expire() ?>
				</td>
				<td class="action">
					<? if (!$c->contrat) { ?><input type="submit" name="prolonger[<?= $c->id ?>]" value="prolonger"><? } ?>
				</td>
				<td class="action">
				<a href="?module=contrats&get_mdp=<?= $c->id ?>" onclick="return  confirm_delete_newmdp()">mot de passe</a></td>
				<td class="action"><a href="?module=contrats&get_mdp=<?= $c->id ?>" onclick="return  confirm_delete_newmdp()">nouveau mot de passe</a></td>
				<td class="action"><a href="?module=contrats&delete_account=<?= $c->id ?>" onclick="return  confirm_delete_classe()" title="supprime la classe et out les elements associés">supprimer</a></td>
				<td class="action">vérouiller</td>
			</tr><? } ?>
		</tbody>
	</table>
	<input type="submit" name="update_classes" value="enregistrer les noms et email">
	<input type="submit" name="update_mdp" value="Envoyer les mots de passe">
	</form>
	<? }
	else { ?>
		<p class="info">Vous n’avez aucune classe d’ouverte. Pour ouvrir des classes, utilisez le formulaire ci-dessous.</p>
	<? } ?>
	
	<h2>Ajouter des classes : </h2>
	<form method="POST">
	<p class="info">Une fois la classe créée, vous disposez d’un délais de deux mois pour l’activer sur 12 mois. Pour ce faire, vous devez nous envoyer un bon de commande associé à cette classe. Durant ces 2 mois l’utilisation est libre. Passé les 2 mois, la classe devient inactive, elle peut-être visualisée mais les données ne peuvent plus être modifiées.</p>
	<table>
		<thead>
			<tr>
				<th>Nombre de classes à ajouter :</th>
				<td><input type="text" value="1" size="2" name="nb_classes"></td>
				<td><input type="submit" name="add_classes"></td>
			</tr>
		</thead>
	</table>
	</form>
	
	<h2>Classe expirées :</h2>
</div>
<? }
