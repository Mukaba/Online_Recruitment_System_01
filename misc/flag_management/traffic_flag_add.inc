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
 | Filename: traffic_flag_add.inc                                               |
 +------------------------------------------------------------------------------+
 | This page allows the user to add flags                                       |
 +------------------------------------------------------------------------------+
*/

//Function: addSharedFlag
//Params: insert_flag_name - input name of flag to be added
//        checked_flags - array storing the flags that were checked off
function addSharedFlag($flag_type,$insert_flag_name,$checked_flags)
{
    if(strlen($insert_flag_name) > 50)
    {
        error("Flag name can be at most 50 characters long.");
    }
    else
    {
        if(sizeof($checked_flags) > 0)
        {
            foreach($checked_flags AS $cf)
            {       
                $checked_codes[] = getDepartmentCode($cf);
            }
        }
        $insert = false;

        $count_sql = "SELECT COUNT(department_id) AS count
            FROM department
            WHERE job_list='1'
            "; 
            $count_result = $GLOBALS['dbh']->Execute($count_sql);
        $count_row = $count_result->FetchRow(); 

        switch($flag_type)
        {   
            case "Student": 
                //inserting flag into main flags table
                $insert_sql = "INSERT INTO student_flags (description, flag_type_id, comment)
                VALUES ('".$insert_flag_name."','1','";

                if(sizeof($checked_flags) > 0)
                {
                    $insert_sql .= (sizeof($checked_flags) > 1 ? "Shared " : ""); 
                    $insert_sql .= ((sizeof($checked_flags) == $count_row["count"]) ? "All": implode(",",$checked_codes));
                }
                $insert_sql .= "')";
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);

                //retrieve the id just inserted  
                $sql = "SELECT LAST_INSERT_ID() AS cur_id
                ";
                $result = $GLOBALS['dbh']->Execute($sql); 
                $row = $result->FetchRow();

                //insert all of the department ids that were checked off on the add list 
                if(sizeof($checked_flags) > 0)
                {
                    foreach($checked_flags AS $shared_dep_id)
                    {
                        $shared_sql = "INSERT INTO department_student_flags (department_id, student_flags_id)
                        VALUES ('".$shared_dep_id."','".$row["cur_id"]."')";
                        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);
                    }
                }
                break;
            case "History":                                                                                                                              
                $insert_sql = "INSERT INTO history_flags (description, comment)                                                                          
                VALUES ('".$insert_flag_name."','";                                    

                if(sizeof($checked_flags) > 0)
                {
                    $insert_sql .= (sizeof($checked_flags) > 1 ? "Shared " : ""); 
                    $insert_sql .= ((sizeof($checked_flags) == $count_row["count"]) ? "All": implode(",",$checked_codes));
                }
                $insert_sql .= "')";
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);                                                                                                  

                $sql = "SELECT LAST_INSERT_ID() AS cur_id                                                                                           
                ";                                                                                                            
                $result = $GLOBALS['dbh']->Execute($sql);                                                                                                                
                $row = $result->FetchRow();                                                                                                          

                if(sizeof($checked_flags) > 0)
                {
                    foreach($checked_flags AS $shared_dep_id)                                                                                                
                    {                                                                                                                                        
                        $shared_sql = "INSERT INTO department_history_flags (department_id, history_flags_id)                                                
                        VALUES ('".$shared_dep_id."','".$row["cur_id"]."')";                                                                  
                        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);                                                                                              
                    }                                                                                                                                        
                }
                break;                                                                                                                                       
            case "Company":                                                                                                                              
                $insert_sql = "INSERT INTO company_flags (flag_name, comment) 
                VALUES ('".$insert_flag_name."','";                                   

                if(sizeof($checked_flags) > 0)
                {
                    $insert_sql .= (sizeof($checked_flags) > 1 ? "Shared " : ""); 
                    $insert_sql .= ((sizeof($checked_flags) == $count_row["count"]) ? "All": implode(",",$checked_codes));
                }
                $insert_sql .= "')";
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);                                                                                                  

                $sql = "SELECT LAST_INSERT_ID() AS cur_id                                                                                                    
                ";                                                                                                            
                $result = $GLOBALS['dbh']->Execute($sql);                                                                                                                
                $row = $result->FetchRow();                                                                                                          
                if(sizeof($checked_flags) > 0)
                {
                    foreach($checked_flags AS $shared_dep_id)                                                                                                
                    {                                                                                                                                        
                        $shared_sql = "INSERT INTO department_company_flags (department_id, flag_id)                                                         
                        VALUES ('".$shared_dep_id."','".$row["cur_id"]."')";                                                                  
                        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);                                                                                              
                    }
                }
                break;
            case "Contact":                                                                                                                              
                $insert_sql = "INSERT INTO contact_flags (flag_name, comment)                                                                            
                VALUES ('".$insert_flag_name."','";

                if(sizeof($checked_flags) > 0)
                {
                    $insert_sql .= (sizeof($checked_flags) > 1 ? "Shared " : ""); 
                    $insert_sql .= ((sizeof($checked_flags) == $count_row["count"]) ? "All": implode(",",$checked_codes));
                }
                $insert_sql .= "')";
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);                                                                                                  

                $sql = "SELECT LAST_INSERT_ID() AS cur_id                                                                                                    
                ";                                                                                                            
                $result = $GLOBALS['dbh']->Execute($sql);                                                                                                                
                $row = $result->FetchRow();                                                                                                          

                if(sizeof($checked_flags) > 0)
                {
                    foreach($checked_flags AS $shared_dep_id)                                                                                                
                    {                                                                                                                                        
                        $shared_sql = "INSERT INTO department_contact_flags (department_id, flag_id)                                                         
                        VALUES ('".$shared_dep_id."','".$row["cur_id"]."')";                                                                  
                        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);                                                                                              
                    }                                                                                                                                        
                }
                break;                                                                                                                                       
            case "Division":                                                                                                                             
                $insert_sql = "INSERT INTO division_flags (flag_name, comment)                                                                           
                VALUES ('".$insert_flag_name."','";                                  

                if(sizeof($checked_flags) > 0)
                {
                    $insert_sql .= (sizeof($checked_flags) > 1 ? "Shared " : ""); 
                    $insert_sql .= ((sizeof($checked_flags) == $count_row["count"]) ? "All": implode(",",$checked_codes));
                }
                $insert_sql .= "')";
                $insert_result = $GLOBALS['dbh']->Execute($insert_sql);                                                                                                  

                $sql = "SELECT LAST_INSERT_ID() AS cur_id                                                                                                    
                ";                                                                                                           
                $result = $GLOBALS['dbh']->Execute($sql);                                                                                                                
                $row = $result->FetchRow();                                                                                                          

                if(sizeof($checked_flags) > 0)
                {
                    foreach($checked_flags AS $shared_dep_id)                                                                                                
                    {                                                                                                                                        
                        $shared_sql = "INSERT INTO department_division_flags (department_id, flag_id)                                                        
                        VALUES ('".$shared_dep_id."','".$row["cur_id"]."')";                                                                  
                        $shared_result = $GLOBALS['dbh']->Execute($shared_sql);                                                                                              
                    }                                                                                                                                        
                }
                break;                                                                                                                                       

            default:                                                                                                                                     
                error("A flag type must be selected.");                                                                                                   
                break;                                                                                                                                   
        }                                                                                                                                                
        notify("Flag has been successfully added.&nbsp;");
    }
}
if($insert)
{
    if($insert_flag_name)
    {       
        if(strlen($insert_flag_name) <= 50)
        {
            if(sizeof($checked_flags) == 0)
            {
                error("Must select a department.");
                echo("<br />");
            }
            else
            {
                addSharedFlag($flag_type, $insert_flag_name, $checked_flags);
                unset($insert_flag_name);
                unset($checked_flags); 
                echo("<br />");
            }
        }
        else
        {
            error("Flag name can be at most 50 characters long.");
            echo("<br />");
        }
    }
    else
    {
        if(sizeof($checked_flags) == 0)
        {
            error("Must select a department.");
        }
        error("Must input a flag name.");
        echo("<br />");
    }
}

