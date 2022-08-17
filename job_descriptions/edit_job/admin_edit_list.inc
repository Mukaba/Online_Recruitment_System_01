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
 | Filename: admin_edit_list.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This file loads up all of the jobs, and allows the admin to     |
 | select one of them to edit.                                                  |
 +------------------------------------------------------------------------------+

*/

// Paging variables
// number of items at which we begin to display paging options form
$paging_overflow = 20;

if ($unchecked_per_page_max == "") { $unchecked_per_page_max = 20; }
if ($unchecked_per_page_max < 5) { $unchecked_per_page_max = 5; }
if ($unchecked_start_row == '') { $unchecked_start_row = 0; }

if ($checked_per_page_max == "") { $checked_per_page_max = 20; }
if ($checked_per_page_max < 5) { $checked_per_page_max = 5; }
if ($checked_start_row == '') { $checked_start_row = 0; }

if ($open_per_page_max == "") { $open_per_page_max = 20; }
if ($open_per_page_max < 5) { $open_per_page_max = 5; }
if ($open_start_row == '') { $open_start_row = 0; }

if ($closed_per_page_max == "") { $closed_per_page_max = 20; }
if ($closed_per_page_max < 5) { $closed_per_page_max = 5; }
if ($closed_start_row == '') { $closed_start_row = 0; }

if ($cancelled_per_page_max == "") { $cancelled_per_page_max = 20; }
if ($cancelled_per_page_max < 5) { $cancelled_per_page_max = 5; }
if ($cancelled_start_row == '') { $cancelled_start_row = 0; }

if ($future_per_page_max == "") { $future_per_page_max = 20; }
if ($future_per_page_max < 5) { $future_per_page_max = 5; }
if ($future_start_row == '') { $future_start_row = 0; }

$nextTermYear = getNextTermYear();

echo("<form method='post' action='" . $PHP_SELF . "'>");
echo("<input type='hidden' name='select' value='edit_job' />");
echo("<input type='hidden' name='order' value='".$order."' />");

echo("<table border='0' cellpadding='2' cellspacing='0'>");

echo("<tr align='center'>");
echo("<td class='row1'>");
echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");

echo("<tr align='center'>");
	echo("<td>");
	echo("<select name='show_future'>");
	echo("<option value='show_future_jobs' ");
	if ($show_future == 'show_future_jobs')
	{
		echo("selected='selected'");
	}
	echo(">Show Future Jobs</option>");
	
	echo("<option value='hide_future_jobs' ");
	if (!$show_future || $show_future == 'hide_future_jobs')
	{
		echo("selected='selected'");
	}
	echo(">Hide Future Jobs</option>");

	echo("</select>");
	echo("</td>");

	echo("<td>");
	echo("<input type='submit' value='Go' />");
	echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("<br />");

//echo("</form>");

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
}
else
{
	$department_ids = "'" . $auth->department . "'";
}

