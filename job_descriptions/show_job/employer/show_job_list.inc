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
 | Description: This file loads up all of the employer's company's jobs, and    |
 | allows them to select one of them to edit.                                   |
 +------------------------------------------------------------------------------+

*/

echo("<tr>");
echo("<td>");
echo("<h3 align='left'>View Jobs</h3>");
echo("</td>");
echo("</tr>");

// Show open jobs

echo("<tr>");
echo("<td>");
echo("<h4>Open Jobs:</h4>");
echo("</td>");
echo("</tr>");

if ($userlevel == ADMINEMPLOYER || $userlevel == EMPLOYER) 
{
    $sql = "SELECT DISTINCT ";

    // used later on (if needed) for the drop down of different pages
    switch ($order) 
    {
        case 'status':
            $sql .= "js.status_name, ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as term_year,
            ji.closing_date, ji.position_title, ji.job_code, ";
            break;

        case 'employer_code':
            $sql .= "ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
            ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'closing_date':
            $sql .= "ji.closing_date, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.employer_code, 
                ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'coop_code':
            $sql .= "ji.job_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date, 
                ji.employer_code, ji.position_title, js.status_name, ";
            break;

        case 'term_year':
            $sql .= "CONCAT(t.term_name, ', ', ji.year) as term_year,
            ji.closing_date, ji.employer_code, ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'position_title':
        default:
            $order = 'position_title';
            $sql .= "ji.position_title, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
            ji.employer_code, ji.job_code, js.status_name, ";
    } //Switch

    $sql .= ("ji.job_id
        FROM job_info AS ji
        INNER JOIN term AS t
        ON ji.term_id = t.term_id
        INNER JOIN job_status AS js
        ON ji.status = js.status_id
        LEFT JOIN contact_employer AS ce
        ON ji.employer_department_id = ce.department_id
        WHERE ji.employer_id = '" . $employer_id . "'
        AND (ce.contact_id = '" . (($userlevel == EMPLOYER) ? $contact_id : $employer_contact_id_login) . "' 
            OR ji.employer_department_id = '0' 
            OR ji.employer_department_id IS NULL)
        AND ji.position_title != ''
        AND ji.position_title IS NOT NULL
        AND ji.admin_status IN ('".NOT_CHECKED_EMPLOYER."','".NOT_CHECKED_STAFF."','".CHECKED_CODED."','".CHECKED_WAITING_DATE."','".CHECKED_WAITING_CONFIRM."'
            ,'".POSTED_VIEWABLE."','".ADMIN_STATUS_CLOSED."','".INTERVIEWING."','".WAIT_RANK."','".RANK_RECV."','".HIRING_HOLD."','".HOLDING_ADMIN."')
        ");

    $sql .= " ORDER BY ";
    switch ($order) 
    {
        case 'status':
            $sql .= "js.status_name, ji.year, ji.term_id, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'employer_code':
            $sql .= "ji.employer_code, ji.year, ji.term_id, ji.closing_date, ji.position_title ";
            break;

        case 'closing_date':
            $sql .= "ji.closing_date, ji.year, ji.term_id, ji.employer_code, ji.position_title ";
            break;

        case 'coop_code':
            $sql .= "ji.job_code, ji.year, ji.term_id, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'term_year':
            $sql .= "ji.year, t.year_order, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'position_title':
        default:
            $sql .= "ji.position_title, ji.employer_code, ji.year, ji.term_id, ji.closing_date ";
    } //switch
}
else 
{
    include ('misc/loguser.inc');
}

$result = $GLOBALS['dbh']->Execute($sql);
$num_jobs = $result->RecordCount();

if ($num_jobs == 0) 
{
    // if there are no open jobs in the database, indicate this 
    echo("<tr>");
    echo("<td>");
    $msg = ("You do not have any open jobs in our system. To view jobs from past terms, select the appropriate term/year from the \"past jobs\" search box below.");
    notify($msg);
    echo("</td>");
    echo("</tr>");
} 
else 
{
    // there are jobs in the system

    $open_jobs_per_page = intval($open_jobs_per_page);

    if ($open_jobs_per_page == "") { $open_jobs_per_page = 20; }
    if ($open_jobs_per_page < 5) { $open_jobs_per_page = 5; }
    if ($open_job_page == '') { $open_job_page = 0; }

    // Display paging box for open jobs.
    if ($num_jobs > 20) {

        echo("<tr>");
        echo("<td>");
        echo("&nbsp;");
        echo("</td>");
        echo("</tr>");

        echo("<tr>");
        echo("<td>");

        $num_pages = ceil($num_jobs / $open_jobs_per_page);

        $open_job_pos_start = $open_job_page * $open_jobs_per_page;
        $open_job_pos_end = $open_job_pos_start + $open_jobs_per_page;

        $sql .= "LIMIT $open_job_pos_start, $open_jobs_per_page";

        echo ("<form method='post' action='$PHP_SELF'>");
        echo ("<input type='hidden' name='order' value='$order' />");
        // Hidden fields from other forms to preserve their values. 
        echo ("<input type='hidden' name='show_term' value='$show_term' />");
        echo ("<input type='hidden' name='show_year' value='$show_year' />");
        echo ("<input type='hidden' name='past_job_page' value='$past_job_page' />");
        echo ("<input type='hidden' name='past_jobs_per_page' value='$past_jobs_per_page' />");
        // end
        echo ("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
        echo ("<tr>");
        echo ("<td valign='bottom'>");
        echo ($num_jobs . " job" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "") .";");
        echo ("</td>");

        echo ("<td valign='bottom'>");
        echo ("Show <input type='text' name='open_jobs_per_page' size='3' value='$open_jobs_per_page' /> jobs per page");
        echo ("</td>");
        echo ("</tr>");

        echo ("<tr>");
        echo ("<td>");
        echo ("<select name='open_job_page'>");

        for ($j = 0; $j < $num_pages; $j ++) {

            $from_pos = $j * $open_jobs_per_page;
            $to_pos = $from_pos + $open_jobs_per_page - 1;
            if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

            $result->Move($from_pos);
            $row = $result->FetchRow();
            $from = $row[0];

            $result->Move($to_pos);
            $row = $result->FetchRow();
            $to = $row[0];

            // special cases to concatenate the strings
            if ($order == 'term_year') {
                $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
            }

            else {
                if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
            }

            echo ("<option value='$j'". (($j == $open_job_page) ? " selected='selected'" : "") .">");
            echo ("$from  to  ". (($to == '') ? "n/a" : $to));
            echo ("</option>");
        }
        echo ("</select>");
        echo ("</td>");

        echo ("<td align='right'>");
        echo ("<input type='submit' value='View' />");
        echo ("</td>");
        echo ("</tr>");
        echo ("</table>");
        echo ("</form>");
        echo("</td>");
        echo("</tr>");
    }

    $jobs_array = array();

    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow()) {

        $position_title = $row["position_title"];
        $closing_date = (($row["closing_date"] == "0000-00-00") ? "Not Yet Set" : $row["closing_date"]);
        $coop_code = (($row["job_code"] == '') ? "Not Yet Set" : $row["job_code"]);
        $employer_code = (($row["employer_code"] == '') ? "n/a" : $row["employer_code"]);
        $job_id = $row["job_id"];
        $term_year = $row["term_year"];
        $status = $row["status_name"];

        if (($employer_code != 'n/a') && (!$has_employer_code)) {
            $has_employer_code = true;
        }

        $jobs_array[] = array (
                "position_title" => $position_title,
                "closing_date" => $closing_date,
                "coop_code" => $coop_code,
                "employer_code" => $employer_code,
                "job_id" => $job_id,
                "term_year" => $term_year,
                "status_name" => $status
                );
    }

    $href = $PHP_SELF ."&amp;past_jobs_per_page=$past_jobs_per_page&amp;past_job_page=$past_job_page&amp;open_jobs_per_page=$open_jobs_per_page&amp;open_job_page=$open_job_page";
    $href .= "&amp;show_term=$show_term&amp;show_year=$show_year&amp;order=";

echo("<tr><td>");
    echo ("
            <table cellspacing='0' cellpadding='0' border='1'><tr><td>
            <table border='0' cellpadding='2'>
            <tr>");
    echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."position_title'><b class='white'>Position Title</b></a>&nbsp;</td>");
    if ($has_employer_code) {
        echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."employer_code'><b class='white'>Your Job Code</b></a>&nbsp;</td>");
    }
    echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."coop_code'><b class='white'>Co-op Job Code</b></a>&nbsp;</td>");
    echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."term_year'><b class='white'>Job Term/Year</b></a>&nbsp;</td>");

    echo ("
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
            <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."status'><b class='white'>Job Status</b></a>&nbsp;</td>
            </tr>
            ");

    for ($rowclass = 0; $rowclass < count($jobs_array); $rowclass++)
    {
        $position_title = $jobs_array[$rowclass]["position_title"];
        $closing_date = $jobs_array[$rowclass]["closing_date"];
        $coop_code = $jobs_array[$rowclass]["coop_code"];
        $employer_code = $jobs_array[$rowclass]["employer_code"];
        $job_id = $jobs_array[$rowclass]["job_id"];
        $term_year = $jobs_array[$rowclass]["term_year"];
        $status = $jobs_array[$rowclass]["status_name"];

        $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);

        echo ("<tr>");
        echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
        echo ("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");
        if ($has_employer_code) {
            echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo ("&nbsp;<a class='blue' href=\"$href\">$employer_code</a>&nbsp;</td>\n");
        }
        echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;<a class='blue' href=\"$href\">$coop_code</a>&nbsp;</td>\n");
        echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;<a class='blue' href=\"$href\">$term_year</a>&nbsp;</td>\n");
        echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");
        echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
        echo ("&nbsp;<a class='blue' href=\"$href\">$status</a>&nbsp;</td>\n");

        echo ("</tr>\n");
    }
    echo ("</table>");
    echo ("</td></tr></table></td></tr>");
} // end else 

