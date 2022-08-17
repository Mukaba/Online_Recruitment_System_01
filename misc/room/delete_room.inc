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
 | Filename: delete_room.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: delete_room.inc

function display_delete_room($level3, $level3_last)
{
	global $building_id;		// available when $level3_last = 1
	global $room_id;			// available when $level3_last = 2, 3
	global $delete;			// available when $level3_last = 3

	// *** START INPUT VALIDATION ***
	switch ($level3_last)
	{
		case "1":
			if (!isValidBuilding($building_id)) { $level3 = "1"; }
			break;

		case "2":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			break;

		case "3":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			elseif ($delete != "Yes") { $level3 = "1"; }
			break;

	}
	// *** END OF INPUT VALIDATION ***
	
				

	
	echo("<h1>" . (($level3 != "4") ? "Delete Room" : "Room Deleted") . "</h1>");
	switch ($level3)
	{
		case "4":
			display_delete_4($room_id);
			break;
		
		case "3":
			display_delete_3($room_id);
			break;

		case "2":
			display_delete_2($building_id);
			break;
			
		case "1":
		default:
			display_delete_1();
			break;
	}
}


function display_delete_1()
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("delete_room", "2", "1"); ?>		
	<table>
		<tr>
			<td valign="top">Building:</td>
			<td valign="top">
<?php display_building_list("building_id", false, true, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center">
		<input type="submit" value="OK" />
		
		</td></tr>
	</table>
	</form>
	
	<?php
}

function display_delete_2($building_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("delete_room", "3", "2"); ?>		
	<table>
<?php
	$building = get_building($building_id);
	display_tr("Building Name:", $building->building_name);
?>
		<tr>
			<td valign="top">Room:</td>
			<td valign="top">
<?php display_room_list("room_id", $building_id, true, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_delete_3($room_id)
{
	global $PHP_SELF;
	
	$room = get_room($room_id);
	?>
	
	
	<table>
	<tr><td colspan="2"><p>Delete <?php echo($room->building->building_code . " " . $room->room_number); ?>?</p></td></tr>
	<tr><td align="right">
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("delete_room", "4", "3"); ?>		
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
		<input type="submit" name="delete" value="Yes" />
	</form>
	</td>
	<td align="left">
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="edit_room" />
		<input type="submit" name="delete" value="No" />
	</form>
	</td>
	</tr>
	</table>
	<hr />

	<table>
<?php
	
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
	display_tr("E-mail:", (($room->email) ? $room->email : "None"));

	if ($room->wait_room) {
		echo("<tr><td>Wait Room:</td><td>" . $room->wait_room->building->building_code . " " . $room->wait_room->room_number . "</td><td><input type='submit' name='edit' value='Edit Wait Room' /></td></tr>");
	}
?>
	</table>
	
	<?php
}


function display_delete_4($room_id)
{
	global $PHP_SELF;
	
	$room = get_room($room_id);
	if (!$room)
	{
		error("<p>Error: Room could not be found.</p>");
	}
	else
	{
		if (delete_room($room_id))
		{
			notify("<p>" . $room->building->building_code . " " . $room->room_number . " was deleted.</p>");
		}
		else
		{	
			notify("<p>" . $room->building->building_code . " " . $room->room_number . " was not deleted.
						(The room is probably booked for an interview.)</p>");
		}
	}	
	?>
	
	<div align="center">
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="add_room" />
		<input type="submit" value="OK" />
	</form>
	</div>
	<?php
}





function delete_room($room_id)
{
	// check if room is in use
	// if it is not delete it

	$result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT	time_id
		FROM		interview_time
		WHERE		int_room_id='" . addslashes($room_id) . "'");

	if ($result)
	{
		$delete_room = !$result->RecordCount();
		$result->Close();
	}

	if (!$delete_room) { return false; }

	$result = $GLOBALS['dbh']->Execute("
		DELETE FROM	room
		WHERE	room_id='" . addslashes($room_id) . "'");
	
	return ($result) ? true : false;
}





?>
