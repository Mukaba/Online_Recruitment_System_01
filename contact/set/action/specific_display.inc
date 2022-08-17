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
 | Filename: specific_display.inc                                               |
 +------------------------------------------------------------------------------+
 | Description: This form is used to let the user edit multiple actions for a   |
 | specific contact, as well as to see the contact's action history.            |
 +------------------------------------------------------------------------------+

*/


$current_date = date("Y-m-d"); 

echo("<form method='post' action='" . $PHP_SELF . "' name='myform'>");

echo("<center>");
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

$sql = ("
	SELECT DISTINCT CONCAT(first_name, ' ', last_name) AS name
	FROM contact
	WHERE contact_id='" . $contact_id . "'
	");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$contact_name = $row["name"];

echo("<tr>");
	echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");

echo("<tr>");   
	echo("<td colspan='2' align='center'>Set Contact Action for: " . $contact_name . "</td>");
echo("</tr>\n");
        
echo("<tr>");
	echo("<td colspan='2' align='center'>");
        echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
            echo("<tr>");
                echo("<td align='right'>");
                echo("Action: ");
                echo("</td>");
                echo("<td align='left'>");
                
                $sql = ("
                    SELECT DISTINCT action_id, action_name
                    FROM action_types
                    WHERE action_id NOT IN ('".CONDUCTED_WORKSITE_VISIT."','".INT_CONDUCTED."','".FINAL_PLACEMENT."','".JOB_DESC_RECEIVED."')
                    ORDER BY action_name
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);

                echo("<select name='action'>");
                echo("<option value='' ");
                if (!$action)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["action_id"] . "' ");
                    if ($action == $row["action_id"])
                    {
                        echo("selected='selected'");
                    }
                    echo(">" . $row["action_name"] . "</option>");
                }
                echo("</select>");
                echo("</td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>");
                echo("Action Method: ");
                echo("</td>");
                echo("<td align='left'>");
                
                $sql = ("
                    SELECT DISTINCT action_method_id, action_method_name
                    FROM action_methods
                    ORDER BY action_method_id
                    ");
                $result = $GLOBALS['dbh']->Execute($sql);

                echo("<select name='action_method'>");
                echo("<option value='' ");
                if (!$action_method)
                {
                    echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["action_method_id"] . "' ");
                    if ($action_method == $row["action_method_id"])
                    {
                        echo("selected='selected'");
                    }
                    echo(">" . $row["action_method_name"] . "</option>");
                }
                echo("</select>");
                echo("</td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>");
                echo("Action performed by: ");
                echo("</td>");
                echo("<td align='left'>");
                
                $sql = ("
                        SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, c.contact_id, ci.netlink_id, ci.login_id
                        FROM contact AS c 
                        INNER JOIN contact_internal AS ci
                        ON c.contact_id=ci.contact_id
                        WHERE ci.department_id='" . $auth->department . "'
                        AND (ci.general_email='1' OR (('".$_SESSION['SESSION_netlog']."' = '" . NIS_LOGIN . "' AND ci.login_id = '".$auth->login."')
                        OR ('".$_SESSION['SESSION_netlog']."' = '" . SCRIPT_LOGIN . "' AND ci.netlink_id = '".$auth->login."')))
                        ORDER BY c.last_name
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);

                echo("<select name='action_by'>");
                while ($row = $result->FetchRow())
                {
                    echo("<option value='" . $row["contact_id"] . "' ");
                    if (
                    $action_by == $row["contact_id"]
                    || (!$action_by && (($_SESSION['SESSION_netlog'] == NIS_LOGIN && $row["login_id"] == $auth->login) || 
                    ($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $row["netlink_id"] == $auth->login)))
                    )
                    {
                        echo("selected='selected'");
                    }
                    echo(">" . $row["name"] . "</option>");
                }
                echo("</select>");
                echo("</td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>");
                echo("Date of Action (YYYY-MM-DD): ");
                echo("</td>");
                echo("<td align='left'>");
                echo("<input type='text' name='action_on' value='" . (($action_on) ? $action_on : $current_date) . "' />");
                echo("&nbsp;".popup('action_on','myform'));
                echo("</td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>Enter new note here:</td>");
                echo("<td align='left'><textarea name='action_note' rows='5' cols='40'>" . stripslashes($action_note) . "</textarea></td>");
            echo("</tr>");
            echo("<tr>");
                echo("<td align='right'>");
                echo("Bring Forward Date (YYYY-MM-DD): ");
                echo("</td>");
                echo("<td align='left'>");
                echo("<input type='text' name='bring_forward_date' value='" . $bring_forward_date . "' />");
                echo("&nbsp;".popup('bring_forward_date','myform'));
                echo("</td>");
            echo("</tr>");
        echo("</table>");
	echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2'><hr /></td>");
