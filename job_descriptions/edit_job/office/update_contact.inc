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
 | Filename: update_contact.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is used to transfer information from the job object   |
 | to the database.                                                             |
 +------------------------------------------------------------------------------+

*/

assert(isset($auth));

// Check and see if the contact has been changed, and if it has, update the new contact's status, as well as those of the division and company.
$sql = ("
    SELECT DISTINCT contact_id, status
    FROM job_info
    WHERE job_id='" . $job_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$job_status = $row['status'];

if ($row["contact_id"] != $job->contact_id)
{
    $contact_has_changed = true;
}

if ($contact_has_changed && ($job_status == POSTED || $job_status == CANCELLED || $job_status == CLOSED))
{
    for ($i = 0; $i < sizeof($job->apply_departments); $i++)
    {
        updateContactStatus($job->contact_id, $job->apply_departments[$i], $job_id, null);
    }
}

/*
 If the department_id is 'none', then set it to nothing.
*/

if ($job->department_id == 'none')
{
	$job->department_id = '';
}

/*
 Update any new company information.
*/

$sql = ("
	UPDATE employer_company
	SET 
	website='" . addSlashes($job->company_website) . "',
	company_description='" . addSlashes($job->company_description) . "'
	WHERE employer_id='" . $job->employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

/*
 Update any new division information.
*/

$sql = ("
	UPDATE employer_department
	SET
	department_website='" . addSlashes($job->department_website) . "',
	department_description='" . addSlashes($job->department_description) . "'
	WHERE department_id='" . $job->department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

/*
 Update any new contact information.
*/
$sql = ("
	UPDATE contact
	SET
	phone='" . addSlashes($job->contact_phone) . "',
	cellphone='" . addSlashes($job->contact_cellphone) . "',
	fax='" . addSlashes($job->contact_fax) . "',
	email='" . addSlashes($job->contact_email) . "'
	WHERE contact_id='" . $job->contact_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Update the job with this new contact_id
$sql = ("
	UPDATE job_info
	SET 
	contact_id='" . $job->contact_id . "',
	employer_id='" . $job->employer_id . "',
	employer_department_id='" . $job->department_id . "',
	receive_address='" . addSlashes($job->receive_address) . "',
	apply_method='" . addSlashes($job->apply_method) . "'
	WHERE job_id='" . $job_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

echo("<h3>All changes for " . $job->job_code . " have been successfully saved.</h3>");
echo("<hr />");

//delete object "job"
unset($job);

?>
