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
 | Filename: timetest.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

/*** 	state variable 'timetest' indicating Calculator is using 	***/


$timetest = 1;

include('interview/add/dayplan_input.inc');
include('interview/add/dayplan.inc');

assert( isset($interview) );

$URL = "$PHP_SELF&amp;select=add&amp;timetest=1";
$control = 	"
		<br />
		<table border='0' cellpadding='4' cellspacing='0' width='100%' class='row1'>
		<tr>"
		. (( $day == $intdays || $intdays == 1)
		?
		"<td align='center' width='50%'>
		 <input type='submit' name='time_mode' value='Finish' />
		 </td>
		 <td align='center' width='50%'>
		 <input type='submit' name='time_mode' value='Finish &amp; Post' />
		 </td>"
		:
		"<td align='center' width='100%'>
		 <input type='submit' name='time_mode' value='Continue' />
		 </td>"
		 )
		 . "
		 </tr></table>";
if( is_string($interview) ) {  $interview = unpackObject($interview); }

if( isset($schedule) && is_string($schedule) )  $schedule = unpackObject(( $schedule ) );

/*** 	process submitted request  	***/

switch($time_mode)
{
	/*** 	Manually Input Mode		***/
   	
   	case "Manual mode":
		display($URL, $intdays, $day, $int_date, $schedule, 1, $control);
		break;
	
	/*** 	Return to calculator mode	***/
			
	case "Close manual mode":
		display($URL, $intdays, $day, $int_date, $schedule, 0, $control);
		break;
	
	/*** 	sub-procedure in 'Manual Mode', update schedule		***/
		
	case "Update and Save changes":

		update_manual_schedule();
		//write to interview object
		$interview->int_date[$day-1] = $int_date;
		$interview->schedules[$day-1] = $schedule;
		if ($day  < $interview->intdays)
		{
    	   		$day++;
			$default_int_date = add_dates($int_date, 1);

    		   	$num_students = (float)count( explode(chr(10), $interview->students ) );
       		    	if( isSwitching($interview->interview_type_id)  || isSimultaneous($interview->interview_type_id)){ $num_students = ceil( $num_students / 2 ); }
    	  		$default_int_num = ceil( $num_students/$interview->intdays );
			
			unset( $start_time );
	   		for( $i = 0; $i < count($schedule); $i++ )
   			{
   				if( $schedule[$i]["TYPE"] == INTERVIEW )
   				{
					$start_time = $schedule[$i]["START"];
   					$length = cal_length($schedule[$i]["START"], $schedule[$i]["END"] );
					break;
   				}
   			}
		      	getDayPlan( $URL, $interview->intdays, $day, $default_int_date, $default_int_num, $length );
		}
		else
    		{	
			display($URL, $intdays, $day, $int_date, $schedule, 1, $control);	
		}
		break;	
	
	/* 	
	 Pass daily schedule to 'interview' obj, go to next day if there is more left.
	 Otherwise, call 'add_job.inc' to store all user input into database.
	*/	
	
	case "Finish & Post":
		/* set the flag "post" to post the job after saving it to database */
		$post = true;
   	
	case "Continue":
   	case "Finish":
		//write to interview object
		$interview->int_date[$day-1] = $int_date;
		$interview->schedules[$day-1] = $schedule;
		if ($day  < $interview->intdays)
		{
			$day++;
			$default_int_date = add_dates($int_date, 1);

    	   		$num_students = (float)count( explode(chr(10), $interview->students ) );
           	
			if( (isSwitching($interview->interview_type)) || (isSimultaneous($interview->interview_type)))
			{
				$num_students = ceil( $num_students / 2 ); 
			}
    	  		
			$default_int_num = ceil( $num_students/$interview->intdays );
			
			unset( $start_time );
	   		for( $i = 0; $i < count($schedule); $i++ )
   			{
   				if( $schedule[$i]["TYPE"] == INTERVIEW )
   				{
					$start_time = $schedule[$i]["START"];
   					$length = cal_length($schedule[$i]["START"], $schedule[$i]["END"] );
					break;
   				}
   			}
		      	getDayPlan( $URL, $interview->intdays, $day, $default_int_date, $default_int_num, $length );
		}
		else
    		{	
			unset($timetest);
	    	     	include('interview/add/add_job.inc');
	    	}
    	break;

 	 	
   case "dayplan_input_complete":

		$error = 0;
			
	/*
	 If we've been given the new schedule after a Reset Times command,
	 and the user has decided to restore breaks, we store them in a 
	 temporary variable.
	*/

	/*
	 The two time variables below are used to test that the date and time are later than
	 the current date and time.
	*/
	
	$current_time = date("H:i:s");
	$next_time = (validTime($start_time));
		
    	if ( check_date($int_date) == 0) 
	{ 
		$error = 1;  
		$error_message = "You must enter a valid date, in the format YYYY-MM-DD, and no earlier than todays date.<br /><br />"; 
	}
    	elseif( $int_date < $default_int_date )
    	{
    		$error = 1;
    		$error_message = "You must enter an interview day on or later than $default_int_date.<br /><br />";
    	}
    	elseif ( validTime($start_time) == -1)
    	{
    		$error = 1;
    		$error_message = "You must enter a valid start time.<br /><br />";
    	}
	elseif ((compareTimes($current_time, $next_time) != 2) && ($int_date == $current_date))
	{
		$error = 1;
		$error_message = ("You must enter a date and time that are not in the past.<br /><br />");
	}
    	elseif ($int_length <= 0 ) { $error = 1;  $error_message = "You must enter the length of the interviews.<br /><br />"; }

    	if ($error == 1)
    	{
        	 error($error_message);
		if ($coming_from_start)
		{
			getDayPlan( $URL, $interview->intdays, $day, $default_int_date, $default_int_num, $length );
		} else
		{
        	     createStartoverDialog($URL, $schedule, $intdays, $day, $int_date, $int_num, $int_length);
		}
    	}
    	else
    	{
    	  	$start = add_times( validTime($start_time), 0 );
  		for( $i = 0; $i < $int_num; $i++ )
  		{
  			$end = add_times($start, $int_length);
  			$schedule[] = array("START"=>$start, "END"=>$end, "TYPE"=>INTERVIEW);	
  			$start = $end;
		}  	
		
		if ($restore == 1)
		{
			for ($i = 0; $i < sizeof($breakstart); $i++)
			{
				add_to_schedule($breakstart[$i], $breaktype[$i], $breaklength[$i]);
			}
		}
			

			display($URL, $intdays, $day, $int_date, $schedule, 0, $control);

    	}
    	break;
    	
   case "Add to schedule":
		
	if( $add_type == INTERVIEW ) 
	{
		/*
		 If this is a sequential interview, we cannot allow interview lengths that are different that the current
		 ones (there should only be one interview length).
		*/
		
		for ($i = 0; $i < sizeof($schedule); $i++)
		{
			if ($schedule[$i]["TYPE"] == INTERVIEW)
			{
				$int_lengths = cal_length($schedule[$i]["START"], $schedule[$i]["END"]);
				break;
			}
		}
		
		if ($add_length != $int_lengths && isSequential($interview->interview_type))
		{
			error("Interviews must all be of the same length for sequential interviews.");
			$not_right_length = true;
		}
		if (!$not_right_length)
		{
			add_to_schedule($add_start, $add_type, $add_length, $add_start_m);	 
		}
	} elseif( $add_type == LUNCH )							{	
		for( $i=0; $i<count($schedule); $i++ )
		{
			if( $schedule[$i]["TYPE"] == LUNCH )
		    	{
		    		$duplicateLunch = true;
		    		error("The lunch is already scheduled at" . formatTime( $schedule[$i]["START"] ) );
		    		break;
		    	}
		}
		if( !$duplicateLunch )
		{
			add_to_schedule($add_start, $add_type, $add_length, $add_start_m);
		}
	} else
	{
		add_to_schedule($add_start, $add_type, $add_length, $add_start_m);
	}

	display($URL, $intdays, $day, $int_date, $schedule, 0, $control);
	break;
		
   case "Reset Times":

        $num_int = 0;
        foreach($schedule as $s )
   		{
   			if( $s["TYPE"] == INTERVIEW )
   			{
   				$num_int++;
   				if( empty( $start_time ) )
   				{
   					$start_time = $s["START"];
   					$length = cal_length($s["START"], $s["END"] );
   				}
   			}
   		}

	createStartoverDialog($URL, $schedule, $intdays, $day, $int_date, $num_int, $length);
	break;
        	
	/***	
		remove checked event (break or interview or lunch) from 
		schedule, adjust other time slots	
	***/

   	case "Remove selected event":

/*
 New code added below.  Previously it was okay to simply increment $j after
 each item was removed.  However, now the remove function can remove multiple
 items, such as in an interview in the first or last slot with a break 
 adjacent to it.  To account for this, we increment $j by the difference
 between the old size of $schedule and the new size of $schedule.
 *Code added by Adam on 2001-01-31.*
*/
		$remove_interviews = 0;
		$total_interviews = 0;
		
		for ($i = 0; $i < sizeof($remove_list); $i++)
		{
			if ($schedule[$remove_list[$i]]['TYPE'] == INTERVIEW)
			{
				$remove_interviews++;
			}
		}

		for ($i = 0; $i < sizeof($schedule); $i++)
		{
			if ($schedule[$i]['TYPE'] == INTERVIEW)
			{
				$total_interviews++;
			}
		}

		if ($remove_interviews == $total_interviews)
		{
			error("You may not remove all of the interview slots");
		} else
		{
 	  		$j = 0;	
			$current_size = 0;
			$new_size = 0;
		
			for( $i = 0; $i < count($remove_list); $i++ )
			{					

				$current_size = sizeof($schedule);

			// We subtract by j here, because each time we remove 
			// something, the schedule will actually contain less
			// elements next iteration.	

				remove( $remove_list[$i]-$j );
				$new_size = sizeof($schedule);
				$j = $j + ($current_size - $new_size);

			}
		}
	
		display($URL, $intdays, $day, $int_date, $schedule, 0, $control);
		break;
			   		
   default:
      assert( isset($current_date) );
      $day = 1;
      $default_int_date = '';

      $num_students = (float)count( explode(chr(10), $interview->students ) );
      if( isSwitching($interview->interview_type) || isSimultaneous($interview->interview_type) ){ $num_students = ceil( $num_students / 2 ); }

      $default_int_num = ceil( $num_students/$interview->intdays );
      getDayPlan($URL, $interview->intdays, $day, $default_int_date, $default_int_num);

      break;
}



