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
 | Filename: traffic_flag_editor.inc                                            |
 +------------------------------------------------------------------------------+
 | This is an interface for which the traffic director can use to edit and      |
 | delete flags                                                                 |
 +------------------------------------------------------------------------------+
*/
function deleteSharedFlag($flag_type, $flag_to_delete, $department_id, $remove_all)
{
    switch($flag_type)
    {
        case "History":
            //Check to see if department_history_flags table has the required shared flags
            $check_sql = "SELECT department_id
                          FROM department_history_flags
                          WHERE history_flags_id='".$flag_to_delete."'";
            if($remove_all[$flag_to_delete])
            {
                $check_result = $GLOBALS['dbh']->Execute($check_sql);    
            }
            else
            {
                $specific_dep_sql = $check_sql . " AND department_id='".$department_id."'";
                $check_result = $GLOBALS['dbh']->Execute($specific_dep_sql);
            } 
            
            //Delete all of the selected flags from the department_history_flags table (all or department_id)
            $delete_sql = "DELETE FROM department_history_flags
                           WHERE history_flags_id='".$flag_to_delete."'";
            if(!$remove_all[$flag_to_delete])
            {
                $delete_sql .= " AND department_id='".$department_id."'";
            }
            $delete_result = $GLOBALS['dbh']->Execute($delete_sql);
            //Check again to see if there are any flags left
            $check_result = $GLOBALS['dbh']->Execute($check_sql);
            
            $dept_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_to_delete, "dept_code"); 
            $update_sql = "UPDATE history_flags
                    SET comment='Shared ".implode(",",$dept_array)."'
                    WHERE history_flags_id='".$flag_to_delete."'"; 
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
            
            //no more shared flags in department_history_flags -> delete flag in history_flags table
            if($check_result->RecordCount() == 0)
            {
                $remove_flag_sql = "DELETE FROM history_flags
                                    WHERE history_flags_id='".$flag_to_delete."'";
                $remove_flag_result = $GLOBALS['dbh']->Execute($remove_flag_sql);
            }                         
        break;
        case "Student":
            $check_sql = "SELECT department_id
                          FROM department_student_flags
                          WHERE student_flags_id='".$flag_to_delete."'";
            if($remove_all[$flag_to_delete])
            {
                $check_result = $GLOBALS['dbh']->Execute($check_sql);    
            }
            else
            {
                $specific_dep_sql = $check_sql . " AND department_id='".$department_id."'";
                $check_result = $GLOBALS['dbh']->Execute($specific_dep_sql);
            } 
            
            $delete_sql = "DELETE FROM department_student_flags
                           WHERE student_flags_id='".$flag_to_delete."'";
            if(!$remove_all[$flag_to_delete])
            {
                $delete_sql .= " AND department_id='".$department_id."'";
            }
            $delete_result = $GLOBALS['dbh']->Execute($delete_sql);
            $check_result = $GLOBALS['dbh']->Execute($check_sql);
            
            $dept_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_to_delete, "dept_code"); 
            $update_sql = "UPDATE student_flags
                    SET comment='Shared ".implode(",",$dept_array)."'
                    WHERE student_flags_id='".$flag_to_delete."'"; 
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
            
            if($check_result->RecordCount() == 0)
            {
                $remove_flag_sql = "DELETE FROM student_flags
                    WHERE student_flags_id='".$flag_to_delete."'";
                $remove_flag_result = $GLOBALS['dbh']->Execute($remove_flag_sql);
            }                         
            break;
        case "Company":
                $check_sql = "SELECT department_id
                FROM department_company_flags
                WHERE flag_id='".$flag_to_delete."'";
            if($remove_all[$flag_to_delete])
            {
                $check_result = $GLOBALS['dbh']->Execute($check_sql);    
            }
            else
            {
                $specific_dep_sql = $check_sql . " AND department_id='".$department_id."'";
                $check_result = $GLOBALS['dbh']->Execute($specific_dep_sql);
            } 

            $delete_sql = "DELETE FROM department_company_flags
                WHERE flag_id='".$flag_to_delete."'";
            if(!$remove_all[$flag_to_delete])
            {
                $delete_sql .= " AND department_id='".$department_id."'";
            }
            $delete_result = $GLOBALS['dbh']->Execute($delete_sql);
            $check_result = $GLOBALS['dbh']->Execute($check_sql);

            $dept_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_to_delete, "dept_code"); 
            $update_sql = "UPDATE company_flags
                    SET comment='Shared ".implode(",",$dept_array)."'
                    WHERE flag_id='".$flag_to_delete."'"; 
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
            
            if($check_result->RecordCount() == 0)
            {
                $remove_flag_sql = "DELETE FROM company_flags
                    WHERE flag_id='".$flag_to_delete."'";
                $remove_flag_result = $GLOBALS['dbh']->Execute($remove_flag_sql);
            }                         
            break;
        case "Contact":
                $check_sql = "SELECT department_id
                FROM department_contact_flags
                WHERE flag_id='".$flag_to_delete."'";
            if($remove_all[$flag_to_delete])
            {
                $check_result = $GLOBALS['dbh']->Execute($check_sql);    
            }
            else
            {
                $specific_dep_sql = $check_sql . " AND department_id='".$department_id."'";
                $check_result = $GLOBALS['dbh']->Execute($specific_dep_sql);
            } 

            $delete_sql = "DELETE FROM department_contact_flags
                WHERE flag_id='".$flag_to_delete."'";
            if(!$remove_all[$flag_to_delete])
            {
                $delete_sql .= " AND department_id='".$department_id."'";
            }
            $delete_result = $GLOBALS['dbh']->Execute($delete_sql);
            $check_result = $GLOBALS['dbh']->Execute($check_sql);

            $dept_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_to_delete, "dept_code"); 
            $update_sql = "UPDATE contact_flags
                    SET comment='Shared ".implode(",",$dept_array)."'
                    WHERE flag_id='".$flag_to_delete."'"; 
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
            
            if($check_result->RecordCount() == 0)
            {
                $remove_flag_sql = "DELETE FROM contact_flags
                    WHERE flag_id='".$flag_to_delete."'";
                $remove_flag_result = $GLOBALS['dbh']->Execute($remove_flag_sql);
            }                         
            break;
        case "Division":
                $check_sql = "SELECT department_id
                FROM department_division_flags
                WHERE flag_id='".$flag_to_delete."'";
            if($remove_all[$flag_to_delete])
            {
                $check_result = $GLOBALS['dbh']->Execute($check_sql);    
            }
            else
            {
                $specific_dep_sql = $check_sql . " AND department_id='".$department_id."'";
                $check_result = $GLOBALS['dbh']->Execute($specific_dep_sql);
            } 

            $delete_sql = "DELETE FROM department_division_flags
                WHERE flag_id='".$flag_to_delete."'";
            if(!$remove_all[$flag_to_delete])
            {
                $delete_sql .= " AND department_id='".$department_id."'";
            }
            $delete_result = $GLOBALS['dbh']->Execute($delete_sql);
            $check_result = $GLOBALS['dbh']->Execute($check_sql);

            $dept_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_to_delete, "dept_code"); 
            $update_sql = "UPDATE contact_flags
                    SET comment='Shared ".implode(",",$dept_array)."'
                    WHERE flag_id='".$flag_to_delete."'"; 
            $update_result = $GLOBALS['dbh']->Execute($update_sql);
            
            if($check_result->RecordCount() == 0)
            {
                $remove_flag_sql = "DELETE FROM division_flags
                    WHERE flag_id='".$flag_to_delete."'";
                $remove_flag_result = $GLOBALS['dbh']->Execute($remove_flag_sql);
            }                         
            break;
    }
}

