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
 | Filename: send_interview_notify.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: Set up and sends the notification.                              |
 +------------------------------------------------------------------------------+

*/

// the file used to send e-mail
include ('applications/email/send_email.inc');

$job_code = getJobCode ($job_id);

// first find out who we're e-mailing this to (from department table - it's a department pref thing)
$sql = "SELECT d.shortlist_contact_id
	FROM department AS d, job_info AS ji
	WHERE d.department_id = ji.department_id
	AND ji.job_id = '$job_id'";
$result = $GLOBALS['dbh']->Execute ($sql);
$row = $result->FetchRow();
$send_to = getCiEmailUsingID ($row["shortlist_contact_id"]);

$sql = "SELECT phone FROM contact WHERE contact_id = '$contact_id'";
$result = $GLOBALS['dbh']->Execute ($sql);
$row = $result->FetchRow();
$phone = $row["phone"];

if ($send_self)
	$send_cc = getCiEmailUsingID ($contact_id);
else
	$send_cc = "";

$shortlist_numbers = unpackObject(($shortlist_numbers));

$contact_name = implode (getContactName ($contact_id), " ");
$body .= "$contact_name has added the following student(s) to the shortlist for $job_code:\n";
foreach ($shortlist_numbers as $sln)
	$body .= getStudentName ($sln) .", ";
	
$body .= "\n\n";
if ((strlen ($mail_body) > 2) && ($send_with_prefs)) {
	$body .= "The following additional information has also been provided:\n";
	$body .= "--------------------------------------------------\n";
	$body .= $mail_body ."\n";
	$body .= "--------------------------------------------------\n\n";
}
elseif ($send_interview_prefs)
	$body .= "No additional information has been provided.\n\n";
$body .= "Please contact $contact_name". ($phone ? " ($phone) " : "") ." to confirm this interview.";

$subject = "Student Shortlist for $job_code";
// WILL NEED TO CHANGE
$from = (getCiEmailUsingID ($contact_id) ? getCiEmailUsingID ($contact_id) : NULL_EMAIL);

if (send_email_employer ($job_id, $send_to, $send_cc, $send_bcc, $subject, $body, $from, $reply_to)) {
	// notificatons, only if form is filled out
	if ($send_interview_prefs)
		notify ("Information has been successfully sent to the Co-op Office. Thank you!");

	// update the flag, only if they sent prefs
	if ($send_interview_prefs)
		$GLOBALS['dbh']->Execute ("UPDATE job_info set interview_notify_sent = 1 WHERE job_id = '$job_id'");
	// go back to viewing applicants
	include ('applications/employer/applicant_list.inc');
}


?>
