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
 | Filename: new_room.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: new_room.inc

/*	Programmer's Notes:
 * -
 * 2000-11-21 09:31 Stephen
 * The e-mail address, if present, should be checked to see if it is of valid form.
 * -
 */

function display_new_room($level3, $level3_last)
{
	global $building_id;					// available when $level3_last = 1, 2, 3, 4, 5
	global $room_number;					// available when $level3_last = 2, 3, 4, 5
	global $phone;							// available when $level3_last = 2, 3, 4, 5
	global $email;							// available when $level3_last = 2, 3, 4, 5
	global $room_type;					// available when $level3_last = 2
	global $edit_wait_room;				// available when $level3_last = 3
	global $wait_room_building_id;	// available when $level3_last = 4
	global $wait_room_id;				// available when $level3_last = 5

//	switch ($level3_last)
//	{
//		case "2":
//			if ($room_type == "Interview Room") { $level3 = "3"; }
//			elseif ($room_type == "Wait Room") { $level3 = "6"; }
//			else { $level3 = "1"; }
//			break;
//		
//		case "3";
//			if ($edit_wait_room == "Self") { $level3 = "6"; }
//			elseif ($edit_wait_room == "Other") { $level3 = "4"; }
//			else { $level3 = "2"; }
//			break;
//	}
		
	// *** START INPUT VALIDATION ***
	switch ($level3_last)
	{
		case "1":
			// if $building_id isn't valid, go back.
			if (!isValidBuilding($building_id)) { $level3 = 1; }
			break;
		
		case "2":
			// if $building_id isn't valid, go to "1".
			// if $room_number is empty go to "2".
			// $phone may be empty
			// $email may be empty but should check for valid email address if present
			// if $room_type != "Interview Room" || "Wait Room" go to "2".

			if (!isValidBuilding($building_id)) { $level3 = 1; }
			elseif((trim($room_number) == ""))	// || !($room_type == "Interview Room" || $room_type == "Wait Room"))
			{
				echo "No room defined";
				$level3 = 2;
			}elseif(roomAlreadyExist($room_number, $building_id)){
				$level3=2;
				error("This room already exists.  Use edit menu to edit the room.");
			}
			break;
			
		case "3":
			if (!isValidBuilding($building_id)) { $level3 = 1; }
			elseif((trim($room_number) == "")) { $level3 = 2; }
			//elseif(($edit_wait_room != "Self") && ($edit_wait_room != "Other")) { $level3 = 3; }
			break;
			
		case "4":
			if (!isValidBuilding($building_id)) { $level3 = 1; }
			elseif((trim($room_number) == "")) { $level3 = 2; }
			elseif(!isValidBuilding($wait_room_building_id)) { $level3 = 4; }
			break;
	
		case "5":
			if (!isValidBuilding($building_id)) { $level3 = 1; }
			elseif((trim($room_number) == "")) { $level3 = 2; }
			elseif(!isValidWaitRoom($wait_room_id)) { $level3 = 4; }
			break;
	}
	// *** END OF INPUT VALIDATION ***

	
	// *** INSERT NEW ROOM (if $level3 == "6") ***
	if ($level3 == "6") // time to run updates
	{
		// verify room with same building, number is not in room tbl
		// make sure wait_room_id, if present, is valid
				
		switch ($level3_last)
		{
			case "2":
				// no wait_room
				$room_id = new_room($building_id, $room_number, $phone, $email);
				break;
				
			case "3":
				// wait_room = room
				$room_id = new_room($building_id, $room_number, $phone, $email, true);
				break;
				
			case "5":
				// other wait_room
				$room_id = new_room($building_id, $room_number, $phone, $email, true, $wait_room_id);
				break;				
		}
	}
	// *** END OF INSERT NEW ROOM ***

				
		echo "<h3>Room Editing - New Room</h3>";
	if ($level3 == "6") {
		notify("New Room Added");
	} else { 
	}
	// echo("<h3>" . (($level3 != "6") ? "New Room" : "New Room Added") . "</h3>");
	switch ($level3)
	{
		case "6":
			display_new_room_6($room_id);
			break;

		case "5":
			display_new_room_5($building_id, $room_number, $phone, $wait_room_building_id, $email);
			break;

		case "4":
			display_new_room_4($building_id, $room_number, $phone, $email);
			break;

		case "3":
			display_new_room_3($building_id, $room_number, $phone, $email);
			break;

		case "2":
			display_new_room_2($building_id);
			break;

		case "1":
		default:
			display_new_room_1();
			break;
	}
}

function display_new_room_1()
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_room", "2", "1"); ?>		
	<table class="row1" border="0" cellspacing="0" cellpadding="4" width="75%">
		<tr>
			<td valign="top">Building:</td>
			<td valign="top">
