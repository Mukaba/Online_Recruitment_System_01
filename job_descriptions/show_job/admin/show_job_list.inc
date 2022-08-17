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
 | Description: This file shows all of the jobs to the administrator.           |
 +------------------------------------------------------------------------------+

*/

// These variables will help reduce the problem of too many companies on a screen

if ($open_per_page_max == "") { $open_per_page_max = 50; }
if ($open_per_page_max < 5) { $open_per_page_max = 5; }
if ($open_start_row == '') { $open_start_row = 0; }

if ($closed_per_page_max == "") { $closed_per_page_max = 25; }
if ($closed_per_page_max < 5) { $closed_per_page_max = 5; }
if ($closed_start_row == '') { $closed_start_row = 0; }

$nextTermYear = getNextTermYear();

// If no values have been given for show_term and show_year yet, use the current ones.

if (!$show_term)
{
    $show_term = $nextTermYear["term"];
}
if (!$show_year)
{
    $show_year = $nextTermYear["year"];
}

// Use the department if no value has been given for show_jobs

if (!$show_jobs) {
    if ($userlevel == OFFICE) {
        $show_jobs = 'department';
    } else {
        $show_jobs = 'uvic';
    }
}
/*
   First, figure out which jobs we will be showing.  Do this by switching the show_jobs variable,
   and creating an extra little bit to stick in our queries.
 */
