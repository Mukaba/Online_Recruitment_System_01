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
 | Filename: save_updated_contact.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file is used to transfer the newly updated contact         |
 | inputs into the database.  This file needs to have $matching_employer_id,    |
 | $matching_department_id set as well as the EditContact class set with all    |
 | of the new inputs.                                                           |
 +------------------------------------------------------------------------------+

*/

$recordedChanges = recordContactChange($updatedContact, "contact");

$sql = ("
	UPDATE contact
	SET title='" . addslashes(trim($updatedContact->contact_title)) . "',
	first_name='" . addslashes(trim($updatedContact->contact_first_name)) . "',
	last_name='" . addslashes(trim($updatedContact->contact_last_name)) . "',
	called_name='" . addslashes(trim($updatedContact->contact_called_name)) . "',
	middle_name='" . addslashes(trim($updatedContact->contact_middle_name)) . "',
	email='" . addslashes(trim($updatedContact->contact_email)) . "',
	phone='" . addslashes(trim($updatedContact->contact_phone)) . "',
	pager='" . addslashes(trim($updatedContact->contact_pager)) . "',
	cellphone='" . addslashes(trim($updatedContact->contact_cellphone)) . "',
	fax='" . addslashes(trim($updatedContact->contact_fax)) . "',
	street_address_1='" . addslashes(trim($updatedContact->contact_street_address_1)) . "',
	street_address_2='" . addslashes(trim($updatedContact->contact_street_address_2)) . "',
	street_address_3='" . addslashes(trim($updatedContact->contact_street_address_3)) . "',
	city='" . addslashes(trim($updatedContact->contact_city)) . "',
	region_id='" . $updatedContact->contact_region_id . "',
	provstate_id='" . $updatedContact->contact_provstate_id . "',
	country_id='" . $updatedContact->contact_country_id . "',
	postal_code='" . addslashes(trim($updatedContact->contact_postal_code)) . "',
	location_info='" . addslashes(trim($updatedContact->contact_location_info)) . "'
	WHERE contact_id='" . addslashes($matching_contact_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

if ($action == 'do_contact_next' || $action == 'ignoreCompany_updateContact')
{
	$sql = ("
		UPDATE contact_employer
		SET employer_id='" . addslashes($new_employer_id) . "'
		WHERE contact_id='" . addslashes($matching_contact_id) . "'
		");
}
//:TODO: This is a problematic query because it creates a contact with no division. That causes problems
//       because we assume that every contact has a division. 
else
{
	$sql = ("
		UPDATE contact_employer
		SET employer_id='" . addslashes($new_employer_id) . "',
		department_id=''
		WHERE contact_id='" . addslashes($matching_contact_id) . "'
		");
}
$result = $GLOBALS['dbh']->Execute($sql);

if ($recordedChanges)
{
	/*
	 Lastly, we need to set the change reasons, dates, and by who.  Do this by first bumping the first
	 and second reasons down one, and then inserting the most recent reason, date and who into the database.
	*/

	$sql = ("
		SELECT DISTINCT changes_recorded_1, changes_recorded_2, change_date_1, change_date_2, change_by_1, change_by_2
		FROM contact_employer
		WHERE contact_id='" . addslashes($matching_contact_id) . "'
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
		UPDATE contact_employer
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
		WHERE department_id='" . addslashes($matching_contact_id) . "'
		");

	$result = $GLOBALS['dbh']->Execute($sql);
}

// Delete all of the pending values.

$tempValues = new PendingEmployer('');
$tempValues->load_pending_values($job_id);

$sql = ("
	DELETE FROM pending_employer_company
	WHERE employer_id='" . addslashes($tempValues->employer_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
	DELETE FROM pending_employer_contact
	WHERE contact_id='" . addslashes($tempValues->contact_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
	DELETE FROM pending_job_description
	WHERE job_id='" . addslashes($tempValues->job_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);

// Notify of success.

$sql = ("
	SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS contact_name, ec.company_name
	FROM contact AS c, employer_company AS ec, contact_employer AS ce
	WHERE c.contact_id='" . addslashes($matching_contact_id) . "'
	AND ce.contact_id='" . addslashes($matching_contact_id) . "'
	AND ec.employer_id=ce.employer_id
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

if ($action == 'ignoreCompany_updateContact' || $action == 'do_contact_next')
{
	$msg .= ("The information for <b>" . $row["contact_name"] . "</b> has been successfully updated, and the approval process");
	$msg .= (" has been finalized.");
}
else
{
	$msg .= ("The information for <b>" . $row["contact_name"] . "</b> has been successfully updated, and they have had their parent");
	$msg .= (" company updated to <b>" . $row["company_name"] . "</b>.");
	
	$msg .= ("<br /><br />If you would like to generate a login ID and password for <b>" . $row["contact_name"] . "</b> and have it e-mailed to them.");
	$msg .= ("  Please click the button below.<br /><br />");
	
	$msg .= ("
	        <center>
	        <table cellspacing=2 cellpadding=2>
	        <tr>
	                <td>
	                <form method='post' action='$PHP_SELF'>
	                <input type='hidden' name='select' value='add_contact' />
	                <input type='hidden' name='level1' value='contact' />
	                <input type='hidden' name='contact_id' value='" . $new_contact_id . "' />
	                <input type='hidden' name='employer_id' value='" . $matching_employer_id . "' />
	                <input type='hidden' name='department_id' value='none' />
	                <input type='hidden' name='continue' value='email_contact_lp' />
	                <input type='submit' value='Generate an E-mail Login/Password' />
	                </form>
	                </td>
	        </tr>
	        </table>
	        </center>
	        ");
}

notify($msg);

?>
