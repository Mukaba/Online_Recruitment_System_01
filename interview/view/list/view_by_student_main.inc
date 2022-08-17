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
 | Filename: view_by_student_main.inc                                           |
 +------------------------------------------------------------------------------+
 | Description: This file displays a form and allows the user to choose the     |
 | student that they would like to view. They may do so by typing in a student  |
 | number, a name, or choosing from a select box.                               |
 +------------------------------------------------------------------------------+

*/

// TODO: In the 'myform', there are two elements named student_num which is invalid.
//       That needs to be changed.

// Get a string of the department ids that belong to the user's group.
if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

?>

<script type="text/javascript" language="javascript">
<!--javascript

function triggerGoButtonEvent() {
    document.go_form.department_id.value = document.myform.department_id.value;
    document.go_form.submit();
}

//-->
</script>

<?php

$go_button = "false";
echo("<form method='post' name='myform' action='" . $PHP_SELF . "&amp;select=view'>");
echo("<input type='hidden' name='level1' value='student' />");
echo("<input type='hidden' name='level2' value='student_num_or_student_name' />");
echo("<table border='0' width='97%' class='row1' cellspacing='0' cellpadding='8'>");

echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>");
    echo("Please enter a student number, name, or choose a student from the list below.<br />");
    echo("  If entering a student's name, it must be done in the format <i>Last Name, First Name.</i><br />");
    echo("  You may enter only the beginning of either the first or last name, but they must be separated by a comma.");
    echo("</b>");
    echo("</td>");
echo("</tr>\n");
echo("<tr>");
    echo("<td align='right'>");
    echo("Student Number:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' value='" . htmlspecialchars(stripslashes($student_num),ENT_QUOTES) . "' size='10' name='student_num' />");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td align='right'>");
    echo("Student Name:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' value='" . htmlspecialchars(stripslashes($student_name),ENT_QUOTES). "' size='30' name='student_name' />");
    echo("</td>");
echo("</tr>\n");

