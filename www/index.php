<? 
include("../config/config.inc");
session_start();
CUser::catchevents();
CTemplate::set_main_menu_handler("mezeval_main_menus_setup");
function HELP_MENU()
{
	?><div <?= $sel ?> class='<?= "$sel" ?>'><a target="_blank" href="http://forum.mezeval.fr/viewforum.php?f=12">Forum et support</a></div><?
}
function APROPOS()
{
	?><div <?= $sel ?> class='<?= "$sel" ?>'><a target="_blank" href="http://www.mezeval.com/fr/mezeval/mezeval-online">À propos</a></div><?
}
function mezeval_main_menus_setup()
{
	CDebug::TRACE(0,"mezeval_main_menus_setup()");
	if ($u = CUser::getCurrentUser())
	{
		$MAIN_MENUS["connexion"] = "Mon Compte";
		if ($u->administrator)
		{
			$MAIN_MENUS["gestion_classes"] = "Mes groupes";
			$MAIN_MENUS["etablissement_stats"] = "Statistiques";
			$MAIN_MENUS["contrats"] = "Commandes";
		} else 
		{
			$MAIN_MENUS["classes"] = "Mes groupes";

		}
		$MAIN_MENUS["connexion&logout=1"] = "Déconnexion";
	} else {
		$MAIN_MENUS = array("connexion" => "Accueil");
	}
	$MAIN_MENUS["*APROPOS"] = "À propos";
	$MAIN_MENUS['*HELP_MENU'] = "Forum et Support";
	return $MAIN_MENUS;
}

/* echo("<?xml version=\"1.0\" encoding=\"utf-8\"?>") 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
        "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
*/
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" >

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr">
<? if (!$_GET["no_html_head"]) {  ?>
<head>
	<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
	<link rel="stylesheet" rev="stylesheet" href="style.css" media="all"/>
	<script src="js/prototype.js" language="javascript" type="text/javascript"></script>
	<script src="utils.js" language="javascript" type="text/javascript"></script>
	<script src="js/xlforms.js" language="javascript" type="text/javascript"></script>
	<script src="js/dialog.js" language="javascript" type="text/javascript"></script>
	<script src="js/numberformat.js" language="javascript" type="text/javascript"></script>
	<script src="js/scriptaculous.js" language="javascript" type="text/javascript"></script>
	<script src="js/livepipe/livepipe.js" type="text/javascript" charset="utf-8"></script>
	<script src="js/livepipe/resizable.js" type="text/javascript" charset="utf-8"></script>
	<script src="js/livepipe/window.js" type="text/javascript" charset="utf-8"></script>
	<script src="js/xldate.js" type="text/javascript" charset="utf-8"></script>
	<script src="js/xlcontrols.js" type="text/javascript" charset="utf-8"></script>
	<script src="js/mezeval.js" language="javascript" type="text/javascript"></script>
	<meta name="description" content="Outils gratuit de gestion des évaluations des élèves du primaire et de la maternelle à destination des professeurs d'écoles."/>
	<meta name="keywords" content="socle commun de connaissances et de compétences, primaire, maternelle, gestion des évaluations, compétences, progressions, CP, CE1, CE2, CM1, CM2, bulletin scolaire"/>
	<title>MeZéval</title>
</head><? } ?>

<body style="text-align: center">
	<? if (!$_GET["no_body_header"]) { ?>
	<div style="width: 900px; margin: 0px auto; position: relative; text-align: left">
		<div id="header">
			<div id="banner">
				<img src="img/logo_head.png" alt="dep.gif"/>
				<div class="head_pub">
					<p>Solution indépendante financée par la publicité. N'hésitez pas à cliquer dessus pour nous permettre d'améliorer et garder notre totale indépendance. Aucune information n'est fournie à des tiers, ministères inclus.</p>

					
				<script type="text/javascript"><!--
					google_ad_client = "ca-pub-2615991052842794";
					/* header */
					google_ad_slot = "1208041614";
					google_ad_width = 468;
					google_ad_height = 60;
					//-->
					</script>
					<script type="text/javascript" src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
					</script>
				</div>
			</div>
		</div><? }
			$module = $_GET["module"];
//			if ($module == "") $module = "default";
				if (CTemplate::isModule($module))
				{
					if 	(CUser::check($module) || CTemplate::isOpen($module)) {
						CTemplate::includeModule($module);
				} else CTemplate::includeLoginModule();
			} else {  CTemplate::includeLoginModule();}
		if (!$_GET["no_body_footer"]) {  ?>
		<div id="footer">
			<div>
				Mézéval, tous droits réservés, les marques et logos cités appartiennent à leurs ayant droits. <a href="?module=cgventes">Conditions d’utilisation</a>. Conçu, réalisé et hébergé par <a url="http://www.expert-solutions.fr">expert-solutions sarl</a>
			</div>
		</div><? } ?>
	</div>
	<script language="javascript"><? 
	if ($x = $_GET["focus_on"]) { ?>if ($('<?= $x ?>')) $('<?= $x ?>').focus();<? }
	if ($x = $_GET["highlight"]) { ?>addCssClass($('<?= $x ?>'),'highlight');<? } 
	if ($x = $_GET["scrollto"]) { ?>if ($('<?= $x ?>')) $('<?= $x ?>').scrollTo();<? } 
	
	?></script>
</body>
</html>
