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
 | Filename: job_descriptions_summary_report.inc                                |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


/*  Filename:  job_descriptions_summary_report.inc
    Purpose:  Generates a report with job postings listing all the students that have applied along with other useful info

    Variables coming in from the form:
        $job_id_list:  comma-delimited string of the job_id's that have come back from a job descriptions search
        $employer_id_list:  comma-delimited string of the employer_id's that have come back from a company search
        $division_id_list:  comma-delimited string of company division id's  that have come back from a division search
        $origin:  origin of where we're calling the report.  this will determine what the sql query is
        $order:  order that we want the report to appear in
        $start_term:  start of term range to use to limit job postings by the terms they're posted for
        $end_term:  upper bound for the above
        $min_date:  date_posted limiting for job postings
        $max_date:  upper bound for the above 
        $show_applied:  boolean for showing a table of students who applied to jobs
        $show_cancelled_jobs:  boolean for showing cancelled jobs
 */

echo '</td><td align="left"><img src="misc/images', $_SESSION['SESSION_colorscheme'], '/topbar_right.jpg" alt="" /></td></tr></table>';

//DEBUG TIMING
//echo date('H:i:s'), '<br />';

switch ($origin) {
default:
case 'job_descriptions_search':
case 'job_descriptions_search_all':
    $job_id_list = urldecode($job_id_list);
    break;
case 'employer_company_search':
    $employer_id_list = urldecode($employer_id_list);
    break;
case 'employer_division_search':
    $division_id_list = urldecode($division_id_list);
    break;
}

// begin input validation
// if max_date is empty, make it default to the present day
if (trim($max_date) == '') {
    $lt = localtime();

    // form a date string, adjusting values from localtime's format
    $max_date = ($lt[5] + 1900).'-'.($lt[4] + 1).'-'.$lt[3];
}

// ensure that either a date_posted or a term-year range was set
if ($origin != job_descriptions_search && !$min_date && (!$start_term || !$start_year || !$end_term || !$end_year)) {
    error('Please specify a range of dates for job postings or a range of terms');
    return;
}

// ensure that both date and term limiting aren't set at the same time
if ($min_date && ($start_term || $start_year || $end_term || $end_year)) {
    error('Please define either a range of dates or terms, not both.');
    return;
}