function createStartoverDialog($URL, $schedule, $intdays, $day, $default_int_date, $default_int_num = 1, $default_int_length = '' )
{
	global $interview;
	global $start_time;

	assert( isset($interview) && is_object($interview) );

		
	echo("
		<table border='1' cellspacing='0' cellpadding='4' style='border-color:#HEFEES;' width='60%'>
		<tr>
		<td>
	");

	echo("
		<form method='post' action='$URL'>
		<input type='hidden' name ='interview' value='" . packObject(($interview)) . "' />
		<input type='hidden' name='day' value='$day' />
		<input type='hidden' name='intdays' value='$intdays' />
		<input type='hidden' name='time_mode' value='dayplan_input_complete' />
		<input type='hidden' name='default_int_date' value='$default_int_date' />
		<input type='hidden' name='int_date' value='$default_int_date' />
	");

	for ($i = 0; $i < sizeof($schedule); $i++)
	{
		if ($schedule[$i]['TYPE'] != INTERVIEW)
		{
			$lunchbreaks[] = array(
				"START" => $schedule[$i]['START'],
				"TYPE" => $schedule[$i]['TYPE'],
				"LENGTH" => (cal_length($schedule[$i]['START'], $schedule[$i]['END']))
				);
		}
	}

	for ($i = 0; $i < sizeof($lunchbreaks); $i++)
	{
		echo("
		<input type='hidden' name='breakstart[".$i."]' value='" . $lunchbreaks[$i]['START'] . "' />
		<input type='hidden' name='breaktype[".$i."]' value='" . $lunchbreaks[$i]['TYPE'] . "' />
		<input type='hidden' name='breaklength[".$i."]' value='" . $lunchbreaks[$i]['LENGTH'] . "' />
		");
	}

	echo("
		<table width='100%' border='0' cellpadding='4' cellspacing='0' class='row0d'>
		<tr>
		<td colspan='2'  nowrap='nowrap'><b>" . formatLongDate($default_int_date) . "</b>
		<br />&nbsp;
		</td>
		</tr>
		<tr>
		<td align='right'  nowrap='nowrap'>First interview starts at:
		</td>
		<td>
		<input type='text' name='start_time' value='" . ((isset($start_time)) ? formatTime($start_time, false) : '') . "' size=20 maxlength='8' />
		</td>
		</tr>
		<tr>
		<td align='right'  nowrap='nowrap'>Interview length:
		</td>
		<td>
		<input type='text' name='int_length' value='$default_int_length' size=20 maxlength='3' />
		</td>
		</tr>
		<tr>				
		<td align='right'  nowrap='nowrap'>Number of interviews:
		</td>
	        <td><select name='int_num'>");
		
	
	for( $i = 1; $i < 31; $i++ )
	{
		echo("<option value='$i' " . ( ( $i == $default_int_num ) ? "selected='selected'" : "" ) . ">$i</option>");							
	}

	echo("
		</select>
		</td>
		</tr>
	");

	echo("
	<tr>
	<td align='right'>Restore lunch &amp; break(s):
	</td>
	<td>
	<input type='radio' name='restore' value='1' checked='checked' />Yes
	&nbsp;&nbsp;&nbsp;&nbsp;
	<input type='radio' name='restore' value='0' />No
	</td>
	</tr>
	");
		
	echo("
	<tr>
	<td align='center' colspan='2'>
	<input type='hidden' name='submit' value='timecalculator' />
	<input type='submit' value='Continue' />
	</td>
	</tr>
	");
	
	echo("
	</table>
	</form>
	</td>
	</tr>
	</table>
	");
	
}



?>