//if not set, default to Engineering and History flags type
//starts in add flags page by default
if($level1 == '')
$level1 = "add";
//Arts and writing since it is the first on the list
if(!isset($flag_type))
    $flag_type = "Company";

include('misc/flag_management/traffic_flag_header.inc');

//Array of flag types
$flag_array = array("Company","Contact","Division","History","Student");

// SQL queries
$dept_sql = "SELECT department_name, department_id
FROM department
WHERE job_list='1'
ORDER BY department_name ";
$dept_result = $GLOBALS['dbh']->Execute($dept_sql);

$continue = true;

//If there exists any flags to delete
if($delete)
{
    foreach($delete_shared AS $flag_to_delete)
    {
        deleteSharedFlag($flag_type, $flag_to_delete, $department_id, $remove_all);
    }
    //reset remove all flag
    $remove_all = false;
    $delete = false;
    
    notify("Flag has been successfully deleted.");
    echo "<br />";
}

if($save_changes)
{
    $error_count = 0;
    foreach($new_flag AS $id=>$name)
    {
        if(strlen($name) > 50)
        {
            error("\"".htmlspecialchars(stripslashes($name),ENT_QUOTES)."\" could not be updated because its length exceeded 50 characters.");
            $error_count++;
        }
        elseif(strlen($name) == 0)
        {
            error("The flag \"".htmlspecialchars(stripslashes($old_flag[$id]),ENT_QUOTES)."\" could not be changed because names must consist of at least 1 character. ");
            $error_count++;
        }
        else
        {
            
            //if the new flag doesn't match the old flag, do update
            if($name != $old_flag[$id])
            {
                switch($flag_type)
                {
                    case "Student":
                        $change_sql = "UPDATE student_flags
                        SET description='".$name."'
                        WHERE student_flags_id='".addslashes($id)."'";
                    break;
                    case "History":
                        $change_sql = "UPDATE history_flags
                        SET description='".$name."'
                        WHERE history_flags_id='".addslashes($id)."'";
                    break;
                    case "Company":
                        $change_sql = "UPDATE company_flags
                        SET flag_name='".$name."'
                        WHERE flag_id='".addslashes($id)."'";
                    break;
                    case "Contact":
                        $change_sql = "UPDATE contact_flags
                        SET flag_name='".$name."'
                        WHERE flag_id='".addslashes($id)."'";
                    break;
                    case "Division":
                        $change_sql = "UPDATE division_flags
                        SET flag_name='".$name."'
                        WHERE flag_id='".addslashes($id)."'";
                    break;
                }
                $change_result = $GLOBALS['dbh']->Execute($change_sql);
                notify("\"".htmlspecialchars(stripslashes($old_flag[$id]),ENT_QUOTES)."\" has been changed to \"".htmlspecialchars(stripslashes($name),ENT_QUOTES)."\"");
                $error_count++;
            }
        }
    }
    if($error_count)
        echo("<br />");
}
if($save_changes && $remove)
{
    include('traffic_flag_delete.inc');
}



