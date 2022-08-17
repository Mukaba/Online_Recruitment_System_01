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
 | Filename: merge_companies.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for taking all of the information      |
 | that we have collected for two companies, and merge them together.           |
 +------------------------------------------------------------------------------+

*/


/* 
 First, update the primary company with the merged values, and 
 any values that the secondary company has but the primary doesn't..
*/

$primaryComp = new EditContact('');
$primaryComp->loadCompanyValues($primary_employer_id);
$secondComp = new EditContact('');
$secondComp->loadCompanyValues($secondary_employer_id);

if (isset($merge_phone))
{
	$sql = ("
		UPDATE employer_company
		SET phone='" . $merge_phone . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_fax))
{
	$sql = ("
		UPDATE employer_company
		SET fax='" . $merge_fax . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_email))
{
	$sql = ("
		UPDATE employer_company
		SET email='" . $merge_email . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_website))
{
	$sql = ("
		UPDATE employer_company
		SET website='" . $merge_website . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_size))
{
	$sql = ("
		UPDATE employer_company
		SET size_id='" . $merge_size . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_industry))
{
	$sql = ("
		UPDATE employer_company
		SET industry_id='" . $merge_industry . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_type))
{
	$sql = ("
		UPDATE employer_company
		SET company_type_id='" . $merge_type . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_address))
{
	$sql = ("
		SELECT DISTINCT street_address_1, street_address_2, street_address_3,
		country_id, provstate_id, region_id, city, postal_code
		FROM employer_company
		WHERE employer_id='" . $merge_address . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
		
	$sql = ("
		UPDATE employer_company
		SET street_address_1='" . addslashes($row["street_address_1"]) . "',
		street_address_2='" . addslashes($row["street_address_2"]) . "',
		street_address_3='" . addslashes($row["street_address_3"]) . "',
		country_id='" . $row["country_id"] . "',
		provstate_id='" . $row["provstate_id"] . "',
		region_id='" . $row["region_id"] . "',
		city='" . addslashes($row["city"]) . "',
		postal_code='" . addslashes($row["postal_code"]) . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_description))
{
	$sql = ("
		SELECT DISTINCT company_description
		FROM employer_company
		WHERE employer_id='" . $merge_description . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$sql = ("
		UPDATE employer_company
		SET company_description='" . addslashes($row["company_description"]) . "'
		WHERE employer_id='" . $primary_employer_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

// Merge the notes for both of these companies.

// First take all of the secondary companies notes, and assign them to the new company, and add merge at the start of them.

if ($secondComp->company_note_ids)
{
	foreach ($secondComp->company_note_ids AS $cni)
	{
		$sql = ("
			SELECT DISTINCT notes
			FROM company_notes
			WHERE note_id='" . $cni . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();
		$old_note = $row["notes"];

		$new_note = "(from merge) - " . trim($old_note);

		$sql = ("
			UPDATE company_notes
			SET employer_id='" . $primary_employer_id . "',
			notes='" . addslashes($new_note) . "'
			WHERE note_id='" . $cni . "'
			");
		$GLOBALS['dbh']->Execute($sql);
	}
}

$sql = ("
	SELECT CURRENT_DATE
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$date = $row["CURRENT_DATE"];
$date = formatLongDate($date);

$merge_notes = getCompanyName($secondary_employer_id) . " was merged with this company on " . $date . ".\n\n";
$merge_notes_by = getContactID($auth->department, $auth->login);
$merge_notes_on = date("Y-m-d H:i:s");

$sql = ("
	INSERT INTO company_notes
	(employer_id, notes, date_entered, entered_by)
	VALUES (
	'" . $primary_employer_id . "',
	'" . addslashes(trim($merge_notes)) . "',
	'" . $merge_notes_on . "',
	'" . $merge_notes_by . "'
	)
	");
$GLOBALS['dbh']->Execute($sql);

// The most recent change should now reflect that this company was merged.
$sql = ("
	UPDATE employer_company
	SET changes_recorded_3=changes_recorded_2,
	changes_recorded_2=changes_recorded_1,
	changes_recorded_1='Merged with " . addslashes(getCompanyName($secondary_employer_id)) . "',
	change_by_3=change_by_2,
	change_by_2=change_by_1,
	change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "',
	change_date_3=change_date_2,
	change_date_2=change_date_1,
	change_date_1=CURRENT_DATE
	WHERE employer_id='" . $primary_employer_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

// Point all divisons at the merged company
$sql = ("
	UPDATE employer_department
	SET employer_id='" . $primary_employer_id . "'
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Point all contacts at the merged company
$sql = ("
	UPDATE contact_employer
	SET employer_id='" . $primary_employer_id . "'
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Point all history records to the merged company
$sql = ("
	UPDATE history
	SET employer_id='" . $primary_employer_id . "'
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$secondary_company_name = getCompanyName($secondary_employer_id);


// Point all job descriptions to the merged company
$sql = ("
	UPDATE job_info
	SET employer_id='" . $primary_employer_id . "'
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Update all instances of employer login
$sql = ("
	UPDATE employer_login
	SET employer_id='" . $primary_employer_id . "'
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($merge_status_employer_id == $secondary_employer_id)
{
    $sql = ("
        DELETE FROM department_company_status
        WHERE employer_id = '".$primary_employer_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);

    $sql = ("
        UPDATE department_company_status
        SET employer_id = '".$primary_employer_id."'                                                                                                   
        WHERE employer_id = '".$secondary_employer_id."'                                                                                               
        ");
    $GLOBALS['dbh']->Execute($sql);
}

// Update the status for each department at our organization.
$sql = ("
    SELECT department_id
    FROM department
    WHERE job_list = '1'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    updateCompanyStatus($primary_employer_id, $row['department_id']);
}

// Delete the old company's status
$sql = ("
	DELETE FROM department_company_status
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

// Delete the old company
$sql = ("
	DELETE FROM employer_company
	WHERE employer_id='" . $secondary_employer_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

notify("<b>" . $secondary_company_name . "</b> has been successfully merged with <b>" . getCompanyName($primary_employer_id) . "</b>.");

?>