<?php display_building_list("building_id", true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><hr /><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_new_room_2($building_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_room", "6", "2"); ?>
		<input type="hidden" name="building_id" value="<?php echo($building_id); ?>" />
	<table class="row1" cellspacing="0" cellpadding="4" border="0">
<?php
	$building = get_building($building_id);
	display_tr("Building:", "<b class=black>".$building->building_name."</b>");
?>
		<tr>
			<td valign="top">Room Number:</td>
			<td valign="top"><input type="text" name="room_number" maxlength="10" /></td>
		</tr>
		<tr>
			<td valign="top">Phone:</td>
			<td valign="top"><input type="text" name="phone" maxlength="25" /></td>
		</tr>
		<tr>
			<td valign="top">E-mail:</td>
			<td valign="top"><input type="text" name="email" maxlength="50" /></td>
		</tr>
		<tr>
			<td valign="top" colspan=2 align=center><hr /><input type="submit" name="room_type" value="Add Room" /></td>
		</tr>
		<!--<tr><td>This is a(n)</td><td><input type="submit" name="room_type" value="Interview Room" /></td></tr>
		<tr><td></td><td><input type="submit" name="room_type" value="Wait Room" /></td></tr>-->
	</table>
	
	</form>
	<?php
}

function display_new_room_3($building_id, $room_number, $phone, $email)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_room", "4_or_6", "3"); ?>		
		<input type="hidden" name="building_id" value="<?php echo($building_id); ?>" />
		<input type="hidden" name="room_number" value="<?php echo($room_number); ?>" />
		<input type="hidden" name="phone" value="<?php echo($phone); ?>" />
		<input type="hidden" name="email" value="<?php echo($email); ?>" />

	<table class="row1" border="0" cellspacing="0" cellpadding="4">
<?php
	$building = get_building($building_id);
	display_building_room_phone($building->building_name, $room_number, $phone);
	display_tr("E-mail:", $email);
	echo("<tr><td>Wait Room:</td><td><input type='submit' name='edit_wait_room' value='Self' /></td></tr>");
	echo("<tr><td colspan=2 align=center><hr /><input type='submit' name='edit_wait_room' value='Other' /></td></tr>");
?>
	</table>
	
	</form>
	<?php
}

function display_new_room_4($building_id, $room_number, $phone, $email)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_room", "5", "4"); ?>
		<input type="hidden" name="building_id" value="<?php echo($building_id); ?>" />
		<input type="hidden" name="room_number" value="<?php echo($room_number); ?>" />
		<input type="hidden" name="phone" value="<?php echo($phone); ?>" />
		<input type="hidden" name="email" value="<?php echo($email); ?>" />
			
	<table>
<?php
	$building = get_building($building_id);
	display_building_room_phone($building->building_name, $room_number, $phone);
	display_tr("E-mail:", $email);
?>
		<tr>
			<td valign="top"><b>Wait Room Building:</b></td>
			<td valign="top">
<?php display_building_list("wait_room_building_id", false, false, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_new_room_5($building_id, $room_number, $phone, $wait_room_building_id, $email)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<?php	hidden_form_inputs("new_room", "6", "5"); ?>
		<input type="hidden" name="building_id" value="<?php echo($building_id); ?>" />
		<input type="hidden" name="room_number" value="<?php echo($room_number); ?>" />
		<input type="hidden" name="phone" value="<?php echo($phone); ?>" />
		<input type="hidden" name="email" value="<?php echo($email); ?>" />
			
	<table>
<?php
	$building = get_building($building_id);
	display_building_room_phone($building->building_name, $room_number, $phone);
	display_tr("E-mail:", $email);
?>
		<tr>
			<td valign="top"><b>Wait Room:</b></td>
			<td valign="top">
<?php display_room_list("wait_room_id", $wait_room_building_id, false, true); ?>
			</td>
		</tr>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}

function display_new_room_6($room_id)
{
	global $PHP_SELF;
	?>
	<form action="<?php echo($PHP_SELF); ?>" method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="add_room" />
	
	<table>
<?php
	$room = get_room($room_id);
	display_building_room_phone($room->building->building_name, $room->room_number, $room->phone);
	display_tr("<b>E-mail:</b>", $room->email);
	
	if ($room->wait_room) {
		display_tr("Wait Room:", $room->wait_room->building->building_code . " " . $room->wait_room->room_number);
	}
?>
		<tr><td colspan="2" align="center"><input type="submit" value="OK" /></td></tr>
	</table>
	
	</form>
	<?php
}





function new_room($building_id, $room_number, $phone, $email, $wait_room=false, $wait_room_id=false)
{
	$room_number = trim($room_number);
	
	$result = $GLOBALS['dbh']->Execute("
		SELECT	room_id
		FROM		room
		WHERE		building_id='" . addslashes($building_id) . "' AND
					room_number='" . addslashes($room_number) . "'");
	if (!$result) { return false; }
	if ($result->RecordCount())
	{
		$row = $result->FetchRow();
		return $row["room_id"];
	}

	if (!$wait_room_id)
	{
		$result = $GLOBALS['dbh']->Execute("
			INSERT INTO room
			(room_number, phone, email, building_id)
			VALUES
			('" . addslashes($room_number) . "', '" . addslashes($phone) . "', '" . addslashes($email) . "', '" . addslashes($building_id) . "')");

		if ($result) { $insert_id = $GLOBALS['dbh']->Insert_ID(); }
		if ($wait_room)
		{
			$result2 = $GLOBALS['dbh']->Execute("
				UPDATE	room
				SET		wait_room_id=room_id
				WHERE		building_id='" . addslashes($building_id) . "' AND
							room_number='" . addslashes($room_number) . "' AND
							phone='" . addslashes($phone) . "' AND
							email='" . addslashes($email) . "'");
		}
	}
	else
	{
		$result = $GLOBALS['dbh']->Execute("
			INSERT INTO room
			(room_number, phone, email, building_id, wait_room_id)
			VALUES
			('" . addslashes($room_number) . "', '" . addslashes($phone) . "', '" . addslashes($email) . "', '" . addslashes($building_id) . "', '" . addslashes($wait_room_id) . "')");
		
		if ($result) { $insert_id = $GLOBALS['dbh']->Insert_ID(); }
	}
	
	return ($result) ? $insert_id : false;
}


function roomAlreadyExist($room_number, $building_id){
	$sql = "SELECT room_id
		FROM room
		WHERE room_number = '" . addslashes(stripslashes($room_number)) . "' AND
			building_id = '" . addslashes(stripslashes($building_id)) . "'";
	$result = $GLOBALS['dbh']->Execute( $sql );

	if (!$result){ return false; }
	elseif($result->RecordCount() >= 1){ return true; }
	else{ return false; }
}


?>