echo("</tr>");
echo("<tr align='center'>");
    echo("<td width='50%' align='right'>");
	echo("<input type='hidden' name='department_contact_flags' value='" . packObject(($department_contact_flags)) . "' />");
	echo("<input type='hidden' name='continue' value='save_specific' />");
    echo("<input type='hidden' name='select' value='set_contact_flag' />");
    echo("<input type='hidden' name='level1' value='actions' />");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
    echo("<input type='hidden' name='start_row' value='".$start_row."' />");
	echo("<input type='submit' value='Save Changes' />");
	echo("</td>");
    echo("<td width='50%' align='left'>");
    echo("<input type='button' value='Back to List' onclick='javascript:document.backtolist_form.submit()' />");
    echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");
echo("</form>");

echo ("<form name='backtolist_form' method='post' action='" . $PHP_SELF . "'>");
    echo("<input type='hidden' name='select' value='set_contact_flag' />");
    echo("<input type='hidden' name='level1' value='actions' />");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
    echo("<input type='hidden' name='form_submitted' value='true' />");
    echo("<input type='hidden' name='continue' value='Back to List' />");
    echo("<input type='hidden' name='start_row' value='".$start_row."' />");
echo("</form>");

// Variables for paging. 
if ($per_page_max == "") { $per_page_max = 25; }
if ($per_page_max < 5) { $per_page_max = 5; }
if ($limit == '') { $limit = 0; }