// Showing open jobs complete, now begin showing past jobs. 

echo("<tr>");
echo("<td>");
echo("<h4>Past Jobs:</h4>");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td>");
instruct("Select a term/year for which you wish to view past jobs.");
echo("<br />");
echo("</td>");
echo("</tr>");

echo("<tr>");
echo("<td>");
echo("<table border='0' cellpadding='2' cellspacing='0'>");

echo("<tr>");
echo("<td class='row1'>");
echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>"); 

echo("<tr align='center'>");
echo("<td>");

$show_jobs = urldecode($show_jobs);

echo ("<form method='post' action='" . $PHP_SELF . "&amp;select=view_job'>");
echo ("<input type='hidden' name='level1' value='" . $level1 . "' />");
// Hidden variables to preserve variables from other paging forms. 
echo ("<input type='hidden' name='open_job_page' value='$open_job_page' />");
echo ("<input type='hidden' name='open_jobs_per_page' value='$open_jobs_per_page' />");
echo ("<input type='hidden' name='order' value='$order' />");

// Get the term and year that we are showing jobs for.

$sql = ("
    SELECT DISTINCT term_id, term_name, end_date, start_date
    FROM term
    ORDER BY start_date
    ");
$term_result = $GLOBALS['dbh']->Execute($sql);

$sql = ("
    SELECT MAX(year) as end_year, MIN(year) as start_year
    FROM job_info
    WHERE employer_id = '".addslashes($employer_id)."' AND year != 0
    ");

$year_result = $GLOBALS['dbh']->Execute($sql);
$year_row = $year_result->FetchRow();

$start_year = $year_row["start_year"];

echo("<select name='show_term'>");
echo("<option value='ALL' ");
if ($show_term == 'ALL')
{
    echo("selected='selected'");
}
echo(">ALL</option>");
while ($term_row = $term_result->FetchRow())
{
    echo("<option value='" . $term_row["term_id"] . "' ");
    if ($show_term == $term_row["term_id"] || (!$show_term && $term_row["term_id"] == $nextTermYear["term"]))
    {
        echo("selected='selected'");
    }
    echo(">" .getMonth($term_row["start_date"]). "</option>");
}
echo("</select>");
echo("&nbsp;");
echo("<select name='show_year'>");
echo("<option value='ALL' ");
if ($show_year == 'ALL')
{
    echo("selected='selected'");
}
echo(">ALL</option>");
for ($i = (int)$year_row["end_year"]; $i >= (int)$start_year; $i--)
{
    echo("<option value='" . $i . "' ");
    if ($show_year == $i || (!$show_year && $i == $nextTermYear["year"]))
    {
        echo("selected='selected'");
    }
    echo(">" . $i . "</option>");
}
echo("</select>");
echo("&nbsp;");
echo("<input type='submit' value='Go' />");
echo("</form>");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

echo("</table>");
echo("</td>");
echo("</tr>");

$nextTermYear = getNextTermYear();

if (!$show_term)
{
    $show_term = $nextTermYear["term"];
}
if (!$show_year)
{
    $show_year = $nextTermYear["year"];
}

if ($userlevel == ADMINEMPLOYER || $userlevel == EMPLOYER) 
{
    $sql = "SELECT DISTINCT ";

    // used later on (if needed) for the drop down of different pages
    switch ($order) 
    {
        case 'status':
            $sql .= "js.status_name, ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as term_year,
            ji.closing_date, ji.position_title, ji.job_code, ";
            break;

        case 'employer_code':
            $sql .= "ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
            ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'closing_date':
            $sql .= "ji.closing_date, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.employer_code, 
                ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'coop_code':
            $sql .= "ji.job_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date, 
                ji.employer_code, ji.position_title, js.status_name, ";
            break;

        case 'term_year':
            $sql .= "CONCAT(t.term_name, ', ', ji.year) as term_year,
            ji.closing_date, ji.employer_code, ji.position_title, ji.job_code, js.status_name, ";
            break;

        case 'position_title':
        default:
            $order = 'position_title';
            $sql .= "ji.position_title, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
            ji.employer_code, ji.job_code, js.status_name, ";
    } //Switch

    $sql .= "ji.job_id
        FROM job_info AS ji, term AS t, job_status as js
        LEFT JOIN contact_employer AS ce
        ON ji.employer_department_id = ce.department_id
        WHERE ji.employer_id = '" . $employer_id . "'
        AND ji.status = js.status_id
        AND ji.term_id = t.term_id
        AND (ce.contact_id = '" . (($userlevel == EMPLOYER) ? $contact_id : $employer_contact_id_login) . "' 
            OR ji.employer_department_id = '0' 
            OR ji.employer_department_id IS NULL)
        AND ji.position_title != ''
        AND ji.position_title IS NOT NULL
        AND ji.admin_status IN ('".REJECTED_JOB."','".REJECTED_EMPLOYER."','".REJECTED_NOT_REAL_EMPLOYER."','".REJECTED_OTHER."','".CANCELLED_DELETED."'
            ,'".NOT_INT_SUIT_APPS."','".NOT_INT_FUNDING."','".NOT_INT_OTHER."','".FILLED_UVIC."','".FILLED_ELSEWHERE."','".UNFILLED_SUIT_APPS."'
            ,'".UNFILLED_NOT_HIRING."')";

    if ($show_term != 'ALL')
    {
        $sql .= " AND ji.term_id = '".addslashes($show_term)."' ";
    }
    if ($show_year != 'ALL')
    {
        $sql .= " AND ji.year = '".addslashes($show_year)."' ";
    }

    $sql .= " ORDER BY ";
    switch ($order) 
    {
        case 'status':
            $sql .= "js.status_name, ji.year, ji.term_id, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'employer_code':
            $sql .= "ji.employer_code, ji.year, ji.term_id, ji.closing_date, ji.position_title ";
            break;

        case 'closing_date':
            $sql .= "ji.closing_date, ji.year, ji.term_id, ji.employer_code, ji.position_title ";
            break;

        case 'coop_code':
            $sql .= "ji.job_code, ji.year, ji.term_id, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'term_year':
            $sql .= "ji.year, t.year_order, ji.closing_date, ji.employer_code, ji.position_title ";
            break;

        case 'position_title':
        default:
            $sql .= "ji.position_title, ji.employer_code, ji.year, ji.term_id, ji.closing_date ";
    } //switch
} // end if
else 
{
    include ('misc/loguser.inc');
}

$result = $GLOBALS['dbh']->Execute($sql);
$num_jobs = $result->RecordCount();

if ($num_jobs == 0) 
{
    // if there are no jobs in the database, indicate this 
    $msg = ("You do not have any jobs in the system");
    if ($show_term != 'ALL' && $show_year != 'ALL')
    {
        $msg .= (" for the selected term and year.");
    }
    elseif ($show_term != 'ALL')
    {
        $msg .= (" for the selected term.");
    }
    elseif ($show_year != 'ALL')
    {
        $msg .= (" for the selected year.");
    }
    else
    {
        $msg .= (".");
    }

    echo("<tr>\n");
    echo("<td>\n");
    notify($msg);
    echo("</td>\n");
    echo("</tr>\n");
} 
else 
{
    // there are jobs in the system

    $past_jobs_per_page = intval($past_jobs_per_page);

    if ($past_jobs_per_page == "") { $past_jobs_per_page = 20; }
    if ($past_jobs_per_page < 5) { $past_jobs_per_page = 5; }
    if ($past_job_page == '') { $past_job_page = 0; }


    // display the thingy if necessary
    if ($num_jobs > 20) {

        echo("<tr>\n");
        echo("<td>\n");
        echo("&nbsp;");
        echo("</td>\n");
        echo("</tr>\n");

        echo("<tr>\n");
        echo("<td>");

        $num_pages = ceil($num_jobs / $past_jobs_per_page);

        $past_job_pos_start = $past_job_page * $past_jobs_per_page;
        $past_job_pos_end = $past_job_pos_start + $past_jobs_per_page;

        $sql .= "LIMIT $past_job_pos_start, $past_jobs_per_page";

        echo ("<form method='post' action='$PHP_SELF'>");
        echo ("<input type='hidden' name='order' value='$order' />");
        // Variables to preserve other form's values. 
        echo ("<input type='hidden' name='show_term' value='$show_term' />");
        echo ("<input type='hidden' name='show_year' value='$show_year' />");
        echo ("<input type='hidden' name='open_jobs_per_page' value='$open_jobs_per_page' />");
        echo ("<input type='hidden' name='open_job_page' value='$open_job_page' />");

        echo ("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
        echo ("<tr>");
        echo ("<td valign='bottom'>");
        echo ($num_jobs . " job" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "") .";");
        echo ("</td>");

        echo ("<td valign='bottom'>");
        echo ("Show <input type='text' name='past_jobs_per_page' size='3' value='$past_jobs_per_page' /> jobs per page");
        echo ("</td>");
        echo ("</tr>");

        echo ("<tr>");
        echo ("<td>");
        echo ("<select name='past_job_page'>");

        for ($j = 0; $j < $num_pages; $j ++) {

            $from_pos = $j * $past_jobs_per_page;
            $to_pos = $from_pos + $past_jobs_per_page - 1;
            if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

            $result->Move($from_pos);
            $row = $result->FetchRow();
            $from = $row[0];

            $result->Move($to_pos);
            $row = $result->FetchRow();
            $to = $row[0];

            // special cases to concatenate the strings
            if ($order == 'term_year') {
                $from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
                $to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
            }

            else {
                if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
            }

            echo ("<option value='$j'". (($j == $past_job_page) ? " selected='selected'" : "") .">");
            echo ("$from  to  ". (($to == '') ? "n/a" : $to));
            echo ("</option>");
        }
        echo ("</select>");
        echo ("</td>");

        echo ("<td align='right'>");
        echo ("<input type='submit' value='View' />");
        echo ("</td>");
        echo ("</tr>");
        echo ("</table>");
        echo ("</form>");
    }

    $result = $GLOBALS['dbh']->Execute($sql);

    if ($result->RecordCount() != 0) {

        $jobs_array = array();

        while ($row = $result->FetchRow()) {

            $position_title = $row["position_title"];
            $closing_date = (($row["closing_date"] == "0000-00-00") ? "Not Yet Set" : $row["closing_date"]);
            $coop_code = (($row["job_code"] == '') ? "Not Yet Set" : $row["job_code"]);
            $employer_code = (($row["employer_code"] == '') ? "n/a" : $row["employer_code"]);
            $job_id = $row["job_id"];
            $term_year = $row["term_year"];
            $status = $row["status_name"];

            if (($employer_code != 'n/a') && (!$has_employer_code)) {
                $has_employer_code = true;
            }

            $jobs_array[] = array (
                    "position_title" => $position_title,
                    "closing_date" => $closing_date,
                    "coop_code" => $coop_code,
                    "employer_code" => $employer_code,
                    "job_id" => $job_id,
                    "term_year" => $term_year,
                    "status_name" => $status
                    );
        }

        $href = $PHP_SELF ."&amp;past_jobs_per_page=$past_jobs_per_page&amp;past_job_page=$past_job_page&amp;open_jobs_per_page=$open_jobs_per_page&amp;open_job_page=$open_job_page";
        $href .= "&amp;show_term=$show_term&amp;show_year=$show_year&amp;order=";

        echo ("<tr>\n");
        echo ("<td>\n");
        echo ("
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>");
        echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."position_title'><b class='white'>Position Title</b></a>&nbsp;</td>");
        if ($has_employer_code) {
            echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."employer_code'><b class='white'>Your Job Code</b></a>&nbsp;</td>");
        }
        echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."coop_code'><b class='white'>Co-op Job Code</b></a>&nbsp;</td>");

        if (($show_term == 'ALL') || ($show_year == 'ALL')) {
            echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."term_year'><b class='white'>Job Term/Year</b></a>&nbsp;</td>");
        }

        echo ("
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."status'><b class='white'>Job Status</b></a>&nbsp;</td>
                </tr>
                ");

        for ($rowclass = 0; $rowclass < count($jobs_array); $rowclass ++)
        {
            $position_title = $jobs_array[$rowclass]["position_title"];
            $closing_date = $jobs_array[$rowclass]["closing_date"];
            $coop_code = $jobs_array[$rowclass]["coop_code"];
            $employer_code = $jobs_array[$rowclass]["employer_code"];
            $job_id = $jobs_array[$rowclass]["job_id"];
            $term_year = $jobs_array[$rowclass]["term_year"];
            $status = $jobs_array[$rowclass]["status_name"];

            $href = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . urlencode($job_id);

            echo ("<tr>");
            echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
            echo ("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");
            if ($has_employer_code) {
                echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
                echo ("&nbsp;<a class='blue' href=\"$href\">$employer_code</a>&nbsp;</td>\n");
            }
            echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo ("&nbsp;<a class='blue' href=\"$href\">$coop_code</a>&nbsp;</td>\n");
            if (($show_term == 'ALL') || ($show_year == 'ALL')) {
                echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
                echo ("&nbsp;<a class='blue' href=\"$href\">$term_year</a>&nbsp;</td>\n");
            }
            echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo ("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");
            echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
            echo ("&nbsp;<a class='blue' href=\"$href\">$status</a>&nbsp;</td>\n");

            echo ("</tr>\n");
        }
        echo ("</table>\n");
        echo ("</td>\n");
        echo ("</tr>\n");
        echo ("</table>\n");
        echo ("</td>");
        echo ("</tr>");

    }  // end if

} // end else

?>
