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
 | Filename: show_job.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: This is the file that is used to redirect users according to    |
 | their user level for job viewing                                             |
 +------------------------------------------------------------------------------+

*/
?>
&nbsp;
<?php
    //This is where you put links for the top menu
   /* 
   $sql = "SELECT DISTINCT job_code, employer_id, employer_department_id ".
       "FROM job_info ".
           "WHERE job_id = '".$job_id."'";
           $result = $GLOBALS['dbh']->Execute($sql);
           $row = $result->FetchRow();
           $job_code = $row["job_code"];
           $employer_id = $row["employer_id"];
           $department_id = $row["employer_department_id"];

   if(($show_quick))
    {

        echo("<form name='viewjob' action='mamook.php' method='post'>");
        echo("<input type='hidden' name='select' value='view_job' />");
        echo ("<input type='hidden' name='job_id' value='". $job_id."' />");
        echo ("<input type='hidden' name='show_quick' value='true' />");
        echo("<input type='hidden' name='PDF' value='1' />");
        echo("<td width='5'>&nbsp;</td>");
        echo("<td valign='middle'><b class='black'>View:</b>&nbsp;");
        echo("<a href='javascript:document.viewjob.submit()'><b class='blue'>View Job&nbsp;</b></a></td>");
        echo("</form>");
    }
    elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
    {
        echo("<form name='viewfolder' action='mamook.php' method='post'>");
        echo("<input type='hidden' name='select' value='view_folders' />");
        echo("<input type='hidden' name='show' value='true' />");
        echo ("<input type='hidden' name='student_number' value='". $student_number."' />");
        echo("<td width='5'>&nbsp;</td>");
        echo("<td valign='middle'><b class='black'>View:</b>&nbsp;");
        echo("<a href='javascript:document.viewfolder.submit()'><b class='blue'>Folders&nbsp;</b></a></td>");
        echo("</form>");
    
    }
    */
?>    
        </td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<?php

            if ($userlevel == TRAFFICDIRECTOR || $auth->department == DIRECTORS_OFFICE_DEPARTMENT)
            {
                if($show_quick)
                {
                    echo("<table width='90%'>");
                    echo("<tr>");
                    echo("<td align='left'>");
                    echo("<h3>Job Description - View</h3>");
                    echo("</td>"); 
                    
                    include_once("job_descriptions/job_quickmenu.inc");
                    
                    echo("</tr>");
                }
                else
                {
                    echo("<table width='90%'>");
                }
                include('job_descriptions/show_job/show_all_traffic.inc');
            }
            elseif ($userlevel == OFFICE) 
            {
                if($show_quick)
                {
                    echo("<table width='90%'>");
                    echo("<tr>");
                    echo("<td align='left'>");
                    echo "<h3>Job Description - View</h3>";
                    echo("</td>");
                    include_once('job_descriptions/job_quickmenu.inc');
                    echo("</tr>");
                }
                else
                {
                    echo("<table width='90%'>");
                }
                include('job_descriptions/show_job/show_all_office.inc');
            }
            elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
            {
                if($show_quick)
                {
                    echo("<table width='90%'>");
                    echo("<tr>");
                    echo("<td align='left'>");
                    echo "<h3>Job Description - View</h3>";
                    echo("</td>");
                    include_once('job_descriptions/job_quickmenu.inc');
                    echo("</tr>");
                }
                else
                {
                    echo("<table width='90%'>");
                }

                include('job_descriptions/show_job/show_all_employer.inc');

                echo("</table>");
            }
            elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
            {

                if ($userlevel == STUDENT)
                {
                    /*
                     Update the students view_job_recent and view_job_last fields
                     so that we know which jobs are new and updated to him.
                    */

                    $sql = ("
                        SELECT CURRENT_DATE
                        ");
                    $date_result = $GLOBALS['dbh']->Execute($sql);
                    $date_row = $date_result->FetchRow();
                    $current_date = $date_row["CURRENT_DATE"];

                    $sql = ("
                        SELECT view_job_recent, view_job_last
                        FROM student
                        WHERE student_number='" . $student_number . "'
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();

                    if ($row["view_job_last"] == '0000-00-00' && $row["view_job_recent"] == '0000-00-00')
                    {
                        $sql = ("
                            UPDATE student SET
                            view_job_last='" . $current_date . "',
                            view_job_recent='" . $current_date . "'
                            WHERE student_number='" . $student_number . "'
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }
                    elseif ($row["view_job_last"] == '0000-00-00' && $row["view_job_recent"] == $current_date)
                    {
                        $sql = ("
                            UPDATE student SET
                            view_job_last='" . $current_date . "'
                            WHERE student_number='" . $student_number . "'
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }
                    elseif ($row["view_job_recent"] != $current_date)
                    {
                        $sql = ("
                            UPDATE student SET
                            view_job_recent='" . $current_date . "',
                            view_job_last='" . $row["view_job_recent"] . "'
                            WHERE student_number='" . $student_number . "'
                            ");
                        $result = $GLOBALS['dbh']->Execute($sql);
                    }
                }
                    echo("<table width='90%'>\n");
                    include('job_descriptions/show_job/show_all_student.inc');

            }
?>
