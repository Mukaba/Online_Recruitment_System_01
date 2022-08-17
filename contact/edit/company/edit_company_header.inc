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
 | Filename: edit_company_header.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This is the header file for editing a company.  It opens and    |
 | closes the table that the rest of the information is displayed in, and it    |
 | determines whether or not we need to give users a choice of companies to     |
 | view and choose from, or to display the company's information to be edited.  |
 +------------------------------------------------------------------------------+

*/


echo("<table border='0' cellspacing='0' cellpadding='2' width='97%'>");
echo("<tr>");
        echo("<td>");

	if (!$employer_id && $continue != 'merge_company_with_another' && $continue != 'merge_companies_chosen' && $continue != 'merge_information_chosen')
	{
		// They haven't chosen a company yet, so give them the list to choose from.
        echo("<h3 align='left'>Employer Information - Edit Company</h3>");
		include('contact/edit/company/company_list.inc');
	}
	else
	{
		
		/*
		 They've chosen a company to edit, so show them the info and let them edit away.  Be
		 sure to pull the company name from the database.  
		*/
		
		$sql = ("
			SELECT DISTINCT company_name
			FROM employer_company
			WHERE employer_id='" . $employer_id . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);
		$row = $result->FetchRow();

		echo("<table border='0' width='100%'>");
		echo("<tr>");
		echo("<td align='left'>");

		echo("<h3 align='left'>Edit Company: " . $row["company_name"] . "</h3>");

		echo("</td>");

		/* 
         This part below is annoying.  We have to properly set the contact class with each of the variables.  This is probably no longer necessary,
         since we no longer need to have a PDF HTMLCompany instance created at the start of the chain, so remove if possible later.
        */

		if ($company_note_ids && is_string($company_note_ids))
		{
			$company_note_ids = unpackObject(($company_note_ids));
		}

		if (!$add_confirmed)
		{
			$contact = new EditContact('');
			$contact->set_company_values($company_name, $company_website, $company_description, $company_street_address_1, 
			$company_street_address_2, $company_street_address_3, $company_city, $company_region_id, $company_provstate_id, 
			$company_country_id, $company_postal_code, $company_phone, $company_fax, $company_email, $company_flags, $company_industry_id,
			$company_size_id, $company_type_id, $company_department_comments, $company_name_legitimate, $company_status, $company_new_note, 
			$company_note_ids, $company_delete_note_ids);
		}
		else
		{
			$contact = unpackObject($contact);
		}

		if ($continue != 'merge_company_with_another' && $continue != 'merge_information_chosen' && $continue != 'merge_companies_chosen')
		{
			// Include the quickmenu
            echo("<td align='right'>");
                include('contact/company_quickmenu.inc');
            echo("</td>");
		}

        echo("</tr>");
        echo("</table>");

		include('contact/edit/company/edit_company.inc');
	}

        echo("</td>");
echo("</tr>");
echo("</table>");

?>
