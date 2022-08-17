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
 | Filename: send_secretary_email.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is called by by students/interview.inc when a         |
 | student either removes himself from the interview sign-up list, or           |
 | adds/removes his phone number from the interview sign-up list, IF the        |
 | employer e-mail has already been sent.                                       |
 +------------------------------------------------------------------------------+

*/

	global $auth;
	global $interview;

        /*
         Get the contact for this interview's id, and use this to find their e-mail.
        */

	$contact_result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT contact
		FROM interview AS b
		WHERE job_id='" . $job_id . "'
		");
	$contact_row = $contact_result->FetchRow();

	$users_contact_id = $contact_row["contact_id"];
        $user_email = getCiEmailUsingID($users_contact_id);

	$to = $user_email;

	$student_result = $GLOBALS['dbh']->Execute("
		SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS name, student_number
		FROM student
		WHERE login_id='" . addslashes($auth->login) . "'
		");
	$student_row = $student_result->FetchRow();

	$student_name = $student_row["name"];
	$student_number = $student_row["student_number"];

	$subject = "Notification of schedule change";

	global $job_id;
	$result = $GLOBALS['dbh']->Execute("
		SELECT job_code
		FROM job_info
		WHERE job_id='" . addslashes($job_id) . "' 
		");

	$row = $result->FetchRow();


	$body = "This e-mail has been sent to notify you that a change has been made to an interview for " . $row["job_code"] . ".  ";

	switch ($reason)
	{
	case "remove student":
		 $body .= $student_name . " (" . $student_number . ") has been removed from an interview time slot at " . $time . ", " . formatLongDate($date) . ".";
		break;
	case "remove phone-number":
		$body .= $student_name . " (" . $student_number . ") has removed his/her phone number from the interview at " . $time . ", " . formatLongDate($date) . ".";
		break;
	case "add phone-number":
		$body .= $student_name . " (" . $student_number . ") has added/modified a phone number to the interview at " . $time . ", " . formatLongDate($date) . ".";
		break;
	}	
				
	$from = BRAND_NAME_SYSTEM_EMAIL;
	$reply_to = $to;		
			
	send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to);


function send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to)
{

	global $PHP_SELF;
	global $interview;
	global $job_id;
	include('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;
	$mail->cc = $cc;
	$mail->bcc = $bcc;
	$mail->headers = "Reply-To: " . $reply_to . "\n";
	
	if (!$mail->send())
	{
		error($mail->error_text." Please push the BACK button on your browser and try again.");
	}	
	unset ($mail);
}

?>
