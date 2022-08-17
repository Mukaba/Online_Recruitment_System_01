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
 | Filename: holding_jobs.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for determining what the user is       |
 | trying to do, checking input, and loading the appropriate includes.          |
 +------------------------------------------------------------------------------+

*/

// Include the header first
include('job_descriptions/holding_jobs/holding_jobs_header.inc');
include_once('job_descriptions/edit_job/EditJob.class');
include_once('contact/edit/EditContact.class');

// Function used for sending e-mails.
if (!function_exists("send_email"))
{
    function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to)
    {

        global $PHP_SELF;
        global $job_id;
        include_once('misc/email.inc');
        
        $mail = new email($to, $subject, unslash($body));
            
        $mail->from = $from;
        $mail->cc = $cc;
        $mail->bcc = $bcc;
        $mail->headers = "Reply-To: " . $reply_to . "\n";
        
        if (!$mail->send())
        {
            $logMsg = ("Error encountered when attempting to send automatic e-mail to notify of a department singing themselves up as applicable to a job.");
            $logMsg .= ("Error text below:\n\n");
            $logMsg .= ($mail->error_text);
            dumpToLog($logMsg);
        }	
        unset ($mail);
    }
}

if (!$continue)
{
	// They just got here.  Load up the appropriate holding_jobs_list for them.

	if ($userlevel == TRAFFICDIRECTOR)
	{
		include('job_descriptions/holding_jobs/traffic_holding_jobs_list.inc');
	}
	elseif ($userlevel == OFFICE)
	{
		include('job_descriptions/holding_jobs/office_holding_jobs_list.inc');
	}
}

