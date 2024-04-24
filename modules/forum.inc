<?php
CTemplate::draw_main_menu("forum");
if ($_POST["contact_send"])
{
	$headers = 'From: contact@mezeval.fr' . "\r\n";
	$headers .= 'X-Mailer: PHP/' . phpversion();
	$msg = sprintf("Nom : %s \nFonction : %s \nEtablissement : %s\nVille : %s\n Info contact : %s \n Message : \n %s",
					$_POST["contact_nom"],$_POST["contact_fonction"],
					$_POST["contact_etablissement"],$_POST["contact_ville"],
					$_POST["contact_mode"],$_POST["contact_msg"]);
//	if (!mail("contact@mezeval.fr","[Mezeval] Demande de contact",$msg,$headers)) echo "erreur";
//	if (!mail("tech@mezeval.fr","[Mezeval] Demande de contact",$msg,$headers)) echo "erreur";
	if (!mail("xpechoultres@clariprint.fr","[Mezeval] Demande de contact",$msg,$headers)) echo "erreur";
}
 ?>
<style>
	div#body
	{
		padding: 10px;
	}

	div#body p.contacts,
	div#body p.contacts a
	{
		font-size: 20px;
		color: orange;
		font-weight: bold;
		text-align: center;
		magin-right: auto;
		margin-left: auto;
	}
</style>
<div id="body" class="forum">
	<h1>Forum et support utilisateurs en ligne</h1>
	
	<p>Un forum dédié à Mezeval est accessible à l’adresse suivante : <a href="http://forum.mezeval.fr">http://forum.mezeval.fr</a>. Ce forum est public et permet d’échanger avec l’équipe de Mezeval ainsi qu’avec les autres utilisateurs.</p>
	<p>Pour des raison de sécurité, le fonctionnement du forum est indépendant du fonctionnement de Mezeval. Pour envoyer des messages ou télécharger des documents, vous devez donc suivre la procédure d’inscription dédiée au forum.</p>
	
	<p class="contacts"><a href="http://forum.mezeval.fr">Accéder au forum Mezeval</a></p>
</div>
