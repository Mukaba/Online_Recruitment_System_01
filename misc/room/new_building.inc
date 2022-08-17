<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: new_building.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: new_building.inc

function display_new_building($level3, $level3_last)
{
	if ($level3 == "2") // time to run updates
	{
		// make sure building_name and building_code are present
		// verify same building_name or building_code is not in buiding tbl
		global $building_name;
		global $building_code;

		$building_name = trim($building_name);
		$building_code = trim($building_code);
		
		if ($building_name && $building_code)
		{
			$building_id = create_building($building_name, $building_code);
		}
		else
		{
			$level3 = "1.5";
		}
	}
		echo "<h3>Room Editing - New Building</h3>";
	
	if ($level3 == "2") { 
		if ($building_id) {
			notify("New Building Added");
		} else {
			error("Building Already Exists");
		}
	} else {
	}
	//echo("<h3>" . (($level3 != "2") ? "New Building" : "New Building Added") . "</h3>");
	switch ($level3)
	{
		case "2":
			if ($building_id) {
				display_new_building_2($building_id);
			} else {
				$level3 = 1;
				display_new_building_1();
			}
			break;

		case "1.5":
			global $building_name;
			global $building_code;
			display_new_building_1(trim($building_name), trim($building_code));
			break;
			
		case "1":
		default:
			display_new_building_1();
			break;
	}
}


function display_new_building_1($building_name="", $building_code="")
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_building", "2", "1"); ?>
	<table class='row1' border='0' cellspacing='0' cellpadding='4'>
		<tr>
			<td>Building Name:</td>
			<td><input type="text" name="building_name" maxlength="50" value="<?php echo($building_name); ?>" /></td>
		</tr>
		<tr>
			<td>Building Code:</td>
			<td><input type="text" name="building_code" maxlength="10" value="<?php echo($building_code); ?>" /></td>
		</tr>
		<tr><td colspan="2" align="center"><hr /><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_new_building_2($building_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="add_room" />
	<table>	
<?php
	$building = get_building($building_id);
	display_tr("<b>Building Name:</b>", $building->building_name);
	display_tr("<b>Building Code:</b>", $building->building_code);
?>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}



function create_building($building_name, $building_code)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	building_id
		FROM		building
		WHERE		building_name='" . addslashes($building_name) . "' OR
					building_code='" . addslashes($building_code) . "'");

	if (!$result || $result->RecordCount()) { return false; }

	$result = $GLOBALS['dbh']->Execute("
		INSERT INTO building (building_name, building_code)
		VALUES ('" . addslashes($building_name) . "', '" . addslashes($building_code) . "')");


	return (($result) ? $GLOBALS['dbh']->Insert_ID() : false);
}



?>