// validate the dates
if ($min_date) {
    $time_min = validDate(trim($min_date));
    $time_max = validDate(trim($max_date));
    if ($time_min == -1 || $time_max == -1) {
        error('Invalid date format.  Please use one of the follwing: yyyy-mm-dd, yyyy-mm-d, yyyy-m-dd, yyyy-m-d,
                yy-mm-dd, yy-mm-d, yy-m-dd, yy-m-d, mm-dd, mm-d, m-dd, m-d,
                yyyy/mm/dd, yyyy/mm/d, yyyy/m/dd, yyyy/m/d, yy/mm/dd, yy/mm/d, yy/m/dd, yy/m/d, mm/dd, mm/d, m/dd, m/d,
                yyyymmdd, yymmdd, mmdd');
        return;
    }
}

// Main sql query.  We use IF() here to get the correct address.  If there is no division address, use the company address.
// We also use it to get the correct set of values for interviews, depending on whether it's a multi-job interview or not.
// This is amalgamated in a single query to avoid multiple calls to the database.
$sql = "SELECT ji.job_id, ji.job_code, ji.position_title, ji.num_positions, ji.date_posted
            , CONCAT(ji.year, t.year_order) AS term
            , d.department_code
            , astatus.status_id, astatus.status_name
            , s.student_number, CONCAT(s.first_name, ' ', s.last_name) AS student_name
            , h.history_id
            , CONCAT(c.first_name, ' ', c.last_name) AS contact_name
            , ce.position
            , ed.department_name
            , ec.company_name
            , IF(c.phone = 'Use Department', 
                 IF(ed.phone = 'Use Company',
                    ec.phone,
                    ed.phone),
                 IF(c.phone = 'Use Company',
                    ec.phone,
                    c.phone)) AS phone
            , IF(ed.phone = 'Use Company', ec.phone, ed.phone) AS ed_phone
            , IF(ed.fax = 'Use Company', ec.fax, ed.fax) AS fax
            , IF (ed.location_info = '".USE_COMPANY."'
                 , ec.street_address_1
                 , ed.street_address_1) street_address_1
            , IF (ed.location_info = '".USE_COMPANY."'
                 , ec.street_address_2
                 , ed.street_address_2) street_address_2
            , IF (ed.location_info = '".USE_COMPANY."'
                 , ec.street_address_3
                 , ed.street_address_3) street_address_3
            , IF (ed.location_info = '".USE_COMPANY."'
                 , ec.city
                 , ed.city) AS city
            , IF (ed.location_info = '".USE_COMPANY."'
                 , ec.postal_code
                 , ed.postal_code) AS postal_code
            , IF (ed.location_info = '".USE_COMPANY."'
                 , pl2.abbreviation
                 , pl1.abbreviation) AS abbreviation
            , IF (ed.location_info = '".USE_COMPANY."'
                 , cl2.country_name
                 , cl1.country_name) AS country_name
            , IF(ijj.interview_job_id IS NULL, i1.interview_place_id, i2.interview_place_id) AS interview_place_id
            , IF(ijj.interview_job_id IS NULL, ss1.student_number, ss2.student_number) AS ss_student_number
            , IF(ijj.interview_job_id IS NULL, im1.description, im2.description) AS description
            , IF(ijj.interview_job_id IS NULL, it1.int_date, it2.int_date) AS int_date
            , IF(ijj.interview_job_id IS NULL, ii1.interviewer_name, ii2.interviewer_name) AS interviewer_name
        FROM job_info ji
        LEFT JOIN department d
          ON ji.department_id = d.department_id
        INNER JOIN term t
          ON ji.term_id = t.term_id
        LEFT JOIN admin_status astatus
          ON ji.admin_status = astatus.status_id
        LEFT JOIN job_status js
          ON ji.status = js.status_id
        LEFT JOIN applications a
          ON a.job_id = ji.job_id
        LEFT JOIN student s
          ON a.student_number = s.student_number
        LEFT JOIN history h
          ON a.job_id = h.job_id AND s.student_number = h.student_number
        INNER JOIN contact c
          ON ji.contact_id = c.contact_id
        INNER JOIN contact_employer ce
          ON c.contact_id = ce.contact_id
        LEFT JOIN employer_department ed
          ON ji.employer_department_id = ed.department_id
        LEFT JOIN employer_company ec
          ON ed.employer_id = ec.employer_id
        LEFT JOIN provstate_list pl1
          ON ed.provstate_id = pl1.provstate_id
        LEFT JOIN country_list cl1
          ON ed.country_id = cl1.country_id
        LEFT JOIN provstate_list pl2
          ON ec.provstate_id = pl2.provstate_id
        LEFT JOIN country_list cl2
          ON ec.country_id = cl2.country_id
        LEFT JOIN interview i1
          ON ji.job_id = i1.job_id
        LEFT JOIN students_shortlisted ss1
          ON i1.job_id = ss1.job_id AND a.student_number = ss1.student_number
        LEFT JOIN interview_medium im1
          ON i1.interview_medium_id = im1.interview_medium_id
        LEFT JOIN interview_time it1
          ON i1.job_id = it1.job_id
        LEFT JOIN interview_interviewer ii1
          ON it1.interviewer_id = ii1.interviewer_id
        LEFT JOIN interview_job_join ijj
          ON ji.job_id = ijj.job_id
        LEFT JOIN interview i2
          ON ijj.interview_job_id = i2.job_id
        LEFT JOIN students_shortlisted ss2
          ON i2.job_id = ss2.job_id AND a.student_number = ss2.student_number
        LEFT JOIN interview_medium im2
          ON i2.interview_medium_id = im2.interview_medium_id
        LEFT JOIN interview_time it2
          ON i2.job_id = it2.job_id
        LEFT JOIN interview_interviewer ii2
          ON it2.interviewer_id = ii2.interviewer_id ";

switch($origin) {
default:

case 'job_descriptions_search_all':

case 'job_descriptions_search':
    $sql .= "WHERE ji.job_id IN ($job_id_list) ";
    break;

case 'employer_company_search':
    $sql .= "WHERE ji.employer_id IN ($employer_id_list) ";
    break;

case 'employer_division_search':
    $sql .= "WHERE ji.employer_department_id IN ($division_id_list) ";
    break;
}

// add the date-limiting clauses
if ($min_date) {
    // limit the job descriptions by date added if dates were specified
    $sql .= "AND ((ji.date_posted BETWEEN '$time_min' AND '$time_max') OR (ji.date_posted = '0000-00-00')) ";
} else {
    if ($origin != 'job_descriptions_search') {
        // limit the job descriptions by term if specified
        $start_year_term = $start_year.$start_term;
        $end_year_term   = $end_year.$end_term;

        $sql .= "AND (CONCAT(ji.year, t.year_order) BETWEEN '$start_year_term' AND '$end_year_term') ";
    }
}

// don't show cancelled jobs?
if (!$show_cancelled_jobs) {
    $sql .= 'AND js.status_id <> '.CANCELLED.' ';
}

switch($order) {
default:

case 'job_code':
    $sql .= 'ORDER BY ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;

case 'contact_name':
    $sql .= 'ORDER BY c.last_name, c.first_name, ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;

case 'company_name':
    $sql .= 'ORDER BY ec.company_name, ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;

case 'status':
    $sql .= 'ORDER BY astatus.status_name, ec.company_name, ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;
   
case 'date_posted':
    $sql .= 'ORDER BY ji.date_posted, ec.company_name, ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;

case 'int_date':
    $sql .= 'ORDER BY int_date DESC, ec.company_name, ji.job_code, s.last_name, s.first_name';
    break;

case 'host_dept':
    $sql .= 'ORDER BY d.department_code, ec.company_name, ji.job_code, s.last_name, s.first_name, int_date DESC';
    break;
}

$result = $GLOBALS['dbh']->Execute($sql);
// DEBUG
//echo "<br />$sql<br />";
//return;
// DEBUG TIMING
//echo date('H:i:s'), '<br />';

$num_rows = $result->RecordCount();
// if we have nothing to display, give an error
if ($num_rows == 0) {
    notify('There are no job postings associated with the employer you have chosen.');
    return;
}

// htmldoc commands
$export_string = '<!-- MEDIA LEFT 21mm
                       MEDIA RIGHT 21mm
                       MEDIA BOTTOM 24mm
                       MEDIA TOP 24mm
                       FOOTER LEFT "Report generated by '.BRAND_NAME.' on $DATE"
                       FOOTER RIGHT "$PAGE of $PAGES" -->';

$export_string .= '<h3>Job Posting Report</h3>';
$export_string .= '<table cellspacing="0" cellpadding="0" border="1" width="100%">';

$counter      = 0;          // counter to keep track of row pointer in the sql result table 
$job_id_array = array();    // array to keep track of which job's we've processed

while ($row = $result->FetchRow()) {
    $counter++;

    if (!$job_id_array[$row['job_id']]) {
        // store the current row to remember what job description the current application table belongs to
        $description                  = $row;
        $job_id_array[$row['job_id']] = true;
    }

    // convenience variable for tracking job_id and the current one while we iterate through blocks of applications for each job 
    $job_id         = $row['job_id'];
    $current_job_id = $job_id;

    // variables we will use to keep track of job info.  arrays are used here in case of multiple values.
    $num_applications = 0;
    $applications     = array();
    $interviewer      = array();
    $int_date         = array();
    $int_medium       = array();
    $int_location     = array();
    
    // concurrently build the string for the table that holds the applications
    $applications_table = '<table cellpadding="0" border="0" width="100%">';
    while ($current_job_id == $job_id) {
        // because of the joins we make in the giant sql query, we'll get duplicates here 
        // if there's multiple interview dates, media, etc.
        // we make sure, using the applications array, that the student hasn't been placed in the table previously
        if (!$applications[$row['student_number']]) {
            $applications[$row['student_number']] = true;

            if ($row['student_number']) {
                $num_applications++;
                $applications_table .= '<tr>';
                $applications_table .= '<td>'.$row['student_name'].'</td>';
                
                // determine if the student has been shortlisted
                $applications_table .= '<td>';
                if ($row['ss_student_number']) {
                    $applications_table .= 'S';
                } else {
                    $applications_table .= '&nbsp;';
                }
                $applications_table .= '</td>';

                // determine if the student has been placed for the job
                $applications_table .= '<td>';
                if ($row['history_id']) {
                    $applications_table .= 'M';
                } else {
                    $applications_table .= '&nbsp;';
                }
                $applications_table .= '</td>';

                $applications_table .= '</tr>';
            }
        }
        
        // compile list of interviewers into a truth table
        $field_name = 'interviewer_name';
        if ($row[$field_name] && !$interviewer[$row[$field_name]]) {
            $interviewer[$row[$field_name]] = true;
        }
        // compile list of interview dates
        $field_name = 'int_date';
        if ($row[$field_name] && !$int_date[$row[$field_name]]) {
            $int_date[$row[$field_name]] = true;
        }
        // compile list of interview types
        $field_name = 'description';
        if ($row[$field_name] && !$int_medium[$row[$field_name]]) {
            $int_medium[$row[$field_name]] = true;
        }
        // compile list of interview locations
        $field_name = 'interview_place_id';
        if ($row[$field_name] && !$int_location[$row[$field_name]]) {
            $int_location[$row[$field_name]] = true;
        }
        // fetch the next row.  break if we've reached the end of the table
        if ($row = $result->FetchRow()) {
            $counter++;
        } else {
            break;
        }
        
        // update the current job id
        $current_job_id = $row['job_id'];

        // DEBUG
        //echo "current job id is $current_job_id and job id is $job_id, student number is ".$row['student_number']."<br />";
    }
    // close the applications table
    if ($num_applications == 0) {
        $applications_table .= '<tr><td>&nbsp;</td></tr>';
    }
    $applications_table .= '</table>';

    // DEBUG
    //echo "new block at ".$row['student_number']." and ".$row['job_id']."<br />";

    // roll back so that the next iteration of the loop starts at the beginning of the new
    // block of job applications
    if ($counter < $num_rows) {
        $result->Move(--$counter);
    } else if ($counter == $num_rows && $job_id != $current_job_id) {
        $result->Move($counter - 1);
    }
    $row = $description;

    // DEBUG
    //echo "now we are at ".$row['student_number']." and job id ".$row['job_id']."<br />";

    // begin building the report string
    // figure out how long each table will be, issue a htmldoc command to break to a new page if that many lines aren't available
    //$export_string .= '<!-- NEED '.(10 + $num_applications).' -->';

    //$export_string .= '<tr class="'.(($i % 2)?"row1d":"row0d").'"><td>';
    $export_string .= '<tr><td>';
    $export_string .= '<table cellpadding="3" border="0" width="100%">';

    $export_string .= '<tr>';
    // job code, position title, contact name
    $export_string .= '<td width="34%" valign="top">';
    $export_string .= '<b>'.$row['job_code'].'</b><br />';
    $export_string .= 'Position:  '.$row['position_title'].'<br />';
    $export_string .= 'Sent Resumes to:<br /><b>&nbsp;&nbsp;&nbsp;'.$row['contact_name'].'</b><br />';
    $export_string .= '<b>&nbsp;&nbsp;&nbsp;'.$row['position'].'</b><br />';
    $export_string .= '</td>';

    // build the address
    $address = $row['company_name']."<br />";
    $address .= ($row['department_name'] != $row['company_name'] ? $row['department_name']."<br />" : "");

    $address .= ($row['street_address_1'] != null ? $row['street_address_1']."<br />" : "");
    $address .= ($row['street_address_2'] != null ? $row['street_address_2']."<br />" : "");
    $address .= ($row['street_address_3'] != null ? $row['street_address_3']."<br />" : "");
    $address .= ($row['city'] != null ? $row['city'].", " : "");
    $address .= ($row['abbreviation'] != null ? $row['abbreviation'] : "");
    $address .= ($row['postal_code'] != null ? "&nbsp;&nbsp;".$row['postal_code'] : "");
    $address .= ($row['country_name'] != null ? "<br />".$row['country_name'] : "");

    $export_string .= '<td width="33%" valign="top" colspan="2">';
    $export_string .= $address;
    $export_string .= '</td>';

    // date posted, status, number of positions, interviewer, interview date
    $export_string .= '<td rowspan="3" width="33%" valign="top">';
    $export_string .= 'Date Posted:  '.$row['date_posted'].'<br />';
    $export_string .= 'Status:  '.$row['status_name'].'<br />';
    $export_string .= 'Number of Positions:  '.$row['num_positions'].'<br />';
    // interviewers
    $export_string .= 'Interviewer'.(sizeof($interviewer) > 1 ? 's:' : ':').'<br />';
    foreach ($interviewer as $name => $bool) {
        $export_string .= '&nbsp;&nbsp;&nbsp;'.$name.'<br />';
    }
    // interview dates
    $export_string .= 'Interview Date'.(sizeof($int_dates) > 1 ? 's' : '').':<br />';
    foreach ($int_date as $date => $bool) {
        $export_string .= '&nbsp;&nbsp;&nbsp;'.$date.'<br />';
    }
    // interview types
    $export_string .= 'Interview Type'.(sizeof($int_medium) > 1 ? 's' : '').':<br />';
    foreach ($int_medium as $medium => $bool) {
        $export_string .= '&nbsp;&nbsp;&nbsp;'.$medium.'<br />';
    }
    // get the interview location
    $export_string .= 'Interview Location:  ';

    foreach ($int_location as $location => $bool) {
        if ($location == 1) {
            $export_string .= "On Campus<br />";
        } elseif ($location == 2) {
            if ($row['interview_job_id']) {
                $interview_job_id = $row['interview_job_id'];
            } else {
                $interview_job_id = $row['job_id'];
            }

            $sql = "SELECT DISTINCT il.street_address_1, il.street_address_2, il.city, il.province, il.country
                    FROM interview_location il
                    WHERE il.job_id = ".$interview_job_id."
                    AND il.street_address_1 IS NOT NULL";
            //echo "<br /><br />$sql";

            $location_result = $GLOBALS['dbh']->Execute($sql);

            if ($location_result->RecordCount()) {
                $il = $location_result->FetchRow();

                $export_string .= ($il['street_address_1'] ? "<br />&nbsp;&nbsp;".$il['street_address_1']."<br />" : "");
                $export_string .= ($il['street_address_2'] ? "&nbsp;&nbsp;".$il['street_address_2']."<br />" : "");
                $export_string .= ($il['province'] ? "&nbsp;&nbsp;".$il['city'].", ".$il['province']."<br />" : "");
                $export_string .= ($il['country'] ? "&nbsp;&nbsp;".$il['country']."<br />" : "");
            } else {
                $export_string .= "Off Campus<br />";
            }
        }
    }

    if (sizeof($int_location) == 0) {
        $export_string .= '<br />';
    }

    // host department
    $export_string .= 'Host Department:  '.$row['department_code'].'<br />';
    $export_string .= '</td>';
    $export_string .= '</tr>';

    $export_string .= '<tr>';
    $export_string .= '<td>';
    $export_string .= '&nbsp;&nbsp;&nbsp;'.$row['phone'].'<br /><hr />';
    $export_string .= '</td>';

    // department phone numbers
    $export_string .= ($row['ed_phone'] ? '<td valign="top">Ph:  '.$row['ed_phone'].'</td>' : '<td></td>');
    $export_string .= ($row['fax'] ? '<td valign="top" align="right">Fax:  '.$row['fax'].'</td>' : '<td></td>');
    $export_string .= '</tr>';

    // table of applications associated with the current job
    if ($show_applied)
    {
        $export_string .= '<tr>';
        $export_string .= "<td>$applications_table</td>";

        // empty cell to balance the table and prevent overlap in htmldoc
        $export_string .= '<td colspan="2">&nbsp;</td></tr>';
    }

    // this closes the table that contains each record in the report
    $export_string .= '</table></td></tr>';
}
// copy the number of jobs we displayed to num_jobs, used later in giving the user informational messages
$export_string .= '</table>';

// create the object and make it create the html file containing our export_string
include("PDF/PDFconversion/ConvertToPDF.class");
$convert = new ConvertToPDF();
$convert->create_input_file($export_string);

// DEBUG
//echo $job_id_list, '<br />';
//foreach($job_id_array as $k => $v)
    //echo '\'', $k, '\',';

// Print some messages to the screen and give them the export to PDF button
switch ($origin) {
default:

case 'job_descriptions_search_all':

case 'job_descriptions_search':
    echo '<h3>Job Descriptions - Job Posting Report</h3>';
    break;

case 'employer_company_search':
    echo '<h3>Employer Information - Job Posting Report</h3>';
    break;

case 'employer_division_search':
    echo '<h3>Employer Information - Job Posting Report</h3>';
    break;
}

echo '<table cellspacing="0" cellpadding="2" border="0">';
    echo '<tr class="row1">';
        echo '<td>';
            echo sizeof($job_id_array), (sizeof($job_id_array) == 1 ? ' posting' : ' postings'), ' returned.  Report ready.<br />';
        echo '</td>';
    echo '</tr>';
    echo '<tr class="row1">';
        echo '<td>';
            echo '<hr />';
        echo '</td>';
    echo '</tr>';
    echo '<tr class="row1">';
        echo '<td align="center">';
            echo '<form action="'.$PHP_SELF.'" method=post name="myform">';
            echo '<input type="submit" name="viewpdf" value=" Export to PDF " /></td></tr>';
            echo '<input type="hidden" name="select" value="reportOutput" />';
            echo '<input type="hidden" name="convertObject" value="' . packObject($convert).'" />';
            echo '<input type="hidden" name="PDF" value="1" />';
            echo '</form>';
        echo '</td>';
    echo '</tr>';
echo '</table>';

//ÊDEBUG TIMING
//echo date('H:i:s'), '<br />';

// DEBUG
//echo $export_string;
?>
