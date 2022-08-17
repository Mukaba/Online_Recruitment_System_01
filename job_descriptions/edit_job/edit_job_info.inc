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
 | Filename: edit_job_info.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This file is the file that does all of the error checking and   |
 | validation for job editing, and calls up and loads the proper pages.         |
 +------------------------------------------------------------------------------+

*/

$error = 0;

if( $submit == '')
{
	//no form has been submited yet, so show first screen
	include('job_descriptions/edit_job/edit_job_form.inc' );
}

if ($submit == 'Refresh Disciplines')
{
	$sql = ("
		SELECT job_code
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$job_code = $row["job_code"];

	$job = new EditJob('');

    $job->set_value_1($position_title, $job_code, $employer_code, $num_positions, $apply_method, $receive_address
            , $country, $provstate, $region, $city, $salary_amount1, $salary_amount2, $salary_period, $start_term
            , $start_year, $start_date, $start_date_date, $start_date_month, $start_date_year, $end_date, $end_date_date
            , $end_date_month, $end_date_year, $closing_date, $closing_time, $workterm_lengths, $workterm_hours, $other_postings
            , $min_academic, $min_workterms, $apply_departments, $job_description, $skills_required, $special_requirements, $comments
            , $admin_comments, $for_student_comments, $admin_status, $status, $job_disciplines, $special_instructions, $special_instr_button
            , $show_special_instr_button, $displayname_on_coverltr
            );

	$job->job_id = $job_id;
	
	include('job_descriptions/edit_job/edit_job_form.inc');
}

if( $submit == 'Save Changes')
{
	// information has been entered on the first screen
   	// check to make sure info from 1st screen was entered correctly, 
	// if so continue, otherwise, go back.  Save inputs by setting the
	// corresponding attributes of the EditJob object

	$curterm = getCurrentTermID();
	$tempdate = explode("-", $current_date);

	if ($apply_method == JOB_APP_METHOD_PAPER)
	{
		$receive_address = '';
	}

    if ((trim($salary_amount1) == '') && (trim($salary_amount2) == ''))
    {       
        $salary_period = '';
    }
    else
    {
        if (trim($salary_amount1) == '')
        {
            $salary_amount1 = $salary_amount2;
        }
        if (trim($salary_amount2) == '')
        {
            $salary_amount2 = $salary_amount1;
        }
        $salary_amount1 = normalizeSalary($salary_amount1);
        $salary_amount2 = normalizeSalary($salary_amount2);
    }

	$sql = ("
		SELECT status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$old_status = $row["status"];

	if ($userlevel == EMPLOYER || $userlevel == HREMPLOYER)
	{
		$admin_status = NOT_CHECKED_EMPLOYER;
		$status = NOT_CHECKED;
	}
	elseif ($old_status == HOLDING && $userlevel != TRAFFICDIRECTOR)
	{
		$admin_status = HOLDING_ADMIN;
		$status = $old_status;
	}
	else
	{
		$sql = ("
			SELECT DISTINCT job_status_id
			FROM admin_status
			WHERE status_id='" . $admin_status . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$status = $row["job_status_id"];
	}

	$apply_departments = trim_array($apply_departments);
	$job_disciplines = trim_array($job_disciplines);
	if (!$job_disciplines)
	{
		$job_disciplines[] = ALL_DISCIPLINES;
	}

	// Get the job id from the database, since we don't get it from the previous form.

	$sql = ("
		SELECT job_code
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$job_code = $row["job_code"];

	$job = new EditJob('');

    $job->set_value_1($position_title, $job_code, $employer_code, $num_positions, $apply_method, $receive_address
            , $country, $provstate, $region, $city, $salary_amount1, $salary_amount2, $salary_period, $start_term
            , $start_year, $start_date, $start_date_date, $start_date_month, $start_date_year, $end_date, $end_date_date
            , $end_date_month, $end_date_year, $closing_date, $closing_time, $workterm_lengths, $workterm_hours, $other_postings
            , $min_academic, $min_workterms, $apply_departments, $job_description, $skills_required, $special_requirements, $comments
            , $admin_comments, $for_student_comments, $admin_status, $status, $job_disciplines, $special_instructions, $special_instr_button
            , $show_special_instr_button, $displayname_on_coverltr
            );

	$job->job_id = $job_id;

	// Begin testing the given values here.
	
	$sql = ("
		SELECT DISTINCT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result->RecordCount())
	{
		$sql = ("
			SELECT DISTINCT job_id
			FROM interview
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		if ($result->RecordCount())
		{
			// This job has an interview.
			$job_has_interview = true;
		}
	}
	else
	{
		/*
		 This is part of a multijob interview.  Check and make sure that
		 all of the other jobs already have not interviewing set.
		*/

		$row = $result->FetchRow();

		$sql = ("
			SELECT DISTINCT ji.admin_status
			FROM interview_job_join AS ijj, job_info AS ji
			WHERE ijj.interview_job_id='" . $row["interview_job_id"] . "'
			AND ijj.job_id!='" . $job_id . "'
			AND ji.job_id=ijj.job_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		$job_has_interview = true;
		while ($row = $result->FetchRow())
		{
			if ($row["admin_status"] != NO_INTERVIEW_NO_SUITABLE_APPS && $row["admin_status"] != NO_INTERVIEW_NO_FUNDING 
                && $row["admin_status"] != NO_INTERVIEW_OTHER)
			{
				$job_has_interview = false;
			}
		}
	}
			

	/*
	 The first thing we need to do is check and make sure that the job code doesn't already exist in the
	 database for the given year and term.
	*/

	$sql = ("
        SELECT job_id, status
        FROM job_info
        WHERE year='" . $job->start_year . "'
        AND term_id='" . $job->start_term . "'
        AND job_code='" . $job->job_code . "'
        AND job_id!='" . $job_id . "'
        AND status!='" . CANCELLED . "'
        AND status!='" . INT_ONLY . "'
        AND job_code IS NOT NULL
        AND job_code!=''
        ");
        
	$result = $GLOBALS['dbh']->Execute($sql);
    if ($result->RecordCount())
    {
        $error = 1;
        $row = $result->FetchRow();
        $msg = ("There is already a job with job code: <b>" . $job_code . "</b> in the system for  ");
        $msg .= (getTermName($start_term) . ", " . $start_year . ".  ");
        error($msg);
        include('job_descriptions/edit_job/edit_job_form.inc');
    }	
	
	elseif( trim($position_title) == '')
	{
		$error = 1;
		error("You must enter the title of the position.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}
	
	/*
	Should be unnecessary now that job codes are automated.
	elseif((trim($job_code) == '') && ($userlevel == OFFICE) && (($status == CHECKED) || ($status == POSTED)))
	{
		$error = 1;
		error("To save this job with a status of <b>Checked</b> or <b>Posted</b>, you must provide a job code.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}
	*/

    elseif ($job->num_positions == '' || !ereg("^([0-9])+$", $job->num_positions))
    {
        $error = 1;
        error("You must enter a valid number of positions");
        include('job_descriptions/edit_job/edit_job_form.inc');
    }
    elseif((!isValidEmail(trim($job->receive_address))) && ($apply_method == JOB_APP_METHOD_ONLINE))
	{
		$error = 1;
		error("If you wish the applications to be online, a valid e-mail address must be included.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif( trim($city) == '')
	{
		$error = 1;
		error("You must enter a city location to add a job.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}
	
	elseif (!isValidSalary($job->salary_amount1) && ($job->salary_amount1 != '')) 
    {
        $error = 1;
        error("A valid salary amount/range must be included, or none at all.");
        include('job_descriptions/edit_job/edit_job_form.inc');
    }	

	// $temp_date is set at the start of this file, and is used
	// to check the validity of start_month and start_year.
	
    //:TODO: Remove hardcoded logic
	elseif (($tempdate[0] == $job->start_year) && (($curterm == 1 && $job->start_term == 2) ||
            ($curterm == 1 && $job->start_term == 3) || ($curterm == 3 && $job->start_term == 2)))
    {
        $error = 1;
        error("You must enter a start date that is in the future or present.");
        include('job_descriptions/edit_job/edit_job_form.inc');
    }

	elseif((check_date($closing_date) == 0) && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) && (($status == POSTED) || ($status == CHECKED)) && $admin_status != CHECKED_WAITING_DATE)
	{
		$error=1;
		error("You must provide a valid closing date in the form YYYY-MM-DD"
		. " and no earlier than today's date $current_date.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif((validTime($closing_time) == -1) && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR) && (($status == POSTED) || ($status == CHECKED)))
	{
		$error=1;
		error("You must provide a valid closing time in 24 hour format");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif (!$workterm_lengths)
    {
        $error = 1;
        error("You must specify at least one workterm length.");
        include('job_descriptions/edit_job/edit_job_form.inc');
    }
	
	elseif (($workterm_hours != '') && (!ereg("(.*[0-9]+.*)+", $workterm_hours)))
	{
		$error = 1;
		error("You must specify a valid number of workterm hours per week, or none at all.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif( trim($job_description) == '')
	{
		$error=1;
		error("The job description field may not be left blank.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif( trim($skills_required) == '')
	{
		$error=1;
		error("The skills required field may not be left blank.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}

	elseif ( ($job->apply_method == JOB_APP_METHOD_SPECIAL) && ($job->show_special_instr_button == 1) && ( trim($job->special_instr_button) == ''))
	{
		$error=1;
		error("The special Instruction button may not be left blank if you wish to display it.");
		include('job_descriptions/edit_job/edit_job_form.inc');
	}
	
	elseif (($job->admin_status == NO_INTERVIEW_NO_SUITABLE_APPS || $job->admin_status == NO_INTERVIEW_NO_FUNDING || $job->admin_status == NO_INTERVIEW_OTHER) && ($job_has_interview))
	{
		$error = 1;
		$msg = ("This job already has an interview in the system.  By changing the status of this job to: <b class='instruct'>");
		$msg .= (getAdminStatusName($job->admin_status) . "</b>, you will cancel this job's interview.  Are you");
		$msg .= (" sure you wish to do this?");
		confirmNoInterview($msg, $job_id, $job);
	}

	/*
       If they've set no applicable departments, set all departments as applicable.  We want to do this
       here, because if we did it on the previous screen, and they got an error, the page would be reloaded
       with all of the checkboxes filled, and would potentially confuse the user.
     */

    if (!($job->apply_departments))
    {
        $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job->apply_departments[] = $row["department_id"];
        }
    }

	foreach ($job->apply_departments AS $ad)
    {
        $disc_depts .= ("'" . $ad . "',");
    }
    $disc_depts = substr($disc_depts, 0, -1);

    if (!$disc_depts)
    {
        $disc_depts = ("''");
    }

    $sql = ("
            SELECT DISTINCT discipline_id
            FROM department_discipline_join
            WHERE department_id IN (" . $disc_depts . ")
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if (sizeof($job->job_disciplines) == $result->RecordCount())
    {
        $job->job_disciplines = '';
        $job->job_disciplines[] = ALL_DISCIPLINES;
    }


    $error_msg = NULL;
    $job->start_date = NULL;
    $job->end_date = NULL;

    if ($job->start_date_date || $job->start_date_month || $job->start_date_year)
    {
        if (!$job->start_date_month)
        {
            $error_msg .= "When entering a start date for your job, the month field is required. ";
        }
        elseif (!$job->start_date_year)
        {
            $error_msg .= "When entering a start date for your job, the year field is required. ";
        }
        else
        {
            if (!$job->start_date_date)
            {
                $job->start_date_date = "00";
            }
            $job->start_date = $job->start_date_year."-".str_pad($job->start_date_month,2,0,STR_PAD_LEFT)."-".str_pad($job->start_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($job->start_date))
            {
                $error_msg .= "The given start date is not a valid date, please re-enter. ";
            }
        }
    }
    if ($job->end_date_date || $job->end_date_month || $job->end_date_year)
    {
        if (!$job->end_date_month)
        {
            $error_msg .= "When entering an end date for your job, the month field is required. ";
        }
        elseif (!$job->end_date_year)
        {
            $error_msg .= "When entering an end date for your job, the year field is required. ";
        }
        else
        {
            if (!$job->end_date_date)
            {
                $job->end_date_date = "00";
            }

            // The valid date function requires dates with the format YYYY-MM-DD, so it doesn't like dates that are like 2004-4-4, so we have to pad
            // the date to look like 2004-04-04

            $job->end_date = $job->end_date_year."-".str_pad($job->end_date_month,2,0,STR_PAD_LEFT)."-".str_pad($job->end_date_date,2,0,STR_PAD_LEFT);

            if(!isValidDate2($job->end_date))
            {
                $error_msg .= "The given end date is not a valid date, please re-enter. ";
            }
        }
    }

    if ($job->end_date && !$job->start_date)
    {
        $error_msg .= "A start date for the job is required because you have provided an end date.";
    }

    if (!$error_msg)
    {
        if ($job->start_date)
        {
            // If a start date is given, then we will override the start term and year that the user has given, as the start date is more accurate. 
            $arr_term_year = getTermYearWithGracePeriod($job->start_date);
            $job->start_term = $arr_term_year["term"];
            $job->start_year = $arr_term_year["year"];
        }

        // If we have a start date and no end date
        // check what term we're in using start_date + grace period
        // use that term's last day as the end date
        if ($job->start_date && !$job->end_date)
        {
            $job->end_date = getTermYearEndDate($job->start_term,$job->start_year);
            notify("No end date was provided for this job, so the last day of the term (".formatSpecialZeroDate($job->end_date).") will be used as the end date.");
        }

        if (isValidDate2($job->start_date) && isValidDate2($job->end_date) && count_days($job->start_date, $job->end_date) < HISTORY_MIN_DAYS_FOR_WT)
        {
            notify("The start date and end date is less than ".HISTORY_MIN_DAYS_FOR_WT." days apart. The workterm has to be longer.");
        }
    }

    if ($error_msg)
    {
        $error = 1;
        error($error_msg);
		include('job_descriptions/edit_job/edit_job_form.inc');
    }

    // Save changes, and show the user the new inputs

   	if( $error == 0 )
   	{
        include('job_descriptions/edit_job/update_job_info.inc');
        $job = new EditJob('');
        $job->loadValues($job_id);

        $no_buttons = true;
        include('job_descriptions/add_job/confirm_inputs.inc');
	}
}

elseif ($submit == 'confirm_no_interview')
{
	if (isset($job) && is_string($job))
	{
		$job = unpackObject($job);
	}
    

    /*
       If they've set no applicable departments, set all departments as applicable.  We want to do this
       here, because if we did it on the previous screen, and they got an error, the page would be reloaded
       with all of the checkboxes filled, and would potentially confuse the user.
     */

    if (!($job->apply_departments))
    {
        $sql = ("
                SELECT department_id
                FROM department
                WHERE job_list = '1'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job->apply_departments[] = $row["department_id"];
        }
    }

    foreach ($job->apply_departments AS $ad)
    {
        $disc_depts .= ("'" . $ad . "',");
    }

    $disc_depts = substr($disc_depts, 0, -1);
    
    if (!$disc_depts)
    {
        $disc_depts = ("''");
    }

    $sql = ("
            SELECT DISTINCT discipline_id
            FROM department_discipline_join
            WHERE department_id IN (" . $disc_depts . ")
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if (sizeof($job->job_disciplines) == $result->RecordCount())
    {
        $job->job_disciplines = '';
        $job->job_disciplines[] = ALL_DISCIPLINES;
    }

	// Save the changes.

	include('job_descriptions/edit_job/update_job_info.inc');

	$sql = ("
	        SELECT a.job_code, b.emailed_newsgroup
	        FROM job_info AS a, interview AS b
	        WHERE a.job_id='$job_id'
	        AND b.job_id=a.job_id
	        ");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	$row = $result->FetchRow();
	$emailed_newsgroup = $row["emailed_newsgroup"];

	if ($emailed_newsgroup)
	{
		// They've already emailed the newsgroup, so email students notification of interview cancellation.
		$msg = ("You have cancelled the interview for this job.  Please complete the form below to notify shortlisted");
		$msg .= (" students of the cancellation.");
		notify($msg);
		include('job_descriptions/edit_job/mail/send_cancel_interview_mail.inc');
	}
	else
	{
		// Load the newly saved job, and show changes to the user.
	
		$job = new EditJob('');
		$job->loadValues($job_id);
	
		$no_buttons = true;
	
		include('job_descriptions/add_job/confirm_inputs.inc');
	}
}

elseif ($submit == 'send_cancel_email')
{
	// We need to send off the email (they've already filled out the form).

	include('job_descriptions/edit_job/mail/send_cancel_interview_mail.inc');

	$job = new EditJob('');
	$job->loadValues($job_id);

	$no_buttons = true;
	
	include('job_descriptions/add_job/confirm_inputs.inc');
}
	

/*
 function below is used to determine whether or not a user wants to edit a job, and is called
 when they attempt to add a job to the database whose job code is already in use.
*/

function confirmNoInterview( $msg, $job_id, $job )
{
        global $PHP_SELF;
        instruct( $msg .  "
                <table cellspacing=2 cellpadding=2><tr><td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='edit_job' />
                        <input type='hidden' name='job_id' value='" . $job_id . "' />
			<input type='hidden' name='job' value='" . packObject($job) . "' />
			<input type='hidden' name='submit' value='confirm_no_interview' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='edit_job' />
			<input type='hidden' name='job_id' value='" . $job_id . "' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form></td></tr>
                </table>");
}

?>

 