echo "<form method='post' name='myform2' action='".$PHP_SELF."&amp;select=flag_add'>";

echo("<input type='hidden' name='level1' value='add'>");
echo("<table name='main3' class='row1' cellspacing='0' cellpadding='5' border='0'>");
echo("<tr align='center'>");
echo("<td>");
echo("<table name='main' cellpadding='2' border='0'>");
echo("<tr>");
echo("<td colspan='2' align='left' valign='middle'>Flag name:&nbsp;");
echo("<input type='text' size='50'  name='insert_flag_name' value='".htmlspecialchars(stripslashes($insert_flag_name),ENT_QUOTES)."'>");
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td align='left' valign='middle'>Flag Type:&nbsp;&nbsp;&nbsp;&nbsp;");
echo("<select name='flag_type'>");
foreach($flag_array as $flag_types)
{
    echo("<option value='".$flag_types."' ");
    if($flag_types == $flag_type)
    {
        echo("selected='selected'");                                                                                                         
    }
    echo(">");
    echo $flag_types."</option>";
}
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2' align='center' valign='middle'>Departments sharing this flag:");
echo("</td>");
echo("</tr>");
echo("<tr>");
echo("<td colspan='2'>&nbsp;</td>");
echo("</tr>");
$newrow = 0;
if($dept_result)
{
    $dept_result->Move(0);
}
else
{
    error("No results round.");
}
while($shared_depts = $dept_result->FetchRow())
{
    if (!$newrow)
    {
        echo("<tr>");
    }
    echo("<td>");
    
    if(sizeof($checked_flags) > 0)
    {
        echo("<input type='checkbox' name='checked_flags[]' ".((in_array($shared_depts["department_id"],$checked_flags))? "checked='checked'":"")." value='".$shared_depts["department_id"]."'>");
    }
    else
    {
        echo("<input type='checkbox' name='checked_flags[]' value='".$shared_depts["department_id"]."'>");
    }
    echo($shared_depts["department_name"]);
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
echo("<tr align='center'><td colspan='2'><hr /><input type='submit' name='insert' value='Insert'></td></tr>");
echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");
?>
