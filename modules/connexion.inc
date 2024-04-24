<?
$erreur_etablissement = false;
$erreur_login = false;
global $DB;


$unlockkey="";
if ($_GET["unlock"])
{
	if ($o = CUser::objectWithUnlockKey($_GET["unlock"]))
	{
		CUser::setAsCurrentUser($o);
		$present_erreur = "Veuillez modifier votre mot de passe afin de pouvoir vous reconnecter normalement ultérieurement.";
		$unlockkey = $_GET["unlock"];
	} else {
		$present_erreur = "Cette clef de déblocage a expirée, veuillez demander une nouvelle clef.";
	}
}

if ($_POST["connect"])
{
	if ($DB->connect())
	{
		if ($DB->database_name() == $DB->name)
		{
			if (!CUser::loginWithLogin($_POST["login"],$_POST["password"]))
				$erreur_login = true;
			else {
				$_SESSION["CDATABASE_NAME"] = $DB->name;
			}
		} else $erreur_etablissement = true;
	} else {
		$erreur_etablissement = true;
	}
} else if ($_POST["new_account"]) {
	if ($_POST["new_type"] == "directeur")
	{
		$x = new CUser();
		$x->nom = trim($_POST["new_account_name"]);
		$x->email = trim($_POST["new_account_email"]);
		$x->login = trim($_POST["new_account_email"]);
		$x->administrator = 1;
		if (CUser::checkAvaibilityOfEmailAndAdmin($x->email))
		{
			$x->create($DB);
			if ($x->id)
			{
				CUser::setAsCurrentUser($x);
				$x->setLogin("ADMIN_" . $x->id);
				$present_password = $x->generatePassword();
				$msg = "Bienvenue sur Mezval.fr \n";
				$msg .= "Vos paramètres de connexion sont : \n";
				$msg .= "identifiant : " .  $x->login  . "\n";
				$msg .= "mot de passe : " .  $present_password  . "\n\n";
				$msg .= "Veuillez garder ces informations de manière sécurisée.\n";
				$msg .= "Nous vous remercions de votre confiance,\n";
				$msg .= "L'équipe de Mezeval.fr";

				mail($x->email,"Paramètres de connexion à Mezeval.fr",$msg,"from: contact@mezeval.fr");
	
				$e = new CEtablissement();
				$e->nom = "École";
				$e->manager = $x->id;
				$e->contact_nom = trim($_POST["new_account_name"]);
				$e->contact_email = trim($_POST["new_account_email"]);
				$e->create($DB);
			} else $present_erreur = "Une erreur est survenue.";
		} else $present_erreur = "Attention : vous ne pouvez pas créer plusieurs compte de directeur avec la même email. En cas de pertes du mot de passe, veuillez utiliser le formulaire adéquat, ou contactez le service commercial";
	} else if ($_POST["new_type"] == "classe")
	{
		$x = new CUser();
		$x->nom = trim($_POST["new_account_name"]);
		$x->email = trim($_POST["new_account_email"]);
		$x->login = $x->email;
		$x->administrator = 0;
		$x->manager = 0;
		$x->create($DB);
		if ($x->id)
		{
			$new_user = true;
			CUser::setAsCurrentUser($x);
			$x->setLogin($x->email);
			$present_password = $x->generatePassword();
			$msg = "Bienvenue sur Mezval.fr \n";
			$msg .= "Vos paramètres de connexion sont : \n";
			$msg .= "identifiant : " .  $x->login  . "\n";
			$msg .= "mot de passe : " .  $present_password  . "\n\n";
			$msg .= "Veuillez garder ces informations de manière sécurisée.\n";
			$msg .= "Nous vous remercions de votre confiance,\n";
			$msg .= "L'équipe de Mezeval.fr";
			mail($x->email,"Paramètres de connexion à Mezeval.fr",$msg,"from: contact@mezeval.fr");
		} else $present_erreur = "Une erreur est survenue."; 
	} else $present_erreur = "Une erreur est survenue.";
}
CTemplate::draw_main_menu("connexion");
?>
<style>
	div#body table.connexion td
	{
		border: 5px solid rgb(70, 73, 86); /* rgb(255, 165, 0); */
		margin: 10px;
		vertical-align: top;
		padding: 0px 5px 5px 5px;
		width: 50%;
	}
	table.connexion
	{
		width: 860px;
		margin-top: 10px;
		margin-bottom: 30px;
		margin-left: auto;
		margin-right: auto;
		border-spacing: 20px; 
	}
	table.connexion p#contact
	{
		text-align : center;
		font-size: 20px
	}
	div#body table.connexion td h2
	{
		font-size : 12px;
		background-color: rgb(255, 165, 0);
	}
	form#new_account input#new_account_name,
	form#new_account input#new_account_email,
	form#new_account input#new_account_etablissement
	{
		width: 90%;
	}
