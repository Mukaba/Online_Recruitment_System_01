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
 | Filename: room_centric.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: room_centric.inc

// variables passed to script: $date, $room_id
// $date is guaranteed to be valid by room.inc
// the rest should be validated

// include functions
include("misc/room/hourminute_functions.inc");

$unbook = ($submit == "Unbook") ? true : false;

if (($unbook) && ($times_individual || $times_grouped) && ($time_id_string = get_edit_time_ids()))
{
	unbook($time_id_string, $date);
}

if (!$room = get_room($date, $room_id))
{
	echo ("Error!  Could not find room.");
}
else
{
	display($date, $room);
}

/*************************
 ** END OF PROGRAM FLOW **
 *************************/










/**************************
 ** FUNCTION DEFINITIONS **
 **************************/




function unbook($time_id_string, $date)
{
	$result = $GLOBALS['dbh']->Execute("
		UPDATE interview_time
		SET int_room_id=NULL
		WHERE	time_id IN (" . addslashes($time_id_string) . ") AND
				int_date='" . addslashes($date) . "' AND
				int_type=1");
}











/*****************************
 ** BEGIN DISPLAY FUNCTIONS **
 *****************************/

function display($date, $room)
{
	display_top($date, $room);
	display_form($date, $room);
	display_bottom($date, $room->building_id);
}





function display_top($date, $room)
{
	?>
	<table>
		<tr>
			<td>Date: </td><td><?php echo(formatLongDate($date)); ?></td>
		</tr>
		<tr>
			<td>Building: </td><td><?php echo($room->building_name); ?></td>
			<td><!-- spacer --></td>
			<td>Room: </td><td><?php echo($room->room_number); ?></td>
		</tr>
	</table>
	<?php
}





function display_form($date, $room)
{
	?>
	<form method="post">
		<input type="hidden" name="select" value="room_booking" />
		<input type="hidden" name="level1" value="room_centric" />
		<input type="hidden" name="date" value="<?php echo($date); ?>" />
		<input type="hidden" name="room_id" value="<?php echo($room->room_id); ?>" />
		
		<table>
			<tr>
				<td valign="top">
					<!-- check boxes, times, job codes, interviewers -->
<?php display_times($room); ?>				
				</td>
			
				<td valign="top">
					<!-- submit buttons -->
					<table>
						<tr>
							<td>
								<input type="submit" name="submit" value="Unbook" />
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





function display_times($room)
{
	?>
	<table>
		<tr>
			<td></td>
			<td align="center" colspan="3">&nbsp;Time&nbsp;</td>
			<td align="center">&nbsp;Job Code&nbsp;</td>
			<td align="center">&nbsp;Interviewer&nbsp;</td>
		</tr>
		
<?php
		$room_temp = $room;
		$room_temp->reduce_time_slots();

		display_time_slots($room_temp->time_slots, "grouped");
?>

		<tr><td colspan="5"><hr /></td></tr>
				
<?php
		display_time_slots($room->time_slots, "individual");
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
		
		echo("<td align='center'>" . $slots[$i]->job_code . "</td>");
		echo("<td align='center'>" . $slots[$i]->interviewer_name . "</td>");
		echo("</tr>\n");
	}
	return $num_slots;
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



function get_room($date, $room_id)
{
	$result = $GLOBALS['dbh']->Execute("
		SELECT	a.room_id, a.room_number, a.phone, a.wait_room_id, a.building_id,
			b.building_code, b.building_name
		FROM	room AS a, building AS b
		WHERE	a.room_id='" . addslashes($room_id) . "' AND a.building_id=b.building_id");

	if (!$result || ($result->RecordCount() != 1)) { return false; }
	if (!$row_room = $result->FetchRow()) { return false; }
		
	// get time_slots
	$sql = "
		SELECT a.time_id, a.int_time, a.end_time, a.interviewer_id, a.job_id,
			 j.job_code,
			 c.interviewer_name
		FROM interview_time AS a, job_info AS j, interview_interviewer AS c,
			interview AS b LEFT JOIN interview_place as p ON p.interview_place_id = b.interview_place_id LEFT JOIN
				interview_medium as m ON m.interview_medium_id = b.interview_medium_id
		WHERE a.int_date='" . addslashes($date) . "' AND a.int_type=1 AND 
			(p.description = '" . ONCAMPUS . "' OR m.description = '" . PHONE . "') AND
			a.int_room_id='" . addslashes($room_id) . "'
			AND j.job_id = a.job_id AND 
			a.job_id=b.job_id AND a.interviewer_id=c.interviewer_id
		ORDER BY a.int_time";
	$result = $GLOBALS['dbh']->Execute($sql);
/*
	// not good -- causes it to say error if the room has no booked times
	if (!$result || !$result->RecordCount()) { return false; }
	
	while($row = $result->FetchRow())
	{
		$time_slots[] = new interview_slot(h2m($row["int_time"]), h2m($row["end_time"]), $row["time_id"], $row["job_id"], $row["job_code"], $row["interviewer_id"], $row["interviewer_name"]);
	}
	if (count($time_slots) == 0) { return false; }
*/

	if ($result)
	{
		while($row = $result->FetchRow())
		{
			$time_slots[] = new interview_slot(h2m($row["int_time"]), h2m($row["end_time"]), $row["time_id"], $row["job_id"], $row["job_code"], $row["interviewer_id"], $row["interviewer_name"]);
		}
	}



	return new room($row_room["room_id"], $row_room["room_number"], $row_room["phone"], $row_room["wait_room_id"], $row_room["building_id"], $row_room["building_code"], $row_room["building_name"], $time_slots);
}






/******************************
 ** END GET INFO FUNCTIONS **
 ******************************/





/***********************
 ** CLASS DEFINITIONS **
 ***********************/


class room
{
	var $room_id;
	
	var $room_number;
	var $phone;
	var $wait_room_id;
	
	var $building_id;
	var $building_code;
	var $building_name;
	
	var $time_slots;
	
	function room($room_id, $room_number, $phone, $wait_room_id, $building_id, $building_code, $building_name, $time_slots)
	{
		$this->room_id = $room_id;
		$this->room_number = $room_number;
		$this->phone = $phone;
		$this->wait_room_id = $wait_room_id;
		$this->building_id = $building_id;
		$this->building_code = $building_code;
		$this->building_name = $building_name;
		$this->time_slots = $time_slots;
	}
	
	function objToString()
	{
		$output = "Object room:(";
		$output .= "room_id: '" . $this->room_id . "'";
		$output .= ", room_number: '" . $this->room_number . "'";
		$output .= ", phone: '" . $this->phone . "'";
		$output .= ", wait_room_id: '" . $this->wait_room_id . "'";

		$output .= ", building_id: '" . $this->building_id . "'";
		$output .= ", building_code: '" . $this->building_code . "'";
		$output .= ", building_name: '" . $this->building_name . "'";
				
		$output .= ", time_slots: {";
		$slots = count($this->time_slots);
		$slotslessone = $slots - 1;
		for ($i = 0; $i < $slots; $i++)
		{
			$output .= $this->time_slots[$i]->objToString();
			if ($i < $slotslessone) { $output .= ", "; }
		}
		$output .= "}";
		
		$output .= ")";
		return $output;
	}
	
	function reduce_time_slots()
	{
	   $i = 0;
	   while ($i < count($this->time_slots) - 1)
   	{
	      if ($this->time_slots[$i]->end == $this->time_slots[$i + 1]->start
      	    &&
   	       $this->time_slots[$i]->job_id == $this->time_slots[$i + 1]->job_id
      	    &&
   	       $this->time_slots[$i]->interviewer_id == $this->time_slots[$i + 1]->interviewer_id
    	       )
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
}










class interview_slot
{
	var $start;    // start time (in minutes from 00:00)
	var $end;      // end time (in minutes from 00:00)
	var $time_id;  // an array of 'time_id's covered by this time slot
	var $job_id;
	var $job_code;
	var $interviewer_id;
	var $interviewer_name;
	
	function interview_slot($start, $end, $time_id, $job_id, $job_code, $interviewer_id, $interviewer_name)
	{
		$this->start = $start;
		$this->end = $end;
		$this->time_id[] = $time_id;
		$this->job_id = $job_id;
		$this->job_code = $job_code;
		$this->interviewer_name = $interviewer_name;
		$this->interviewer_id = $interviewer_id;
	}
	
	function objToString()
	{
		$output = "Object interview_slot:(";
		$output .= "start: '" . m2h($this->start) . "'";
		$output .= ", end: '" . m2h($this->end) . "'";
		$output .= ", time_id: {";
		$times = count($this->time_id);
		$timeslessone = $times - 1;
		for ($i = 0; $i < $times; $i++)
		{
			$output .= $this->time_id[$i];
			if ($i < $timeslessone) { $output .= ", "; }
		}
		$output .= "}";
		$output .= ", job_id: '" . $this->job_id . "'";
		$output .= ", job_code: '" . $this->job_code . "'";
		$output .= ", interviewer_id: '" . $this->interviewer_id . "'";
		$output .= ", interviewer_name: '" . $this->interviewer_name . "'";
		$output .= ")";
		return $output;
	}
}

?>