$sql = ("
    SELECT DISTINCT cas.action_id, cas.contact_id, cas.action_by, cas.action_on, cas.action_method_id, cas.action_note, cas.bring_forward_date 
    FROM contact_actions_set AS cas 
    WHERE cas.contact_id = '" .$contact_id. "' 
    ORDER BY action_on DESC, cas.contact_actions_id DESC
    ");
$result = $GLOBALS['dbh']->Execute($sql);

$action_row = array();
if ($result->RecordCount())
{
    while ($row = $result->FetchRow())
    {
        $action_row[] = $row;
    }
}

$row_count = sizeof($action_row);

$pages = ceil($row_count / $per_page_max);

$first = $limit + 1;
$end = $limit + $per_page_max;                                                                                                           
if ($end > $row_count)
{
    $end = $row_count;
}

echo("<br />");
echo("<br />");

//Display History of Actions

if ($row_count > $per_page_max)
{
    echo("<div align='left'>");
    echo("<form method='post' name='paging_form' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>\n");
    echo("<input type='hidden' name='form_submitted' value='true' />\n");
    echo("<input type='hidden' name='level1' value='actions' />\n");
    echo("<input type='hidden' name='continue' value='show_specific' />\n");
    echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");

    echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

    echo("<tr>");
        echo("<td>");
        echo($row_count . " action" . (($row_count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";");
        echo("</td>");

        echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");
        echo(" actions per page.");
        echo("</td>");
    echo("</tr>");                                                                                                                           

    echo("<tr>");                                                                                                                            
        echo("<td>");                                                                                                                            

        echo("<select name='limit'>");                                                                                                       

        for ($i = 0; $i < $pages; $i++)
        {
            $row_num_start = $i * $per_page_max;
            $row_num_end = $row_num_start + $per_page_max - 1;
            if ($row_num_end >= $row_count)
            {
                $row_num_end = $row_count - 1;
            }

            // What we display in the paging's drop down menu.
            $action_start = $action_row[$row_num_start]["action_on"];
            $action_end = $action_row[$row_num_end]["action_on"];

            echo("<option value='" . $row_num_start . "' ");
            if ($limit == $row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . formatShortDate($action_end) . " to " . formatShortDate($action_start) . "</option>\n");
        }
        
        echo("</select>"); 

        echo("</td>");

        echo("<td align='right'>");                                                                                                              
            echo("<input type='submit' value='Go' />");
        echo("</td>");
    echo("</tr>\n");
    echo("</table>\n");
    echo("</form>");
    echo("</div>");
}


if ($row_count)
{
    echo("<form method='post' name='delete_form' action='" . $PHP_SELF . "'>");

echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
	echo("<tr>");
        echo("<td align='center'>&nbsp;</td>");
	echo("</tr>\n");

    echo("<tr>");
        echo("<td align='center'>");
        echo($contact_name . "'s History of Actions: ");
        echo("</td>");
    echo("</tr>\n");
	
	echo("<tr>");
		echo("<td>");

	        echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");

            echo("<tr>");
                echo("<td align='center' colspan='7'>&nbsp;</td>");
            echo("</tr>\n");

            // Get the first name and last name of the user's contact_id. need to verify the netlink_id of the performer (action_by_name) matches each action
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
       
            $count = 0; // keep a counter to identify the actions displayed 

            for ($i=$limit; $i<$end; $i++)
            {
                $row = $action_row[$i];

                // Get action name
                $mini_sql = ("
                    SELECT brief_action_name 
                    FROM action_types
                    WHERE action_id = '" . $row['action_id']. "'
                    ");
                $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                $mini_row = $mini_result->FetchRow();
                $action_name = $mini_row["brief_action_name"];

                // Get action method name
                // Note: action methods introduced at later development date, thus some actions methods were "Never Set"
                $mini_sql =("
                    SELECT action_method_name
                    FROM action_methods
                    WHERE action_method_id = '" .$row["action_method_id"]. "'
                    ");
                $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                $mini_row = $mini_result->FetchRow();
                $action_method_name = ($mini_row["action_method_name"]!="" ? $mini_row["action_method_name"] : "Never Set");
           
                // get name of action_by
                $mini_sql = ("
                    SELECT CONCAT(first_name,' ', last_name) AS name
                    FROM contact
                    WHERE contact_id='".$row['action_by']."'
                    ");
                $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                $mini_row = $mini_result->FetchRow();
                $display_name = $mini_row['name'];
                
                // Display our query results
                echo("<tr>");
                    // Display bring_forward_date in pop-up form if this action had one 
                    if(($row["bring_forward_date"]!='0000-00-00') && ($row["bring_forward_date"] != NULL) && ($row["bring_forward_date"] > $current_date))
                    {
                        echo("<td nowrap='nowrap'>");
                            echo("<input type='hidden' name='bring_forward[".$count."]' value='".$row['bring_forward_date']."' />");
                            
                            // pop-up function call 
                            echo("<a href='javascript:void(0);' onmouseover=\"return overlib('".$row['bring_forward_date']."', CAPTION, 'Bring Forward Date', LEFT, VAUTO, STICKY, ol_closeclick);\" onmouseout=\"nd();\">");
                            echo("<img border='0' src='misc/images/clock.gif' alt='' />");
                        echo("</a>");                    
                        echo("</td>\n");
                    }
                    else
                    {
                        echo("<td nowrap='nowrap'>&nbsp;");
                        echo("<input type='hidden' name='bring_forward[".$count."]' value='' />");
                        echo("</td>");
                    }
                    
                    // Display note in pop-up form if this action had one 
                    if($row["action_note"])
                    {
                        echo("<td nowrap='nowrap'>");
                            $contact_action_note = trim($row['action_note']);
                            echo("<input type='hidden' name='note[".$count."]' value='".packObject(($row['action_note']))."' />");
                            $contact_action_note = addslashes($contact_action_note);
                            
                            //  newlines, carriage returns, and double quotes passed into overlib() function must be changed to work properly
                            $contact_action_note = str_replace("\"", "'", $contact_action_note);
                            $contact_action_note = str_replace("\r", "", $contact_action_note);
                            $contact_action_note = str_replace("\n", "<br />", $contact_action_note);
                            
                            // pop-up function call 
                            echo("<a href='javascript:void(0);' onmouseover=\"return overlib('".$contact_action_note."', CAPTION, 'Action Note', LEFT, VAUTO, STICKY, ol_closeclick);\" onmouseout=\"nd();\">");
                            echo("<img border='0' src='misc/images/sticky_sm.gif' alt='' />");
                        echo("</a>");                    
                        echo("</td>\n");
                    }
                    else
                    {
                        echo("<td nowrap='nowrap'>&nbsp;");
                        echo("<input type='hidden' name='note[".$count."]' value='".packObject(($row['action_note']))."' />");
                        echo("</td>");
                    }
                    
                    //date of action
                    echo("<td nowrap='nowrap'>");
                    echo("<b class='black'>On:</b> ".formatShortDate($row["action_on"]));
                    echo("<input type='hidden' name='date[".$count."]' value='".$row['action_on']."' />");
                    echo("</td>\n");
                    $date[$count] = formatShortDate($row["action_on"]);

                    //action chosen
                    echo("<td nowrap='nowrap'>");
                    if ($row['action_id'] == GENERIC_NOTE)
                    {
                        echo("<b class='black'>Action:</b> <b>" . $action_name . "</b>");
                    } else {
                        echo("<b class='black'>Action:</b> " . $action_name);
                    }
                    echo("<input type='hidden' name='action_chosen[".$count."]' value='".$row['action_id']."' />");
                    echo("<input type='hidden' name='action_name_chosen[".$count."]' value='".$action_name."' />");
                    echo("</td\n>");
                    
                    //action performed by
                    if($row['action_id'] == INT_CONDUCTED)
                    {  
                        $mini_sql = ("
                            SELECT interviewer_name
                            FROM interview_interviewer
                            WHERE interviewer_id='".$row['action_by']."'
                            ");
                        $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                        $mini_row  = $mini_result->FetchRow();
                        $display_name = $mini_row[0];
                    }
                    // 'By' field will contain interviewer names, not the staff memeber who entered the interview into system
                    echo("<td nowrap='nowrap'>");
                    echo("<b class='black'>By:</b> ".$display_name);
                    echo("<input type='hidden' name='action_by[".$count."]' value='".$row['action_by']."' />");
                    echo("<input type='hidden' name='action_by_name[".$count."]' value='".$display_name."' />");
                    echo("</td>\n");
                    
                    // method of action (do not display method column if action is "GENERIC NOTE" or if it's JOB_DESC_RECIEVED)
                    echo("<td nowrap='nowrap'>");
                    echo(($row["action_id"] == GENERIC_NOTE || $row["action_id"] == JOB_DESC_RECEIVED)? "&nbsp;" : "<b class='black'>Method:</b> " . $action_method_name);
                    echo("<input type='hidden' name='method[".$count."]' value='".(($row['action_method_id'] !=NULL)? $row['action_method_id']: NULL) ."' />");
                    echo("<input type='hidden' name='method_name[".$count."]' value='".(($action_method_name !=NULL)? $action_method_name : NULL) ."' />");
                    echo("</td>\n");
                   
                    $mini_sql = ("
                            SELECT netlink_id
                            FROM contact_internal
                            WHERE contact_id='".$row['action_by']."'
                            ");
                    $mini_result = $GLOBALS['dbh']->Execute($mini_sql);
                    $mini_row = $mini_result->FetchRow();
                   
                    // "delete" checkbox, cant delete other's or autmomated statuses. Can delete others if you're traffic or director's office
                    if($mini_row['netlink_id'] == $verify_netlink_id || $auth->department == TRAFFIC_DIRECTOR_DEPARTMENT || $auth->department == DIRECTORS_OFFICE_DEPARTMENT)
                    {
                        if(!($row['action_id'] == CONDUCTED_WORKSITE_VISIT || $row['action_id'] == JOB_DESC_RECEIVED || $row['action_id'] == INT_CONDUCTED || $row['action_id'] == FINAL_PLACEMENT))
                        {
                            echo("<td nowrap='nowrap'>");
                            echo("<input type='checkbox' name='delete_action[".$count++."]' value='".$count."' />");
                            echo("</td>\n");
                        } else {
                            echo("<td nowrap='nowrap'>");
                            echo("&nbsp;");
                            echo("</td>\n");
                        }
                    }
                    else
                    {
                        echo("<td nowrap='nowrap'>");
                        echo("&nbsp;");
                        echo("</td>\n");
                    }
                echo("</tr>\n");
            }
            echo("</table>");
            echo("<input type='hidden' name='num_actions' value='".$count."' />");
            echo("<input type='hidden' name='select' value='set_contact_flag' />");
            echo("<input type='hidden' name='level1' value='actions' />");
            echo("<input type='hidden' name='contact_id' value='" . $contact_id . "' />");
            echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
		echo("</td>");
	echo("</tr>\n");
    if($count != 0)
    {
        echo("<tr>");
        echo("<td><hr /></td>");
        echo("</tr>");

        echo("<tr align='center'>");
            echo("<td>");
            echo("<input type='hidden' name='continue' value='delete_actions' />");
            echo("<input type='hidden' name='deleted' value='no' />");
            echo("<input type='submit' value='Delete Selected Actions' />");
            echo("</td>");
        echo("</tr>");
    }
    else
    {
        echo("<tr>");
        echo("<td>&nbsp;</td>");
        echo("</tr>");
    }
echo("</table>");
echo("</form>");
}
else
{
echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");
    echo("<tr>");
		echo("<td colspan='2' align='center'>");
	        echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
            echo("<tr>");
                echo("<td colspan='3'>&nbsp;</td>");
            echo("</tr>\n");
            echo("<tr>");
                echo("<td align='center' colspan='3'>");
                echo($contact_name . " does not currently have any actions set.");
                echo("</td>");
            echo("</tr>\n");
            echo("<tr>");
                echo("<td align='center' colspan='3'>&nbsp;</td>");
            echo("</tr>\n");
            echo("</table>");
        echo("</td>");
    echo("</tr>\n");
echo("</table>");
}


?>
