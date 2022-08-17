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
 | Filename: student_applications_report.inc                                    |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/

/*  Filename:  student_applications_report.inc
    Purpose:  Generates a report that shows the applications 
              associated with a set of students 
    
    Variables coming in from referring pages:
        $record_id_list    = list of record_id's from a student advanced search 
        $order_by_1        = order by, field 1
        $order_by_2        = order by, field 2
        $show_applications = whether or not we should show applications
        $termsused         = term/years chosen by checkboxes
        $term              = array of selected terms
        $year              = array of selected years

    Note:  - this could and probably should have been written 
           in mvc style so that it becomes at least somewhat more extensible
           in terms of the way the data is represented (excel format, pdf, etc)
           - the algorithm here is convoluted because there doesn't seem to
           be a way of doing multiple ordering by aggregate functions working
           over columns of several different joined tables--at least not
           in a single pass.

    Algorithm:
           1. query once to get summary info about each student and get it
              back in the order that the user specified.
           2. query once to get the number of interviews a student has based
              on time period. 
           3. query again to get all the applications a student has based on 
              a time period.
           4. traverse all the applications and put results into a 3D array 
              keyed by student number.
           5. traverse through arrays of all queries and build HTML output.
           6. convert to pdf.
 */

include('history/container_functions.inc');
echo '</td><td align="left"><img src="misc/images'
     , $_SESSION['SESSION_colorscheme'], '/topbar_right.jpg" alt="" /></td></tr></table>';

// DEBUG TIMING
//echo 'fjg', date('H:i:s'),'<br />';

// determine which terms were selected by comparing the terms checked on
// in termsused and the ones selected in the pulldown menu in term[] and year[]
$num_terms = sizeof($term);
if (sizeof($termsused) > 0) {
    for ($i = 1; $i <= $num_terms; $i++) {
        if (!isset($termsused[$i])) {
            unset($term[$i]);
            unset($year[$i]);
        }
    }
    $terms = implode(',', $term);

    // calculate numeric equivalents of terms
    $numeric_terms = array();   // format: YEAR + term.year_order 
    foreach ($term as $i => $t) {
        switch ($t) {
        case 1:
            $numeric_terms[$i] = $year[$i].'3';
            break;
        case 2:
            $numeric_terms[$i] = $year[$i].'1';
            break;
        case 3:
            $numeric_terms[$i] = $year[$i].'2';
            break;
        }
    }

    // sort the numeric terms in reverse order so that we 
    // will have a way of knowing the latest term chosen.
    // we need this in order to have a valid range from the earliest
    // term to the latest term.
    arsort($numeric_terms);
    $latest_term   = array_shift($numeric_terms);
    array_unshift($numeric_terms, $latest_term);
    $earliest_term = array_pop($numeric_terms);
    array_push($numeric_terms, $earliest_term);

// build a boolean statement for the selected term and year combinations
    $ji_term_boolean = '';
    $h_term_boolean  = '';
    $title_term      = '';
    $j               = 1;
    foreach ($term as $i => $t) {
        $ji_term_boolean .= "(ji.term_id = $t AND ji.year = ".$year[$i].')';
        $h_term_boolean  .= "(h.term_id = $t AND h.year = ".$year[$i].')';
        $title_term .= getTermName($t)." ".$year[$i];

        if (sizeof($term) > 1 && $j < sizeof($term)) {
            $ji_term_boolean .= ' OR ';
            $h_term_boolean  .= ' OR ';
            $title_term      .= ', ';
        }
        $j++;
    }
}

// formulate an ORDER BY clause.
// push all the parameters onto a stack and use implode to form the clause.
// in the future, if more ordering options are added, just push them onto
// the stack.
$order_by_choices = array('order_by_1', 'order_by_2');
$order            = array();
foreach ($order_by_choices as $o) {
    switch ($$o) {
    case 'last_name':
        array_push($order, 's.last_name, s.first_name');
        break;
    case 'discipline':
        array_push($order, 'd.discipline_name');
        break;
    case 'advisor':
        array_push($order, 'c.last_name, c.first_name');
        break;
    case 'apps_count':
        array_push($order, 'apps_count desc');
        break;
    case 'interview_count':
        array_push($order, 'interview_count desc');
        break;
    case '':
        break;
    default:
        array_push($order, $$o);
        break;
    }
}
if (sizeof($order) > 0) {
    $order           = array_unique($order);
    $order_by_clause = 'ORDER BY '.implode(', ', $order);
}

