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
 | Filename: view_apps_list.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of all the applications in the system for the   |
 | student.                                                                     |
 +------------------------------------------------------------------------------+

 //:TODO: Fix up this file's pagination. It's inefficient.

*/

echo ("<h3>Job Applications - View</h3>");
?>

<script type='text/javascript' language='javascript'>
<!--javascript

function load_order(order)
{
        document.form_order.order_by.value=order;
            document.form_order.submit();
}
//-->
</script>

<?php
$nextTermYear = getNextTermYear();
$show_term = ($show_term ? $show_term : $nextTermYear["term"]);
$show_year = ($show_year ? $show_year : $nextTermYear["year"]);

// Now display the actual list of applications based on the criteria chosen above

//first do ONLINE applications
//it'll only return online ones cause we're checking for coverletter id & resume id
// since paper won't have either, it won't return them (plus we're checking to make sure it ain't paper)

if($order_by)
{
    if($order_by == "applied_date")
    {
        $select_sql = "SELECT DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, ec.company_name, a.application_id, 
        a.job_id, j.student_status, h.history_id, aps.app_status_description, a.coverletter_id, a.resume_id, 
        j.job_code, r.name, a.transcript, j.resumes_pulled, j.term_id, j.year, COUNT(app.app_status) AS app_count ";
    }
    elseif($order_by == "term_year")
    {
        $select_sql = "SELECT CONCAT(t.term_name, ', ', j.year) term_year, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, 
        ec.company_name, a.application_id, a.job_id, j.student_status, h.history_id, aps.app_status_description, a.coverletter_id, a.resume_id,
        j.job_code, r.name, a.transcript, j.resumes_pulled, j.term_id, j.year, COUNT(app.app_status) AS app_count ";
    }
    elseif($order_by == "app_count")
    {
        $select_sql = "SELECT COUNT(app.app_status) AS app_count, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, 
        ec.company_name, a.application_id, a.job_id, j.student_status, h.history_id, aps.app_status_description, a.coverletter_id, a.resume_id,
        j.job_code, r.name, a.transcript, j.resumes_pulled, j.term_id, j.year ";
    }
    else
    {
        $select_sql = "SELECT $order_by, ec.company_name, a.application_id, a.job_id, j.student_status, h.history_id, aps.app_status_description,
        a.coverletter_id, a.resume_id, j.job_code, r.name, a.transcript, j.resumes_pulled, j.term_id, j.year,
        DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, COUNT(app.app_status) AS app_count "; 
    }
}
else
{
    $select_sql = "SELECT ec.company_name, a.application_id, a.job_id, j.student_status, h.history_id, aps.app_status_description, 
                a.coverletter_id, a.resume_id, j.job_code, r.name, a.transcript, j.resumes_pulled, j.term_id, j.year,
                DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date, COUNT(app.app_status) AS app_count ";
}
$from_sql = "FROM applications as a, job_info as j, resume as r, term as t
        LEFT JOIN history AS h ON (h.job_id = j.job_id AND h.student_number='" . $student_number . "')
        LEFT JOIN employer_company AS ec ON ec.employer_id = j.employer_id
        LEFT JOIN applications AS app ON j.job_id = app.job_id
        LEFT JOIN application_status AS aps ON a.app_status = aps.app_status_id 
        WHERE a.student_number='$student_number' 
        AND a.job_id=j.job_id 
        AND j.term_id=t.term_id 
        AND a.resume_id=r.resume_id
        AND j.apply_method='".JOB_APP_METHOD_ONLINE."'
        ". ($show_term != 'ALL' ? "AND j.term_id = '$show_term' ": "") ."
        ". ($show_year != 'ALL' ? "AND j.year = '$show_year' ": "") ."
        AND (app.app_status='".APP_APPLIED_ONLINE."' OR app.app_status='".APP_ACTIVATED_ONLINE."' OR app.app_status='".APP_UNAVAILABLE."')
        GROUP BY j.job_id ";
