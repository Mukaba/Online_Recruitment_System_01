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
 | Filename: student_multiple_departments_form.inc                              |
 +------------------------------------------------------------------------------+
 | Description: This file displays a form that is called when a student         |
 | number entered by the user matches with a student with multiple departments  |
 | in the user's group.  The user is prompted to choose which department        |
 | record they would like to view.                                              |
 |                                                                              |
 | $student_num must have been set by view_student.inc before calling this      |
 | script.                                                                      |
 +------------------------------------------------------------------------------+

 */

// Display appropriate header: 'own job placement' and 'returning placement' function synonymously; only diff between them is header and page re-routes
if ($select == "add_history")
{
    echo("<h3 align='left'>Job Offers - Own Job Placement</h3>");
}
if ($select == "returning_placement")
{
    echo("<h3 align='left'>Job Offers - Returning Job Placement</h3>");
}

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='level' value='select_company' />");
// re-route to apporiate page, own job or returning job
if($select == "add_history")
{
    echo("<input type='hidden' name='select' value='add_history' />");
}
if($select == "returning_placement")
{
    echo("<input type='hidden' name='select' value='returning_placement' />");
}

// Get a string of the department ids that belong to the user's group.
if (sizeof($departments_in_group) > 1)
{
    $dept_string = "'" . implode("', '", $departments_in_group) . "'";
}
else
{
    $dept_string = "'" . $auth->department . "'";
}

$msg = ("The student that matches the student number you have entered is current in multiple departments. ");
$msg .= ("Please choose the department record that you would like to place this student in.");
instruct($msg);

echo("<br />");

echo("<table cellspacing='0' cellpadding='0' border='1'>");

echo("<tr>");
    echo("<td>");
    echo("<table border='0' cellpadding='2'>");

    echo("<tr>");
        echo("<td class='rowgrey' align='center'>&nbsp;</td>");
        echo("<td class='rowgrey' align='center'><b class='white'>&nbsp;Student&nbsp;</b></td>");
        echo("<td class='rowgrey' align='center'><b class='white'>&nbsp;Student Number&nbsp;</b></td>");
        echo("<td class='rowgrey' align='center'><b class='white'>&nbsp;Student Department&nbsp;</b></td>");
    echo("</tr>");

    $sql = ("
        SELECT CONCAT(s.last_name, ', ', s.first_name) AS name, sd.record_id, d.department_name, s.student_number
        FROM student AS s
        INNER JOIN student_department AS sd 
        ON sd.student_number=s.student_number
        INNER JOIN department AS d
        ON sd.department_id = d.department_id
        WHERE s.student_number='" . $addHistory->student_number . "'
        AND sd.department_id IN (" . $dept_string . ")
        ORDER BY d.department_name
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $rowclass = 0;

    while ($row = $result->FetchRow())
    {
        echo("<tr>");
            echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("<input type='radio' name='record_id' onclick='submit();' value='" . $row["record_id"] . "' ");
            echo(($record_id == $row["record_id"]) ? "checked='checked'" : "");
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
        echo("<input type='hidden' name='addHistory' value='".packObject(($addHistory))."' />");
        echo("<input type='submit' value='Place Student' />");
        echo("</td>");
    echo("</tr>\n");

    echo("</table>");
    echo("</td>");
    echo("</tr>");

echo("</table>");
echo("</form>");
?>
