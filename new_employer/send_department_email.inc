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
 | Filename: send_department_email.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send an email to the default department    |
 | contact to alert them that their department has been assigned a new pending  |
 | job.                                                                         |
 +------------------------------------------------------------------------------+

*/

global $auth;

$sql = ("
	SELECT DISTINCT jobhost_contact_id
	FROM department
	WHERE department_id = ?
	");
$stmt = $GLOBALS['dbh']->Prepare($sql);
$contact_result = $GLOBALS['dbh']->Execute($stmt, $host_department);
$contact_row = $contact_result->FetchRow();

$users_contact_id = $contact_row["jobhost_contact_id"];
$user_email = getCiEmailUsingID($users_contact_id);

$to = $user_email;

$subject = "Notification of new job pending approval";

$body = ("This e-mail has been sent to notify you that a job has been posted by a new employer, and is pending approval.");
$body .= ("\n\nThe job's information is as follows:");
$body .= ("\n\nCompany Name: " . $company_name);
$body .= ("\nContact Name: " . $contact_name);
$body .= ("\nPosition Title: " . $position_title);

$from = BRAND_NAME_SYSTEM_EMAIL;
$reply_to = $to;		
		
send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to);

?>