switch ($show_jobs)
{
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

            // Now pull out all departments associated with this group, and put them into a comma delimited string.

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
                WHERE department_id IN (" . $department_ids . ")
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }
            $job_id_list = substr($job_id_list, 0 , -1);

            if (!$job_id_list)
            {
                $job_id_list = ("''");
            }

            $extra_sql = ("
                AND (a.job_id IN (" . $job_id_list . ") OR a.department_id IN (" . $department_ids . "))
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
        $job_id_list = substr($job_id_list, 0, -1);

        if (!$job_id_list)
        {
            $job_id_list = ("''");
        }

        $extra_sql = ("
            AND (a.job_id IN (" . $job_id_list . ") OR a.department_id='" . $auth->department . "')
            ");
        break;

    case "host":
        
        // Get all the jobs that the department is hosting
        $sql = ("
            SELECT job_id
            FROM job_info
            WHERE department_id = '".$auth->department."' 
            ");
        
        $result = $GLOBALS['dbh']->Execute($sql);
        
        while ($row = $result->FetchRow())
        {
            $job_id_list .= ("'".$row["job_id"]."',");
        }
        $job_id_list = substr($job_id_list, 0, -1);

        if (!$job_id_list)
        {
            $job_id_list = ("''");
        }
        $extra_sql = ("
            AND (a.job_id IN (" . $job_id_list . ") OR a.department_id='" . $auth->department . "')
            ");
        break;
        
    case "exclude_group":

        /*
         If they want to exclude those jobs for their group, first check if they're department is part of a group,
         If they're not, then just continue on and display as if they had selected to view jobs excluding their
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

            // Now pull out all departments associated with this group, and put them into a comma delimited string
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
            that have one of these departments listed. These job_ids will be excluded from 
            the query.
            */

            $sql = ("
                SELECT DISTINCT job_id
                FROM department_job_join
                WHERE department_id IN (" . $department_ids . ")
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
        
            while ($row = $result->FetchRow())
            {
                $job_id_list .= ("'" . $row["job_id"] . "',");
            }
            $job_id_list = substr($job_id_list, 0 , -1);

            if (!$job_id_list)
            {
                $job_id_list = ("''");
            }
            //now exclude these jobs from our query
            $extra_sql = ("
                AND (a.job_id NOT IN (" . $job_id_list . ") )
                ");
            break;
        }
    case "exclude_department":
        // Get all of the jobs that have this department listed as applicable. These will be excluded from the query
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
        $job_id_list = substr($job_id_list, 0, -1);

        if (!$job_id_list)
        {
            $job_id_list = ("''");
        }

        $extra_sql = ("
            AND (a.job_id NOT IN (" . $job_id_list . ")) 
        ");
        break;
    case "uvic":
    default:

        // If they want to see all jobs registered at UVic, then do nothing to the query.
        if ($userlevel == TRAFFICDIRECTOR || $userlevel == DIRECTORS_OFFICE) {
            if ($show_jobs && $show_jobs != 'uvic') {
                // Get all of the jobs that have this department listed as applicable.
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
                $job_id_list = substr($job_id_list, 0, -1);

                if (!$job_id_list)
                {
                    $job_id_list = ("''");
                }

                $extra_sql = ("
                        AND job_id IN (" . $job_id_list . ")
                        ");
            }
        }
        break;
} //End of switch statement.

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='view_job' />");
echo("<input type='hidden' name='show_term' value='" . $show_term . "' />");
echo("<input type='hidden' name='show_year' value='" . $show_year . "' />");
echo("<input type='hidden' name='show_closed' value='" . $show_closed . "' />");
echo("<input type='hidden' name='show_jobs' value='" . $show_jobs . "' />");
echo("<input type='hidden' name='order' value='" . $order . "' />");

$sql = "SELECT DISTINCT ";
switch ($order)
{
    case 'company_name':
        $sql.="b.company_name, a.job_code, a.job_id, a.min_academic, a.closing_date, a.admin_status, a.status, a.department_id, a.position_title, ed.department_name";
        break;

    case 'closing_date':
        $sql.="a.closing_date, b.company_name, a.job_code, a.job_id, a.min_academic, a.admin_status, a.status, a.department_id, a.position_title, ed.department_name ";
        break;

    case 'status':
        $sql.="a.admin_status, a.status, a.closing_date, b.company_name, a.job_code, a.job_id, a.min_academic, a.department_id, a.position_title, ed.department_name ";
        break;

    case 'min_academic':
        $sql.="a.min_academic, a.job_code, a.job_id, a.closing_date, a.status, a.admin_status, b.company_name, a.department_id, a.position_title, ed.department_name ";
        break;
    case 'job_code':
        $sql.="a.job_code, b.company_name, a.job_id, a.min_academic, a.closing_date, a.status, a.admin_status, a.department_id, a.position_title, ed.department_name ";
        break;
    default:
        $sql.="b.company_name, a.job_code, a.job_id, a.min_academic, a.closing_date, a.status, a.admin_status, a.department_id, a.position_title, ed.department_name ";
        break;
}

$sql .= ("
        FROM job_info AS a 
        LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
        LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
        WHERE a.status!=" . INT_ONLY . "
        AND a.status!=" . REJECTED . "
        AND a.status!=" . CLOSED . "
        AND a.status!=" . CANCELLED
        );
// If a term and year were specified, set up the query to include those.

if ($show_term != 'ALL')
{
    $sql .= ("
            AND a.term_id='" . $show_term . "'
            ");
}
if ($show_year != 'ALL')
{
    $sql .= ("
            AND a.year='" . $show_year . "'
            ");
}

$sql .= $extra_sql;
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
        $order_sql = " ORDER BY b.company_name $data_flow, a.job_code, a.closing_date";
    }
    elseif($order == "closing_date")
    {
        $order_sql = " ORDER BY a.closing_date $data_flow, a.job_code, b.company_name";
    }
    elseif($order == "status")
    {
        $order_sql = " ORDER BY a.admin_status $data_flow, a.job_code, b.company_name";
    }
    elseif($order == "min_academic")
    {
        $order_sql = " ORDER BY a.min_academic $data_flow, a.job_code, b.company_name";
    }
    else
    {
        $order_sql = " ORDER BY a.job_code $data_flow, b.company_name, a.closing_date";
    }
}
else
{
    $order_sql = " ORDER BY  b.company_name, a.job_code, a.closing_date";
    $data_flow="ASC";
    $order="company_name";
}

$sql .= $order_sql;


$num_open_result = $GLOBALS['dbh']->Execute($sql);
$num_open = $num_open_result->RecordCount();

if ($num_open > 20)
{
    $sql .= ("
        LIMIT " . $open_start_row . ", " . $open_per_page_max
        );
}

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0)
{
    // if there are no job in the database, indicate this

    if ($userlevel == OFFICE) {
        if ($show_term != 'ALL' || $show_year != 'ALL')
        {
            notify("There are no current jobs registered in the system for the chosen term/year.");
        }
        else
        {
            notify("There are no current jobs registered in the system.");
        }
    } else {
        if ($show_jobs != 'uvic')
        {
            $sql = ("
                    SELECT DISTINCT department_name
                    FROM department
                    WHERE department_id='" . $show_jobs . "'
                    ");
            $result2 = $GLOBALS['dbh']->Execute($sql);
            $row2 = $result2->FetchRow();
            $department_name = $row2["department_name"];
        }

        if ($show_term != 'ALL' || $show_year != 'ALL')
        {
            notify("There are no current " . ($department_name ? "<b>" . $department_name . "</b>" : "") . " jobs registered in the system for the chosen term/year.");
        }
        else
        {
            notify("There are no current " . ($department_name ? "<b>" . $department_name . "</b>" : "") . " jobs registered in the system.");
        }
    }

} 
else
{
    // there are jobs in the system

    $open_pages = ceil($num_open / $open_per_page_max);

    $first = $open_start_row + 1;
    $end = $open_start_row + $open_per_page_max;
    if ($end > $num_open) { $end = $num_open; }

    if ($show_year != 'ALL' && $show_term != 'ALL')
    {
        echo("<h4 align='left'>Current jobs for " . getTermName($show_term) . ", " . $show_year . ":</h4>");
    }
    elseif ($show_year != 'ALL')
    {
        echo("<h4 align='left'>Current jobs in " . $show_year . ":</h4>");
    }
    elseif ($show_term != 'ALL')
    {
        echo("<h4 align='left'>Current jobs in " . getTermName($show_term) . ":</h4>");
    }
    else
    {
        echo("<h4 align='left'>Current jobs registered in the system:</h4>");
    }

    if ($num_open > 20)
    {
        echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

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

            $num_open_result->Move($open_row_num_end);
            $num_open_result_row = $num_open_result->FetchRow();
            $open_end = $num_open_result_row[0];

            if ($order=='job_code' OR $order=='closing_date' OR !$order)
            {
                if ($open_start == '')
                {
                    $open_start = 'Not Yet Set';
                }
                if ($open_end == '')
                {
                    $open_end = 'Not Yet Set';
                }
            }

            if ($order=='status') {
                $sql_status = "SELECT status_name from admin_status where status_id=".$open_start;
                $result_status=$GLOBALS['dbh']->Execute($sql_status);
                while ($row_status = $result_status->FetchRow()) {
                    $open_start = $row_status['status_name'];
                }
                $sql_status2 = "SELECT status_name from admin_status where status_id=".$open_end;
                $result_status2=$GLOBALS['dbh']->Execute($sql_status2);
                while ($row_status2 = $result_status2->FetchRow()) {
                    $open_end = $row_status2['status_name'];
                }
            }
            if ($order=='min_academic'){
                $open_start = "Min year ".$open_start;
                $open_end = "Min year ".$open_end;
            }

            if (strlen($open_start) > 15) { $open_start = substr($open_start, 0, 13) . "..."; }
            if (strlen($open_end) > 15) { $open_end = substr($open_end, 0, 13) . "..."; }

            echo("<option value='" . $open_row_num_start . "' ");
            if ($open_start_row == $open_row_num_start)
            {
                echo("selected='selected'");
            }
            echo(">" . htmlentities($open_start) . " to " . htmlentities($open_end) . "</option>\n");
        }

        echo("</select>");
        echo("</td>");

        echo("<td>");
        echo("<input type='submit' name='continue' value='View' />");
        echo("</td>");

        echo("</tr>");
        echo("</table>");
    }

    $href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_term=" . $show_term."&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
    $href .= ("&amp;show_year=" . $show_year . "&amp;show_closed=" . urlencode($show_closed) . "&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;open_per_page_max=" . $open_per_pag_max . "&amp;order=");

    ?>
        <table cellspacing="0" cellpadding="0" border="1"><tr><td>
        <table border="0" cellpadding="2">
        <tr>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>company_name'><b class='white'>Company Name</b></a>&nbsp;</td> 
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>job_code'><b class="white">Job Code</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td> 
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td> 
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td> 
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>closing_date'><b class="white">Closing Date</b></a>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>status'><b class="white">Status</b></a>&nbsp;</td>
        </tr>
        <?php
        $rowclass = 0;
    while ($row = $result->FetchRow())
    {
        $job_code = $row["job_code"];
        $company_name = $row["company_name"];
        $division_name = $row["department_name"];
        $min_academic = $row["min_academic"];
        $closing_date = validDate($row["closing_date"]);
        $status= $row["status"];
        $job_id = $row["job_id"];
        $department_id = $row["department_id"];
        $position_title = $row["position_title"];

        $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);
        $href .= ("&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_term=" . $show_term . "&amp;show_year=");
        $href .= ($show_year ."&amp;show_quick=true". "&amp;show_closed=" . urlencode($show_closed));

        echo("<tr>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo("&nbsp;<a class='blue' href=\"" . $href . "\">");
        echo(htmlentities($company_name) . (($division_name != $company_name) ? " (" . htmlentities($division_name) . ")</a>&nbsp;" : "</a>&nbsp;"));
        echo("</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo("&nbsp;<a class='blue' href=\"$href\">" . ($job_code ? $job_code : "Not Yet Set") . "</a>&nbsp;</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo(htmlentities($position_title));
        echo("</td>");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        $dept_string = '';
        $dept_array = getDeptIDArray($job_id);
        for ($i = 0; $i < sizeof($dept_array); $i++)
        {
            if ($dept_array[$i] == $department_id) {
                $dept_string .= "<b class=\"black\">";
            }

            $dept_string .= getDepartmentCode($dept_array[$i]);

            if ($dept_array[$i] == $department_id) {
                $dept_string .= "</b>";
            }

            if ($i < sizeof($dept_array) - 1) {
                $dept_string .= ", ";
            }
        }
        echo($dept_string);
        echo("</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
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
        echo(($closing_date != -1 ? $closing_date : "Not Yet Set") . "</td>\n");

        echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo(getAdminStatusGif($job_id));
        echo("</td>\n");

        echo("</tr>\n");
        $rowclass++;
    }
    echo("</table>");
    echo("</td></tr></table>");

}

if ($show_closed == 'Show Closed Jobs')
{

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
        case 'company_name':
            $sql.="b.company_name, a.job_code, a.job_id, a.min_academic, a.closing_date, a.admin_status, a.status, a.department_id, a.position_title, ed.department_name ";
            break;

        case 'closing_date':
            $sql.="a.closing_date, b.company_name, a.job_code, a.job_id, a.min_academic, a.admin_status, a.status, a.department_id, a.position_title, ed.department_name ";
            break;

        case 'status':
            $sql.="a.admin_status, a.status, a.closing_date, b.company_name, a.job_code, a.job_id, a.min_academic, a.department_id, a.position_title, ed.department_name ";
            break;

        case 'min_academic':
            $sql.="a.min_academic, a.job_code, a.job_id, a.closing_date, a.status, a.admin_status, b.company_name, a.department_id, a.position_title, ed.department_name ";
            break;
        case 'job_code':
            $sql.="a.job_code, b.company_name, a.job_id, a.min_academic, a.closing_date, a.status, a.admin_status, a.department_id, a.position_title, ed.department_name ";
            break;
        default:
            $sql.="b.company_name, a.job_code, a.job_id, a.min_academic, a.closing_date, a.status, a.admin_status, a.department_id, a.position_title, ed.department_name ";
            break;
    }

    $sql.= ("
            FROM job_info AS a 
            LEFT JOIN employer_company AS b ON b.employer_id=a.employer_id
            LEFT JOIN employer_department AS ed ON ed.department_id=a.employer_department_id
            WHERE (a.status!=" . INT_ONLY . " AND a.status!=" . REJECTED . ")
            AND a.status=" . CLOSED . "
            ");
    //AND (a.status=" . CLOSED . " OR a.status=" . CANCELLED . ")";

    // If a term and year were specified, set up the query to include those.

    if ($show_term != 'ALL')
    {
        $sql .= ("
                AND a.term_id='" . $show_term . "'
                ");
    }
    if ($show_year != 'ALL')
    {
        $sql .= ("
                AND a.year='" . $show_year . "'
                ");
    }

    $sql .= $extra_sql;

    $sql .= $order_sql;
    $num_closed_result = $GLOBALS['dbh']->Execute($sql);
    $num_closed = $num_closed_result->RecordCount();

    if ($num_closed > 20)
    {
        $sql .= ("
            LIMIT " . $closed_start_row . ", " . $closed_per_page_max
            );
    }

    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() == 0)
    {
        // if there are no closed jobs in the database, indicate this

        if ($show_term != 'ALL' || $show_year != 'ALL')
        {
            //echo("<p>There are currently no closed jobs registered in the system for the chosen term/year.</p>\n")
        }
        else
        {
            //echo("<p>There are currently no closed jobs registered in the system.</p>\n");
        }
    } else
    {
        // there are jobs in the system


        $closed_pages = ceil($num_closed / $closed_per_page_max);

        $first = $closed_start_row + 1;
        $end = $closed_start_row + $closed_per_page_max;
        if ($end > $num_closed) { $end = $num_closed; }

        if ($show_year != 'ALL' && $show_term != 'ALL')
        {
            echo("<h4 align='left'>Closed jobs for " . getTermName($show_term) . ", " . $show_year . ":</h4>");
        }
        elseif ($show_year != 'ALL')
        {
            echo("<h4 align='left'>Closed jobs in " . $show_year . ":</h4>");
        }
        elseif ($show_term != 'ALL')
        {
            echo("<h4 align='left'>Closed jobs in " . getTermName($show_term) . ":</h4>");
        }
        else
        {
            echo("<h4 align='left'>Closed jobs in the system:</h4>");
        }

        if ($num_closed > 20)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

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

                    $num_closed_result->Move($closed_row_num_end);
                    $num_closed_result_row = $num_closed_result->FetchRow();
                    $closed_end = $num_closed_result_row[0];

                    if ($order=='job_code' OR $order=='closing_date' OR !$order)
                    {
                        if ($closed_start == '')
                        {
                            $closed_start = 'Not Yet Set';
                        }
                        if ($closed_end == '')
                        {
                            $closed_end = 'Not Yet Set';
                        }
                    }

                if ($order=='status') {
                    $sql_status = "SELECT status_name from admin_status where status_id=".$closed_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $closed_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name from admin_status where status_id=".$closed_end;
                    $result_status2=$GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $closed_end = $row_status2['status_name'];
                    }
                }
                if ($order=='min_academic'){
                    $closed_start = "Min year ".$closed_start;
                    $closed_end = "Min year ".$closed_end;
                }
                if (strlen($closed_start) > 15) { $closed_start = substr($closed_start, 0, 13) . "..."; }
                if (strlen($closed_end) > 15) { $closed_end = substr($closed_end, 0, 13) . "..."; }

                echo("<option value='" . $closed_row_num_start . "' ");
                if ($closed_start_row == $closed_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . htmlentities($closed_start) . " to " . htmlentities($closed_end) . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }

        $href = ($PHP_SELF . "&amp;select=view_job&amp;show_jobs=" . urlencode($show_jobs) . "&amp;show_term=" . $show_term."&amp;lastorder=".$order."&amp;data_flow=".$data_flow);
        $href .= ("&amp;show_year=" . $show_year . "&amp;show_closed=" . urlencode($show_closed) . "&amp;closed_per_page_max=" . $closed_per_page_max . "&amp;open_per_page_max=" . $open_per_pag_max . "&amp;order=");

        ?>
            <table cellspacing="0" cellpadding="0" border="1"><tr><td>
            <table border="0" cellpadding="2">
            <tr>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>job_code'><b class="white">Job Code</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td> 
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td> 
            <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td> 
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>closing_date'><b class="white">Closing Date</b></a>&nbsp;</td>
            <td class="rowgrey" align='center'>&nbsp;<a class='orderable' href='<?php echo($href); ?>status'><b class="white">Status</b></a>&nbsp;</td>
            </tr>
            <?php
            $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $job_code = $row["job_code"];
            $company_name = $row["company_name"];
            $division_name = $row["department_name"];
            $min_academic = $row["min_academic"];
            $closing_date = validDate($row["closing_date"]);
            $status= $row["status"];
            $job_id = $row["job_id"];
            $department_id = $row["department_id"];
            $position_title = $row["position_title"];

            $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);
            $href .= ("&amp;show_jobs=" . urlencode($show_jobs)."&amp;show_quick=true" . "&amp;show_term=" . $show_term . "&amp;show_year=");
            $href .= ($show_year . "&amp;show_closed=" . urlencode($show_closed));

            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"" . $href . "\">");
            echo(htmlentities($company_name) . (($division_name != $company_name) ? " (" . htmlentities($division_name) . ")</a>&nbsp;" : "</a>&nbsp;"));
            echo("</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">" . ($job_code ? $job_code : "Not Yet Set") . "</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo(htmlentities($position_title));
            echo("</td>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $dept_string = '';
            $dept_array = getDeptIDArray($job_id);
            for ($i = 0; $i < sizeof($dept_array); $i++)
            {
                if ($dept_array[$i] == $department_id) {
                    $dept_string .= "<b class=\"black\">";
                }

                $dept_string .= getDepartmentCode($dept_array[$i]);

                if ($dept_array[$i] == $department_id) {
                    $dept_string .= "</b>";
                }

                if ($i < sizeof($dept_array) - 1) {
                    $dept_string .= ", ";
                }
            }
            echo($dept_string);
            echo("</td>\n");

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
            echo(($closing_date != -1 ? $closing_date : "Not Yet Set") . "</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo(getAdminStatusGif($job_id));
            echo("</td>");
            echo("</tr>\n");
            $rowclass++;
        }
        echo("</table>");
        echo("</td></tr></table>");
    }
}

// Status icon key

echo("<h4>Status Icon Key</h4>");

echo("<table cellspacing='0' cellpadding='0' border='1'>");
echo("<tr>");
echo("<td>");
echo("<table border='0' cellpadding='2'>");

echo("<tr>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");

echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Icon</b>&nbsp;</td>");
echo("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
echo("</tr>");

$admin_status_array = getAdminStatusGifArray();
$newrow = 0;
$rowclass = 0;

foreach ($admin_status_array AS $asa)
{
    if (!($newrow % 2))
    {
        echo("<tr>");
    }
    echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
    echo($asa["gif"]);
    echo("</td>\n");

    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
    echo($asa["name"]);
    echo("</td>\n");

    if ($newrow % 2)
    {
        echo("</tr>");
        $rowclass++;
    }

    $newrow++;
}

if ($newrow % 2)
{
    // Fill out any extra columns
    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
    echo("<td class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>&nbsp;</td>");
    echo("</tr>");
}

echo("</table>");
echo("</td>");
echo("</tr>");
echo("</table>");
echo("</form>");

?>
