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
 | Filename: general_display.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file is used to show the user a list of contacts, along    |
 | with criteria to narrow down this list. This file also gives the user the    |
 | ability to set multiple actions at once for a large group of contacts.       |
 +------------------------------------------------------------------------------+

 :TODO: This file is not pulling out statuses correctly for a contact. The file
        should reflect how contact advanced search pulls out statuses; it allows
        users to select the status and department for the contact as search
        parameters. 
*/

if ($checked_flags && is_string($checked_flags))
{
    $checked_flags = unpackObject(($checked_flags));
}

// The initial form we get to when a user goes to set actions. 

if (!$form_submitted)
{
    echo("<form method='post' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>");
    echo("<input type='hidden' name='level1' value='actions' />");
    echo("<input type='hidden' name='form_submitted' value='true' />");

    echo("<table border='0' cellpadding='2' cellspacing='0'>");
    echo("<tr align='center'>");
    echo("<td class='row1'>");
    echo("<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
    echo("<tr align='center'>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>Show contacts that match the criteria below:<br />(Leave any fields blank that you do not wished to be used in the search)</b>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'><hr width='50%' size='1' /></td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("First name starts with: ");
    echo("<input type='text' name='first_name' size='10' value='" . $first_name . "' />");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("Last name starts with: ");
    echo("<input type='text' name='last_name' size='10' value='" . $last_name . "' />");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("Company starts with: ");
    echo("<input type='text' name='company_name' size='10' value='" . $company_name . "' />");
    echo("</td>");
    echo("</tr>");

    $sql = ("
            SELECT status_id, status_name
            FROM employer_info_statuses
            ORDER BY level_of_activity
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    // :TODO: Current when searching on a contact's status, we check to see if the target contact has a status we selected in *ANY* department. This should be like
    //        contact advanced search where you can select which statuses you want to look for, and which departments those statuses belong to.
    echo("Status of Contact: ");
    echo("<select name='contact_status_id'>");
    echo("<option value='' ");
    if (!$contact_status_id)
    {
        echo("selected='selected'");
    }
    echo(">&nbsp;</option>");
    while ($row = $result->FetchRow())
    {
        echo("<option value='" . $row["status_id"] . "' ");
        if ($contact_status_id == $row["status_id"])
        {
            echo("selected='selected'");
        }
        echo(">" . $row["status_name"] . "</option>");
    }
    echo("</select>");
    echo("</td>");
    echo("</tr>");

    echo("<tr>");
    echo("<td colspan='2'>&nbsp;</td>");
    echo("</tr>");

    $sql = ("
            SELECT DISTINCT cf.flag_id, cf.flag_name
            FROM contact_flags AS cf
            INNER JOIN department_contact_flags AS dcf 
            ON cf.flag_id = dcf.flag_id
            WHERE dcf.department_id='" . $auth->department . "'
            ORDER BY cf.flag_name");
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount())
    {

        echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("Contacts with these flags checked:");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td colspan='2'>");
        echo("<table align='center' cellpadding='3' cellspacing='3'>");
        echo("<tr align='center'>");
        echo("<td align='center' class='row2'>");
        echo("<table width='100%' class='row2'>");

        $newrow = 0;

        while ($row = $result->FetchRow())
        {		
            if (!$newrow)
            {
                echo("<tr>");
            }

            echo("<td>");
            echo("<input type='checkbox' class='row1' name='checked_flags[]' value='" . $row["flag_id"] . "' ");
            if (is_array($checked_flags) && in_array($row["flag_id"], $checked_flags))
            {
                echo("checked='checked'");
            }
            echo(" />");
            echo($row["flag_name"]);
            echo("</td>");

            if ($newrow)
            {
                echo("</tr>");
            }

            $newrow = (($newrow) ? 0 : 1);
        }

        if ($newrow)
        {
            echo("<td>&nbsp;</td>");
            echo("</tr>");
        }

        echo("</table>");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td colspan='2'>&nbsp;</td>");
        echo("</tr>");

    }

    echo("<tr>");
    echo("<td colspan='2' align='center'><hr />");
    echo("<input type='submit' value='Go' />");
    echo("</td>");
    echo("</tr>");

    echo("</table>");
    echo("</td>");
    echo("</tr>");
    echo("</table>");
    echo("</form>");
}

// This block of code displays the results of a search. The results page is where one can set actions. 

if ($form_submitted)
{
    ?>

    <script type="text/javascript" language="javascript">
    <!--javascript

    function setDateCurrent()
    {

        for (var i = 0; i < document.myform.elements.length; i++)
        {
            var e = document.myform.elements[i];
            if (e.type == 'text')
            {
                var f = document.myform.elements[(i - 2)];

                if (f.selectedIndex != 0)
                {

                    if (document.myform.set_or_clear.value == 'set')
                    {
                        e.value = document.myform.current_date.value;
                    }
                    else
                    {
                        e.value = '';
                    }

                }
                else
                {
                    e.value = '';
                }
            }
            // dont set the bring forward date to today's date
            if(i == (document.myform.elements.length-4))
            {
                e.value ='';
            }
        }
        if (document.myform.set_or_clear.value == 'set')
        {
            document.myform.set_or_clear.value = 'clear';
        }
        else
        {
            document.myform.set_or_clear.value = 'set';
        }
    }

    // javascript function to SET ALL actions
    function syncActions()
    {
        for (var i = 0; i < document.myform.elements.length; i++)
        { 
            var action_pattern = /action\[.+\]/;
            var e = document.myform.elements[i];

            var action_result = (e.name).match(action_pattern);
            
            if(action_result != null)
            {
                e.selectedIndex = document.myform.allselect.selectedIndex;
            }
        }
    }

    // javascript function to SET ALL method actions
    function syncActions1()
    {
         for (var i = 0; i < document.myform.elements.length; i++)
         {
            var pattern = /action_method\[.+\]/;
             var e = document.myform.elements[i];
             var result = (e.name).match(pattern);
            
            if(result != null)
            { 
                //verify that an synymous action has been set
                var f = document.myform.elements[(i - 1)];
                if (f.selectedIndex != 0)
                {
                     e.selectedIndex = document.myform.allselect1.selectedIndex;
                }
                else
                {
                    e.selectedIndex = 0;
                }
            }
        }
    }

    function viewSpecific(contactId)
    {
        document.specificViewForm.contact_id.value = contactId;
        document.specificViewForm.submit();
    }

    //-->

    </script>

    <?php

    if ($contact_id_list)
    {
        $sql = ("
                SELECT DISTINCT c.contact_id, c.last_name, c.first_name, ec.company_name
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON c.contact_id = ce.contact_id
                INNER JOIN employer_company AS ec
                ON ec.employer_id = ce.employer_id
                WHERE ce.deleted_flag = '0'
                AND ce.contact_id IN (".$contact_id_list.")
                ");
        
        if ($order)
        {
            $sql .= ("
                    ORDER BY " . $order . "
                    ");
        }
        else
        {
            $sql .= ("
                    ORDER BY c.last_name, c.first_name, ec.company_name
                    ");
        }

        $result = $GLOBALS['dbh']->Execute($sql);

        while ($row = $result->FetchRow()) 
        {
            $contact_row[] = $row;
        }
    }
    else
    {
        // Pull the contacts out of the database, along with the flags requested.
        $sql = ("
                SELECT DISTINCT c.contact_id, c.last_name, c.first_name, ec.company_name
                FROM contact AS c
                INNER JOIN contact_employer AS ce
                ON c.contact_id = ce.contact_id
                INNER JOIN employer_company AS ec
                ON ec.employer_id = ce.employer_id
                ");
        if (trim($contact_status_id))
        {
            $sql .= ("
                LEFT JOIN department_contact_status dcs
                ON ce.contact_id = dcs.contact_id
                ");
        }

        // These are flags that users are searching for.
        if (sizeof($checked_flags) > 0 && is_array($checked_flags))
        {
            for ($i=0; $i < sizeof($checked_flags); $i++)
            {
                $x = $i - 1;

                // In the first iteration, we have to join with the contact_employer table.
                if ($i==0)
                {
                    $sql .= ("
                            INNER JOIN contact_flags_set cfs".$i."
                            ON ce.contact_id = cfs".$i.".contact_id
                            ");
                }

                // Otherwise, join contact_flags_set with the last contact_flags_set table.
                else
                {
                    $sql .= ("
                            INNER JOIN contact_flags_set cfs".$i."
                            ON cfs".$x.".contact_id = cfs".$i.".contact_id
                            ");
                }
            }
        }

        $sql .= ("
            WHERE ce.deleted_flag = '0'
            ");


        if (trim($first_name))
        {
            $sql .= ("
                    AND c.first_name LIKE '" . addslashes(removeslashes(trim($first_name))) . "%'
                    ");
        }

        if (trim($last_name))
        {
            $sql .= ("
                    AND c.last_name LIKE '" . addslashes(removeslashes(trim($last_name))) . "%'
                    ");
        }

        if (trim($company_name))
        {
            $sql .= ("
                    AND ec.company_name LIKE '" . addslashes(removeslashes(trim($company_name))) . "%'
                    ");
        }

        if (trim($contact_status_id))
        {
            $sql .= ("
                    AND dcs.status_id='" . $contact_status_id . "'
                    AND dcs.department_id='" . $auth->department . "'
                    AND dcs.current_status = '1'
                    ");
        }

        // Search for contacts with certain flags checked.                                                                                           
        if (sizeof($checked_flags) > 0 && is_array($checked_flags))                                                                                  
        {                                                                                                                                            
            for ($i=0; $i < sizeof($checked_flags); $i++)                                                                                            
            {                                                                                                                                        
                $sql .= ("                                                                                                                           
                        AND cfs".$i.".flag_id = '".$checked_flags[$i]."'                                                                                 
                        ");                                                                                                                              
            }                                                                                                                                        
        }

        if ($order)
        {
            $sql .= ("
                    ORDER BY " . $order . "
                    ");
        }
        else
        {
            $sql .= ("
                    ORDER BY c.last_name, c.first_name, ec.company_name
                    ");
        }

        $result = $GLOBALS['dbh']->Execute($sql);

        $contact_id_list = array();

        while ($row = $result->FetchRow()) 
        {
            $contact_row[] = $row;
            $contact_id_list[] = $row['contact_id'];
        }

        $contact_id_list = implode(",",$contact_id_list);
    }

    if (is_array($error_array) && sizeof($error_array))
    {
        foreach($error_array as $error_msg)
        {
            error($error_msg);
        }
    }

    // This form is submitted when a user clicks on a specific user.
    echo("<form name='specificViewForm' method='post' action='" . $PHP_SELF . "'>\n");
    echo("<input type='hidden' name='select' value='set_contact_flag' />\n");
    echo("<input type='hidden' name='continue' value='show_specific' />\n");
    echo("<input type='hidden' name='level1' value='actions' />\n");
    echo("<input type='hidden' name='contact_id' value='' />\n");
    echo("<input type='hidden' name='contact_id_list' value='".$contact_id_list."' />\n");
    echo("<input type='hidden' name='start_row' value='".$start_row."' />");
    echo("</form>\n");

    if (!sizeof($contact_row)) 
    {
        notify("There are no contacts in the database that match the criteria you have entered.");
    }
    else
    {
        // Variables for paging. 
        if ($per_page_max == "") { $per_page_max = 25; }
        if ($per_page_max < 5) { $per_page_max = 5; }
        if ($start_row == '') { $start_row = 0; }

        $row_count = sizeof($contact_row);

        $pages = ceil($row_count / $per_page_max);
                                                                                                                                                             
        $first = $start_row + 1;
        $end = $start_row + $per_page_max;
        if ($end > $row_count)
        {
            $end = $row_count;
        }

        if ($row_count > 0)
        {
            echo("<form method='post' name='paging_form' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>\n");
            echo("<input type='hidden' name='form_submitted' value='true' />\n");
            echo("<input type='hidden' name='level1' value='actions' />\n");
            echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />\n");
            echo("<input type='hidden' name='order' value='" . $order . "' />\n");

            echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

            echo("<tr>");
            echo("<td>");                                                                                                                            
            echo($row_count . " contact" . (($row_count != 1) ? "s" : "") . " on " . $pages . " page" . (($pages > 1) ? "s" : "").";</td>");                 
            echo("<td><input type='text' name='per_page_max' size='4' maxlength='4' value='" . $per_page_max . "' />");                                
            echo(" contacts per page.");                                                                                                             
            echo("</td>");                                                                                                                           
            echo("</tr>");                                                                                                                           

            echo("<tr>");                                                                                                                            

            echo("<td>");                                                                                                                            
            if ($order)
            {
                // $order usually equals something like c.last_name,c.first_name,ec.company_name
                // The regular expression matches the one or more letters, followed by a . and then we grab everything between the first . and the following , 
                // The string between these the . and , must contain either letters or underscores. The matched string is stored into the back reference \\1.
                // Using the example above, we'd match last_name
                $order_column_name = preg_replace("/\w+\.([\w|_]+),.*/","\\1",$order);

                if ($order_column_name != "last_name" && $order_column_name != "company_name")
                {
                    $order_column_name = NULL;
                }
            }
            echo("<select name='start_row'>");                                                                                                       

            for ($i = 0; $i < $pages; $i++)                                                                                                          
            {                                                                                                                                        
                $row_num_start = $i * $per_page_max;                                                                                                 
                $row_num_end = $row_num_start + $per_page_max - 1;                                                                                   
                if ($row_num_end >= $row_count)                                                                                                          
                {                                                                                                                                    
                    $row_num_end = $row_count - 1;                                                                                                       
                }                                                                                                                                    

                // What we display in the paging's drop down menu.                                                                                   
                if ($order_column_name && $order)                                                                                                    
                {                                                                                                                                    
                    $contact_start = $contact_row[$row_num_start][$order_column_name];                                                                       
                }                                                                                                                                    
                else                                                                                                                                 
                {                                                                                                                                    
                    $contact_start = $contact_row[$row_num_start]['last_name'];                                                                              
                }                                                                                                                                    

                if (strlen($contact_start) > 15)                                                                                                     
                {                                                                                                                                    
                    $contact_start = substr($contact_start, 0, 13) . "...";                                                                          
                }                                                                                                                                    

                // What we display in the paging's drop down menu.                                                                                   
                if ($order_column_name && $order)                                                                                                    
                {                                                                                                                                    
                    $contact_end = $contact_row[$row_num_end][$order_column_name];
                }                                                                                                                                    
                else                                                                                                                                 
                {                                                                                                                                    
                    $contact_end = $contact_row[$row_num_end]['last_name'];                                                                                  
                }                                                                                                                                    

                if (strlen($contact_end) > 15)                                                                                                       
                {                                                                                                                                    
                    $contact_end = substr($contact_end, 0, 13) . "...";                                                                              
                }                                                                                                                                    

                echo("<option value='" . $row_num_start . "' ");                                                                                     
                if ($start_row == $row_num_start)                                                                                                    
                {                                                                                                                                    
                    echo("selected='selected'");                                                                                                            
                }                                                                                                                                    
                echo(">" . $contact_start . " to " . $contact_end . "</option>\n");                                                                  
            }                                                                                                                                        

            echo("</select>");                                                                                                                       
            echo("</td>");                                                                                                                           

            echo("<td align='right'>");                                                                                                              
            echo("<input type='submit' value='Go' />");                                                                 
            echo("</td>");                                                                                                                           

            echo("</tr>");                                                                                                                           

            echo("</table>");                                                                                                                        
            echo("</form>");                                                                                                                         
        }

        // Set up arrays that will be used to populate drop down menus. 
        $sql = ("
                SELECT action_id, action_name
                FROM action_types
                WHERE action_id NOT IN ('".CONDUCTED_WORKSITE_VISIT."','".INT_CONDUCTED."','".FINAL_PLACEMENT."','".JOB_DESC_RECEIVED."')
                ORDER by action_name
                ");
        $action_result = $GLOBALS['dbh']->Execute($sql);
        while ($action_row = $action_result->FetchRow())
        {
            $actions_to_do[] = $action_row;
        }

        $sql = ("
                SELECT DISTINCT action_method_id, action_method_name
                FROM action_methods
                ORDER by action_method_id
                ");
        $action_result = $GLOBALS['dbh']->Execute($sql);
        while ($action_methods_row = $action_result->FetchRow())
        {
            $action_methods_to_do[] = $action_methods_row;
        }

        $sql = ("
                SELECT DISTINCT CONCAT(c.first_name, ' ', c.last_name) AS name, c.contact_id, ci.netlink_id, ci.login_id
                FROM contact_internal AS ci, contact AS c
                WHERE ci.department_id='" . $auth->department . "'
                AND c.contact_id=ci.contact_id
                AND (ci.general_email='1' OR (('".$_SESSION['SESSION_netlog']."' = '" . NIS_LOGIN . "' AND ci.login_id = '".$auth->login."')
                        OR ('".$_SESSION['SESSION_netlog']."' = '" . SCRIPT_LOGIN . "' AND ci.netlink_id = '".$auth->login."')))
                ORDER BY c.last_name, c.first_name
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while($row = $result->FetchRow())
        {
            $action_performed_for[] = $row;
        }

        echo("<form method='post' name='myform' action='" . $PHP_SELF . "&amp;select=set_contact_flag&amp;level1=actions'>\n");
        // Create a current date hidden input, so that the user can set all actions to current date.
        echo("<input type='hidden' name='set_or_clear' value='set' />");
        echo("<input type='hidden' name='current_date' value='" . date("Y-m-d") . "' />");

        echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />\n");
        echo("<input type='hidden' name='order' value='" . $order . "' />\n");
        echo("<input type='hidden' name='start_row' value='" . $start_row . "' />\n");

        echo("<table cellspacing='0' cellpadding='0' border='1'>");
        echo("<tr>");
        echo("<td>");
        echo("<table border='0' cellpadding='2'>");
        echo("<tr>");
        
        // When a column title is clicked, the results will order by that field. This form handles that.
        echo("<td class='rowgrey' align='center' nowrap='nowrap'>&nbsp;&nbsp;");
        echo("<a class='orderable' href='javascript:document.form1.submit()'><b class='white'>Contact Name</b>&nbsp;&nbsp;</a>");
        echo("</td>");
        echo("<td class='rowgrey' align='center' nowrap='nowrap'>&nbsp;&nbsp;");
        echo("<a class='orderable' href='javascript:document.form2.submit()'><b class='white'>Company Name</b>&nbsp;&nbsp;</a>");
        echo("</td>");
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;<b class='white'>Action</b>&nbsp;&nbsp;</td>");
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;<b class='white'>Action Method</b>&nbsp;&nbsp;</td>");
        echo("<td class='rowgrey' align='center'>&nbsp;&nbsp;<b class='white'>Date of Action</b>&nbsp;&nbsp;</td>");
        echo("</tr>");

        $rowclass = 0;

        $count = 0;
        $contact_ids = array();

        for ($x=$start_row; $x<$end; $x++)
        {
            $row = $contact_row[$x];
            
            // The contact_ids array is used to keep track of which contact id's were including by the criteria, so we have
            // an idea which contacts to set and unset flags for when told to do so by the user.

            $contact_ids[] = $row["contact_id"];

            echo("<tr>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<a class='blue' href='javascript:viewSpecific(" . $row["contact_id"] . ")'>" . $row["last_name"].", ".$row["first_name"] . "</a>&nbsp;</td>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<a class='blue' href='javascript:viewSpecific(" . $row["contact_id"] . ")'>" . $row["company_name"] . "</a>&nbsp;</td>");
            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center' nowrap='nowrap'>&nbsp;&nbsp;");
            echo("<select name='action[" . $count . "]'>");
            echo("<option value='' ");
            if (!$action[$count])
            {
                echo("selected='selected'");
            }
            echo(">&nbsp;</option>");
            for ($i = 0; $i < sizeof($actions_to_do); $i++)
            {
                echo("<option value='" . $actions_to_do[$i]["action_id"] . "' ");
                if ($action[$count] == $actions_to_do[$i]["action_id"])
                {
                    echo("selected='selected'");
                }
                echo(">" . $actions_to_do[$i]["action_name"] . "</option>");
            }
            echo("</select>");
            echo("&nbsp;&nbsp;");
            echo("</td>");

            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center' nowrap='nowrap'>&nbsp;&nbsp;");
            echo("<select name='action_method[" . $count . "]'>");
            echo("<option value='' ");
            if (!$action_method[$count])
            {
                echo("selected='selected'");
            }
            echo(">&nbsp;</option>");
            for ($i = 0; $i < sizeof($action_methods_to_do); $i++)
            {
                echo("<option value='" . $action_methods_to_do[$i]["action_method_id"] . "' ");
                if ($action_method[$count] == $action_methods_to_do[$i]["action_method_id"])
                {
                    echo("selected='selected'");
                }
                echo(">" . $action_methods_to_do[$i]["action_method_name"] . "</option>");
            }

            echo("</select>");
            echo("</td>");

            echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>&nbsp;&nbsp;");
            echo("<input type='text' name='action_date[" . $count . "]' size='12' maxlength='15' value='");
            echo($action_date[$count]);
            echo("' />&nbsp;");
            $variable = "action_date[".$count."]";
            echo(popup($variable,'myform'));
            echo("</td>");
            echo("</tr>\n");

            $rowclass = (($rowclass) ? 0 : 1);
            $count++;
        }

        echo("<tr>");
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='2' align='center'>");
        echo("Setting Actions For: ");
        echo("<select name='action_for'>");
        for ($i=0; $i<sizeof($action_performed_for); $i++)
        {
            echo("<option value='" . $action_performed_for[$i]["contact_id"] . "' ");
            if (($action_for == $action_performed_for[$i]["contact_id"]) 
                    || (!$action_for && ($_SESSION['SESSION_netlog'] == NIS_LOGIN && $action_performed_for[$i]["login_id"] == $auth->login) 
                        || ($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $action_performed_for[$i]["netlink_id"] == $auth->login)))
            {
                echo("selected='selected'");
            }
            echo(">" . $action_performed_for[$i]["name"] . "</option>");

        }
        
        echo("</select>");
        echo("</td>");
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
        echo("Set all the above actions to: ");
        echo("<select name='allselect' onchange='syncActions()'>");
        echo("<option value='' ");
        if (!$action[($row["contact_id"])])
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        for ($i = 0; $i < sizeof($actions_to_do); $i++)
        {
            echo("<option value='" . $actions_to_do[$i]["action_id"] . "' ");
            if ($action[($row["contact_id"])] == $actions_to_do[$i]["action_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $actions_to_do[$i]["action_name"] . "</option>");
        }
        echo("</select>");
        echo("</td>");
        
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
        echo("Set all the above action methods to: ");
        echo("<select name='allselect1' onchange='syncActions1()'>");
        echo("<option value='' ");
        if (!$action_method[($row["contact_id"])])
        {
            echo("selected='selected'");
        }
        echo(">&nbsp;</option>");
        for ($i = 0; $i < sizeof($action_methods_to_do); $i++)
        {
            echo("<option value='" . $action_methods_to_do[$i]["action_method_id"] . "' ");
            if ($action_method[($row["contact_id"])] == $action_methods_to_do[$i]["action_method_id"])
            {
                echo("selected='selected'");
            }
            echo(">" . $action_methods_to_do[$i]["action_method_name"] . "</option>");
        }
        echo("</select>");
        echo("</td>");

        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
        echo("<input type='button' value='Set/Unset Current Date' onclick='setDateCurrent()' />");
        echo("</td>"); 
        echo("</tr>");
        
        echo("<tr align='center'>");
        
        $rowclass = (($rowclass) ? 0 : 1);
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='2' align='center'>Enter new note here:</td>");
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='3' align='center'><textarea name='action_note' rows='3' cols='70'>" .stripslashes($action_note). "</textarea>");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        $rowclass = (($rowclass) ? 0 : 1);
        echo("<td COLSPAN ='5' class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>");
        echo("Bring Forward Date(YYYY-MM-DD): ");
        echo("<input type='text' name='bring_forward_date' value='" . $bring_forward_date . "' />");
        echo("&nbsp;".popup('bring_forward_date','myform'));
        echo("</td>");
        echo("</tr>"); 

        echo("<tr>");
        $rowclass = (($rowclass) ? 0 : 1);
        echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' colspan='5' align='center'>");
        echo("<input type='hidden' name='contact_ids' value='" . packObject(($contact_ids)) . "' />");
        echo("<input type='hidden' name='continue' value='save_general' />");
        echo("<input type='submit' value='Save Changes' />");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</form>");

        echo("<div align='center'>-OR-</div>");
        echo("<br />");

        if (is_array($error_array) && sizeof($error_array))
        {
            foreach($error_array as $error_msg)
            {
                error($error_msg);
            }
        }

        // Start of second form which allows a user to set actions for ALL matched contacts.

        echo("<form method='post' action='" . $PHP_SELF . "' name='set_all_actions'>");
        echo("<input type='hidden' name='select' value='set_contact_flag' />");
        echo("<input type='hidden' name='level1' value='actions' />");
        echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />");
        echo("<input type='hidden' name='start_row' value='".$start_row."' />");
        echo("<input type='hidden' name='continue' value='save_actions_for_all' />");

        echo("<center>");
        echo("<table border='0' cellpadding='5' cellspacing='0' width='100%' class='row1'>");

        echo("<tr>");
            echo("<td colspan='2'>&nbsp;</td>");
        echo("</tr>");

        echo("<tr>");   
            echo("<td colspan='2' align='center'>Set Actions for ALL Matched Contacts</td>");
        echo("</tr>\n");
                
        echo("<tr>");
            echo("<td colspan='2' align='center'>");
                echo("<table cellpadding='5' cellspacing='0' border='0' class='row0' align='center'>\n");
                    echo("<tr>");
                        echo("<td align='right'>");
                        echo("Action: ");
                        echo("</td>");
                        echo("<td align='left'>");
                        
                        echo("<select name='all_action_id'>");
                        echo("<option value='' ");
                        if (!$all_action_id)
                        {
                            echo("selected='selected'");
                        }
                        echo(">&nbsp;</option>");
                        for ($i = 0; $i < sizeof($actions_to_do); $i++)
                        {
                            echo("<option value='" . $actions_to_do[$i]["action_id"] . "' ");
                            if ($all_action_id == $actions_to_do[$i]["action_id"])
                            {
                                echo("selected='selected'");
                            }
                            echo(">" . $actions_to_do[$i]["action_name"] . "</option>");
                        }
                        echo("</select>");
                        echo("</td>");
                    echo("</tr>");
                    echo("<tr>");
                        echo("<td align='right'>");
                        echo("Action Method: ");
                        echo("</td>");
                        echo("<td align='left'>");
                        
                        echo("<select name='all_action_method'>");
                        echo("<option value='' ");
                        if (!$all_action_method)
                        {
                            echo("selected='selected'");
                        }
                        echo(">&nbsp;</option>");
                        for ($i = 0; $i < sizeof($action_methods_to_do); $i++)
                        {
                            echo("<option value='" . $action_methods_to_do[$i]["action_method_id"] . "' ");
                            if ($all_action_method == $action_methods_to_do[$i]["action_method_id"])
                            {
                                echo("selected='selected'");
                            }
                            echo(">" . $action_methods_to_do[$i]["action_method_name"] . "</option>");
                        }
                        echo("</select>");
                        echo("</td>");
                    echo("</tr>");
                    echo("<tr>");
                        echo("<td align='right'>");
                        echo("Action performed by: ");
                        echo("</td>");
                        echo("<td align='left'>");
                        
                        echo("<select name='all_action_by'>");
                        for ($i=0; $i<sizeof($action_performed_for); $i++)
                        {
                            echo("<option value='" . $action_performed_for[$i]["contact_id"] . "' ");
                            if (
                            $all_action_by == $action_performed_for[$i]["contact_id"]
                            || (!$all_action_by && (($_SESSION['SESSION_netlog'] == NIS_LOGIN && $action_performed_for[$i]["login_id"] == $auth->login) || 
                            ($_SESSION['SESSION_netlog'] == SCRIPT_LOGIN && $action_performed_for[$i]["netlink_id"] == $auth->login)))
                            )
                            {
                                echo("selected='selected'");
                            }
                            echo(">" . $action_performed_for[$i]["name"] . "</option>");
                        }
                        echo("</select>");
                        echo("</td>");
                    echo("</tr>");
                    echo("<tr>");
                        echo("<td align='right'>");
                        echo("Date of Action (YYYY-MM-DD): ");
                        echo("</td>");
                        echo("<td align='left'>");
                        echo("<input type='text' name='all_action_on' value='" . (($all_action_on) ? $all_action_on : $current_date) . "' />");
                        echo("&nbsp;".popup('all_action_on','set_all_actions'));
                        echo("</td>");
                    echo("</tr>");
                    echo("<tr>");
                        echo("<td align='right'>Enter new note here:</td>");
                        echo("<td align='left'><textarea name='all_action_note' rows='5' cols='40'>" . stripslashes($all_action_note) . "</textarea></td>");
                    echo("</tr>");
                    echo("<tr>");
                        echo("<td align='right'>");
                        echo("Bring Forward Date (YYYY-MM-DD): ");
                        echo("</td>");
                        echo("<td align='left'>");
                        echo("<input type='text' name='all_bring_forward_date' value='" . $all_bring_forward_date . "' />");
                        echo("&nbsp;".popup('all_bring_forward_date','set_all_actions'));
                        echo("</td>");
                    echo("</tr>");
                echo("</table>");
            echo("</td>");
        echo("</tr>");

        echo("<tr>");
            echo("<td colspan='2'><hr /></td>");
        echo("</tr>");

        echo("<tr align='center'>");
            echo("<td colspan='2'>");
            echo("<input type='submit' value='Save Changes' />");
            echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("</center>");
        echo("</form>");
    }

    echo("<form method='post' name='form1' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>\n");
    echo("<input type='hidden' name='form_submitted' value='true' />\n");
    echo("<input type='hidden' name='level1' value='actions' />\n");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />\n");
    echo("<input type='hidden' name='order' value='c.last_name,c.first_name,ec.company_name' />\n");
    echo("</form>");

    echo("<form method='post' name='form2' action='" . $PHP_SELF . "&amp;select=set_contact_flag'>\n");
    echo("<input type='hidden' name='form_submitted' value='true' />\n");
    echo("<input type='hidden' name='level1' value='actions' />\n");
    echo("<input type='hidden' name='contact_id_list' value='" . $contact_id_list . "' />\n");
    echo("<input type='hidden' name='order' value='ec.company_name,c.last_name,c.first_name' />\n");
    echo("</form>");
}	

?>
