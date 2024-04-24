<?
include("../../config/config.inc");
session_start();
header('Cache-Control: no-cache, must-revalidate');
header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
header('Content-type: application/json');

switch($_POST["action"])
{
	case "load":
		echo json_encode(CPeriode::objectsForClasse($_SESSION["classe"]->id));
		break;
	case "delete":
		if ($e = CPeriode::objectWithIdAndClasse($_POST["id"],$_SESSION["classe"]->id))
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
		$x = new CPeriode;
		$x->initWithJson((array)$obj);
		$x->classe = $_SESSION["classe"]->id;
		$x->create();
		echo json_encode(CPeriode::objectsForClasse($_SESSION["classe"]->id));
		break;
	case "import":
		echo json_encode(CPeriode::objectsForClasse($_SESSION["classe"]->id));
		break;
	case "update":
		$str = stripslashes($_POST["obj"]);
		$obj = (array)json_decode($str);
		print_r($obj);
		$x = CPeriode::objectWithArray($obj);
		$x->debut = CDateUtil::unixDate($obj["debut"]);
		$x->fin = CDateUtil::unixDate($obj["fin"]);
		$x->update();
		CDebug::TRACE(0,print_r($x));
		return "true";
}
