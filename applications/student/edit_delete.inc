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
 | Filename: edit_delete.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This file deals with the editing and/or deleting of             |
 | applications from the student's side                                         |
 +------------------------------------------------------------------------------+

*/

//first do online applications
//it'll only return online ones cause we're checking for coverletter id & resume id
// since paper won't have either, it won't return them (plus we're checking to make sure it ain't paper)

echo ("<h3>Job Applications - Edit/Delete</h3>");

notify ("You are allowed to edit / delete applications only if the job is still open.");

$sql = "SELECT a.application_id as application_id, a.job_id as job_id, j.student_status as job_status, h.history_id, 
        aps.app_status_description as student_status, a.coverletter_id as coverletter_id, a.resume_id as resume_id, 
        j.job_code as job_code, r.name as name, a.transcript as transcript, j.resumes_pulled,
        DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, ec.company_name
        FROM applications as a, job_info as j, resume as r, application_status as aps
        LEFT JOIN history AS h ON (h.job_id = j.job_id AND h.student_number = '" . $student_number . "')
        LEFT JOIN employer_company AS ec ON ec.employer_id = j.employer_id
        WHERE a.student_number = '$student_number' 
        AND j.status = " . POSTED . "
        AND j.closing_date != ''
        AND a.job_id = j.job_id 
        AND a.resume_id = r.resume_id
        AND a.app_status = aps.app_status_id 
        AND j.apply_method = '".JOB_APP_METHOD_ONLINE."'
        ORDER BY j.job_code";

$result = $GLOBALS['dbh']->Execute( $sql );

if ($result->RecordCount() <= 0) {

    echo ("<h4>Online Applications:</h4>");
    notify("You do not have any applications which can be edited or deleted.");
}

else {

    echo ("<h4>Online Applications (".$result->RecordCount()."):</h4>");

    echo ("<form method='post' action='$PHP_SELF'>");
    echo ("<input type='hidden' name='select' value='applications' />");
    echo ("<input type='hidden' name='option' value='edit_delete' />");

    echo ("<table cellspacing='0' cellpadding='0' border='1'>");
    echo ("<tr>");
        echo ("<td>");

        echo ("<table border='0' cellpadding='2'>");
        echo ("<tr>");
            echo ("<td class='rowgrey' align='center'>&nbsp;&nbsp;</td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Company&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Job Code&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Cover Letter&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Resume&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Transcript&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Last Modified&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Job Status&nbsp;</b></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Your Status&nbsp;</b></td>");
        echo ("</tr>");

    $rowclass = 0;
    
    while ($row = $result->FetchRow()) {

        $rowclass % 2 ? $class = "row0d" : $class = "row1d";

        $jobHREF = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"] . "&amp;show_jobs=preferences&amp;show_future=";
        $resumeHREF = $PHP_SELF . "&amp;select=resume&amp;page=View+HTML&amp;PDF=1&amp;resume_id=" . $row["resume_id"];
        $coverHREF = $PHP_SELF . "&amp;select=coverletter&amp;page=View+HTML&amp;PDF=1&amp;coverletter_id=" . $row["coverletter_id"];

        echo ("<tr>");
            echo ("<td class='$class' align='center' valign='middle'><input name='application_id' type='radio' class='$class' value='" . $row["application_id"] . "' /></td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;<a href='$jobHREF' class='blue'>" . $row["company_name"] . "</a>&nbsp;</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;<a href='$jobHREF' class='blue'>" . $row["job_code"] . "</a>&nbsp;</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;<a href='$coverHREF' class='blue'>Specific</a>&nbsp;</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;<a href='$resumeHREF' class='blue'>" . $row["name"] . "</a>&nbsp;</td>");
            echo ("<td class='$class' align='center' valign='middle'>" . (($row["transcript"] == "1" ) ? "Yes" : "No") . "</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;" . $row["applied_date"] . "&nbsp;</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;" . getStudentStatusGif($row["job_id"]) . "</td>");
            echo ("<td class='$class' align='center' valign='middle'>&nbsp;" . getStudentOwnStatusGif($row["job_id"], $student_number) . "</td>");
        echo ("</tr>");
        $rowclass++;
    }

    echo ("</table>");
    echo ("</td>");
    echo ("</tr>");
    echo ("</table>");

    echo ("<br />");
    echo ("<input type='submit' name='edit' value='Edit' />");
    echo ("&nbsp; &nbsp;");
    echo ("<input type='submit' name='delete' value='Delete' />");
    echo ("</form>");
} //  there are some records returned

?>
