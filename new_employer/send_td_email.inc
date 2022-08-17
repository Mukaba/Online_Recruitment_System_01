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
 | Filename: send_td_email.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send an email to the traffic director to   |
 | alert them that a new job has been entered in the system pending approval.   |
 +------------------------------------------------------------------------------+

*/

global $auth;

$sql = ("
	SELECT DISTINCT contact_id
	FROM contact_internal
	WHERE department_id='" . TRAFFIC_DIRECTOR_DEPARTMENT . "'
	AND traffic_director_email='1'
	");
$contact_result = $GLOBALS['dbh']->Execute($sql);

while ($contact_row = $contact_result->FetchRow())
{
	$users_contact_id = $contact_row["contact_id"];
	$user_email .= getCiEmailUsingID($users_contact_id) . ",";
}

if ($user_email)
{
	$user_email = substr($user_email, 0, -1);
}

$to = $user_email;

$subject = "Notification of new job pending approval";

$body = ("This e-mail has been sent to notify you that a job has been posted by a new employer, and is pending approval.");
$body .= ("\n\nThe job and employer's information is as follows:");
$body .= ("\n\nCompany Name: " . $company_name);
$body .= ("\nContact Name: " . $contact_name);
$body .= ("\nPosition Title: " . $position_title);

if ($no_host_department)
{
	$body .= ("\n\nThis job does not yet have a host department assigned to it, ");
	$body .= ("so this will need to be done in addition to the job being approved.");
}

$from = BRAND_NAME_SYSTEM_EMAIL;
$reply_to = $to;		
		
send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to);

?>
