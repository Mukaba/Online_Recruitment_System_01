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
 | Filename: merge_contacts.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is responsible for taking all of the information      |
 | that we have collected for two contacts, and merge them together.            |
 +------------------------------------------------------------------------------+

*/


// First, update the primary contact with the merged values, and 
// any values that the secondary contact has but the primary doesn't..

$primaryContact = new EditContact('');
$primaryContact->loadContactValues($primary_contact_id);
$secondContact = new EditContact('');
$secondContact->loadContactValues($secondary_contact_id);
$merge_flags = trim_array($merge_flags);

$sql = ("
    SELECT DISTINCT login_id
    FROM employer_login
    WHERE contact_id='" . $primary_contact_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$first_login_id = $row["login_id"];

$sql = ("
    SELECT DISTINCT login_id
    FROM employer_login
    WHERE contact_id='" . $secondary_contact_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$second_login_id = $row["login_id"];

if (trim($first_login_id) == '' && trim($second_login_id))
{
    $sql = ("
        UPDATE employer_login
        SET contact_id='" . $primary_contact_id . "',
        employer_id='" . $primaryContact->contact_employer_id . "'
        WHERE login_id='" . $second_login_id . "'
        AND contact_id='" . $secondary_contact_id . "'
        ");
    $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_title))
{
	$sql = ("
		UPDATE contact
		SET title='" . addslashes($merge_title) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_email))
{
	$sql = ("
		UPDATE contact
		SET email='" . addslashes($merge_email) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_greeting))
{
	$sql = ("
		UPDATE contact
		SET greeting='" . addslashes($merge_greeting) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_phone))
{
	$sql = ("
		UPDATE contact
		SET phone='" . addslashes($merge_phone) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_fax))
{
	$sql = ("
		UPDATE contact
		SET fax='" . addslashes($merge_fax) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_pager))
{
	$sql = ("
		UPDATE contact
		SET pager='" . addslashes($merge_pager) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_cellphone))
{
	$sql = ("
		UPDATE contact
		SET cellphone='" . addslashes($merge_cellphone) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_position_title))
{
	$sql = ("
		UPDATE contact_employer
		SET position='" . addslashes($merge_position_title) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_department_name))
{
	$sql = ("
		UPDATE contact_employer
		SET department_name='" . addslashes($merge_department_name) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_location))
{
        $sql = ("
                UPDATE contact
                SET location_info='" . $merge_location . "'
                WHERE contact_id='" . $primary_contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
}

if (isset($merge_address))
{
	$sql = ("
		SELECT DISTINCT street_address_1, street_address_2, street_address_3,
		country_id, provstate_id, region_id, city, postal_code
		FROM contact
		WHERE contact_id='" . $merge_address . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
		
	$sql = ("
		UPDATE contact
		SET street_address_1='" . addslashes($row["street_address_1"]) . "',
		street_address_2='" . addslashes($row["street_address_2"]) . "',
		street_address_3='" . addslashes($row["street_address_3"]) . "',
		country_id='" . $row["country_id"] . "',
		provstate_id='" . $row["provstate_id"] . "',
		region_id='" . $row["region_id"] . "',
		city='" . addslashes($row["city"]) . "',
		postal_code='" . addslashes($row["postal_code"]) . "'
		WHERE contact_id='" . $primary_contact_id . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
}

/* 
 Copy all of the flags that are associated with a department other than the 
 current user's, and that are SET for the secondary contact, over to the primary contact.
*/

$sql = ("
    SELECT DISTINCT cfs.flag_id
    FROM contact_flags_set AS cfs, department_contact_flags AS dcf
    WHERE cfs.contact_id='" . $primary_contact_id . "'
    AND dcf.department_id!='" . $auth->department . "'
    AND cfs.flag_id=dcf.flag_id
    ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    $flags_to_ignore .= "'" . $row["flag_id"] . "',";
}
if ($flags_to_ignore)
{
    $flags_to_ignore = substr($flags_to_ignore, 0, -1);
}
else
{
    $flags_to_ignore = "''";
}

$sql = ("
    SELECT DISTINCT cfs.flag_id
    FROM contact_flags_set AS cfs, department_contact_flags AS dcf
    WHERE cfs.contact_id='" . $secondary_contact_id . "'
    AND cfs.flag_id=dcf.flag_id
    AND dcf.department_id!='" . $auth->department . "'
    AND cfs.flag_id NOT IN (" . $flags_to_ignore . ")
    ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    $sql = ("
        INSERT INTO contact_flags_set
        (contact_id, flag_id)
        VALUES (
        '" . $primary_contact_id . "',
        '" . $row["flag_id"] . "'
        )
        ");
    $GLOBALS['dbh']->Execute($sql);
}

// Delete and reinsert all of the flags for this contact that are associated with THIS department.  Flags for other companies are all copied.
$sql = ("
    SELECT DISTINCT flag_id
    FROM department_contact_flags
    WHERE department_id='" . $auth->department . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow())
{
    $flags_to_delete .= "'" . $row["flag_id"] . "',";
}
if ($flags_to_delete)
{
    $flags_to_delete = substr($flags_to_delete, 0, -1);
}
else
{
    $flags_to_delete = "''";
}

if (isset($merge_flags))
{
	$sql = ("
		DELETE FROM contact_flags_set
		WHERE contact_id='" . $primary_contact_id . "'
        AND flag_id IN (" . $flags_to_delete . ")
		");
	$result = $GLOBALS['dbh']->Execute($sql);

    if (is_array($merge_flags))
    {
        foreach ($merge_flags AS $mf)
        {
            $sql = ("
                INSERT INTO contact_flags_set
                (contact_id, flag_id)
                VALUES (
                '" . $primary_contact_id . "',
                '" . $mf . "'
                )
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }
    }
}

// Merge the actions for these two contacts
// Take all of the secondary contact's notes, and assign them to the new contact.
if ($secondContact->contact_action_ids)
{
    foreach ($secondContact->contact_action_ids AS $cai)
    {
        $sql = ("
            UPDATE contact_actions_set
            SET contact_id='" . $primary_contact_id . "'
            WHERE contact_actions_id='" . $cai . "'
            ");
        $GLOBALS['dbh']->Execute($sql);
    }
}

// Merge the notes for these two contacts
// First take all of the secondary contact's notes, and assign them to the new contact, and add merge at the start of them.
if ($secondContact->contact_note_ids)
{
    foreach ($secondContact->contact_note_ids AS $cni)
    {
        $sql = ("
            SELECT DISTINCT action_note 
            FROM contact_actions_set
            WHERE contact_actions_id='" . $cni . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $old_note = $row["action_note"];

        $new_note = "(from merge) - " . trim($old_note);

        $sql = ("
            UPDATE contact_actions_set
            SET contact_id='" . $primary_contact_id . "',
            action_note='" . addslashes($new_note) . "'
            WHERE contact_actions_id='" . $cni . "'
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

$first_contact_name = getContactName($primary_contact_id);
$first_contact = $first_contact_name["first_name"] . " " . $second_contact_name["last_name"];

$second_contact_name = getContactName($secondary_contact_id);
$second_contact = $second_contact_name["first_name"] . " " . $second_contact_name["last_name"];

$merge_notes = $second_contact . " was merged with this contact on " . $date . ".\n\n";
$merge_notes_by = getContactID($auth->department, $auth->login);
$merge_notes_on = date("Y-m-d H:i:s");

$sql = ("
    INSERT INTO contact_actions_set
    (contact_id, action_note, action_on, action_by, action_id)
    VALUES (
    '" . $primary_contact_id . "',
    '" . addslashes(trim($merge_notes)) . "',
    '" . $merge_notes_on . "',
    '" . $merge_notes_by . "',
    '" . GENERIC_NOTE . "'
    )
    ");
$GLOBALS['dbh']->Execute($sql);

// The most recent change should now reflect that this contact was merged.
$sql = ("
	UPDATE contact_employer
	SET changes_recorded_3=changes_recorded_2,
	changes_recorded_2=changes_recorded_1,
	changes_recorded_1='Merged with " . addslashes($second_contact_name["first_name"]) . " " . addslashes($second_contact_name["last_name"]) . "',
	change_by_3=change_by_2,
	change_by_2=change_by_1,
	change_by_1='" . addslashes(trim(getContactID($auth->department, $auth->login))) . "',
	change_date_3=change_date_2,
	change_date_2=change_date_1,
	change_date_1=CURRENT_DATE
	WHERE contact_id='" . $primary_contact_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
    UPDATE job_info 
    SET contact_id = '".$primary_contact_id."'
    WHERE contact_id = '".$secondary_contact_id."'
    ");
$GLOBALS['dbh']->Execute($sql);

// Get the old and new employer_id and division_id
$sql = ("
    SELECT DISTINCT ce.employer_id AS new_employer_id, ce.department_id AS new_division_id, ce2.employer_id AS old_employer_id, 
    ce2.department_id AS old_division_id
    FROM contact_employer AS ce
    INNER JOIN contact_employer AS ce2
    ON ce2.contact_id='" . $secondary_contact_id . "'
    WHERE ce.contact_id='" . $primary_contact_id . "'
    ");
$idresult = $GLOBALS['dbh']->Execute($sql);
$idrow = $idresult->FetchRow();

$old_division_id = $idrow["old_division_id"];
$new_division_id = $idrow["new_division_id"];
$old_employer_id = $idrow["old_employer_id"];
$new_employer_id = $idrow["new_employer_id"];

// If the user has chosen the secondary contact's status, we delete the primary contact status and set the secondary contact status as the primary one. 
if ($merge_status_contact_id == $secondary_contact_id)
{
    $sql = ("
        DELETE FROM department_contact_status
        WHERE contact_id = '".$primary_contact_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);

    $sql = ("
        UPDATE department_contact_status
        SET contact_id = '".$primary_contact_id."'
        WHERE contact_id = '".$secondary_contact_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);
}

// Iterate through each department other than this one, and merge the statuses, based on the highest level of activity.
$sql = ("
    SELECT department_id
    FROM department
    WHERE job_list = '1'
    ");
$result = $GLOBALS['dbh']->Execute($sql);

while ($row = $result->FetchRow())
{
    // get current status of primary contact 
    $current_status_sql = ("
        SELECT activity_date, status_id
        FROM department_contact_status
        WHERE contact_id = '".$primary_contact_id."' AND department_id = '".$row["department_id"]."' AND current_status = 1
        ");
    $current_status_result = $GLOBALS['dbh']->Execute($current_status_sql);
    $current_status_row = $current_status_result->FetchRow();

    // We now need to reevaluate the status for our new contact.
    $status_array = reevaluateContactStatus($primary_contact_id, $row["department_id"]);

    if ($status_array['status'] != $current_status_row['status_id'])
    {
        $sql = ("
            INSERT INTO department_contact_status
            (contact_id, status_id, department_id, entered_on, activity_date, activity_type_id, term_id, year)
            VALUES (
                '".$primary_contact_id."'
                , '".$status_array["status"]."'
                , '".$row["department_id"]."'
                , now()
                , '".$current_status_row["activity_date"]."'
                , '".AUTOMATED."'
                , '0'
                , '0000'
            )
            ");
        updateContactCurrentStatus($primary_contact_id, $row["department_id"]);
        updateDivisionStatus($new_division_id, $row["department_id"]);
        updateCompanyStatus($new_employer_id, $row["department_id"]);
    }

    // Now we need to update the status of the old and new companies and divisions
    updateDivisionStatus($old_division_id, $row["department_id"]);
    updateCompanyStatus($old_employer_id, $row["department_id"]);
}


// Delete all of the secondary contact's remaining information.
$sql = ("
	DELETE FROM contact_flags_set
	WHERE contact_id='" . $secondary_contact_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
	DELETE FROM contact_actions_set
	WHERE contact_id='" . $secondary_contact_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
	DELETE FROM contact
	WHERE contact_id='" . $secondary_contact_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
	DELETE FROM contact_employer
	WHERE contact_id='" . $secondary_contact_id . "'
	");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
    DELETE FROM department_contact_status
    WHERE contact_id='" . $secondary_contact_id . "'
    ");
$GLOBALS['dbh']->Execute($sql);

$sql = ("
    DELETE FROM employer_login
    WHERE contact_id='" . $secondary_contact_id . "'
    ");
$GLOBALS['dbh']->Execute($sql);

$msg = ("<b>" . $second_contact_name["first_name"] . " " . $second_contact_name["last_name"]);
$msg .= ("</b> has been successfully merged with <b>" . $first_contact_name["first_name"] . " " . $first_contact_name["last_name"] . "</b>.");
notify($msg);

?>
