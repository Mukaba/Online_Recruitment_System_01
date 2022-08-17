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
 | Filename: save_updated_company_then_contact.inc                              |
 +------------------------------------------------------------------------------+
 | Description: This file is used to transfer the newly updated company         |
 | inputs into the database.  This file needs to have $matching_employer_id     |
 | set as well as the EditContact class set with all of the new inputs.         |
 +------------------------------------------------------------------------------+

*/

$recordedChanges = recordContactChange($updatedCompany, "company");

$sql = ("
	UPDATE employer_company
	SET website='" . addslashes($updatedCompany->company_website) . "',
	company_description='" . addslashes($updatedCompany->company_description) . "',
	street_address_1='" . addslashes($updatedCompany->company_street_address_1) . "',
	street_address_2='" . addslashes($updatedCompany->company_street_address_2) . "',
	street_address_3='" . addslashes($updatedCompany->company_street_address_3) . "',
	city='" . addslashes($updatedCompany->company_city) . "',
	region_id='" . addslashes($updatedCompany->company_region_id) . "',
	provstate_id='" . addslashes($updatedCompany->company_provstate_id) . "',
	country_id='" . addslashes($updatedCompany->company_country_id) . "',
	postal_code='" . addslashes($updatedCompany->company_postal_code) . "',
	phone='" . addslashes($updatedCompany->company_phone) . "',
	fax='" . addslashes($updatedCompany->company_fax) . "',
	email='" . addslashes($updatedCompany->company_email) . "',
	industry_id='" . addslashes($updatedCompany->company_industry_id) . "',
	size_id='" . addslashes($updatedCompany->company_size_id) . "',
	company_type_id='" . addslashes($updatedCompany->company_type_id) . "'
	WHERE employer_id='" . addslashes($matching_employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($recordedChanges)
{
	/*
	 Lastly, we need to set the change reasons, dates, and by who.  Do this by first bumping the first
	 and second reasons down one, and then inserting the most recent reason, date and who into the database.
	*/

	$sql = ("
		SELECT DISTINCT changes_recorded_1, changes_recorded_2, change_date_1, change_date_2,
		change_by_1, change_by_2
		FROM employer_company
		WHERE employer_id='" . addslashes($matching_employer_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();

	$changes_recorded_2 = $row["changes_recorded_1"];
	$changes_recorded_3 = $row["changes_recorded_2"];
	$change_date_2 = $row["change_date_1"];
	$change_date_3 = $row["change_date_2"];
	$change_by_2 = $row["change_by_1"];
	$change_by_3 = $row["change_by_2"];

	$sql = ("
		UPDATE employer_company
		SET
		changes_recorded_1='" . addslashes(trim($recordedChanges)) . "',
		changes_recorded_2='" . addslashes(trim($changes_recorded_2)) . "',
		changes_recorded_3='" . addslashes(trim($changes_recorded_3)) . "',
		change_date_1=CURRENT_DATE,
		change_date_2='" . $change_date_2 . "',
		change_date_3='" . $change_date_3 . "',
		change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "',
		change_by_2='" . $change_by_2 . "',
		change_by_3='" . $change_by_3 . "'
		WHERE employer_id='" . addslashes($matching_employer_id) . "'
		");

	$result = $GLOBALS['dbh']->Execute($sql);
}

// Notify of success.

$sql = ("
    SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS name
    FROM contact
    WHERE contact_id='" . addslashes($matching_contact_id) . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$contact_name = $row["name"];

$sql = ("
	SELECT DISTINCT company_name
	FROM employer_company
	WHERE employer_id='" . addslashes($matching_employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$msg .= ("The information for <b>" . $row["company_name"] . "</b> has been successfully updated.");

$msg .= ("<br /><br />Shown below are the old inputs for <b>" . $contact_name . "</b>.  The new inputs are written in <b class='black'>bold</b>");
$msg .= (" below each field.  Please check over the data, make any changes you wish, and click Save at the bottom.");

notify($msg);

//pendingEmployer and matching_employer_id must be set before preceding.

$pendingEmployer = new PendingEmployer('');
$pendingEmployer->load_pending_values($job_id);

$oldValues = new EditContact('');
$oldValues->loadContactValues($matching_contact_id);

$new_employer_id = $matching_employer_id;

include('job_descriptions/pending_jobs/approve_job/show_old_new_contact_inputs.inc');

?>