</style>
<div id="body" class="connexion">
	<? 
	if ($_POST["new_pwd_login"])
	{
		$email = trim($_POST["new_pwd_login"]);
		$l = CUser::objectsWithEmail($email);
		if (count($l) > 0)
		{
			$n = count($l);
			$subject = "[Mezeval] Clefs de déblocage";
			$corp = "Madame, Monsieur,
	une demande de clefs de déblocage a été générée sur l’application www.mezeval.fr concernant votre adresse mail.
Si vous n’êtes pas concerné, veuillez nous pardonner du dérangement et ignorer cet email.

Votre adresse est attaché à $n comptes, pour chaque compte une adresse de déblocage vous permettra de changer le mot de passe :
";
			foreach($l as $u)
			{
				$k = $u->setUnlockKey();
				$corp .= "\nClasse " . $u->nom;
				$corp .= "\n * identifiant : " . $u->login;
				$corp .= "\n * adresse de déblocage : https://www.mezeval.fr/?module=connexion&unlock=$k\n";
				$corp .= "\n";
			}
			$corp .= "\n";
			$corp .= "L’équipe de Mezeval vous remercie de votre confiance.";
			$head = "From: contact@mezeval.fr";
			mail($email,$subject,$corp,$head);
		} else { $present_erreur = "L’email $email n’est pas reconnue !"; }
	}

	if ($present_erreur)
	{ ?><div class="alert"><?= $present_erreur ?></div><? } 
	
	if (!CUser::is_logged()) { ?>
	<table class="connexion">
		<tr>
			<td>
				
				<h2>Vous possédez des identifiants de connexion :</h2>
				<? if ($erreur_login) { ?><div id="error">Attention : les informations fournies ne permettent pas de vous identifier.</div><? } ?>	
				<form method="post" action="?module=connexion" id="login" style="margin: 0px auto; text-align: left;">
					Identifiant :<br>
					<input type="text" name="login" id="login" <? if ($_POST["login"]) { ?>value="<?= $_POST["login"] ?>"<? } ?>><br>
					Mot de passe :<br>
					<input type="password" name="password" id="password" <? if ($_POST["password"]) { ?>value="<?= $_POST["password"] ?>"<? } ?>><br>
					<input type="submit" name="connect" value="Connexion">
				</form>
			</td>
			<td>
				<form method="POST" id="new_account"  action="?module=connexion">
					<h2>Vous souhaitez créer un identifiant et créer un compte :</h2>
					Votre email (utilisé comme identifiant) :<br>
					<input xl_validate="email" type="text"name="new_account_email" id="new_account_email" xl_validate_info="Une adresse email valide est nécessaire pour créer un compte !"><br>
					Indiquez vos nom et prénom :<br>
					<input xl_validate="length" xl_validate_length="5" type="text" value="" name="new_account_name" id="new_account_name" xl_validate_info="Vos nom et prénom doivent au moins avoit plus de 5 caractères"><br>
					Le nom de votre établissement (optionnel) :<br>
					<input  type="text" value="" name="new_account_etablissement" id="new_account_etablissement"><br>
					Type de compte :<br>
					<input type="radio" name="new_type" checked id="radio_classe" value="classe"><label for="radio_classe">Professeur d’école</label><br>
<!--					<input type="radio" name="new_type" id="radio_etablissement" value="directeur"><label for="radio_etablissement">Directeur d’école</label><br> -->
					<input type="submit" name="new_account" onclick="return xl_validate_form(this.form);" value="Suite >>">
				</form>
				NB: Ces informations sont confidentielles et ne seront fournies à des tiers que sur votre accord.
			</td>
		<tr>
			<td>				<h2>En cas de perte du mot de passe :</h2>
				<form method="post" id="login" style="margin: 0px auto; text-align: left;"  action="?module=connexion">
					Email :<br>
					<input type="text" name="new_pwd_login" id="login" value="<?= $_POST["new_pwd_login"] ?>"><br>
					<input type="submit" name="get_new_pwd" value="Recevoir un nouveau mot de passe">
				</form>
			</td>
			<td>
				<h2>Besoin d’aide, de conseils ou d’informations : </h2>
						
				 	<p id="contact">Contactez-nous : 09.53.42.36.61</p>
				 	<p id="contact">ou <a href="mailto:contact@mezeval.fr">contact@mezeval.fr</a></p>
						
			</td>
		</table>
	</div>
	<? } else if ($new_user) {
		if ($present_password)
		{
			$u = CUser::user();
			?><div class="success">Bienvenue <?= $u->nom ?><br>
				Vos paramètres de connexions sont : 
				<ul>
					<li>identifiant : <?= $u->login ?>
					<li>Mot de passe : <?= $present_password ?>
				</ul>
				Veuillez garder ces informations, elles seront necessaire à une reconnexion. Vous pouvez imprimer cette page
			</div><? 
		} ?>
		<h2>Accès au forum (facultatif mais recommandé)</h2>
		<p>Le forum Mezeval recèle de nombreuses information concernant l'utilisation de Mezeval, il vous permet de poser des questions à l'équipe de Mezeval, des consulter la documentation, de partager des informations (comme des fichiers de progressions) avec les autres utilisateurs.</p>
		<p>Pour des raison de sécurité des données des élèves, le forum est séparé de Mézeéval, pour participer, vous devez créer un identifiant spécifique : </p>
		<input type="button" onclick="window.open('http://forum.mezeval.fr/ucp.php?mode=register')" value="créer un identifiant/mot  de passe pour le forum maintenant." />
		
		<p><i>La création d'un identifiant pour le forum est facultatif pour l'utilisation de Mezeval. Vous pouvez le créer à tout moment. Vous pouvez utiliser aussi une adresse email différentes que celle utilisée dans Mézeval.</i></p>
		<h2>Créer ma première classe</h2>
		<input type="button" onclick="window.location = '?module=classes&nouvelle_classe=1'" value="créer ma classe >>" />
		<?
		
	} else  {
		CUser::user()->reload();
		if ($present_password)
		{
			$u = CUser::user();
			?><div class="success">Bienvenue <?= $u->nom ?><br>
				Vos paramètres de connexions sont : 
				<ul>
					<li>identifiant : <?= $u->login ?>
					<li>Mot de passe : <?= $present_password ?>
				</ul>
				Veuillez garder ces informations, elles seront nécessaires à une reconnexion. Vous pouvez imprimer cette page
			</div><? 
		}
		if ($_POST["update_info"])
		{
			$u = CUser::user();
			$u->nom = $_POST["nom"];
			$u->email = $_POST["email"];
			$u->telephone = $_POST["telephone"];
			$u->etablissement = $_POST["etablissement"];
			$u->update();
			?><div class="success">Informations modifiées avec succès.</div><?
		} else if ($_POST["update_pwd"])
		{
			$u = CUser::user();
			echo "la";
			echo $_POST["unlock_key"];
			if ($u->updatePassword($_POST["update_pwd_old"],$_POST["update_pwd_new"],$_POST["unlock_key"]))
			{
				?><div class="success">Mot de passe modifié avec succès.</div><?
			} else 
			{
				?><div class="alert">Attention : Le mot de passe n’a pu être modifié.</div><?
			}
		}
		?>
	<h1>Vous êtes connectés à Mezeval en tant que : <?= CUser::user()->nom ?></h1>
	<h2>Informations générales sur mon compte</h2>
	<form method="POST" action="?module=connexion">
		<table>
			<thead>
				<th>Nom :</th>
				<th>Email :</th>
				<th>Téléphone :</th>
			</thead>
			<tbody>
				<tr>
					<td><input type="text" name="nom" value="<?= CUser::user()->nom ?>" style="width: 250px"></td>
					<td><input type="text" name="email" value="<?= CUser::user()->email ?>" style="width: 350px"></td>
					<td><input type="text" name="telephone" value="<?= CUser::user()->telephone ?>" style="width: 150px"></td>
					<td rowspan="4"><input type=submit name="update_info" value="Enregistrer"></td>
				</tr>
			</tbody>
			<thead>
				<tr>
					<th colspan="3">Nom de l’établissement (apparaissant sur les impressions) :</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td colspan="3"><input style="width: 100%" type="text" name="etablissement" value="<?= CUser::user()->etablissement ?>"></td>
				</tr>
			</tbody>
			<thead>
				<th>Identifiant : </th>
				<th>Date de création : </th>
				<? if (CUser::isAdministrator()) { ?>
				<th colspan="2">Fin d’accès en modification : </th><? } ?>
			</thead>
			<tbody>
				<tr>
					<td><?= CUser::user()->login ?>
					<td><?= CUser::user()->created_date() ?>
				<? if (CUser::isAdministrator()) { ?>
					<td colspan="2"><?= CUser::user()->expire() ?>
						<? if (!CUser::user()->contrat) { ?>
					<br>Pour prolonger la date d’utilisation de 12 mois, voyez ci-dessous.<? } ?></td><? } ?>
				</tr>
			</tbody>
		</table>
	</form>
		<table id="nav" class="control">
			<tr>
				<td id="back"></td>
				<td></td>
				<td id="next"><input type="button" onclick="window.location='?module=classes'" value="Suivant : Gérer mes groupes >>"></td>
			</tr>
		</table>
	<h2>Modifier mon mot de passe : </h2>
	<form method="POST" action="?module=connexion">
		<table>
			<thead>
				<tr>
					<th>Ancien mot de passe :</th>
					<th>Nouveau mot de passe : </th>
					<th>Confirmation du nouveau mot de passe : </th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td><input type="password" value="<?= $unlockkey ?>" name="update_pwd_old" xl_validate="required" style="width: 250px"><? if ($unlockkey) { ?><input type="hidden" name="unlock_key" value="<?= $unlockkey ?>"><? } ?></td>
					<td><input type="password" name="update_pwd_new" xl_validate="length" xl_validate_length="6" style="width: 250px"></td>
					<td><input type="password" name="update_pwd_new2" style="width: 250px" xl_validate="identity" xl_validate="identity" xl_identic_at="update_pwd_new"></td>
					<td><input type=submit name="update_pwd" value="Enregistrer" onclick="return xl_validate_form(this.form)"></td>
				</tr>
			</tbody>
		</table>
	</form>
	<? if (FALSE) { ?>
	<h2>Etablissement de rattachement :</h2>
	<? if ($e = CUser::user()->etablissement()) { ?>
	<table>
		<tbody>
			<tr>
				<th>Nom :</th>
				<td><?= $e->nom ?></td>
			</tr>
			<tr>
				<th>siret :</th>
				<td><?= $e->siret ?></td>
			</tr>
			<tr>
				<th>UAI :</th>
				<td><?= $e->uai ?></td>
			</tr>
			<tr>
				<th>Adresse :</th>
				<td><?= $e->contact_adresse_1 ?><br><?= $e->contact_adresse_2 ?><br><?= $e->contact_codepostal ?> <?= $e->contact_ville ?></td>
			</tr>
		</table>
	<? } else { 
		if ($_POST["demande_rattachement"]) {
			$email = trim($_POST["demande_rattachement_email"]);
			if ($o = CUser::administratorWithEmail($email))
			{
				CUser::user()->setRattachement($email);
				$o->demandeRattachement(CUser::user());
				?><div class="success">Une demande de rattachement a été envoyé à l’adresse <?= $email ?>.</div><?
				
			} else {
				?><div class='alert'>Attention : l’adresse email <?= $email ?> n’est pas reconnue. Veuillez demander à votre directeur d’école de créer un compte utilisateur de type "Directeur". Ce compte permettra d’émettre un bon de commande, ainsi que de visualiser des statistiques globales sur les résultats des élèves des classes attachées.
Une fois ce compte créé, vous devrez recommancer la demande d’attachement.</div><?
			}
		}
		else { ?>
		<p>Vous n’êtes attaché à aucun établissement. Pour ce faire, utilisez le formulaire ci-dessous. Un mail sera envoyé contenant une demande d’attachement et des codes d’accès de gestion si il n’est pas encore renseigné dans Mezeval.</p><? } ?>
		<form method="POST"  action="?module=connexion">
		<table>
			<thead>
				<tr>
					<th colspan="2">Demande de rattachement à un établissement</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>email officiel du gestionnaire de votre établissement :<br>
						<input type="text" name="demande_rattachement_email" style="width: 500px" value="<?= CUser::user()->rattachement ?>"></td>
					<td><input type="submit" name="demande_rattachement" value="Envoyer la demande">
				</tr>
			</tbody>
		</table>
		</form><? }}

/*-------------------------------------------------- */	
	if (CUser::isAdministrator() && $etablissement = CEtablissement::uniqueObject()) { 
		?>
	<h2>Information sur mon établissement :</h2>
		<?
		if ($_POST["update_etablissement_info"])
		{
			$etablissement->nom = $_POST["etablissemnt_nom"];
			$etablissement->contact_nom = $_POST["contact_nom"];
			$etablissement->contact_adresse_1 = $_POST["contact_adresse_1"];
			$etablissement->contact_adresse_2 = $_POST["contact_adresse_2"];
			$etablissement->contact_codepostal = $_POST["contact_codepostal"];
			$etablissement->contact_ville = $_POST["contact_ville"];
			$etablissement->contact_telephone = $_POST["contact_telephone"];
			$etablissement->contact_fax = $_POST["contact_fax"];
			$etablissement->contact_email = $_POST["contact_email"];
			$etablissement->facturation_nom = $_POST["facturation_nom"];
			$etablissement->facturation_adresse_1 = $_POST["facturation_adresse_1"];
			$etablissement->facturation_adresse_2 = $_POST["facturation_adresse_2"];
			$etablissement->facturation_codepostal = $_POST["facturation_codepostal"];
			$etablissement->facturation_ville = $_POST["facturation_ville"];
			$etablissement->update();
			?><div class="success">Modifications enregistrées avec succès.</div><?
		}
		?>
	<form method="POST"  action="?module=connexion">
	<table>
		<thead>
			<tr>
				<td>Coordonnée de l’établissement</td>
				<td>Coordonnée de facturation</td>
		</thead>
		<tbody>
			<tr>
				<td colspan="2">Nom :<br><input name="etablissemnt_nom" value="<?= $etablissement->nom ?>" type="text" size="40" maxlength="100" style="width: 100%"></td>
			<tr>
				<th colspan="1">Adresse de l’établissement</th>
				<th colspan="1">Adresse de Facturation (si différente)</th>
			<tr>
				<td style="vertical-align: top">
					<input name="contact_nom" value="<?= $etablissement->contact_nom ?>" type="text" size="40" maxlength="45"><br>
					Rue :<br>
					<input name="contact_adresse_1" value="<?= $etablissement->contact_adresse_1 ?>" type="text" size="40" maxlength="45"><br>
					<input name="contact_adresse_2" value="<?= $etablissement->contact_adresse_2 ?>" type="text" size="40" maxlength="45"><br>
					Code Postal :<br>
					<input name="contact_codepostal" value="<?= $etablissement->contact_codepostal ?>" type="text" maxlength="6"><br>
					Ville :<br>
					<input name="contact_ville" value="<?= $etablissement->contact_ville ?>" type="text" size="40" maxlength="100"><br>
					Téléphone :<br>
					<input name="contact_telephone" value="<?= $etablissement->contact_telephone ?>" type="text" maxlength="20"><br>
					Fax :<br>
					<input name="contact_fax" value="<?= $etablissement->contact_fax ?>" type="text" maxlength="20">
				</td>
				<td style="vertical-align: top">
					<input name="facturation_nom" value="<?= $etablissement->facturation_nom ?>" type="text" size="40" maxlength="45"><br>
					Rue :<br>
					<input name="facturation_adresse_1" value="<?= $etablissement->facturation_adresse_1 ?>" type="text" size="40" maxlength="45"><br>
					<input name="facturation_adresse_2" value="<?= $etablissement->facturation_adresse_2 ?>" type="text" size="40" maxlength="45"><br>
					Code Postal :<br>
					<input name="facturation_codepostal" value="<?= $etablissement->facturation_codepostal ?>" type="text" maxlength="6"><br>
					Ville :<br>
					<input name="facturation_ville" value="<?= $etablissement->facturation_ville ?>" type="text" size="40" maxlength="100"><br>
					<br>
					<p align="center"><input type="submit" name="update_etablissement_info" value="Enregistrer">
				</td>
			</tr>
		</tbody>
	</table>
		
	</form>		
		
	<? } ?>
	<script language="javascript">
		/*	document.location = '?module=classes'; */
	</script>
	<? } ?>
</div>
