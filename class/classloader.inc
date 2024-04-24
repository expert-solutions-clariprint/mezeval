<?
if (version_compare("5", PHP_VERSION, ">")) {
	echo "la";
	require_once("class/cdatabase.phpclass");
	require_once("class/cuser.phpclass");
	require_once("class/cevaluationstats.phpclass");
	require_once("class/cevaluationeleve.phpclass");
	require_once("class/cperiode.phpclass");
	require_once("class/celeve.phpclass");
	require_once("class/cetablissement.phpclass");
	require_once("class/cclasse.phpclass");
	require_once("class/cprogramme.phpclass");
	require_once("class/ccompetence.phpclass");
	require_once("class/cevaluationcompetence.phpclass");
	require_once("class/cevaluation.phpclass");
	require_once("class/cusergroup.phpclass");
	require_once("class/classloader.inc");
	require_once("class/cticket.phpclass");
	require_once("class/cpticket.phpclass");
	require_once("class/cuseretablissement.phpclass");
	require_once("class/ccontrat.phpclass");
	require_once("class/cdateutil.phpclass");
	require_once("class/ctemplate.phpclass");
	require_once("class/cdebug.phpclass");
}

function __autoload($class_name)
{
	if (!require_once(APP_PATH . "/class/" . strtolower($class_name) . ".phpclass"))
		die( "ERREUR DE CHARGEMENT DE $class_name");
}

