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
 | Filename: save_company_info.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the company inputs from      |
 | the $contact variable into the database.  Assertions must be made to ensure  |
 | that the userlevel is either OFFICE or ADMINISTRATOR and that the $contact   |
 | variable is set. NOTE: This file will need a more thorough method for        |
 | checking if we are adding a duplicate company, but for right now, this       |
 | prototype will do.                                                           |
 +------------------------------------------------------------------------------+

*/

assert(isset($newEmployer));

$error = 0;

if ($error == 0)
{

	/*
	 Insert the new values into the employer_company database.
	*/

	$sql = ("
		INSERT INTO pending_employer_company (
		company_name, company_website, company_description,
		street_address_1, street_address_2, street_address_3, city, region_id, 
		provstate_id, country_id, postal_code, phone, fax, 
		email, department_name, industry_id, size_id, entered_on, company_type_id
		)
		VALUES
		(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NOW(), ?)
        ");
    $stmt = $GLOBALS['dbh']->Prepare($sql);
    $result = $GLOBALS['dbh']->Execute($stmt,
            array($newEmployer->company_name,
                $newEmployer->company_website,
                $newEmployer->company_description,
                $newEmployer->company_street_address_1,
                $newEmployer->company_street_address_2,
                $newEmployer->company_street_address_3,
                $newEmployer->company_city,
                $newEmployer->company_region_id,
                $newEmployer->company_provstate_id,
                $newEmployer->company_country_id,
                $newEmployer->company_postal_code,
                $newEmployer->company_phone,
                $newEmployer->company_fax,
                $newEmployer->company_email,
                $newEmployer->company_department_name,
                $newEmployer->company_industry_id,
                $newEmployer->company_size_id,
                $newEmployer->company_type_id));

    $sql = ("
            SELECT LAST_INSERT_ID() as employer_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    $row = $result->FetchRow();
    $newEmployer->employer_id = $row["employer_id"];

    // Save the department name in the pending_employer_contact table

    $sql = ("
            UPDATE pending_employer_contact
            SET department_name = ?, employer_id = ?
            WHERE contact_id = ?
            ");
    $stmt = $GLOBALS['dbh']->Prepare($sql);
    $result = $GLOBALS['dbh']->Execute($stmt,
            array($newEmployer->company_department_name,
                $newEmployer->employer_id,
                $newEmployer->contact_id));

    if (!$form_name)
    {
        $form_name = 'Job Description Form';
    }

    $msg .= ("The Company information you entered has been successfully saved.  Please fill out the $form_name below.");

    instruct($msg);
}

?>
