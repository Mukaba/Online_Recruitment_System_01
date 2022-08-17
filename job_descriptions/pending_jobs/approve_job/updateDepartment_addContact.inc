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
 | Filename: updateDepartment_addContact.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is used to shift all of the inputs for a pending      |
 | company, contact and job into a permanent spot for them in the database.     |
 | The company and division are existing ones, and the division will be         |
 | updated with the given information.  The contact will be added as new to     |
 | either the company or the division, depending on which was chosen.           |
 +------------------------------------------------------------------------------+

*/

assert(isset($job_id));

// Draw pending values from database.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

// Insert contact values

$sql = ("
	INSERT INTO contact
	(
	title, first_name, last_name, called_name, middle_name, email, phone, pager, cellphone, fax, street_address_1, 
	street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code
	)
	VALUES (
	'" . addslashes(trim($pendingEmployer->contact_title)) . "',
	'" . addslashes(trim($pendingEmployer->contact_first_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_last_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_called_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_middle_name)) . "',
	'" . addslashes(trim($pendingEmployer->contact_email)) . "',
	'" . addslashes(trim($pendingEmployer->contact_phone)) . "',
	'" . addslashes(trim($pendingEmployer->contact_pager)) . "',
	'" . addslashes(trim($pendingEmployer->contact_cellphone)) . "',
	'" . addslashes(trim($pendingEmployer->contact_fax)) . "',
	'" . addslashes(trim($pendingEmployer->contact_street_address_1)) . "',
	'" . addslashes(trim($pendingEmployer->contact_street_address_2)) . "',
	'" . addslashes(trim($pendingEmployer->contact_street_address_3)) . "',
	'" . addslashes(trim($pendingEmployer->contact_city)) . "',
	'" . addslashes(trim($pendingEmployer->contact_region_id)) . "',
	'" . addslashes(trim($pendingEmployer->contact_provstate_id)) . "',
	'" . addslashes(trim($pendingEmployer->contact_country_id)) . "',
	'" . addslashes(trim($pendingEmployer->contact_postal_code)) . "'
	)
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
	SELECT LAST_INSERT_ID() AS new_contact_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$new_contact_id = $row["new_contact_id"];

// Check and make sure that we have an employer_id.  If not, draw it out from the employer_department table, based on the $matching_department_id variable.
if (!$matching_employer_id)
{
    $sql = ("
        SELECT DISTINCT employer_id
        FROM employer_department
        WHERE department_id='" . addslashes(trim($matching_department_id)) . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    $matching_employer_id = $row["employer_id"];
}

// Insert contact_employer values

$sql = ("
	INSERT INTO contact_employer
	(
	contact_id, employer_id, department_id, entered_by, entered_on
	)
	VALUES (
	'" . addslashes(trim($new_contact_id)) . "',
	'" . addslashes(trim($matching_employer_id)) . "',
	'" . addslashes(trim($matching_department_id)) . "',
	'" . addslashes(trim($auth->contact_id)) . "',
	CURRENT_DATE
	)
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Deletion from pending contact table will be done at the end.

// Insert the job description

$pendingEmployer->job_workterm_lengths = implode(",", $pendingEmployer->job_workterm_lengths);

$sql = ("
	INSERT INTO job_info
	(
	employer_id, department_id, contact_id, employer_department_id, term_id, year, position_title, num_positions, 
	apply_method, receive_address, country_id, provstate_id, region_id, city, salary_amount1, salary_amount2, salary_period, 
	workterm_lengths, other_postings, job_description, min_academic, min_workterms, special_requirements, skills_required, 
	comments, status, entered_by, workterm_hours, employer_code, is_rejectable, admin_status, date_entered, temp_date_entered,
    start_date, end_date, displayname_on_coverltr
	)
	VALUES (
	'" . addslashes(trim($matching_employer_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_host_department)) . "',
	'" . addslashes(trim($new_contact_id)) . "',
	'" . addslashes(trim($matching_department_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_start_term)) . "',
	'" . addslashes(trim($pendingEmployer->job_start_year)) . "',
	'" . addslashes(trim($pendingEmployer->job_position_title)) . "',
	'" . addslashes(trim($pendingEmployer->job_num_positions)) . "',
	'" . addslashes(trim($pendingEmployer->job_apply_method)) . "',
	'" . addslashes(trim($pendingEmployer->job_receive_address)) . "',
	'" . addslashes(trim($pendingEmployer->job_country_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_provstate_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_region_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_city)) . "',
	'" . addslashes(trim($pendingEmployer->job_salary_amount1)) . "',
	'" . addslashes(trim($pendingEmployer->job_salary_amount2)) . "',
	'" . addslashes(trim($pendingEmployer->job_salary_period)) . "',
	'" . addslashes(trim($pendingEmployer->job_workterm_lengths)) . "',
	'" . addslashes(trim($pendingEmployer->job_other_postings)) . "',
	'" . addslashes(trim($pendingEmployer->job_description)) . "',
	'" . addslashes(trim($pendingEmployer->job_min_academic)) . "',
	'" . addslashes(trim($pendingEmployer->job_min_workterms)) . "',
	'" . addslashes(trim($pendingEmployer->job_special_requirements)) . "',
	'" . addslashes(trim($pendingEmployer->job_skills_required)) . "',
	'" . addslashes(trim($pendingEmployer->job_comments)) . "',
	'" . NOT_CHECKED . "',
	'" . addslashes(trim($auth->contact_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_workterm_hours)) . "',
	'" . addslashes(trim($pendingEmployer->job_employer_code)) . "',
	'1',
	'1',
    CURRENT_DATE(),
    '" . getTempDateEntered($pendingEmployer->job_start_term, $pendingEmployer->job_start_year) . "',
    ". ($pendingEmployer->job_start_date ? "'".$pendingEmployer->job_start_date."'" : "NULL").",
    ". ($pendingEmployer->job_end_date ? "'".$pendingEmployer->job_end_date."'" : "NULL").",
    ". ($pendingEmployer->job_displayname_on_coverltr ? "'".$pendingEmployer->job_displayname_on_coverltr."'" : "0")."

	)
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
	SELECT LAST_INSERT_ID() AS new_job_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$new_job_id = $row["new_job_id"];
	
// Insert apply departments

for ($i = 0; $i < sizeof($pendingEmployer->job_apply_departments); $i++)
{
	$sql = ("
		INSERT INTO department_job_join(
		department_id, job_id
		)
		VALUES (
		'" . addslashes($pendingEmployer->job_apply_departments[$i]) . "',
		'" . addslashes($new_job_id) . "'
		)
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

// Insert applicable disciplines

for ($i = 0; $i < sizeof($pendingEmployer->job_disciplines); $i++)
{
    $sql = ("
            INSERT INTO job_discipline_join
            (job_id, discipline_id)
            VALUES (
            '" . addslashes($new_job_id) . "',
            '" . addslashes($pendingEmployer->job_disciplines[$i]) . "'
            )
            ");
    $GLOBALS['dbh']->Execute($sql);
}

// Update the statuses of the new contact, division and company, for each applicable department
$sql = ("
    SELECT department_id
    FROM department
    WHERE job_list = '1'
    ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    updateContactStatus($new_contact_id, $row['department_id'], null, null);
}
 
// Deletion from the pending job table will be done at the end.

$sql = ("
	SELECT company_name
	FROM employer_company
	WHERE employer_id='" . addslashes($matching_employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_name = $row["company_name"];

if ($matching_department_id)
{
	$sql = ("
		SELECT department_name
		FROM employer_department
		WHERE department_id='" . addslashes($matching_department_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	$department_name = $row["department_name"];
}

$msg .= ("The contact and job information have been saved into the database, with <b>" . $company_name . "</b> as their parent company");
$msg .= (" and <b>" . $department_name . "</b> as their parent division");

$msg .= (".  The job description has also been saved with <b>Unchecked</b> status.  You may now edit the job's inputs and update it's");
$msg .= (" status using the <b>Edit Job</b> option under <b>Job Descriptions</b>.");

$msg .= ("<br /><br />Shown below are the old inputs for <b>" . $department_name . "</b>.  The new inputs are written in <b class='black'>bold</b> ");
$msg .= ("below each field.  Please check over the data, make any changes you wish, and click Save at the bottom.");

notify($msg);

//pendingEmployer and matching_employer_id must be set before preceding.

$oldValues = new EditContact('');
$oldValues->loadDepartmentValues($matching_department_id);

include('job_descriptions/pending_jobs/approve_job/show_old_new_department_inputs.inc');

?>
