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
 | Description: This file is used to move the edited inputs from the contact    |
 | object into the databse.                                                     |
 +------------------------------------------------------------------------------+

*/


if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
{
    assert(isset($contact));

    $error = 0;

    if (!$add_confirmed)
    {

        /*
         Check in the database for any companies that have similar names, and ask the user if they are
         sure they want to add THIS company.
        */

        $check_name = $contact->company_name[0] . $contact->company_name[1] . $contact->company_name[2];

        $sql = ("
            SELECT DISTINCT company_name
            FROM employer_company
            WHERE employer_id='" . $employer_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        if(strcmp($row["company_name"],$contact->company_name) != 0)
        {

            $sql = ("
                    SELECT DISTINCT company_name
                    FROM employer_company
                    WHERE company_name LIKE '" . addslashes($check_name) . "%'
                    AND employer_id!='" . $employer_id . "'
                    AND company_display
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ($row["company_name"]);
                    $msg .= (" is an exisiting company in the database with a name similar to");
                    $msg .= (" the one you are saving.");
                    $msg .= ("<br /><br />Are you sure you want to change this company's name to " . $contact->company_name . "?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $company_names_array[] = $row["company_name"];
                    }
                    for ($i = 0; $i < (sizeof($company_names_array) - 1); $i++)
                    {
                        $msg .= ($company_names_array[$i] . ", ");
                    }
                    $msg .= ("and " . $company_names_array[$i] . " ");
                $msg .= ("are companies with similar names to the one you are saving.");
                $msg .= ("<br /><br />Are you sure you want to change this company's name to " . $contact->company_name . "?<br /><br />");
            }

            promptSaveCompanyChanges($msg, $contact, $employer_id);
        }
        }
    }

    if ($error == 0)
    {

        $contact->employer_id = $employer_id;

        $recordedChanges = recordContactChange($contact, "company");

        // Save the new values into the employer_company database.

        $sql = ("
            UPDATE employer_company
            SET company_name='" . addslashes($contact->company_name) . "',
            website='" . addslashes($contact->company_website) . "',
            company_description='" . addslashes($contact->company_description) . "',
            street_address_1='" . addslashes($contact->company_street_address_1) . "',
            street_address_2='" . addslashes($contact->company_street_address_2) . "',
            street_address_3='" . addslashes($contact->company_street_address_3) . "',
            city='" . addslashes($contact->company_city) . "',
            region_id='" . addslashes($contact->company_region_id) . "',
            provstate_id='" . addslashes($contact->company_provstate_id) . "',
            country_id='" . addslashes($contact->company_country_id) . "',
            postal_code='" . addslashes($contact->company_postal_code) . "',
            phone='" . addslashes($contact->company_phone) . "',
            fax='" . addslashes($contact->company_fax) . "',
            email='" . addslashes($contact->company_email) . "',
            industry_id='" . $contact->company_industry_id . "',
            company_type_id='" . $contact->company_type_id . "',
            size_id='" . $contact->company_size_id . "',
            company_name_legitimate='" . addslashes(trim($contact->company_name_legitimate)) . "'
            WHERE employer_id='" . $employer_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        /*
         Check if we already have an entry for department specific comments in the
         database.  If we do, update those comments, and if we don't, set them.
        */

        $sql = ("
            SELECT DISTINCT comments
            FROM employer_company_comments
            WHERE employer_company_id='" . $employer_id . "'
            AND department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            if (trim($contact->company_department_comments))
            {
                $sql = ("
                    UPDATE employer_company_comments
                    SET comments='" . addslashes($contact->company_department_comments) . "'
                    WHERE employer_company_id='" . $employer_id . "'
                    AND department_id='" . $auth->department . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
            else
            {
                $sql = ("
                    DELETE FROM employer_company_comments
                    WHERE employer_company_id='" . $employer_id . "'
                    AND department_id='" . $auth->department . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }
        elseif (trim($contact->company_department_comments))
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

        /*
         Check if we already have company flags in the database, and if so, update them.
         Otherwise, insert them.
        */

        // First get a list of the flags that this co-op department can set.

        $sql = ("
            SELECT DISTINCT flag_id
            FROM department_company_flags
            WHERE department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $dep_flags[] = $row["flag_id"];
        }

        // Now set an array that contains all of the flags that have been set as off for this employer.

        for ($i = 0; $i < sizeof($dep_flags); $i++)
        {
            if (!(is_array($contact->company_flags)) || !(in_array($dep_flags[$i], $contact->company_flags)))
            {
                $delete_flags[] = $dep_flags[$i];
            }
        }

        /*
         Now set an array that contains all of the flags that have been set as on for this employer,
         and that are not already in the database.
        */

        $sql = ("
            SELECT DISTINCT flag_id
            FROM company_flags_set
            WHERE employer_id='" . $employer_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $set_flags[] = $row["flag_id"];
        }

        for ($i = 0; $i < sizeof($contact->company_flags); $i++)
        {
            if ((!($set_flags) || !(in_array($contact->company_flags[$i], $set_flags))) && is_array($contact->company_flags))
            {
                $insert_flags[] = $contact->company_flags[$i];
            }
        }

        /*
         We now have two arrays.  $delete_flags holds all of the flags that need to be deleted from the
         company_flags_set table, and $insert_flags holds all of the flags that need to be inserted
         into this table.
        */

        if ($delete_flags)
        {
            // Delete from the table the flags that have been set to off by the user.

            for ($i = 0; $i < sizeof($delete_flags); $i++)
            {
                $sql = ("
                    DELETE FROM company_flags_set
                    WHERE flag_id='" . $delete_flags[$i] . "'
                    AND employer_id='" . $employer_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }

        if ($insert_flags)
        {

            // Insert all of the company flags that have been set to on into the database.

            for ($i = 0; $i < sizeof($insert_flags); $i++)
            {
                $sql = ("
                    INSERT INTO company_flags_set
                    (employer_id, flag_id)
                    VALUES
                    (
                     '" . $employer_id . "',
                     '" . $insert_flags[$i] . "'
                    )
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }

        }

        // Insert the new note if there is one.

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
        }

        // Delete the notes the user has requested be deleted

        if ($contact->company_delete_note_ids)
        {
            foreach($contact->company_delete_note_ids AS $cdni)
            {
                $sql = ("
                    DELETE FROM company_notes
                    WHERE note_id='" . $cdni . "'
                    ");
                $GLOBALS['dbh']->Execute($sql);
            }
        }

        if ($recordedChanges)
        {
            /*      
             Lastly, we need to set the change reasons, dates, and by who.  Do this by first bumping the first
             and second reasons down one, and then inserting the most recent reason, date and who into the database.
            */              

            $sql = ("       
                SELECT DISTINCT changes_recorded_1, changes_recorded_2, change_date_1, change_date_2,
                change_by_1, change_by_2
                FROM employer_company
                WHERE employer_id='" . $employer_id . "'
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
                UPDATE employer_company
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
                WHERE employer_id='" . $employer_id . "'
                ");

            $result = $GLOBALS['dbh']->Execute($sql);
        }


        notify("All changes to company: " . $contact->company_name . " have been successfully saved.");
        echo("<hr />");

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadCompanyValues($employer_id);

        include('contact/view/company/view_specific_company.inc');
        unset($contact);
        unset($add_confirmed);

        /*
         the employer_id MUST be set to proceed to the next screen, because it allows the user to directly
         add a division to this company from the next screen.
        */
    }
}
else
{
    include('misc/loguser.inc');
}

// End of save_company.inc code.

/*
 Function is called when we have found a name similar to the company we are trying to save.
 Checks to make sure the user really wants to add this company.  If so, then save
 the company to the database.  Otherwise, return the user to the edit screen.
*/

function promptSaveCompanyChanges($msg, $contact, $employer_id)
{
    global $PHP_SELF;
    error($msg .  "
        <table cellspacing=2 cellpadding=2>
        <tr>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='company' />
            <input type='hidden' name='add_confirmed' value='true' />
            <input type='hidden' name='continue' value='add_confirmed' />
            <input type='hidden' name='contact' value='" . packObject($contact) . "' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
            </form>
            </td>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='company' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
            </form>
            </td>
        </tr>
        </table>
        ");
}

?>
