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
 | Filename: ignoreCompany_updateContact.inc                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is used to save the new job description to the and    |
 | then update the chosen contact's information.  The company information is    |
 | discarded.                                                                   |
 +------------------------------------------------------------------------------+

*/

assert(isset($job_id));

// Draw pending values from database.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

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
	'" . addslashes(trim($new_employer_id)) . "',
	'" . addslashes(trim($pendingEmployer->job_host_department)) . "',
	'" . addslashes(trim($matching_contact_id)) . "',
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
		INSERT INTO job_discipline_join(
		job_id, discipline_id
		)
		VALUES (
		'" . addslashes($new_job_id) . "',
		'" . addslashes($pendingEmployer->job_disciplines[$i]) . "'
		)
		");
	$GLOBALS['dbh']->Execute($sql);
}

// Deletion of old values will be done after.

// Notify of success.

$sql = ("
	SELECT company_name
	FROM employer_company
	WHERE employer_id='" . addslashes($new_employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$company_name = $row["company_name"];

$sql = ("
	SELECT CONCAT(first_name, ' ', last_name) AS contact_name
	FROM contact
	WHERE contact_id='" . addslashes($matching_contact_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$contact_name = $row["contact_name"];

$msg .= ("The job description have been saved into the database with <b>Unchecked</b> status.  You may ");
$msg .= ("now edit the job's inputs and update it's status using the <b>Edit Job</b> option under <b>Job Descriptions</b>.");

$msg .= ("<br /><br />Shown below are the old inputs for <b>" . $contact_name . "</b>.  The new inputs are written in <b class='black'>bold</b>");
$msg .= (" below each field.  Please check over the data, make any changes you wish, and click Save at the bottom.");

notify($msg);

//pendingEmployer and matching_employer_id must be set before preceding.

$oldValues = new EditContact('');
$oldValues->loadContactValues($matching_contact_id);

$action = 'ignoreCompany_updateContact';

$new_employer_id = $matching_employer_id;

include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');

?>
