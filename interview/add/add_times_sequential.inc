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
 | Filename: add_times_sequential.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: add interview times and breaks to the database when the         |
 | interviewers are sequential.                                                 |
 +------------------------------------------------------------------------------+

*/

assert( count($interview->schedules) == $interview->intdays );

//for each day
for($d = 0; $d < $interview->intdays; $d++)
{
   	$schedule = $interview->schedules[$d];
	$stagger_amount = cal_length($schedule[0]["START"], $schedule[0]["END"]);

	foreach( $schedule as $s )
	{
		assert( isset($s) );
		
		/*
		 We need to set a stagger length here so that we know how much extra time each other interviewers 
		 schedule is offset by.  
		*/
		$start_time = $s["START"];
		$end_time = $s["END"];
		$type = $s["TYPE"];
		
		assert( $type == INTERVIEW || $type == INT_BREAK || $type == LUNCH );
		
		if( $interview->no_time && $type == INTERVIEW )
		{
			$type = NO_TIME;
		}
		
		/*
		 Set up the sequential schedules for the other employers here.
		*/

		for ($j = 0; $j < $interview->num_interviewers; $j++)
		{
			if (!$j) //If this is the first interviewers schedule, set it accordingly.
			{
				$sequential[] = array(
					"job_id" => $job_id,
					"start_time" => $start_time,
					"end_time" => $end_time,
					"int_date" => $int_date[$d],
					"interviewer_id" => $interviewer_id[$j],
					"locations" => $locations[$j],
					"wait_locations" => $wait_locations[$j],
					"type" => $type,
					"sequential" => $j
					);
			} else //The start and end times depend on this interviewers previous schedule.
			{
				$sequential[] = array(
					"job_id" => $job_id,
					"start_time" => add_times($sequential[$j - 1]["start_time"], $stagger_amount),
					"end_time" => add_times($sequential[$j - 1]["end_time"], $stagger_amount), 
					"int_date" => $int_date[$d],
					"interviewer_id" => $interviewer_id[$j],
					"locations" => $locations[$j],
					"wait_locations" => $wait_locations[$j],
					"type" => $type,
					"sequential" => $j
					);
				
			}
		}

		/*
		 We now have an array called $sequential that holds the schedule for this day, for EACH interviewer.
		 We now need to add this schedule into our database.
		*/


		for ($j = 0; $j < sizeof($sequential); $j++)
		{
			$sql = ("
				INSERT INTO interview_time(
					job_id, int_time, end_time, int_date, interviewer_id, int_room_id, 
					int_wait_room_id, int_type, sequential)
				VALUES (
					'" . $sequential[$j]["job_id"] . "',
					'" . $sequential[$j]["start_time"] . "',
					'" . $sequential[$j]["end_time"] . "',
					'" . $sequential[$j]["int_date"] . "',
					'" . $sequential[$j]["interviewer_id"] . "',
					'" . $sequential[$j]["locations"] . "',
					'" . $sequential[$j]["wait_locations"] . "',
					'" . $sequential[$j]["type"] . "',
					'" . $sequential[$j]["sequential"] . "'
					)
				");
			
			$GLOBALS['dbh']->Execute($sql);
		}
		unset($sequential);
        }
}   
            

?>