elseif ($continue == 'apply_unapply_office_multiple') 
{
	// The office wants to make themselves applicable/unapplicable to each selected job.

	// Trim the apply_to_job and unapply_to_job arrays, and fix the total_job_ids array

	$total_job_ids = unpackObject(($total_job_ids));
	$apply_to_job = trim_array($apply_to_job);
    $unapply_to_job = trim_array($unapply_to_job);

    // Before we do anything, we need to get a list of all of the jobs that this department previously had applicable to them.
    // We need to then compare this list to the list of all jobs that are in the $apply_to_job array.  For any job that has newly
    // been made applicable to this department, we need to update that job's contact's status.
    $sql = ("
        SELECT DISTINCT job_id
        FROM department_job_join
        WHERE department_id='" . $auth->department . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $old_applicable_jobs[] = $row["job_id"];
    }

	// First unapply this department for all jobs they have checked that they were previously applied to.
    if (is_array($unapply_to_job))
    {
        foreach($unapply_to_job AS $utj)
        {
            $sql = ("
                DELETE FROM department_job_join
                WHERE job_id='" . $utj . "'
                AND department_id='" . $auth->department . "'
                ");
            $GLOBALS['dbh']->Execute($sql);
        }
    }

	if (is_array($apply_to_job))
	{
		foreach($apply_to_job AS $atb)
		{
			$sql = ("
				INSERT INTO department_job_join
				(job_id, department_id)
				VALUES (
				'" . $atb . "',
				'" . $auth->department . "'
				)
				");
			$GLOBALS['dbh']->Execute($sql);

			// Mail the host department for each job to alert them of a recently applied department.
			$applied_department = $auth->department;
			$applied_contact = $auth->contact_id;
			$applied_job = $atb;

			include('job_descriptions/holding_jobs/mail_applied_department.inc');
		}
	}	

	$msg = ("Your department has been successfully applied/unapplied to each of the selected jobs.");
	$msg .= ("  For each of the jobs your department has been applied to, the host department has been");
	$msg .= (" notified via e-mail.");
	notify($msg);

	include('job_descriptions/holding_jobs/office_holding_jobs_list.inc');
}

elseif ($continue == 'apply_traffic_multiple')
{
	// The traffic director wants to make the chosen department applicable to each selected job.

	// Fix the total_job_ids array, and trim the apply_to_job

	$total_job_ids = unpackObject(($total_job_ids));
	$apply_to_job = trim_array($apply_to_job);

    // Before we do anything, we need to get a list of all of the jobs that this department previously had applicable to them.
    // We need to then compare this list to the list of all jobs that are in the $apply_to_job array.  For any job that has newly
    // been made applicable to this department, we need to update that job's contact's status.
    $sql = ("
        SELECT DISTINCT job_id
        FROM department_job_join
        WHERE department_id='" . $apply_this_department . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $old_applicable_jobs[] = $row["job_id"];
    }

	// First unapply the selected department for all existing jobs, then apply for the checked ones.
	foreach($total_job_ids AS $tji)
	{
		$sql = ("
			DELETE FROM department_job_join
			WHERE job_id='" . $tji . "'
			AND department_id='" . $apply_this_department . "'
			");
		$GLOBALS['dbh']->Execute($sql);
	}

	if (is_array($apply_to_job))
	{
		foreach($apply_to_job AS $atb)
		{
			$sql = ("
				INSERT INTO department_job_join
				(job_id, department_id)
				VALUES (
				'" . $atb . "',
				'" . $apply_this_department . "'
				)
				");
			$GLOBALS['dbh']->Execute($sql);
		}
	}
        
	$msg = ("<b>" . getDepartmentName($apply_this_department) . "</b> has been successfully applied to each of the selected jobs, ");
	$msg .= ("and unapplied from the unselected jobs.");
	notify($msg);

	include('job_descriptions/holding_jobs/traffic_holding_jobs_list.inc');
}

elseif ($continue == 'view_specifics_of_job')
{
	// The user wants to see the specifics of a given job.

	if ($userlevel == TRAFFICDIRECTOR)
	{
		include('job_descriptions/holding_jobs/traffic_job_specifics.inc');
	}
	elseif ($userlevel == OFFICE)
	{
		include('job_descriptions/holding_jobs/office_job_specifics.inc');
	}
}

elseif ($continue == 'view_company_specifics')
{
	// The user wants to see the specifics of a given company.

	include('job_descriptions/holding_jobs/view_company_specifics.inc');
}

elseif ($continue == 'apply_to_single_office')
{
	// The office wants to make themselves applicable to the specific job.

	$sql = ("
		INSERT INTO department_job_join
		(job_id, department_id)
		VALUES (
		'" . $job_id . "',
		'" . $auth->department . "'
		)
		");
	$GLOBALS['dbh']->Execute($sql);

	// Mail the host department that this department has made themselves applicable.

	$applied_department = $auth->department;
	$applied_contact = $auth->contact_id;
	$applied_job = $job_id;

	include('job_descriptions/holding_jobs/mail_applied_department.inc');

	notify("Your department has been successfully applied to the job.");
	
	include('job_descriptions/holding_jobs/office_holding_jobs_list.inc');
}

elseif ($continue == 'unapply_from_single_office')
{
	// The office wants to unapply their department from this job.

	$sql = ("
		DELETE FROM department_job_join
		WHERE job_id='" . $job_id . "'
		AND department_id='" . $auth->department . "'
		");
	$GLOBALS['dbh']->Execute($sql);

	notify("Your department has been successfully unapplied from this job.");

	include('job_descriptions/holding_jobs/office_holding_jobs_list.inc');
}

elseif ($continue == 'apply_traffic_single')
{
	// The traffic director wants to make the chosen departments applicable to the specific job.
	$apply_department = trim_array($apply_department);

    // Before we do anything, get a list of all of the departments that are applied for this job
    $sql = ("
        SELECT DISTINCT department_id
        FROM department_job_join
        WHERE job_id='" . $job_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $old_department_ids[] = $row["department_id"];
    }

    // Delete the old departments that are applicable to the job.
	$sql = ("
		DELETE FROM department_job_join
		WHERE job_id='" . $job_id . "'
		");
	$GLOBALS['dbh']->Execute($sql);

    // Insert all of the marked departments that are applicable to the job.
	if (is_array($apply_department))
	{
		foreach($apply_department AS $ad)
		{ 
			$sql = ("
				INSERT INTO department_job_join
				(job_id, department_id)
				VALUES (
				'" . $job_id . "',
				'" . $ad . "'
				)
				");
			$GLOBALS['dbh']->Execute($sql);
		}
	}

	notify("The selected departments have been applied to this job, and the unselected departments have been unapplied.");

	include('job_descriptions/holding_jobs/traffic_holding_jobs_list.inc');
}

?>
