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
 | Filename: coarse.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: coarse.inc
// This is the coarse room scheduling portion.  It allows the user to schedule
// 	all the interview times for a given day and interview to a room.

define(RESOLUTION, 15); // some number that divides evenly into 60 (ex. 5, 10, 12, 15, 20, 30)

// include classes
include ("misc/room/room.class.inc");
include ("misc/room/room_slot.class.inc");
include ("misc/room/interviewer.class.inc");
include ("misc/room/interview_slot.class.inc");
// include functions
include("misc/room/hourminute_functions.inc");
/***************************
 ** START OF PROGRAM FLOW **
 ***************************/
if ($num_room_requests)
{
	for ($i = 0; $i < $num_room_requests; $i++)
	{
		eval("\$room_request = \$room_requests_" . $i . ";");
		$room_request = urldecode($room_request);
		if ($room_request != "-")
		{
			$room_request = unpackObject(($room_request));
			book_soft($room_request->date, $room_request->job_id, $room_request->interviewer_id, $room_request->room_id);
		}
	}
}

if (!$result_building = query_buildings($building_id))
{
	// Error: No building_id matched the building_id passed to the script (or multiple ones did).
	$error_message = "Invalid building specified.";
	echo("Error:<br />" . $error_message);
}
else
{
	list (, $building_code, $building_name) = $result_building->FetchRow();
	
	display_date_building($date, $building_name);

	if (!$result_interviewers = query_interviewers($date))
	{
		// Error: No local interviews on given day
		$error_message = "Date was invalid or no interviews were found on the date.";
		echo("Error:<br />" . $error_message);
	}
	else
	{
		list ($interviewers, $begin_at, $end_at) = make_interviewers_array($result_interviewers, $date);
		
		// *** build room time info ***
		if (!$result_rooms = query_rooms($building_id))
		{
			// Error: No rooms in building
			$error_message = "No rooms were found in the building.";
			echo("Error:<br />" . $error_message);
		}
		else
		{
			$rooms = make_rooms_array($result_rooms, $date);

//	variables available:
//	$date, $building_id, $interviewers, $rooms
//	$earliest - the start of the earliest local interview on date
//	$latest - the end of the last local interview on date			
//	$begin_at - the time the chart should begin at (in minutes from 00:00)
//	$end_at - the time the chart should end at (in minutes from 00:00)
			
			display_form($interviewers, $rooms, $begin_at, $end_at, $building_code, $building_id, $date);
		
//			debug_coarse_output();

		
		} // end of buildings found
	} // end of interviewers found
} // end of building found





/*************************
 ** END OF PROGRAM FLOW **
 **                     **
 **   FUNCTIONS BELOW   **
 *************************/

class form_info
{
	var $date;
	var $job_id;
	var $interviewer_id;
	var $room_id;

	function form_info($date, $job_id, $interviewer_id, $room_id)
	{
		$this->date = $date;
		$this->job_id = $job_id;
		$this->interviewer_id = $interviewer_id;
		$this->room_id = $room_id;
	}
	
	function objToString()
	{
		$output = "Object form_info:(";
		$output .= "date: '" . $this->date . "'";
		$output .= ", job_id: '" . $this->job_id . "'";
		$output .= ", interviewer_id: '" . $this->interviewer_id . "'";
		$output .= ", room_id: '" . $this->room_id . "'";
		$output .= ")";
		return $output;
	}
}





function book_soft($date, $job_id, $interviewer_id, $room_id)
{
	book($date, $job_id, $interviewer_id, $room_id, false);
}

function book_hard($date, $job_id, $interviewer_id, $room_id)
{
	book($date, $job_id, $interviewer_id, $room_id, true);
}

