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
 | Filename: merge_departments.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for taking all of the information      |
 | that we have collected for two departments, and merge them together.         |
 +------------------------------------------------------------------------------+

*/


// First, update the primary division with the merged values, and 
// any values that the secondary department has but the primary doesn't..

$primaryDiv = new EditContact('');
$primaryDiv->loadDepartmentValues($primary_department_id);
$secondDiv = new EditContact('');
$secondDiv->loadDepartmentValues($secondary_department_id);

if (isset($merge_phone))
{
	$sql = ("
		UPDATE employer_department
		SET phone='" . addslashes($merge_phone) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_fax))
{
	$sql = ("
		UPDATE employer_department
		SET fax='" . addslashes($merge_fax) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_email))
{
	$sql = ("
		UPDATE employer_department
		SET email='" . addslashes($merge_email) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_website))
{
	$sql = ("
		UPDATE employer_department
		SET department_website='" . addslashes($merge_website) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_size))
{
	$sql = ("
		UPDATE employer_department
		SET size_id='" . $merge_size . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_industry))
{
	$sql = ("
		UPDATE employer_department
		SET industry_id ='" . $merge_industry . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_location))
{
	$sql = ("
		UPDATE employer_department
		SET location_info='" . $merge_location . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_address))
{
	$sql = ("
		SELECT DISTINCT street_address_1, street_address_2, street_address_3,
		country_id, provstate_id, region_id, city, postal_code
		FROM employer_department
		WHERE department_id='" . $merge_address . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
		
	$sql = ("
		UPDATE employer_department
		SET street_address_1='" . addslashes($row["street_address_1"]) . "',
		street_address_2='" . addslashes($row["street_address_2"]) . "',
		street_address_3='" . addslashes($row["street_address_3"]) . "',
		country_id='" . $row["country_id"] . "',
		provstate_id='" . $row["provstate_id"] . "',
		region_id='" . $row["region_id"] . "',
		city='" . addslashes($row["city"]) . "',
		postal_code='" . addslashes($row["postal_code"]) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_description))
{
	$sql = ("
		SELECT DISTINCT department_description
		FROM employer_department
		WHERE department_id='" . $merge_description . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$sql = ("
		UPDATE employer_department
		SET department_description='" . addslashes($row["department_description"]) . "'
		WHERE department_id='" . $primary_department_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

// Merge the divisions notes.
// First take all of the secondary divisions notes, and assign them to the new division, and add merge at the start of them.
if ($secondDiv->department_note_ids)
{
        foreach ($secondDiv->department_note_ids AS $dni)
        {
                $sql = ("
                        SELECT DISTINCT notes
                        FROM division_notes
                        WHERE note_id='" . $dni . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                $old_note = $row["notes"];

                $new_note = "(from merge) - " . trim($old_note);

                $sql = ("
                        UPDATE division_notes
                        SET department_id='" . $primary_department_id . "',
                        notes='" . addslashes($new_note) . "'
                        WHERE note_id='" . $dni . "'
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

$merge_notes = getDivisionName($secondary_department_id) . " was merged with this division on " . $date . ".\n\n";
$merge_notes_by = getContactID($auth->department, $auth->login);
$merge_notes_on = date("Y-m-d H:i:s");

$sql = ("
        INSERT INTO division_notes
        (department_id, notes, date_entered, entered_by)
        VALUES (
        '" . $primary_department_id . "',
        '" . addslashes(trim($merge_notes)) . "',
        '" . $merge_notes_on . "',
        '" . $merge_notes_by . "'
        )
        ");
$GLOBALS['dbh']->Execute($sql);

// The most recent change should now reflect that this department was merged.
$sql = ("
	UPDATE employer_department
	SET changes_recorded_3=changes_recorded_2,
	changes_recorded_2=changes_recorded_1,
	changes_recorded_1='Merged with " . addslashes(getDivisionName($secondary_department_id)) . "',
	change_by_3=change_by_2,
	change_by_2=change_by_1,
	change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "',
	change_date_3=change_date_2,
	change_date_2=change_date_1,
	change_date_1=CURRENT_DATE
	WHERE department_id='" . $primary_department_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

// Point all contacts at the merged department
$sql = ("
	UPDATE contact_employer
	SET department_id='" . $primary_department_id . "',
    employer_id='" . $primaryDiv->department_company_id . "'
	WHERE department_id='" . $secondary_department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Point all history records to the merged department
$sql = ("
	UPDATE history
	SET company_department_id='" . $primary_department_id . "'
	WHERE company_department_id='" . $secondary_department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$secondary_division_name = getDivisionName($secondary_department_id);


// Point all job descriptions to the merged department
$sql = ("
	UPDATE job_info
	SET employer_department_id='" . $primary_department_id . "',
	employer_id='" . $primaryDiv->department_company_id . "'
	WHERE employer_department_id='" . $secondary_department_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Get the old and new employer_id
$sql = ("
    SELECT DISTINCT ed.employer_id AS new_employer_id, ed2.employer_id AS old_employer_id 
    FROM employer_department AS ed
    INNER JOIN employer_department AS ed2
    ON ed2.department_id='" . $secondary_department_id . "'
    WHERE ed.department_id='" . $primary_department_id . "'
    ");
$idresult = $GLOBALS['dbh']->Execute($sql);
$idrow = $idresult->FetchRow();

$old_employer_id = $idrow["old_employer_id"];
$new_employer_id = $idrow["new_employer_id"];

if ($merge_status_department_id == $secondary_department_id)
{
    $sql = ("
        DELETE FROM department_division_status
        WHERE division_id = '".$primary_department_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);

    $sql = ("
        UPDATE department_division_status
        SET division_id = '".$primary_department_id."'                                                                                                   
        WHERE division_id = '".$secondary_department_id."'                                                                                               
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
    updateCompanyStatus($old_employer_id, $row["department_id"]);
    updateCompanyStatus($new_employer_id, $row["department_id"]);
    updateDivisionStatus($primary_department_id, $row["department_id"]);
}

// Delete the old division's status
$sql = ("
	DELETE FROM department_division_status
	WHERE division_id='" . $secondary_department_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

// Delete the old department
$sql = ("
	DELETE FROM employer_department
	WHERE department_id='" . $secondary_department_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

notify("<b>" . $secondary_division_name . "</b> has been successfully merged with <b>" . getDivisionName($primary_department_id) . "</b>.");

?>
