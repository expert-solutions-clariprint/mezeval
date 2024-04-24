<?

if ($classe = $_SESSION["classe"])
{
	if ($type = $_GET["type"])
	{
		if ($code = $_GET["code"])
		{
			$socle = null;
			if ($type == "langue")
				$socle = $classe->langue();
			else if ($type == "langue")
				$socle = $classe->langue();
			else $socle = $classe->socle();
			if ($r = $socle->getElementWithCode($code))
				echo $r->info();
			else echo "?";
		}
	}
}