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
 | Filename: save_updated_department.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: This file is used to transfer the newly updated division        |
 | inputs into the database.  This file needs to have $matching_employer_id,    |
 | $matching_department_id set as well as the EditContact class set with all    |
 | of the new inputs.                                                           |
 +------------------------------------------------------------------------------+

*/

$recordedChanges = recordContactChange($updatedDepartment, "department");

$sql = ("
	UPDATE employer_department
	SET department_website='" . addslashes($updatedDepartment->department_website) . "',
	department_description='" . addslashes($updatedDepartment->department_description) . "',
	street_address_1='" . addslashes($updatedDepartment->department_street_address_1) . "',
	street_address_2='" . addslashes($updatedDepartment->department_street_address_2) . "',
	street_address_3='" . addslashes($updatedDepartment->department_street_address_3) . "',
	city='" . addslashes($updatedDepartment->department_city) . "',
	region_id='" . addslashes($updatedDepartment->department_region_id) . "',
	provstate_id='" . addslashes($updatedDepartment->department_provstate_id) . "',
	country_id='" . addslashes($updatedDepartment->department_country_id) . "',
	postal_code='" . addslashes($updatedDepartment->department_postal_code) . "',
	phone='" . addslashes($updatedDepartment->department_phone) . "',
	fax='" . addslashes($updatedDepartment->department_fax) . "',
	email='" . addslashes($updatedDepartment->department_email) . "',
	industry_id='" . addslashes($updatedDepartment->department_industry_id) . "',
	size_id='" . addslashes($updatedDepartment->department_size_id) . "',
	location_info='" . addslashes($updatedDepartment->department_location_info) . "'
	WHERE department_id='" . addslashes($matching_department_id) . "'
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
		FROM employer_department
		WHERE department_id='" . addslashes($matching_department_id) . "'
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
		UPDATE employer_department
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
		WHERE department_id='" . addslashes($matching_department_id) . "'
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
	SELECT DISTINCT department_name
	FROM employer_department
	WHERE department_id='" . addslashes($matching_department_id) . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$msg .= ("The information for <b>" . $row["department_name"] . "</b> has been successfully updated.");

if (!$action)
{
	$sql = ("
		SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS contact_name
		FROM contact
		WHERE contact_id='" . addslashes($new_contact_id) . "'
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	
	$msg .= ("  If you would like to generate a login ID and password for <b>" . $row["contact_name"] . "</b> and have it e-mailed to them.");
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
