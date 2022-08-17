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
 | Filename: mail_applied_department.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This file is used to send an email to the new host              |
 | department's primary contact to alert them that they have now been assigned  |
 | as the host department to this job.                                          |
 +------------------------------------------------------------------------------+

*/

// We have $applied_department, $applied_contact, $applied_job

$sql = ("
	SELECT DISTINCT department_name
	FROM department
	WHERE department_id='" . $applied_department . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$applied_department_name = $row["department_name"];

$contact_names = getContactName($applied_contact);
$applied_contact_name = $contact_names["first_name"] . " " . $contact_names["last_name"];

$sql = ("
	SELECT DISTINCT ji.position_title, ji.job_code, ji.department_id, ec.company_name
	FROM job_info AS ji
    INNER JOIN employer_company AS ec 
        ON ec.employer_id=ji.employer_id
	WHERE ji.job_id='" . $applied_job . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$position_title = $row["position_title"];
$job_code = $row["job_code"];
$host_department = $row["department_id"];
$company_name = $row["company_name"];

$sql = ("
	SELECT DISTINCT jobhost_contact_id
	FROM department
	WHERE department_id='" . $host_department . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
	$users_contact_id = $row["jobhost_contact_id"];
	$user_email .= getCiEmailUsingID($users_contact_id) . ",";
}
if ($user_email)
{
	$user_email = substr($user_email, 0, -1);
}

$sql = ("
	SELECT ec.company_name, ji.position_title
	FROM job_info AS ji
    INNER JOIN employer_company AS ec 
        ON ec.employer_id=ji.employer_id
	WHERE job_id='" . $job_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$to = $user_email;

$subject = "Notification of a recently applied department";

$body = ("This e-mail has been sent to notify you that a new department has applied to a\n");
$body .= ("job your department is currently hosting.  Details are supplied below.");
$body .= ("\n\nPosition Title: " . $position_title);
$body .= ("\nJob Code: " . $job_code);
$body .= ("\nCompany Name: " . $company_name);
$body .= ("\n\nRecently applied department: " . $applied_department_name);
$body .= ("\nDepartment applied by: " . $applied_contact_name);

$from = BRAND_NAME_SYSTEM_EMAIL;
$reply_to = $to;		
		
send_email($job_id, $to, $cc, $bcc, $subject, $body, $from, $reply_to);
unset($user_email);

?>
