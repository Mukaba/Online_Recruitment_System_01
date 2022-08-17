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
 | Filename: view_interview.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the main file that displays specific interviews.        |
 | This is called when we are passed a specific job_id (otherwise we won't      |
 | have a unique match).                                                        |
 +------------------------------------------------------------------------------+

*/

if ($department_id == ""){
    $department_id = $auth->department;
}
if ($department_id != "0"){
    $department_str = "'" . $department_id . "'";

    // Build Department ID string..
    $select_group = $GLOBALS['dbh']->Execute("SELECT group_id from department_group where department_id = '$department_id'");
    if ($select_group AND $select_group->RecordCount()) {
        $row = $select_group->FetchRow();
        $group_id = $row["group_id"];

        $get_depts = $GLOBALS['dbh']->Execute("SELECT department_id from department_group where group_id = '$group_id'");
        $dept_string = "'',";
        while ($row = $get_depts->FetchRow()) {
            $dept_string .= "'" . $row["department_id"] . "',";
        }

        $dept_string = substr($dept_string, 0, -1);
        $department_str = $dept_string;
        if (strlen($dept_string) < 2) { $dept_string = "''"; }

    } else {
        $department_str = "'" . $department_id . "'";
    }



} else {
    $department_str = "'" . implode("', '", $departments_in_group) . "'";
}
if ($job_id == "") 
{
    $job_code = trim($job_code);
    $result = $GLOBALS['dbh']->Execute("
            SELECT job_id FROM job_info WHERE job_code='" . addslashes($job_code) . "'");
    if ($row = $result->FetchRow())
    {
        $job_id = $row["job_id"];
    }
    else
    {
        $job_id = -1;
    }
}

$sql = ("
        SELECT c.job_code, c.department_id, a.interview_place_id, b.company_name, d.description as medium, a.notes, a.staff_notes, a.cancelled
        FROM interview AS a, employer_company AS b, job_info AS c, interview_medium AS d
        WHERE a.job_id='$job_id' 
        AND d.interview_medium_id = a.interview_medium_id 
        AND c.job_id = a.job_id 
        AND c.employer_id=b.employer_id 
        AND c.department_id IN (" . $department_str . ")
        ");
$result = $GLOBALS['dbh']->Execute($sql);

// bypass variable is set when a job is not in department group but a student is signed up for it.
if(!$result->RecordCount() && $bypass == "yes") {
    $result->Close();

    $sql = ("
            SELECT c.job_code, c.department_id, a.interview_place_id, b.company_name, d.description as medium, 
            a.notes, a.staff_notes, a.cancelled
            FROM interview AS a, employer_company AS b, job_info AS c, interview_medium AS d
            WHERE a.job_id='$job_id' 
            AND d.interview_medium_id = a.interview_medium_id 
            AND c.job_id = a.job_id 
            AND c.employer_id=b.employer_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    $showquickmenu = "no";
}

if ($row = $result->FetchRow())
{
    $notes = $row["notes"];
    $staff_notes = $row["staff_notes"];

    if ($showquickmenu != "no") {

        echo("
                <table width='100%' style='margin-top:-1em;'>
                <tr>
                <td><hr /></td>
                </tr>
                ");

        if ($row["cancelled"])
        {
            echo("<tr>");
            echo("<td>");
            error("NOTE: This interview has been cancelled.");
            echo("</td>");
            echo("</tr>");
        }
        $sql = "SELECT contact_id, employer_department_id, department_id, employer_id, job_code
            FROM job_info
            WHERE job_id='" . $job_id . "'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row_quick = $result->FetchRow();
        echo("<tr>");
        echo("<td align='right' nowrap='nowrap'>
                <form method='post' action='".$PHP_SELF."&amp;PDF=1'>

        <input type='hidden' name='employer_department_id' value='".$jobrow['employer_department_id']."' />          
                <input type='hidden' name='department_id' value='".$row_quick['department_id']."' />          
                <input type='hidden' name='employer_id' value='".$row_quick['employer_id']."' />
                <input type='hidden' name='job_id' value='".$job_id."' />
                <input type='hidden' name='contact_id' value='".$row_quick['contact_id']."' />
                <input type='hidden' name='job_code' value='".$row_quick['job_code']."' />

                <select name='selection'>
                <option value='view_company'>View this Company</option>
                ".(($row_quick['employer_department_id']) ? "<option value='view_department'>View this Job's Division</option>" : "")."
                ".(($row_quick['contact_id']) ? "<option value='view_contact'>View this Job's Contact</option>" : "")."
                <option value='view_applications'>View this Job's Applications</option>
                <option value='final'>Final Interview Schedule</option>
                <option value='final_student'>Final Interview Schedule (Student Numbers)</option>
                <option value='sign_up'>Sign up Students for this Interview</option>
                <option value='single'>Preview Schedule</option>
                <option value='edit'>Edit this Interview</option>
                <option value='email_schedule'>E-mail Schedule to Employer</option>
                ");
        $sql = ("
                SELECT emailed_newsgroup
                FROM interview
                WHERE job_id='" . $job_id . "'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $emailrow = $result->FetchRow();
        if (trim($emailrow["emailed_newsgroup"]))
        {
            echo("<option value='email_students'>E-mail Shortlisted Students</option>");
        }
        echo("
                </select>
                &nbsp;
                <input type='image' src='misc/images/arrowg.gif' alt='go' />
                </form></td></tr></table>");
    }

?>

        <table>
        <tr><td><b>Company:&nbsp;</b></td>
        <td><?php echo(htmlentities($row["company_name"])); ?></td></tr>
        <tr><td><b>Job Code(s):&nbsp;</b></td>
        <td><?php echo("<a href='$PHP_SELF&amp;select=view_job&amp;level1=job_id&amp;job_id=$job_id' target='_blank'>".htmlentities($row['job_code'])); ?></a></td></tr>
        <tr><td><b>Host Department:&nbsp;</b></td>
        <td><?php echo(htmlentities(getDepartmentName($row["department_id"]))); ?></td></tr>
        <tr><td><b>Medium:&nbsp;</b></td>
        <td><?php echo(htmlentities($row["medium"])); ?></td></tr>
<?php
    // Display location information.

        echo("<tr><td valign='top'><b>Location:&nbsp;</b></td>");

    if ($row["interview_place_id"] == 1)
    {
        echo("<td>On Campus</td>");
        echo("</tr>");
    }
    elseif ($row["interview_place_id"] == 2)
    {
        $sql = ("
                SELECT DISTINCT * 
                FROM interview_location
                WHERE job_id='" . $job_id . "'
                AND street_address_1!=''
                AND city!=''
                ");
        $result = $GLOBALS['dbh']->Execute($sql);

        if ($result->RecordCount())
        {
            $row = $result->FetchRow();

            echo("<td>");
            echo(htmlentities($row["street_address_1"]) . "<br />");
            if ($row["street_address_2"])
            {
                echo(htmlentities($row["street_address_2"]) . "<br />");
            }
            echo(htmlentities($row["city"]));
            if ($row["province"])
            {
                echo(", " . htmlentities($row["province"]));
            }
            if ($row["country"])
            {
                echo(", " . htmlentities($row["country"]));
            }
            echo("</td>");
            echo("</tr>");
            if ($row["directions"])
            {
                echo("<tr><td valign='top'><b>Directions:&nbsp;</b></td>");
                echo("<td>" . $row["directions"] . "</td>");
                echo("</tr>");
            }
        }
        else
        {
            echo("<td>Off Campus</td>");
            echo("</tr>");
        }
    }



    echo("<tr><td valign='top'><b>Notes:</b></td><td>" . (($notes) ? $notes : "None") . "</td></tr>");
    echo("<tr><td valign='top'><b>Co-op Staff Notes:</b></td><td>" . (($staff_notes) ? $staff_notes : "None") . "</td></tr>");

    // Presentation Info
    $result = $GLOBALS['dbh']->Execute("SELECT * FROM presentation WHERE job_id='$job_id'");
    if ($row = $result->FetchRow())
    {
        echo("<tr><td colspan='2'>&nbsp;<hr width='50%' align='left'></td></tr>");
        echo("<tr><td valign='top'><b>Presentation:&nbsp;</b></td><td>" . formatLongDate($row["pres_date"]) . " at " . formatTime($row["pres_time"]) . "</td></tr>");
        echo("<tr><td valign='top'><b>Location:&nbsp;</b></td><td>" . $row["pres_loc"] . "</td></tr>");
        echo("<tr><td valign='top'><b>Notes:&nbsp;</b></td><td>" . (($row["pres_notes"]) ? $row["pres_notes"] : "None") . "</td></tr>");
    }
    echo("</table>");
    echo("<br />");
    echo("<h3>Interview Schedule</h3>");

    $result = $GLOBALS['dbh']->Execute("
            SELECT DISTINCT a.int_date, a.interviewer_id, b.interviewer_name
            FROM interview_time AS a, interview_interviewer AS b
            WHERE a.job_id='" . addslashes($job_id) . "' AND a.interviewer_id=b.interviewer_id
            ORDER BY a.int_date, a.sequential, b.interviewer_name");

    while ($row = $result->FetchRow()) {

        ?>
            <table>
            <tr><td><b>Interviewer:</b></td><td><?php echo(htmlentities($row["interviewer_name"])); ?></td></tr>
            <tr><td><b>Date:</b></td><td><?php echo(formatLongDate($row["int_date"])); ?></td></tr>
            </table>
            <?php

            //emily feb 7
            //ELT(a.int_type, 'interview', 'break', 'lunch') does not seem to work in this query therefore
            //I have changed it to "IF (a.int_type =1, '....)"
            //There are other problems with ELT noted in other files

            $result_times = $GLOBALS['dbh']->Execute ("
                    SELECT 	a.job_id, a.time_id, a.int_time, b.student_number,
                    IF(a.int_type=1, 'interview', IF(a.int_type=2, 'break', 'lunch')) AS int_type,
                    CONCAT(b.first_name, ' ', b.last_name) AS student_name,
                    CONCAT(c2.building_code, ' ', c.room_number) AS int_room,
                    CONCAT(d2.building_code, ' ', d.room_number) AS wait_room,
                    e.phone AS phone_number
                    FROM	interview_time AS a
                    LEFT OUTER JOIN	student AS b
                    ON		a.student_number=b.student_number
                    LEFT OUTER JOIN room AS c
                    ON	a.int_room_id=c.room_id
                    LEFT OUTER JOIN building AS c2
                    ON c.building_id=c2.building_id
                    LEFT OUTER JOIN room AS d
                    ON	a.int_wait_room_id = d.room_id
                    LEFT OUTER JOIN building AS d2
                    ON d.building_id=d2.building_id
                    LEFT OUTER JOIN interview_phone AS e
                    ON	a.time_id=e.time_id
                    WHERE	a.int_date='" . $row["int_date"] . "' AND
                    a.interviewer_id='" . $row["interviewer_id"] . "' AND
            a.job_id='" . addslashes($job_id) . "'
            ORDER BY a.int_time");
        ?>
            <table cellspacing="0" cellpadding="0" border="1"><tr><td>
            <table border="0" cellpadding="2">
            <tr>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Interview Time</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Student</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<b class="white">Room or Phone</b>&nbsp;</td>
            </tr>
            <?php
            while ($row_times = $result_times->FetchRow())
            {
                switch($row_times["int_type"])
                {
                    case "interview":
                        if (!$int_room = $row_times["int_room"]) { $int_room = "TBA"; }
                    if (!$wait_room = $row_times["wait_room"]) { $wait_room = "TBA"; }
                    echo("<tr>");
                    echo("<td class='row0d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
                    echo("<td class='row0d'>&nbsp;" . htmlentities($row_times["student_name"]) . "&nbsp;</td>");
                    if ($row_times["phone_number"])
                    {
                        echo("<td class='row0d'>&nbsp;" . htmlentities($row_times["phone_number"]) . "&nbsp;</td>");
                    }
                    elseif ($int_room == "TBA")
                    {
                        echo("<td class='row0d'>&nbsp;" . htmlentities($int_room) . "&nbsp;</td>");
                    }
                    else
                    {
                        echo("<td class='row0d'>&nbsp;" . htmlentities($int_room) . " / " . htmlentities($wait_room) . "&nbsp;</td>");
                    }
                    echo("</tr>");
                    break;

                    case "break":
                        echo("<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
                    echo("<td class='row1d'>&nbsp;Break&nbsp;</td>");
                    echo("<td class='row1d'>&nbsp;</td></tr>");
                    break;

                    case "lunch":
                        echo("<tr><td class='row1d' align='center'>" . formatTime($row_times["int_time"]) . "</td>");
                    echo("<td class='row1d'>&nbsp;Lunch&nbsp;</td>");
                    echo("<td class='row1d'>&nbsp;</td></tr>");
                    break;
                }
            }
?>
            </table>
            </td></tr></table>
<?php
    }

    echo("<hr />");
    include('interview/view/main/students_short_listed.inc');
}
else
{
    echo ("Job was not found.");
}
?>

