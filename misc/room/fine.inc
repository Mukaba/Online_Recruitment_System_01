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
 | Filename: fine.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: fine.inc

// variables passed to script: $date, $building_id, $job_id, $interviewer_id
// $date is guaranteed to be valid by room.inc
// the rest should be validated

// include functions
include("misc/room/hourminute_functions.inc");
$book = false;
$unbook = false;
switch ($submit)
{
	case "Book":
		$book = true;
		break;
		
	case "Unbook":
		$unbook = true;
		break;
}

if (($book || $unbook) && ($times_individual || $times_grouped) && ($time_id_string = get_edit_time_ids()))
{
	if ($unbook)
	{
		unbook($time_id_string, $date);
	}
	elseif ($book && $room)
	{
		if (isValidRoomID($room))
		{
			book($room, $time_id_string, $date);
		}
		else
		{
			$error_message = "Invalid room specified.";
			echo("Error:<br />" . $error_message);
		}
	}
	else
	{
		echo("A room must be specified!");
	}
}




$building = get_building($building_id, true);
$interviewer = get_interviewer($interviewer_id, $date);
$interview = get_interview($job_id);


if (!$building || !$interviewer || !$interview)
{
	echo ("Error!");
}
else
{
	display($date, $building, $interview, $interviewer);
	
/*
	// for debugging purposes only
	echo("<p>" . $building->objToString() . "</p>\n");
	echo("<p>" . $interviewer->objToString() . "</p>\n");
	echo("<p>" . $interview->objToString() . "</p>\n");
*/
}

/*************************
 ** END OF PROGRAM FLOW **
 *************************/










/**************************
 ** FUNCTION DEFINITIONS **
 **************************/




function isValidRoomID($room_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT room_id
		FROM room
		WHERE room_id='" . addslashes($room_id) . "'");
	if (!$result || !($result->RecordCount() == 1)) { return false; }
	if (!$row = $result->FetchRow()) { return false; }
	if ($row["room_id"] != $room_id) { return false; }
	return true;
}





