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
 | Filename: edit_jobinfo.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This part of the form allows the user to edit most of the       |
 | information pertaining to the job.  The students short listed cannot be      |
 | edited from here, nor can the interview times and dates be edited.  They     |
 | can be seen however.                                                         |
 +------------------------------------------------------------------------------+

*/

include('misc/isEmailSent.inc');

assert($job_id);
if ($submit != "Save the changes")
{
	$jobresult = $GLOBALS['dbh']->Execute("
		SELECT contact_id, department_id, employer_department_id, employer_id, job_code
		FROM job_info 
		WHERE job_id='" . $job_id . "'");
	$jobrow = $jobresult->FetchRow();
	$job_code = $jobrow["job_code"];
}

if ($uncancel)
{
	// Uncancel the interview.
	$sql = ("
		UPDATE interview
		SET cancelled='0'
		WHERE job_id='" . $job_id . "'
		");
	$GLOBALS['dbh']->Execute($sql);

	// If the interview is associated with jobs, set their status to interviewing.

	$sql = ("
		SELECT DISTINCT job_id
		FROM interview_job_join
		WHERE interview_job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	if (!$result->RecordCount())
	{
		$sql = ("
			UPDATE job_info
			SET admin_status='16'
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	else
	{
		while ($row = $result->FetchRow())
		{
			$sql = ("
				UPDATE job_info
				SET admin_status='16'
				WHERE job_id='" . $row["job_id"] . "'
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
		}
	}
	
	error("Please note: This interview has been uncancelled.");
	unset($uncancel);
}


// if a job has been selected...

/*
 if multiple users have been editing this job (or even the same user two times)...
*/

if( isset($interview2) )
{
	$temp = urldecode($interview2);
	$temp = unserialize($temp);
	$interview2 = new EditInterview($temp);
}

if( isset($interview) )
{
	$interview = unpackObject($interview);
   		
   	if(!empty($interview) && empty($subselection) && !$changedDetected)
	{
	
		$old_interview_medium = $interview->interview_medium;

		$interview->updateJobInfo($job_code, $company_name, $employer_email, $interviewers, $contact,
			$notes, $staff_notes, $interview_medium, $pres_date, $pres_time, $pres_len,
			$pres_loc, $pres_notes, $pres_viewable_by_all, $no_time);
	}
}

?>
	<table width='100%'>
	<tr>
	<td>
	<h3>Interview - Edit  
<?php 
	echo($job_code . "</h3>");
	echo("</td><td align='right'>");
?>
	<form method='post' action='<?php echo $PHP_SELF ?>&amp;PDF=1&amp;job_id=<?php echo $job_id ?>'>
        <input type='hidden' name='department_id' value='<?php echo $jobrow['department_id']; ?>' />          
        <input type='hidden' name='employer_department_id' value='<?php echo $jobrow['employer_department_id']; ?>' />          
        <input type='hidden' name='employer_id' value='<?php echo $jobrow['employer_id'];?>' />
        <input type='hidden' name='job_id' value='<?php echo $job_id;?>' />
        <input type='hidden' name='contact_id' value='<?php echo $jobrow['contact_id'];?>' />
        <input type='hidden' name='job_code' value='<?php echo $jobrow['job_code'];?>' />

	<select name='selection'>
        <option value='view_company'>View this Company</option>
        <?php echo (($jobrow['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" :"" ); ?>
        <?php echo (($jobrow['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" :"" ); ?>
        <option value='view_applications'>View this Job's Applications</option>

        <option value='sign_up'>Sign up Students for this Interview</option> 
	    <option value='single'>Preview Schedule</option>
	    <option value='final'>Final Interview Schedule</option>
        <option value='final_student'>Final Interview Schedule (Student Numbers)</option>
	    <option value='email_schedule'>E-mail Schedule to Employer</option>
<?php

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
	</td></tr></table>
	");
	
$submit = urldecode( $submit );

switch ($submit)
{
case "E-mail Shortlist to Students":

	/*
	 Set email_job_id for e-mailing newsgroup.
	*/

	$email_job_id = $job_id;

	/*
	 Set email_students array, which contains name, student_number, and 
	 e-mail for all students shortlisted for this job.
	*/

	$sql = ("
		SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
		FROM student AS a, students_shortlisted AS b
		WHERE b.job_id='" . addslashes( $email_job_id ) . "'
		AND a.student_number=b.student_number
		ORDER BY a.last_name
		");
	$result = $GLOBALS['dbh']->Execute($sql);

	while ($row = $result->FetchRow())
	{
		$email_address = getStudentEmail($row["student_number"]);
		
		$email_students[] = array(
			"name" => $row["name"],
			"student_number" => $row["student_number"],
			"email" => $email_address
			);
	}
	$result->Close();

	/*
	 Set email_job array, which contains job_id, job_code, int_dates, and
	 the interview medium.
	*/

	$sql = ("
		SELECT DISTINCT int_date
		FROM interview_time
		WHERE job_id='" . addslashes($email_job_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	while ($row = $result->FetchRow())
	{
		$email_int_dates[] = $row["int_date"];
	}
	$result->Close();

	$sql = ("
		SELECT DISTINCT a.job_code, b.company_name
		FROM job_info AS a, employer_company AS b
		WHERE a.job_id='" . addslashes($email_job_id) . "'
		AND b.employer_id=a.employer_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$result->Close();
	$email_job_code = $row["job_code"];
	$email_company_name = $row["company_name"];

	$sql = ("
		SELECT DISTINCT a.description
		FROM interview_medium AS a, interview AS b
		WHERE b.job_id='" . addslashes($email_job_id) . "'
		AND a.interview_medium_id=b.interview_medium_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$result->Close();
	$email_job_medium = $row["description"];

	/*
	 Now we have all the variables, just assign them to the job array.
	*/

	$email_job = array(
		"job_id" => $email_job_id,
		"job_code" => $email_job_code,
		"company_name" => $email_company_name,
		"medium" => $email_job_medium,
		"int_dates" => $email_int_dates
		);
	

	/*
	 Now call the send_newsgroup_mail include file.
	*/

	$return_to = $PHP_SELF . "&amp;select=edit&amp;job_id=" . $job_id . "&amp;level1=jobinfo&amp;submit=";
	$last_part =  urlencode("E-mail Shortlist to Students");
	$return_to .= $last_part;
	$include_after = "interview/add/show_data.inc";
	if (!$case) 
	{
		$case = "email";
	}
	include('interview/edit/mail/send_newsgroup_mail.inc');
	break;

case "E-mail Shortlist to Newsgroup":

	/*
         Set email_job_id for e-mailing newsgroup.
        */

        $email_job_id = $job_id;

        /*
         Set email_students array, which contains name, student_number, and
         e-mail for all students shortlisted for this job.
        */

        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
                FROM student AS a, students_shortlisted AS b
                WHERE b.job_id='" . addslashes( $email_job_id ) . "'
                AND a.student_number=b.student_number
                ORDER BY a.last_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
                $email_address = getStudentEmail($row["student_number"]);

                $email_students[] = array(
                        "name" => $row["name"],
                        "student_number" => $row["student_number"],
                        "email" => $email_address
                        );
        }
        $result->Close();

        /*
         Set email_job array, which contains job_id, job_code, int_dates, and
         the interview medium.
        */

        $sql = ("
                SELECT DISTINCT int_date
                FROM interview_time
                WHERE job_id='" . addslashes($email_job_id) . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
                $email_int_dates[] = $row["int_date"];
        }
        $result->Close();

        $sql = ("
                SELECT DISTINCT a.job_code, b.company_name
                FROM job_info AS a, employer_company AS b
                WHERE a.job_id='" . addslashes($email_job_id) . "'
                AND b.employer_id=a.employer_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $result->Close();
        $email_job_code = $row["job_code"];
        $email_company_name = $row["company_name"];

        $sql = ("
		SELECT DISTINCT a.description
                FROM interview_medium AS a, interview AS b
                WHERE b.job_id='" . addslashes($email_job_id) . "'
                AND a.interview_medium_id=b.interview_medium_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $result->Close();
        $email_job_medium = $row["description"];

        /*
         Now we have all the variables, just assign them to the job array.
        */

        $email_job = array(
                "job_id" => $email_job_id,
                "job_code" => $email_job_code,
                "company_name" => $email_company_name,
                "medium" => $email_job_medium,
                "int_dates" => $email_int_dates
                );


        /*
         Now call the send_newsgroup_mail2 include file.
        */

        $return_to = $PHP_SELF . "&amp;select=edit&amp;job_id=" . $job_id . "&amp;level1=jobinfo&amp;submit=";
        $last_part =  urlencode("E-mail Shortlist to Newsgroup");
        $return_to .= $last_part;
        $include_after = "interview/add/show_data.inc";
        if (!$case)
        {
                $case = "email";
        }
        include('interview/edit/mail/send_newsgroup_mail2.inc');
        break;

case "Send Shortlisted Students an E-mail":

	/*
         Set email_job_id for e-mailing newsgroup.
        */

        $email_job_id = $job_id;

        /*
         Set email_students array, which contains name, student_number, and
         e-mail for all students shortlisted for this job.
        */

        $sql = ("
                SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
                FROM student AS a, students_shortlisted AS b
                WHERE b.job_id='" . addslashes( $email_job_id ) . "'
                AND a.student_number=b.student_number
		ORDER BY a.last_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
                $email_address = getStudentEmail($row["student_number"]);

                $email_students[] = array(
                        "name" => $row["name"],
                        "student_number" => $row["student_number"],
                        "email" => $email_address
                        );
        }
        $result->Close();

	/*
	 We now have the students e-mail addresses, and can load the e-mail form for the user.
	*/

	/*
         Now call the send_newsgroup_mail include file.
        */

        $return_to = $PHP_SELF . "&amp;select=edit&amp;job_id=" . $job_id . "&amp;level1=jobinfo&amp;submit=";
        $last_part =  urlencode("Send Shortlisted Students an E-mail");
        $return_to .= $last_part;
        $include_after = "interview/add/show_data.inc";
        include('interview/edit/mail/send_shortlisted_students_email.inc');
        break;

case "Send Email":
	
	// The e-mail has been written, time to send it.
	include('interview/edit/mail/send_phone_email.inc');
	break;

case "Send Waitroom Email":
	
	// The waitroom e-mail is ready to go.  Send it off.
	include('interview/edit/mail/send_waitroom_email.inc');
	break;

case "Save the changes":

	// ...and the edited job has been resubmitted, update the database
	if (!$changeDetected && isPhoneChangeBad())
	{
		include('interview/edit/confirmChange.inc');
	} else
	{
	        include('interview/edit/update_jobinfo.inc');
	}
        break;

case "Confirm":

	if ($email_send)
	{
		// Interview has been cancelled and e-mail composed.  Now just send the e-mail off.
		include('interview/edit/mail/send_cancel_interview_email.inc');
	}
	else
	{
        	// ...and the interview was cancelled, mark it as such in the database
        	include('interview/edit/cancel_job.inc');
	}
	
        break;

case "Add a presentation":

	$interview->presentation = 1;
	$interview->pres_date = $interview->int_dates[0];
	include('interview/edit/edit_jobinfo_form.inc');
        break;

case "Cancel presentation":

	$interview->presentation = 0;
	error("NOTE: You have cancelled the presentation for this interview.  You will need to save the changes to make this cancellation complete.");
        include('interview/edit/edit_jobinfo_form.inc');
        break;

case "Post Interview":
		
	if( $subselection == "Confirm" )
	{      	
		// Update the student status in the job info table.

		$sql = ("
			SELECT job_id
			FROM interview_job_join
			WHERE interview_job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if ($result->RecordCount())
		{
			while ($row = $result->FetchRow())
			{
				$sql = ("
					UPDATE job_info
					SET student_status='3'
					WHERE job_id='" . $row["job_id"] . "'
					");
				$result2 = $GLOBALS['dbh']->Execute($sql);
			}
		}
		else
		{
			$sql = ("
				UPDATE job_info
				SET student_status='3'
				WHERE job_id='" . $job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}

		// Update the interview information.

		$result = $GLOBALS['dbh']->Execute("
			UPDATE interview 
			SET student_visible= 1
			WHERE job_id = '$job_id'
			");
	        $interview->student_visible= 1;
	
		$result = $GLOBALS['dbh']->Execute("REPLACE INTO interview_edit(job_id, date_edited, editor, menu_edited)
					VALUES('" . $interview->job_id . "', NULL, '" . addslashes($auth->login) . "', 'Button')");

		/*
		 Now we need to set up everything for the e-mail to newsgroups.
		*/

		/*
		 Set email_job_id for e-mailing newsgroup.
		*/
	
		$email_job_id = $job_id;
	
		/*
		 Set email_students array, which contains name, student_number,
		 and e-mail for all students shortlisted for this job.
		*/
	
		$sql = ("
			SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
			FROM student AS a, students_shortlisted AS b
			WHERE b.job_id='" . addslashes( $email_job_id ) . "'
			AND a.student_number=b.student_number
			ORDER BY a.last_name
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	
		while ($row = $result->FetchRow())
		{
			$email_address = getStudentEmail($row["student_number"]);
	
			$email_students[] = array(
				"name" => $row["name"],
				"student_number" => $row["student_number"],
				"email" => $email_address
				);
		}
		$result->Close();
	
		/*
		 Set email_job array, which contains job_id, job_code, 
		 int_dates, and the interview medium.
		*/
	
		$sql = ("
			SELECT DISTINCT int_date
			FROM interview_time
			WHERE job_id='" . addslashes($email_job_id) . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		while ($row = $result->FetchRow())
		{
			$email_int_dates[] = $row["int_date"];
		}
		$result->Close();
	
		$sql = ("
			SELECT DISTINCT a.job_code, b.company_name
			FROM job_info AS a, employer_company AS b
			WHERE a.job_id='" . addslashes($email_job_id) . "'
			AND b.employer_id=a.employer_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$result->Close();
		$email_job_code = $row["job_code"];
		$email_company_name = $row["company_name"];
	
		$sql = ("
			SELECT DISTINCT a.description
			FROM interview_medium AS a, interview AS b
			WHERE b.job_id='" . addslashes($email_job_id) . "'
			AND a.interview_medium_id=b.interview_medium_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$result->Close();
		$email_job_medium = $row["description"];
	
		/*
		 Now we have all the variables, just assign them to the job 
		 array.
		*/
	
		$email_job = array(
			"job_id" => $email_job_id,
			"job_code" => $email_job_code,
			"company_name" => $email_company_name,
			"medium" => $email_job_medium,
			"int_dates" => $email_int_dates
			);
		
	
		/*
		 Now call the send_newsgroup_mail include file.
		*/
	
		$return_to = $PHP_SELF . "&amp;select=edit&amp;job_id=".$job_id."&amp;level1=jobinfo&amp;submit=";
		$last_part = urlencode("E-mail Shortlist to Students");
		$return_to .= $last_part;
		$include_after = "interview/add/show_data.inc";
		$case = "post and email";
		include('interview/edit/mail/send_newsgroup_mail.inc');
	    	
	} elseif( $subselection == "Cancel" )
	{
    		include('interview/edit/edit_jobinfo_form.inc');
	} else
	{
		$message = "Are you sure you want to post interview $interview->job_code?";
		$param[] = array( "interview", packObject(($interview) ) );
		$param[] = array( "submit", "Post Interview" );			
		confirmDialog( $URL, $message, $param );
	}
      	break;
 	
case "Remove from posting":
		
	if( $subselection == "Confirm" )
	{      	
		// Update the student status in the job info table.

		$sql = ("
			SELECT job_id
			FROM interview_job_join
			WHERE interview_job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if ($result->RecordCount())
		{
			// This is a multi job interview.  Update all of the jobs.
			while ($row = $result->FetchRow())
			{
				$sql = ("
					UPDATE job_info
					SET student_status='2'
					WHERE job_id='" . $row["job_id"] . "'
					");
				$result2 = $GLOBALS['dbh']->Execute($sql);
			}
		}
		else
		{
			$sql = ("
				UPDATE job_info
				SET student_status='2'
				WHERE job_id='" . $job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}

		// Update the interview information.

		$result = $GLOBALS['dbh']->Execute("
			UPDATE interview 
			SET student_visible= 0, 
			emailed_newsgroup = 0 
			WHERE job_id = '$job_id'
			");

	        $interview->student_visible= 0;
		$result = $GLOBALS['dbh']->Execute("REPLACE INTO interview_edit (job_id, date_edited, editor, menu_edited)
					VALUES('" . $interview->job_id . "', NULL, '" . addslashes($auth->login) . "', 'Button')");

		
		include('interview/edit/edit_jobinfo_form.inc');
	
	}elseif( $subselection == "Cancel" ){
    	
		include('interview/edit/edit_jobinfo_form.inc');
	
	}else{
		$message = "Are you sure you want to remove interview $interview->job_code from posting?";
		$param[] = array( "interview", packObject(($interview) ) );
		$param[] = array( "submit", "Remove from posting" );			
		confirmDialog( $URL, $message, $param );
	}
      	break;

case "Cancel Interview":

         // ...and the interview was cancelled, mark it as such in the database

         include('interview/edit/confirm_cancel.inc');
         break;

default:

         // ...show the job form
         // get the data from the database

	$interview = new EditInterview('');
	$interview->loadValues($job_id, "Job Info");
	if ($userlevel == OFFICE)
	{
		include('interview/edit/edit_jobinfo_form.inc');
	} else
	{
		include('misc/loguser.inc');
	}
	break;
}







/***		ALL functions are located below this point		***/








/*** reschedule interview on selected day ***/
function createStartoverDialog($day)
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
	
	echo("<table border='1' cellspacing='0' cellpadding='4' style='border-color:#HEFEES;' width='60%'><tr><td>
			<form method='post' action = '$URL'>
			<input type='hidden' name = 'interview' value='" . packObject(( $interview )) . "' />
			<input type='hidden' name = 'day' value='$day' />
			<input type='hidden' name = 'submit' value='Time Calculator' />
			<input type='hidden' name='time_mode' value='Reset Times' /> 			
			
			<table width='100%' border='0' cellpadding='4' cellspacing='0'  class='row0d'>
			<tr>
				<td colspan='2' nowrap='nowrap'><b>" . formatLongDate($interview->int_dates[$day-1]) . "</b><br />&nbsp;</td></tr>
			<tr>
				<td align = 'right'  nowrap='nowrap'>First interview starts at:</td>
				<td><input type='text' name='currStart' value='" . formatTime( $prevStart, false ) . "' size=20 maxlength='8' /></td></tr>
			<tr>
				<td align = 'right'  nowrap='nowrap'>Interview length:</td>
				<td><input type='text' name='currLength' value='$prevLength' size=20 maxlength='3' /></td></tr>
			<tr>				
				<td align = 'right'  nowrap='nowrap'>Number of interviews:</td>
	            <td><select name='currNumInterviews'>");
	
	for( $i = 1; $i < 31; $i++ )
	{
		echo("	<option value='$i' " . ( ( $i == $prevNumInterviews ) ? "selected='selected'" : "" ) . ">$i</option>");							
	}		            	
	echo("  		</select></td></tr>
			<tr>
				<td align='right'>Restore lunch &amp; break(s):</td>
				<td>
					<input type='radio' name='restore' value='1' checked='checked' />Yes
					&nbsp;&nbsp;&nbsp;&nbsp;
					<input type='radio' name='restore' value='0' />No
					</td></tr>
			
			<tr>
				<td align='center'><input type='submit' name='startover' value='Confirm' /></td>
				<td align='center'><input type='submit' name='startover' value='Cancel' /></td></tr>
					
			</table></form></td></tr></table>");
	
}
  	
function confirmDialog( $URL, $message, $param=array() )
{
	echo("<form name='myform' method='post' action='$URL'>");
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
			<td>
				<input type='submit' name='subselection' value='Confirm' />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type='submit' name='subselection' value='Cancel' /></td></tr>
		</table>
		</td></tr></table>
		</form>");
}

function isPhoneChangeBad()
{
	global $interview;

	/*
	 Check here to see if we have changed an off-campus phone interview 
	 into an on-campus phone interview.  If yes, then we check to see 
	 if any students had signed up phone numbers for this interview.
	 If this is the case, true is returned.  If not, then false is 
	 returned.
	*/
	
	$checkresult = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT interview_medium_id, interview_place_id
		FROM interview
		WHERE job_id='" . $interview->job_id . "'
		");
	
	$checkrow = $checkresult->FetchRow();
	$checkresult->Close();
	
	if (isPhone($checkrow["interview_medium_id"]) && !isOnCampus($checkrow["interview_place_id"]) && isPhone($interview->interview_medium) && isOnCampus($interview->interview_place))
	{
		/*
		 The interview has changed from off-campus to on-campus.  If 
		 any students had signed up with phone numbers, grab their 
		 student numbers, and remove their phone numbers from the 
		 database.
		*/

		$phoneresult = $GLOBALS['dbh']->Execute("
			SELECT DISTINCT a.student_number, a.first_name, a.last_name, c.time_id
			FROM student AS a, interview_time as b, interview_phone as c
			WHERE b.job_id='" . $interview->job_id . "'
			AND c.time_id=b.time_id
			AND b.student_number=a.student_number
			AND c.phone IS NOT NULL
			");
		
		if ($phoneresult->RecordCount() != 0)
		{
			global $phone_student;
			while ($phonerow = $phoneresult->FetchRow())
			{
				$name = $phonerow["first_name"] . " " . $phonerow["last_name"];
				$phone_student[] = array(
					"student_number" => $phonerow["student_number"],
					"name" => $name
				);
			}
			return true;
		} else
		{
			return false;
		}
	} else
	{
		return false;
	}
}


?>
