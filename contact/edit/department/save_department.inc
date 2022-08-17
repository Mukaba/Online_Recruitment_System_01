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
         Check in the database for any divisions at this company with similar names, and ask the user if they are
         sure they want to add THIS division.
        */
        
        $check_name = $contact->department_name[0] . $contact->department_name[1] . $contact->department_name[2];

        $sql = ("
                SELECT DISTINCT department_name
                FROM employer_department
                WHERE employer_id='" . $employer_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        if(strcmp($row["department_name"],$contact->department_name) != 0)
        {        


            $sql = ("
                    SELECT DISTINCT a.department_name, b.company_name
                    FROM employer_department AS a, employer_company AS b
                    WHERE a.department_name LIKE '" . addslashes($check_name) . "%'
                    AND a.employer_id='" . $employer_id . "'
                    AND b.employer_id='" . $employer_id . "'
                    AND a.department_id!='" . $department_id . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                $error = 1;
                if ($result->RecordCount() == 1)
                {
                    $row = $result->FetchRow();
                    $msg .= ($row["department_name"]);
                    $msg .= (" is an exisiting division at " . $row["company_name"] . " already in the database ");
                    $msg .= (" with a name similar to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to save the changes made to this division's information?<br /><br />");
                }
                else
                {
                    while ($row = $result->FetchRow())
                    {
                        $department_names_array[] = $row["department_name"];
                    }
                    for ($i = 0; $i < (sizeof($department_names_array) - 1); $i++)
                    {
                        $msg .= ($department_names_array[$i] . ", ");
                    }
                    $msg .= ("and " . $department_names_array[$i] . " ");
                    $msg .= ("are divisions at " . $row["company_name"] . " already in the database ");
                    $msg .= ("with similar names to the one you are entering.");
                    $msg .= ("<br /><br />Are you sure you want to save the changes made to this division's information?<br /><br />");
                }

                promptSaveDepartmentChanges($msg, $contact, $employer_id, $department_id);
            }
        }
    }

    if ($error == 0)
    {

        $contact->department_id = $department_id;

        $recordedChanges = recordContactChange($contact, "department");

        // Save the new values into the employer_department database.

        $sql = ("
                UPDATE employer_department
                SET department_name='" . addslashes($contact->department_name) . "',
                department_website='" . addslashes($contact->department_website) . "',
                department_description='" . addslashes($contact->department_description) . "',
                street_address_1='" . addslashes($contact->department_street_address_1) . "',
                street_address_2='" . addslashes($contact->department_street_address_2) . "',
                street_address_3='" . addslashes($contact->department_street_address_3) . "',
                city='" . addslashes($contact->department_city) . "',
                region_id='" . addslashes($contact->department_region_id) . "',
                provstate_id='" . addslashes($contact->department_provstate_id) . "',
                country_id='" . addslashes($contact->department_country_id) . "',
                postal_code='" . addslashes($contact->department_postal_code) . "',
                phone='" . addslashes($contact->department_phone) . "',
                fax='" . addslashes($contact->department_fax) . "',
                email='" . addslashes($contact->department_email) . "',
                industry_id='" . $contact->department_industry_id . "',
                size_id='" . $contact->department_size_id . "',
                location_info='" . addslashes(trim($contact->department_location_info)) . "',
                department_name_legitimate='" . addslashes(trim($contact->department_name_legitimate)) . "'
                WHERE department_id='" . $department_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        /*
         Check if we already have an entry for department specific comments in the
         database.  If we do, update those comments, and if we don't, set them.
         */

        $sql = ("
                SELECT DISTINCT comments
                FROM employer_department_comments
                WHERE employer_department_id='" . $department_id . "'
                AND department_id='" . $auth->department . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            if (trim($contact->department_department_comments))
            {
                $sql = ("
                        UPDATE employer_department_comments
                        SET comments='" . addslashes($contact->department_department_comments) . "'
                        WHERE employer_department_id='" . $department_id . "'
                        AND department_id='" . $auth->department . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
            else
            {
                $sql = ("
                    DELETE FROM employer_department_comments
                    WHERE employer_department_id='" . $department_id . "'
                    AND department_id='" . $auth->department . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }
        elseif (trim($contact->department_department_comments))
        {
            $sql = ("
                INSERT INTO employer_department_comments
                (employer_department_id, department_id, comments)
                VALUES
                (
                 '" . $department_id . "',
                 '" . $auth->department . "',
                 '" . addslashes($contact->department_department_comments) . "'
                )
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        /*
         Check if we already have division flags in the database, and if so, update them.
         Otherwise, insert them.
        */

        // First get a list of the flags that this co-op department can set.

        $sql = ("
            SELECT DISTINCT flag_id
            FROM department_division_flags
            WHERE department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $dep_flags[] = $row["flag_id"];
        }

        // Now set an array that contains all of the flags that have been set as off for this employer department.

        for ($i = 0; $i < sizeof($dep_flags); $i++)
        {
            if (!(is_array($contact->department_flags)) || !(in_array($dep_flags[$i], $contact->department_flags)))
            {
                $delete_flags[] = $dep_flags[$i];
            }
        }

        /*
         Now set an array that contains all of the flags that have been set as on for this division,
         and that are not already in the database.
        */

        $sql = ("
            SELECT DISTINCT flag_id
            FROM department_flags_set
            WHERE department_id='" . $department_id . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while($row = $result->FetchRow())
        {
            $set_flags[] = $row["flag_id"];
        }

        for ($i = 0; $i < sizeof($contact->department_flags); $i++)
        {
            if ((!($set_flags) || !(in_array($contact->department_flags[$i], $set_flags))) && is_array($contact->department_flags))
            {
                $insert_flags[] = $contact->department_flags[$i];
            }
        }

        /*
         We now have two arrays.  $delete_flags holds all of the flags that need to be deleted from the
         department_flags_set table, and $insert_flags holds all of the flags that need to be inserted
         into this table.
        */

        if ($delete_flags)
        {
            /*
             Delete from the table the flags that have been set to off by the user.
            */

            for ($i = 0; $i < sizeof($delete_flags); $i++)
            {
                $sql = ("
                    DELETE FROM department_flags_set
                    WHERE flag_id='" . $delete_flags[$i] . "'
                    AND department_id='" . $department_id . "'
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }

        if ($insert_flags)
        {

            /*
             Insert all of the divisions flags that have been set to on into the database.
            */

            for ($i = 0; $i < sizeof($insert_flags); $i++)
            {
                $sql = ("
                    INSERT INTO department_flags_set
                    (department_id, flag_id)
                    VALUES
                    (
                     '" . $department_id . "',
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
                SELECT DISTINCT changes_recorded_1, changes_recorded_2, change_date_1, change_date_2,
                change_by_1, change_by_2
                FROM employer_department
                WHERE department_id='" . $department_id . "'
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
                UPDATE employer_department
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
                WHERE department_id='" . $department_id . "'
                ");

            $result = $GLOBALS['dbh']->Execute($sql);
        }

        // Insert the new note if there is one.

        if (trim($contact->department_new_note))
        {
            $sql = ("
                INSERT INTO division_notes
                (department_id, notes, date_entered, entered_by)
                VALUES (
                '" . $department_id . "',
                '" . addslashes(trim($contact->department_new_note)) . "',
                '" . date("Y-m-d H:i:s") . "',
                '" . getContactID($auth->department, $auth->login) . "'
                )
                ");
            $GLOBALS['dbh']->Execute($sql);
        }

        // Delete the notes the user has requested be deleted

        if ($contact->department_delete_note_ids)
        {
            foreach($contact->department_delete_note_ids AS $ddni)
            {
                $sql = ("
                    DELETE FROM division_notes
                    WHERE note_id='" . $ddni . "'
                    ");
                $GLOBALS['dbh']->Execute($sql);
            }
        }

        notify("All changes to division: " . $contact->department_name . " have been successfully saved.");
        echo("<hr />");

        $no_buttons = true;

        $contact = new EditContact('');
        $contact->loadDepartmentValues($department_id);

        include('contact/view/company/view_specific_department.inc');

        unset($contact);
        unset($add_confirmed);
    }
}
else
{
    include('misc/loguser.inc');
}

/*
 End of save_department.inc code.
*/

/*
 Function is called when we have found a name similar to the company we are trying to save.
 Checks to make sure the user really wants to add this company.  If so, then save
 the company to the database.  Otherwise, return the user to the edit screen.
*/

function promptSaveDepartmentChanges($msg, $contact, $employer_id, $department_id)
{
    global $PHP_SELF;
    error($msg .  "
        <table cellspacing=2 cellpadding=2>
        <tr>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='department' />
            <input type='hidden' name='add_confirmed' value='true' />
            <input type='hidden' name='continue' value='add_confirmed' />
            <input type='hidden' name='contact' value='" . packObject($contact) . "' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input type='hidden' name='department_id' value='" . $department_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;Yes&nbsp;' />
            </form>
            </td>
            <td>
            <form method='post' action='$PHP_SELF'>
            <input type='hidden' name='select' value='edit_contact' />
            <input type='hidden' name='level1' value='department' />
            <input type='hidden' name='employer_id' value='" . $employer_id . "' />
            <input type='hidden' name='department_id' value='" . $department_id . "' />
            <input style='width: 5em;' type='submit' value='&nbsp;No&nbsp;' />
            </form>
            </td>
        </tr>
        </table>
        ");
}

?>
