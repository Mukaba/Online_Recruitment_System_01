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
 | Filename: edit_room.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: edit_room.inc

/*Programmer's Notes:
 *-
 * Modified checking routines to make sure the phone number given for a room is valid
 * or not there (representing "empty").
 *
 * On editing the email address, if present, it should be checked as to whether it is valid
 */

function display_edit_room($level3, $level3_last)
{
	global $building_id;		// available when $level3_last = 1
	global $room_id;			// available when $level3_last = 2, 3, 4a, 4b, 5, 6
	global $edit;				// available when $level3_last = 3
	global $phone;				// available when $level3_last = 4a (may be empty)
	global $edit_wait_room;	// available when $level3_last = 4b
	global $email;				// available when $level3_last = 4c (may be empty)
	
	global $wait_room_building_id;	// available when $level3_last = 5
	global $wait_room_id;	// available when $level3_last = 6


/*No longer use wait room
No longer user the multiple edit forms	
	if ($level3_last == "3")
	{
		switch ($edit)
		{
			case "Edit Phone":
				$level3 = "4a";
				break;
			
			case "Edit Wait Room":
				$level3 = "4b";
				break;
			
			case "Edit E-mail":
				$level3 = "4c";
				break;
			
			default:
				$level3 = "1";
				break;
		}
	}

	if ($level3_last == "4b")
	{
		if ($edit_wait_room == "Self") { $level3 = "7"; }
		elseif ($edit_wait_room == "Other") { $level3 = "5"; }
		else { $level3 = "1"; }
	}
*/
	
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

                        // Phone validation added by Bill 2001-10-05 11:17 pm
			else if (!isValidPhoneNumber ($phone) && strlen ($phone))
                        {
			  error ("Phone number must be in (xxx) xxx-xxxx format.");
                          $level3 = 3;
                        }

			elseif($edit == "Cancel"){$level3="1";}elseif ($edit == "Update"){$level3 = "7";}else{$level3="3";}
			//elseif (	$edit != "Edit Phone" && $edit != "Edit E-mail" &&
			//			$edit != "Edit Wait Room" && $edit != "OK") { $level3 = "3"; }
			break;

		case "4a":
			if (!isValidRoom($room_id)) { $level3 = "1"; }

                        // Phone validation added by Bill 2001-10-05 11:17 pm
			if (!isValidPhoneNumber ($phone) && strlen ($phone)) 
			{
			  error ("Phone number must be in (xxx) xxx-xxxx format.");
			  $level3 = "4a";
			}

			break;

		case "4b":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			elseif ($edit_wait_room != "Self" && $edit_wait_room != "Other") { $level3 = "4b"; }
			break;

		case "4c":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			break;
		
		case "5":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			elseif (!isValidBuilding($wait_room_building_id)) { $level3 = "5"; }
			break;
	
		case "6":
			if (!isValidRoom($room_id)) { $level3 = "1"; }
			elseif (!isValidWaitRoom($wait_room_id)) { $level3 = "5"; }
			break;
	}
	// *** END OF INPUT VALIDATION ***
	
				
	if ($level3 == "7") // time to run updates
	{
		switch ($level3_last)
		{
			
			case "3":
				$success = update_phone($room_id, $phone);
				$success = update_email($room_id, $email);
				break;
			case "4a":
				// update phone number				
				$success = update_phone($room_id, $phone);
				break;
		
//			case "4b":
//				// update wait room with self
//				$success = update_wait_room($room_id);
//				break;
		
			case "4c":
				// update email address
				$success = update_email($room_id, $email);
				break;
			
//			case "6":
//				// update wait room with other
//				// check to make sure wait_room_id is valid
//				$success = update_wait_room($room_id, $wait_room_id);
//				break;
		}
	}


	if ($level3 == "7") {
		notify("Room Edited");
	} else {
		echo "<h3>Room Editing - Edit Room</h3>";
	}
	//echo("<h3>" . (($level3 != "7") ? "Edit Room" : "Room Edited") . "</h3>");
	switch ($level3)
	{
		case "7":
			display_edit_3($room_id);
			//display_edit_7($room_id);
			break;
	
		case "6":
			display_edit_6($room_id, $wait_room_building_id);
			break;

		case "5":
			display_edit_5($room_id);
			break;

		case "4c":
			display_edit_4c($room_id);
			break;
					
		case "4b":
			display_edit_4b($room_id);
			break;
			
		case "4a":
			display_edit_4a($room_id);
			break;
			
		case "3":
			display_edit_3($room_id);
			break;
		
		case "2":
			display_edit_2($building_id);
			break;
			
		case "1":
		default:
			display_edit_1();
			break;
	}
}


function display_edit_1()
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "2", "1"); ?>		
	<table class="row1" border="0" cellspacing="0" cellpadding="4" width="75%">
		<tr>
			<td valign="top">Building:</td>
			<td valign="top">
