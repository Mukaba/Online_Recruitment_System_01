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
 | Filename: edit_contact_header.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This is the header file for editing a contact.  It opens and    |
 | closes the table that the rest of the information is displayed in, and it    |
 | determines whether or not we need to give users a choice of                  |
 | companies/divisions/contacts to view and choose from, or to display the      |
 | contact's information to be edited.                                          |
 +------------------------------------------------------------------------------+

*/


echo("<table border='0' cellspacing='0' cellpadding='2' width='97%'>");
echo("<tr>");
    echo("<td>");

    if (!$contact_id && $continue != 'merge_contacts_chosen' && $continue != 'merge_information_chosen' && $continue != 'merge_contact_with_another'
    && $continue != 'chosen_job_contact' && $continue != 'choose_contact_after_move')
    {
        // They haven't chosen a company, division and contact yet, so give them the list to choose from.
        echo("<h3 align='left'>Employer Information - Edit Contact</h3>");

        if ($choose_contact == 'Choose Contact Directly')
        {
            include('contact/edit/contact/contact_list.inc');
        }
        else
        {
            include('contact/edit/contact/company_department_list.inc');
        }
    }
    else
    {
        /*
         They've chosen a contact to edit, so show them the info and let them edit away.  Be
         sure to pull the contact name from the database.  
        */

        /*
         If they chose the contact directly, they will not have an employer_id or department_id
         assigned yet.  Assign these variables.
        */

        if ($choose_contact == 'Choose Contact Directly')
        {
            $sql = ("
                    SELECT DISTINCT employer_id, department_id
                    FROM contact_employer
                    WHERE contact_id='" . $contact_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row = $result->FetchRow();

            $employer_id = $row["employer_id"];

            if ($row["department_id"])
            {
                $department_id = $row["department_id"];
            }
            else
            {
                $department_id = 'none';
            }
        }

        $sql = ("
                SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS contact_name
                FROM contact
                WHERE contact_id='" . $contact_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        echo("<table border='0' width='100%'>");
        echo("<tr>");
        echo("<td align='left'>");

        echo("<h3>Edit Contact: " . $row["contact_name"]);
        echo("</h3>");
        echo("</td>");

        // Quickmenu

		/* 
         This part below is annoying.  We have to properly set the contact class with each of the variables.  This is probably no longer necessary,
         since we no longer need to have a PDF HTMLCompany instance created at the start of the chain, so remove if possible later.
        */
        if ($contact_note_ids && is_string($contact_note_ids))
        {
            $contact_note_ids = unpackObject(($contact_note_ids));
        }

        if (!($add_confirmed || $global_status_change))
        {
            $contact = new EditContact('');
            $contact->set_contact_values($employer_id, $department_id, $contact_title, $contact_first_name, $contact_middle_name, 
                $contact_last_name, $contact_called_name, $contact_greeting, $contact_email, $contact_phone, $contact_pager, 
                $contact_cellphone, $contact_fax, $contact_position_title, $contact_department_name, $contact_location_info, 
                $contact_street_address_1, $contact_street_address_2, $contact_street_address_3, $contact_city, $contact_region_id, 
                $contact_provstate_id, $contact_country_id, $contact_postal_code, $contact_department_comments, 
                $contact_flags, $contact_new_note, $contact_note_ids, $contact_delete_note_ids);
        }
        else
        {
            $contact = unpackObject($contact);
        }

        if ($continue != 'merge_contacts_chosen' && $continue != 'merge_information_chosen' && $continue != 'chosen_job_contact')
        {
            // Include the quickmenu
            echo("<td align='right'>");
                include('contact/contact_quickmenu.inc');
            echo("</td>");
        }
        echo("</tr>");
        echo("</table>");

       // echo("</h3>");
        include('contact/edit/contact/edit_contact.inc');
    }

    echo("</td>");
echo("</tr>");
echo("</table>");

?>
