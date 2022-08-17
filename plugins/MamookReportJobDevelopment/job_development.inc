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
 | Filename: job_development.inc                                                |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+
*/


/* 
 Filename:  job_development.inc
 Description:  Generates a pdf batch report of contacts
 
 Variables coming in from the form:
    $department:  either a department_id, or a comma delimited list of all department_id's
    $contact_id_string:  string of contact_id's
    $show_placements:  integer that tells us the maximum number of placements to show per contact, SHOW_ALL (-1) for unlimited
    $min_date:  lower bound date
    $max_date:  upper bound
    $order:  what to order our report by (contact name, company name)
    $show_contact_notes:  whether or not to show contact notes

 Notes:
    -Several sql queries are done one at a time at the top of the script with their results dumped to
    arrays for speed reasons.  This has been tested not to be too heavy on resources.
    -This script will time out on large inputs.  TODO: move more queries outside the main loop to gain more speed.
*/

echo('</td><td align="left"><img src="misc/images'.$_SESSION['SESSION_colorscheme'].'/topbar_right.jpg" alt="" /></td></tr></table>');
echo('<h3>Employer Information - Contact Report</h3>');

// form a sql statement fragment for ordering based on what the form brings back
switch ($order) {
case 'contact_name':
    $order = 'c.last_name, c.first_name, ec.company_name';
    break;

case 'company_name':
    $order = 'ec.company_name, c.last_name, c.first_name';
    break;
}

$contact_id_string = unpackObject($contact_id_string);
$contact_id_array  = explode(',', $contact_id_string);

// make sure our inputs are valid 
if ($show_placements == null) {
    define(SHOW_ALL, -1);
    $show_placements = SHOW_ALL;
} elseif (preg_match('/\D/',$show_placements)) {
    error('Please specify a number for the maximum number of placements shown.');
    return;
}

// if max_date is empty, make it default to the present day
if (trim($max_date) == '') {
    $lt = localtime();

    // form a date string, adjusting values from localtime's format
    $max_date = ($lt[5] + 1900).'-'.($lt[4] + 1).'-'.$lt[3];
}

