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
 | Filename: multiple_student_dept_form.inc                                     |
 +------------------------------------------------------------------------------+
 | Description: This file shows a list of departments a student is current in.  |
 |              A user enters this page if the student is in multiple           |
 |              departments. They also enter this page if a user is searching   |
 |              for a student based on name. This page requires the $placement  |
 |              object to exist.                                                |
 +------------------------------------------------------------------------------+

*/

echo("<h3 align='left'>Job Offers - Place by Student</h3>");

// Get a string of the department ids that belong to the user's group.
if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

if ($placement->first_name || $placement->last_name)
{
    $msg = ("Multiple students have been returned that match the name you have typed in.  Please choose the");
    $msg .= (" student that you wish to place.");
}
elseif ($placement->student_number)
{
    $msg = ("The student that matches the student number you have entered is current in multiple departments. ");
    $msg .= ("Please choose the department record that you would like to place this student in.");
}
instruct($msg);

echo("<br />");

echo("<table cellspacing='0' cellpadding='0' border='1'>");
echo("<tr>");
echo("<td>");
echo("<form method='post' action='" . $PHP_SELF . "&amp;select=placement_by_student'>");
echo("<input type='hidden' name='placement' value='".packObject(($placement))."' />");
echo("<input type='hidden' name='level' value='show_jobs' />");

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
        ");

if ($placement->first_name || $placement->last_name)
{
    $sql .= ("
        AND sd.department_id IN (" . $dept_string . ")
        ");
    if ($placement->first_name)
    {
        $sql .= ("
            AND s.first_name LIKE '" . $placement->first_name . "%'
            ");
    }

    if ($placement->last_name)
    {
        $sql .= ("
            AND s.last_name LIKE '" . $placement->last_name . "%'
            ");
    }
}

// search based on student number as default case
else
{
    $sql .= ("
        AND sd.student_number = '".$placement->student_number."'
        ");
}

$sql .= (" 
    ORDER BY s.last_name, s.first_name, d.department_name
    ");

$result = $GLOBALS['dbh']->Execute($sql);
$rowclass = 0;

while ($row = $result->FetchRow())
{
    echo("<tr>");
    echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
    echo("<input type='radio' name='record_id' onclick='submit();' value='" . $row["record_id"] . "' ");
    echo(" />");
    echo("</td>");

    echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
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

echo("<tr>");
echo("<td colspan='4' nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
echo("<input type='submit' value='Place Student' />");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

echo("</table>");

?>
