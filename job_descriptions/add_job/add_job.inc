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
 | Filename: add_job.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: job information is moved from the object "job" to the joblist   |
 | database.                                                                    |
 +------------------------------------------------------------------------------+

*/
 

// get data from object "job" and set variables

assert( isset($auth) );

/*
 If there is already a job in the database with this job code for the chosen term, ask user if they
 would like to edit it (this will only happen if they've hit the refresh button).
*/

if ($job->job_code != '')
{
	$sql = ("
		SELECT DISTINCT job_id, status
		FROM job_info
		WHERE year='" . $job->start_year . "'
		AND term_id='" . $job->start_term . "'
		AND job_code='" . $job->job_code . "'
		");
	
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{       
		$error = 1;
		$row = $result->FetchRow();
		$msg = ("There is already a job with job code: <b>" . $job->job_code . "</b> in the system for  ");
		$msg .= (getTermName($job->start_term) . ", " . $job->start_year . ".  ");
		if ($row["status"] == CANCELLED)
		{
			$msg .= ("This job has been cancelled.  ");
		}
		$msg .= ("<br /><br />Would you like to edit it?<br /><br />");
	
		promptEdit($msg, $row["job_id"]);
	}
}
elseif (!$add_confirmed)
{
	/*
	 This is the case in which an employer is entering a job, and thus there is no job code.
	 Check and see if there is a job with the same position title, term, year,  and the same 
	 employer_id in the database and if there is, make sure that they want to still add this job.
	*/

	$sql = ("
		SELECT DISTINCT job_id, status
		FROM job_info
		WHERE position_title='" . addslashes($job->position_title) . "'
		AND employer_id='" . $job->employer_id . "'
		AND contact_id='" . $job->contact_id . "'
		AND year='" . $job->start_year . "'
		AND term_id='" . $job->start_term . "'
		");
	if ($job->department_id != 'none')
	{
		$sql .= ("
			AND employer_department_id='" . $job->department_id . "'
			");
	}

	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount())
	{
		$error = 1;
		if ($job->department_id != 'none')
		{
			$msg = ($job->department_name . " already has a job registered in the system named " . $job->position_title);
		}
		else
		{
			$msg = ($job->company_name . " already has a job registered in the system named " . $job->position_title);
		}
		$msg .= (" for " . getTermName($job->start_term) . ", " . $job->start_year . ". ");
		$msg .= ("<br /><br />Are you sure you want to add this job?<br /><br />");

		promptAdd($msg, $job);
	}
}

if ($error == 0)
{
	(($userlevel == EMPLOYER || $userlevel == HREMPLOYER) ? $is_rejectable = 1 : $is_rejectable = 0);

	//insert data into database
			
	$job->workterm_lengths = implode(',', $job->workterm_lengths);

	// If there is no division, then department_id will equal 'none'.  Change this to nothing.

	if ($job->department_id == 'none')
	{
		$job->department_id = '';
	}

	if ($job->status == POSTED)
	{
		$time_added = getCurrentTime();
	}
	else
	{
		$time_added = '';
	}

	if ($job->start_term && $job->start_year)
	{
		$job_code = getNextJobCode($job->start_term, $job->start_year);
		$job->job_code = $job_code;
	}

    $temp_date_entered = getTempDateEntered($job->start_term,$job->start_year);
	
	$sql = ("
		INSERT INTO job_info(
		position_title, job_code, employer_code,
		num_positions, apply_method, 
		receive_address, country_id, 
		provstate_id, region_id, city,
		salary_amount1, salary_amount2,
		salary_period, term_id, year, 
		closing_date, closing_time, 
		workterm_lengths, workterm_hours, other_postings, 
		min_academic, min_workterms, department_id,
		job_description, skills_required, 
		special_requirements, comments, admin_comments, for_student_comments,
		admin_status, status, student_status, date_posted, entered_by,
		contact_id, employer_id, employer_department_id,
		is_rejectable, time_added, special_instructions,
		special_instr_button, show_special_instr_button, displayname_on_coverltr,
		date_entered, temp_date_entered, start_date, end_date
		)
		VALUES (
		'" . addslashes($job->position_title) . "',
		'" . addslashes(trim($job_code)) . "',
		'" . addslashes($job->employer_code) . "',
		'" . ($job->num_positions) . "',
		'" . ($job->apply_method) . "',
		'" . addslashes($job->receive_address) . "',
		'" . ($job->country) . "',
		'" . ($job->provstate) . "',
		'" . ($job->region) . "',
		'" . addslashes($job->city) . "',
		'" . addslashes($job->salary_amount1) . "',
		'" . addslashes($job->salary_amount2) . "',
		'" . ($job->salary_period) . "',
		'" . ($job->start_term) . "',
		'" . ($job->start_year) . "',
		'" . addslashes($job->closing_date) . "',
		'" . addslashes($job->closing_time) . "',
		'" . ($job->workterm_lengths) . "',
		'" . addslashes($job->workterm_hours) . "',
		'" . addslashes($job->other_postings) . "',
		'" . ($job->min_academic) . "',
		'" . ($job->min_workterms) . "',
		'" . ($job->host_department) . "',
		'" . addslashes($job->job_description) . "',
		'" . addslashes($job->skills_required) . "',
		'" . addslashes($job->special_requirements) . "',
		'" . addslashes($job->comments) . "',
		'" . addslashes($job->admin_comments) . "',
		'" . addslashes($job->for_student_comments) . "',
		'" . addslashes($job->admin_status) . "',
		'" . ($job->status) . "',
		'" . addslashes($job->student_status) . "',
		'" . addslashes($job->date_posted) . "',
		'" . addslashes($auth->contact_id) . "',
		'" . ($job->contact_id) . "',
		'" . ($job->employer_id) . "',
		'" . ($job->department_id) . "',
		'" . ($is_rejectable) . "',
		'" . ($time_added) . "',
		'" . addslashes($job->special_instructions) . "',
		'" . addslashes($job->special_instr_button) . "',
		'" . ($job->show_special_instr_button). "',
		'" . ($job->displayname_on_coverltr). "',
		CURRENT_DATE(),
        '" . $temp_date_entered . "',
        ".($job->start_date ? "'".$job->start_date."'" : "NULL").",
        ".($job->end_date ? "'".$job->end_date."'" : "NULL")."
		)
		");
	
	$GLOBALS['dbh']->Execute($sql);

	// Get the job_id of the newly entered job.
	$sql = ("
		SELECT LAST_INSERT_ID() AS job_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	$job_id = $row["job_id"];

    // insert automated action 'Job Description Received' for the employer's contact_id 
    // this will be viewable in Employer Info -> Set Actions/Flags
    $sql = ("   
        SELECT * 
        FROM contact_actions_set
        WHERE contact_id='".$job->contact_id."'
        AND action_id = '".JOB_DESC_RECEIVED."'
        AND action_note = 'Job Code: ".$job_code."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    
    if(!$result->RecordCount())
    {
        $sql = ("
            INSERT INTO contact_actions_set(
                contact_id,
                action_by,
                action_id,
                action_on,
                action_note
            )VALUES (
                '".$job->contact_id."',
                '".$auth->contact_id."',
                '".JOB_DESC_RECEIVED."',
                CURRENT_DATE(),
                'Job Code: ".$job_code."'
            )
            ");
        $result = $GLOBALS['dbh']->Execute($sql); 
    }

	// Save the departments that can currently apply to this job into the join table.  As well, update the status of the contact for this job for each
    // department that has been listed as applicable.

	if ($job_id)
	{
		for ($i = 0; $i < sizeof($job->apply_departments); $i++)
		{
			$sql = ("
				INSERT INTO department_job_join(
				department_id, job_id
				)
				VALUES (
				'" . addslashes($job->apply_departments[$i]) . "',
				'" . $job_id . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}

	// Save the disciplines that apply to this job (if any) in the discipline_job_join table.

	if ($job_id && $job->job_disciplines)
	{
		foreach ($job->job_disciplines AS $jd)
		{
			$sql = ("
				INSERT INTO job_discipline_join(
				job_id, discipline_id)
				VALUES (
				'" . $job_id . "',
				'" . $jd . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	elseif ($job_id)
	{
		$sql = ("
			INSERT INTO job_discipline_join
			(job_id, discipline_id)
			VALUES (
			'" . $job_id . "',
			'" . ALL_DISCIPLINES . "'
			)
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}

	// If this job's status has been set to the holding pattern, set the time at which it entered the holding pattern.

	if ($job->status == HOLDING)
	{
		$hold_end_datetime = getHoldEndTime();

		$sql = ("
			UPDATE job_info
			SET hold_begin_datetime=NOW(),
			hold_end_datetime='" . $hold_end_datetime . "'
			WHERE job_id='" . $job_id . "'
			");
		$GLOBALS['dbh']->Execute($sql);
	}
    
    // Update the status for this contact, and its company and divisions.  We want to do this ONLY for the departments that this job is applicable to.
    if ($job->admin_status == POSTED_VIEWABLE)
    {
        for ($i = 0; $i < sizeof($job->apply_departments); $i++)
        {
            updateContactStatus($job->contact_id, $job->apply_departments[$i], $job_id, null);
        }
    }
	
	notify("Job: " . $job->position_title . " has been successfully added.");
	echo("<hr />");
	$no_buttons = true;

	include('job_descriptions/add_job/confirm_inputs.inc');

	//delete object "job"
	unset($job);
}

function promptAdd( $msg, $job )
{
        global $PHP_SELF;
        error( $msg .  "
                <table cellspacing=2 cellpadding=2><tr><td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='add_job' />
			<input type='hidden' name='add_confirmed' value='true' />
			<input type='hidden' name='submit' value='Add job now' />
                        <input type='hidden' name='job' value='" . packObject($job) . "' />
			<input type='hidden' name='continue' value='add_job_confirmed' />
                        <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
                </form>
                </td>
                <td>
                <form method='post' action='$PHP_SELF'>
                        <input type='hidden' name='select' value='' />
                        <input type='hidden' name='menu_select' value='' />
                        <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
                </form></td></tr>
                </table>");
}
?>