if($department_id != TRAFFIC_DIRECTOR_DEPARTMENT)
{
    // Pull out all of the choices for the order_hrefs across the top.
    $sql = ("
            SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS order_letter
            FROM student AS s
            INNER JOIN student_department AS sd ON sd.student_number=s.student_number
            INNER JOIN student_flags_join as sfj ON sfj.record_id=sd.record_id
            WHERE s.last_name IS NOT NULL
            AND s.last_name!=''
            AND sd.department_id IN (" . $dept_string . ")
            AND sfj.student_flags_id='" . CURRENT_FLAG . "' 
            ORDER BY s.last_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        if (!is_array($order_choices) || !in_array(strtoupper($row["order_letter"]), $order_choices))
        {
            $order_choices[] = strtoupper($row["order_letter"]);
        }
    }

    if (!$order)
    {
        $order = $order_choices[0];
    }


    echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<input type='hidden' name='order' value='" . $order . "' />");

    // Display the order hrefs.
    if (in_array($department_id, $departments_in_group))
    {
        $order_href = ($PHP_SELF . "&amp;select=view&amp;department_id=" . $department_id);
    }
    else
    {
        $order_href = ($PHP_SELF . "&amp;select=view&amp;department_id=" . $auth->department);
    }

    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "&amp;level1=student'>");
        echo($order_choices[$i]);
        echo("</a> | ");
    }

    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "&amp;level1=student'>");
    echo($order_choices[$i]);
    echo("</a>");

    echo("</td>");
    echo("</tr>\n");

    // Display all of the students that fit the chosen order letter.
    $sql = ("
            SELECT s.student_number, sd.department_id, sd.record_id, CONCAT(s.last_name, ', ', s.first_name) AS name
            FROM student AS s 
            INNER JOIN student_department AS sd ON s.student_number=sd.student_number
            INNER JOIN student_flags_join as sfj ON sfj.record_id=sd.record_id
            WHERE s.last_name LIKE '" . $order . "%' 
            AND sfj.student_flags_id='" . CURRENT_FLAG . "' 
            ");

    if ($department_id && in_array($department_id, $departments_in_group))
    {
        $sql .= ("
                AND sd.department_id='" . $department_id . "'
                ");
    }
    else
    {
        $sql .= ("
                AND sd.department_id IN (" . $dept_string . ")
                ");
    }

    $sql .= ("
            ORDER BY s.last_name, s.first_name
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
}

if($department_id == TRAFFIC_DIRECTOR_DEPARTMENT)
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("Please select a department below");
        echo("</td>");
    echo("</tr>\n");

}    
elseif (!$result->RecordCount())
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("There are no students in your department with last names that start with the chosen letter.");
        echo("</td>");
    echo("</tr>\n");
}
else
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>");

        $size = 10;
        if ($result->RecordCount() < 10)
        {
            $size = $result->RecordCount();

            // hack to prevent browser from automatically choosing the first student in the select box when we have one student
            // returned from the query 
            if ($size == 1) { 
                $size++;        
            }
        }

        echo("<select name='student_num' size='" . $size . "'>");
        while ($row = $result->FetchRow())
        {
            echo("<option value='" . $row["student_number"] . "'>");
            echo(($row["department_id"] == $auth->department && sizeof($departments_in_group) > 1) ? "*" : "");
            echo("[" . $row["student_number"] . "] " . $row["name"] . "</option>");
        }
        echo("</select>");

        echo("</td>");
    echo("</tr>\n");
}
if (sizeof($departments_in_group) > 1)
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>Show students for:&nbsp;");
        echo("<select name='department_id'>");

        if($auth->department == TRAFFIC_DIRECTOR_DEPARTMENT)
        {
            //only pull out departments that contain jobs and interviews
            $sql = "SELECT DISTINCT d.department_id, d.department_name 
                FROM job_info AS ji
                INNER JOIN department AS d
                ON ji.department_id=d.department_id
                WHERE ji.interview_year != 0 
                AND d.job_list
                ORDER BY department_name
                ";
            $result = $GLOBALS['dbh']->Execute($sql);
            
            // to have the first department selected by default
            echo("<option selected='selected' value=''>&nbsp;</option>");
            while ($row = $result->FetchRow())
            {       
                if ($row["department_id"] == $department_id)
                {
                    echo "<option selected='selected' value='" . $row["department_id"] . "'>" . $row["department_name"] . "</option>\n";
                }                                                                                                                                        
                else                                                                                                                                     
                {                                                                                                                                        
                    echo "<option value='" . $row["department_id"] . "'>" . $row["department_name"]. "</option>\n";                                      
                }                                                                                                                                        

            }     
            
        }
        else{
            if (sizeof($departments_in_group) > 1)
            {
                echo("<option value='0' ");
                echo(($department_id == 0) ? "selected='selected'" : "");
                echo(">Your Co-op Group</option>");
            }

            foreach($departments_in_group as $dig)
            {
                echo("<option value='" . $dig . "' ");
                echo(($department_id == $dig) ? "selected='selected'" : "");
                echo(">" . getDepartmentName($dig) . "</option>");
            }
        }
        echo("</select>");
        echo("&nbsp;&nbsp;");
        echo("<input type='button' value='Go' onclick='javascript:triggerGoButtonEvent();' />");
        echo("</td>");
    echo("</tr>\n");
}

if (sizeof($departments_in_group) > 1) 
{
    echo("<tr>");
        echo("<td colspan='2' align='center'>");
        echo("Note: students with an asterisk (*) are in your department.");
        echo("</td>");
    echo("</tr>\n");
}


echo("<tr>");
    echo("<td colspan='2' align='center'>Display Past Interviews:&nbsp;<input type='checkbox' name='past' value='true'");
    if ($past == true) {
        echo("checked='checked'");
    }
    echo(" /><br /></td>");
echo("</tr>");
    
echo("<tr>");
    echo("<td colspan='2' align='center'>");
        echo("<hr />");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2' align='center'>");
        echo("<input type='submit' name='submit' value='View Interviews' />");
        echo("&nbsp;&nbsp;&nbsp;");
        echo("<input type='reset' name='reset' value='Reset Form' />");
    echo("</td>");
echo("</tr>");

echo("</table>");
echo("</form>");
echo("<form method='post' name='go_form' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view' />");
echo("<input type='hidden' name='level1' value='student' />");
echo("<input type='hidden' name='level2' value='student_num_or_student_name' />");
echo("<input type='hidden' name='department_id' value='' />");
echo("<input type='hidden' name='go_button' value='Go' />");
echo("</form>");

?>
