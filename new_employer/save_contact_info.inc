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
 | Filename: save_contact_info.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the contact inputs from      |
 | the $newEmployer variable into the database.                                 |
 +------------------------------------------------------------------------------+

*/

assert(isset($newEmployer));

$error = 0;

if ($error == 0)
{

	// Insert the new values into the employer_company database.

	$sql = ("
		INSERT INTO pending_employer_contact (
		title, first_name, middle_name, last_name, called_name, email, phone, 
		pager, cellphone, fax, street_address_1, street_address_2, street_address_3, city, region_id,
		provstate_id, country_id, postal_code, entered_on, ip_added_with, module
		)
		VALUES
		(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?, ?)
        ");
    $stmt = $GLOBALS['dbh']->Prepare($sql);
    $GLOBALS['dbh']->Execute($stmt,
            array($newEmployer->contact_title,
                $newEmployer->contact_first_name,
                $newEmployer->contact_middle_name,
                $newEmployer->contact_last_name,
                $newEmployer->contact_called_name,
                $newEmployer->contact_email,
                $newEmployer->contact_phone,
                $newEmployer->contact_pager,
                $newEmployer->contact_cellphone,
                $newEmployer->contact_fax,
                $newEmployer->contact_street_address_1,
                $newEmployer->contact_street_address_2,
                $newEmployer->contact_street_address_3,
                $newEmployer->contact_city,
                $newEmployer->contact_region_id,
                $newEmployer->contact_provstate_id,
                $newEmployer->contact_country_id,
                $newEmployer->contact_postal_code,
                $REMOTE_ADDR,
                ($module_to_save == 'eval' ? "eval" : "job")));

	$sql = ("
		SELECT LAST_INSERT_ID() as contact_id
		");
	$result = $GLOBALS['dbh']->Execute($sql);
	
	$row = $result->FetchRow();
	$newEmployer->contact_id = $row["contact_id"];

	$msg .= ("The Contact information you entered has been successfully saved.  Please fill out the Company Information Form below.");

	instruct($msg);
}

?>