if($continue){
    echo("<form method='post' name='myform' action='". $PHP_SELF ."'>");
    echo("<input type='hidden' name='select' value='flag_editor'>");
    
    switch($level1){

        case "edit_delete":
        echo("<table name='main1' class='row1' cellspacing='0' cellpadding='5' border='0'>");
        echo("<tr>");
        echo("<td align='left' valign='middle'>Department:&nbsp;");
        echo("<select name='department_id'>");
        while($row = $dept_result->FetchRow())
        {
            if(!isset($department_id))
                $department_id = $row["department_id"];
            echo("<option value='". $row["department_id"]. "' ");
            if($row["department_id"] == $department_id)
                echo("selected='selected'");
            echo(">");
            echo $row["department_name"]."</option>";
        }
        echo("</select>");
        echo("</td>");
        echo("</tr>");
        echo("<tr>");
        echo("<td align='left' valign='middle'>Flag Type:&nbsp;&nbsp;&nbsp;&nbsp;");

        echo("<select name='flag_type'>");
        foreach($flag_array as $flag_types)
        {
            echo("<option value='".$flag_types."' ");
            if($flag_types == $flag_type)
                echo("selected='selected'");
            echo(">");
            echo $flag_types."</option>";
        }

        echo("</td>");
        echo("</tr>");
        echo("<tr align='center'><td><hr /><input type='submit' name='submit' value='Go'></td>");
        echo("</tr>");
        echo("</table>");
        if($department_id)
        {
            switch($flag_type)
            {
                case "Student":    
                    $flag_sql = "SELECT f.description, df.student_flags_id
                    FROM student_flags AS f
                    INNER JOIN department_student_flags AS df
                    ON f.student_flags_id=df.student_flags_id
                    WHERE df.department_id='".addslashes($department_id)."'
                    AND f.flag_type_id='1'
                    ORDER BY f.description";
                break;
                case "Company":    
                    $flag_sql = "SELECT f.flag_name, df.flag_id
                    FROM company_flags AS f
                    INNER JOIN department_company_flags AS df
                    ON f.flag_id=df.flag_id
                    WHERE df.department_id='".addslashes($department_id)."'
                    ORDER BY f.flag_name";
                break;
                case "Division":    
                    $flag_sql = "SELECT f.flag_name, df.flag_id
                    FROM division_flags AS f
                    INNER JOIN department_division_flags AS df
                    ON f.flag_id=df.flag_id
                    WHERE df.department_id='".addslashes($department_id)."'
                    ORDER BY f.flag_name";
                break;
                case "Contact":    
                    $flag_sql = "SELECT f.flag_name, df.flag_id
                    FROM contact_flags AS f
                    INNER JOIN department_contact_flags AS df
                    ON f.flag_id=df.flag_id
                    WHERE df.department_id='".addslashes($department_id)."'
                    ORDER BY f.flag_name";
                break;
                case "History":    
                default:
                    $flag_sql = "SELECT f.description, df.history_flags_id
                        FROM history_flags AS f
                        INNER JOIN department_history_flags AS df
                        ON f.history_flags_id=df.history_flags_id
                        WHERE df.department_id='".addslashes($department_id)."'
                        ORDER BY f.description";
                    break;
            }        
            $flag_result = $GLOBALS['dbh']->Execute($flag_sql);
            $total_flags = $flag_result->RecordCount();
        }
        else
        {
            error("Please select a department");
        }   

        echo("<h4>Listing for ".$flag_type." Flags (".$total_flags.")&nbsp;</h4>");
        echo("<table name='main2' cellspacing='0' cellpadding='0' border='1'>");
        echo("<tr align='center'>");
        echo("<td>");
        echo("<table name='main' cellpadding='2' border='0'>");
        echo("<tr>");
        echo("<td class='rowgrey' align='center'><b class=white>&nbsp;Listing for ". $flag_type ." flags&nbsp;</b></td>");
        echo("<td class='rowgrey' align='center'><b class=white>&nbsp;Delete&nbsp;</b></td>");
        echo("<td class='rowgrey' align='center'><b class=white>&nbsp;Shared&nbsp;</b></td>");
        echo("</tr>");

        $rowclass = 0;
        while($flag_row = $flag_result->FetchRow())
        {
            switch($flag_type)
            {
                case "Student":
                    $shared_id = $flag_row["student_flags_id"];
                break;

                case "History":
                    $shared_id = $flag_row["history_flags_id"];
                break;

                case "Company":
                    case "Division":
                    case "Contact":
                default:
                    $shared_id = $flag_row["flag_id"];
                    break;
            } 
            echo("<input type='hidden' name='old_flag[".$shared_id."]' value='". (($flag_row["flag_name"])? htmlspecialchars(stripslashes($flag_row["flag_name"]),ENT_QUOTES) : htmlspecialchars(stripslashes($flag_row["description"]),ENT_QUOTES))."'>     ");
            echo("<tr>");
            echo("<td align='center' class='". (($rowclass % 2) ? "row0d" : "row1d" )."'>");
            echo("&nbsp;    ");
            echo("<input type='text' name='new_flag[".$shared_id."]' maxlength='50' size='50' value='". htmlspecialchars(stripslashes((($flag_row["flag_name"])? $flag_row["flag_name"] : $flag_row["description"])),ENT_QUOTES)."'>     ");
            echo("&nbsp;    ");
            echo("</td>");
            echo("<td align='center' class='". (($rowclass % 2) ? "row0d" : "row1d" )."'>");
            echo("&nbsp;");
            echo("<input type='checkbox' align='center' name='remove[]' value='". $shared_id ."'>");
            echo("&nbsp;    ");
            echo("</td>");
            echo("<td align='center' class='". (($rowclass % 2) ? "row0d" : "row1d" )."'>");
            if($flag_type && $department_id)
            {
                switch($flag_type)
                {
                    case "History":
                        $shared_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_row["history_flags_id"],"dept_code");
                    break;
                    case "Student":
                        $shared_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_row["student_flags_id"],"dept_code");
                    break;
                    default:
                        $shared_array = getFlagSharedDeptCode($flag_type, $department_id, $flag_row["flag_id"],"dept_code");
                    break;
                }
                if(sizeof($shared_array) > 0)
                    echo(implode(", ",$shared_array));
            }
            echo("</td>");
            echo("</tr>");
            $rowclass++;
        }
        echo("</table>");
        echo("<tr>");
        echo("<td class='". (($rowclass % 2) ? "row0d" : "row1d" )."' align='center'>");
        echo("<input type='submit' name='save_changes' value='Save Changes'>");
        echo("</td>");
        echo("</tr>");
        echo("</table>");
        echo("&nbsp;");
        break;
        
        case "add":
            include('misc/flag_management/traffic_flag_add.inc');
        break;
    }
    echo("<input type='hidden' name='level1' value='". $level1 ."'>");
    echo("</form>");
}
?>
