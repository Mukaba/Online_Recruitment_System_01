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
 | Filename: mail_host_department.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send an email to the new host              |
 | department's primary contact to alert them that they have now been assigned  |
 | as the host department to this job.                                          |
 +------------------------------------------------------------------------------+

*/

// We first need, the new host department, department making the change, new primary contact, and contact making the change.

$dept_making_change = $auth->department;
$sql = ("
	SELECT DISTINCT department_name
	FROM department
	WHERE department_id='" . $auth->department . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$dept_making_change = $row["department_name"];

$contact_making_change = $auth->contact_id;
$contact_names = getContactName($contact_making_change);
$contact_making_change = $contact_names["first_name"] . " " . $contact_names["last_name"];

$new_host_department = $job->host_department;

$sql = ("
	SELECT DISTINCT jobhost_contact_id
	FROM department
	WHERE department_id='" . $new_host_department . "'
	");
$contact_result = $GLOBALS['dbh']->Execute($sql);

while ($contact_row = $contact_result->FetchRow())
{
	$users_contact_id = $contact_row["jobhost_contact_id"];
	$user_email .= getCiEmailUsingID($users_contact_id) . ",";
}
if ($user_email)
{
	$user_email = substr($user_email, 0, -1);
}

// Now get the job info

$sql = ("
	SELECT ec.company_name, ji.position_title
	FROM job_info AS ji, employer_company AS ec
	WHERE job_id='" . $job_id . "'
	AND ji.employer_id=ec.employer_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$to = $user_email;

$subject = "Notification of assignment as a new host department";

$body = ("This e-mail has been sent to notify you that your department has been assigned as the new host department for a job.");
$body .= ("\n\nThe job and employer's information is as follows:");
$body .= ("\n\nPosition Title: " . $row["position_title"]);
$body .= ("\nCompany Name: " . $row["company_name"]);
$body .= ("\n\nYour department was assigned by: " . $contact_making_change . " in department: " . $dept_making_change . ".");

$from = BRAND_NAME_SYSTEM_EMAIL;
$reply_to = $to;		
		
send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to);

function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to)
{

	global $PHP_SELF;
	global $interview;
	global $job_id;
	include_once('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
	$mail->cc = $cc;
	$mail->bcc = $bcc;
	$mail->headers = "Reply-To: " . $reply_to . "\n";
	
	if (!$mail->send())
	{
		//error($mail->error_text." Please push the BACK button on your browser and try again.");
	}	
	unset ($mail);
}
?>