<?php display_building_list("building_id", false, true, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><hr /><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_edit_2($building_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "3", "2"); ?>		
	<table class="row1" border="0" cellspacing="0" cellpadding="4">
<?php
	$building = get_building($building_id);
	echo("<tr><td>Building Name: </td><td><b class=black>". $building->building_name."</b></td></tr>");
?>
		<tr>
			<td valign="top">Room:</td>
			<td valign="top">
<?php display_room_list("room_id", $building_id, true, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><hr /><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_edit_3($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "4", "3"); ?>		
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
	<table class="row1" border="0" cellspacing="0" cellpadding="4"> 
<?php
	$room = get_room($room_id);
	display_tr("Building Name:", "<b class=black>".$room->building->building_name."</b>", true);
	display_tr("Room Number:", $room->room_number, true);
	echo("<tr><td>Phone:</td><td><input type='text' name='phone' maxlength='25' value='$room->phone' />(" . (($room->phone != "") ? $room->phone : "none") . ")</td></tr>\n");
	echo("<tr><td>E-mail:</td><td><input type='text' name='email' maxlength='50' value='$room->email' />(" . (($room->email!= "")? $room->email : "none") . ")</td></tr>\n");
	//echo("<tr><td>Phone:</td><td>" . $room->phone . "</td><td><input type='submit' name='edit' value='Edit Phone' /></td></tr>");
	//echo("<tr><td>E-mail:</td><td>" . (($room->email) ? $room->email : "None") . "</td><td><input type='submit' name='edit' value='Edit E-mail' /></td></tr>");

//	if ($room->wait_room) {
//		echo("<tr><td>Wait Room:</td><td>" . $room->wait_room->building->building_code . " " . $room->wait_room->room_number . "</td><td><input type='submit' name='edit' value='Edit Wait Room' /></td></tr>");
//	}
?>
		<tr><td colspan="2" align="center"><hr /><input type="submit" name="edit" value="Update" />&nbsp;&nbsp;&nbsp;<input type="submit" name="edit" value="Cancel" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_edit_4a($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "7", "4a"); ?>		
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
			
	<table class="row1" border="0" cellspacing="0" cellpadding="4">
<?php
	$room = get_room($room_id);
	display_tr("Building Name:", "<b class=black>".$room->building->building_name."</b>");
	display_tr("Room Number:", $room->room_number);
	echo("<tr><td>Phone:</td><td><input type='text' maxlength='25' name='phone' value='" . $room->phone . "' /></td></tr>");

//	if ($room->wait_room) {
//		echo("<tr><td>Wait Room:</td><td>" . $room->wait_room->building->building_code . " " . $room->wait_room->room_number . "</td></tr>");
//	}
?>
		<tr><td colspan="2" align="center"><hr /><input type="submit" name="edit_wait_room" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

/*
function display_edit_4b($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "5_or_7", "4b"); ?>
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
			
	<table>
<?php
	$room = get_room($room_id);
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
	echo("<tr><td>Wait Room: </td><td><input type='submit' name='edit_wait_room' value='Self' /></td></tr>");
	echo("<tr><td></td><td><input type='submit' name='edit_wait_room' value='Other' /></td></tr>");
?>
	</table>
	
	</form>
	<?php
}
*/

function display_edit_4c($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "7", "4c"); ?>
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
			
	<table class="row1" border="0" cellspacing="0" cellpadding="4">
<?php
	$room = get_room($room_id);
	display_tr("Building Name:", "<b class=black>".$room->building->building_name."</b>");
	display_tr("Room Number:", $room->room_number);
	echo("<tr><td>Phone:</td><td>" . $room->phone . "</td></tr>");
	echo("<tr><td>E-mail:</td><td><input type='text' maxlength='50' name='email' value='" . $room->email . "' /></td></tr>");

//	if ($room->wait_room) {
//		echo("<tr><td>Wait Room:</td><td>" . $room->wait_room->building->building_code . " " . $room->wait_room->room_number . "</td></tr>");
//	}
?>
		<tr><td colspan="2" align="center"><hr /><input type="submit" name="edit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

/*
function display_edit_5($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "6", "5"); ?>
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
			
	<table>
<?php
	$room = get_room($room_id);
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
?>
		<tr>
			<td valign="top">Wait Room Building</td>
			<td valign="top">
<?php display_building_list("wait_room_building_id", false, false, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}
*/


/*
function display_edit_6($room_id, $wait_room_building_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("edit_room", "7", "6"); ?>
		<input type="hidden" name="room_id" value="<?php echo($room_id); ?>" />
			
	<table>
<?php
	$room = get_room($room_id);
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
?>
		<tr>
			<td valign="top">Wait Room:</td>
			<td valign="top">
<?php display_room_list("wait_room_id", $wait_room_building_id, false, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}
*/

function display_edit_7($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="add_room" />
	
	<table class="row1" border="0" cellspacing="0" cellpadding="4">
<?php
	$room = get_room($room_id);
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
	display_tr("E-mail:", (($room->email) ? $room->email : "None"));
	
//	if ($room->wait_room) {
//		display_tr("Wait Room:", $room->wait_room->building->building_code . " " . $room->wait_room->room_number);
//	}
?>
<!--		<tr><td colspan="2" align="center"><input type="submit" name="edit_wait_room" value="OK" /></td></tr>-->
	</table>
	
	</form>
	<?php
}





function update_phone($room_id, $phone)
{
	$phone = trim($phone);
	$result = $GLOBALS['dbh']->Execute("
		UPDATE room
		SET phone=" . (($phone) ? ("'" . addslashes(trim($phone)) . "'") : "NULL") . "
		WHERE room_id='" . addslashes($room_id) . "'");

	return ($result) ? true : false;
}





function update_email($room_id, $email)
{
	$email = trim($email);
	$result = $GLOBALS['dbh']->Execute("
		UPDATE room
		SET email=" . (($email) ? ("'" . addslashes($email) . "'") : "NULL") . "
		WHERE room_id='" . addslashes($room_id) . "'");

	return ($result) ? true : false;
}




/*
function update_wait_room($room_id, $wait_room_id=false)
{
	// $wait_room_id was already checked to be valid
	if (!$wait_room_id) { $wait_room_id = $room_id; }
	
	$result = $GLOBALS['dbh']->Execute("
		UPDATE 	room
		SET		wait_room_id='" . addslashes($wait_room_id) . "'
		WHERE		room_id='" . addslashes($room_id) . "'");
		
	return ($result) ? true : false;
}

*/



?>
