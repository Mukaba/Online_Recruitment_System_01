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
 | Filename: save_company.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file is called to move all of the company inputs from      |
 | the $contact variable into the database.  Assertions must be made to ensure  |
 | that the userlevel is either OFFICE or ADMINISTRATOR and that the $contact   |
 | variable is set.                                                             |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
{
    assert(isset($contact));

    $error = 0;

    $sql = ("
            SELECT DISTINCT company_name, employer_id
            FROM employer_company
            WHERE company_name='" . addslashes($contact->company_name) . "'
            AND company_display = '1'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {
        $error = 1;
        $row = $result->FetchRow();
        $msg .= ("<b class='instruct'>".$row['company_name']."</b>");
        $msg .= (" is already entered in the database.");
        $msg .= ("<br /><br />Would you like to edit this company?<br /><br />");

        echo("<br />");
        echo("<table border='0' width='96%'>");
        echo("<tr>");
        echo("<td>");
        promptEditCompany($msg, $row["employer_id"]);
        echo("</td>");
        echo("</tr>");
        echo("</table>");
    }
    elseif (!$add_confirmed)
    {

        /*
         Check in the database for any companies that have similar names, and ask the user if they are
         sure they want to add THIS company.
         */

        $check_name = $contact->company_name[0] . $contact->company_name[1] . $contact->company_name[2];

        $sql = ("
                SELECT DISTINCT company_name
                FROM employer_company
                WHERE company_name LIKE '" . addslashes($check_name) . "%'
                AND company_display = '1'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            $error = 1;
            if ($result->RecordCount() == 1)
            {
                $row = $result->FetchRow();
                $msg .= (htmlentities($row["company_name"]));
                $msg .= (" is an exisiting company in the database with a name similar to");
                $msg .= (" the one you are entering.");
                $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
            }
            else
            {
                while ($row = $result->FetchRow())
                {
                    $company_names_array[] = $row["company_name"];
                }
                for ($i = 0; $i < (sizeof($company_names_array) - 1); $i++)
                {
                    $msg .= (htmlentities($company_names_array[$i]) . ", ");
                }
                $msg .= ("and " . htmlentities($company_names_array[$i]) . " ");
                $msg .= ("are companies with similar names to the one you are entering.");
                $msg .= ("<br /><br />Are you sure you want to add this company to the database?<br /><br />");
            }

            echo("<br />");
            echo("<table border='0' width='96%'>");
            echo("<tr>");
            echo("<td>");
            promptAddCompany($msg, $contact);
            echo("</td>");
            echo("</tr>");
            echo("</table>");
        }
    }
    elseif ($add_confirmed && is_string($contact))
    {
        $contact = unpackObject($contact);
    }

    if ($error == 0)
    {

        // Insert the new values into the employer_company database.
        $sql = ("
                INSERT INTO employer_company (
                    company_name, website, company_description, street_address_1, street_address_2, street_address_3, city,
                    region_id, provstate_id, country_id, postal_code, phone, fax, email, industry_id, size_id, entered_by, 
                    entered_on, company_display, company_type_id)
                VALUES (
                '" . addslashes($contact->company_name) . "',
                '" . addslashes($contact->company_website) . "',
                '" . addslashes($contact->company_description) . "',
                '" . addslashes($contact->company_street_address_1) . "',
                '" . addslashes($contact->company_street_address_2) . "',
                '" . addslashes($contact->company_street_address_3) . "',
                '" . addslashes($contact->company_city) . "',
                '" . addslashes($contact->company_region_id) . "',
                '" . addslashes($contact->company_provstate_id) . "',
                '" . addslashes($contact->company_country_id) . "',
                '" . addslashes($contact->company_postal_code) . "',
                '" . addslashes($contact->company_phone) . "',
                '" . addslashes($contact->company_fax) . "',
                '" . addslashes($contact->company_email) . "',
                '" . $contact->company_industry_id . "',
                '" . $contact->company_size_id . "',
                '" . addslashes($auth->login) . "',
                CURRENT_DATE,
                '1',
                '" . $contact->company_type_id . "'
                )
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $sql = ("
                SELECT LAST_INSERT_ID() as employer_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();
        $employer_id = $row["employer_id"];

        // Create a division under this company using the same information.
        $sql = ("
                INSERT INTO employer_department (
                    employer_id, department_name, department_website, department_description, street_address_1, street_address_2, 
                    street_address_3, city, region_id, provstate_id, country_id, postal_code, phone, fax, email, 
                    industry_id, size_id, entered_by, entered_on)
                VALUES (
                '" . addslashes($employer_id) . "',
                '" . addslashes($contact->company_name) . "',
                '" . addslashes($contact->company_website) . "',
                '" . addslashes($contact->company_description) . "',
                '" . addslashes($contact->company_street_address_1) . "',
                '" . addslashes($contact->company_street_address_2) . "',
                '" . addslashes($contact->company_street_address_3) . "',
                '" . addslashes($contact->company_city) . "',
                '" . addslashes($contact->company_region_id) . "',
                '" . addslashes($contact->company_provstate_id) . "',
                '" . addslashes($contact->company_country_id) . "',
                '" . addslashes($contact->company_postal_code) . "',
                '" . addslashes($contact->company_phone) . "',
                '" . addslashes($contact->company_fax) . "',
                '" . addslashes($contact->company_email) . "',
                '" . $contact->company_industry_id . "',
                '" . $contact->company_size_id . "',
                '" . addslashes($auth->login) . "',
                CURRENT_DATE
                )
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $sql = ("
                SELECT LAST_INSERT_ID() as division_id
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $row = $result->FetchRow();
        $division_id = $row["division_id"];

        /*
         Below is not used at UVic

         if ($contact->company_department_comments)
         {

         $sql = ("
         INSERT INTO employer_company_comments
         (employer_company_id, department_id, comments)
         VALUES
         (
         '" . $employer_id . "',
         '" . $auth->department . "',
         '" . addslashes($contact->company_department_comments) . "'
         )
         ");
         $result = $GLOBALS['dbh']->Execute($sql);

         }
         */

        if ($contact->company_flags)
        {

            // Insert the company flags into the database.
            for ($i = 0; $i < sizeof($contact->company_flags); $i++)
            {
                $sql = ("
                        INSERT INTO company_flags_set
                        (employer_id, flag_id)
                        VALUES
                        (
                         '" . $employer_id . "',
                         '" . $contact->company_flags[$i] . "'
                        )
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

//:TODO Decide whether or not we want company flags to save as division flags too.
// Could be dangerous if the flags are not set up the same.
            // Insert the company flags as division flags into the division's table
/*
            for ($i = 0; $i < sizeof($contact->company_flags); $i++)
            {
                $sql = ("
                        INSERT INTO department_flags_set
                        (department_id, flag_id)
                        VALUES
                        (
                         '" . $employer_id . "',
                         '" . $contact->company_flags[$i] . "'
                        )
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
*/
        }

        // Save the company's status as CONTACT_POTENTIAL_LEAD for each department
        $sql = ("
            SELECT department_id
            FROM department
            WHERE job_list = '1'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        
        while ($row = $result->FetchRow())
        {
            updateDivisionStatus($division_id,$row['department_id']);
            updateCompanyStatus($employer_id,$row['department_id']);
        }

        // Save the company's notes
        if (trim($contact->company_new_note))
        {
            $sql = ("
                    INSERT INTO company_notes
                    (employer_id, notes, date_entered, entered_by)
                    VALUES (
                        '" . $employer_id . "',
                        '" . addslashes(trim($contact->company_new_note)) . "',
                        '" . date("Y-m-d H:i:s") . "',
                        '" . getContactID($auth->department, $auth->login) . "'
                        )
                    ");
            $GLOBALS['dbh']->Execute($sql);

            // Save the company's notes into the division notes
            $sql = ("
                    INSERT INTO division_notes
                    (department_id, notes, date_entered, entered_by)
                    VALUES (
                        '" . $division_id . "',
                        '" . addslashes(trim($contact->company_new_note)) . "',
                        '" . date("Y-m-d H:i:s") . "',
                        '" . getContactID($auth->department, $auth->login) . "'
                        )
                    ");
            $GLOBALS['dbh']->Execute($sql);
        }

        $no_buttons = true;

        /*
         The employer_id MUST be set to proceed to the next screen, because it allows the user to directly
         add a division to this company from the next screen.
         */

        include('contact/add/company/confirm_company_inputs.inc');

        //delete object "contact"
        unset($contact);
        unset($add_confirmed);
    }
}
else
{
    include('misc/loguser.inc');
}

?>
