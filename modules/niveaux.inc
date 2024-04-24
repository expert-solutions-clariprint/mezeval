<?
if (!isset($classe)) $classe = $_SESSION["classe"];

if (!isset($classe)) return;
if ($_POST["save"])
{
	$classe->a_color = $_POST["a_color"];
	$classe->b_color = $_POST["b_color"];
	$classe->c_color = $_POST["c_color"];
	$classe->z_color = $_POST["z_color"];

	$classe->a_title = stripslashes($_POST["a_title"]);
	$classe->b_title = stripslashes($_POST["b_title"]);
	$classe->c_title = stripslashes($_POST["c_title"]);
	$classe->z_title = stripslashes($_POST["z_title"]);

	$classe->a_code = $_POST["a_code"];
	$classe->b_code = $_POST["b_code"];
	$classe->c_code = $_POST["c_code"];
	$classe->z_code = $_POST["z_code"];
	$classe->update();
}

?>

	<table class="liste alternate">
		<thead>
			<tr>
				<td>Code</td>
				<td>Intitulé</td>
				<td>Couleur</td>
			</tr>
		</thead>
		<tbody>
			<tr>
				<td><input type="text" name="a_code" value="<?= $classe->a_code() ?>" style="width: 20px"></td>
				<td><input type="text" name="a_title" value="<?= $classe->a_title() ?>" style="width: 200px"></td>
				<td><input type="text" name="a_color" id="a_color" value="<?= $classe->a_color ?>" onclick="openPicker('a_color');"  style="cursor: pointer; background-color: <?= $classe->a_color() ?>; width: 40px"></td>
			</tr>

			<tr>
				<td><input type="text" name="b_code" value="<?= $classe->b_code() ?>" style="width: 20px"></td>
				<td><input type="text" name="b_title" value="<?= $classe->b_title() ?>" style="width: 200px"></td>
				<td><input type="text" name="b_color" id="b_color" value="<?= $classe->b_color ?>" onclick="openPicker('b_color');"  style="cursor: pointer; background-color: <?= $classe->b_color() ?>; width: 40px"></td>
			</tr>

			<tr>
				<td><input type="text" name="c_code" value="<?= $classe->c_code() ?>" style="width: 20px"></td>
				<td><input type="text" name="c_title" value="<?= $classe->c_title() ?>" style="width: 200px"></td>
				<td><input type="text" name="c_color" id="c_color" value="<?= $classe->c_color ?>" onclick="openPicker('c_color');"  style="cursor: pointer; background-color: <?= $classe->c_color() ?>; width: 40px"></td>
			</tr>

			<tr>
				<td><input type="text" name="z_code" value="<?= $classe->z_code() ?>" style="width: 20px"></td>
				<td><input type="text" name="z_title" value="<?= $classe->z_title() ?>" style="width: 200px"></td>
				<td><input type="text" name="z_color" id="z_color" value="<?= $classe->z_color ?>" onclick="openPicker('z_color');"  style="cursor: pointer; background-color: <?= $classe->z_color() ?>; width: 40px"></td>
			</tr>
		</tbody>
		<tfoot>
			<tr>
				<td colspan="4">
					<input type="submit" name="save" value="Enregistere" id="save">
				</td>
			</tr>
		</tfoot>
			
	</table>
