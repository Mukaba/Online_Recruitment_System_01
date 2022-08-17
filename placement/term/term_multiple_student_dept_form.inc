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
 | Filename: term_multiple_student_dept_form.inc                                |
 +------------------------------------------------------------------------------+
 | Description: This file take a list of students that are in multiple          |
 |              departments, and shows the user a list of departments the       |
 |              student is current in. Assumes the existence of                 |
 |              $arr_multiple_dept which is an array of placement objects.      |
 +------------------------------------------------------------------------------+

*/

assert($arr_multiple_dept);

echo("<h3 align='left'>Job Offers - Place by Term</h3>");
if (sizeof($error_array) && is_array($error_array))
{
    foreach($error_array as $error_msg)
    {
        error($error_msg);
    }
}

$msg = ("The students that you have entered are current in multiple departments. ");
$msg .= ("Please choose the department records that you would like to place these students in.");
instruct($msg);

echo("<br />");

if (sizeof($arr_multiple_dept) && is_array($arr_multiple_dept))
{
    echo("<form method='post' action='" . $PHP_SELF . "&select=placement_by_term'>");
    if ($arr_ready)
    {
        echo("<input type='hidden' name='arr_ready' value='".packObject(($arr_ready))."' />");
    }
    if ($arr_container)
    {
        echo("<input type='hidden' name='arr_container' value='".packObject(($arr_container))."' />");
    }
    echo("<input type='hidden' name='arr_multiple_dept' value='".packObject(($arr_multiple_dept))."' />");
    echo("<input type='hidden' name='level' value='container_info' />");

    foreach($arr_multiple_dept as $placement)
    {
        if (is_a($placement,"Placement"))
        {
            echo("<table cellspacing='0' cellpadding='0' border='1'>");
            echo("<tr>");
            echo("<td>");
            echo("<table border='0' cellpadding='2'>");

            echo("<tr>");
            echo("<td class='rowgrey' align='center'>&nbsp;</td>");
            echo("<td class='rowgrey' align='center'><b class='white'>Student Name</b></td>");
            echo("<td class='rowgrey' align='center'><b class='white'>Student Number</b></td>");
            echo("<td class='rowgrey' align='center'><b class='white'>Student Department</b></td>");
            echo("</tr>");

            $sql = ("
                    SELECT DISTINCT CONCAT(s.last_name, ', ', s.first_name) AS name, sd.record_id, d.department_name, s.student_number
                    FROM student AS s
                    INNER JOIN student_department AS sd 
                    ON sd.student_number=s.student_number
                    INNER JOIN department AS d
                    ON sd.department_id = d.department_id
                    INNER JOIN student_flags_join sfj
                    ON sfj.record_id = sd.record_id
                    WHERE sfj.student_flags_id = '".CURRENT_FLAG."'
                    AND sd.student_number = '".$placement->student_number."'
                    ORDER BY s.last_name, s.first_name, d.department_name
                    ");

            $result = $GLOBALS['dbh']->Execute($sql);
            $rowclass = 0;

            while ($row = $result->FetchRow())
            {
                echo("<tr>");
                echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                echo("<input type='radio' name='arr_record_id[".$placement->student_number."]' value='" . $row["record_id"] . "' ");
                echo("/>");
                echo("</td>");

                echo("<td nowrap align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                echo($row["name"]);
                echo("</td>");

                echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                echo($row["student_number"]);
                echo("</td>");

                echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
                echo($row["department_name"]);
                echo("</td>");
                echo("</tr>");
                $rowclass++;
            }

            echo("</table>");
            echo("</td>");
            echo("</tr>");

            echo("</table>");
            echo("<br />");
        }
    }

    echo("<input type='submit' value='Place Students'>");
}
else
{
    error("No students provided, please go back to the place students by term page.");
}
echo("</form>");

?>
