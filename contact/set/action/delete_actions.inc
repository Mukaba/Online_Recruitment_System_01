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
 | Filename: delete_actions.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is a verification file. It asks the user if they're   |
 | sure they want to delete the selected actions.                               | 
 +------------------------------------------------------------------------------+
*/

// NOTE: actual deletion occurs in actions.inc

echo("<table border='0' class='error' cellspacing='0' cellpadding='7' width='100%'>");
    //Note: All these hidden attributes are used to pass the deletable information between action.inc and delete_actions.inc
    $msg = ""; //string that holds html for the Yes/No buttons in the error box.
    $msg .= ("<tr>");
    $msg .= ("<td>");
    $msg .= ("Are you sure you want to delete the following ". (sizeof($delete_action)==1? "action?": "actions?"));
    $msg .= ("</td>");
    $msg .= ("</tr>");
    $msg .= ("<tr>");
    $msg .= ("<td align='center'>");
    $msg .= ("<form method='post' name='delete_form' action='" . $PHP_SELF . "'>");
    
    $msg .= ("<input type='hidden' name='select' value='set_contact_flag' />");
    $msg .= ("<input type='hidden' name='level1' value='actions' />");
    $msg .= ("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    $msg .= ("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
    $msg .= ("<input type='hidden' name='continue' value='delete_actions' />");
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

   
    // go through each action saved for this contact and store the info for each action selected for deletion to pass to action.inc 
    for($i = 0; $i < $num_actions ; $i++)
    {
       $note[$i] = stripslashes($note[$i]);
       if($delete_action[$i] != NULL)
       {
            $msg .= ("<input type='hidden' name='delete_action[".$i."]' value='".$delete_action[$i]."' />");
            $msg .= ("<input type='hidden' name='date[".$i."]' value='".$date[$i]."' />");
            $msg .= ("<input type='hidden' name='note[".$i."]' value='".$note[$i]."' />");
            $msg .= ("<input type='hidden' name='bring_forward[".$i."]' value='".$bring_forward[$i]."' />");
            $msg .= ("<input type='hidden' name='method[".$i."]' value='".$method[$i]."' />");
            $msg .= ("<input type='hidden' name='method_name[".$i."]' value='".$method_name[$i]."' />");
            $msg .= ("<input type='hidden' name='action_chosen[".$i."]' value='".$action_chosen[$i]."' />");
            $msg .= ("<input type='hidden' name='action_name_chosen[".$i."]' value='".$action_name_chosen[$i]."' />");
            $msg .= ("<input type='hidden' name='action_by[".$i."]' value='".$action_by[$i]."' />");
            $msg .= ("<input type='hidden' name='action_by_name[".$i."]' value='".$action_by_name[$i]."' />");
            $msg .= ("<input type='hidden' name='num_actions' value='".$num_actions."' />");
            $msg .= ("<input type='hidden' name='verify_netlink_id' value='".$verify_netlink_id."' />");
        }
    }

    // display YES/NO buttons
    $msg .= ("<input type='submit' name='verify_delete' value='yes' />");
    $msg .= ("&nbsp;");
    $msg .= ("<input type='submit' name='verify_delete' value='no' />");
    $msg .= ("</form>");
    $msg .= ("</td>");
    $msg .= ("</tr>");
    
    //count the num of the user's own actions from those the user has chosen to delete (they can delete only their own)
    $count = 0; 
    for($i = 0; $i < $num_actions ; $i++)
    {
        if($delete_action[$i] != NULL)
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
                if(!($action_chosen[$i] == CONDUCTED_WORKSITE_VISIT || $action_chosen[$i] == JOB_DESC_RECEIVED || $action_chosen == INT_CONDUCTED || $action_chosen == FINAL_PLACEMENT))
                {
                    $count++;
                }
            }
        }
    }
    
    // if all actions they've chosen to delete are other's actions, throw an error and send them back to specific display
    // otherwise, they've chosen at least 1 action that is their own, so give them option to delete (display $msg)
    // NOTE: This is here just in case some hacking occurs and they manage to select others' actions from specific display
    if($count==0)
    { 
        $err = "";
        $err .= ("<tr>");
            $err .= ("<td>");
            $err .= ("None of the actions you have chosen to delete were entered by you. You may delete only your actions.");
            $err .= ("</td>");
        $err .= ("</tr>");
        $err .= ("<tr>");
            $err .= ("<td align='center'>");
            $err .= ("<form method='post' name='back' action='" . $PHP_SELF . "'>");
            $err .= ("<input type='hidden' name='select' value='set_contact_flag' />");
            $err .= ("<input type='hidden' name='level1' value='actions' />");
            $err .= ("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
            $err .= ("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
            $err .= ("<input type='submit' name='verify_delete' value='Back' />");
            $err .= ("<input type='hidden' name='continue' value='delete_actions' />");
            $err .= ("</form>");
            $err .= ("</td>");
        $err .= ("</tr>");
        echo($err);
    }
    else
    {
        echo($msg);
    }
echo("</table>");
echo("<br />"); 

    // display the notes that user wants to delete. those that the user can't delete will be stored in an array 'arr_cant_delete'
    // just in case hacking occurs and they manage to choose ones they cant delete in specific_display.inc
    echo("<table border='0' cellspacing='0' cellpadding='7' width='100%'>");
    echo("<tr>");
        echo("<td colspan='2'>");
            echo("<hr />");
        echo("</td>");
    echo("</tr>");

    // go through all actions for this contact and display those that have been tagged for deletion by user
    // NOTE: user can only delete actions they've entered.
    $undeletable_count=0;
    for($i = 0; $i < $num_actions ; $i++)
    {
        $display_count++;
        $note[$i] = stripslashes($note[$i]);
        if($delete_action[$i] != NULL)
        {
            $display="";
            $display .= ("<tr>");
                $display .= ("<td nowrap='nowrap'>");
                    $display .= ("<b>On:</b> ".$date[$i]);
                $display .= ("</td>");
                $display .= ("<td nowrap='nowrap'>");
                    $display .= ("<b>Action:</b> ".$action_name_chosen[$i]);
                $display .= ("</td>");
            $display .= ("</tr>");
            $display .= ("<tr>");
                $display .= ("<td nowrap='nowrap'>");
                    $display .= ("<b>By:</b> ".$action_by_name[$i]);
                $display .= ("</td>");
                $display .= ("<td nowrap='nowrap'>");
                if($action_chosen[$i] == GENERIC_NOTE)
                { 
                    $display .= ("<b>Method:</b> N/A");
                }
                else
                {
                    $display .= ("<b>Method:</b> ".$method_name[$i]);
                }
                $display .= ("</td>");
            $display .= ("</tr>");
            $display .= ("<tr>");
                $display .= ("<td colspan='2'>");
                if(unpackObject(($note[$i])) == "")
                {
                     $display .= ("<b>Note:</b> N/A");
                }
                else
                {
                    $display .= ("<b>Note:</b> ".unpackObject(($note[$i])));
                }
                $display .= ("</td>");
            $display .= ("</tr>");
            $display .= ("<tr>");
                $display .= ("<td colspan='2'>");
                if($bring_forward[$i] == "0000-00-00" || $bring_forward[$i] == NULL)
                {
                     $display .= ("<b>Bring Forward Date:</b> N/A");
                }
                else
                {
                    $display .= ("<b>Bring Forward Date:</b> ".$bring_forward[$i]);
                }
                $display .= ("</td>");
            $display .= ("</tr>");
            $display .= ("<tr>");
                $display .= ("<td colspan='2'>");                        
                $display .= ("<hr />"); 
                $display .= ("</td>");
            $display .= ("</tr>");

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
                if(!($action_chosen[$i] == CONDUCTED_WORKSITE_VISIT || $action_chosen[$i] == JOB_DESC_RECEIVED || $action_chosen[$i] == INT_CONDUCTED || $action_chosen[$i]== FINAL_PLACEMENT))
                {
                    echo($display);
                }
                else
                {
                    $arr_cant_delete[$undeletable_count++] = $display;
                }
            }
            else
            {
                $arr_cant_delete[$undeletable_count++] = $display;
            }
        }
    }
    for($i = 0; $i < sizeof($arr_cant_delete) ; $i++)
    {
        if($i == 0)
        {
            echo("<tr>");
            echo("<td colspan='2'>");
            error("The actions below are not your actions OR are automated actions, therefore, these actions will NOT be deleted.");
            echo("</td>");
            echo("</tr>");
        }
        echo($arr_cant_delete[$i]);
   }
    echo("</table>");
?>
