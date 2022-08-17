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
 | Filename: send_lp_mail.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file sends off the login and password e-mail to the        |
 | employer.                                                                    |
 +------------------------------------------------------------------------------+

*/


	$sql = ("
		SELECT DISTINCT email
		FROM contact
		WHERE contact_id='" . $contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$to = $row["email"];

    $subject = ORGANIZATION_CODE." Co-op login ID and password";

	$body = ("This e-mail has been sent automatically to notify you of your new login ID ");
	$body .= ("and password to access the ".ORGANIZATION_CODE." Co-op Database System.");

	if ($old_info_existent)
	{
		$body .= ("\n\n");
		$body .= ("The login ID that you had previously is still the same, but you now need to ");
		$body .= ("log in with the new password shown below.  You have been given a new password ");
		$body .= ("because either you or an administrator has requested one. ");
	}
	$body .= ("\n\n");
	$body .= ("Login ID: " . $loginID . "\n");
	$body .= ("Password: " . $password . "\n\n");
    $body .= ("Your ".BRAND_NAME." UserID and Password are case-specific, so please type them in exactly as they are (e.g. use a capital letter where there is a capital).\n\n");
    $body .= ("We suggest that once you log in, you select Change Password from the menu at left, and change your password to something that you can easily remember.\n\n");
    $body .= ("Keep your loginID and password in a safe place. Anyone with these two pieces of information is able to log in to the system and post jobs, view applicants, etc. as you.\n\n");
    $body .= ("If you have any difficulties logging on please get in touch with your regular co-op program contact, or contact our Employer Service Coordinator at employer@uvic.ca.\n\n");
	$body .= ("To get to the Co-op database system, follow this link: ".ORGANIZATION_DB_URL."\n");

	$from = BRAND_NAME_SYSTEM_EMAIL;
	
	// Who do we make the reply_to point to?

	$reply_to = $to;		
			
	send_email($to, $subject, $body, $from, $reply_to, $come_from_apps, $job_id);


function send_email($to, $subject, $body, $from, $reply_to, $come_from_apps, $job_id)
{

	global $PHP_SELF;
	include('misc/email.inc');
	
	$mail = new email($to, $subject, unslash($body));
		
	$mail->from = $from;

	$mail->headers = "Reply-To: " . $reply_to . "\n";
	
	if (!$mail->send())
	{
		error($mail->error_text." Please push the BACK button on your browser and try again.");
	}	
	else
	{
		notify("The login ID and password have been successfully sent to <b class='black'>" . $to . "</b>.");
	
		if ($come_from_apps) {
			include ('applications/admin/applicant_list.inc');
		}
	}
	unset ($mail);
}

?>
