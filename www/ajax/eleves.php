<?
include("../../config/config.inc");
session_start();
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

CDebug::TRACE(0,$_POST["action"]);

switch($_POST["action"])
{
	case "load":
		echo json_encode(CEleve::objectsForClasse($_SESSION["classe"]->id));
		break;
	case "delete":
		if ($e = CEleve::objectWithIdAndClasse($_POST["id"],$_SESSION["classe"]->id))
		{
			$e->del();
			echo "true";
		} else  echo "false";
		break;
	case "add":
		CDebug::TRACE(0,"adding");
		$str = stripslashes($_POST["obj"]);
		$obj = (array)json_decode($str);
		CDebug::TRACE(0,$str);
		CDebug::TRACE(0,print_r($obj,true));
		$x = CEleve::objectWithArray($obj);
		$x->classe = $_SESSION["classe"]->id;
		$x->create();
		echo json_encode(CEleve::objectsForClasse($_SESSION["classe"]->id));
		break;
}