function unbook($time_id_string, $date)
{

	$result = $GLOBALS['dbh']->Execute("
		UPDATE interview_time
		SET int_room_id=NULL
		WHERE	time_id IN (" . addslashes($time_id_string) . ") AND
				int_date='" . addslashes($date) . "' AND
				int_type=1");

}





function book($room_id, $time_id_string, $date)
{
	$room = get_room($room_id);
	
	$result_interviews = $GLOBALS['dbh']->Execute("
		SELECT	a.time_id, a.int_time, a.end_time
		FROM 	interview_time AS a, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN
			interview_medium as m ON m.interview_medium_id = b.interview_medium_id
		WHERE	a.time_id IN (" . addslashes($time_id_string) . ") AND
			a.int_date='" . addslashes($date) . "' AND
			a.job_id=b.job_id AND
			a.int_type=1 AND 
			(p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "')
		ORDER BY a.int_time");
	while ($row = $result_interviews->FetchRow())
	{
		$int_start = h2m($row["int_time"]);
		$int_end = h2m($row["end_time"]);
		$time_id = $row["time_id"];
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT int_time, end_time
			FROM interview_time AS a
			WHERE int_date='" . addslashes($date) . "' AND
					int_type=1 AND int_room_id='" . addslashes($room_id) . "'
			ORDER BY int_time");
		$conflict = false;
		while($row = $result->FetchRow())
		{
			$room_start = h2m($row["int_time"]);
			$room_end = h2m($row["end_time"]);
			
			if (($room_start >= $int_start && $room_start < $int_end)
				||
				($room_end <= $int_end && $room_end > $int_start)
				||
				($int_start >= $room_start && $int_end <= $room_end))
			{
				$conflict = true;
				break;
			}
		}
		
		if ($conflict)
		{
			// warning that times could not be moved
			echo ("<b>Conflict prevents moving interview times ");
			echo (formatTime(m2h($int_start), false) . " to " . formatTime(m2h($int_end), false) . " ");
			echo("to room " . $room->room_number . "<br />");
		}
		else
		{
			echo("<b>Booked interview times ");
			echo(formatTime(m2h($int_start), false) . " to " . formatTime(m2h($int_end), false) . " ");
			echo("to room " . $room->room_number . "<br />");
			
			$result = $GLOBALS['dbh']->Execute("
				UPDATE	interview_time
				SET		int_room_id='" . addslashes($room_id) . "'
				WHERE		time_id='" . addslashes($time_id) . "'");
		}
	}
}







/*****************************
 ** BEGIN DISPLAY FUNCTIONS **
 *****************************/

function display($date, $building, $interview, $interviewer)
{
	display_top($date, $building, $interview, $interviewer);
	display_form($date, $building, $interview, $interviewer);	
	display_bottom($date, $building->building_id);
}





function display_top($date, $building, $interview, $interviewer)
{
	?>
	<table>
		<tr>
			<td>Date: </td><td><?php echo(formatLongDate($date)); ?></td>
			<td><!-- spacer --></td>
			<td>Job Code: </td><td><?php echo($interview->job_code); ?></td>
		</tr>
		<tr>
			<td>Building: </td><td><?php echo($building->building_name); ?></td>
			<td><!-- spacer --></td>
			<td>Interviewer: </td><td><?php echo($interviewer->interviewer_name); ?></td>
		</tr>
	</table>
	<?php
}





function display_form($date, $building, $interview, $interviewer)
{
	?>
	<form method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="fine" />
		<input type="hidden" name="date" value="<?php echo($date); ?>" />
		<input type="hidden" name="building_id" value="<?php echo($building->building_id); ?>" />
		<input type="hidden" name="job_id" value="<?php echo($interview->job_id); ?>" />
		<input type="hidden" name="interviewer_id" value="<?php echo($interviewer->interviewer_id); ?>" />
		
		<table>
			<tr>
				<td valign="top">
					<!-- check boxes, times, interview rooms -->
<?php display_times($interviewer); ?>				
				</td>
			
				<td valign="top">
					<!-- rooms, submit buttons -->
					<table>
						<tr><td>Rooms:</td></tr>
						<tr>
							<td>
								<!-- rooms, select table -->
<?php display_room_select($building); ?>
							</td>
						</tr>
						<tr>
							<td>
								<input type="submit" name="submit" value="Book">&nbsp;<input type="submit" name="submit" value="Unbook" />
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
	</form>
	<?php
}





function display_bottom($date, $building_id)
{
	?>
	<table>
		<tr>
			<td>
<?php
	global $PHP_SELF;
	$link = $PHP_SELF . "&amp;select=room_booking&amp;level1=coarse&amp;date=" . urlencode($date) . "&amp;building_id=" . urlencode($building_id);
?>
				<a class="blue" href="<?php echo($link); ?>">Plotted Schedule</a>
			</td>
		</tr>
	</table>
	<?php
}





function display_times($interviewer)
{
	?>
	<table>
		<tr>
			<td></td>
			<td align="center" colspan="3">&nbsp;Time&nbsp;</td>
			<td align="center">&nbsp;Interview Room&nbsp;</td>
		</tr>
		
<?php
		$interviewer_temp = $interviewer;
		$interviewer_temp->reduce_time_slots();

		display_time_slots($interviewer_temp->time_slots, "grouped");
?>

		<tr><td colspan="5"><hr /></td></tr>
				
<?php
		display_time_slots($interviewer->time_slots, "individual");
?>
	</table>
	<?php
}





function display_time_slots($slots, $id)
{
	$num_slots = count($slots);
	
	for ($i = 0; $i < $num_slots; $i++)
	{
		echo("<tr>");
		echo("<td>");
		if ($i == 0) { echo("<input type='hidden' name='times_" . $id . "' value='" . $num_slots . "' />"); }
		echo("<input type='checkbox' name='times_" . $id . $i . "' value='" . packObject(($slots[$i]->time_id)) . "' /></td>");
		echo("<td align='right'>" . formatTime(m2h($slots[$i]->start)) . "</td>");
		echo("<td> - </td>");
		echo("<td align='right'>" . formatTime(m2h($slots[$i]->end)) .  "</td>");
		echo("<td align='center'>" . $slots[$i]->building_code . " " . $slots[$i]->room_number . "</td>");
		echo("</tr>\n");
	}
	return $num_slots;
}




function display_room_select($building)
{
	$rooms = $building->rooms;
	$num_rooms = count($rooms);
	
	echo("<select name='room' size='" . (($num_rooms > 10) ? "10" : $num_rooms) . "'>\n");
	for ($i = 0; $i < $num_rooms; $i++)
	{
		echo("<option value='" . $rooms[$i]->room_id . "'>" . $building->building_code . " " . $rooms[$i]->room_number . "</option>\n");
	}
	echo("</select>\n");
}

/***************************
 ** END DISPLAY FUNCTIONS **
 ***************************/





/**************************
 ** BEGIN EDIT FUNCTIONS **
 **************************/

function get_edit_time_ids()
{
	$time_id_string = get_time_ids("individual");
	if (!$time_id_string) { $time_id_string = get_time_ids("grouped"); }
	return $time_id_string;
}

function get_time_ids($base)
{
	$time_id_string = "";
	eval("global \$times_" . $base . ";");
	eval("\$times = \$times_" . $base . ";");
	
	for ($i = 0; $i < $times; $i++)
	{
		eval("global \$times_" . $base . $i . ";");
		eval("\$times_i = \$times_" . $base . $i . ";");
		
		if ($times_i)
		{
			$time_id_array = unpackObject(($times_i));
			for ($j = 0; $j < count($time_id_array); $j++)
			{
				$time_id_string .= $time_id_array[$j] . ",";
			}
		}
	}
	return substr($time_id_string, 0, -1);
}

/************************
 ** END EDIT FUNCTIONS **
 ************************/





/******************************
 ** BEGIN GET INFO FUNCTIONS **
 ******************************/

function get_building($building_id, $get_interview_rooms=false)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	*
		FROM		building
		WHERE		building_id='" . addslashes($building_id) . "'");
	if (!$result || ($result->RecordCount() != 1)) { return false; }

	if ($row = $result->FetchRow())
	{
		$building_name = $row["building_name"];
		$building_code = $row["building_code"];
	}
	else
	{
		return false;
	}
		
	if ($get_interview_rooms)
	{
		if (!$interview_rooms = get_interview_rooms($building_id)) { return false; }
		$building = new building($building_id, $building_code, $building_name, $interview_rooms);
	}
	else
	{
		$building = new building($building_id, $building_code, $building_name);
	}
	return $building;
}





function get_interview_rooms($building_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	*
		FROM		room
		WHERE		building_id='" . addslashes($building_id) . "' AND
					NOT ISNULL(wait_room_id)
		ORDER BY	room_number");
	if (!$result || !$result->RecordCount()) { return false; }
	
	$rooms = Array();
	while ($row = $result->FetchRow())
	{
		$rooms[] = new room($row["room_id"], $row["room_number"], $row["phone"], $row["wait_room_id"]);
	}
	
	if (count($rooms) == 0) { return false; }
	return $rooms;
}




function get_room($room_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT room_id, room_number, phone, wait_room_id
		FROM	room
		WHERE	room_id='" . addslashes($room_id) . "'");

	if (!$result || ($result->RecordCount() != 1)) { return false; }
	if (!$row = $result->FetchRow()) { return false; }
	
	return new room($room_id, $row["room_number"], $row["phone"], $row["wait_room_id"]);
}





function get_interviewer($interviewer_id, $date)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	*
		FROM		interview_interviewer
		WHERE		interviewer_id='" . addslashes($interviewer_id) . "'");
	if (!$result || ($result->RecordCount() != 1)) { return false; }

	if (!$row = $result->FetchRow()) { return false; }
	$interviewer_name = $row["interviewer_name"];
		
	// get time_slots
	$result = $GLOBALS['dbh']->Execute("
		SELECT a.time_id, a.int_time, a.end_time, a.int_room_id, b.room_number, c.building_code
		FROM interview_time AS a, interview AS d
		LEFT OUTER JOIN room AS b
		ON		a.int_room_id=b.room_id
		LEFT OUTER JOIN building AS c
		ON		b.building_id=c.building_id
		LEFT JOIN interview_medium AS m
		ON 		d.interview_medium_id = m.interview_medium_id
		LEFT JOIN interview_place as p
		ON		p.interview_place_id = d.interview_place_id
		WHERE a.int_date='" . addslashes($date) . "' AND a.int_type=1 AND a.job_id=d.job_id 
			AND (p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "') AND
			a.interviewer_id='" . addslashes($interviewer_id) . "'
		ORDER BY a.int_time");
	if (!$result || !$result->RecordCount()) { return false; }
	
	while($row = $result->FetchRow())
	{
//		$time_slots[] = new interview_slot($row["int_room_id"], h2m($row["int_time"]), h2m($row["end_time"]), $row["time_id"]);
$time_slots[] = new interview_slot($row["int_room_id"], h2m($row["int_time"]), h2m($row["end_time"]), $row["time_id"], $row["building_code"], $row["room_number"]);
	}

	if (count($time_slots) == 0) { return false; }
			
	return new interviewer($interviewer_id, $interviewer_name, $time_slots);
}





function get_interview($job_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	job_id, job_code
		FROM	job_info	
		WHERE	job_id='" . addslashes($job_id) . "'");

	if (!$result || ($result->RecordCount() != 1)) { return false; }
	
	if (!$row = $result->FetchRow()) { return false; }
	
	return new interview($job_id, $row["job_code"]);
}

/******************************
 ** END GET INFO FUNCTIONS **
 ******************************/





/***********************
 ** CLASS DEFINITIONS **
 ***********************/



class building
{
	var $building_id;
	var $building_code;
	var $building_name;
	var $rooms;
	
	// $rooms=false for pseudo overloading
	function building($building_id, $building_code, $building_name="", $rooms=false)
	{
		$this->building_id = $building_id;
		$this->building_name = $building_name;
		$this->building_code = $building_code;
		if ($rooms) { $this->rooms = $rooms; }
	}

	function objToString()
	{
		$output = "Object building:(";
		$output .= "building_id: '" . $this->building_id . "'";
		$output .= ", building_code: '" . $this->building_code . "'";
		$output .= ", building_name: '" . $this->building_name . "'";
		$output .= ", rooms: {";
		
		$rooms = count($this->rooms);
		$roomslessone = $rooms - 1;
		for ($i = 0; $i < $rooms; $i++)
		{
			$output .= "'";
			$output .= $this->rooms[$i]->objToString();
			$output .= "'";
			if ($i < $roomslessone) { $output .= ", "; }
		}
		
		$output .= "}";
		$output .= ")";
		return $output;
	}
}





class room
{
	var $room_id;
	var $room_number;
	var $phone;
	var $wait_room_id;

	function room($room_id, $room_number, $phone="", $wait_room_id="")
	{
		$this->room_id = $room_id;
		$this->room_number = $room_number;
		$this->phone = $phone;
		$this->wait_room_id = $wait_room_id;
	}
	
	function objToString()
	{
		$output = "Object room:(";
		$output .= "room_id: '" . $this->room_id . "'";
		$output .= ", room_number: '" . $this->room_number . "'";
		$output .= ", phone: '" . $this->phone . "'";
		$output .= ", wait_room_id: '" . $this->wait_room_id . "'";
		$output .= ")";
		return $output;
	}
}





class interviewer
{
	var $interviewer_id;
	var $interviewer_name;
	var $time_slots;
	
	function interviewer($interviewer_id, $interviewer_name, $time_slots)
	{
		$this->interviewer_id = $interviewer_id;
		$this->interviewer_name = $interviewer_name;
		$this->time_slots = $time_slots;
	}

	// NEW FUNCTION BELOW
	function reduce_time_slots()
	{
	   $i = 0;
	   while ($i < count($this->time_slots) - 1)
   	{
	      if ($this->time_slots[$i]->end == $this->time_slots[$i + 1]->start
      	    &&
   	       $this->time_slots[$i]->room_id == $this->time_slots[$i + 1]->room_id)
	      {
         	$this->time_slots[$i]->end = $this->time_slots[$i + 1]->end;
      	   $this->time_slots[$i]->time_id = array_merge((array) $this->time_slots[$i]->time_id, (array) $this->time_slots[$i + 1]->time_id);
   	      $dump = array_splice($this->time_slots, $i + 1, 1);
	      }
      	else
   	   {
	         $i++;
   	   }
	   }
	}
	// NEW FUNCTION ABOVE
	
	function objToString()
	{
		$output = "Object interviewer:(";
		$output .= "interviewer_id: '" . $this->interviewer_id . "'";
		$output .= ", interviewer_name: '" . $this->interviewer_name . "'";
		$output .= ", time_slots: {";
		$slots = count($this->time_slots);
		$slotslessone = $slots - 1;
		for ($i = 0; $i < $slots; $i++)
		{
			$output .= "'";
			$output .= $this->time_slots[$i]->objToString();
			$output .= "'";
			if ($i < $slotslessone) { $output .= ", "; }
		}
		$output .= "}";
		$output .= ")";
		return $output;
	}
}





class interview
{
	var $job_id;
	var $job_code;

	function interview($job_id, $job_code)
	{
		$this->job_id = $job_id;
		$this->job_code = $job_code;
	}
	
	function objToString()
	{
		$output = "Object interview:(";
		$output .= "job_id: '" . $this->job_id . "'";
		$output .= ", job_code: '" . $this->job_code . "'";
		$output .= ")";
		return $output;
	}
}





class interview_slot
{
	var $time_id;  // an array of 'time_id's covered by this time slot
	var $start;    // start time (in minutes from 00:00)
	var $end;      // end time (in minutes from 00:00)
	var $room_id;  // room_id (can be empty)
	var $building_code;
	var $room_number;
	
	function interview_slot($room_id, $start, $end, $time_id, $building_code, $room_number)
	{
		$this->room_id = $room_id;
		$this->start = $start;
		$this->end = $end;
		$this->time_id[] = $time_id;
		$this->building_code = $building_code;
		$this->room_number = $room_number;
	}
	
	function objToString()
	{
		$output = "Object interview_slot:(";
		$output .= "start: '" . m2h($this->start) . "'";
		$output .= ", end: '" . m2h($this->end) . "'";
		$output .= ", room_id: '" . $this->room_id . "'";
		$output .= ", time_id: {";
		$times = count($this->time_id);
		$timeslessone = $times - 1;
		for ($i = 0; $i < $times; $i++)
		{
			$output .= $this->time_id[$i];
			if ($i < $timeslessone) { $output .= ", "; }
		}
		$output .= "}";
		$output .= ", building_code: '" . $this->building_code . "'";
		$output .= ", room_number: '" . $this->room_number . "'";
		$output .= ")";
		return $output;
	}
}

?>