function book($date, $job_id, $interviewer_id, $room_id, $force)
{
	// INTERVIEW TIMES
	$job_code = ""; // who cares?
	$room_name = get_room_name($room_id);
	// locale needs some work (ie. (oncampus OR (phone)))
	$sql = "SELECT	a.time_id, a.int_time, a.end_time, a.int_room_id
		FROM interview_time AS a, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN interview_medium as m ON m.interview_medium_id = b.interview_medium_id
		WHERE	a.int_date='" . addslashes($date) . "' AND
			a.job_id=b.job_id AND
			a.interviewer_id='" . addslashes($interviewer_id) . "' AND
			a.job_id='" . addslashes($job_id) . "' 
			AND a.int_type=1 AND (p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "')
			" . (($force) ? "" : "AND (ISNULL(a.int_room_id) OR a.int_room_id = '0')") . "
		ORDER BY a.int_time";
	$result_interviews = $GLOBALS['dbh']->Execute($sql);
	while($row = $result_interviews->FetchRow())
	{
		$time_slots[] = new interview_slot($row["int_room_id"], h2m($row["int_time"]), h2m($row["end_time"]), $row["time_id"]);
	}

	$interview = new interviewer($job_id, $job_code, $interviewer_id, $time_slots);
	unset($time_slots); // get rid of time_slots array
	$interview->reduce_time_slots();
	
		
	// ROOM -- since room bookings might change as we go through
	//         this loop, reload the room info each time
	$room_number = ""; // who cares?
	$result = $GLOBALS['dbh']->Execute("
		SELECT a.int_time, a.end_time, a.job_id, a.interviewer_id
		FROM interview_time AS a, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN interview_medium as m ON m.interview_medium_id = b.interview_medium_id
		WHERE a.int_date='" . addslashes($date) . "' AND
			a.job_id=b.job_id AND
			a.int_type=1 AND (p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "')
			AND a.int_room_id='" . addslashes($room_id) . "'
		ORDER BY a.int_time");
	
	$time_slots = Array();
	while($result && ($row = $result->FetchRow()))
	{
		$time_slots[] = new room_slot($row["job_id"], $row["interviewer_id"], h2m($row["int_time"]), h2m($row["end_time"]));
	}
	
	$room = new room($room_id, $room_number, $time_slots);
	unset($time_slots);

	
	$interview_slot_count = count($interview->time_slots);
	
	for ($i = 0; $i < $interview_slot_count; $i++)
	{
		$int_start = $interview->time_slots[$i]->start;
		$int_end = $interview->time_slots[$i]->end;
		
		$room_slot_count = count($room->time_slots);
		for ($j = 0, $conflict = false; $j < $room_slot_count; $j++)
		{
			$room_start = $room->time_slots[$j]->start;
			$room_end = $room->time_slots[$j]->end;
			
			if (
				($room_start >= $int_start && $room_start < $int_end)
				||
				($room_end <= $int_end && $room_end > $int_start)
				||
				($int_start >= $room_start && $int_end <= $room_end)
				)
			{
				$conflict = true;
				break;
			}
		}
		if ($conflict)
		{
			// warning that times could not be moved
			echo ("<b>Conflict prevents moving interview times ");
			echo (formatTime(m2h($interview->time_slots[$i]->start), false) . " to " . formatTime(m2h($interview->time_slots[$i]->end), false) . " ");
			echo("to room, " . $room_name . "<br />");
		}
		else
		{
			echo ("<b>Booked interview times ");
			echo (formatTime(m2h($interview->time_slots[$i]->start), false) . " to " . formatTime(m2h($interview->time_slots[$i]->end), false) . " ");
			echo("to room, " . $room_name . "<br />");
			
			for ($k = 0; $k < count($interview->time_slots[$i]->time_id); $k++)
			{
				$result = $GLOBALS['dbh']->Execute("
					UPDATE	interview_time
					SET	int_room_id='" . addslashes($room_id) . "'
					WHERE	time_id='" . addslashes($interview->time_slots[$i]->time_id[$k]) . "'");
				
			}
		}
	}
}





function query_buildings($building_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT building_id, building_code, building_name
		FROM building
		WHERE building_id='" . $building_id . "'");
	
	return ($result && ($result->RecordCount() == 1)) ? $result : false;	
}





function query_interviewers($date)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT a.job_id, a.interviewer_id, j.job_code
		FROM interview_time AS a, job_info as j, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN interview_medium as m ON m.interview_medium_id = b.interview_medium_id
		WHERE a.int_date='" . addslashes($date) . "' AND j.job_id = b.job_id AND a.int_type=1 
			AND (p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "') AND a.job_id=b.job_id
		GROUP BY a.job_id, a.interviewer_id
		ORDER BY j.job_code");
	
	return ($result && $result->RecordCount()) ? $result : false;
}





function query_rooms($building_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT a.room_id, a.room_number
		FROM room AS a, building AS b
		WHERE a.building_id=b.building_id AND
				b.building_id='" . addslashes($building_id) . "' AND
				NOT ISNULL(a.wait_room_id)
				
		ORDER BY a.room_number");
	
	return ($result && $result->RecordCount()) ? $result : false;
}





function display_form($interviewers, $rooms, $begin_at, $end_at, $building_code, $building_id, $date)
{
	?>
		<form method="post">
			<input type="hidden" name="select" value="room_booking" />
			<input type="hidden" name="level1" value="coarse" />
			<input type="hidden" name="building_id" value="<?php echo($building_id); ?>" />
			<input type="hidden" name="date" value="<?php echo($date); ?>" />
	<?php
			
	display_plot($interviewers, $rooms, $begin_at, $end_at, $building_code, $date, $building_id);

	?>
			<div align="center"><input type="submit" value="Book" /></div>
		</form>
	<?php
}





function make_interviewers_array($result_interviewers, $date)
{
	$earliest = 1440;
	$latest = 0;

	while ($row = $result_interviewers->FetchRow())
	{
		$job_id = $row["job_id"];
		$job_code = $row["job_code"];
		$interviewer_id = $row["interviewer_id"];
			
		$result_interviews = $GLOBALS['dbh']->Execute("
			SELECT a.time_id, a.int_time, a.end_time, a.int_room_id
			FROM interview_time AS a, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN interview_medium as m ON m.interview_medium_id = b.interview_medium_id
			WHERE a.int_date='" . addslashes($date) . "' AND a.int_type=1 
				AND (p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "') AND 
				a.job_id=b.job_id AND
				a.interviewer_id='" . addslashes($interviewer_id) . "'
			ORDER BY a.int_time");
		
		while($row = $result_interviews->FetchRow())
		{
			if (($start = h2m($row["int_time"])) < $earliest) { $earliest = $start; }
			if (($end = h2m($row["end_time"])) > $latest) { $latest = $end; }
			$time_slots[] = new interview_slot($row["int_room_id"], $start, $end, $row["time_id"]);
		}

		$interviewers[] = new interviewer($job_id, $job_code, $interviewer_id, $time_slots);
		$interviewers[count($interviewers) - 1]->reduce_time_slots();
		unset($time_slots);
	}
	
	$begin_at = floor($earliest / RESOLUTION) * RESOLUTION;
	$end_at = ceil($latest / RESOLUTION) * RESOLUTION;

	return Array($interviewers, $begin_at, $end_at);
}





function make_rooms_array($result_rooms, $date)
{
	while($row = $result_rooms->FetchRow())
	{
		$room_id = $row["room_id"];
		$room_number = $row["room_number"];
	
		
		$result = $GLOBALS['dbh']->Execute("
			SELECT a.int_time, a.end_time, a.job_id, a.interviewer_id
			FROM interview_time AS a, interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN interview_medium as m ON m.interview_medium_id = b.interview_medium_id
			WHERE a.int_date='" . addslashes($date) . "' AND
					a.job_id=b.job_id AND
					(p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "')
					AND a.int_type=1 AND a.int_room_id='" . addslashes($room_id) . "'
			ORDER BY a.int_time");
	

		$time_slots = Array();
		while($result && ($row = $result->FetchRow()))
		{
			$time_slots[] = new room_slot($row["job_id"], $row["interviewer_id"], h2m($row["int_time"]), h2m($row["end_time"]));
		}
		
		$rooms[] = new room($room_id, $room_number, $time_slots);
		unset($time_slots);
	}
	return $rooms;
}





function display_date_building($date, $building_name)
{
	?>
	<table cellspacing="0" cellpadding="0">
		<tr><td>Date: </td><td><?php echo(formatLongDate($date)); ?></td></tr>
		<tr><td>Building: </td><td><?php echo($building_name); ?></td></tr>
	</table>
	<?php
}





function display_plot($interviewers, $rooms, $begin_at, $end_at, $building_code, $date, $building_id)
{
	global $client;
	
	if ($client->browser == NETSCAPE)
	{
		echo("<table cellspacing='0' cellpadding='0'>\n");
	}
	else
	{
		echo("<table cellspacing='1' cellpadding='0'>\n");
	}

	display_times_plot($begin_at, $end_at, $building_code);
	display_interviews_plot($interviewers, $begin_at, $end_at, $rooms, $date, $building_id);
	display_rooms_plot($rooms, $begin_at, $end_at, $building_code, $date);

	echo("</table>\n");
}





function display_times_plot($begin_at, $end_at, $building_code)
{
	echo("<tr>\n");
	echo("<td><span style='font-size: 9pt;'>Job Code</span></td><td>&nbsp;</td>\n");
	for ($j = $begin_at; $j < $end_at; $j += RESOLUTION)
	{
		echo("<td align='left' width='10'>");
		$hour = $j / 60;
		if ($hour == floor($hour)) { echo("<img src='misc/images/h" . $hour . ".gif' alt='' />"); }
		echo("</td>");
	}
	echo("<td></td><td align='center'>" . $building_code . "</td>\n");
	echo("</tr>\n");
}





function display_interviews_plot($interviewers, $begin_at, $end_at, $rooms, $date, $building_id)
{
	$interviewers_count = count($interviewers);
	for ($i = 0; $i < $interviewers_count; $i++)
	{
		echo("<tr>\n");
		global $PHP_SELF;
		$link = $PHP_SELF . "&amp;select=room_booking&amp;level1=fine&amp;date=" . urlencode($date) . "&amp;building_id=" . urlencode($building_id) . "&amp;job_id=" . urlencode($interviewers[$i]->job_id) . "&amp;interviewer_id=" . urlencode($interviewers[$i]->interviewer_id);
		echo("<td><a class='blue' href='$link'>" . $interviewers[$i]->job_code . "</a></td>\n");
		echo("<td>&nbsp;</td>\n");
		for ($j = $begin_at; $j < $end_at; $j += RESOLUTION)
		{
			$first = $j;
			$last = $j + RESOLUTION;
			
			$time_slots_count = count($interviewers[$i]->time_slots);
		
			$booked_time = $not_booked_time = false;
		
			for ($k = 0; $k < $time_slots_count; $k++)
			{
				$start = $interviewers[$i]->time_slots[$k]->start;
				$end = $interviewers[$i]->time_slots[$k]->end;
				if (($first >= $start && $first < $end) || ($last > $start && $last <= $end))
				{
					if ($interviewers[$i]->time_slots[$k]->room_id != "") { $booked_time = true; }
					else { $not_booked_time = true; }
				}
			}
		
			if ($booked_time && $not_booked_time) { $color = "#660066";	}
			elseif ($booked_time) { $color = "#0000FF"; }
			elseif ($not_booked_time) { $color = "#FF0000"; }
			else
			{
				$color = (($j / 60) == floor($j / 60)) ? "#333333" : "#000000";
			}
				
			echo("<td bgcolor='" . $color . "'>&nbsp;</td>");
		}
		echo("<td>&nbsp;</td>\n");

		echo("<td>\n<select name='room_requests_" . $i . "'>\n");
		echo("<option selected='selected'>-</option>\n");
		$rooms_count = count($rooms);
		for ($m = 0; $m < $rooms_count; $m++)
		{
			$serialized = packObject((new form_info($date, $interviewers[$i]->job_id, $interviewers[$i]->interviewer_id, $rooms[$m]->room_id)));
			echo("<option value='" . $serialized . "'>" . " " . $rooms[$m]->room_number . "</option>\n");
		}
		echo("</select>\n</td>\n");
		echo("</tr>\n");
	}
	echo("<input type='hidden' name='num_room_requests' value='" . $interviewers_count . "' />\n");
}





function display_rooms_plot($rooms, $begin_at, $end_at, $building_code, $date)
{
	echo("<tr><td><hr /></td></tr>");
	$rooms_count = count($rooms);
	for ($i = 0; $i < $rooms_count; $i++)
	{
		echo("<tr>\n");
		global $PHP_SELF;
		$link = $PHP_SELF . "&amp;select=room_booking&amp;level1=room_centric&amp;date=" . urlencode($date) . "&amp;room_id=" . urlencode($rooms[$i]->room_id);
		echo("<td><a class='blue' href='$link'>" . $building_code . " " . $rooms[$i]->room_number . "</a></td>\n");
		echo("<td>&nbsp;</td>\n");
	
		for ($j = $begin_at; $j < $end_at; $j += RESOLUTION)
		{
			$first = $j;
			$last = $j + RESOLUTION;
			$time_slots_count = count($rooms[$i]->time_slots);
		
			$booked = false;
			for ($k = 0; $k < $time_slots_count; $k++)
			{
				$start = $rooms[$i]->time_slots[$k]->start;
				$end = $rooms[$i]->time_slots[$k]->end;
				if (($first >= $start && $first < $end) || ($last > $start && $last <= $end))
				{
					$booked = true;
					break;
				}
			}
   	
			if ($booked) { $color = "#FF0000"; }
			else { $color = (($j / 60) == floor($j / 60)) ? "#333333" : "#000000"; }
			echo("<td bgcolor='" . $color . "'>&nbsp;</td>");
		}
		echo("</tr>\n");
	}
}





function get_room_name($room_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT CONCAT(building_code, '&nbsp;', room_number) AS room_name
		FROM	room, building
		WHERE	room_id='" . addslashes($room_id) . "' AND room.building_id=building.building_id");
	
	if (!$result || ($result->RecordCount() != 1)) { return false; }
	if (!$row = $result->FetchRow()) { return false; }
	
	return $row["room_name"];
}





function debug_coarse_output()
{
	echo("<br /><br /><hr />");
	echo("<h1>Debugging</h1>");

	/*
	echo ("<p>The first interview starts at " . m2h($earliest) . ".<br />");
	echo ("The last interview ends at " . m2h($latest) . "</p>");
	echo ("<p>The time chart should begin at " . m2h($begin_at) . ".<br />");
	echo ("The time chart should end at " . m2h($end_at) . "</p>");
	*/

	global $interviewers;
	echo("<h3>Interviewers</h3>");
	for ($i = 0; $i < count($interviewers); $i++)
	{
		echo ($interviewers[$i]->objToString() . "<br /><br />");
	}

	global $rooms;
	echo("<h3>Rooms</h3>");
	for ($i = 0; $i < count($rooms); $i++)
	{
		echo ($rooms[$i]->objToString() . "<br /><br />");
	}
}

?>
