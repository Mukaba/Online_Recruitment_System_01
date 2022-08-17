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
 | Filename: update_times_sequential.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: Update the interview time slots if the interview is             |
 | sequential                                                                   |
 +------------------------------------------------------------------------------+

*/

foreach( $interview->schedules[$j] as $s )
{
	
	$start = $s['START'];
	$end = $s['END'];
	$type = $s['TYPE'];
	
	if( $interview->no_time && $type == INTERVIEW )
		$type = NO_TIME;
	
	for($x = 0; $x < count( $interviewers ); $x++ )
  	{  		

		/*
		 Find the times we should be adding, by adding the current stagger amount to the start and end times.
		 If this is our first time through, then do not add a stagger amount.
		*/
	
		if ($x)
		{
			/*
			 The add_times function takes a number of minutes and adds them to the specified time.  If
			 using cal_length before hand to determine how many minutes to add, we could run into problems,
			 as cal_length returns the length in TIME format, which can int_val to weird things.  If
			 having problems here, it might be due to the cal_length.
			*/

			$start = add_times($start, $stagger_amount);
			$end = add_times($end, $stagger_amount);
		}
 
		$location = $locations[$x];
		$wait_location = $wait_locations[$x];	 	
	 	
		if( $type == INTERVIEW )
		{
			
			if( count( $time_ids ) > 0 )	 	
			{
				$time_id = array_pop($time_ids);
				
				$GLOBALS['dbh']->Execute("	
					INSERT INTO interview_time(
						time_id, 
						job_id, 
						int_time, 
						end_time, 
						int_type, 
						int_date, 
						interviewer_id, 
						int_room_id, 
						int_wait_room_id, 
						sequential)
					VALUES (
						'$time_id', 
						'$job_id', 
						'$start',
						'$end',
						'$type', 
						'$int_dates[$j]', 
						'$interviewer_ids[$x]', 
						'$location', 
						'$wait_location', 
						'$x')
				");
			}
			else
			{
				$GLOBALS['dbh']->Execute("	
					INSERT INTO interview_time(
						job_id, 
						int_time, 
						end_time, 
						int_type, 
						int_date, 
						interviewer_id, 
						int_room_id, 
						int_wait_room_id, 
						sequential)
					VALUES (
						'$job_id', 
						'$start',
						'$end',
						'$type', 
						'$int_dates[$j]', 
						'$interviewer_ids[$x]', 
						'$location', 
						'$wait_location', 
						'$x')
					");
			}
		}
	 	else
		{
			if( count( $time_ids ) > 0 )	 	
			{
				$time_id = array_pop($time_ids);
		  		$GLOBALS['dbh']->Execute("	
					INSERT INTO interview_time(
						time_id,
						job_id,
						int_time,
						end_time,
						int_type,
						int_date,
						interviewer_id,
						int_room_id,
						int_wait_room_id,
						sequential)
  					VALUES ('$time_id',
						'$job_id',
						'$start',
						'$end',
						'$type',
						'$int_dates[$j]',
						'$interviewer_ids[$x]',
						'$location',
						'$wait_location',
						'$x')
					");
			}
			else
			{
		  		$GLOBALS['dbh']->Execute("
					INSERT INTO interview_time(
						job_id,
						int_time,
						end_time,
						int_type,
						int_date,
						interviewer_id,
						int_room_id,
						int_wait_room_id,
						sequential)
  					VALUES (
						'$job_id',
						'$start',
						'$end',
						'$type',
						'$int_dates[$j]', 
						'$interviewer_ids[$x]', 
						'$location',
						'$wait_location',
						'$x')
					");
			}		
		}
	}
	unset($cur_stagger);
}

?>