// query once to get a summary of information about each student.
// this will also give us a way to get an ordered list of record_id's
// for use later.
// Note:  -we use COUNT(DISTINCT ...) because of the duplicity of records
//         after joining with history: it's a one to many relationship.

//:TODO: Remove broken apart queries.  apps count is also incorrect for a certain term. base on apply date? 
$sql = "SELECT sd.record_id
            , d.discipline_name
            , sd.discipline_id
            , sd.department_id
            , CONCAT(c.first_name, ' ', c.last_name) AS advisor
            , (MAX(h1.work_term_number) + 1)         AS work_term_number
            , CONCAT(s.first_name, ' ', s.last_name) AS student_name
            , s.student_number
            , COUNT(DISTINCT a2.application_id)      AS apps_count
            , (COUNT(DISTINCT ssa.job_id)+COUNT(DISTINCT ssb.job_id)) AS interview_count
        FROM student_department sd
        LEFT JOIN discipline d
            ON sd.discipline_id = d.discipline_id
        LEFT JOIN contact c
            ON sd.coop_advisor = c.contact_id
        LEFT JOIN history h1
            ON sd.student_number = h1.student_number
            AND sd.department_id = h1.department_id
        LEFT JOIN student s
            ON sd.student_number = s.student_number
        LEFT JOIN applications a
            ON s.student_number = a.student_number
        LEFT JOIN job_info ji
            ON a.job_id = ji.job_id
            ".($ji_term_boolean ? "AND ($ji_term_boolean)" : '')."
        LEFT JOIN applications AS a2
            ON a2.job_id = ji.job_id
            ".($ji_term_boolean ? "AND ($ji_term_boolean)" : '')."
            AND a2.student_number = sd.student_number
        LEFT JOIN interview_job_join ijj
            ON ji.job_id = ijj.job_id
        LEFT JOIN interview i
            ON ijj.interview_job_id = i.job_id
        LEFT JOIN students_shortlisted ssa
            ON i.job_id = ssa.job_id
            AND sd.student_number=ssa.student_number
        LEFT JOIN students_shortlisted ssb
            ON ji.job_id = ssb.job_id
            AND sd.student_number=ssb.student_number
        WHERE sd.record_id IN ($record_id_list)
        GROUP BY sd.student_number
        $order_by_clause";

$result = $GLOBALS['dbh']->Execute($sql);

// DEBUG
//echo $sql,'<br /><br />';

// dump the results to an array and reorder record_list to our custom ordering
$summary_info      = array();
$student_num_array = array();
while ($row = $result->FetchRow()) {
    $summary_info[$row['student_number']] 
        = array('discipline_name'  => $row['discipline_name'],
                'department_id'    => $row['department_id'], 
                'advisor'          => $row['advisor'],
                'work_term_number' => $row['work_term_number'],
                'student_name'     => $row['student_name'],
                'interview_count'  => $row['interview_count']
                );
    $student_num_array[] = $row['student_number'];
}

// Another small query to get own job placements.
$sql = "SELECT sd.student_number
            , h.company_name
        FROM student_department sd
        INNER JOIN history h
          ON sd.student_number = h.student_number
            AND sd.department_id = h.department_id
        WHERE sd.record_id IN ($record_id_list)
          AND h.job_id IS NULL
            ".($h_term_boolean ? "AND $h_term_boolean" : '');
// DEBUG
//echo $sql,'<br /><br />';

$result = $GLOBALS['dbh']->Execute($sql);
while ($row = $result->FetchRow()) {
    $own_jobs[$row[0]][] = $row[1];
}