// validate the dates
$time_min = validDate(trim($min_date));
$time_max = validDate(trim($max_date));
if ($time_min == -1 || $time_max == -1) {
    error('Invalid date format.  Please use one of the follwing: yyyy-mm-dd, yyyy-mm-d, yyyy-m-dd, yyyy-m-d, 
     yy-mm-dd, yy-mm-d, yy-m-dd, yy-m-d, mm-dd, mm-d, m-dd, m-d,
     yyyy/mm/dd, yyyy/mm/d, yyyy/m/dd, yyyy/m/d, yy/mm/dd, yy/mm/d, yy/m/dd, yy/m/d, mm/dd, mm/d, m/dd, m/d,
     yyyymmdd, yymmdd, mmdd');
    return;
}

// make the dates friendly for CONCAT(h.year, t.year_order) used in term comparisons for rows in the history table
$sql = 'SELECT t.year_order,t.start_date
        FROM term t
        WHERE '.substr(trim($time_min), 5, 2).' BETWEEN t.start_date AND t.end_date';
$year_result = $GLOBALS['dbh']->Execute($sql);
$row         = $year_result->FetchRow();

$term_min = substr(trim($time_min), 0, 4).$row['start_date'];
$time_min = substr(trim($time_min), 0, 4).$row['year_order'];

$sql = 'SELECT t.year_order, t.end_date
        FROM term t
        WHERE '.substr(trim($time_max), 5, 2).' BETWEEN t.start_date AND t.end_date';
$year_result = $GLOBALS['dbh']->Execute($sql);
$row         = $year_result->FetchRow();

$term_max = substr(trim($time_max), 0, 4).$row['end_date'];
$time_max = substr(trim($time_max), 0, 4).$row['year_order'];


// more error handling--see if the specified time period is valid.  maybe we should change this later so we accept forward and backwards-specified time periods?
if ($time_min > $time_max) {
    error('The specified "From" date is later than the "To" date.  Please choose a valid time period.');
    return;
}

// DEBUG
//echo $contact_id_string;
$sql = "SELECT DISTINCT c.contact_id, CONCAT(c.first_name, ' ', c.last_name) AS name, c.street_address_1, c.street_address_2, c.street_address_3, c.city, c.email, c.phone, c.cellphone, c.fax, c.postal_code
        , pl.abbreviation
        , cl.country_name
        , ce.department_id, ce.employer_id, ce.position
        , ed.department_name
        , ec.company_name
        FROM contact c
        LEFT JOIN provstate_list pl
          ON c.provstate_id = pl.provstate_id
        LEFT JOIN country_list cl
          ON c.country_id = cl.country_id
        INNER JOIN contact_employer ce
          ON c.contact_id = ce.contact_id
        INNER JOIN employer_department ed
          ON ce.department_id = ed.department_id
        INNER JOIN employer_company ec
          ON ce.employer_id = ec.employer_id
        LEFT JOIN history h
          ON ce.employer_id = h.employer_id AND ce.department_id = h.company_department_id
        LEFT JOIN term t
          ON h.term_id = t.term_id
        WHERE c.contact_id IN ($contact_id_string)
        ORDER BY $order";
$result = $GLOBALS['dbh']->Execute($sql);

// DEBUG
//echo '</td><td align="left"><img src="misc/images'.$_SESSION['SESSION_colorscheme'].'/topbar_right.jpg" alt="" /></td></tr></table>';
//echo $sql;

if ($result->RecordCount() < 1) {
    notify('No contacts returned');
    return;
}

// number of placements in time period per contact_id
$sql = "SELECT c.contact_id, COUNT(h.history_id) AS num_placements
        FROM contact c
        INNER JOIN contact_employer ce
          ON c.contact_id = ce.contact_id
        INNER JOIN history h
          ON ce.department_id = h.company_department_id AND ce.employer_id = h.employer_id
        INNER JOIN term t
          ON h.term_id = t.term_id
        WHERE c.contact_id IN ($contact_id_string)
          AND h.department_id IN ($department)
          AND CONCAT(h.year, t.year_order) BETWEEN $time_min AND $time_max
        GROUP BY c.contact_id";
$num_placements_results = $GLOBALS['dbh']->Execute(addslashes($sql));
// store the results in an associative array for quick access later
$num_placements = array();
while ($r = $num_placements_results->FetchRow()) {
    $num_placements[$r['contact_id']] = $r['num_placements'];
}

// information we need to fill out the placements tables
if ($show_placements) {
    $sql = "SELECT c.contact_id
            , CONCAT(t.term_name, ' ', h.year) AS placement_date, h.supervisor_name
            , d.department_code
            , CONCAT(s.first_name, ' ', s.last_name) AS student_name
            FROM contact c
            INNER JOIN contact_employer ce
              ON c.contact_id = ce.contact_id
            INNER JOIN history h
              ON ce.department_id = h.company_department_id AND ce.employer_id = h.employer_id
            INNER JOIN term t
              ON h.term_id = t.term_id
            LEFT JOIN student_department sd
              ON h.department_id = sd.department_id AND h.student_number = sd.student_number
            LEFT JOIN department d
              ON sd.department_id = d.department_id
            LEFT JOIN student s
              ON sd.student_number = s.student_number
            WHERE c.contact_id IN ($contact_id_string)
              AND h.department_id IN ($department)
              AND CONCAT(h.year, t.year_order) BETWEEN $time_min AND $time_max
            ORDER BY CONCAT(h.year, t.year_order) DESC, d.department_code, s.last_name, s.first_name";
    $placement_result = $GLOBALS['dbh']->Execute($sql);

    // dump to 2d array
    while ($r = $placement_result->FetchRow()) {
        $placements[$r['contact_id']][] = array('placement_date' => $r['placement_date'],
                                                'supervisor_name' => $r['supervisor_name'],
                                                'department_code' => $r['department_code'],
                                                'student_name' => $r['student_name']);
    }
}

// htmldoc commands
$export_string = '<!-- MEDIA LEFT 21mm
                       MEDIA RIGHT 21mm
                       MEDIA BOTTOM 24mm
                       MEDIA TOP 24mm
                       FOOTER LEFT "Report generated by '.BRAND_NAME.' on $DATE"
                       FOOTER RIGHT "$PAGE of $PAGES" -->';

$export_string .= '<h3>Job Development Contact Report: '.validDate($min_date).' to '.validDate($max_date).'</h3>';

// begin building the grand table
$export_string .= '<table cellspacing="0" cellpadding="0" border="1" width="100%">';

for($i = 0; $row = $result->FetchRow(); $i++) {
    // figure out how long each contact record will be, and issue a htmldoc command to break to a new page if that many lines aren't available
    if ($show_placements == SHOW_ALL) {
        $max_lines = $num_placements[$row['contact_id']];
    } else {
        $max_lines = $show_placements;
    }
    $export_string .= '<!-- NEED '.($show_placements ? 10 + $max_lines : '7').' -->';

    // begin row by row outputting of each contact and their info
    $export_string .= '<tr class="'.(($i % 2)?"row1d":"row0d").'"><td>';
    $export_string .= '<table cellpadding="2" border="0" width="100%">';
    $export_string .= '<tr>';
        // contact name in bold
        $export_string .= '<td width="28%">';
        $export_string .= '<b>'.$row['name'].'</b>';
        $export_string .= '</td>';
        // company name
        // define widths here for optimal pdf output
        $export_string .= '<td width="30%">'.$row['company_name'].'</td>';
        // date of last action
        $sql = "SELECT cas.action_on
                    , at.action_name
                    , CONCAT(c.first_name, ' ', c.last_name) AS contact_name
                FROM contact_actions_set cas
                INNER JOIN action_types at
                  ON cas.action_id = at.action_id
                INNER JOIN contact c
                  ON cas.action_by = c.contact_id
                WHERE cas.contact_id = $row[contact_id]
                AND cas.action_id != '".GENERIC_NOTE."'
                ORDER BY cas.action_on DESC
                LIMIT 0,1";
        $action_result = $GLOBALS['dbh']->Execute($sql);
        $action        = $action_result->FetchRow();
        $export_string .= '<td width="25%">Date of last action:</td>';
        $export_string .= '<td width="17%">'.$action['action_on'].'</td>';
    $export_string .= '</tr>';

    $export_string .= '<tr>';
        // contact title
        $export_string .= '<td>'.$row['position'].'</td>';
        // division name
        $export_string .= '<td>'.($row['department_name'] == $row['company_name'] ? "" : $row['department_name']).'</td>';
        // type of last action
        $export_string .= '<td>Type of last action:</td>';
        $export_string .= '<td>'.$action['action_name'].'</td>';
    $export_string .= '</tr>';

    $export_string .= '<tr>';
        // contact email
        $export_string .= '<td>'.$row['email'].'</td>';
        // address
        // we have to find the most relevant address, searching contact, division, and company in succession
        if ($row['street_address_1'] == null) {
            $sql = "SELECT ed.street_address_1, ed.street_address_2, ed.street_address_3, ed.city, ed.postal_code
                    , pl.abbreviation
                    , cl.country_name
                    FROM employer_department ed
                    LEFT JOIN provstate_list pl
                      ON ed.provstate_id = pl.provstate_id
                    LEFT JOIN country_list cl
                      ON ed.country_id = cl.country_id
                    WHERE ed.department_id = $row[department_id] AND ed.employer_id = $row[employer_id]";
            $address_result = $GLOBALS['dbh']->Execute($sql);
            $address_array  = $address_result->FetchRow();

            // see if we have to go higher and get the company's address
            if ($address_array['street_address_1'] == null) {
                $sql = "SELECT ec.street_address_1, ec.street_address_2, ec.street_address_3, ec.city, ec.postal_code
                        , pl.abbreviation
                        , cl.country_name
                        FROM employer_company ec
                        LEFT JOIN provstate_list pl
                          ON ec.provstate_id = pl.provstate_id
                        LEFT JOIN country_list cl
                          ON ec.country_id = cl.country_id
                        WHERE ec.employer_id = $row[employer_id]";
                $address_result = $GLOBALS['dbh']->Execute($sql);
                $address_array  = $address_result->FetchRow();
            }

            // build the address (check for nulls--it's still possible at this point
            $address  = ($address_array['street_address_1'] != null ? $address_array['street_address_1'].'<br />' : '');
            $address .= ($address_array['street_address_2'] != null ? $address_array['street_address_2'].'<br />' : '');
            $address .= ($address_array['street_address_3'] != null ? $address_array['street_address_3'].'<br />' : '');
            $address .= ($address_array['city'] != null ? $address_array['city'].', ' : '');
            $address .= ($address_array['abbreviation'] != null ? $address_array['abbreviation'] : '');
            $address .= ($address_array['postal_code'] != null ? '&nbsp;&nbsp;'.$address_array['postal_code'] : '');
            $address .= ($address_array['country_name'] != null ? '<br />'.$address_array['country_name'] : '');
        } else {
            // build the address
            $address = ($row['street_address_1'] != null ? $row['street_address_1'].'<br />' : '');
            $address .= ($row['street_address_2'] != null ? $row['street_address_2'].'<br />' : '');
            $address .= ($row['street_address_3'] != null ? $row['street_address_3'].'<br />' : '');
            $address .= ($row['city'] != null ? $row['city'].', ' : '');
            $address .= ($row['abbreviation'] != null ? $row['abbreviation'] : '');
            $address .= ($row['postal_code'] != null ? '&nbsp;&nbsp;'.$row['postal_code'] : '');
            $address .= ($row['country_name'] != null ? '<br />'.$row['country_name'] : '');
        }
        $export_string .= '<td rowspan="3">'.$address.'</td>';
        $export_string .= '<td>Last action done by:</td>';
        $export_string .= '<td>'.$action['contact_name'].'</td>';
    $export_string .= '</tr>';

    $export_string .= '<tr>';
        // contact phone
        // get the phone number from company department if the contact has designated either to be their phone number 
        switch (trim($row['phone'])) {
        case 'Use Department':
            $sql = "SELECT ed.phone
                    FROM employer_department ed
                    WHERE ed.department_id = $row[department_id] AND ed.employer_id = $row[employer_id]";
            $phone = $GLOBALS['dbh']->GetOne($sql);

            // in case the department has specified "Use Company, we will want to fall through to the next case
            if (trim($phone) != 'Use Company') {
                break;
            }

        case 'Use Company':
            $sql = "SELECT ec.phone
                    FROM employer_company ec
                    WHERE ec.employer_id = $row[employer_id]";
            $phone = $GLOBALS['dbh']->GetOne($sql);
            break;

        default:
            $phone = $row['phone'];
        }
        $export_string .= '<td>'.($phone ? 'Ph: ' : '').$phone.'</td>';
        // number of postings in this time period
        // form dates from the upper and lower bounds of a term, from the term_min and max dates we built above
        $date_min = date('Y-m-d', mktime(0,0,0,substr($term_min,4,2),1,substr($term_min,0,4)));
        $date_max = date('Y-m-d', mktime(0,0,0,substr($term_max,4,2)+1,0,substr($term_max,0,4)));

        // date of last posting
        $sql = "SELECT DATE_FORMAT(ji.closing_date, '%b %Y')
                FROM job_info ji
                WHERE ji.employer_department_id = $row[department_id] AND ji.employer_id = $row[employer_id]
                  AND ji.status <> ".INT_ONLY.'
                ORDER BY ji.closing_date DESC';
        $posting_date = $GLOBALS['dbh']->GetOne($sql);
        $export_string .= '<td>Date of last posting:</td>';
        $export_string .= '<td>'.$posting_date.'</td>';
    $export_string .= '</tr>';

    $export_string .= '<tr>';
        // contact fax
        // get the appropriate fax number, as we do for phone numbers
        switch (trim($row['fax'])) {
            case 'Use Department':
                $sql = "SELECT ed.fax
                        FROM employer_department ed
                        WHERE ed.department_id = $row[department_id] AND ed.employer_id = $row[employer_id]";
                $fax = $GLOBALS['dbh']->GetOne($sql);
                
                // in case the department has specified "Use Company, we will want to fall through to the next case
                if (trim($fax) != 'Use Company') {
                    break;
                }

            case 'Use Company':
                $sql = "SELECT ec.fax
                        FROM employer_company ec
                        WHERE ec.employer_id = $row[employer_id]";
                $fax = $GLOBALS['dbh']->GetOne($sql);
                break;

            default:
                $fax = $row['fax'];
        }
        $export_string .= '<td>'.($fax ? 'Fax: ' : '').$fax.'</td>';
        // number of postings in this time period
        $sql = "SELECT COUNT(ji.job_id)
                FROM job_info ji
                WHERE ji.employer_department_id = $row[department_id] AND ji.employer_id = $row[employer_id]
                  AND ji.closing_date BETWEEN '$date_min' AND '$date_max'
                  AND ji.status <> ".INT_ONLY.'
                ORDER BY ji.closing_date DESC';
        $posting_num = $GLOBALS['dbh']->GetOne($sql);
        $export_string .= '<td># of postings in time period:</td>';
        $export_string .= '<td>'.$posting_num.'</td>';
    $export_string .= '</tr>';

    // extra row for # of placements in period
    $export_string .= '<tr>';

    if(trim($row['cellphone']))
    {
        $cellphone = $row['cellphone'];
        $export_string .= '<td>'.($phone ? 'Cell: ' : '').$cellphone.'</td>';
        $export_string .= '<td></td>';
    }
    else
    {
        $export_string .= '<td></td><td></td>';
    } 
    $export_string .= '<td># of placements in time period:</td>';
    $export_string .= '<td>'.$num_placements[$row['contact_id']].'</td>';
    $export_string .= '</tr>';

    // table of job placements
    //if ($placement_result->RecordCount() > 0 && $show_placements) {
    if (sizeof($placements[$row['contact_id']]) > 0 && $show_placements) {
        $export_string .= '<tr>';
            $export_string .= '<td colspan="4" halign="center">';
            $export_string .= '<table cellspacing="0" cellpadding="1" border="1" width="100%">';
                $export_string .= '<caption><b>Placements:</b></caption>';
                $export_string .= '<tr class="rowgrey">';
                $export_string .= '<td align="center"><b class="white">&nbsp;Term&nbsp;</b></td>';
                $export_string .= '<td align="center"><b class="white">&nbsp;Student Name&nbsp;</b></td>';
                $export_string .= '<td align="center"><b class="white">&nbsp;Department&nbsp;</b></td>';
                $export_string .= '<td align="center"><b class="white">&nbsp;Supervisor&nbsp;</b></td>';
                $export_string .= '</tr>';

            $i = 0;
            foreach ($placements[$row['contact_id']] as $placement)
            {
                if ($i >= $show_placements && $show_placements != SHOW_ALL) {
                    break;
                }

                $export_string .= '<tr>';
                    $export_string .= '<td align="center">&nbsp;'.$placement['placement_date'].'&nbsp;</td>';
                    $export_string .= '<td align="center">&nbsp;'.$placement['student_name'].'&nbsp;</td>';
                    $export_string .= '<td align="center">&nbsp;'.$placement['department_code'].'&nbsp;</td>';
                    $export_string .= '<td align="center">&nbsp;'.$placement['supervisor_name'].'&nbsp;</td>';
                $export_string .= '</tr>';

                $i++;
            }
            $export_string .= '</table>';
            $export_string .= '</td>';
        $export_string .= '</tr>';
    }
    // contact notes
    if ($show_contact_notes) {
        $sql = "SELECT cas.action_note, cas.action_on
                    , CONCAT(c.first_name, ' ', c.last_name) AS advisor_name
                FROM contact_actions_set AS cas
                LEFT JOIN contact AS c
                  ON cas.action_by = c.contact_id
                WHERE cas.contact_id = '".$row[contact_id]."'
                AND cas.action_id = '".GENERIC_NOTE."'
                ORDER BY cas.action_on DESC";
        $notes_results = $GLOBALS['dbh']->Execute($sql);

        if ($notes_results->RecordCount()) {
            $export_string .= '<tr><td><b>Notes:</b></td></tr>';
        
            while ($notes_row = $notes_results->FetchRow()) {
                $export_string .= '<tr>';
                    $export_string .= '<td align="right" valign="top">'.$notes_row['advisor_name']
                                       .'&nbsp;&nbsp;&nbsp;&nbsp;'.$notes_row['action_on'].'&nbsp;&nbsp;&nbsp;</td>';
                    $export_string .= '<td colspan="3">-'.strip_tags(htmlentities($notes_row['action_note'])).'</td>';
                $export_string .= '</tr>';
            }
        }
    }

    $export_string .= '</table>';
    $export_string .= '</td></tr>';
}

$export_string .= '</table>';

// create the object and make it create the html file containing our export_string
include("PDF/PDFconversion/ConvertToPDF.class");
$convert = new ConvertToPDF();
$convert->create_input_file($export_string);

// Print some messages to the screen and give them the export to PDF button
$html .= '<table cellspacing="0" cellpadding="2" border="0">';
    $html .= '<tr class="row1">';
        $html .= '<td>';
        $html .= sizeof($contact_id_array).(sizeof($contact_id_array) == 1 ? ' contact' : ' contacts').' returned.  Report ready.<br />';
        $html .='</td>';
    $html .= '</tr>';
    $html .= '<tr class="row1">';
        $html .= '<td>';
            $html .= '<hr />';
        $html .= '</td>';
    $html .= '</tr>';
    $html .= '<tr class="row1">';
        $html .= '<td align="center">';
        $html .= '<form action="'.$PHP_SELF.'" method="post" name="myform">';
            $html .= '<input type="submit" name="viewpdf" value=" Export to PDF " /></td></tr>';
            $html .= '<input type="hidden" name="select" value="reportOutput" />';
            //$html .= '<input type="hidden" name="reportString" value="', urlencode($export_string), '" />';
            $html .= '<input type="hidden" name="convertObject" value="' . packObject($convert).'" />';
            $html .= '<input type="hidden" name="PDF" value="1" />';
        $html .= '</form>';
        $html .= '</td>';
    $html .= '</tr>';
$html .= '</table>';

echo $html;

// DEBUG
//echo $export_string;
?>
