<?
CTemplate::draw_main_menu(""); 
?><div id="body"><?
if ($_POST["connect"])
{
	if (CUser::login($_POST["login"],$_POST["password"]))
	{
		echo "connecté";
		?><script language="javascript">
		document.location = document.location;
		</script><? 
	}
}
if (!CUser::is_logged()) { ?>
	<div style="width: 100%; text-align: center">
	<form method="post" id="login" style="margin: 0px auto; text-align: left;">
		email :<br>
		<input type="text" name="login" id="login"><br>
		mot de passe :<br>
		<input type="password" name="password"><input type="submit" name="connect" value="connexion">
		<? /*
		<br>
		<br>
		<br>
		mot de passe oulié ?<br>
	<input type="text" name="email" id="email" value="<?= $email ?>"><input type="submit" name="connect_" value="recevoir un mot de passe"><? */ ?>
	</form></div><? } ?>
</div>