$sql = ("
	SELECT DISTINCT position_title, closing_date, job_id, job_code
	FROM job_info
	WHERE term_id='" . $nextTermYear["term"] . "'
	AND year='" . $nextTermYear["year"] . "'
	AND status!=" . INT_ONLY . "
	AND department_id IN (" . $department_ids . ")
	");

$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0)
{
	// if there are no job in the database, indicate this 

	notify("There are no current jobs registered in the system.");
} 
else      
{
	// there are jobs in the system

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.department_id, ji.job_code, ji.job_id, ji.admin_status, ji.position_title, ec.company_name, ed.department_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.department_id, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ec.company_name, ed.department_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.department_id, ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ed.department_name";
                break;
            default:
            case 'job_code':
                $sql .= "ji.job_code, ji.department_id, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ec.company_name, ed.department_name";
                break;
    }

	$sql .= ("
		FROM job_info AS ji
        LEFT JOIN employer_company ec
          ON ji.employer_id = ec.employer_id
        LEFT JOIN employer_department AS ed 
          ON ji.employer_department_id=ed.department_id
		WHERE ji.status=" . NOT_CHECKED . "
		AND ji.department_id IN (" . $department_ids . ")
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		");
	$sql .= $extra_sql;
	
	$sql .= ("
		ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
        : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
        : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
        : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
        : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
		);

    $num_unchecked_result = $GLOBALS['dbh']->Execute($sql);
    $num_unchecked = $num_unchecked_result->RecordCount();

    if ($num_unchecked > $paging_overflow)
    {
        $sql .= ("
            LIMIT " . $unchecked_start_row . ", " . $unchecked_per_page_max
                );
    }
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
        echo("<h4>Unchecked Jobs:</h4>");

        $unchecked_pages = ceil($num_unchecked / $unchecked_per_page_max);

        $first = $unchecked_start_row + 1;
        $end = $unchecked_start_row + $unchecked_per_page_max;

        // paging options form
        if ($num_unchecked > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

            echo("<tr>");
            echo("<td>");
            echo($num_unchecked . " job" . (($num_unchecked != 1) ? "s" : "") . " on " . $unchecked_pages . " page" . (($unchecked_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='unchecked_per_page_max' size='4' maxlength='4' value='" . $unchecked_per_page_max . "' />");
            echo(" jobs per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='unchecked_start_row'>");

            for ($i = 0; $i < $unchecked_pages; $i++)
            {
                $unchecked_row_num_start = $i * $unchecked_per_page_max;
                $unchecked_row_num_end = $unchecked_row_num_start + $unchecked_per_page_max - 1;
                if ($unchecked_row_num_end >= $num_unchecked) $unchecked_row_num_end = $num_unchecked - 1;

                $num_unchecked_result->Move($unchecked_row_num_start);
                $num_unchecked_result_row = $num_unchecked_result->FetchRow();
                $unchecked_start = $num_unchecked_result_row[0];

                $num_unchecked_result->Move($unchecked_row_num_end);
                $num_unchecked_result_row = $num_unchecked_result->FetchRow();
                $unchecked_end = $num_unchecked_result_row[0];

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
                {
                    if ($unchecked_start == '')
                    {
                        $unchecked_start = 'Not Yet Set';
                    }
                    if ($unchecked_end == '')
                    {
                        $unchecked_end = 'Not Yet Set';
                    }
                }

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$unchecked_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $unchecked_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$unchecked_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $unchecked_end = $row_status2['status_name'];
                    }
                }
                
                if (strlen($unchecked_start) > 15) { $unchecked_start = substr($unchecked_start, 0, 13) . "..."; }
                if (strlen($unchecked_end) > 15) { $unchecked_end = substr($unchecked_end, 0, 13) . "..."; }

                echo("<option value='" . $unchecked_row_num_start . "' ");
                if ($unchecked_start_row == $unchecked_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $unchecked_start . " to " . $unchecked_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }

        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";



            echo("
            <table cellspacing='0' cellpadding='0' border='1'><tr><td>
            <table border='0' cellpadding='2'>
            <tr>  
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
            </tr>
            ");
            
        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];

            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }

            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $company_name = $row["company_name"];
            $position_title = $row["position_title"];
            $division_name = $row["department_name"];
            $department_id = $row["department_id"];
            
            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">");
            echo($company_name . (($division_name != $company_name) ? " (" . $division_name . ")</a>&nbsp;" : ""));
            echo("</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");

            $rowclass++;
        }
        echo("</table>");
        echo("</td></tr></table>");
	} 

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.department_id, ji.job_code, ji.job_id, ji.admin_status, ji.position_title, ec.company_name, ed.department_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.department_id, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ec.company_name, ed.department_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.department_id, ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ed.department_name";
                break;
            default:
            case 'job_code':
                $sql .= "ji.job_code, ji.department_id, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ec.company_name, ed.department_name";
                break;
    }

    $sql .= ("
		FROM job_info AS ji
        LEFT JOIN employer_company AS ec
          ON ji.employer_id = ec.employer_id
        LEFT JOIN employer_department AS ed 
          ON ji.employer_department_id=ed.department_id
		WHERE ji.status=" . CHECKED . "
		AND ji.department_id IN (" . $department_ids . ")
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
        ");
	$sql .= $extra_sql;

	$sql .= ("
		ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
        : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
        : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
        : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
        : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
		);

    $num_checked_result = $GLOBALS['dbh']->Execute($sql);
    $num_checked = $num_checked_result->RecordCount();

    if ($num_checked > $paging_overflow)
    {
        $sql .= ("
            LIMIT " . $checked_start_row . ", " . $checked_per_page_max
                );
    }
	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
        echo("<h4>Checked Jobs (not yet posted):</h4>");

        $checked_pages = ceil($num_checked / $checked_per_page_max);

        $first = $checked_start_row + 1;
        $end = $checked_start_row + $checked_per_page_max;

        // paging options form
        if ($num_checked > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

            echo("<tr>");
            echo("<td>");
            echo($num_checked . " job" . (($num_checked != 1) ? "s" : "") . " on " . $checked_pages . " page" . (($checked_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='checked_per_page_max' size='4' maxlength='4' value='" . $checked_per_page_max . "' />");
            echo("<input type='hidden' name='order' value='".$order."' />");
            echo(" jobs per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='checked_start_row'>");

            for ($i = 0; $i < $checked_pages; $i++)
            {
                $checked_row_num_start = $i * $checked_per_page_max;
                $checked_row_num_end = $checked_row_num_start + $checked_per_page_max - 1;
                if ($checked_row_num_end >= $num_checked) $checked_row_num_end = $num_checked - 1;

                $num_checked_result->Move($checked_row_num_start);
                $num_checked_result_row = $num_checked_result->FetchRow();
                $checked_start = $num_checked_result_row[0];

                $num_checked_result->Move($checked_row_num_end);
                $num_checked_result_row = $num_checked_result->FetchRow();
                $checked_end = $num_checked_result_row[0];

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
                {
                    if ($checked_start == '')
                    {
                        $checked_start = 'Not Yet Set';
                    }
                    if ($checked_end == '')
                    {
                        $checked_end = 'Not Yet Set';
                    }
                }

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$checked_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $checked_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$checked_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $checked_end = $row_status2['status_name'];
                    }
                }

                if (strlen($checked_start) > 15) { $checked_start = substr($checked_start, 0, 13) . "..."; }
                if (strlen($checked_end) > 15) { $checked_end = substr($checked_end, 0, 13) . "..."; }

                echo("<option value='" . $checked_row_num_start . "' ");
                if ($checked_start_row == $checked_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $checked_start . " to " . $checked_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }
        
        //$href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs")."&amp;order=";
        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";

	
        echo("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>  
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
                </tr>
                ");

        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];
            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }
            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $company_name = $row["company_name"];
            $position_title = $row["position_title"];
            $division_name = $row["department_name"];
            $department_id = $row["department_id"];

            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");
            $rowclass++;
        }
        echo("</table>");
        echo("</td></tr></table>");
    } 

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.job_code, ji.job_id, ji.position_title, ji.admin_status, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.admin_status, ji.department_id, ed.department_name";
                break;
            default:
            case 'job_code':
                $sql .= "ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ji.department_id, ed.department_name, ec.company_name";
                break;
    }

	$sql .= ("
		FROM job_info AS ji
        LEFT JOIN employer_company AS ec
          ON ji.employer_id = ec.employer_id
        LEFT JOIN employer_department AS ed 
          ON ji.employer_department_id=ed.department_id
		WHERE (ji.status=" . POSTED . " OR ji.status=" . HOLDING . ")
		AND ji.department_id IN (" . $department_ids . ")
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
        ");

    $sql .= $extra_sql;
	
	$sql .= ("
		ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
        : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
        : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
        : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
        : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
		);
	
    $num_open_result = $GLOBALS['dbh']->Execute($sql);
    $num_open = $num_open_result->RecordCount();

    if ($num_open > $paging_overflow)
    {
        $sql .= ("
            LIMIT " . $open_start_row . ", " . $open_per_page_max
                );
    }
	$result = $GLOBALS['dbh']->Execute($sql);
	if ($result->RecordCount() != 0)
	{
        echo("<h4>Open Jobs:</h4>");

        $open_pages = ceil($num_open / $open_per_page_max);

        $first = $open_start_row + 1;
        $end = $open_start_row + $open_per_page_max;

        // paging options form
        if ($num_open > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

            echo("<tr>");
            echo("<td>");
            echo($num_open . " job" . (($num_open != 1) ? "s" : "") . " on " . $open_pages . " page" . (($open_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='open_per_page_max' size='4' maxlength='4' value='" . $open_per_page_max . "' />");
            echo("<input type='hidden' name='order' value='".$order."' />");
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

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
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

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$open_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $open_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$open_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $open_end = $row_status2['status_name'];
                    }
                }

                if (strlen($open_start) > 15) { $open_start = substr($open_start, 0, 13) . "..."; }
                if (strlen($open_end) > 15) { $open_end = substr($open_end, 0, 13) . "..."; }

                echo("<option value='" . $open_row_num_start . "' ");
                if ($open_start_row == $open_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $open_start . " to " . $open_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }

        //$href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs")."&amp;order=";
        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";


        echo("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>  
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
                </tr>
                ");
        
        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];
            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }
            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $company_name = $row["company_name"];
            $position_title = $row["position_title"];
            $division_name = $row["department_name"];
            $department_id = $row["department_id"];

            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

            echo("<tr>");
            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");
            $rowclass++;
        }

        echo("</table>");
        echo("</td></tr></table>");
    }

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.job_code, ji.job_id, ji.position_title, ji.admin_status, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.department_id, ed.department_name, ji.admin_status";
                break;
            case 'job_code':
                $sql .= " ji.job_code, ec.company_name, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ji.department_id, ed.department_name ";
                break;
            default:
                $sql .= " ec.company_name, ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ji.department_id, ed.department_name ";
                break;
    }

    $sql .= ("
            FROM job_info AS ji
            LEFT JOIN employer_company AS ec
              ON ji.employer_id = ec.employer_id
            LEFT JOIN employer_department AS ed 
              ON ji.employer_department_id=ed.department_id
            WHERE ji.status=" . CLOSED . "
            AND ji.department_id IN (" . $department_ids . ")
            AND ji.term_id='" . $nextTermYear["term"] . "'
            AND ji.year='" . $nextTermYear["year"] . "'
            AND ec.employer_id=ji.employer_id
            ");

    $sql .= $extra_sql;

    $sql .= ("
            ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
            : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
            : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
            : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
            : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
            );

    $num_closed_result = $GLOBALS['dbh']->Execute($sql);
    $num_closed = $num_closed_result->RecordCount();

    if ($num_closed > $paging_overflow)
    {
        $sql .= ("
                LIMIT " . $closed_start_row . ", " . $closed_per_page_max
                );
    }

    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() != 0)
    {
        echo("<h4>Closed Jobs:</h4>");

        $closed_pages = ceil($num_closed / $closed_per_page_max);

        $first = $closed_start_row + 1;
        $end = $closed_start_row + $closed_per_page_max;
        if ($end > $num_closed) { $end = $num_closed; }

        // paging options form
        if ($num_closed > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");
            echo("<tr>");
            echo("<td>");
            echo($num_closed . " job" . (($num_closed != 1) ? "s" : "") . " on " . $closed_pages . " page" . (($closed_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='closed_per_page_max' size='4' maxlength='4' value='" . $closed_per_page_max . "' />");
            echo("<input type='hidden' name='order' value='".$order."' />");
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

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
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

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$closed_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $closed_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$closed_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $closed_end = $row_status2['status_name'];
                    }
                }
                
                if (strlen($closed_start) > 15) { $closed_start = substr($closed_start, 0, 13) . "..."; }
                if (strlen($closed_end) > 15) { $closed_end = substr($closed_end, 0, 13) . "..."; }

                echo("<option value='" . $closed_row_num_start . "' ");
                if ($closed_start_row == $closed_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $closed_start . " to " . $closed_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }

        //$href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs")."&amp;order=";
        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";

        echo("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>  
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
                </tr>
                ");

        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];
            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }
            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $company_name = $row["company_name"];
            $position_title = $row["position_title"];
            $division_name = $row["department_name"];
            $department_id = $row["department_id"];

            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");
            $rowclass++;
        }
        echo("</table>");
        echo("</td></tr></table>");
    } 

    echo("<h4>Cancelled Jobs:</h4>");

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.job_code, ji.job_id, ji.position_title, ji.admin_status, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.department_id, ed.department_name, ji.admin_status";
                break;
            default:
            case 'job_code':
                $sql .= "ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ji.department_id, ed.department_name, ec.company_name";
                break;
    }

	$sql .= ("
		FROM job_info AS ji
        LEFT JOIN employer_company AS ec
          ON ji.employer_id = ec.employer_id
        LEFT JOIN employer_department AS ed
          ON ji.employer_department_id = ed.department_id
		WHERE ji.status=" . CANCELLED . "
		AND ji.department_id IN (" . $department_ids . ")
		AND ji.term_id='" . $nextTermYear["term"] . "'
		AND ji.year='" . $nextTermYear["year"] . "'
		AND ec.employer_id=ji.employer_id
		");

    $sql .= ("
            ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
            : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
            : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
            : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
            : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
            );

    $num_cancelled_result = $GLOBALS['dbh']->Execute($sql);
    $num_cancelled = $num_cancelled_result->RecordCount();

    if ($num_cancelled > $paging_overflow)
    {
        $sql .= ("
                LIMIT " . $cancelled_start_row . ", " . $cancelled_per_page_max
                );
    }
    $result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0)
	{
        $cancelled_pages = ceil($num_cancelled / $cancelled_per_page_max);

        $first = $cancelled_start_row + 1;
        $end = $cancelled_start_row + $cancelled_per_page_max;

        // paging options form
        if ($num_cancelled > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

            echo("<tr>");
            echo("<td>");
            echo($num_cancelled . " job" . (($num_cancelled != 1) ? "s" : "") . " on " . $cancelled_pages . " page" . (($cancelled_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='cancelled_per_page_max' size='4' maxlength='4' value='" . $cancelled_per_page_max . "' />");
            echo("<input type='hidden' name='order' value='".$order."' />");
            echo(" jobs per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='cancelled_start_row'>");

            for ($i = 0; $i < $cancelled_pages; $i++)
            {
                $cancelled_row_num_start = $i * $cancelled_per_page_max;
                $cancelled_row_num_end = $cancelled_row_num_start + $cancelled_per_page_max - 1;
                if ($cancelled_row_num_end >= $num_cancelled) $cancelled_row_num_end = $num_cancelled - 1;

                $num_cancelled_result->Move($cancelled_row_num_start);
                $num_cancelled_result_row = $num_cancelled_result->FetchRow();
                $cancelled_start = $num_cancelled_result_row[0];

                $num_cancelled_result->Move($cancelled_row_num_end);
                $num_cancelled_result_row = $num_cancelled_result->FetchRow();
                $cancelled_end = $num_cancelled_result_row[0];

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
                {
                    if ($cancelled_start == '')
                    {
                        $cancelled_start = 'Not Yet Set';
                    }
                    if ($cancelled_end == '')
                    {
                        $cancelled_end = 'Not Yet Set';
                    }
                }

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$cancelled_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $cancelled_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$cancelled_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $cancelled_end = $row_status2['status_name'];
                    }
                }
                
                if (strlen($cancelled_start) > 15) { $cancelled_start = substr($cancelled_start, 0, 13) . "..."; }
                if (strlen($cancelled_end) > 15) { $cancelled_end = substr($cancelled_end, 0, 13) . "..."; }

                echo("<option value='" . $cancelled_row_num_start . "' ");
                if ($cancelled_start_row == $cancelled_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $cancelled_start . " to " . $cancelled_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }
        
        //$href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs")."&amp;order=";
        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";

        echo("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>  
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
                </tr>
                ");

        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];
            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }
            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $company_name = $row["company_name"];
            $position_title = $row["position_title"];
            $division_name = $row["department_name"];
            $department_id = $row["department_id"];

            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);
            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");
            $rowclass++;
        }
        echo("</table>");
        echo("</td></tr></table>");
    } 
}

