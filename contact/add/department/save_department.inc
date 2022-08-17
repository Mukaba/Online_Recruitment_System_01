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
 | Filename: save_department.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the division inputs from     |
 | the $contact variable into the database.  Assertions must be made to ensure  |
 | that the userlevel is either OFFICE or ADMINISTRATOR and that the $contact   |
 | variable is set. NOTE: This file will need a more thorough method for        |
 | checking if we are adding a duplicate division, but for right now, this      |
 | prototype will do.                                                           |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
{
	assert(isset($contact));

	$error = 0;
	
	if (!$add_confirmed)
	{
		$sql = ("
			SELECT DISTINCT a.department_name, a.department_id, b.company_name
			FROM employer_department AS a, employer_company AS b
			WHERE a.department_name='" . addslashes($contact->department_name) . "'
			AND a.employer_id='" . $contact->department_company_id . "'
			AND b.employer_id=a.employer_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
	
		if ($result->RecordCount())
		{
			$error = 1;
			$row = $result->FetchRow();
			$msg .= ("<b class='instruct'>".$row['department_name']."</b>");
			$msg .= (" at " . $row["company_name"] . " already exists in the database.");
			$msg .= ("<br /><br />Would you like to edit this division?<br /><br />");
	
			echo("<br />");
	                echo("<table border='0' width='96%'>");
	                echo("<tr>");
	                echo("<td>");
			promptEditDepartment($msg, $row["department_id"]);
	                echo("</td>");
	                echo("</tr>");
	                echo("</table>");
		}
		elseif (!$add_confirmed && is_string($contact))
		{
			$contact = unpackObject($contact);
		}
	}
	
	if ($error == 0)
	{
		
		// Insert the new values into the employer_department database.
		$sql = ("
			INSERT INTO employer_department (
                employer_id, department_name, department_website, department_description, street_address_1, 
                street_address_2, street_address_3, city, region_id, provstate_id, country_id, postal_code, phone, fax, email, 
                industry_id, size_id, entered_by, entered_on, location_info)
			VALUES
			(
			'" . $contact->department_company_id . "',
			'" . addslashes($contact->department_name) . "',
			'" . addslashes($contact->department_website) . "',
			'" . addslashes($contact->department_description) . "',
			'" . addslashes($contact->department_street_address_1) . "',
			'" . addslashes($contact->department_street_address_2) . "',
			'" . addslashes($contact->department_street_address_3) . "',
			'" . addslashes($contact->department_city) . "',
			'" . $contact->department_region_id . "',
			'" . $contact->department_provstate_id . "',
			'" . $contact->department_country_id . "',
			'" . addslashes($contact->department_postal_code) . "',
			'" . addslashes($contact->department_phone) . "',
			'" . addslashes($contact->department_fax) . "',
			'" . addslashes($contact->department_email) . "',
			'" . $contact->department_industry_id . "',
			'" . $contact->department_size_id . "',
			'" . addslashes($auth->login) . "',
			CURRENT_DATE,
			'" . addslashes($contact->department_location_info) . "'
			)
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		$sql = ("
			SELECT LAST_INSERT_ID() as employer_department_id
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		
		$row = $result->FetchRow();
		$employer_department_id = $row["employer_department_id"];
	
		if ($contact->department_flags)
		{

			/*
			 Add the flags into the database.
			*/
		
			for ($i = 0; $i < sizeof($contact->department_flags); $i++)
			{
				$sql = ("
					INSERT INTO department_flags_set
					(department_id, flag_id)
					VALUES
					(
					'" . $employer_department_id . "',
					'" . $contact->department_flags[$i] . "'
					)
					");
				$result = $GLOBALS['dbh']->Execute($sql);
			}
		}

		// Insert the specific comments into the database.
		$sql = ("
			INSERT INTO employer_department_comments
			(employer_department_id, department_id, comments)
			VALUES
			(
			'" . $employer_department_id . "',
			'" . $auth->department . "',
			'" . addslashes($contact->department_department_comments) . "'
			)
			");
		$result = $GLOBALS['dbh']->Execute($sql);

        // Save the division's status as CONTACT_POTENTIAL_LEAD for each department
        $sql = ("
            SELECT department_id
            FROM department
            WHERE job_list = '1'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        
        while ($row = $result->FetchRow())
        {
            updateDivisionStatus($employer_department_id,$row['department_id']);
            updateCompanyStatus($contact->department_company_id,$row['department_id']);
        }

		// Save the department's notes
        if (trim($contact->department_new_note))
        {
            $sql = ("
                INSERT INTO division_notes
                (department_id, notes, date_entered, entered_by)
                VALUES (
                '" . $employer_department_id . "',
                '" . addslashes(trim($contact->department_new_note)) . "',
                '" . date("Y-m-d H:i:s") . "',
                '" . getContactID($auth->department, $auth->login) . "'
                )
                ");
            $GLOBALS['dbh']->Execute($sql);
        }

		/*
		 Set the variable department_id, so that the next screen can let users directly add a contact to
		 this division.
		*/
	
		$department_id = $employer_department_id;
	        $no_buttons = true;
	
	        include('contact/add/department/confirm_department_inputs.inc');

        	//delete object "contact"
        	unset($contact);
		unset($add_confirmed);
	}
}

?>
