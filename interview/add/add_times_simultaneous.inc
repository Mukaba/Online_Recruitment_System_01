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
 | Filename: add_times_simultaneous.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: Add interview times and breaks to the database when the         |
 | multiple interviewers are interviewing simultaneously.                       |
 +------------------------------------------------------------------------------+

*/

assert( count($interview->schedules) == $interview->intdays );

//for each day
for($d = 0; $d < $interview->intdays; $d++)
{
   	$schedule = $interview->schedules[$d];

	foreach( $schedule as $s )
	{
		assert( isset($s) );
		
		$start_time = $s["START"];
		$end_time = $s["END"];
		$type = $s["TYPE"];
		
		assert( $type == INTERVIEW || $type == INT_BREAK || $type == LUNCH );
		
		if( $interview->no_time && $type == INTERVIEW )
			$type = NO_TIME;
		
		for($j = 0; $j < $interview->num_interviewers; $j++)
		{
        		$GLOBALS['dbh']->Execute("
				INSERT INTO interview_time
				(job_id, int_time, end_time, int_date, interviewer_id, int_room_id, int_wait_room_id, int_type)
    		    VALUES (
				'$job_id', 
				'$start_time', 
				'$end_time', 
				'$int_date[$d]', 
				'$interviewer_id[$j]', 
				'$locations[$j]', 
				'$wait_locations[$j]', 
				'$type')
				");
       		}
	}
}

?>



