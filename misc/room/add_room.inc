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
 | Filename: add_room.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: add_room.inc

DEFINE(ADD_ROOM_LIST_LENGTH, 15);

switch ($level2)
{
	case "new_building":
		include("misc/room/new_building.inc");
		display_new_building($level3, $level3_last);
		break;

	case "new_room":
		include("misc/room/new_room.inc");
		display_new_room($level3, $level3_last);
		break;

	case "delete_room":
		include("misc/room/delete_room.inc");
		display_delete_room($level3, $level3_last);
		break;
			
	case "edit_room":
	default:
		include("misc/room/edit_room.inc");
		display_edit_room($level3, $level3_last);
		break;
}



/*************************
 ** END OF PROGRAM FLOW **
 *************************/





/**************************
 ** FUNCTION DEFINITIONS **
 **************************/





/*
function display_main()
{
	global $PHP_SELF;
	$link = $PHP_SELF . "&amp;select=room_booking&amp;level1=add_room";
	?>
	<table width="100%">
		<tr><td><a class="blue" href="<?php echo($link); ?>&amp;level2=new_building">New Building</a></td></tr>
		<tr><td><a class="blue" href="<?php echo($link); ?>&amp;level2=new_room">New Room</a></td></tr>
		<tr><td><a class="blue" href="<?php echo($link); ?>&amp;level2=edit_room">Edit Room</a></td></tr>
	</table>
	<?php
}
*/




function hidden_form_inputs($level2, $level3, $level3_last)
{
?>
	<input type="hidden" name="select" value="room_booking" />
	<input type="hidden" name="level1" value="add_room" />
	<input type="hidden" name="level2" value="<?php echo($level2); ?>" />
	<input type="hidden" name="level3_last" value="<?php echo($level3_last); ?>" />
	<input type="hidden" name="level3" value="<?php echo($level3); ?>" />
<?php
}





// $with_interview_rooms and $with_wait_rooms are only of consequence if !$empty_buildings
function display_building_list($name, $empty_buildings=false, $with_interview_rooms=true, $with_wait_rooms=false)
{
	if ($empty_buildings)
	{
		$result = $GLOBALS['dbh']->Execute("
			SELECT	building_id, building_code
			FROM		building
			ORDER BY	building_code");
	}
	else
	{
		if (!$with_interview_rooms) { $with_wait_rooms = true; }
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.building_id, a.building_code
			FROM building AS a, room AS b
			WHERE a.building_id=b.building_id
			" . ((!$with_interview_rooms) ? "AND ISNULL(b.wait_room_id)" : "") . "
			" . ((!$with_wait_rooms) ? "AND NOT ISNULL(b.wait_room_id)" : "") . "
			ORDER BY a.building_code");
	}
	
	if (!$result || !($num_rows = $result->RecordCount())) { return false; }
		
	if ($num_rows > ADD_ROOM_LIST_LENGTH) { $num_rows = ADD_ROOM_LIST_LENGTH; }
	echo("<select name='" . $name . "' size='" . $num_rows . "'>\n");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["building_id"] . "'>" . $row["building_code"] . "</option>\n");
	}
	echo("</select>\n");
}





function display_room_list($name, $building_id, $interview_rooms=true, $wait_rooms=false)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	a.room_id, CONCAT(b.building_code, '&nbsp;', a.room_number) AS room_name
		FROM		room AS a, building AS b
		WHERE		a.building_id=b.building_id AND
					a.building_id='" . addslashes($building_id) . "'
		" . ((!$wait_rooms) ? "AND NOT ISNULL(a.wait_room_id)" : "") . "
		" . ((!$interview_rooms) ? "AND ISNULL(a.wait_room_id)" : "") . "
		ORDER BY	b.building_code, a.room_number");

	if (!$result || !($num_rows = $result->RecordCount())) { return false; }
		
	if ($num_rows > ADD_ROOM_LIST_LENGTH) { $num_rows = ADD_ROOM_LIST_LENGTH; }
	echo("<select name='" . $name . "' size='" . $num_rows . "'>\n");
	while ($row = $result->FetchRow())
	{
		echo("<option value='" . $row["room_id"] . "'>" . $row["room_name"] . "</option>\n");
	}
	echo("</select>\n");
}





function display_tr($name, $attribute, $third_column=false)
{
	echo("<tr><td>" . $name . "</td><td>" . $attribute . "</td>" . (($third_column) ? "<td></td>" : "") . "</tr>");
}

function display_building_room_phone($building_name, $room_number, $phone)
{
	display_tr("<b>Building Name:</b>", $building_name);
	display_tr("<b>Room Number:</b>", $room_number);
	display_tr("<b>Phone:</b>", (($phone) ? $phone : ""));
}

function display_building_room_phone_2($room_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT 	a.room_number, a.phone, b.building_name
		FROM		room AS a, building AS b
		WHERE		a.room_id='" . addslashes($room_id) . "' AND
					a.building_id=b.building_id");
	
	if (!$result || ($result->RecordCount() != 1) || !$row = $result->FetchRow())
	{
		return false;
	}
	
	display_building_room_phone($row["building_name"], $row["room_number"], $row["phone"]);
	return true;
}





// ex. $room = get_room($room_id);
function get_room($room_id, $recurse=true)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	*
		FROM	room
		WHERE	room_id='" . addslashes($room_id) . "'");
	
	if (!$result || ($result->RecordCount() != 1) || !($row = $result->FetchRow()))
	{
		return false;
	}
	
	if (!$building = get_building($row["building_id"])) { return false; }
	$wait_room = ($recurse) ? get_room($row["wait_room_id"], false) : false;
	return new room($room_id, $row["room_number"], $row["phone"], $building, $wait_room, $row["email"]);
}





function get_building($building_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	*
		FROM		building
		WHERE		building_id='" . addslashes($building_id) . "'");
		
	if (!$result || ($result->RecordCount() != 1) || !($row = $result->FetchRow()))
	{
		return false;
	}
	
	return new building($row["building_id"], $row["building_name"], $row["building_code"]);
}






/***********************
 ** CLASS DEFINITIONS **
 ***********************/





class building
{
	var $building_id;
	var $building_name;
	var $building_code;

	function building($building_id, $building_name, $building_code)
	{
		$this->building_id = $building_id;
		$this->building_name = $building_name;
		$this->building_code = $building_code;
	}

	function objToString()
	{
		$output = "Object building:(";
		$output .= "building_id: '" . $this->building_id . "'";
		$output .= ", building_name: '" . $this->building_name . "'";
		$output .= ", building_code: '" . $this->building_code . "'";
		$output .= ")";
		return $output;
	}
}





class room
{
	var $room_id;
	var $room_number;
	var $phone;
	
	var $building;
	var $wait_room;
	var $email;
	
	function room($room_id, $room_number, $phone, $building, $wait_room, $email)
	{
		$this->room_id = $room_id;
		$this->room_number = $room_number;
		$this->phone = $phone;
		$this->building = $building;
		$this->wait_room = $wait_room;
		$this->email = $email;
	}
	
	function objToString()
	{
		$output = "Object room:(";
		$output .= "room_id: '" . $this->room_id . "'";
		$output .= ", room_number: '" . $this->room_number . "'";
		$output .= ", phone: '" . $this->phone . "'";
		$output .= ", building: {" . (($this->building) ? $this->building->objToString() : "") . "}";
		$output .= ", wait_room: {" . (($this->wait_room) ? $this->wait_room->objToString() : "") . "}";
		$output .= ", e-mail: '" . $this->email . "'";
		$output .= ")";
		return $output;
	}
}




?>
