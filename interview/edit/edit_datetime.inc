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
 | Filename: edit_datetime.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This file is called by editchoose.inc and allows the user to    |
 | edit the interview dates and times for various interviews.                   |
 +------------------------------------------------------------------------------+

*/

	assert($job_id);
	$jobresult = $GLOBALS['dbh']->Execute("
			SELECT contact_id, department_id, employer_department_id, employer_id, job_code
			FROM job_info 
			WHERE job_id='" . $job_id . "'
			");
	$jobrow = $jobresult->FetchRow();
	$job_code = $jobrow["job_code"];
		
	$URL = "$PHP_SELF&amp;select=edit&amp;level1=timedate&amp;job_id=$job_id";

	
	$control = "
	<hr width='100%' align='left' />
	<table cellpadding='4' cellspacing='0' width='100%' border='0' class='row1'>
	<tr>
	<td align='center' colspan='2'>
	<input type='submit' name='time_mode' value='Save changes' />
	</td>
	</tr>
	</table>";

	

	$control2 = "
	<input type='hidden' name='submit' value='Time Calculator' />
	<table border='0' cellpadding='2' cellspacing='0' width='100%'>
	<tr>
	<td colspan='2'>
	</td>
	</tr>
	<tr  class='row0d'>
	<td colspan='2' align='center'>
	<table border='0' cellspacing='0' cellpadding='4' width='100%' class='row1'>
	<tr align='center'>
		<td>
		<input type='submit' name='time_mode' value='Reset Times' />
		</td>
		<td>
		<input type='submit' name='time_mode' value='Manual mode' />
		</td>
	</tr>
	<tr align='center'>
		<td>
		<input type='submit' name='submit' value='Add an interview date' />
		</td>
		<td>
		<input type='submit' name='submit' value='Change date' />
		</td>
	</tr>
	</table>
	</td>
	</tr>
	</table>
	";

	
   // if a job has been selected...
   if( isset($interview) )
   {
		$interview = unpackObject($interview);
   		
		if( $submit != "Close Time Calculator" && $submit != "Modify Schedule" && $submit != "Time Calculator" && empty($subselection) && $submit != "Change date" && $submit != "Add an interview date")
   		{	   			
   			$interview->updateValues($interview->job_code, $interview->company_name, $interview->employer_email, 
						$interview->interviewers, $interview->contact, 
						$int_dates, $interview->students, $interview->notes, $interview->staff_notes, 
						$interview->interview_medium, $interview->pres_date, $interview->pres_time, 
						$interview->pres_len, $interview->pres_loc, $interview->pres_notes, $interview->pres_viewable_by_all,
						$interview->interview_place, $interview->no_time, $use_room_number );
		}
   if( $submit == "Time Calculator" )
   {
		if ( $date == "date_done")
		{
			$interview->updateValues($interview->job_code, $interview->company_name, $interview->employer_email, 
						$interview->interviewers, $interview->contact, 
						$int_dates, $interview->students, $interview->notes, $interview->staff_notes, 
						$interview->interview_medium, $interview->pres_date, $interview->pres_time, 
						$interview->pres_len, $interview->pres_loc, $interview->pres_notes, $interview->pres_viewable_by_all, 
						$interview->interview_place, $interview->no_time, $use_room_number );
		}
		if( isset($schedule) && is_string($schedule) )  $schedule = unpackObject(( $schedule ) );
   }
   }

// ?>
	<table width='100%'>
	<tr>
	<td>
	<h3>Interview - Edit 

	<?php 

    echo($job_code); 
	echo("</h3>");
	echo("</td><td align='right'>");
	echo("
	<form method='post' action='$PHP_SELF&amp;PDF=1&amp;job_id=$job_id'>
    
        <input type='hidden' name='employer_department_id' value='".$jobrow['employer_department_id']."' />          
    <input type='hidden' name='department_id' value='".$jobrow['department_id']."' />          
    <input type='hidden' name='employer_id' value='".$jobrow['employer_id']."' />
    <input type='hidden' name='job_id' value='".$job_id."' />
    <input type='hidden' name='contact_id' value='".$jobrow['contact_id']."' />
    <input type='hidden' name='job_code' value='".$jobrow['job_code']."' />

    <select name='selection'>
        <option value='view_company'>View this Company</option>
        ".(($jobrow['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" : "")."
        ".(($jobrow['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" : "")."
        <option value='view_applications'>View this Job's Applications</option>

        <option value='sign_up'>Sign up Students for this Interview</option> 
	    <option value='single'>Preview Schedule</option>
	    <option value='final'>Final Interview Schedule</option>
        <option value='final_student'>Final Interview Schedule (Student Numbers)</option>
	    <option value='email_schedule'>E-mail Schedule to Employer</option>
	");

