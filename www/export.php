<? 
include("../config/config.inc");
session_start();
CUser::catchevents();

$module = $_GET["module"];

if ($module == "") $module = "default";
if (CTemplate::isModule($module))
{
	if 	(CUser::check($module) || CTemplate::isOpen($module))
	{
		CTemplate::includeModule($module);
	} else include(APP_PATH  . "/modules/login.inc");
} else { ?>Erreur<? }