// Main sql query that checks how many applications a student has. 
$sql = "SELECT sd.record_id
            , s.student_number
            , h.history_id AS placed
            , ji.job_code
            , IF(STRCMP(ec.company_name,ed.department_name)
                , CONCAT(ec.company_name,' [',ed.department_name,']')
                , ec.company_name) AS co_div_name  
            , ss1.student_number AS student_shortlist
            , ss2.student_number AS student_shortlist_multiple
        FROM student_department sd
        INNER JOIN student s
          ON sd.student_number = s.student_number
        LEFT JOIN applications a
          ON s.student_number = a.student_number
        LEFT JOIN history h
          ON sd.student_number = h.student_number 
            AND sd.department_id = h.department_id AND a.job_id = h.job_id
        LEFT JOIN job_info ji
          ON a.job_id = ji.job_id
        LEFT JOIN employer_department ed
          ON ji.employer_department_id = ed.department_id
        LEFT JOIN employer_company ec
          ON ed.employer_id = ec.employer_id
        LEFT JOIN interview i1
          ON ji.job_id = i1.job_id
        LEFT JOIN students_shortlisted ss1
          ON i1.job_id = ss1.job_id AND sd.student_number = ss1.student_number
        LEFT JOIN interview_job_join ijj
          ON ji.job_id = ijj.job_id
        LEFT JOIN interview i2
          ON ijj.interview_job_id = i2.job_id
        LEFT JOIN students_shortlisted ss2
          ON i2.job_id = ss2.job_id AND sd.student_number = ss2.student_number
        WHERE sd.record_id IN ($record_id_list)
            AND ji.status != '".INT_ONLY."' 
            ".($ji_term_boolean ? "AND ($ji_term_boolean)" : '')."
            ORDER BY sd.record_id, ec.company_name";
$result = $GLOBALS['dbh']->Execute($sql);

// DEBUG
//echo $sql,'<br /><br />';

// Take results of the main query and place them into the $applications array. $applications is a three dimensional array. The first field
// is its student number, the second field is the nth application, and the last field contains application attributes like job_code, 
// company/division name, etc.  
$applications = array();

if ($result->RecordCount() > 0)
{
    $current_student_number = NULL;

    while ($row = $result->FetchRow())
    {
        if ($current_student_number != $row["student_number"])
        {
            $i = 0;
        }
        
        // Example of accessing a student's application job code: $applications[0024633][0]['job_code'], $applications[0024633][3]['job_code']
        $applications[$row["student_number"]][$i]["placed"] = $row["placed"];
        $applications[$row["student_number"]][$i]["job_code"] = $row["job_code"];
        $applications[$row["student_number"]][$i]["co_div_name"] = $row["co_div_name"];
        $applications[$row["student_number"]][$i]["student_shortlist"] = $row["student_shortlist"];
        $applications[$row["student_number"]][$i]["student_shortlist_multiple"] = $row["student_shortlist_multiple"];

        $current_student_number = $row["student_number"];
        $i++;
    }
}

// DEBUG TIMING
//echo date('H:i:s'),'<br />';

// DEBUG 
//echo $sql;
//echo '<br />';

// nested table for each student
$report = array();
$rowcount= $result->RecordCount();

// htmldoc commands
$export_string = '<!-- MEDIA LEFT 21mm
MEDIA RIGHT 21mm
MEDIA BOTTOM 24mm
MEDIA TOP 24mm
FOOTER LEFT "Report generated by '.BRAND_NAME.' on $DATE"
FOOTER RIGHT "$PAGE of $PAGES" -->';

$export_string .= '<h3>Student Applications Report - '.($title_term ? $title_term : 'All Terms/Years').'</h3>';
$export_string .= '<table cellspacing="0" cellpadding="2" border="1" width="640">';

$num_applications = 0;

