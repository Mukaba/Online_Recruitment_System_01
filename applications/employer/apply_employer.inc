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
 | Filename: apply_employer.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file displays all of the current applications for          |
 | students to jobs that this employer has submitted.                           |
 +------------------------------------------------------------------------------+

*/

// most of the ones below will check if the user is an employer (since admin can log in as an employer)

// see if flagging or unflagging, then keep going
if ($flag_unflag) {

	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
		for ($i = 0; $i < sizeof($selected_app_id); $i ++) {

			if (is_app_flagged ($selected_app_id[$i], $contact_id)) {
				// get rid of any that are checked
				$sql = "DELETE FROM employer_apps_flagged
					WHERE contact_id = '$contact_id'
					AND application_id = '". $selected_app_id[$i] ."'";
				$GLOBALS['dbh']->Execute ($sql);
			}

			else {
				// now insert any
				$sql = "INSERT INTO employer_apps_flagged
					(application_id, contact_id)
					VALUES ('". $selected_app_id[$i] ."', '$contact_id')";
				$GLOBALS['dbh']->Execute ($sql);
			}
		} // for loop
		include ('applications/employer/applicant_list.inc');
	}

	else {
		$admin_is_emp_error = true;
    }

}


// we're gonna shortlist some students!! lucky them
// it's placed in it's own file to increase modularity
elseif (!$cancel_confirm && ($shortlist_submit || $shortlist || ($email_submit && ($email_who == 'interview_notify'))))  {

	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
		include ('applications/employer/shortlist/shortlist.inc');
    }
	else {
		$admin_is_emp_error = true;
    }
}

// e-mail somebody, but set it up first
elseif ($email_submit) {

	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
	
		// make sure we're good to go
		switch ($email_who) {
		
			case 'selected':
				if (sizeof ($selected_app_id) > 0) {
					$has_addresses = true;
                }
				break;

			case 'flagged':
				$sql = "SELECT DISTINCT a.application_id, a.student_number
					FROM applications AS a, employer_apps_flagged AS eaf
					WHERE a.application_id = eaf.application_id
					AND eaf.contact_id = '". ($userlevel == ADMINEMPLOYER ? $employer_contact_id_login : $contact_id) ."'
					AND a.job_id = '$job_id'";
				$result = $GLOBALS['dbh']->Execute ($sql);

				if ($result->RecordCount() > 0) {
					$has_addresses = true;
                }
				break;

			case 'shortlisted':
				$sql = "SELECT DISTINCT a.application_id, a.student_number
					FROM applications AS a, students_shortlisted AS ss
					WHERE a.student_number = ss.student_number
					AND a.job_id = '$job_id'
					AND ss.job_id = '$job_id'";
				$result = $GLOBALS['dbh']->Execute ($sql);

				if ($result->RecordCount() > 0) {
					$has_addresses = true;
                }
				break;

			case 'all':
			default:
				$sql = "SELECT DISTINCT application_id, student_number
					FROM applications
					WHERE application_id IN ('". implode ( unpackObject(($all_apps)), "','") ."')";
				$result = $GLOBALS['dbh']->Execute ($sql);

				if ($result->RecordCount() > 0) {
					$has_addresses = true;
                }
		}

		// check if everything's good to go
		if ($has_addresses) {
			$send_students = true;
			include ('applications/email/email.inc');
		}
		else {
			error ("No students have been selected to e-mail.");
			include ('applications/employer/applicant_list.inc');
		}
	}

	else {
		$admin_is_emp_error = true;
    }
}

// actually send it
elseif ($email_step == 'Send') { 

	if (($userlevel == EMPLOYER) || ($userlevel == HREMPLOYER)) {
		include ('applications/email/email.inc');
    }
	else {
		$admin_is_emp_error = true;
    }
}


// keep going, or we cancelled the confirmation of shortlisting
elseif (($continue == 'view_applicants') || $cancel_confirm) {
	// They've chosen a job, display the list of applicants in their department for this job.
	//  Make sure that this job belongs to them.
	assert($job_id);

	include('applications/employer/applicant_list.inc');
}

// problem, or just got here
else {
	include ('applications/employer/job_list.inc');
}


// when an admin is signed in as an employer and they do something they're not allowed to
if ($admin_is_emp_error) {

	error ("You are not allowed to perform this operation while signed in as an employer.");
	include ('applications/employer/applicant_list.inc');
}

?>