if($order_by)
{
    if(!$change)
    {
        if(($lastorder == $order_by) && ($data_flow == "ASC"))
        {
            $data_flow = "DESC";
        }
        elseif(($lastorder == $order_by) && ($data_flow == "DESC"))
        {
            $data_flow = "ASC";
        }
        elseif(!$data_flow)
        {
            $data_flow = "DESC";
        }
    }
    
    if($order_by == "applied_date")
    {
        $order_by = "a.applied_date";
    }

    if($order_by == "j.job_code")
    {
        $order_sql = "ORDER BY $order_by $data_flow ";
    }
    elseif($order_by == "term_year")
    {
        $order_sql = "ORDER BY j.year $data_flow, j.term_id $data_flow";
    }
    else
    {
        $order_sql = "ORDER BY $order_by $data_flow, j.job_code ";
    }

}
else
{
    $order_sql = "ORDER BY ec.company_name";
    $order_by = "ec.company_name";
    $data_flow = "ASC";
}

$sql = $select_sql." ".$from_sql." ".$order_sql;

//echo ("<form action='$PHP_SELF' method='post' name='app_term_chooser'>");
echo("<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
echo("<tr>");
    echo("<td align='left'>");
    echo ("<form action='$PHP_SELF' method='post' name='app_term_chooser'>");
    echo("<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
    echo("<tr>");
    echo("<td align='left'>");
    echo ("<input type='hidden' name='select' value='applications' />");
    echo ("\n<input type='hidden' name='PDF' value='1' />");
    echo ("\n<input type='hidden' name='change' value='no' />");
    echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
    echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
    echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");
    echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
    echo ("\n<input type='hidden' name='job_page' value='". $job_page ."' />");

    echo ("Show applications for jobs:");

    echo ("<select name='show_term'>");
    echo ("<option value='ALL'". ($show_term == 'ALL' ? " selected='selected'" : "") .">ALL</option>");

    $tsql = "SELECT term_id, term_name
    FROM term
    ORDER BY term_id";
    $result = $GLOBALS['dbh']->Execute ($tsql);
    while ($row = $result->FetchRow()) {

        echo ("<option value='". $row["term_id"] ."'". ($show_term == $row["term_id"] ? " selected='selected'" : "") .">". $row["term_name"] ."</option>");
    }
    echo ("</select> &nbsp;");

    echo ("<select name='show_year'>");
    echo ("<option value='ALL'". ($show_year == 'ALL' ? " selected='selected'" : "") .">ALL</option>");

    $tsql = "SELECT MIN(year) AS min_year
    FROM job_info
    WHERE (status!='0' AND status!='7')
    AND year > 1960";
    $result = $GLOBALS['dbh']->Execute($tsql);
    $row = $result->FetchRow();

    for ($year = ($nextTermYear["year"] + 1); $year >= (int)$row["min_year"]; $year--)
    {
        echo ("<option value='$year'". ($show_year == $year ? " selected='selected'" : "") .">". $year ."</option>");
    }
    echo ("</select>");

    echo ("</td>");
    echo ("<td align='right'>");
    echo ("<input type='submit' value='Go' />");
    echo ("</td>");
//echo ("</tr>");
    //echo ("</td>");
echo ("</tr>");
echo ("</table>");
echo ("</form>");
echo ("</td>");
echo ("</tr>");
echo ("</table>");
$results = $GLOBALS['dbh']->Execute( $sql );
$num_jobs = $results->RecordCount();

if ($num_jobs <= 0) 
{
// Close the above box, since there is nothing more to add.
//echo("</table>");
echo ("<h4>Online Applications:</h4>");
notify("You do not have any applications for the chosen term / year.");
}

else {
if ($jobs_per_page == "") { $jobs_per_page = 20; }
if ($jobs_per_page < 5) { $jobs_per_page = 5; }
if ($job_page == '') { $job_page = 0; }

if ($num_jobs > 20) {

    $num_pages = ceil($num_jobs / $jobs_per_page);

    $job_pos_start = $job_page * $jobs_per_page;
    $job_pos_end = $job_pos_start + $jobs_per_page;

    $sql .= " LIMIT $job_pos_start, $jobs_per_page";

    unset($PDF);
   echo ("\n<form action='$PHP_SELF' method='post' name='app_term_chooser_1'>");
           echo ("\n<table border='0' class='row1' cellspacing='0' cellpadding='4'>");

        echo ("\n<tr><td colspan='2'><hr /></td></tr>");
        echo ("\n<tr>");
        echo ("\n<td valign='bottom' colspan='2'>");
        echo ("\n<input type='hidden' name='select' value='applications' />");
        echo ("\n<input type='hidden' name='PDF' value='1' />");
        echo ("\n<input type='hidden' name='order_by' value='$order_by' />");
        echo ("\n<input type='hidden' name='change' value='no' />");
        echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
        echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");
        echo ("\n<input type='hidden' name='show_term' value='". $show_term ."' />");
        echo ("\n<input type='hidden' name='show_year' value='". $show_year ."' />");

        echo("\n<table border='0' cellpadding='2' cellspacing='0' class='row1'>");
        echo("\n<tr>");
        echo("\n<td align='left'>");
        $page_number = $job_page + 1;
        echo ($num_jobs . " cover letter" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "; "));
        echo ("\n</td>
        <td align='left'>");
        echo ("<input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> cover letters per page");
        echo("</td>");
        echo ("\n</tr>");

        echo ("\n<tr>");
        echo ("\n<td>");
        echo ("\n<select name='job_page'>");

        for ($j = 0; $j < $num_pages; $j ++)
        {

            $from_pos = $j * $jobs_per_page;
            $to_pos = $from_pos + $jobs_per_page - 1;
            if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

            $results->Move($from_pos);
            $from_row = $results->FetchRow();
            $from = $from_row[0];

            $results->Move($to_pos);
            $to_row = $results->FetchRow();
            $to = $to_row[0];

            // special cases to concatenate the strings
            if ($order_by == 'term_year')
            {
                $from = substr($from, 0, 6) . " " . substr($from, -4, 4);
                $to = substr($to, 0, 6) . " " . substr($to, -4, 4);
            }
            if ($order_by == 'app_count')
            {
                $from = getApplicantRange($from);
                $to = getApplicantRange($to);
            }
            else
            {
                if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
                if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
            }

            echo ("\n<option value='$j'". (($j == $job_page) ? " selected='selected'" : "") .">");
            echo ((($from == '') ? "n/a" : $from) ." to  ". (($to == '') ? "n/a" : $to));
            echo ("\n</option>");
        }
        echo ("\n</select>");
        echo ("\n</td>");

        echo ("\n<td align='left'>");
        echo ("\n<input type='submit' value='View' />");
        echo ("\n</td>");
        echo ("\n</tr>");
        echo ("\n</table>");
        echo ("\n</td>");
        echo ("\n</tr>");
        echo ("\n</table>");
        $table_close = 1;
       echo ("\n</form>");

        // Execute new $sql with the limit attached to it
        $results = $GLOBALS['dbh']->Execute( $sql );
    }

    if($table_close == 1)
    {
        unset($table_close);
    }
    else
    {
       // echo ("\n</table>");
    }

    $results->MoveFirst();
    echo ("<h4>Online Applications (".$results->RecordCount()."):</h4>");
   echo ("<form action='$PHP_SELF' method='post' name='online_apps'>");

    echo ("<table cellspacing='0' cellpadding='0' border='1'>");
    echo ("<tr>");
        echo ("<td>");
        echo ("\n<table border='0' cellpadding='2'>");
        echo ("\n<tr>");
            echo ("\n<td class='rowgrey' align='center'>&nbsp;&nbsp;</td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"ec.company_name\")'>
                    <b class='white'>&nbsp;Company&nbsp;</b></a></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"j.job_code\")'>
                    <b class='white'>&nbsp;Job Code&nbsp;</b></a></td>");
            if ($show_term == 'ALL' || $show_year == 'ALL') {

                echo ("\n<td class='rowgrey' align='center'>
                        <a class='orderable' href='javascript:load_order(\"term_year\")'>
                        <b class='white'>&nbsp;Term, Year&nbsp;</b></a></td>");
            }
            echo ("\n<td class='rowgrey' align='center'><b class='white'>
                    &nbsp;Cover Letter&nbsp;</b></td>");
            echo ("\n<td class='rowgrey' align='center'><b class='white'>
                    &nbsp;Resume&nbsp;</b></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"a.transcript\")'>
                    <b class='white'>&nbsp;Transcript&nbsp;</b></a></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"applied_date\")'>
                    <b class='white'>&nbsp;Last Modified&nbsp;</b></a></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"j.admin_status\")'>
                    <b class='white'>&nbsp;Job Status&nbsp;</b></a></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <b class='white'>&nbsp;Your Status&nbsp;</b></td>");
            echo ("\n<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"app_count\")'>
                    <b class='white'>&nbsp;# Students Applied&nbsp;</b></a></td>");
        echo ("\n</tr>");

        $rowclass = 0;

        while ($row = $results->FetchRow()) {

            $rowclass % 2 ? $class = "row0d" : $class = "row1d";

            $jobhref = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"] . "&amp;show_jobs=preferences&amp;show_future=";
            $resumehref = $PHP_SELF . "&amp;select=resume&amp;page=View+HTML&amp;PDF=1&amp;resume_id=" . $row["resume_id"];
            $coverhref = $PHP_SELF . "&amp;select=coverletter&amp;page=View+HTML&amp;PDF=1&amp;coverletter_id=" . $row["coverletter_id"];

            echo ("\n<tr>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;<input name='selected_app_id' type='radio' class='$class' value='" . $row["application_id"] . "' />&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;<a href='$jobhref' class='blue'>" . $row["company_name"] . "</a>&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;<a href='$jobhref' class='blue'>" . $row["job_code"] . "</a>&nbsp;</td>");
                if ($show_term == 'ALL' || $show_year == 'ALL') {
                
                    echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;". getTermName ($row["term_id"]) .", ". $row["year"] ."&nbsp;</td>");
                }
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;<a href='$coverhref' class='blue'>Specific</a>&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;<a href='$resumehref' class='blue'>" . $row["name"] . "</a>&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>" . (($row["transcript"] == "1" ) ? "Yes" : "No") . "&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;" . $row["applied_date"] . "&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;" . getStudentStatusGif($row["job_id"]) . "&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;" . getStudentOwnStatusGif($row["job_id"], $student_number) . "&nbsp;</td>");
                echo ("\n<td class='$class' align='center' valign='middle'>&nbsp;" . getApplicantRange($row["app_count"]) . "&nbsp;</td>");
            echo ("\n</tr>");
            $rowclass++;
        }
       echo ("\n</table>");
  // echo ("</form>");
        echo ("\n</td>");
    echo ("\n</tr>");
    echo ("\n</table>");
    echo ("<br />");
    echo ("<input type='submit' name='view_app' value='View PDF' />");
        echo ("<input type='hidden' name='view_who' value='selected' />");
        echo ("<input type='hidden' name='PDF' value='1' />");
        echo ("<input type='hidden' name='select' value='applications' />");
  echo ("</form>");
} //  there are some records returned

// next comes PAPER applications -----------------------------------------------------------------------------------------
if($order_by)
{
    if($order_by == "applied_date")
    {
        $select_sql = "SELECT DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,  a.application_id as application_id, 
            a.job_id as job_id, aps.app_status_description as status, j.job_code as job_code, j.student_status as job_status, 
        h.history_id, ec.company_name, j.term_id, j.year ";
    }
    elseif($order_by == "term_year" || $order_by == "app_count")
    {
        $select_sql = "SELECT CONCAT(j.term_id, ', ', j.year) term_year, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,  a.application_id as application_id,
        a.job_id as job_id, aps.app_status_description as status, j.job_code as job_code, j.student_status as job_status,
        h.history_id, ec.company_name, j.term_id, j.year ";
    }
    else
    {
        $select_sql = "SELECT $order_by, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,  a.application_id as application_id,
        a.job_id as job_id, aps.app_status_description as status, j.job_code as job_code, j.student_status as job_status,
        h.history_id, ec.company_name, j.term_id, j.year ";
    }
}
else
{
    $select_sql = "SELECT ec.company_name, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,  a.application_id as application_id,
    a.job_id as job_id, aps.app_status_description as status, j.job_code as job_code, j.student_status as job_status,
                    h.history_id, ec.company_name, j.term_id, j.year";
}
$from_sql = " FROM applications as a, job_info as j, application_status as aps
    LEFT JOIN history AS h ON (h.job_id=j.job_id AND h.student_number='" . $student_number . "')
    LEFT JOIN employer_company AS ec ON ec.employer_id = j.employer_id
    WHERE a.student_number='$student_number'
    AND j.job_id=a.job_id
    AND a.app_status = aps.app_status_id
    AND j.apply_method='".JOB_APP_METHOD_PAPER."'
    ". ($show_term != 'ALL' ? "AND j.term_id = '$show_term' ": "") ."
    ". ($show_year != 'ALL' ? "AND j.year = '$show_year' ": "") ."
    ";

// We don't want these applications to be ordered by the number of applicants per job (only for online applications)
if($order_by)
{
    if(($lastorder == $order_by) && ($data_flow == "ASC") && ($order_by != "app_count"))
    {
        $data_flow = "DESC";
    }
    elseif(($lastorder == $order_by) && ($data_flow == "DESC") && ($order_by != "app_count"))
    {
        $data_flow = "ASC";
    }
    elseif(!$data_flow && ($order_by != "app_count"))
    {
        $data_flow = "DESC";
    }
    
    if($order_by == "applied_date")
    {
        $order_by = "a.applied_date";
    }

    if($order_by == "j.job_code")
    {
        $order_sql = "ORDER BY $order_by $data_flow ";
    }
    elseif($order_by == "term_year" || $order_by == "app_count")
    {
        $order_sql = "ORDER BY j.year $data_flow, j.term_id $data_flow";
    }
    else
    {
        $order_sql = "ORDER BY $order_by $data_flow, j.job_code ";
    }

}
else
{
    $order_sql = "ORDER BY ec.company_name";
    $order_by = "ec.company_name";
    $data_flow = "ASC";
}

$sql = $select_sql." ".$from_sql." ".$order_sql;

$result = $GLOBALS['dbh']->Execute( $sql );

if ($result->RecordCount() <= 0) {
    
    echo ("<h4>Paper Applications:</h4>");
    echo ("(Please note: Paper applications will not show up until after the job has closed.)");
    notify("You do not have any applications for the chosen term / year.");
}

else {

    echo ("<h4>Paper Applications (".$result->RecordCount()."):</h4>");
    echo ("(Please note: Paper applications will not show up until after the job has closed.)");

   echo ("<table cellspacing='0' cellpadding='0' border='1'>");
   echo ("<tr>");
        echo ("<td>");

        echo ("<table border='0' cellpadding='2'>");
        echo ("<tr>");
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"ec.company_name\")'>
                    <b class='white'>&nbsp;Company&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"j.job_code\")'>
                    <b class='white'>&nbsp;Job Code&nbsp;</b></a></td>");
            if ($show_term == 'ALL' || $show_year == 'ALL') {

                echo ("<td class='rowgrey' align='center'>
                        <a class='orderable' href='javascript:load_order(\"term_year\")'>
                        <b class='white'>&nbsp;Term, Year&nbsp;</b></a></td>");
            }
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"applied_date\")'>
                    <b class='white'>&nbsp;Last Modified&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"j.status\")'>
                    <b class='white'>&nbsp;Job Status&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'>
                    <b class='white'>&nbsp;Your Status&nbsp;</b></td>");
        echo ("</tr>");

        $rowclass=0;

        while ($row = $result->FetchRow()) {

            $rowclass % 2 ? $class = "row0d" : $class = "row1d";

            $jobhref = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"] . "&amp;show_jobs=preferences&amp;show_future=";

            echo ("<tr>");
                echo ("<td class='$class' align='center'>&nbsp;<a href='$jobhref' class='blue'>" . $row["company_name"] . "</a>&nbsp;</td>");
                echo ("<td class='$class' align='center'>&nbsp;<a href='$jobhref' class='blue'>" . $row["job_code"] . "</a>&nbsp;</td>");
                if ($show_term == 'ALL' || $show_year == 'ALL') {

                    echo ("<td class='$class' align='center' valign='middle'>&nbsp;". getTermName ($row["term_id"]) .", ". $row["year"] ."&nbsp;</td>");
                }
                echo ("<td class='$class' align='center'>&nbsp;" . $row["applied_date"] . "&nbsp;</td>");
                echo ("<td class='$class' align='center'>&nbsp;" . getStudentStatusGif($row["job_id"]) . "&nbsp;</td>");
                echo ("<td class='$class' align='center'>&nbsp;" . getStudentOwnStatusGif($row["job_id"], $student_number) . "&nbsp;</td>");
            echo ("</tr>");

            $rowclass++;
        }// while loop

        echo ("</table>");
        echo ("</td>");
    echo ("</tr>");
    echo ("</table>");
} //if there are some return 


if($order_by)
{
    if($order_by == "applied_date")
    {
        $select_sql = "SELECT DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,  ec.company_name. a.application_id as application_id, a.job_id as job_id,
        aps.app_status_description, j.job_code as job_code, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,
        j.student_status as job_status, h.history_id, ec.company_name, j.term_id, j.year";
    }
    elseif($order_by == "term_year"  || $order_by == "app_count")
    {
        $select_sql = "SELECT CONCAT(j.term_id, ', ', j.year) term_year, ec.company_name, a.application_id as application_id, a.job_id as job_id,
        aps.app_status_description, j.job_code as job_code, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,
        j.student_status as job_status, h.history_id, ec.company_name, j.term_id, j.year";
    }
    else
    {
        $select_sql = "SELECT $order_by, ec.company_name, a.application_id as application_id, a.job_id as job_id, 
            aps.app_status_description, j.job_code as job_code, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,
        j.student_status as job_status, h.history_id, ec.company_name, j.term_id, j.year";
    }
}
else
{
    $select_sql = "SELECT ec.company_name, a.application_id as application_id, a.job_id as job_id, aps.app_status_description,
        j.job_code as job_code, DATE_FORMAT(a.applied_date, '%b %d, %Y %H:%i') as applied_date,
        j.student_status as job_status, h.history_id, ec.company_name, j.term_id, j.year";
}
$from_sql = "  FROM applications as a, job_info as j, application_status as aps
    LEFT JOIN history AS h ON (h.job_id=j.job_id AND h.student_number='" . $student_number . "')
    LEFT JOIN employer_company AS ec ON ec.employer_id = j.employer_id
    WHERE a.student_number='$student_number'
    AND j.job_id=a.job_id
    AND a.app_status = aps.app_status_id
    AND j.apply_method='".JOB_APP_METHOD_SPECIAL."'
    ". ($show_term != 'ALL' ? "AND j.term_id = '$show_term' ": "") ."
    ". ($show_year != 'ALL' ? "AND j.year = '$show_year' ": "") ."
    ";
    

// We don't want these applications to be ordered by the number of applicants per job (only for online applications)
if($order_by)
{
    if(($lastorder == $order_by) && ($data_flow == "ASC") && ($order_by != "app_count"))
    {
        $data_flow = "DESC";
    }
    elseif(($lastorder == $order_by) && ($data_flow == "DESC") && ($order_by != "app_count"))
    {
        $data_flow = "ASC";
    }
    elseif(!$data_flow && ($order_by != "app_count"))
    {
        $data_flow = "DESC";
    }
    
    if($order_by == "applied_date" )
    {
        $order_by = "a.applied_date";
    }
    if($order_by == "j.job_code")
    {
        $order_sql = "ORDER BY $order_by $data_flow ";
    }
    elseif($order_by == "term_year" || $order_by == "app_count")
    {
        $order_sql = "ORDER BY j.year $data_flow, j.term_id $data_flow";
    }
    else
    {
        $order_sql = "ORDER BY $order_by $data_flow, j.job_code ";
    }

}
else
{
    $order_sql = "ORDER BY ec.company_name";
    $order_by = "ec.company_name";
    $data_flow = "ASC";
}

$sql = $select_sql." ".$from_sql." ".$order_sql;

$result = $GLOBALS['dbh']->Execute( $sql );

if ($result->RecordCount() <= 0) {
    
    echo ("<h4>Special Applications:</h4>");
    notify("You do not have any applications for the chosen term / year.");
}
else {

    echo ("<h4>Special Applications (".$result->RecordCount()."):</h4>");

    echo ("<table cellspacing='0' cellpadding='0' border='1'>");
    echo ("<tr>");
        echo ("<td>");

        echo ("<table border='0' cellpadding='2'>");
        echo ("<tr>");
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"ec.company_name\")'>
                    <b class='white'>&nbsp;Company&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Job Code&nbsp;</b></td>");
            if ($show_term == 'ALL' || $show_year == 'ALL') {

                echo ("<td class='rowgrey' align='center'>
                        <a class='orderable' href='javascript:load_order(\"term_year\")'>
                        <b class='white'>&nbsp;Term, Year&nbsp;</b></a></td>");
            }
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"applied_date\")'>
                    <b class='white'>&nbsp;Last Modified&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'>
                    <a class='orderable' href='javascript:load_order(\"j.status\")'>
                    <b class='white'>&nbsp;Job Status&nbsp;</b></a></td>");
            echo ("<td class='rowgrey' align='center'><b class='white'>&nbsp;Your Status&nbsp;</b></td>");
        echo ("</tr>");

        $rowclass = 0;

        while ($row = $result->FetchRow()) {

            $rowclass % 2 ? $class = "row0d" : $class = "row1d";

            $jobhref = $PHP_SELF . "&amp;select=view_job&amp;level1=job_id&amp;job_id=" . $row["job_id"] . "&amp;show_jobs=preferences&amp;show_future=";

            echo ("<tr>");
                echo ("<td class='$class' align='center'> &nbsp;<a href='$jobhref' class='blue'>" . $row["company_name"] . "</a>&nbsp;</td>");
                echo ("<td class='$class' align='center'> &nbsp;<a href='$jobhref' class='blue'>" . $row["job_code"] . "</a>&nbsp;</td>");
                if ($show_term == 'ALL' || $show_year == 'ALL') {

                    echo ("<td class='$class' align='center' valign='middle'>&nbsp;". getTermName ($row["term_id"]) .", ". $row["year"] ."&nbsp;</td>");
                }
                echo ("<td class='$class' align='center'> &nbsp;" . $row["applied_date"] . " &nbsp;</td>");
                echo ("<td class='$class' align='center'> &nbsp;" . getStudentStatusGif($row["job_id"]) . "&nbsp;</td>");
                echo ("<td class='$class' align='center'> &nbsp;" . getStudentOwnStatusGif($row["job_id"], $student_number) . "&nbsp;</td>");
            echo ("</tr>");
            $rowclass++;
        }// while loop

        echo ("</table>");
        echo ("</td>");
    echo ("</tr>");
    echo ("</table>");
} //if there are some return 


// now display the status key
include ('applications/student/status_key.inc');
        echo ("<table>");
        echo ("<tr>");
    echo ("<td>");

echo ("<form action='$PHP_SELF' method='post' name='form_order'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("\n<input type='hidden' name='order_by' value='' />");
echo ("\n<input type='hidden' name='PDF' value='1' />");
echo ("\n<input type='hidden' name='lastorder' value='$order_by' />");
echo ("\n<input type='hidden' name='data_flow' value='$data_flow' />");
echo ("\n<input type='hidden' name='student_number' value='".$student_number."' />");
echo ("\n<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
echo ("\n<input type='hidden' name='job_page' value='". $job_page ."' />");
echo ("\n<input type='hidden' name='show_term' value='". $show_term ."' />");
echo ("\n<input type='hidden' name='show_year' value='". $show_year ."' />");
echo ("\n</form>");
echo ("</td>");
        echo ("</tr>");
    echo ("</table>");

?>
