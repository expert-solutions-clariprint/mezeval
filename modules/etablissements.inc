<? 
CTemplate::draw_main_menu("");

print_r($_POST);
print_r($_FILES);
if ($_POST["import"] && $_FILES["file_import"])
{
	$parser = xml_parser_create();
	xml_set_element_handler($parser,"xml_start_element_handler","xml_end_element_handler");
	$fp = fopen( $_FILES["file_import"]["tmp_name"],"r");
	if ($fp)
	{
		while ($data = fread($fp, 4096))
		{
			if (!xml_parse($parser, $data, feof($fp))) {
				printf("XML error: %s at line %d",
										xml_error_string(xml_get_error_code($parser)),
										xml_get_current_line_number($parser));
				break;
			}
		}
		fclose($fp);
    }
	xml_parser_free($parser);
}
$xml_current_object = null;
function xml_start_element_handler($parser, $name, $attrs) 
{
	global $xml_current_object;
	global $count;
	switch($name)
	{
		case "ETABLISSEMENT" :
			$xml_current_object = new CEtablissement();
			$count++;
			break;
		case "KEY" :
			if ($xml_current_object)
			{
				if ($attrs["VALUE"])
				{
					$val = $attrs["VALUE"];
					switch ($attrs["NAME"])
					{
						case "N°UAI" :
							$xml_current_object->uai =  $val;
							break;
						case "Etat":
							if ($val =="FERME") {
								$xml_current_object = null;
								$count--;
							}
							break;
//						case "Ouverture=10/08/1965
						case "N°SIRET" :
							$xml_current_object->siret =  $val;
							break;
						
//						case "Secteur=PU
//						case "Fermeture=
//						case "N°FINESS=
//						case "Contrat=SANS OBJET ( 99 )
//						case "MAJ=02/05/2007
//						case "Sigle=ECOLE ELEMENTAIRE PUBLIQUE ( E.E.PU )
						case "Tel" :
							$xml_current_object->contact_telephone =  $val;
							break;
						case "Appellation" :
							$xml_current_object->nom =  $val;
							break;
						case "Fax" :
							$xml_current_object->contact_fax =  $val;
							break;
						case "Dénomination" : 
							if (!$xml_current_object->nom) $xml_current_object->nom = $val;
							break;
						case "Mel" :
							$xml_current_object->contact_email =  $val;
							break;
						case "Patronyme" :
							$xml_current_object->contact_nom =  $val;
							break;
//						case "Mention distribution"
						case "Adresse" :
							$xml_current_object->contact_adresse_1 .=  $val;
							break;
						case "Lieu dit" :
						case "Boîte postale" :
							$xml_current_object->contact_adresse_2 .=  $val;
							break;
						case "Code postal" :
							$xml_current_object->contact_codepostal .=  $val;
							break;
						case "Acheminement" : 
							$xml_current_object->contact_ville =  $val;
							break;
					}
				}
			}
	}
}

function xml_end_element_handler($parser, $name) 
{
	global $xml_current_object;
	global $count;
	switch($name)
	{
		case "ETABLISSEMENT" :
			if ($xml_current_object) $xml_current_object->create($DB);
			break;
		case "ETABLISSEMENTS" :
			echo "Importation de $count etablissements";
	}
}

?>
<div id="body" class="etablissements">
	<h2>Etablissements</h2>
	<table>
		<thead>
			<tr>
				<th>Nom</th>
				<th>Uai / siret</th>
				<th>adresse</th>
				<th>cp</th>
				<th>ville</th>
				<th>tél.</th>
				<th>email</th>
			</tr>
		</thead>
		<tbody>
			<? foreach(CEtablissement::objects() as $e) { ?>
			<tr>
				<td><?= $e->nom ?></td>
				<td><?= $e->uai ?><br><?= $e->siret ?></td>
				<td><?= $e->contact_adresse_1 ?><br><?= $e->contact_adresse_2 ?></td>
				<td><?= $e->contact_codepostal ?></td>
				<td><?= $e->contact_ville ?></td>
				<td><?= $e->contact_telephone ?></td>
				<td><?= $e->contact_email ?></td>
				<td><a href="?module=contrats&id=<?= $e->id ?>">infos</a></td>
				<td><a href="?module=utilisateurs&etablissement=<?= $e->id ?>">utilsateurs</a></td>
			</tr><? } ?>
		</tbody>
	</table>
	<form method="post" ENCTYPE="multipart/form-data">
		<h2>Importer : </h2>
		<input type="file" name="file_import"><input type="submit" name="import" value="importer">
	</form>
</div>

