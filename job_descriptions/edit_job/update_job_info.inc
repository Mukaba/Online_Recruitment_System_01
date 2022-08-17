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
 | Filename: update_job_info.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: Changed job information is saved from the job object into the   |
 | database.                                                                    |
 +------------------------------------------------------------------------------+

*/
 
 //:TODO: Get rid of hardcoded values in this file

// get data from object "job" and set variables

assert( isset($auth) );

//insert data into database

/*
 At this point we're going to need to figure out if we're going to consider a job as being newly added if it is
 posted from a closed/cancelled status, or if it simply updated.  If so, we will use a technique like the one
 shown below to achieve this.  For now, we will simply assume that the job will only ever be UPDATED from here.
*/

// We need to find out the status for this job to determine if we need to set the date_posted or not.

$sql = ("
	SELECT DISTINCT status 
	FROM job_info
	WHERE job_id='" . $job_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$row = $result->FetchRow();
$current_status = $row["status"];
$job_status = $current_status; 

if ($row["status"] != POSTED)
{
	$action = 'NOT_YET_POSTED';
} 
else
{
	$action = 'UPDATED';
}

if ($row["status"] == POSTED || $row["status"] == CLOSED)
{
	if ($job->status == CANCELLED)
	{
		$sql = ("
			UPDATE job_info
			SET student_status='".STUDENT_NOT_HIRING."'
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
}

if ($row["status"] != HOLDING && $job->status == HOLDING)
{
	$begin_holding_period = true;
}

// If the status has been changed, this job can no longer be rejected

(($current_status == $job->status) ? $no_more_rejection = 0 : $no_more_rejection = 1);

$job->workterm_lengths = implode(",", $job->workterm_lengths);

// If the start_term and start_year have changed, we must get a new job code for this job.
$sql = ("
	SELECT term_id, year, job_code
	FROM job_info
	WHERE job_id='" . $job_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
if (!trim($job->job_code))
{
	$job_code = getNextJobCode($job->start_term, $job->start_year);
	$job->job_code = $job_code;
}
elseif ($job->start_term != $row["term_id"] || $job->start_year != $row["year"])
{
	$job_code = getNextJobCode($job->start_term, $job->start_year);
	$job->job_code = $job_code;
	$job_code_has_changed = true;
}
else
{
	$job->job_code = $row["job_code"];
}

$sql = ("
	UPDATE job_info
	SET 
	position_title='" . addslashes($job->position_title) . "',
	job_code='" . addslashes($job->job_code) . "',
	employer_code='" . addslashes($job->employer_code) . "',
	num_positions='" . ($job->num_positions) . "', 
	apply_method='" . ($job->apply_method) . "',
	receive_address='" . addslashes($job->receive_address) . "',
	country_id='" . ($job->country) . "',
	provstate_id='" . ($job->provstate) . "',
	region_id='" . ($job->region) . "',
	city='" . addslashes($job->city) . "',
	salary_amount1='" . addslashes($job->salary_amount1) . "',
	salary_amount2='" . addslashes($job->salary_amount2) . "',
	salary_period='" . ($job->salary_period) . "',
	term_id='" . ($job->start_term) . "',
	year='" . ($job->start_year) . "',
	closing_date='" . addslashes($job->closing_date) . "', 
	closing_time='" . addslashes($job->closing_time) . "', 
	workterm_lengths='" . ($job->workterm_lengths) . "',
	workterm_hours='" . addslashes($job->workterm_hours) . "',
	other_postings='" . addslashes($job->other_postings) . "',
	min_academic='" . ($job->min_academic) . "',
	min_workterms='" . ($job->min_workterms) . "', 
	job_description='" . addslashes($job->job_description) . "', 
	skills_required='" . addslashes($job->skills_required) . "', 
	special_requirements='" . addslashes($job->special_requirements) . "', 
	comments='" . addslashes($job->comments) . "', 
	admin_comments='" . addslashes($job->admin_comments) . "',
	for_student_comments='" . addslashes($job->for_student_comments) . "',
	status='" . ($job->status) . "',
	special_instructions='" . addslashes($job->special_instructions) . "',
	special_instr_button='" . addslashes($job->special_instr_button) . "',
	show_special_instr_button='" . ($job->show_special_instr_button) . "',
	displayname_on_coverltr='" . ($job->displayname_on_coverltr) . "',
	admin_status='" . ($job->admin_status) . "',
    start_date=".($job->start_date ? "'".$job->start_date."'" : "NULL")." ,
    end_date=".($job->end_date ? "'".$job->end_date."'" : "NULL")."
	WHERE job_id='" . ($job_id) . "'
	");
$GLOBALS['dbh']->Execute($sql);

// insert automated action 'Job Description Received' for the employer's contact_id
// this will be viewable in Employer Info -> Set Actions/Flags

//get contact_id to set this action for from job_info 
$mini_sql = ("
    SELECT contact_id
    FROM job_info 
    WHERE job_code='".addslashes($job->job_code)."'
    ");
$mini_result = $GLOBALS['dbh']->Execute($mini_sql);
$mini_row = $mini_result->FetchRow();
$contact_id = $mini_row['contact_id'];

// check that the action has not already been set
$sql = ("
    SELECT * from contact_actions_set
    WHERE contact_id='".$contact_id."'
    AND action_id = '".JOB_DESC_RECEIVED."'
    AND action_note = 'Job Code: ".$job->job_code."'
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
            '".$contact_id."',
            '".$auth->contact_id."',
            '".JOB_DESC_RECEIVED."',
            CURRENT_DATE(),
            'Job Code: ".$job->job_code."'
        )
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
}

if ($no_more_rejection)
{
	$sql = ("
		UPDATE job_info
		SET is_rejectable='0'
		WHERE job_id='" . ($job_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if ($job->admin_status == NO_INTERVIEW_NO_SUITABLE_APPS || $job->admin_status == NO_INTERVIEW_NO_FUNDING || $job->admin_status == NO_INTERVIEW_OTHER)
{
	$sql = ("
		SELECT interview_job_id
		FROM interview_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if (!$result->RecordCount())
	{
		// This is the only job for the interview, cancel the interview.

		$sql = ("
			UPDATE interview
			SET cancelled='1'
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
	else
	{
		/*
		 This is a multi job interview.  Check the status of each job associated
		 with this interview.
		*/

		$row = $result->FetchRow();
		$interview_job_id = $row["interview_job_id"];

		$sql = ("
			SELECT DISTINCT job_id
			FROM interview_job_join
			WHERE interview_job_id='" . $interview_job_id . "'
			AND job_id!='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		$interview_can_be_cancelled = true;

		while ($row = $result->FetchRow())
		{
			$sql = ("
				SELECT DISTINCT admin_status
				FROM job_info
				WHERE job_id='" . $row["job_id"] . "'
				");
			$result2 = $GLOBALS['dbh']->Execute($sql);
			$row2 = $result->FetchRow();
			if ($row2["admin_status"] != '13' && $row2["admin_status"] != '14' && $row2["admin_status"] != '15')
			{
				$interview_can_be_cancelled = false;
			}
		}
		if ($interview_can_be_cancelled)
		{
			$sql = ("
				UPDATE interview
				SET cancelled='1'
				WHERE job_id='" . $interview_job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
}

if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
{

	// Delete all of the old applicable departments from the database, and insert all of the new ones.

	$sql = ("
		DELETE FROM department_job_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);

    //:TODO: Use job object to get things about the job. 
    $sql = ("
        SELECT contact_id, employer_department_id, employer_id, term_id, year
        FROM job_info
        WHERE job_id = '".$job_id."'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

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

    // If the status has been posted, closed, or cancelled, and someone wants to make this job
    // applicable to other departments, we want to give those other departments credit for the
    // contact status. However, we want to credit a department only once, so we scan for existing
    // statuses for the department for this specific job. 
    if ($job_status == POSTED || $job_status == CLOSED || $job_status == CANCELLED) 
    {
        $existing_status_sql = ("
            SELECT department_id
            FROM department_contact_status
            WHERE contact_id = '".$row['contact_id']."' AND job_id = '".$job_id."'
            ");
        $existing_status_result = $GLOBALS['dbh']->Execute($existing_status_sql);

        $existing_status_array = array();
        
        if ($existing_status_result->RecordCount())
        {
            while ($existing_status_row = $existing_status_result->FetchRow())
            {
                $existing_status_array[] = $existing_status_row['department_id'];
            }
        }

        for ($i = 0; $i < sizeof($job->apply_departments);  $i++)
        {
            // If this is a department that hasn't been credited with this contact status, then update the status. 
            if (!in_array($job->apply_departments[$i],$existing_status_array))
            {
                updateContactStatus($row['contact_id'], $job->apply_departments[$i],$job_id, null);
            }
        }
    }
	// Delete all the old disciplines for this job, and insert all the new ones.

	$sql = ("
		DELETE FROM job_discipline_join
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	if ($job->job_disciplines)
	{
		for ($i = 0; $i < sizeof($job->job_disciplines); $i++)
		{
			$sql = ("
				INSERT INTO job_discipline_join
				(discipline_id, job_id)
				VALUES (
				'" . addslashes($job->job_disciplines[$i]) . "',
				'" . $job_id . "'
				)
				");
			$result = $GLOBALS['dbh']->Execute($sql);
		}
	}
	else
	{
		$sql = ("
			INSERT INTO job_discipline_join
			(discipline_id, job_id)
			VALUES (
			'" . ALL_DISCIPLINES . "',
			'" . $job_id . "'
			)
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
}

// If the job's status has been changed to the 24 hour hold, we need to set the begin_hold_time.

if ($begin_holding_period)
{
	// Determine when this job leaves the holding pattern.

	$sql = ("
		SELECT NOW()
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$hold_begin_datetime = $row["NOW()"];

	$date_time = split(" ", $hold_begin_datetime);
	$date = split("-", $date_time[0]);
	$time = split(":", $date_time[1]);
	$timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);

	$date_for_week = getdate($timestamp);
	$day_of_week = $date_for_week["wday"];

	if ($day_of_week == 5)
	{
		// If today is Friday, the job closes 72 hours from now.

		$date[2] += 3;

		$end_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$hold_end_datetime = date("Y-m-d H:i:s", $end_timestamp);
	}
	elseif ($day_of_week == 6 || $day_of_week == 0)
	{
		// Today is Sat or Sun, so we need to set the time to end at 11:59:59 Mon.

		if ($day_of_week == 6)
		{
			$date[2] += 2;
		}
		elseif ($day_of_week == 0)
		{
			$date[2] += 1;
		}

		$mon_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$mon_date = date("Y-m-d", $mon_timestamp);

		$hold_end_datetime = $mon_date . " 23:59:59";
	}
	else
	{
		$date[2] += 1;
		$end_timestamp = mktime($time[0], $time[1], $time[2], $date[1], $date[2], $date[0]);
		$hold_end_datetime = date("Y-m-d H:i:s", $end_timestamp);
	}

	$sql = ("
		UPDATE job_info
		SET hold_begin_datetime=NOW(),
		hold_end_datetime='" . $hold_end_datetime . "'
		WHERE job_id='" . $job_id . "'
		");
	$GLOBALS['dbh']->Execute($sql);
}

// If the job is being POSTED, then set the date_posted field, otherwise set last_updated_on and last_updated_by.

if ($action == 'NOT_YET_POSTED' && $job->status == POSTED)
{
    $sql = ("
		UPDATE job_info
		SET 
		date_posted=CURRENT_DATE, 
		entered_by='" . addslashes($auth->contact_id) . "',
		time_added=CURRENT_TIME,
		student_status='1'
		WHERE job_id='" . $job_id . "'
		");
    $GLOBALS['dbh']->Execute($sql);
} elseif ($action == 'UPDATED')
{
	$GLOBALS['dbh']->Execute("
		UPDATE job_info
		SET 
		last_updated_on=CURRENT_DATE,
		last_updated_time=CURRENT_TIME,
		last_updated_by='" . addslashes($auth->login) . "'
		WHERE job_id='" . $job_id . "'
		");
}
unset($action);

if ($job->status == CLOSED)
{
	$sql = ("
		SELECT DISTINCT student_status
		FROM job_info
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	if ($row["student_status"] == 1)
	{
		$sql = ("
			UPDATE job_info
			SET student_status='2'
			WHERE job_id='" . $job_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	}
}

// Update the student status for this job.

switch ($job->admin_status)
{
	case "11" :
		$new_student_status = '1';
		break;
	case "12":
		$new_student_status = '2';
		break;
	case "13":
	case "14":
	case "15":
		$new_student_status = '7';
		break;
	case "16":
		$new_student_status = '3';
		break;
	case "17":
		$new_student_status = '4';
		break;
	case "19":
		$new_student_status = '5';
		break;
	case "20":
		$new_student_status = '6';
		break;
	case "21":
	case "22":
	case "23":
		$new_student_status = '7';
		break;
}

if ($new_student_status)
{
	$sql = ("
		UPDATE job_info
		SET student_status='" . $new_student_status . "'
		WHERE job_id='" . $job_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}
		

if ($job->job_code && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR))
{
	$msg .= ("All changes for " . $job->job_code . " have been successfully saved.");
	if ($job_code_has_changed)
	{
		$msg .= ("<br /><br />Please note: Since you have changed the starting term and year for this job, the job code ");
		$msg .= ("has been changed to <b>" . $job->job_code . "</b>");
	}
	notify($msg);
}
else
{
	notify("All changes have been successfully saved.");
}

echo("<hr />");





//delete object "job"
unset($job);

?>
