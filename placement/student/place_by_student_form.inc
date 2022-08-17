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
 | Filename: place_by_student_form.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file displays a form and allows the user to choose the     |
 | student that they would like to place. They may do so by typing in a student |
 | number, a name, or choosing from a select box.                               |
 +------------------------------------------------------------------------------+

*/

// Get a string of the department ids that belong to the user's group.
if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

//if $department_id is not set, then set it to the users dept and access the default settings (either GROUP or DEPT) of their drop-down menus for 
//proper drop-down menu display later
if($department_id=="")
{
    $sql = ("
        SELECT pulldown_menu_group 
        FROM contact_internal 
        WHERE department_id='".addslashes($auth->department)."' AND (login_id='".addslashes($login_id)."' OR netlink_id='".addslashes($login_id)."')
        ");
    
    $results=$GLOBALS['dbh']->Execute($sql);
    $row=$results->FetchRow();

    if ($row['pulldown_menu_group'] == 1)
    {
        //GROUP drop-down menu default
        $department_id = 0;
    }
    else
    {
        //DEPT drop-down menu default
        $department_id = $auth->department;
    }                          
}

if ($placement->student_number)
{
    $student_num = $placement->student_number;
}

?>

<script type="text/javascript" language="javascript">
<!--javascript

function triggerGoButtonEvent() {
    document.go_form.department_id.value = document.myform.department_id.value;
    document.go_form.student_num.value = document.myform.student_num.value;
    document.go_form.student_name.value = document.myform.student_name.value;
    document.go_form.submit();
}

//-->
</script>

<?php

echo("<h3>Job Offers - Place by Student</h3>");

if (sizeof($error_array) && is_array($error_array))
{
    foreach($error_array as $error_msg)
    {
        error($error_msg);
    }
}

echo("<form method='post' name='myform' action='" . $PHP_SELF . "&amp;select=placement_by_student'>");
echo("<input type='hidden' name='level' value='multiple_department_check' />");
echo("<table border='0' width='97%' class='row1' cellspacing='0' cellpadding='8'>");
echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<b class='black'>");

    echo("Please enter a student number, name, or choose a student from the list below.<br />");
    echo("  If entering a student's name, it must be done in the format <i>Last Name, First Name.</i><br />");
    echo("  You may enter only the beginning of either the first or last name, but they must be separated by a comma.");
    echo("<br />Also Select the term in which you want place the student.");
    echo("</b>");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td align='right'>");
    echo("Student Number:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' size='10' name='student_num' value='".stripslashes(htmlentities($student_num, ENT_QUOTES))."' />");
    echo("</td>");
echo("</tr>\n");

echo("<tr>");
    echo("<td align='right'>");
    echo("Student Name:");
    echo("</td>");

    echo("<td align='left'>");
    echo("<input type='text' value='" . stripslashes(htmlentities($student_name,ENT_QUOTES)) . "' size='30' name='student_name' />");
    echo("</td>");
echo("</tr>\n");

// Pull out all of the choices for the order_hrefs across the top.
$sql = ("
        SELECT DISTINCT SUBSTRING(s.last_name, 1, 1) AS order_letter
        FROM student AS s
        INNER JOIN student_department AS sd 
        ON sd.student_number=s.student_number
        INNER JOIN student_flags_join as sfj 
        ON sfj.record_id=sd.record_id
        WHERE s.last_name IS NOT NULL
        AND s.last_name!=''
        AND sd.department_id IN (" . $dept_string . ")
        AND sfj.student_flags_id='" . addslashes(CURRENT_FLAG) . "' 
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
    if($department_id == 0)
    {
        $order_href = ($PHP_SELF . "&amp;select=placement_by_student&amp;department_id=0");
    }
    elseif (in_array($department_id, $departments_in_group))
    {
        $order_href = ($PHP_SELF . "&amp;select=placement_by_student&amp;department_id=" . $department_id);
    }
    else
    {
        $order_href = ($PHP_SELF . "&amp;select=placement_by_student&amp;department_id=" . $auth->department);
    }

    for ($i = 0; $i < (sizeof($order_choices) - 1); $i++)
    {
        echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>");
        echo($order_choices[$i]);
        echo("</a> | ");
    }

    echo("<a class='blue' href='" . $order_href . "&amp;order=" . $order_choices[$i] . "'>");
    echo($order_choices[$i]);
    echo("</a>");

    echo("</td>");
echo("</tr>\n");

// Display all of the students that fit the chosen order letter.
$sql = ("
    SELECT s.student_number, sd.department_id, CONCAT(s.last_name, ', ', s.first_name) AS name
    FROM student AS s 
    INNER JOIN student_department AS sd 
    ON s.student_number=sd.student_number
    INNER JOIN student_flags_join as sfj 
    ON sfj.record_id=sd.record_id
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

if (!$result->RecordCount())
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

        echo("<select name='student_num_select' size='" . $size . "'>");
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

//if user has group, display drop-down menu of group's departments
if (sizeof($departments_in_group) > 1)
{
    echo("<tr>");
                                                                    
        echo("<td colspan='2' align='center'>Show students for:&nbsp;");
        echo("<select name='department_id'>");

        echo("<option value='0' ");
        echo(($department_id == 0) ? "selected='selected'" : "");
        echo(">Your Co-op Group</option>");

        foreach($departments_in_group as $dig)
        {
            echo("<option value='" . $dig . "' ");
            echo(($department_id == $dig) ? "selected='selected'" : "");
            echo(">" . getDepartmentName($dig) . "</option>");
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

if (!$term_id && !$year) {
    $nextTermYear = getNextTermYear();
    $term_id = $nextTermYear["term"];
    $year = $nextTermYear["year"];
}

echo("<tr>");
echo("<td align='center' colspan='2'>");
echo("<select name='term_id'>");

$sqlterm = ("
    SELECT term_id,term_name 
    FROM term 
    ORDER BY year_order
    ");
$sqlterm_result = $GLOBALS['dbh']->Execute($sqlterm);

while ($sqltermrow = $sqlterm_result->FetchRow()) {
    if ($sqltermrow['term_id'] == $term_id) {
        echo("<option value='".$sqltermrow['term_id']."' selected='selected'>".$sqltermrow['term_name']."</option>");
    } else {
        echo("<option value='".$sqltermrow['term_id']."'>".$sqltermrow['term_name']."</option>");
    }                                                                                                                                            
}
echo("</select>");
echo("&nbsp;");
echo("<select name='year'>");
/* reversed for loop to reverse year order in pulldown */
$date = date("Y");
for ($j=10;$j>-10;$j--){
    $the_year=$date+$j;
    if ((!$year && $j==0) || ($year == $the_year)){
        echo ("<option value='".$the_year."'  selected='selected'>".$the_year."</option>");
    }else{
        echo ("<option value='".$the_year."' >".$the_year."</option>");
    }
}
echo("</select>");
echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2' align='center'>");
        echo("<hr />");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td colspan='2' align='center'>");
    echo("<input type='submit' value='Place Student' />");
    echo("&nbsp;&nbsp;&nbsp;");
    echo("<input type='button' name='reset' value='Reset Form' onclick='javascript:document.reset_form.submit()' />");
    echo("</td>");
echo("</tr>\n");
echo("</table>");
echo("</form>");

echo("<form method='post' name='go_form' action='" . $PHP_SELF . "&amp;select=placement_by_student'>");
echo("<input type='hidden' name='continue' value='Go' />");
echo("<input type='hidden' name='department_id' value='' />");
echo("<input type='hidden' name='student_num' value='' />");
echo("<input type='hidden' name='student_name' value='' />");
echo("</form>");

echo("<form method='post' name='reset_form' action='" . $PHP_SELF . "&amp;select=placement_by_student'>");
echo("</form>"); 

?>
