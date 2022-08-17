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
 | Description: This file is used to save the changed inputs into the           |
 | database from the contact object.                                            |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
{
    assert(isset($contact));
    $error = 0;

    $sql = ("
        SELECT DISTINCT first_name, last_name
        FROM contact
        WHERE contact_id='" . $contact_id . "'
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

    if (!$add_confirmed && ($row["first_name"] != $contact->contact_first_name || $row["last_name"] != $contact->contact_last_name))
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
                AND a.contact_id!='" . $contact_id . "'
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
                AND a.contact_id!='" . $contact_id . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        if ($result->RecordCount())
        {
            $error = 1;
            if ($result->RecordCount() == 1)
            {
                $row = $result->FetchRow();
                $msg .= ($row["name"]);
                $msg .= (" is an existing contact at " . $row["company_name"] . " already in the database ");
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
                    $msg .= ($contact_names_array[$i] . "<br />");
                }
                $msg .= ($contact_names_array[$i] . "<br /><br />");
                $msg .= ("are contacts at " . $row["company_name"] . " already in the database ");
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

        $warn .= ("NOTE: " . $contact->contact_first_name . " " . $contact->contact_last_name);
        $warn .= (" is also a contact at: <br /><br />" . $row["company_name"]);
    }
    elseif ($result->RecordCount() > 1)
    {
        $warn .= ("NOTE: " . $contact->contact_first_name . " " . $contact->contact_last_name);
        $warn .= (" is also a contact at: <br /><br />");

        while ($row = $result->FetchRow())
        {
            $warn .= ($row["company_name"] . "<br />");
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
            $msg .= ("<br /><br />Are you sure you want to save the changes to this contact?<br /><br />");
            promptSaveContactChanges($msg, $contact, $employer_id, $department_id, $contact_id);
        }
        else
        {
            $msg .= ("<br /><br />Are you sure you want to save the changes to this contact?<br /><br />");
            promptSaveContactChanges($msg, $contact, $employer_id, $department_id, $contact_id);
        }
    }

    elseif ($add_confirmed && is_string($contact))
    {
        /*
         They've already seen the warning if they're here, so reset it.
         */

        unset($warn);
        $contact = unpackObject($contact);
    }

    if ($error == 0)
    {

        $contact->contact_id = $contact_id;

        $recordedChanges = recordContactChange($contact, "contact");

        if ($warn)
        {
            error($warn);
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

        // Now save the updated values into the database.

        $sql = ("
            UPDATE contact
            SET title='" . addslashes(trim($contact->contact_title)) . "',
            first_name='" . addslashes(trim($contact->contact_first_name)) . "',
            last_name='" . addslashes(trim($contact->contact_last_name)) . "',
            called_name='" . addslashes(trim($contact->contact_called_name)) . "',
            middle_name='" . addslashes(trim($contact->contact_middle_name)) . "',
            greeting='" . addslashes(trim($contact->contact_greeting)) . "',
            email='" . addslashes(trim($contact->contact_email)) . "',
            phone='" . addslashes(trim($contact->contact_phone)) . "',
            pager='" . addslashes(trim($contact->contact_pager)) . "',
            cellphone='" . addslashes(trim($contact->contact_cellphone)) . "',
            fax='" . addslashes(trim($contact->contact_fax)) . "',
            street_address_1='" . addslashes(trim($contact->contact_street_address_1)) . "',
            street_address_2='" . addslashes(trim($contact->contact_street_address_2)) . "',
            street_address_3='" . addslashes(trim($contact->contact_street_address_3)) . "',
            city='" . addslashes(trim($contact->contact_city)) . "',
            region_id='" . $contact->contact_region_id . "',
            provstate_id='" . $contact->contact_provstate_id . "',
            country_id='" . $contact->contact_country_id . "',
            postal_code='" . addslashes(trim($contact->contact_postal_code)) . "',
            location_info='" . addslashes(trim($contact->contact_location_info)) . "'
            WHERE contact_id='" . $contact_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        $sql = ("
            UPDATE contact_employer
            SET employer_id='" . $employer_id . "', 
            department_id='" . $department_id . "',
            position='" . addslashes(trim($contact->contact_position_title)) . "',
            department_name='" . addslashes(trim($contact->contact_department_name)) . "'
            WHERE contact_id='" . $contact_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);


        /*
         Next, check if we already have department specific comments in the database.
         If so, we need to update or delete them, and if not we might need to insert
         them.
         */

        $sql = ("
            SELECT DISTINCT comments
            FROM employer_contact_comments
            WHERE employer_contact_id='" . $contact_id . "'
            AND department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            if (trim($contact->contact_department_comments))
            {
                $sql = ("
                    UPDATE employer_contact_comments
                    SET comments='" . addslashes($contact->contact_department_comments) . "'
                    WHERE employer_contact_id='" . $contact_id . "'
                    AND department_id='" . $auth->department . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
            else
            {
                $sql = ("
                    DELETE FROM employer_contact_comments
                    WHERE employer_contact_id='" . $contact_id . "'
                    AND department_id='" . $auth->department . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }
        elseif (trim($contact->contact_department_comments))
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

        /*
         Check if we already have contact flags in the database, and if so, update them.
         Otherwise, insert them.
         */

        // First get a list of the flags that this co-op department can set.

        $sql = ("
            SELECT DISTINCT flag_id
            FROM department_contact_flags
            WHERE department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $dep_flags[] = $row["flag_id"];
        }

        // Now set an array that contains all of the flags that have been set as off for this contact.

        for ($i = 0; $i < sizeof($dep_flags); $i++)
        {
            if (!(is_array($contact->contact_flags)) || !(in_array($dep_flags[$i], $contact->contact_flags)))
            {
                $delete_flags[] = $dep_flags[$i];
            }
        }

        /*
         Now set an array that contains all of the flags that have been set as on for this contact,
         and that are not already in the database.
         */

        $sql = ("
            SELECT DISTINCT flag_id
            FROM contact_flags_set
            WHERE contact_id='" . $contact_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $set_flags[] = $row["flag_id"];
        }

        for ($i = 0; $i < sizeof($contact->contact_flags); $i++)
        {
            if ((!($set_flags) || !(in_array($contact->contact_flags[$i], $set_flags))) && is_array($contact->contact_flags))
            {
                $insert_flags[] = $contact->contact_flags[$i];
            }
        }

        /*
         We now have two arrays.  $delete_flags holds all of the flags that need to be deleted from the
         contact_flags_set table, and $insert_flags holds all of the flags that need to be inserted
         into this table.
         */

        if ($delete_flags)
        {
            // Delete from the table the flags that have been set to off by the user.

            for ($i = 0; $i < sizeof($delete_flags); $i++)
            {
                $sql = ("
                    DELETE FROM contact_flags_set
                    WHERE flag_id='" . $delete_flags[$i] . "'
                    AND contact_id='" . $contact_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }

        if ($insert_flags)
        {

            // Insert all of the contact flags that have been set to on into the database.

            for ($i = 0; $i < sizeof($insert_flags); $i++)
            {
                $sql = ("
                    INSERT INTO contact_flags_set
                    (contact_id, flag_id)
                    VALUES
                    (
                     '" . $contact_id . "',
                     '" . $insert_flags[$i] . "'
                    )
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

        }

        if ($recordedChanges)
        {
            /*
             Lastly, we need to set the change reasons, dates, and by who.  Do this by first bumping the first
             and second reasons down one, and then inserting the most recent reason, date and who into the database.
             */

            $sql = ("
                SELECT DISTINCT changes_recorded_1, changes_recorded_2, 
                change_date_1, change_date_2, change_by_1, change_by_2
                FROM contact_employer
                WHERE contact_id='" . $contact_id . "'
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
                WHERE contact_id='" . $contact_id . "'
                ");

            $result = $GLOBALS['dbh']->Execute($sql);
        }

        $msg = ("All changes to contact: " . (($contact->contact_title) ? getTitleName($contact_title) . " " : "&nbsp;") . $contact->contact_first_name);
        $msg .= (" " . $contact->contact_last_name . " have been successfully saved.");

        notify($msg);

        echo("<hr />");

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadContactValues($contact_id);

        include('contact/view/contact/view_specific_contact.inc');

        //delete object "contact"
        unset($contact);
        unset($add_confirmed);
    }
}
else
{
    include('misc/loguser.inc');
}

// End of save_contact.inc code.

/*              
 Function is called when we have found a name similar to the company we are trying to save.
 Checks to make sure the user really wants to add this company.  If so, then save
 the company to the database.  Otherwise, return the user to the edit screen.
*/                              

function promptSaveContactChanges($msg, $contact, $employer_id, $department_id, $contact_id)
{                
    global $PHP_SELF;       
    error($msg .  " 
        <table cellspacing=2 cellpadding=2>
        <tr>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='contact' />
            <input type='hidden' name='add_confirmed' value='true' />
            <input type='hidden' name='continue' value='add_confirmed' />
            <input type='hidden' name='contact' value='" . packObject($contact) . "' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input type='hidden' name='department_id' value='" . $department_id . "' />
            <input type='hidden' name='contact_id' value='" . $contact_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
            </form> 
            </td>
            <td>    
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='contact' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input type='hidden' name='department_id' value='" . $department_id . "' />
            <input type='hidden' name='contact_id' value='" . $contact_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
            </form>
            </td>
        </tr>
        </table>
        ");
}

?>
