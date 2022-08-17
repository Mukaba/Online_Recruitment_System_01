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
 | Filename: contact_functions.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This file contains any functions that are specific to the       |
 | contact info module. Most of these functions operate with the Contact.class  |
 | file.                                                                        |
 |                                                                              |
 | Function List:                                                               |
 |                                                                              |
 | void promptAddCompany($msg, $contact) void promptAddDepartment($msg,         |
 | $contact) void promptAddContact($msg, $contact) void                         |
 | promptEditCompany($msg, $employer_id) void promptEditDepartment($msg,        |
 | $department_id) void promptEditContact($msg, $contact_id)                    |
 | array checkCompanyChanges(class $initial_contact,                            |
 | class $attempted_save_contact) array                                         |
 | checkDivisionchanges(class $initial_contact, class $attempted_save_contact)  |
 | array checkContactChanges(class $initial_contact, class                      |
 | $attempted_save_contact)                                                     |
 +------------------------------------------------------------------------------+

*/
// returns a string that shows a table with the contact's last 10 actions. 
function showMostRecentContactActions($contact_id) {
    $sql_actions = ("
            SELECT DISTINCT cas.action_id, cas.contact_id, cas.action_by, cas.action_on, cas.action_method_id, cas.action_note, cas.bring_forward_date
            FROM contact_actions_set AS cas
            WHERE cas.contact_id = '" .$contact_id. "'
            ORDER BY cas.action_on DESC, cas.contact_actions_id DESC
            LIMIT 10
            ");
    $result_actions = $GLOBALS['dbh']->Execute($sql_actions);

    $contactOutput = null;

    if ($result_actions->RecordCount()) {

        // init
        $arr_contact_name = getContactName($contact_id);

        $contactOutput .= ("<table cellpadding='5' cellspacing='0' border='0' align='center'>\n");
        $contactOutput .= ("<tr>");
        $contactOutput .= ("<td align='center' colspan='6'>");
        $contactOutput .= ("Ten Most Recent Actions/Notes for ".$arr_contact_name['first_name']." ".$arr_contact_name['last_name'].": ");
        $contactOutput .= ("</td>");
        $contactOutput .= ("</tr>\n");
        $contactOutput .= ("<tr>");
        $contactOutput .= ("<td align='center' colspan='6'>&nbsp;</td>");
        $contactOutput .= ("</tr>\n");

        $count = 0; // keep a counter to identify the actions displayed
        while ($row_actions = $result_actions->FetchRow())
        {
            // Get action name
            $mini_sql = ("
                    SELECT brief_action_name
                    FROM action_types
                    WHERE action_id = '" . $row_actions['action_id']. "'
                    ");
            $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
            $mini_row = $mini_result->FetchRow();
            $action_name = $mini_row["brief_action_name"];

            // Get action method name
            // Note: action methods introduced at later development date, thus some actions methods were "Never Set"
            $mini_sql =("
                    SELECT action_method_name
                    FROM action_methods
                    WHERE action_method_id = '" .$row_actions["action_method_id"]. "'
                    ");
            $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
            $mini_row = $mini_result->FetchRow();
            $action_method_name = ($mini_row["action_method_name"]!="" ? $mini_row["action_method_name"] : "Never Set");

            // get name of action_by
            $mini_sql = ("
                    SELECT CONCAT(first_name,' ', last_name) AS name
                    FROM contact
                    WHERE contact_id='".$row_actions['action_by']."'
                    ");
            $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
            $mini_row = $mini_result->FetchRow();
            $display_name = $mini_row['name'];

            // Display our query results
            $contactOutput .= ("<tr>");
            
            // Display bring_forward_date in pop-up form if this action had one
            if(($row_actions["bring_forward_date"]!='0000-00-00') && ($row_actions["bring_forward_date"] != NULL) && ($row_actions["bring_forward_date"] > date('Y-m-d')))
            {
                $contactOutput .= ("<td nowrap='nowrap'>");

                // pop-up function call
                $contactOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$row_actions['bring_forward_date']."', CAPTION, 'Bring Forward Date', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">");       
                $contactOutput .= ("<img border='0' src='misc/images/clock.gif' alt='' />");
                $contactOutput .= ("</a>");
                $contactOutput .= ("</td>\n");
            }
            else
            {
                $contactOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
            }

            // Display note in pop-up form if this action had one
            if($row_actions["action_note"])
            {
                $contactOutput .= ("<td nowrap='nowrap'>");
                $contact_action_note = trim($row_actions['action_note']);

                //  newlines, carriage returns, and double quotes passed into overlib() function must be changed to work properly
                $contact_action_note = makeStringOverlibSafe($contact_action_note);

                // pop-up function call
                $contactOutput .= ("<a href='javascript:void(0);' onMouseOver=\"return overlib('".$contact_action_note."', CAPTION, 'Action Note', LEFT, VAUTO, STICKY, ol_closeclick);\" onMouseOut=\"nd();\">"); 
                $contactOutput .= ("<img border='0' src='misc/images/sticky_sm.gif' alt='' />");
                $contactOutput .= ("</a>");
                $contactOutput .= ("</td>\n");
            }
            else
            {
                $contactOutput .= ("<td nowrap='nowrap'>&nbsp;</td>");
            }

            //date of action
            $contactOutput .= ("<td nowrap='nowrap'>");
            $contactOutput .= ("<b class='black'>On:</b> " . formatShortDate($row_actions["action_on"]));
            $contactOutput .= ("</td>\n");
            $date[$count] = formatShortDate($row_actions["action_on"]);

            //action chosen
            $contactOutput .= ("<td nowrap='nowrap'>");
            if ($row_actions['action_id'] == GENERIC_NOTE)
            {
                $contactOutput .= ("<b class='black'>Action:</b> <b>" . $action_name . "</b>");
            } else {
                $contactOutput .= ("<b class='black'>Action:</b> " . $action_name);
            }
            $contactOutput .= ("</td>\n");

            //action performed by
            if($row_actions['action_id'] == INT_CONDUCTED)
            {
                $mini_sql = ("
                        SELECT interviewer_name
                        FROM interview_interviewer
                        WHERE interviewer_id='".$row_actions['action_by']."'
                        ");
                $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                $mini_row  = $mini_result->FetchRow();
                $display_name = $mini_row['interviewer_name'];
            }
            // 'By' field will contain interviewer names, not the staff memeber who entered the interview into system
            $contactOutput .= ("<td nowrap='nowrap'>");
            $contactOutput .= ("<b class='black'>By: </b>".$display_name);
            $contactOutput .= ("</td>\n");

            // method of action (do not display method column if action is "GENERIC NOTE" or if it's JOB_DESC_RECIEVED)
            $contactOutput .= ("<td nowrap='nowrap'>");
            $contactOutput .= (($row_actions["action_id"] == GENERIC_NOTE || $row_actions["action_id"] == JOB_DESC_RECEIVED)? "&nbsp;" : "<b class='black'>Method:</b> " . $action_method_name); 
            $contactOutput .= ("</td>\n");
            $contactOutput .= ("</tr>\n");
        }
        $contactOutput .= ("</table>");
    }

    return $contactOutput;
}
?>
