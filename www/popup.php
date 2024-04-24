<? 
include("../config/config.inc");
session_start();
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" >
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr">
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
	</head>
	<body class="popup"><?
			$module = $_GET["module"];
			if (CTemplate::isModule($module))
			{
				if 	(CUser::check($module) || CTemplate::isOpen($module)) {
					CTemplate::includeModule($module);
			} ?>
	</body>
</html>
