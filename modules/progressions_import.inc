<?
include("classe_header.inc");
?>
<div id="body" class="progressions">
	<h1>Importation de fichier CSV (Excel)</h1>
	
	<h2>Procédure</h2>
	<ol>
		<li>Dans Excel organiser les colonnes dans l'ordre suivant : "code de la progression ou numero","intitulé de la progression", "reférence socle commun"

if ($_GET["step"] == "1")
{
	?>
