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
 | Filename: edit.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: This page basically prompts the user for info to apply to a     |
 | job, and asks them if they would like to include a transcript.               |
 +------------------------------------------------------------------------------+

*/


$a_sql = "SELECT a.resume_id, a.transcript, ji.job_code, ji.position_title 
        FROM applications as a
        LEFT JOIN job_info as ji ON ji.job_id = a.job_id 
        WHERE a.application_id = '$application_id'
        AND a.student_number = '$student_number'";
$a_result = $GLOBALS['dbh']->Execute ($a_sql);
// problem, couldn't find what we were looking for
if (!($a_row = $a_result->FetchRow())) {

    error ("There was an error attempting to retrive your application information");
}
elseif ($userlevel == ADMINSTUDENT) {
    error ("<h1 class=\"black\">Error</h1><p>You are not allowed to edit a student's application from here.
            You must log out of the student section and edit the cover letter and/or resume individually from within
            the \"Resumes\" or \"Cover Letters\" sections of \"Student Info\".</p>");
}
else {

    if($page == "save coverletter")
    {
        include( "coverletters/save.inc" );
    }

    $app_resume_id = $a_row["resume_id"];
    $app_inc_trans = $a_row["transcript"];
    $job_code = $a_row["job_code"];
    $pos_title = $a_row["position_title"];
    echo ("<h4>" . $job_code . " : " . $pos_title . "</h4>");

    echo ("<form method='post' action='$PHP_SELF'>");
    echo ("<input type='hidden' name='select' value='applications' />");
    echo ("<input type='hidden' name='application_id' value='$application_id' />");
    echo ("<input type='hidden' name='option' value='edit_delete' />");
    echo ("<input type='hidden' name='edit_save' value='1' />");
    echo ("<table class='row1' border='0' cellpadding='5' cellspacing='0'>");
    /*
        echo ("<input type='hidden' name='select' value='applications' />");
        echo ("<input type='hidden' name='option' value='edit_delete' />");
        echo ("<input type='hidden' name='application_id' value='$application_id' />");
        echo ("<input type='hidden' name='edit_save' value='1' />");
        */

    echo ("<tr>");
        echo ("<td align='right' width='50%'>");
            echo ("<b class='black'>Cover Letter:</b> &nbsp;");
        echo ("</td>");

        $sql = "SELECT c.coverletter_id AS coverletter_id 
            FROM applications AS a, job_info AS i, coverletter AS c
            LEFT JOIN employer_company AS ec ON ec.employer_id = i.employer_id
            WHERE i.job_id = a.job_id
            AND a.coverletter_id = c.coverletter_id
            AND a.application_id = '$application_id'
            ORDER BY i.job_code";

        $result = $GLOBALS['dbh']->Execute ($sql);
        $row = $result->FetchRow();
        $coverletter_id = $row["coverletter_id"];


        echo ("<td align='left' width='50%'>");
            echo ("<input type='button' value='Edit' onclick='javascript:document.editform.submit()' />");
        echo ("</td>");
    echo ("</tr>");


    echo ("<tr>");
        echo ("<td align='right' width='50%'>");
        echo ("<b class='black'>Resume:</b> &nbsp;");
        echo ("</td>");

        echo ("<td align='left' width='50%'>");
        // display a dropdown list of resumes currently associated with that student
        $sql = "SELECT name, resume_id, student_number, type_id FROM resume
                WHERE student_number='$student_number'";
        $result = $GLOBALS['dbh']->Execute ($sql);

        echo ("<select name='resume_id'>");
        while ($row = $result->FetchRow()) {
            echo ("<option value='". $row["resume_id"]. "'". ($app_resume_id == $row["resume_id"] ? " selected='selected'" : "") ."> ". $row["name"] ."</option>");
        }
        echo ("</select>");
        echo ("</td>");
    echo ("</tr>");

    $num_transcripts = $GLOBALS['dbh']->GetOne("SELECT COUNT(*) FROM transcript WHERE student_number='$student_number'");
    if ($num_transcripts) {
        echo ("<tr>");
            echo ("<td align='right'>");
            echo ("Would you like to include your transcript<br />
                   when applying to this job? (Recommended)");
            echo ("</td>");

            echo ("<td align='left'>");
            echo ("<input type='radio' name='include_transcript' value='1'". ($app_inc_trans ? " checked='checked'" : "") ." />Yes&nbsp;&nbsp;");
            echo ("<input type='radio' name='include_transcript' value='0'". ($app_inc_trans ? "" : " checked='checked'") ." />No&nbsp;");
            echo ("</td>");
        echo ("</tr>");
    } else {
        echo ('<tr>');
            echo ('<td align="center" colspan="2">');
                echo ('<b class="black">Note: </b>You do not have a transcript in the system to include with your application.');
                echo ('<input type="hidden" name="include_transcript" value="0" />');
            echo ('</td>');
        echo ('</tr>');
    }

    echo ("<tr>");
        echo ("<td colspan='2'><hr /></td>");
    echo ("</tr>");


    echo ("<tr>");
        echo ("<td align='right'>");
            echo ("<input type='submit' value=' Save ' />");
        echo ("</td>");

        echo ("<td align='left'>");
            echo ("<input type='button' value='Cancel' onclick='javascript:document.cancelform.submit()' />");
        echo ("</td>");
    echo ("</tr>");

    echo ("</table>");
    echo ("</form>");

    echo ("<form method='post' action='$PHP_SELF' name='editform'>");
        echo ("<input type='hidden' name='select' value='coverletter' />");
        echo ("<input type='hidden' name='application_id' value='$application_id' />");
        echo ("<input type='hidden' name='coverletter_id' value='$coverletter_id' />");
        echo ("<input type='hidden' name='PDF' value='1' />");
        echo ("<input type='hidden' name='page' value='Edit' />");
    echo ("</form>");
    echo ("<form method='post' action='$PHP_SELF' name='cancelform'>");
        echo ("<input type='hidden' name='select' value='applications' />");
        echo ("<input type='hidden' name='option' value='edit_delete' />");
    echo ("</form>");
}
?>
