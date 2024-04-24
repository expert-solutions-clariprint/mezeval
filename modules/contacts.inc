<? 
CTemplate::draw_main_menu("contacts");
if ($_POST["contact_send"])
{
	$headers = 'From: contact@mezeval.fr' . "\r\n";
	$headers .= 'X-Mailer: PHP/' . phpversion();
	$msg = sprintf("Nom : %s \nFonction : %s \nEtablissement : %s\nVille : %s\n Info contact : %s \n Message : \n %s",
					$_POST["contact_nom"],$_POST["contact_fonction"],
					$_POST["contact_etablissement"],$_POST["contact_ville"],
					$_POST["contact_mode"],$_POST["contact_msg"]);
	if (!mail("xpechoultres@clariprint.fr","[Mezeval] Demande de contact",$msg,$headers)) echo "erreur";
	else {$AFFICHE_REMERCIEMENT_MESSAGE = true;}
}
?>
<style>
	div#body
	{
		padding: 10px;
	}
</style>
<div id="body" class="default">
	<? if ($AFFICHE_REMERCIEMENT_MESSAGE) { ?> 
	<div class="success">
		Nous vous avons bien enregistré votre demande, nous allons y répondre dans les meilleurs délais en vous contactant au <?= $_POST["contact_mode"] ?>. 
		Veuillez noter que vous pouvez aussi utiliser le <a href="http://forum.mezeval.fr>">forum</a> pour poser des questions. De nombreuses informations sont aussi librement accessibles sur le <a href="http://forum.mezeval.fr>">forum</a>.
	</div>	<? } ?>
	<h1>Contacts</h1>
	
	<p>Mezeval est un service proposé par eXpert soLutions sarl :</p>
	
	EXPERT SOLUTIONS SARL<br>
	1 rue Fleming<br>
	17ooo La Rochelle.<br>
	SIRET : 424 836 989 000 30
	<h2>Commercial</h2>
	Pour toute question commerciale, contactez nous : 05 46 30 13 75
	<ul> 
		<li> par téléphone du lundi au vendredi de 9h à 18h : 05 
		<li> par mail : contact@mezeval.fr
	</ul>
	
	<h2>Aide et informations techniques</h2>
	Pour toute aide concernant l’utilisation de Mezeval ou information technique, contactez-nous :
	<ul> 
		<li> par téléphone du lundi au vendredi de 9h à 18h : 05 46 30 13 75
		<li> par mail : tech@mezeval.com
	</ul>
	<h2>Vous désirez être contacté, laissez nous vos coordonnées :</h2>
	<form method="post">
		<table>
			<tr>
				<td><b>Nom :</b><br><input xl_validate_length="5" xl_validate_info="Vos nom et prenom doivent avoir plus de 5 caractères (au total)." xl_validate="length" type="text" name="contact_nom" size="100"></td>
			<tr>
				<td><b>Numéro de téléphone ou email :</b><br><input xl_validate="length" xl_validate_length="5" type="text" xl_validate_info="Votre email ou votre numéro de téléphone ne semble pas valide" name="contact_mode" size="100"></td>
			<tr>
				<td>Fonction :<br><input type="text" name="contact_fonction" size="100"></td>
			<tr>
				<td>Établissement :<br><input type="text" name="contact_etablissement" size="100"></td>
			<tr>
				<td>Ville ou code postal :<br><input type="text" name="contact_ville" size="100"></td>
			<tr>
				<td>Message (optionnel) :<br><textarea style="width: 400px; height: 50px" name="contact_msg" style=""></textarea></td>
			<tr>
				<td><input type="submit" name="contact_send" onclick="return xl_validate_form(this.form);" value="envoyer la demande de contact"></td>
			</tr>
		</table>
	</form>
</div>