$sql = ("
	SELECT DISTINCT emailed_newsgroup
	FROM interview
	WHERE job_id='" . $job_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

if (trim($row["emailed_newsgroup"]))
{
	echo("<option value='email_students'>E-mail Shortlisted Students</option>");
}

echo("
	</select>
	&nbsp;
	<input type='image' src='misc/images/arrowg.gif' alt='go' />
	</form>
	</td></tr></table><br />
	");

   $submit = urldecode( $submit );
   switch ($submit)
   {
      case "Time Calculator":
		calculatorHandler($URL, $day);    	
   		break;   	

      case "Modify Schedule":
		if ($selection_x < 59)
		{
			if ($interview->int_dates[$modifyIndex] < $current_date)
			{
				error("You may not edit interview times or dates in the past.");
				displayAll($interview, $URL);
			} else
			{
				display($URL, count( $interview->int_dates ), $modifyIndex+1, $interview->int_dates[$modifyIndex], $interview->schedules[$modifyIndex], 0, $control, $control2 );
			}
		} elseif ($selection_x < 135)
		{
			change_dates($URL, count( $interview->int_dates ));
		} else
		{
			if ($interview->int_dates[$modifyIndex] < $current_date)
			{
				error("You may not edit interview times or dates in the past.");
				displayAll($interview, $URL);
			} else
			{
				$cancel_day[0] = $modifyIndex;
				$cancel_date = $interview->int_dates[$modifyIndex];
				include('interview/edit/delete_date.inc');
			}
		}
	        break;

      case "Change date":
		change_dates($URL, count( $interview->int_dates));
		break;

      case "Save the changes":
         // ...and the edited job has been resubmitted, update the database
         include('interview/edit/update_jobinfo.inc');
	 break;

	
      case "Add an interview date":

	 //copy the daily schedule of previous last day to new added day

	 if( count($interview->schedules) >= 1 && count($interview->int_dates) >= 1 )
	 {
		 /*
		  Set the new days schedule to be identical to the last day currently existant.
		 */

		 $interview->schedules[count($interview->schedules)] = end($interview->schedules);		
	
		 $interview->int_dates[count($interview->int_dates)] = add_dates( end($interview->int_dates), 1);		
	 }			
         else
         {
		 $interview->schedules[count($interview->schedules)] = array( 0=>array("START"=>'12:00:00', "END"=>'12:30:00', "TYPE"=>INTERVIEW) );		
		 $interview->int_dates[count($interview->int_dates)] = $current_date;		
         }

	 createStartoverDialog( sizeof($interview->int_dates), true);

	break;

	 
	
      case "Calculate time":

         displayAll($interview, $URL);
		 break;
		
      case "Close Time Calculator":
	  			
		 include('interview/edit/edit_jobinfo_form.inc');
		 break;		

      case "Send email":
		include('interview/edit/mail/send_dropped_email.inc');
		break;
 	
      case "Send Date Email":
		include('interview/edit/mail/send_date_email.inc');
		break;

      case "Back to Edit":
      	 unset($submit); 		//re-initialize

      default:
         // ...show the job form

         // get the data from the database

	$interview = new EditInterview('');
	$interview->loadValues($job_id, 'Date/Time');
         
	if (sizeof($interview->int_dates) == 1)
	{
		display($URL, count( $interview->int_dates ), 1, $interview->int_dates[0], $interview->schedules[0], 0, $control, $control2);
	} else
	{
		displayAll($interview, $URL);
	}
        break;
   }










/** 		     All functions found below this point 		**/












/*** display all interview schedules for selected job ***/

function displayAll($interview, $URL)
{
	for( $i=0; $i < count($interview->int_dates); $i++ )
   	{
		
		echo("<table width='100%' cellpadding='0' cellspacing='0' border='1' style='border-color:#HEFEES;'>
			<tr>
			  <td>
			    <form method='post' action='$URL'>\n");			
		
		echo("<input type='hidden' name = 'interview' value='" . packObject(( $interview )) . "' />
			  <input type='hidden' name = 'modifyIndex' value='$i' />
		      <input type='hidden' name='submit' value='Modify Schedule' />");	
		
		echo("<table width='100%' border='0'>
			<tr>
			<td>
			<b>" . formatLongDate($interview->int_dates[$i]) . "</b>
			</td>
			<td align='right' valign='middle'>
			<input type='image' name='selection' src='misc/images/select_bar.gif' border='0' />&nbsp;");
		echo("
			</td>
			");
		echo("</tr>
				<tr><td colspan='2'>");
		if ( $interview->modes[$i] != "MIN")
		{
			displayTimeTable( $interview->schedules[$i], 0, $interview->int_dates[$i]);			
		}
					
		echo("</td></tr></table>");
		
		echo("</form><br /></td></tr></table>");
		
	}
	echo("
	<form method='post' action='$URL'>
	<input type='hidden' name='interview' value='" . packObject(( $interview )) . "' />
	<table width='100%'><tr align='center'>
	<td>
	<input type='submit' name='submit' value='Add an interview date' />
	</td>
	</tr>
	</table>
	</form>");
	echo("<hr />");
}

function calculatorHandler($URL, $day)
{
	global $interview;
	global $interview2;
	global $job_id;
	global $error_happened;
	global $schedule;
	global $m_schedule;
	global $time_mode;
	global $control;
	global $control2;
	global $studentemail;
	global $students_dropped;
	global $noemail_studentnum;
	global $noemail_studentname;
	global $email_employer;
	global $cancel_day;
	global $day_added;
	global $date_changed;
	global $ignore;
	
	global $add_start;
	global $add_type;
	global $add_length;
	global $add_start_m;
	global $remove_list;
	
	global $startover;
	global $currDate;
	global $currStart;
	global $currLength;
	global $currNumInterviews;
	global $restore;

	echo($error_happened);
	switch( $time_mode )
	{	
	
	   	case "Manual mode":
			display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 1, $control, $control2);
			break;
	
	/*** 	Return to calculator mode	***/
			
		case "Close manual mode":
			display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 0, $control, $control2);
			break;


	/*** 	sub-procedure in 'Manual Mode', update schedule		***/

		case "Update and Save changes":

			update_manual_schedule();
			
			$interview->schedules[$day-1] = $schedule;

			include('interview/edit/update_datetime.inc');

			if ($error_happened)
			{
				$interview = new EditInterview('');
				$interview->loadValues($job_id, 'Date/Time');
			}
			
			if (trim($students_dropped) != '')
			{
				include('interview/edit/mail/send_dropped_email.inc');
			}
			else
			{
				display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 1, $control, $control2, '', $edit_conflict);
			}
			break;

	/***	Save those changes!		***/
			
		case "Save changes":
			
			include('interview/edit/update_datetime.inc');
			if ($date_changed || $error_happened)
			{
				$interview = new EditInterview('');
				$interview->loadValues($job_id, 'Date/Time');
			}
			if ($date_changed && $error_happened)
			{
				if ($edit_conflict){
					if (sizeof($interview->int_dates) == 1){
						display($URL, count( $interview->int_dates ), 1, $interview->int_dates[0], $interview->schedules[0], 0, $control, $control2, '', $edit_conflict);
					}else{
						displayAll($interview, $URL);
						display_edit_conflict($interview2);
					}
				}else{
					change_dates($URL, count( $interview->int_dates ));
				}
				unset($date_changed);
			}
			elseif ($date_changed && !$error_happened && $date_email)
			{
				include('interview/edit/mail/send_date_email.inc');
			}
			elseif (trim($students_dropped) != '')
			{
				include('interview/edit/mail/send_dropped_email.inc');
			}
			elseif (sizeof($interview->int_dates) == 1)
			{
				display($URL, count( $interview->int_dates ), 1, $interview->int_dates[0], $interview->schedules[0], 0, $control, $control2, '', $edit_conflict);
			} else
			{
				echo("<br /><br />");
				displayAll($interview, $URL);
				if ($edit_conflict){
					display_edit_conflict($interview2);
				}
			}
			break;
	
	
/* 
 Pass daily schedule to 'Temp' obj, go to next day if there is more left.
 Otherwise, call 'add_job.inc' to store all user input into database
*/	
	
   		case "Finish":
			//write to temp object
			$interview->schedules[$day-1] = $schedule;
	        	displayAll($interview, $URL);
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
        	}	
		elseif( $add_type == LUNCH )
		{	
			for( $i=0; $i<count($schedule); $i++ )
	   		{
				if( $schedule[$i]["TYPE"] == LUNCH )
		    		{
		    			error("The lunch is already scheduled at" . formatTime( $schedule[$i]["START"] ) );
		    			$duplicate_lunch = true;
					break;
				}
			}
			if (!$duplicate_lunch)
			{
				add_to_schedule($add_start, $add_type, $add_length, $add_start_m);
			}
		} else
		{
			add_to_schedule($add_start, $add_type, $add_length, $add_start_m);
		}
					
		$interview->schedules[$day-1] = $schedule;
					
		display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 0, $control, $control2);
			
		break;
		
   		
/*
 Remove checked event (break or lunch) from schedule, adjust other time slots
*/
   	case "Remove selected event":
	  	
		$j = 0;		

		/*
		 Do a quick check to make sure we aren't deleting the only interview for this day.
		*/

		$count_ints = 0;
		for ($i = 0; $i < sizeof($schedule); $i++)
		{
			if ($schedule[$i]["TYPE"] == INTERVIEW)
			{
				$count_ints++;
			}
		}

		if ($schedule[$remove_list[0]]["TYPE"] == INTERVIEW && $count_ints == 1)
		{
			error("You may not remove the last interview for the day.");
		}
		else
		{
			for( $i = 0; $i < count($remove_list); $i++ )
			{
				remove( $remove_list[$i]-$j );
				$j++;
			}
			$interview->schedules[$day-1] = $schedule;
		}
				
		display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 0, $control, $control2);
		
		break;
		
	case "Reset Times":
		
		if ( $startover == "Confirm" )
		{
			$error = 0;
			if (check_date($currDate) == 0)
			{
				$error = 1;
				error("You must provide a valid date, that is not in the past, and is in the format YYYY-MM-DD.");
				createStartoverDialog( $day, $day_added );
			}
			else
			{
				$interview->int_dates[$day - 1] = $currDate;
				// If there is a problem in this section, this could be why (above)
				//$interview->int_dates[count($interview->int_dates) - 1] = $currDate;
			}

			if (($error == 0) && isDuplicate( $interview->int_dates, $duplicate))
        		{
        		        $error = 1;
        		        error("Duplicate interview dates $duplicate.");
				createStartoverDialog($day, $day_added);
        		}

			if(($error == 0) && (($start = validTime( $currStart )) == -1 || ($length = intval($currLength)) < 1))
			{
				$error = 1;
				echo("Invalid " . (( $start == -1 )? "time<br /><br />" : "interview length<br /><br />" ));
				createStartoverDialog( $day, $day_added );
    			}

    			if ($error == 0)
    			{
	    		   	$start = add_times( $start, 0 );
	    			for( $i = 0; $i < $currNumInterviews; $i++ )
		  		{
	  				$end = add_times($start, $length);
					$schedule[] = array( "START"=>$start, "END"=>$end, "TYPE"=>INTERVIEW );	
					$start = $end;
				}  	
	    		   	/* restore lunch and breaks */
	    		   	if( $restore )
	    		   	{
				   	for( $i = 0; $i < count( $interview->schedules[$day-1] ); $i++ )
	    			   	{
						if( $i >= count( $schedule ) )
						break;								
						if( $interview->schedules[$day-1][$i]["TYPE"] != INTERVIEW )
						{
							add_to_schedule($schedule[$i]["START"], $interview->schedules[$day-1][$i]["TYPE"], cal_length($interview->schedules[$day-1][$i]["START"],$interview->schedules[$day-1][$i]["END"]) );
						}								
	    			   	}
	    			}
						
				$interview->schedules[$day-1] = $schedule;
			
				if ($day_added)
				{
					include('interview/edit/update_datetime.inc');
					displayAll($interview, $URL);
					if($edit_conflict){
						display_edit_conflict($interview2);
					}
				} else
				{
					display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 0, $control, $control2);
				}
			}	
		} else if ( $startover == "Cancel" )
		{
			display($URL, count($interview->int_dates), $day, $interview->int_dates[$day-1], $interview->schedules[$day-1], 0, $control, $control2);
		} else
		{
			createStartoverDialog( $day, $day_added );
		}
		break;   	
	
	} //switch ($time_mode)

} //function CalculatorHandler

/*** 		reschedule interview on selected day 		***/

function createStartoverDialog($day, $day_added='')
{
	global $URL;
	global $interview;

	$prevNumInterviews = 0;
	$prevStart = null;

	foreach( $interview->schedules[$day-1] as $s )
	{
		if( $s['TYPE'] == INTERVIEW )
		{
			if( $prevStart == null )
			{
				$prevStart = $s['START'];
				$prevLength = cal_length( $s['START'], $s['END'] );
			}
			
			$prevNumInterviews++;			
		}
	}

	echo("
		<form method='post' name='myform' action = '$URL'>
		<input type='hidden' NAME = 'interview' value='" . packObject(( $interview )) . "' />
		<input type='hidden' NAME = 'day' value='$day' />
		<input type='hidden' NAME = 'submit' value='Time Calculator' />
		<input type='hidden' name='time_mode' value='Reset Times' />
		<input type='hidden' name='day_added' value='$day_added' />
	");
	

	echo("
		<table width='100%' border='0' cellpadding='4' cellspacing='0' class='row1'>
		<tr>
		<td align='right' nowrap='nowrap'>Interview Date:</td>
		<td><input type='text' name='currDate' value='" . (($currStart) ? $currStart  : $interview->int_dates[$day - 1]) . "' />");
		 echo popup('currDate','myform');
	echo("
		</td>
		</tr>
		<tr>
		<td align = 'right'  nowrap='nowrap'>First interview starts at:
		</td>
		<td>
		<input type='text' name='currStart' value='" . formatTime( $prevStart, false ) . "' size='20' maxlength='8' />
		</td>
		</tr>
		<tr>
		<td align = 'right'  nowrap='nowrap'>
		" . (($interview->interview_type == 2) ? "Total time a student is interviewed for:" : "Interview length (minutes):") . "
		</td>
		<td>
		<input type='text' name='currLength' value='$prevLength' size='20' maxlength='3' />
		</td>
		</tr>
		<tr>				
		<td align = 'right'  nowrap='nowrap'>
		" . (($interview->interview_type == 3) ? "Number of students<br />seeing each interviewer:" : "Number of interview times:") . "
		</td>
	        <td><select name='currNumInterviews'>");
		
	
	for( $i = 1; $i < 31; $i++ )
	{
		echo("<option value='$i' " . ( ( $i == $prevNumInterviews ) ? "selected='selected'" : "" ) . ">$i</option>");							
	}

	echo("
		</select>
		</td>
		</tr>
	");

	if (!$day_added)
	{
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
	}
	
	if ($day_added)
	{
		echo("
		<tr>
		<td align='center' colspan='2'><hr />
		<input type='submit' name='startover' value='Confirm' />
		</td>
		</tr>
		");
	} else
	{
		echo("
		<tr>
		<td colspan='2' align='center'><hr />
		<input type='submit' name='startover' value='Confirm' />&nbsp;&nbsp;&nbsp;&nbsp;
		<input type='submit' name='startover' value='Cancel' />
		</td>
		</tr>
		");
	}
	echo("
		</table>
		</form>
	");
	
}
  	
function confirmDialog( $URL, $message, $param=array() )
{
	echo("<form method='post' action='$URL'>");
	for( $i = 0; $i < count( $param ); $i++ )
	{
		echo("<input type='hidden' name='{$param[$i][0]}' value='{$param[$i][1]}' />");
	}
	echo("
		<table width='100%' cellpadding='2' cellspacing='0' class='row1' border='1'><tr><td>
		<table width='100%' cellpadding='4' cellspacing='0'>
		<tr>
			<td align='left'>$message</td></tr>
		<tr>
			<td><hr />
				<input type='submit' name='subselection' value='Confirm' />&nbsp;&nbsp;
				<input type='submit' name='subselection' value='Cancel' /></td></tr>
		</table>
		</td></tr></table>
		</form>");
}

function change_dates($URL, $intdays)
{

	global $interview;
	global $current_date;

	$msg = ("
		<b class=instruct>Warning:</b> Changing dates that have students signed up for interviews will drop those students from their interviews unless the identical time/day slot exists after the change.  If you're unsure of whether or not a change will affect an interview, push the BACK button on your browser to double-check.
		");
	instruct($msg);
	
	$form_header = ("
		<form name='myform' method='post' action='$URL'>
		<table border='0' cellpadding='4' cellspacing='0' width='100%'>
		<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />
		<input type='hidden' name='date' value='date_done' />
		<input type='hidden' name='job_id' value='$interview->job_id' />
		");
		

	echo($form_header);

	
/***	 Display the dates and boxes to change them in.		***/
	
	echo("
	<tr align='center'>
	<td>
	<table border='0' cellpadding='2' cellspacing='0'>
	<tr class='row0d'>
	<td>
	<table border='0' cellspacing='0' cellpadding='5' class='row1'>
	<tr><td><table border='0'>
	<tr>
		<td colspan='2'>
		<br />
		</td>
	</tr>
	");
	$count=3;
	for ($i = 0; $i < sizeof($interview->int_dates); $i++)
	{
		echo("
		<tr>
		<td align='center'>&nbsp;&nbsp;Change date:&nbsp;
		" . $interview->int_dates[$i] . "
		&nbsp;&nbsp;&nbsp;To:&nbsp;");
		if ($interview->int_dates[$i] < $current_date)
		{
		echo("
		<input type='hidden' name='int_dates[$i]' value='{$interview->int_dates[$i]}' />
		<input type='hidden' name='ignore[$i]' value='1' />
		");
		echo($interview->int_dates[$i] . "&nbsp;&nbsp;</td></tr>");
		} else
		{
		echo("		
		<input type='text' name='int_dates[$i]' value='{$interview->int_dates[$i]}' size='12' maxlength='12' />&nbsp;");
		$variable="int_dates[".$i."]";
		echo popup($variable,'myform');	
		echo("
		<input type='hidden' name='ignore[$i]' value='0' />
		</td>
		</tr>
		");
		}
		echo("
		<tr><td colspan='2'><br /></td></tr>
		");
		$count=$count+2;
	}
			
	echo("
		<tr>
		<td colspan='2' align='center'><hr />
	");	
	echo("
		<input type='hidden' name='submit' value='Time Calculator' />
		<input type='hidden' name='date_changed' value='true' />
		<input type='submit' name='time_mode' value='Save changes' />
	");

	echo("
		</td>
		</tr>
		</table>
		</td>
		</tr>
		</table>
		</td>
		</tr>
		</table>
		</tr>
		</td>
		</table>
		</form>
	");



}

function remove_sign_up($remove_start, $remove_date, $remove_job_id)
{
	/*
		validation - already guaranteed
		1. valid job_id
		
		validation - add only
		1. time_id<->job_id valid interview time with switch=0
		2. time_id is currently not empty
	*/

	global $job_id;
 			
	if (!isValidTimeID($time_id, $job_id))
	{
		// someone is probably URL hacking if they end up here.
		return false;
	}
		if (isUnsignedTimeID($time_id))
	{
		// you can't cancel a non-existent interview time
		return false;
	}
	
	if (!$datetime = getDateTime($time_id))
	{
		error("Query failed.  The sign-up could not be processed.  Please try again.");
		return false;
	}
	$date = $datetime["date"];
	$time = $datetime["time"];
		if (!$time_ids = get_switch($time_id)) { return false; }
		$result = $GLOBALS['dbh']->Execute("
		UPDATE	interview_time
		SET		student_number=NULL
		WHERE		time_id IN (" . $time_ids . ")");
	if (!$result || !$GLOBALS['dbh']->Affected_Rows())
	{
		error("Query failed.  Sign-up may not have been removed (please check).");
		return false;
	}
	notify("The sign-up for the " . formatLongDate($date) . " " . formatTime($time, false)
						. " interview has been cancelled.  Please select another time.");
	
	$result = $GLOBALS['dbh']->Execute("
		DELETE FROM interview_phone
		WHERE		time_id IN (" . $time_ids . ")");
}




?>
