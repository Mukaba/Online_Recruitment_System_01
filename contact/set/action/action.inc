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
 | Filename: action.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: This is the main action file.  It determines which page it      |
 | should be loading, or whether or not it needs to save the actions for this   |
 | contact(s).                                                                  |
 +------------------------------------------------------------------------------+

*/

if (!$continue)
{
    /*
     They haven't done anything yet, so display the general form for them.
     */
    include('contact/set/action/general_display.inc');
}

elseif ($continue == 'save_general')
{
    

    if ($contact_ids && is_string($contact_ids))
    {
        $contact_ids = unpackObject(($contact_ids));
    }

    $error = 0;
    /*
     We've been passed back three arrays: action, which holds the actions that has been
     set for each contact (or none), action_method, which holds the action methods for each contact,
     action_date, which holds the dates that these actions
     were performed on, and contact_ids, which holds all of the contact ids that had
     been displayed.  We also have an action_for variable which holds the contact id of
     the person that these actions are being saved for.
     */

    /*
     Check that we have an action method and date for every action being set.
     Also, keep a action_counter, which, if it equals 0 after this loop, will tell us
     if no actions have been set (dont want to bother saving an empty form).
     */
    $action_counter = 0;
    for ($i = 0; $i < sizeof($action_date); $i++)
    {
        if ($action[$i])
        {
            $action_counter++;
            
            if(!($action_method[$i]) && ($action[$i] != GENERIC_NOTE))
            {
                $error = 1;
                error("You must select a method for each action.");
                $form_submitted = true;
                include('contact/set/action/general_display.inc');
            }
            elseif (validDate($action_date[$i]) == '-1')
            {
                $error = 1;
                error("You must include a valid date for each action that you would like to set in the format YYYY-MM-DD.");
                $form_submitted = true;
                include('contact/set/action/general_display.inc');
            }
            elseif($action[$i] == GENERIC_NOTE && $action_note=="")
            {
                // didn't write a note for thier generic note!
                $error = 1;
                error("You must enter a note for your action (Generic Note).");
                $form_submitted = true;
                include('contact/set/action/general_display.inc');
            }
            elseif($bring_forward_date)
            {
                if(validDate($bring_forward_date) ==  '-1')
                {
                    $error = 1;
                    error("You must include a valid bring-forward date for this action in the format YYYY-MM-DD.");
                    $form_submitted = true;
                    include('contact/set/action/general_display.inc');
                }
                else
                {
                    //make sure that the date is greater than today's date 
                    $sql = ("
                        SELECT CURRENT_DATE as today
                    ");
                    $date_result = $GLOBALS['dbh']->Execute($sql);
                    $date_row = $date_result->FetchRow();
                    $todays_date = $date_row["today"];

                    if($bring_forward_date <= $todays_date)
                    {
                        $error = 1;
                        error("The bring-forward date must be greater than today's date.");
                        $form_submitted = true;
                        include('contact/set/action/general_display.inc');
                    }
                }
            }
            else
            {
                $action_date[$i] = validDate($action_date[$i]);
            }
        }
    }
    
    //if no actions have been set (form is empty!), throw an error
    if($action_counter =='0')
    {
        $error = 1;
        error("You did not select any actions.");
        $form_submitted = true;
        include('contact/set/action/general_display.inc');
    }
        
    if ($error == 0)
    {	
        if (($action_counter > 1) && ($bring_forward_date != '0000-00-00') && ($bring_forward_date != NULL))
        {
            $sql = ("
                    LOCK TABLES contact_actions_set
                    WRITE
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $sql = ("
                    SELECT MAX(multiple_bring_forward_date) AS max_bf
                    FROM contact_actions_set
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row_bf = $result->FetchRow(); 
            $multiple_bf_date = $row_bf['max_bf']+1;
        }

        for ($i = 0; $i < sizeof($action); $i++)
        {
            if ($action[$i])
            {
                /*
                 Check to see if this action is already in the database, with this date, and by the
                 same person.
                 */

                $sql = ("
                        SELECT DISTINCT *
                        FROM contact_actions_set
                        WHERE action_id='" . $action[$i] . "'
                        AND contact_id='" . $contact_ids[$i] . "'
                        AND action_by='" . $action_for . "'
                        AND action_on='" . $action_date[$i] . "'
                        AND action_method_id='" .$action_method[$i]. "'
                        AND action_note ='" .addslashes(stripslashes($action_note)). "'
                        AND bring_forward_date = '".$bring_forward_date."'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);

                if (!$result->RecordCount())
                {
                    if ($multiple_bf_date)
                    {
                        $sql = ("
                                INSERT INTO contact_actions_set 
                                (action_id, contact_id, action_by, action_on,  action_method_id, action_note, bring_forward_date, multiple_bring_forward_date)
                                VALUES
                                (
                                 '" . $action[$i] . "',
                                 '" . $contact_ids[$i] . "',
                                 '" . $action_for . "',
                                 '" . $action_date[$i] . "',
                                 '" . $action_method[$i]. "',
                                 '" . addslashes(stripslashes(trim($action_note))) ."',
                                 '" . $bring_forward_date. "',
                                 '" . $multiple_bf_date. "'
                                )
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    } else {
                        $sql = ("
                                INSERT INTO contact_actions_set 
                                (action_id, contact_id, action_by, action_on,  action_method_id, action_note, bring_forward_date)
                                VALUES
                                (
                                 '" . $action[$i] . "',
                                 '" . $contact_ids[$i] . "',
                                 '" . $action_for . "',
                                 '" . $action_date[$i] . "',
                                 '" . $action_method[$i]. "',
                                 '" . addslashes(stripslashes(trim($action_note))) ."',
                                 '" . $bring_forward_date. "'
                                )
                                ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }
                }
            }
        }

        $sql = ("
                UNLOCK TABLES
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        notify("All actions have been successfully saved.");

        $form_submitted = true;
        unset($action);
        unset($action_method);
        unset($action_date);
        unset($action_note);
        unset($action_by);
        unset($bring_forward_date);

        include('contact/set/action/general_display.inc');
    }
}

// We enter this state if a user is setting actions for ALL matched contacts. 
elseif ($continue == 'save_actions_for_all')
{
    $error_array = array();
    
    if (!isInteger($all_action_id))
    {
        $error_array[] = "You must specify which action you would like to add for these contacts.";
    }
    else
    {
        $sql = ("
                SELECT action_id 
                FROM action_types
                WHERE action_id='" . $all_action_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount())
        {
            $error_array[] = "You have attempted to set an action type that does not exist.  Please try again.";
        }
    }

    if(!isInteger($all_action_method) && ($all_action_id != GENERIC_NOTE))
    {
        $error_array[] = "You must specify which action method you would like to add for these contacts.";
    }
    else
    {
        $sql = ("
                SELECT action_method_id 
                FROM action_methods
                WHERE action_method_id='" . $all_action_method . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount())
        {
            $error_array[] = "You have attempted to set an action method that does not exist.  Please try again.";
        }
    }

    if (!isInteger($all_action_by))
    {
        $error_array[] = "You must specify who the action is performed by.";
    }

    else
    {
        // Check and make sure they haven't hacked the contact_id to add an action
        // using someone that isn't from there division.

        $department_in_str = department_groups($auth->department);
        $department_in_str = "'". implode("','",$department_in_str) . "'";
        
        $sql = ("
                SELECT contact_id
                FROM contact_internal
                WHERE department_id IN (" . $department_in_str . ")
                AND contact_id='" . $all_action_by . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount())
        {
            $error_array[] = "You may only set actions for contacts that are in your Co-op group.";
        }
    }

    if (validDate($all_action_on) == '-1')
    {
        $error_array[] = "You must include a valid date for these actions in the format YYYY-MM-DD.";
    }

    if($all_bring_forward_date)
    {
        if(validDate($all_bring_forward_date) ==  '-1')
        {
            $error_array[] = "You must include a valid bring forward date for these actions in the format YYYY-MM-DD.";
        }
        else
        {
            $todays_date = date("Y-m-d");

            if($all_bring_forward_date <= $todays_date)
            {
                $error_array[] = "The bring-forward date must be greater than today's date.";
            }
        }
    }

    if ($contact_id_list)
    {
        $contact_ids = explode(",", $contact_id_list);
        if (is_array($contact_ids) && sizeof($contact_ids))
        {
            foreach ($contact_ids as $cids)
            {
                if (!isInteger($cids))
                {
                    $error_array[] = "The list of matched contacts is invalid. Please try again.";
                    break;
                }
            }
        }
    }

    // didn't write a note for thier generic note!
    if($all_action_id == GENERIC_NOTE && trim($all_action_note) == "")
    {
        $error_array[] = "You must enter a note for your actions (Generic Note).";
    }   
    
    if (is_array($error_array) && sizeof($error_array))
    {
        $form_submitted = true;
        include('contact/set/action/general_display.inc');
    }

    // Data validated, commence inserting. 
    else
    {
        if (($all_bring_forward_date != '0000-00-00') && ($all_bring_forward_date != NULL))
        {
            $sql = ("
                    LOCK TABLES contact_actions_set
                    WRITE
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $sql = ("
                    SELECT MAX(multiple_bring_forward_date) AS max_bf
                    FROM contact_actions_set
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
            $row_bf = $result->FetchRow(); 
            $multiple_bf_date = $row_bf['max_bf']+1;
        }

        for ($i = 0; $i < sizeof($contact_ids); $i++)
        {
            // Check to see if this action is already in the database, with this date, and by the
            // same person.

            $sql = ("
                    SELECT contact_actions_id 
                    FROM contact_actions_set
                    WHERE action_id='" . $all_action_id . "'
                    AND contact_id='" . $contact_ids[$i] . "'
                    AND action_by='" . $all_action_by . "'
                    AND action_on='" . $all_action_on . "'
                    AND action_method_id='" .$all_action_method. "'
                    AND action_note ='" .addslashes(stripslashes($all_action_note)). "'
                    AND bring_forward_date = '".$all_bring_forward_date."'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if (!$result->RecordCount())
            {
                if ($multiple_bf_date)
                {
                    $sql = ("
                            INSERT INTO contact_actions_set 
                            (action_id, contact_id, action_by, action_on, action_method_id, action_note, bring_forward_date, multiple_bring_forward_date)
                            VALUES
                            (
                             '" . $all_action_id . "',
                             '" . $contact_ids[$i] . "',
                             '" . $all_action_by . "',
                             '" . $all_action_on . "',
                             '" . $all_action_method. "',
                             '" . addslashes(stripslashes(trim($all_action_note))) ."',
                             '" . $all_bring_forward_date. "',
                             '" . $multiple_bf_date. "'
                            )
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                } 
                else 
                {
                    $sql = ("
                            INSERT INTO contact_actions_set 
                            (action_id, contact_id, action_by, action_on, action_method_id, action_note, bring_forward_date)
                            VALUES
                            (
                             '" . $all_action_id . "',
                             '" . $contact_ids[$i] . "',
                             '" . $all_action_by . "',
                             '" . $all_action_on . "',
                             '" . $all_action_method . "',
                             '" . addslashes(stripslashes(trim($all_action_note))) ."',
                             '" . $bring_forward_date. "'
                            )
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                }
            }
        }

        if (($all_bring_forward_date != '0000-00-00') && ($all_bring_forward_date != NULL))
        {
            $sql = ("
                    UNLOCK TABLES
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        notify("All actions have been successfully saved.");

        $form_submitted = true;
        unset($all_action_id);
        unset($all_action_method);
        unset($all_action_on);
        unset($all_action_note);
        unset($all_action_by);
        unset($all_bring_forward_date);

        include('contact/set/action/general_display.inc');
    }
}

elseif ($continue == 'Back to List') 
{
    // Return the user to the general action setting page.

    include('contact/set/action/general_display.inc');
}

elseif ($continue == 'show_specific')
{
    // The user would like to edit this contact's flags specifically.

    include('contact/set/action/specific_display.inc');
}

elseif ($continue == 'save_specific')
{

    $error = 0;

    // The user would like to save the action they have just entered for this contact.
    // If they haven't set an action, then do nothing.

    if (!trim($action))
    {
        $error = 1;
        error("You must specify which action you would like to add for this contact.");
        $form_submitted = true;
        include('contact/set/action/specific_display.inc');
    }
    elseif(!trim($action_method) && ($action != GENERIC_NOTE))
    {
        $error = 1;
        error("You must specify which action method you would like to add for this contact.");
        $form_submitted = true;
        include('contact/set/action/specific_display.inc');
    } 
    elseif (validDate($action_on) == '-1')
    {
        $error = 1;
        error("You must include a valid date for this action in the format YYYY-MM-DD.");
        $form_submitted = true;
        include('contact/set/action/specific_display.inc');
    }
    elseif($bring_forward_date)
    {
        if(validDate($bring_forward_date) ==  '-1')
        {
            $error = 1;
            error("You must include a valid bring forward date for this action in the format YYYY-MM-DD.");
            $form_submitted = true;
            include('contact/set/action/specific_display.inc');
        }
        else
        {
            //make sure that the date is greater than today's date 
            $sql = ("
                SELECT CURRENT_DATE as today
                ");
            $date_result = $GLOBALS['dbh']->Execute($sql);
            $date_row = $date_result->FetchRow();
            $todays_date = $date_row["today"];

            if($bring_forward_date <= $todays_date)
            {
                $error = 1;
                error("The bring-forward date must be greater than today's date.");
                $form_submitted = true;
                include('contact/set/action/specific_display.inc');
            }
        }
    }
    elseif($action == GENERIC_NOTE && $action_note=="")
    {
        // didn't write a note for thier generic note!
        $error = 1;
        error("You must enter a note for your action (Generic Note).");
        $form_submitted = true;
        include('contact/set/action/specific_display.inc');
    }   
    else
    {	
        $action_on = validDate($action_on);
    }

    if ($error == 0)
    {
        /*
         Check and make sure they haven't hacked the contact_id to add an action
         using someone that isn't from there division.
         */

        $sql = ("
                SELECT DISTINCT a.department_id
                FROM department_group AS a, department_group AS b
                WHERE b.department_id='" . $auth->department . "'
                AND a.group_id=b.group_id
                ");
        $group_result = $GLOBALS['dbh']->Execute($sql);

        if ($group_result->RecordCount())
        {
            while($group_row = $group_result->FetchRow())
            {
                $department_ids .= ("'" . $group_row["department_id"] . "',");
            }

            $department_ids = substr($department_ids, 0, (strlen($department_ids) - 1));
        }
        else
        {
            $department_ids = ("'" . $auth->department . "'");
        }

        $sql = ("
                SELECT DISTINCT contact_id
                FROM contact_internal
                WHERE department_id IN (" . $department_ids . ")
                AND contact_id='" . $action_by . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount())
        {
            $error = 1;
            error("You may only set actions for contacts that are in your Co-op group.");
            include('contact/set/action/specific_display.inc');
        }
        else
        {
            $sql = ("
                    SELECT DISTINCT *
                    FROM action_types
                    WHERE action_id='" . $action . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if (!$result->RecordCount())
            {
                $error = 1;
                error("You have attempted to set an action for this contact that does not exist.  Please try again.");
                include('contact/set/action/specific_display.inc');
            }
        }
    }

    if ($error == 0)
    {

        /*
         Check if there already exists an entry in the database with this action, contact, date, and person
         the action is being added for.
         */
        
        // prepare the note (when passing forms, the browser adds slashes to special chars)
        // also, the overLib pop-up that displays this note does not read carriage returns, newlines or doubles quotes 
        $action_note = stripslashes($action_note);
        $action_note = trim($action_note);
        $action_note = str_replace("\"", "'", $action_note);
        $action_note = str_replace("\r", "", $action_note);
        $action_note = str_replace("\n", "<br />", $action_note);
        
        $sql = ("
                SELECT DISTINCT *
                FROM contact_actions_set
                WHERE action_id='" . $action . "'
                AND contact_id='" . $contact_id . "'
                AND action_by='" . $action_by . "'
                AND action_on='" . $action_on . "'
                AND action_method_id='" . $action_method ."'
                AND action_note = '" . addslashes(trim($action_note)) . "'
                ANd bring_forward_date = '" .$bring_forward_date . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if (!$result->RecordCount())
        {
            $sql = ("
                    INSERT INTO contact_actions_set
                    (action_id, contact_id, action_by, action_on, action_method_id, action_note, bring_forward_date)
                    VALUES
                    (
                     '" . $action . "',
                     '" . $contact_id . "',
                     '" . $action_by . "',
                     '" . $action_on . "',
                     '" . $action_method ."',
                     '" . addslashes(trim($action_note)) ."',
                     '" . $bring_forward_date ."'
                    )
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);
        }

        notify("Action successfully saved.");

        $form_submitted = true;

        /*
         Notify that the changes have been successfully made, and reload the page.
         */

        unset($action);
        unset($action_method);
        unset($action_on);
        unset($action_note);
        unset($action_by);
        unset($bring_forward_date);

        include('contact/set/action/specific_display.inc');
    }
}
elseif($continue == 'delete_actions')
{
    if(sizeof($delete_action) == '0' && !($verify_delete))
    {
        notify("You have not selected any actions to delete.");
        include('contact/set/action/specific_display.inc');
    }
    else
    { 
        if($verify_delete!='yes')
        {
            if($verify_delete == 'no' || $verify_delete == 'Back')
            {
                //they've been sent to confirmation page, and have chosen not to delete
                notify("No actions were deleted");
                unset($deleted);
                unset($verify_delete);
                unset($action);
                unset($action_method);
                unset($action_on);
                unset($action_note);
                unset($action_by);
                unset($bring_forward_date);
                include('contact/set/action/specific_display.inc');
            }
            else
            {
                // user has chosen to delete something, but have not been directed yet to delete page
                include('contact/set/action/delete_actions.inc');
            }
        }
        elseif($verify_delete=='yes')
        {
            // Get the first name and last name of the user's contact_id. need to verify the netlink_id of the performer (action_by_name) matches each 
            // selected for deletion, as user can delete only notes that they've entered. NOTE: a user logged in as ENGR may delete a note they entered
            // while logged in as BIOLOGY
            $check_sql = ("
                SELECT ci.netlink_id 
                FROM contact as c 
                INNER JOIN contact_internal as ci 
                ON c.contact_id=ci.contact_id
                WHERE c.contact_id='".$auth->contact_id."'
                ");
            $check_result = $GLOBALS['dbh']->Execute($check_sql);
            $check_row = $check_result->FetchRow();
            $verify_netlink_id = $check_row['netlink_id'];
            
            for($i = 0; $i < $num_actions ; $i++)
            {
                $mini_sql = ("
                    SELECT netlink_id
                    FROM contact_internal
                    WHERE contact_id='".$action_by[$i]."'
                    ");
                $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                $mini_row = $mini_result->FetchRow();

                // "delete" checkbox, cant delete other's or autmomated statuses. Can delete others if your traffic or director's office
                if($mini_row['netlink_id'] == $verify_netlink_id || $auth->department == TRAFFIC_DIRECTOR_DEPARTMENT || $auth->department == DIRECTORS_OFFICE_DEPARTMENT)
                {
                    if(!($action_chosen[$i] == JOB_DESC_RECEIVED || $action_chosen[$i] == CONDUCTED_WORKSITE_VISIT ||  $action_chosen[$i]==INT_CONDUCTED || $action_chosen[$i] == FINAL_PLACEMENT))
                    {
                        $note[$i] = stripslashes($note[$i]);
                        if($delete_action[$i] != NULL)
                        {
                            //before we delete, verify that what we're deleting is in the database
                            $check_sql = ("
                                SELECT * FROM contact_actions_set
                                WHERE contact_id='" .$contact_id. "' 
                                AND action_id='" .$action_chosen[$i]. "' 
                                AND action_note='" .addslashes(unpackObject(($note[$i]))). "'
                                AND action_on='" .$date[$i]. "' 
                                AND action_method_id='" .$method[$i]. "' 
                                AND action_by ='" .$action_by[$i]. "'
                            ");
                            $result = $GLOBALS['dbh']->Execute($check_sql);
                            $check_row1 = $result->FetchRow();
                            // delete if it's in the db 
                            if($result->RecordCount())
                            {
                                $sql = ("
                                    DELETE FROM contact_actions_set 
                                    WHERE contact_actions_id='" .$check_row1['contact_actions_id']. "' 
                                ");
                                $result = $GLOBALS['dbh']->Execute($sql);
                            }
                        }
                    }
                }
            }
            unset($deleted);
            unset($verify_delete);
            unset($action);
            unset($action_method);
            unset($action_on);
            unset($action_note);
            unset($action_by);
            unset($bring_forward_date);
            
            $form_submitted = true;
            notify("Actions Deleted.");
            include('contact/set/action/specific_display.inc');
        }
        
    }
}

?>