// If they would like to see future jobs, then display this for them.
if ($show_future == 'show_future_jobs')
{
    echo("<h4>Future Jobs:</h4>");

    $sql = ("
            SELECT DISTINCT start_date, end_date
            FROM term
            WHERE term_id='" . $nextTermYear["term"] . "'
            ");

    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $start_end = $row;

    $sql = "SELECT DISTINCT ";
    switch ($order)
    {
            case 'closing_date':
                $sql .= "ji.closing_date, ji.job_code, ji.job_id, ji.position_title, ji.admin_status, ji.term_id, ji.year, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'admin_status':
                $sql .= "ji.admin_status, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.term_id, ji.year, ji.department_id, ed.department_name, ec.company_name";
                break;
            case 'company_name':
                $sql .= "ec.company_name, ji.job_code, ji.closing_date, ji.job_id, ji.position_title, ji.term_id, ji.year, ji.department_id, ed.department_name, ji.admin_status";
                break;
            default:
            case 'job_code':
                $sql .= "ji.job_code, ji.closing_date, ji.job_id, ji.admin_status, ji.position_title, ji.term_id, ji.year, ji.department_id, ed.department_name, ec.company_name";
                break;
    }

    $sql .= ("
            FROM job_info AS ji, term AS b
            LEFT JOIN employer_company AS ec
              ON ji.employer_id = ec.employer_id
            LEFT JOIN employer_department AS ed
              ON ji.employer_department_id = ed.department_id
            WHERE ji.status!=" . INT_ONLY . "
            AND ec.employer_id=ji.employer_id
            AND ji.department_id IN (" . $department_ids . ")
            AND ( (ji.term_id=b.term_id AND b.start_date>'" . $start_end["end_date"] . "' AND ji.year='" . $nextTermYear["year"] . "')
                OR ji.year>'" . $nextTermYear["year"] . "' )
            ");

    $sql .= $extra_sql;

    $sql .= ("
            ORDER BY " .(($order == 'closing_date') ? "ji.closing_date, ji.job_code, ec.company_name, ji.position_title, ji.admin_status"
            : (($order == 'admin_status') ? "ji.admin_status, ji.job_code, ec.company_name, ji.position_title"
            : (($order == 'company_name') ? "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"
            : (($order == 'job_code') ? "ji.job_code, ec.company_name, ji.closing_date, ji.position_title, ji.admin_status"
            : "ec.company_name, ji.job_code, ji.closing_date, ji.position_title, ji.admin_status"))))
            );

    $num_future_result = $GLOBALS['dbh']->Execute($sql);
    $num_future = $num_future_result->RecordCount();

    if ($num_future > $paging_overflow)
    {
        $sql .= ("
                LIMIT " . $future_start_row . ", " . $future_per_page_max
                );
    }
    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() == 0)
    {
        // if there are no job in the database, indicate this

        notify("There are no future jobs registered in the system.");
    } else
    {
        $future_pages = ceil($num_future / $future_per_page_max);

        $first = $future_start_row + 1;
        $end = $future_start_row + $future_per_page_max;

        // paging options form
        if ($num_future > $paging_overflow)
        {
            echo("<table border='0' class='row1' cellpadding='4' cellspacing='0'>");

            echo("<tr>");
            echo("<td>");
            echo($num_future . " job" . (($num_future != 1) ? "s" : "") . " on " . $future_pages . " page" . (($future_pages > 1) ? "s" : "").";</td>");
            echo("<td><input type='text' name='future_per_page_max' size='4' maxlength='4' value='" . $future_per_page_max . "' />");
            echo("<input type='hidden' name='order' value='".$order."' />");
            echo(" jobs per page.");
            echo("</td>");
            echo("</tr>");

            echo("<tr>");

            echo("<td>");
            echo("<select name='future_start_row'>");

            for ($i = 0; $i < $future_pages; $i++)
            {
                $future_row_num_start = $i * $future_per_page_max;
                $future_row_num_end = $future_row_num_start + $future_per_page_max - 1;
                if ($future_row_num_end >= $num_future) $future_row_num_end = $num_future - 1;

                $num_future_result->Move($future_row_num_start);
                $num_future_result_row = $num_future_result->FetchRow();
                $future_start = $num_future_result_row[0];

                $num_future_result->Move($future_row_num_end);
                $num_future_result_row = $num_future_result->FetchRow();
                $future_end = $num_future_result_row[0];

                if ($order == 'job_code' OR $order == 'closing_date' OR !$order) 
                {
                    if ($future_start == '')
                    {
                        $future_start = 'Not Yet Set';
                    }
                    if ($future_end == '')
                    {
                        $future_end = 'Not Yet Set';
                    }
                }

                if ($order == 'admin_status')
                {
                    $sql_status = "SELECT status_name FROM admin_status WHERE status_id = ".$future_start;
                    $result_status=$GLOBALS['dbh']->Execute($sql_status);
                    while ($row_status = $result_status->FetchRow()) {
                        $future_start = $row_status['status_name'];
                    }
                    $sql_status2 = "SELECT status_name FROM admin_status WHERE status_id = ".$future_end;
                    $result_status2 = $GLOBALS['dbh']->Execute($sql_status2);
                    while ($row_status2 = $result_status2->FetchRow()) {
                        $future_end = $row_status2['status_name'];
                    }
                }
                
                if (strlen($future_start) > 15) { $future_start = substr($future_start, 0, 13) . "..."; }
                if (strlen($future_end) > 15) { $future_end = substr($future_end, 0, 13) . "..."; }

                echo("<option value='" . $future_row_num_start . "' ");
                if ($future_start_row == $future_row_num_start)
                {
                    echo("selected='selected'");
                }
                echo(">" . $future_start . " to " . $future_end . "</option>\n");
            }

            echo("</select>");
            echo("</td>");

            echo("<td>");
            echo("<input type='submit' name='continue' value='View' />");
            echo("</td>");

            echo("</tr>");
            echo("</table>");
        }
        // there are jobs in the system

        //$href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs")."&amp;order=";
        $href = $PHP_SELF . "&amp;select=edit_job".(($show_future == 'show_future_jobs') ? "&amp;show_future=show_future_jobs" : "&amp;show_future=hide_future_jobs");
        $href .= "&amp;unchecked_per_page_max=" . $unchecked_per_page_max;
        $href .= "&amp;checked_per_page_max=" . $checked_per_page_max;
        $href .= "&amp;open_per_page_max=" . $open_per_page_max;
        $href .= "&amp;closed_per_page_max=" . $closed_per_page_max;
        $href .= "&amp;cancelled_per_page_max=" . $cancelled_per_page_max;
        $href .= "&amp;future_per_page_max=" . $future_per_page_max;
        $href .= "&amp;order=";

        echo("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."company_name'><b class='white'>Company Name</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."job_code'><b class='white'>Job Code</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Position Title</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Departments</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Disciplines</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<b class='white'>Term</b>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='".$href."admin_status'><b class='white'>Status</b></a>&nbsp;</td>
                </tr>
                ");

        $rowclass = 0;
        while ($row = $result->FetchRow())
        {
            $position_title = $row["position_title"];
            if (validDate($row["closing_date"]) != -1)
            {
                $closing_date = $row["closing_date"];
            }
            else
            {
                $closing_date = "Not yet set";
            }
            if ($row["job_code"])
            {
                $job_code = $row["job_code"];
            }
            else
            {
                $job_code = "Not yet set";
            }
            $job_id = $row["job_id"];
            $term_id = $row["term_id"];
            $year = $row["year"];
            $company_name = $row["company_name"];

            $href = $PHP_SELF . "&amp;select=edit_job&amp;job_id=" . urlencode($job_id);

            echo("<tr>");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$company_name</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$job_code</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");

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
            echo(getTermName($term_id) . ", " . $year . "</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");

            echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d") . "'>");
            $admin_status_gif = getAdminStatusGif($job_id);
            echo($admin_status_gif);
            echo("</td>\n");

            echo("</tr>\n");
            $rowclass++;
        }

        echo("</table>");
        echo("</td></tr></table>");
    }
}

// Status icon key

echo("<h4>Status Icon Key:</h4>");

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
