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
 | Filename: edit_department_header.inc                                         |
 +------------------------------------------------------------------------------+
 | Description: This is the header file for editing a division.  It opens and   |
 | closes the table that the rest of the information is displayed in, and it    |
 | determines whether or not we need to give users a choice of                  |
 | companies/divisions to view and choose from, or to display the division's    |
 | information to be edited.                                                    |
 +------------------------------------------------------------------------------+

*/


echo("<table border='0' cellspacing='0' cellpadding='2' width='97%'>");
echo("<tr>");
        echo("<td>");

	if (!$department_id && $continue != 'merge_divisions_chosen' && $continue != 'merge_information_chosen')
	{
		// They haven't chosen a company and division yet, so give them the list to choose from.
        echo("<h3 align='left'>Employer Information - Edit Division</h3>");
		include('contact/edit/department/company_department_list.inc');
	}
	else
	{
		/*
		 They've chosen a division to edit, so show them the info and let them edit away.  Be
		 sure to pull the division name from the database.  
		*/
		
		$sql = ("
			SELECT DISTINCT department_name
			FROM employer_department
			WHERE department_id='" . $department_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();

		echo("<table border='0' width='100%'>");
		echo("<tr>");
		echo("<td align='left'>");

		echo("<h3 align='left'>Edit Division: " . $row["department_name"] . "</h3>");

		echo("</td>");

		// Quickmenu

		/* 
         This part below is annoying.  We have to properly set the contact class with each of the variables.  This is probably no longer necessary,
         since we no longer need to have a PDF HTMLCompany instance created at the start of the chain, so remove if possible later.
        */

		if ($department_note_ids && is_string($department_note_ids))
		{
			$department_note_ids = unpackObject(($department_note_ids));
		}

		if (!$add_confirmed)
		{
			$contact = new EditContact('');
			$contact->set_department_values($employer_id, $department_name, $department_website, $department_description,
			$department_location_info, $department_street_address_1, $department_street_address_2, $department_street_address_3,
			$department_city, $department_region_id, $department_provstate_id, $department_country_id, $department_postal_code,
			$department_phone, $department_fax, $department_email, $department_flags, $department_industry_id, $department_size_id,
			$department_department_comments, $department_name_legitimate, $department_status, $department_new_note, $department_note_ids,
			$department_delete_note_ids);
		}
		else
		{
			$contact = unpackObject($contact);
		}

		if ($continue != 'merge_divisions_chosen' && $continue != 'merge_information_chosen')
		{
			// Include the quickmenu
            echo("<td align='right'>");
                include('contact/division_quickmenu.inc');
            echo("</td>");
		}
			echo("</tr>");
			echo("</table>");

 		echo("</h3>");
		include('contact/edit/department/edit_department.inc');
	}

        echo("</td>");
echo("</tr>");
echo("</table>");

?>