// Note: It is very important that the sum of all column widths add up to the table width. The PDF converter gets fussy if the widths 
//       don't add up, or if you give it a non-absolute number. 
for ($i = 0; $i < sizeof($student_num_array); $i++) 
{
    $current_student_number = $student_num_array[$i];
    
    // row 1: Display student name, number of applications, and work term number
    $export_string .= '<tr>';
    $export_string .= '<td>';
    $export_string .= '<table cellspacing="0" cellpadding="0" border="0" width="100%">';
    $export_string .= '<tr>';
    $export_string .= '<td width="150"><b>'.$summary_info[$current_student_number]['student_name'].'</b></td>';
    $apps_count = sizeof($applications[$current_student_number]);
    if ($apps_count == 1 && !$applications[$current_student_number][0]['job_code'])
    {
        $apps_count = 0;
    }
    $num_applications += $apps_count;
    $export_string .= '<td width="420">'.$apps_count.' '.(($apps_count == 1) ? 'application' : 'applications').'</td>';
    $export_string .= '<td width="30"> </td>';

    $mode = useContainers($current_student_number, $summary_info[$current_student_number]['department_id']);
    if ($mode)
    {
        // use this to get the next work term code for history containers
        $work_term_number = getNextWorktermCode($current_student_number, $summary_info[$current_student_number]['department_id'], $mode);
    }
    else
    {
        $work_term_number = $summary_info[$current_student_number]['work_term_number'];
    }
    
    $export_string .= "<td align='right' width='40'><b>WT".($work_term_number ? $work_term_number : '1')."</b></td>";
    $export_string .= '</tr>';

    // row 2: Display student number, and number of interviews 
    $export_string .= '<tr>';
    $export_string .= '<td><b>'.$current_student_number.'</b></td>';
    $interview_count = $summary_info[$current_student_number]['interview_count'];
    $export_string .= '<td>'.$interview_count.' '.(($interview_count == 1) ? 'interview' : 'interviews').'</td>';
    $export_string .= '<td></td>';
    $export_string .= '<td></td>';
    $export_string .= '</tr>';

    // row 3:  Display discipline and advisor
    $export_string .= '<tr>';
    $export_string .= '<td>'.$summary_info[$current_student_number]['discipline_name'].'</td>';
    $export_string .= '<td>Co-op Advisor: '.$summary_info[$current_student_number]['advisor'].'</td>';
    $export_string .= '</tr>';

    // Show all applications the student has.
    if ($show_applications && ($apps_count || $interview_count || sizeof($own_jobs[$current_student_number]))) 
    {
        $export_string .= '<tr><td colspan="4"><hr /></td></tr>';
        for ($j = 0; $j < sizeof($applications[$current_student_number]); $j++)
        {
            $export_string .= '<tr>';
            $export_string .= '<td valign="top">&nbsp;&nbsp;'.$applications[$current_student_number][$j]['job_code'].'</td>';
            $export_string .= '<td>'.$applications[$current_student_number][$j]['co_div_name'].'</td>';
            // determine if the student has been shortlisted
            $export_string .= "<td valign='top' align='left'>";
            if ($applications[$current_student_number][$j]['student_shortlist']) {
                $export_string .= 'S';
            } elseif ($applications[$current_student_number][$j]['student_shortlist_multiple']) {
                $export_string .= 'S+';
            } else {
                $export_string .= '&nbsp;';
            }
            $export_string .= '</td>';

            // determine if the student has been placed for the job
            $export_string .= "<td valign='top' align='right'>";
            if ($applications[$current_student_number][$j]['placed']) {
                $export_string .= 'M';
            } else {
                $export_string .= '&nbsp;';
            }
            $export_string .= '</td>';
            $export_string .= '</tr>';
        }

        // Display all of the student's own jobs or jobs that have no job code. 
        for ($j = 0; $j < sizeof($own_jobs[$current_student_number]); $j++)
        {
            $export_string .= '<tr>';
            $export_string .= '<td>&nbsp;&nbsp;No Job Code/Own Job</td>';
            // print company name
            $export_string .= '<td>'.$own_jobs[$current_student_number][$j].'</td>';
            $export_string .= "<td align='right'>&nbsp;</td>";
            $export_string .= "<td align='right'>M</td>";
        }
    }
    $export_string .= '</table>';
    $export_string .= '</td>';
    $export_string .= '</tr>';
}
$export_string .= '</table>';

// DEBUG
//echo ("EXPORT=".$export_string);

// create the object and make it create the html file containing our export_string
include("PDF/PDFconversion/ConvertToPDF.class");
$convert = new ConvertToPDF();
$convert->create_input_file($export_string);

// Print some messages to the screen and give them the export to PDF button
echo '<h3>Student Info - Student Applications Report</h3>';
echo '<form action="', $PHP_SELF, '" method=post name="myform">';
echo '<table cellspacing="0" cellpadding="2" border="0">';
echo '<tr class="row1">';
echo '<td>';
$num_students = sizeof($student_num_array);
echo $num_students, ($num_students == 1 ? ' student, ' : ' students, '), $num_applications, ($num_applications == 1 ? ' application' : ' applications'), ' returned.  Report ready.<br />';
echo '</td>';
echo '</tr>';
echo '<tr class="row1">';
echo '<td>';
echo '<hr />';
echo '</td>';
echo '</tr>';
echo '<tr class="row1">';
echo '<td align="center">';
//echo '<form action="', $PHP_SELF, '" method=post name="myform">';
echo '<input type="submit" name="viewpdf" value="Export to PDF" />';
echo '<input type="hidden" name="select" value="reportOutput" />';
echo '<input type="hidden" name="convertObject" value="', packObject($convert), '" />';
echo '<input type="hidden" name="PDF" value="1" />' ;
echo '</td>';
echo '</tr>';
echo '</table>';
echo '</form>';
//echo '</td>';
//echo '</tr>';
//echo '</table>';
// DEBUG TIMING
//echo date('H:i:s'), '<br />';
?>
