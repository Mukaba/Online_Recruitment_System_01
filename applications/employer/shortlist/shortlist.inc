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
 | Filename: shortlist.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file deals mainly with the employer side for               |
 | shortlisting students. Altough it could probably be placed in it's own       |
 | file, it's placed here because it's accessed from within the applications    |
 | side of things. It's called from apply_employer.inc Although it could be     |
 | kept there to ensure flow of the code, it was placed seperatly to ensure     |
 | modularity                                                                   |
 +------------------------------------------------------------------------------+

*/


// students chosen, but make sure they have applicants selected then let's confirm 
if ($shortlist_submit) {

	if (($shortlist_who == 'selected') && (sizeof ($selected_app_id) < 1 )) {
		error ("No student applications were selected to shortlist. Please ensure you have at least 1 student checked.");
		include ('applications/employer/applicant_list.inc');
	}
	else
		include ('applications/employer/shortlist/confirm_shortlist.inc');
}


// we're gonna shortlist some students!! lucky them
elseif ($shortlist_confirmed) {
	
	$stud_nums = unpackObject(($shortlist_numbers));

	foreach ($stud_nums as $sn) {

		if (!isStudentShortlisted ($job_id, $sn)) {

			$ins = "INSERT INTO students_shortlisted
				(student_number, job_id)
				VALUES ('$sn', '$job_id')";
			
			if ($GLOBALS['dbh']->Execute ($ins))
				$success .= getStudentName ($sn) .", ";
			else
				$unsuccess .= getStudentName ($sn) .", ";
		}
		
		else
			$already .= getStudentName ($sn) .", ";

	}

	if ($unsuccess)
		error ("There were problems adding the folling students: <br />
			$unsuccess <br />
			Please try shortlisting them again.");
	if ($success)
		notify ("The following students were successfully added to the shortlist: <br />
			$success");

	if ($already)
		notify ("The following students have already been shortlisted, they do not need to be added again:<br />
			$already");

	// what to do next?
	// if they said no, and there's been some successful ones, send a notify to the office
	if (($send_notify == 'no') && ($success)) {
		include ('applications/employer/shortlist/send_interview_notify.inc');
	}
	// if they said no, but they're all either unsuccessful or already added, dont bother
	elseif (($send_notify == 'no') && (!$success) && ($unsuccess || $already)) {
		include ('applications/employer/applicant_list.inc');
	}
}


// if we've set it above we're gonna display a form to send re: prefs for interviewing; although it's agreed that this
// could be placed in the applications/email directory it's placed here cause it doesn't look like an actual e-mail 
// from the user's point of view
if (($send_notify == 'yes') || ($email_submit && ($email_who == 'interview_notify'))) {
	include ('applications/employer/shortlist/interview_notify.inc');
}

// sets up and e-mails the actual stuff
elseif ($send_interview_prefs) {
	include ('applications/employer/shortlist/send_interview_notify.inc');
}


?>
