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
 | Filename: show_job_list.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This is the file that displays all of the jobs to a student,    |
 | ordered by academic year.                                                    |
 +------------------------------------------------------------------------------+

*/
// These variables will help reduce the problem of too many companies on a screen
include_once('job_descriptions/folders/functions.inc');
if ($open_per_page_max == "") { $open_per_page_max = 50; }
if ($open_per_page_max < 5) { $open_per_page_max = 5; }
if ($open_start_row == '') { $open_start_row = 0; }

if ($closed_per_page_max == "") { $closed_per_page_max = 25; }
if ($closed_per_page_max < 5) { $closed_per_page_max = 5; }
if ($closed_start_row == '') { $closed_start_row = 0; }


$nextTermYear = getNextTermYear();

/*
 First, figure out which jobs we will be showing.  Do this by switching the show_jobs variable,
 and creating an extra little bit to stick in our queries.
*/


switch ($show_jobs)
{
    case "preferences":
        //Do nothing here right now, later add preferences to query.
        break;

    case "group":

        /*
         If they want to see jobs for their group, first check if they're department is part of a group,
         If they're not, then just continue on and display as if they had selected to view jobs for their
         department.
        */

        $group_sql = ("
            SELECT DISTINCT group_id
            FROM department_group
            WHERE department_id='" . addslashes($auth->department) . "'
            ");
        $group_result = $GLOBALS['dbh']->Execute($group_sql);

        if ($group_result->RecordCount())
        {
            //First find out the group they are in.
            $group_row = $group_result->FetchRow();
            $group = $group_row["group_id"];

            /*
             Now pull out all departments associated with this group, and put them into a comma delimited
             string.
             */

            $sql = ("
                SELECT DISTINCT department_id
                FROM department_group
                WHERE group_id='" . $group . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);

            while ($row = $result->FetchRow())
            {
                $department_ids .= ("'" . $row["department_id"] . "',");
            }
            $department_ids = substr($department_ids, 0, -1);

            /*
             Now pull out all of the job_ids that are listed in the department_job_join table
             that have one of these departments listed.  This means that we will be returned
             all of the job_ids in the system that have one of the departments in this group
             listed as applicable.
            */

            $sql = ("
                SELECT DISTINCT job_id
                FROM department_job_join
                WHERE job_id IN (" . $department_ids . ")
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }

            if ($job_id_list)
            {
                $job_id_list = substr($job_id_list, 0, -1);
            }
            else
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                    AND a.job_id IN (" . $job_id_list . ")
                    ");

            break;
        }

        // If they weren't part of a group, they'll fall through here into the department case.  This is desirable.

    case "department":

        // Get all of the jobs that have this department listed as applicable.

        $sql = ("
            SELECT DISTINCT job_id
            FROM department_job_join
            WHERE department_id='" . $auth->department . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job_id_list .= ("'" . $row["job_id"] . "',");
        }

        if ($job_id_list)
        {
            $job_id_list = substr($job_id_list, 0, -1);
        }
        else
        {
            $job_id_list = ("''");
        }

        $extra_sql = ("
            AND a.job_id IN (" . $job_id_list . ")
            ");

        break;

    case "flagged":

        // Get all of the job_ids that have been flagged by this student.

        $sql = ("
            SELECT DISTINCT job_id
            FROM student_jobs_flagged
            WHERE student_number='" . $student_number . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job_id_list .= ("'" . $row["job_id"] . "',");
        }

        if ($job_id_list)
        {
            $job_id_list = substr($job_id_list, 0, -1);
        }
        else
        {
            $job_id_list = ("''");
        }

        $extra_sql = ("
                AND a.job_id IN (" . $job_id_list . ")
                ");
        break;

    case "applied":

        // Get all of the job_ids that have been applied to by this student

        $sql = ("
            SELECT DISTINCT job_id
            FROM applications
            WHERE student_number='" . $student_number . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        while ($row = $result->FetchRow())
        {
            $job_id_list .= ("'" . $row["job_id"] . "',");
        }

        if ($job_id_list)
        {
            $job_id_list = substr($job_id_list, 0, -1);
        }
        else
        {
            $job_id_list = ("''");
        }

        $extra_sql = ("
                AND a.job_id IN (" . $job_id_list . ")
                ");
        break;

    case "uvic":
        // If they want to see all jobs registered at UVic, do nothing to the query.
        break;
    default:
        // If they got here, they're probably viewing jobs for a specific department.

        if ($show_jobs && preg_match("/\d+/", $show_jobs))
        {
            $sql = ("
                SELECT DISTINCT job_id
                FROM department_job_join
                WHERE department_id='" . $show_jobs . "'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }

            if ($job_id_list)
            {
                $job_id_list = substr($job_id_list, 0, -1);
            }
            else
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                AND a.job_id IN (" . $job_id_list . ")
                ");
        }
        break;
} //End of switch statement.

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_job' />");
echo("<input type='hidden' name='show_term_year' value='" . $show_term_year . "' />");
echo("<input type='hidden' name='show_future' value='" . $show_future . "' />");
echo("<input type='hidden' name='show_jobs' value='" . $show_jobs . "' />");

if ($show_term_year)
{
    $term_year = explode("-", $show_term_year);
    $show_term = $term_year[0];
    $show_year = $term_year[1];
}
else
{
    $nextTermYear = getNextTermYear();

    if (!$show_term)
    {
        $show_term = $nextTermYear["term"];
    }
    if (!$show_year)
    {
        $show_year = $nextTermYear["year"];
    }
}
// First show students jobs that are new and updated.
//:TODO Display departments here as WELL as company names
$sql = ("
    SELECT DISTINCT
    ");

switch ($order)
{
    case 'min_academic':
        $sql.= "a.min_academic, a.job_code, b.company_name, c.department_name, a.job_id, a.resumes_pulled, 
            a.closing_date, a.date_posted, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'closing_date':
        $sql.= "a.closing_date, a.job_code, b.company_name, c.department_name, a.job_id, a.resumes_pulled, 
            a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'status':
        $sql.= "a.student_status, a.job_code, b.company_name, c.department_name, a.job_id, a.resumes_pulled, 
            a.closing_date, a.date_posted, a.min_academic, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'job_code':
        $sql.= "a.job_code, b.company_name, c.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'app_count':
        $sql.= "COUNT(app.app_status) AS app_count, a.job_code, b.company_name, c.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.date_posted, a.min_academic, a.student_status, ss.student_number ";
        break;
    case 'company_name':
    default:
        $sql.= "b.company_name, c.department_name, a.job_code, a.job_id, a.resumes_pulled, a.closing_date, 
            a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
}
if($folder_id)
{
    $sql .= ", sjfj.folder_id";
}

$apptypes = implode(",",array(APP_APPLIED_ONLINE, APP_PAPER_APPLIED, APP_ACTIVATED_ONLINE, APP_UNAVAILABLE, APP_PAPER_SENT, APP_SPECIAL));
$sql .= ("
    FROM job_info AS a 
    LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
    LEFT JOIN applications AS app ON a.job_id=app.job_id AND app.app_status IN (".$apptypes.")
    LEFT JOIN employer_department AS c ON c.department_id=a.employer_department_id ");
    if($folder_id)
    {
        if($folder_id == -1)
        {
            $sql.=" LEFT JOIN student_job_folder_join AS sjfj ON a.job_id = sjfj.job_id AND sjfj.student_number='" . $student_number . "'";
        }
        else
        {
            $sql.=" INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.folder_id = '".$folder_id."' AND sjfj.student_number='" . $student_number . "'";
        }
    }
    $sql .= (" LEFT JOIN students_shortlisted AS ss ON (ss.job_id=a.job_id AND ss.student_number='" . $student_number . "')
    WHERE a.status=" . POSTED  
    );

    if($folder_id == -1)
    {
        $sql.=" AND sjfj.job_id IS NULL ";
    }
    $sql.=(" AND a.term_id='" . addslashes($show_term) . "'
    AND a.year='" . addslashes($show_year) . "'
    AND (a.date_posted>='" . addslashes($last_viewed_on) . "' OR a.last_updated_on>='" . addslashes($last_viewed_on) . "')
     ");
    
$sql .= $extra_sql;
$sql .= (" GROUP BY a.job_id ");
if($order)
{
    if(!$change)
    {
        if(($lastorder == $order) && ($data_flow == "ASC"))
        {
            $data_flow = "DESC";
        }
        elseif(($lastorder == $order) && ($data_flow == "DESC"))
        {
            $data_flow = "ASC";
        }
        elseif(!$data_flow)
        {
            $data_flow = "DESC";
        }
    }


    if($order == "company_name")
    {
        $order_sql = "ORDER BY b.company_name ".$data_flow.", a.job_code, a.closing_date";
    }
    elseif($order == "closing_date")
    {
        $order_sql = "ORDER BY a.closing_date ".$data_flow.", a.job_code, b.company_name";
    }
    elseif($order == "status")
    {
        $order_sql = "ORDER BY a.admin_status ".$data_flow.", a.job_code, b.company_name";
    }
    elseif($order == "min_academic")
    {
        $order_sql = "ORDER BY a.min_academic ".$data_flow.", a.job_code, b.company_name";
    }
    elseif($order == "app_count")
    {
        $order_sql = "ORDER BY app_count ".$data_flow.", a.job_code, b.company_name";
    }
    else
    {
        $order_sql = "ORDER BY a.job_code ".$data_flow.", b.company_name, a.closing_date";
    }
}
else
{
    $order_sql = "ORDER BY b.company_name, a.job_code, a.closing_date";
    $order = "company_name";
    $data_flow = "ASC";
}

$sql .= $order_sql;
$new_result = $GLOBALS['dbh']->Execute($sql);
//Display new and Updated Jobs

if($folder_id)
{
    if($folder_id == -1)
    {
        $folder_name = "that are NOT assigned to a folder";
    }
    else
    {
        $folder_name="in the ".getFolderName($folder_id, $student_number)." folder";
    }
    echo("<h4 align='left'>New and Updated jobs for " . getTermName($show_term) . ", " . $show_year);
    echo("  (" . $new_result->RecordCount() . ") ".$folder_name.":</h4>");
}
else
{
    echo("<h4 align='left'>New and Updated jobs for " . getTermName($show_term) . ", " . $show_year);
    echo(" (" . $new_result->RecordCount() . "):</h4>");
}

$href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;closed_start_row=" . $closed_start_row);
$href .= ("&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;show_future=" . urlencode($show_future));
$href .= ("&amp;show_term_year=".$show_term_year);
$href .= ("&amp;job_page=".$job_page. "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
$href .= ("&amp;open_start_row=" . $open_start_row . "&amp;open_per_page_max=" . $open_per_page_max ."&amp;folder_id=".$folder_id. "&amp;order=");
?>

    <table cellspacing='0' cellpadding='0' border='1'>
    <tr>
        <td>
        <table border='0' cellpadding='2'>
        <tr>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>&nbsp;</b>&nbsp;</td>
            <td class='rowgrey' align='center'><img src='misc/images/job_flagged.gif' alt='' /></td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>company_name'><b class="white">Company</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>min_academic'><b class="white">Min Year</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>closing_date'><b class="white">Closing Date</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>status'><b class='white'>Status</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Your Status</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>app_count'><b class="white"># Students Applied</b></a>&nbsp;</td>
        </tr>

    <?php
    $rowclass = 0;
    $counter = 0;
    while ($new_row = $new_result->FetchRow())
    {
        $counter++; 
        $company_name = $new_row["company_name"];
        $division_name = $new_row["department_name"];
        $job_id = $new_row["job_id"];
        $min_academic = $new_row["min_academic"];
        $closing_date = $new_row["closing_date"];
        $last_updated_on = $new_row["last_updated_on"];
        $total_apps = $new_row["app_count"];
        $date_posted = $new_row["date_posted"];
        $student_status = $new_row["student_status"];
        $resumes_pulled = $new_row["resumes_pulled"];
        $job_code = $new_row["job_code"];

        $sql = ("
            SELECT DISTINCT d.department_code
            FROM department_job_join AS djj, department AS d
            WHERE djj.job_id='" . $job_id . "'
            AND d.department_id=djj.department_id
            ORDER BY d.department_id
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql);

        $apply_departments = '';
        if ($result2->RecordCount())
        {
            while ($row2 = $result2->FetchRow())
            {
                $apply_departments .= ($row2["department_code"] . ", ");
            }
            $apply_departments = substr($apply_departments, 0, -2);
        }
        else
        {
            $apply_departments = ("None");
        }

        $href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;closed_start_row=" . $closed_start_row);
        $href .= ("&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;show_future=" . urlencode($show_future));
        $href .= ("&amp;job_page=".$job_page. "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
        $href .= ("&amp;open_start_row=" . $open_start_row . "&amp;open_per_page_max=" . $open_per_page_max . "&amp;folder_id=".$folder_id."&amp;order=");

        $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);
        $href .= ("&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_future=" . urlencode($show_future));

        echo("<tr>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        if (is_job_flagged($job_id, $student_number))
        {
            echo("<input type='checkbox' name='jobs_to_unflag[]' value='" . $job_id . "' />");
        }
        else
        {
            echo("<input type='checkbox' name='jobs_to_flag[]' value='" . $job_id . "' />");
        }
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo((is_job_flagged($job_id, $student_number)) ? "<img src='misc/images/job_flagged.gif' alt='' />" : "&nbsp;");
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo("&nbsp;<a class='blue' href=\"" . $href . "\">");
        echo(htmlentities($company_name) . (($division_name != $company_name) ? " (" . htmlentities($division_name) . ")" : ""));
        echo("</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo($apply_departments);
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        $disc_string = '';
        $disc_array = getDiscArray($job_id);
        foreach ($disc_array AS $da)
        {
            $disc_string .= ($da . ", ");
        }
        $disc_string = substr($disc_string, 0, -2);
        echo($disc_string);
        echo("</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($min_academic . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($closing_date . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo(getStudentStatusGif($job_id));
        echo("</td>\n");

        echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        // Determine how far this student has progressed in this job.
        echo(getStudentOwnStatusGif($job_id, $student_number));
        echo("</td>");
        
        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;".getApplicantRange($total_apps)."&nbsp;");
        echo("</td>\n");

        echo("</tr>\n");
        $rowclass++;
        
    }

    echo("<tr>");
        echo("<td align='center' colspan='11' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            if($counter == 0)
            {
                if($folder_id)
                {
                    if($folder_id == -1)
                    {
                        $folder_name = "that are NOT assigned to a folder";
                    }
                    else
                    {
                        $folder_name= "in the ".getFolderName($folder_id, $student_number)." folder";
                    }
                    echo("<b>There are no New and Updated jobs ".$folder_name."</b>");
                }
                else
                {
                    echo("<b>There are no New and Updated jobs for " . getTermName($show_term) . ", " . $show_year."</b>");
                }
            }
            else{
                
                echo("<input type='submit' name='level1' value='Flag/Unflag Selected Jobs' />&nbsp;<b> -OR- </b>&nbsp;");
                echo("<input type='hidden' name='folder_id' value='".$folder_id."' />");
                echo("<select name = 'move_folder_id'>");
                echo("<option value='0'>Move Selected jobs to Folder</option>\n");
                $sql = ("
                        SELECT folder_id
                        FROM student_job_folder 
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                    echo("<option value='".$row["folder_id"]."'>".getFolderName($row["folder_id"],$student_number)."</option>\n");
                }
                if($folder_id)
                {
                    echo("<option value='-1'>No Folder</option>\n");
                }

                echo("</select>");
                echo("&nbsp;&nbsp;<input type = 'submit' name='level1' value='Move' />"); 
            }
            echo("</td>");
    echo("</tr>");

    echo("</table>");
    echo("</td></tr></table>");
echo("</form>");

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_job' />");
echo("<input type='hidden' name='show_term_year' value='" . $show_term_year . "' />");
echo("<input type='hidden' name='show_future' value='" . $show_future . "' />");
echo("<input type='hidden' name='show_jobs' value='" . $show_jobs . "' />");

//Now display the regular jobs.

$sql = ("
    SELECT DISTINCT
    ");

switch ($order)
{
    case 'min_academic':
        $sql.= "a.min_academic, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, st.description, a.closing_date, a.date_posted, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count";
        break;
    case 'closing_date':
        $sql.= "a.closing_date, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, st.description, a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'status':
        $sql.= "st.description, a.student_status, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, a.closing_date, a.date_posted, a.min_academic, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'job_code':
        $sql.= "a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, st.description, a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'app_count':
        $sql.= "COUNT(app.app_status) AS app_count, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, st.description, a.date_posted, a.min_academic, a.student_status, ss.student_number ";
        break;
    case 'company_name':
    default:
        $sql.= "b.company_name, ed.department_name, a.job_code, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, st.description, a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
}
if($folder_id)
{
    $sql .= ", sjfj.folder_id";
}

$apptypes = implode(",",array(APP_APPLIED_ONLINE, APP_PAPER_APPLIED, APP_ACTIVATED_ONLINE, APP_UNAVAILABLE, APP_PAPER_SENT, APP_SPECIAL));
$sql.= ("
    FROM job_info AS a, student_status as st
    LEFT JOIN applications AS app ON a.job_id=app.job_id AND app.app_status IN (".$apptypes.")
    LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
    LEFT JOIN students_shortlisted AS ss ON (ss.job_id=a.job_id AND ss.student_number='" . $student_number . "') ");
    if($folder_id)
    {
        if($folder_id == -1)
        {
            $sql.=" LEFT JOIN student_job_folder_join AS sjfj ON a.job_id = sjfj.job_id AND sjfj.student_number='" . $student_number . "'";
        }
        else
        {
            $sql.=" INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.folder_id = '".$folder_id."' AND sjfj.student_number='" . $student_number . "'";
        }
    }
    $sql .= ("LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
    WHERE a.status=" . POSTED 
    );

    if($folder_id == -1)
    {
        $sql.=" AND sjfj.job_id IS NULL ";
    }

    $sql.=(" AND a.closing_date!='' 
    AND st.status_id=a.student_status
    AND a.term_id='" . addslashes($show_term) . "' 
    AND a.year='" . addslashes($show_year) . "'
    AND (a.date_posted<'" . addslashes($last_viewed_on) . "' AND a.last_updated_on<'" . addslashes($last_viewed_on) . "')
     ");

$sql .= $extra_sql;
$sql .= (" GROUP BY a.job_id ");
$sql .= $order_sql;

$num_open_result = $GLOBALS['dbh']->Execute($sql);
$num_open = $num_open_result->RecordCount();

if ($num_open > $open_per_page_max)
{
    $sql .= ("
        LIMIT " . $open_start_row . ", " . $open_per_page_max
        );
}

$result = $GLOBALS['dbh']->Execute($sql);

    $open_pages = ceil($num_open / $open_per_page_max);

    $first = $open_start_row + 1;
    $end = $open_start_row + $open_per_page_max;
    if ($end > $num_open) { $end = $num_open; }

    if($folder_id)
    {
        if($folder_id == -1)
        {
            $folder_name = "that are NOT assigned to a folder";
        }
        else
        {
            $folder_name= "in the ".getFolderName($folder_id, $student_number)." folder";
        }
        echo("<h4 align='left'>Open jobs for " . getTermName($show_term) . ", " . $show_year);
        echo("  (" .$num_open . ") ".$folder_name.":</h4>");    
    }
    else
    {   echo("<h4 align='left'>Open jobs for " . getTermName($show_term) . ", " . $show_year);
        echo(" (" . $num_open . "):</h4>");
    }

    


    if ($num_open > $open_per_page_max)
    {
        echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
        echo("<tr>");
        echo("<td>");
        echo($num_open . " job" . (($num_open != 1) ? "s" : "") . " on " . $open_pages . " page" . (($open_pages > 1) ? "s" : "").";</td>");
        echo("<td><input type='text' name='open_per_page_max' size='4' maxlength='4' value='" . $open_per_page_max . "' />");
        echo("<input type='hidden' name='order' value='$order' />");
        echo("<input type='hidden' name='data_flow' value='$data_flow' />");
        echo("<input type='hidden' name='change' value='no' />");
        echo(" jobs per page.");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");

        echo("<td>");
        echo("<select name='open_start_row'>");

        for ($i = 0; $i < $open_pages; $i++)
        {
            $open_row_num_start = $i * $open_per_page_max;
            $open_row_num_end = $open_row_num_start + $open_per_page_max - 1;
            if ($open_row_num_end >= $num_open) $open_row_num_end = $num_open - 1;

            $num_open_result->Move($open_row_num_start);
            $num_open_result_row = $num_open_result->FetchRow();
            $open_start = $num_open_result_row[0];

            if (strlen($open_start) > 15) { $open_start = substr($open_start, 0, 13) . "..."; }

            $num_open_result->Move($open_row_num_end);
            $num_open_result_row = $num_open_result->FetchRow();
            $open_end = $num_open_result_row[0];

            if (strlen($open_end) > 15) { $open_end = substr($open_end, 0, 13) . "..."; }
            if ($order=='min_academic'){
                $open_start = "Min year ".$open_start;
                $open_end = "Min year ".$open_end;
            }
            if($order =='app_count'){
                $open_start = getApplicantRange($open_start);
                $open_end = getApplicantRange($open_end);
            }

            echo("<option value='" . $open_row_num_start . "' ");
            if ($open_start_row == $open_row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . $open_start . " to " . $open_end . "</option>\n");
        }
        echo("</select>");
        echo("</td>");

        echo("<td align='right'>");
        echo("<input type='submit' name='continue' value='View' />");
        echo("</td>");

        echo("</tr>");
        echo("</table>");

    }
    //$href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs). "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
    //$href .= ("&amp;show_future=" . urlencode($show_future) . "&amp;folder_id=".$folder_id."&amp;order=");
    
    $href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;closed_start_row=" . $closed_start_row);
    $href .= ("&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;show_future=" . urlencode($show_future));
    $href .= ("&amp;job_page=".$job_page. "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
    $href .= ("&amp;show_term_year=".$show_term_year);
    $href .= ("&amp;open_start_row=" . $open_start_row . "&amp;open_per_page_max=" . $open_per_page_max . "&amp;folder_id=".$folder_id."&amp;order=");

    ?>

    <table cellspacing='0' cellpadding='0' border='1'>
    <tr>
        <td>
        <table border='0' cellpadding='2'>
        <tr>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>&nbsp;</b>&nbsp;</td>
            <td class='rowgrey' align='center'><img src='misc/images/job_flagged.gif' alt='' /></td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>company_name'><b class="white">Company</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>min_academic'><b class="white">Min Year</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>closing_date'><b class="white">Closing Date</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>status'><b class='white'>Status</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Your Status</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>app_count'><b class="white"># Students Applied</b></a>&nbsp;</td>
        </tr>

    <?php
    $rowclass = 0;
    $counter = 0; 
    
    while ($row = $result->FetchRow())
    {
        $counter++;
        $job_code = $row["job_code"];
        $company_name = $row["company_name"];
        $division_name = $row["department_name"];
        $student_status = $row["student_status"];
        $total_apps2 = $row["app_count"];
        $job_id = $row["job_id"];
        $min_academic = $row["min_academic"];
        $closing_date = $row["closing_date"];
        $last_updated_on = $row["last_updated_on"];
        $date_posted = $row["date_posted"];
        $resumes_pulled = $row["resumes_pulled"];

        $sql = ("
            SELECT DISTINCT d.department_code
            FROM department_job_join AS djj, department AS d
            WHERE djj.job_id='" . $job_id . "'
            AND d.department_id=djj.department_id
            ORDER BY d.department_id
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql);

        $apply_departments = '';
        if ($result2->RecordCount())
        {
            while ($row2 = $result2->FetchRow())
            {
                $apply_departments .= ($row2["department_code"] . ", ");
            }
            $apply_departments = substr($apply_departments, 0, -2);
        }
        else
        {
            $apply_departments = ("None");
        }

        $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);
        $href .= ("&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_future=" . urlencode($show_future));

        echo("<tr>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        if (is_job_flagged($job_id, $student_number))
        {
            echo("<input type='checkbox' name='jobs_to_unflag[]' value='" . $job_id . "' />");
        }
        else
        {
            echo("<input type='checkbox' name='jobs_to_flag[]' value='" . $job_id . "' />");
        }
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo((is_job_flagged($job_id, $student_number)) ? "<img src='misc/images/job_flagged.gif' alt='' />" : "&nbsp;");
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo("&nbsp;<a class='blue' href=\"" . $href . "\">");
        echo(htmlentities($company_name) . (($division_name != $company_name) ? " (" . htmlentities($division_name) . ")" : ""));
        echo("</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo($apply_departments);
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        $disc_string = '';
        $disc_array = getDiscArray($job_id);
        foreach ($disc_array AS $da)
        {
            $disc_string .= ($da . ", ");
        }
        $disc_string = substr($disc_string, 0, -2);
        echo($disc_string);
        echo("</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($min_academic . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($closing_date . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo(getStudentStatusGif($job_id));
        echo("</td>\n");

        echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo("&nbsp;");
        // Determine how far this student has progressed in this job.
        echo(getStudentOwnStatusGif($job_id, $student_number));
        echo("</td>");
        
        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;".getApplicantRange($total_apps2)."&nbsp;");
        echo("</td>\n");
        
        echo("</tr>\n");
        $rowclass++;
        
    }

    echo("<tr>");
        echo("<td align='center' colspan='11' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            if($counter == 0)
            {
                if($folder_id)
                {
                    if($folder_id == -1)
                    {
                        $folder_name = "that are NOT assigned to a folder";
                    }
                    else
                    {
                        $folder_name= "in the ".getFolderName($folder_id, $student_number)." folder";
                    }
                    echo("<b>There are no Open jobs ".$folder_name."</b>");
                }
                else
                {
                    echo("<b>There are no Open jobs for " . getTermName($show_term) . ", " . $show_year."</b>");
                }
            }
            else{

                echo("<input type='submit' name='level1' value='Flag/Unflag Selected Jobs' />&nbsp;<b> -OR- </b>&nbsp;");
                echo("<input type='hidden' name='folder_id' value='".$folder_id."' />\n");
                echo("<select name = 'move_folder_id'>");
                echo("<option value='0'>Move Selected jobs to Folder</option>\n");
                $sql = ("
                        SELECT folder_id
                        FROM student_job_folder
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                    echo("<option value='".$row["folder_id"]."'>".getFolderName($row["folder_id"],$student_number)."</option>\n");
                }
                if($folder_id)
                {
                    echo("<option value='-1'>No Folder</option>\n");
                }

                echo("</select>");
                echo("&nbsp;&nbsp;<input type = 'submit' name='level1' value='Move' />");
            }
            echo("</td>");
    echo("</tr>");

    echo("</table>");
    echo("</td></tr></table>"); 
    echo("</form>");

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_job' />");
echo("<input type='hidden' name='show_term_year' value='" . $show_term_year . "' />");
echo("<input type='hidden' name='show_future' value='" . $show_future . "' />");
echo("<input type='hidden' name='show_jobs' value='" . $show_jobs . "' />");

$sql = ("
    SELECT DISTINCT
    ");

switch ($order)
{
    case 'min_academic':
        $sql.= "a.min_academic, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, a.closing_date, st.description, a.date_posted, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'closing_date':
        $sql.= "a.closing_date, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, st.description, a.date_posted, a.min_academic, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'status':
        $sql.= "st.description, a.student_status, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, 
            a.last_updated_on, a.closing_date, a.date_posted, a.min_academic, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'job_code':
        $sql.= "a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, a.date_posted, a.min_academic, st.description, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
    case 'app_count':
        $sql.= "COUNT(app.app_status) AS app_count, a.job_code, b.company_name, ed.department_name, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, a.date_posted, a.min_academic, st.description, a.student_status, ss.student_number ";
        break;
    case 'company_name':
    default:
        $sql.= "b.company_name, ed.department_name, a.job_code, a.job_id, a.resumes_pulled, a.closing_date, 
            a.last_updated_on, a.date_posted, a.min_academic, st.description, a.student_status, ss.student_number, COUNT(app.app_status) AS app_count ";
        break;
}
if($folder_id)
{
    $sql .= ", sjfj.folder_id";
}

$apptypes = implode(",",array(APP_APPLIED_ONLINE, APP_PAPER_APPLIED, APP_ACTIVATED_ONLINE, APP_UNAVAILABLE, APP_PAPER_SENT, APP_SPECIAL));
$sql.= ("
    FROM job_info AS a, student_status as st
    LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
    LEFT JOIN applications AS app ON a.job_id=app.job_id AND app.app_status IN (".$apptypes.")
    LEFT JOIN students_shortlisted AS ss ON (ss.job_id=a.job_id AND ss.student_number='" . $student_number . "') ");
    if($folder_id)
    {
        if($folder_id == -1)
        {
            $sql.=" LEFT JOIN student_job_folder_join AS sjfj ON a.job_id = sjfj.job_id AND sjfj.student_number='" . $student_number . "'";
        }
        else
        {
            $sql.=" INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.folder_id = '".$folder_id."' AND sjfj.student_number='" . $student_number . "'";
        }
    }
    $sql .= (" LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
    WHERE (a.status=" . CLOSED . " OR (a.status=" . CANCELLED . " AND a.student_status='7'))
    ");

    if($folder_id == -1)
    {
        $sql.=" AND sjfj.job_id IS NULL ";
    }
    $sql.=(" AND a.closing_date!=''
    AND st.status_id=a.student_status
    AND a.term_id='" . addslashes($show_term) . "'
    AND a.year='" . addslashes($show_year) . "'
     ");
$sql .= $extra_sql;
$sql .= (" GROUP BY a.job_id ");
$sql .= $order_sql;

$num_closed_result = $GLOBALS['dbh']->Execute($sql);
$num_closed = $num_closed_result->RecordCount();

if (($closed_per_page_max = (int)$closed_per_page_max) < 5){
    $closed_per_page_max = 5;
}
if ($num_closed > 20)
{
    $sql .= ("
        LIMIT " . $closed_start_row . ", " . $closed_per_page_max
        );
}

$result = $GLOBALS['dbh']->Execute($sql);

    $closed_pages = ceil($num_closed / $closed_per_page_max);

    $first = $closed_start_row + 1;
    $end = $closed_start_row + $closed_per_page_max;
    if ($end > $num_closed) { $end = $num_closed; }

    if($folder_id)
    {
        if($folder_id == -1)
        {
            $folder_name = "that are NOT assigned to a folder";
        }
        else
        {
            $folder_name= "in the ".getFolderName($folder_id, $student_number)." folder";
        }
        echo("<h4 align='left'>Closed jobs for " . getTermName($show_term) . ", " . $show_year);
        echo("  (" . $num_closed. ") ".$folder_name.":</h4>");
    }
    else
    {   
        echo("<h4 align='left'>Closed jobs for " . getTermName($show_term) . ", " . $show_year);
        echo(" (" . $num_closed . "):</h4>");
    }

    if ($num_closed > 20)
    {
        echo("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

        echo("<tr>");
        echo("<td>");
        echo($num_closed . " job" . (($num_closed != 1) ? "s" : "") . " on " . $closed_pages . " page" . (($closed_pages > 1) ? "s" : "").";</td>");
        echo("<td><input type='text' name='closed_per_page_max' size='4' maxlength='4' value='" . $closed_per_page_max . "' />");
        echo("<input type='hidden' name='order' value='$order' />");
        echo("<input type='hidden' name='data_flow' value='$data_flow' />");
        echo("<input type='hidden' name='change' value='no' />");
        echo(" jobs per page.");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");

        echo("<td>");
        echo("<select name='closed_start_row'>");

        for ($i = 0; $i < $closed_pages; $i++)
        {
            $closed_row_num_start = $i * $closed_per_page_max;
            $closed_row_num_end = $closed_row_num_start + $closed_per_page_max - 1;
            if ($closed_row_num_end >= $num_closed) $closed_row_num_end = $num_closed - 1;

            $num_closed_result->Move($closed_row_num_start);
            $num_closed_result_row = $num_closed_result->FetchRow();
            $closed_start = $num_closed_result_row[0];

            if (strlen($closed_start) > 15) { $closed_start = substr($closed_start, 0, 13) . "..."; }

            $num_closed_result->Move($closed_row_num_end);
            $num_closed_result_row = $num_closed_result->FetchRow();
            $closed_end = $num_closed_result_row[0];

            if (strlen($closed_end) > 15) { $closed_end = substr($closed_end, 0, 13) . "..."; }

            if ($order=='min_academic'){
                $closed_start = "Min year ".$closed_start;
                $closed_end = "Min year ".$closed_end;
            }
            if($order =='app_count'){
                $closed_start = getApplicantRange($closed_start);
                $closed_end = getApplicantRange($closed_end);
            }
            echo("<option value='" . $closed_row_num_start . "' ");
            if ($closed_start_row == $closed_row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . $closed_start . " to " . $closed_end . "</option>\n");
        }

        echo("</select>");
        echo("</td>");

        echo("<td align='right'>");
        echo("<input type='submit' name='continue' value='View' />");
        echo("</td>");

        echo("</tr>");
        echo("</table>");
    }

    //$href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs). "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
    //$href .= ("&amp;show_future=" . urlencode($show_future) . "&amp;folder_id=".$folder_id."&amp;order=");

    $href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;closed_start_row=" . $closed_start_row);
    $href .= ("&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;show_future=" . urlencode($show_future));
    $href .= ("&amp;show_term_year=".$show_term_year);
    $href .= ("&amp;job_page=".$job_page. "&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
    $href .= ("&amp;open_start_row=" . $open_start_row . "&amp;open_per_page_max=" . $open_per_page_max . "&amp;folder_id=".$folder_id."&amp;order=");
    ?>

    <table cellspacing='0' cellpadding='0' border='1'>
    <tr>
        <td>
        <table border='0' cellpadding='2'>
        <tr>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>&nbsp;</b>&nbsp;</td>
            <td class='rowgrey' align='center'><img src='misc/images/job_flagged.gif' alt='' /></td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>company_name'><b class="white">Company</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>min_academic'><b class="white">Min Year</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>closing_date'><b class="white">Closing Date</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>status'><b class='white'>Status</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Your Status</b>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>app_count'><b class="white"># Students Applied</b></a>&nbsp;</td>
        </tr>

    <?php
    $rowclass = 0;
    $counter = 0;

    while ($row = $result->FetchRow())
    {
        $counter++;
        $job_code = $row["job_code"];
        $company_name = $row["company_name"];
        $division_name = $row["department_name"];
        $student_status = $row["student_status"];
        $job_id = $row["job_id"];
        $min_academic = $row["min_academic"];
        $total_apps3 = $row["app_count"];
        $closing_date = $row["closing_date"];
        $last_updated_on = $row["last_updated_on"];
        $date_posted = $row["date_posted"];
        $resumes_pulled = $row["resumes_pulled"];
        
        $sql = ("
            SELECT DISTINCT d.department_code
            FROM department_job_join AS djj, department AS d
            WHERE djj.job_id='" . $job_id . "'
            AND d.department_id=djj.department_id
            ORDER BY d.department_id
            ");
        $result2 = $GLOBALS['dbh']->Execute($sql);

        $apply_departments = '';
        if ($result2->RecordCount())
        {
            while ($row2 = $result2->FetchRow())
            {
                $apply_departments .= ($row2["department_code"] . ", ");
            }
            $apply_departments = substr($apply_departments, 0, -2);
        }
        else
        {
            $apply_departments = ("None");
        }

        $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);
        $href .= ("&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_future=" . urlencode($show_future));

        echo("<tr>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        if (is_job_flagged($job_id, $student_number))
        {
            echo("<input type='checkbox' name='jobs_to_unflag[]' value='" . $job_id . "' />");
        }
        else
        {
            echo("<input type='checkbox' name='jobs_to_flag[]' value='" . $job_id . "' />");
        }
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo((is_job_flagged($job_id, $student_number)) ? "<img src='misc/images/job_flagged.gif' alt='' />" : "&nbsp;");
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo("&nbsp;<a class='blue' href=\"" . $href . "\">");
        echo(htmlentities($company_name) . (($division_name != $company_name) ? " (" . htmlentities($division_name) . ")" : ""));
        echo("</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo($apply_departments);
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        $disc_string = '';
        $disc_array = getDiscArray($job_id);
        foreach ($disc_array AS $da)
        {
            $disc_string .= ($da . ", ");
        }
        $disc_string = substr($disc_string, 0, -2);
        echo($disc_string);
        echo("</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($min_academic . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo($closing_date . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo(getStudentStatusGif($job_id));
        echo("</td>\n");

        echo("<td nowrap='nowrap' align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
        echo("&nbsp;");
        // Determine how far this student has progressed in this job.
        echo(getStudentOwnStatusGif($job_id, $student_number));
        echo("</td>");
        
        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;".getApplicantRange($total_apps3)."&nbsp;");
        echo("</td>\n");

        echo("</tr>\n");
        $rowclass++;
        
    }

    echo("<tr>");
        echo("<td align='center' colspan='11' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            if($counter == 0)
            {
                if($folder_id)
                {
                    if($folder_id == -1)
                    {
                        $folder_name = "that are NOT assigned to a folder";
                    }
                    else
                    {
                        $folder_name= "in the ".getFolderName($folder_id, $student_number)." folder";
                    }
                    echo("<b>There are no Closed jobs ".$folder_name."</b>");
                }
                else
                {
                    echo("<b>There are no Closed jobs for " . getTermName($show_term) . ", " . $show_year."</b>");
                }
            }
            else{

                echo("<input type='submit' name='level1' value='Flag/Unflag Selected Jobs' />&nbsp;<b> -OR- </b>&nbsp;");
                echo("<input type='hidden' name='order' value='$order' />");
                echo("<input type='hidden' name='closed_row_num_start' value='" . $closed_row_num_start . "' />");
                echo("<input type='hidden' name='folder_id' value='".$folder_id."' />");
                echo("<select name = 'move_folder_id'>");
                echo("<option value='0'>Move Selected jobs to Folder</option>\n");
                $sql = ("
                        SELECT folder_id
                        FROM student_job_folder
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
                while ($row = $result->FetchRow())
                {
                    echo("<option value='".$row["folder_id"]."'>".getFolderName($row["folder_id"],$student_number)."</option>\n");
                }
                if($folder_id)
                {
                    echo("<option value='-1'>No Folder</option>\n");
                }

                echo("</select>");
                echo("&nbsp;&nbsp;<input type = 'submit' name='level1' value='Move' />");
            }
            echo("</td>");
    echo("</tr>");

    echo("</table>");
    echo("</td></tr></table>");
    echo("</form>");

?>

<h4>Status Icon Key:</h4>

<table cellspacing="0" cellpadding="0" border="1">
<tr>
<td>
<table border="0" cellpadding="2">
<?php

echo("<tr>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/doc10.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName("1") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/cancel.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName("2") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/Warning.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;Applications pulled by admin&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/job_shortlist.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName("3") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/clock.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName("4") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/smchk_gr.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName("5") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/smchk_yl.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;" . getStudentStatusName("6") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/X.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;" . getStudentStatusName("7") . "&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_applied.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;You have applied to this job&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/your_app_has_been_pulled.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;Your application was added by admin&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_shortlisted.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;You have been shortlisted&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/you_have_an_interview.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;You have an interview&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row0d' align='center'>&nbsp;<img src='misc/images/you_have_been_placed.gif' alt='' />&nbsp;</td>");
echo("<td class='row0d' align='center'>&nbsp;You have been placed&nbsp;</td>");
echo("</tr>");
echo("<tr>");
echo("<td class='row1d' align='center'>&nbsp;<img src='misc/images/no_more_progress.gif' alt='' />&nbsp;</td>");
echo("<td class='row1d' align='center'>&nbsp;You did not get this job&nbsp;</td>");
echo("</tr>");

?>
</table>
</td>
</tr>
</table>

