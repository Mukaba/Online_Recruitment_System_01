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
 | Filename: save_contact.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the contact inputs from      |
 | the $contact variable into the database.  Assertions must be made to ensure  |
 | that the userlevel is either OFFICE or ADMINISTRATOR and that the $contact   |
 | variable is set.                                                             |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
{
    assert(isset($contact));

    $error = 0;

    if (!$add_confirmed)
    {

        /*
         Check in the database for any contacts at this company/division with similar names, and ask the
         user if they are sure they want to add THIS contact.  First check if we have a matching phone number.
         If we do, then display all contacts that share the last initial as this one, and check to make sure
         the user wishes to add this contact.  If no matching phone number, then show the user all contacts
         with similar last names to this contact and prompt for confirmation.
         */

        $check_name = $contact->contact_last_name[0] . $contact->contact_last_name[1] . $contact->contact_last_name[2];
        $check_initial = $contact->contact_last_name[0];

        if (trim($contact->contact_phone) != '' && (trim($contact->contact_phone) != USE_COMPANY) && (trim($contact->contact_phone) != USE_DEPARTMENT))
        {
            $sql = ("
                    SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                    FROM contact AS a, employer_company AS b, contact_employer AS c
                    WHERE a.last_name LIKE '" . addslashes($check_initial) . "%'
                    AND a.phone='" . addslashes($contact->contact_phone) . "'
                    AND c.employer_id='" . $contact->contact_employer_id . "'
                    AND a.contact_id=c.contact_id
                    AND b.employer_id=c.employer_id
                    ");
            $result  = $GLOBALS['dbh']->Execute($sql);
        }

        if (!($result->RecordCount()) || (trim($contact->contact_phone) == '') || (trim($contact->contact_phone) == USE_COMPANY) || (trim($contact->contact_phone) == USE_DEPARTMENT))
        {
            $sql = ("
                    SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, b.company_name
                    FROM contact AS a, employer_company AS b, contact_employer AS c
                    WHERE a.last_name LIKE '" . addslashes($check_name) . "%'
                    AND c.employer_id='" . $contact->contact_employer_id . "'
                    AND a.contact_id=c.contact_id
                    AND b.employer_id=c.employer_id
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        if ($result->RecordCount())
        {
            $error = 1;
            if ($result->RecordCount() == 1)
            {
                $row = $result->FetchRow();
                $msg .= ("<b class=instruct>".$row['name']."</b>");
                $msg .= (" is an existing contact at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                $msg .= (" with a name similar to the one you are entering.");
            }
            else
            {
                while ($row = $result->FetchRow())
                {
                    $contact_names_array[] = $row["name"];
                }
                for ($i = 0; $i < (sizeof($contact_names_array) - 1); $i++)
                {
                    $msg .= ("<b class=instruct>".$contact_names_array[$i] . "</b><br />");
                }
                $msg .= ("<b class=instruct>".$contact_names_array[$i] . "</b><br /><br />");
                $msg .= ("are contacts at <b class=instruct>" . $row["company_name"] . "</b> already in the database ");
                $msg .= ("with similar names to the one you are entering.");
            }

        }

    }

    // Check if there are any contacts with the same name at other companies.
    $sql = ("
            SELECT DISTINCT a.company_name
            FROM employer_company AS a, contact AS b, contact_employer AS c
            WHERE b.first_name='" . addslashes($contact->contact_first_name) . "'
            AND b.last_name='" . addslashes($contact->contact_last_name) . "'
            AND c.contact_id=b.contact_id
            AND a.employer_id=c.employer_id
            AND a.employer_id!='" . $contact->contact_employer_id . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() == 1)
    {
        $row = $result->FetchRow();

        $warn .= ("NOTE: <b class=instruct>" . $contact->contact_first_name . " " . $contact->contact_last_name."</b>");
        $warn .= (" is also a contact at: <br /><br /><b class=instruct>" . $row['company_name']."</b>");
    }
    elseif ($result->RecordCount() > 1)
    {
        $warn .= ("NOTE: <b class=instruct>" . $contact->contact_first_name . " " . $contact->contact_last_name);
        $warn .= ("</b> is also a contact at: <br /><br /><b class=instruct>");

        while ($row = $result->FetchRow())
        {
            $warn .= ($row["company_name"] . "</b><br />");
        }
    }

    if ($msg)
    {
        /*
         Similar names were detected, so display an error message.  If we noted the same
         contact name but with a different company, then include this in the warning.
         */

        if ($warn)
        {
            $msg .= ("<br /><br />" . $warn);
        }

        $msg .= ("<br /><br />Are you sure you want to add this contact to the database?<br /><br />");
        echo("<br />");
        echo("<table border='0' width='96%'>");
        echo("<tr>");
        echo("<td>");
        promptAddContact($msg, $contact);
        echo("</td>");
        echo("</tr>");
        echo("</table>");
    }

    elseif ($add_confirmed && is_string($contact))
    {
        // They've already seen the warning if they're here, so reset it.

        unset($warn);
        $contact = unpackObject($contact);
    }

    if ($error == 0)
    {

        if ($warn)
        {
            notify($warn);
        }

        // Insert the new values into the employer_contact database.

        /*
         If there was no division specified for the contact, then the department_id will be 'none'.
         Switch this to a null value.
         */

        if ($contact->contact_department_id == 'none')
        {
            $contact->contact_department_id = '';
        }

        $sql = ("
                INSERT INTO contact
                (type, title, first_name, middle_name, last_name,
                 called_name, greeting, email, phone, pager,
                 cellphone, fax, street_address_1, street_address_2, street_address_3,
                 city, country_id, provstate_id, region_id, postal_code, location_info)
                VALUES
                (
                 'employer',
                 '" . addslashes($contact->contact_title) . "',
                 '" . addslashes($contact->contact_first_name) . "',
                 '" . addslashes($contact->contact_middle_name) . "',
                 '" . addslashes($contact->contact_last_name) . "',
                 '" . addslashes($contact->contact_called_name) . "',
                 '" . addslashes($contact->contact_greeting) . "',
                 '" . addslashes($contact->contact_email) . "',
                 '" . addslashes($contact->contact_phone) . "',
                 '" . addslashes($contact->contact_pager) . "',
                 '" . addslashes($contact->contact_cellphone) . "',
                 '" . addslashes($contact->contact_fax) . "',
                 '" . addslashes($contact->contact_street_address_1) . "',
                 '" . addslashes($contact->contact_street_address_2) . "',
                 '" . addslashes($contact->contact_street_address_3) . "',
                 '" . addslashes($contact->contact_city) . "',
                 '" . $contact->contact_country_id . "',
                 '" . $contact->contact_provstate_id . "',
                 '" . $contact->contact_region_id . "',
                 '" . addslashes($contact->contact_postal_code) . "',
                 '" . addslashes($contact->contact_location_info) . "'
                )
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $sql = ("
                SELECT LAST_INSERT_ID() as contact_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();
        $contact_id = $row["contact_id"];

        $sql = ("
                INSERT INTO contact_employer
                (contact_id, employer_id, department_id, department_name, 
                 position, entered_on, entered_by)
                VALUES
                (
                 '" . $contact_id . "',
                 '" . $contact->contact_employer_id . "',
                 '" . $contact->contact_department_id . "',
                 '" . addslashes(trim($contact->contact_department_name)) . "',
                 '" . addslashes(trim($contact->contact_position_title)) . "',
                 CURRENT_DATE,
                 '" . addslashes($auth->login) . "'
                )
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        // Insert the flags into the databsae.

        for ($i = 0; $i < sizeof($contact->contact_flags); $i++)
        {
            $sql = ("
                    INSERT INTO contact_flags_set
                    (contact_id, flag_id)
                    VALUES
                    (
                     '" . $contact_id . "',
                     '" . $contact->contact_flags[$i] . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        // Insert the division specific comments into the database.

        if (trim($contact->contact_department_comments))
        {
            $sql = ("
                    INSERT INTO employer_contact_comments
                    (employer_contact_id, department_id, comments)
                    VALUES
                    (
                     '" . $contact_id . "',
                     '" . $auth->department . "',
                     '" . addslashes($contact->contact_department_comments) . "'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        // Save the contact's status as CONTACT_POTENTIAL_LEAD for each department
        // Also, check for the parent company and division and see if we need to update their status.
        $sql = ("
            SELECT department_id
            FROM department
            WHERE job_list = '1'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow())
        {
            // potential lead set
            updateContactStatus($contact_id, $row['department_id'], null, null);
        }

        // Save the contact's notes
        if (trim($contact->contact_new_note))
        {
            $sql = ("
                    INSERT INTO contact_actions_set
                    (
                     contact_id,
                     action_id,
                     action_on,
                     action_by,
                     action_note
                    )
                    VALUES
                    (
                     '".$contact_id."',
                     '".GENERIC_NOTE."',
                     '".date("Y-m-d H:i:s")."',
                     '".getContactID($auth->department, $auth->login)."',
                     '".addslashes(trim($contact->contact_new_note))."'
                    )
                    ");

            $GLOBALS['dbh']->Execute($sql);
        }

        $no_buttons = true;
        include('contact/add/contact/confirm_contact_inputs.inc');

        //delete object "contact"
        unset($contact);
    }
}
?>
