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
 | Filename: history_quickmenu.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: To check whether this history has a job description;            |
 +------------------------------------------------------------------------------+

*/
$flag = 0;
$sql = "SELECT job_id
    FROM history
    WHERE job_id IS NOT NULL
    AND history_id=".$history_id;
$results = $GLOBALS['dbh']->Execute($sql);
$row = $results->FetchRow();
$job_id = $row['job_id'];
if ($results->RecordCount() != 0)
{
    $sql2 = "SELECT job_code
        FROM job_info
        WHERE job_id ='".$job_id."'";
    $results2 = $GLOBALS['dbh']->Execute($sql2);
    $row2 = $results2->FetchRow();
    if ($results2->RecordCount() != 0)
    {
        $flag = 1;
        $job_code = $row2['job_code'];
    }
        
}

//:TODO: This department_id is interferring with student_department_id
$sql = "SELECT employer_id, company_department_id
    FROM history
    WHERE history_id='".$history_id."'";
$results = $GLOBALS['dbh']->Execute($sql);
$row = $results->FetchRow();
$employer_id = $row["employer_id"];
$department_id = $row["company_department_id"];

// For many of the quickmenu options, we need a record_id
$sql = ("
    SELECT sd.record_id, sd.department_id
    FROM student_department AS sd
    INNER JOIN history AS h
        ON h.student_number=sd.student_number
        AND h.department_id=sd.department_id
    WHERE history_id='" . $history_id . "'
    ");
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();
$record_id = $row["record_id"];
$student_department = $row["department_id"];

echo("<td align='right'>");
echo(" <form action='$PHP_SELF' method='post'>");
echo("<input type='hidden' name='PDF' value='1' />");
echo ("<input type='hidden' name='select' value='". urlencode('View Student')."' />");
echo ("<input type='hidden' name='student_num' value='". $student_num."' />");
echo ("<input type='hidden' name='searchHistory' value='".$searchHistory."' />");
echo ("<input type='hidden' name='history_id' value='". $history_id."' />");
echo ("<input type='hidden' name='employer_id' value='". $employer_id."' />");
echo ("<input type='hidden' name='department_id' value='". $department_id."' />");
echo ("<input type='hidden' name='history_list' value='". $history_list."' />");
echo ("<input type='hidden' name='student_list' value='". $student_list."' />");
echo ("<input type='hidden' name='student_department' value='".$student_department."' />");
echo ("<input type='hidden' name='searchStudents' value='". $searchStudents."' />");
echo("<input type='hidden' name='record_id' value='" . $record_id . "' />");
if ($flag == 1)
{
    echo ("<input type='hidden' name='job_code' value='". $job_code."' />");
    echo ("<input type='hidden' name='job_id' value='". $job_id."' />");
    echo ("<input type='hidden' name='showquick' value='true' />");
}
echo ("<select name='select'>");
echo ("<option value='history'>View Placement History</option>");
echo ("<option value='student_history'>View All Student's Placement History</option>\n");
echo("<option value='view_student'>View this Student</option>\n");
echo("<option value='student_coverletters'>View Student's Cover Letters</option>\n");
echo("<option value='student_resumes'>View Student's Resumes</option>\n");
echo("<option value='student_transcripts'>View Student's Transcripts</option>\n");
echo("<option value='student_interviews'>View Student's Interviews</option>\n");


if($employer_id)
{
    echo("<option value='view_this_company'>View this Company</option>");
    echo("<option value='view_this_division'>View this Division</option>");
}

if ($flag == 1)
{
    echo("<option value='view_job'>View Job Description</option>");
} 
echo ("<option value='edithistory'>Edit Placement History</option>");
echo("<option value='edit_student'>Edit this Student</option>\n");
echo("<option value='place_student_posted_job'>Place this Student in a Posted Job</option>\n");
echo("<option value='place_student_own_job'>Place this Student in \"Own Job\"</option>\n");
echo("<option value='place_student_returning_job'>Place this Student in \"Returning Job\"</option>\n");
echo("<option value='login_as_student'>Log in as this Student</option>\n");
echo ("</select>");
echo ("&nbsp;<input type='image' src='misc/images/arrowg.gif' alt='go' />");
echo ("</form>");
echo("</td>");
echo("</tr>");
echo("</table>");

